/*
 * File:        wm8510.h
 * Author:      Sam Faull
 * Description: Driver for WM8510 CODEC
 */

#include <xc.h>
#include "wm8510.h"
#include "system.h"

/*
 * SPI connections:
 * SCK1 = pin2 (RP22)
 * SDO1 = pin15 (RP15)
 * ADCout = pin11 (RP13)
 * DACin = pin10 (RP12)
 * FRClk = pin9 (RP11)
 * MCLK = pin4 (RP24)
 * bCLK = pin5 (RP25)
 */

void wm8510_init(void)
{
    // PERIPHERAL PIN SELECT
    __builtin_write_OSCCONL(OSCCON & ~(1<<6));  // Unlock the registers
    RPO24 = OC2_out; //RP24 (pin 4) = PWM CLOCKOUT
    RPO15 = SDO1_out; // SDO1
    RPO22 = SCK1_out; // SCK1
    TRISCbits.TRISC7 = 0; //RC7 (Pin 16) = output = WM8510_SS (SPI slave select)
    
    CSCK_in = RPI25; // CSCLK << bCLK
    COFS_in = RPI11; // COFS << FRClk
    CSDI_in = RPI13; // CSDI << ADCout
    RPO12 = CSDO_out; // CSDO >> DACin
    __builtin_write_OSCCONL(OSCCON | (1<<6));   // Lock the registers

    // Initialise SPI1
    WM8510_SS = 1; //disable WM8510
    SPI1CON1bits.DISSCK = 0; //clock enabled
    SPI1CON1bits.DISSDO = 0; //SDO enabled
    SPI1CON1bits.MODE16 = 1; //16 bit mode
    SPI1CON1bits.SMP = 0; //data sampled in middle
    SPI1CON1bits.CKE = 1; //data changes on falling edge (active to idle)
    SPI1CON1bits.CKP = 0; //idle is low
    SPI1CON1bits.PPRE = 0; //64:1 primary prescaler
    SPI1CON1bits.SPRE = 0; //8:1 secondary prescaler (78.125kHz SPI clock)
    SPI1CON1bits.MSTEN = 1; //master mode
    SPI1STATbits.SPIEN = 1; //enable SPI
    
    //Initialise DCI module
    DCICON1bits.CSCKD = 1; //CSCK is an input
    DCICON1bits.COFSD = 1; //COFS is an input
    DCICON1bits.COFSM = 1; //I2S frame sync
    DCICON1bits.CSCKE = 1; //sample on rising edge
    DCICON1bits.DJST = 0; //data transmission starts 1 clock after frame
    DCICON2bits.BLEN = 3; //4 data words buffered between interrupts
    DCICON2bits.COFSG = 1; //data frame has 2 words
    DCICON2bits.WS = 15; //word data size is 16 bits
    TSCON = 3; //transmit in time slots 0 and 1
    RSCON = 3; //receive in time slots 0 and 1
    DCICON3 = 0;
    IPC15bits.DCIIP = 6; //Set DCI interrupt priority
    IFS3bits.DCIIF = 0; //clear DCI interrupt
    IEC3bits.DCIIE = 1; //enable DCI interrupt
    DCICON1bits.DCIEN = 1; //module enabled
    //DCICON1bits.DLOOP = 1; //loopback
    
    //Initialise output compare module 1 for PWM output (CLOCKOUT)
    OC1CONbits.OCM = 0; //disable module
    OC1R = 2; //default duty cycle
    OC1CONbits.OCTSEL = 0; //Select timer 2 as time base
    OC1CONbits.OCM = 0b110; //Select output mode PWM
    
    //Initialise timer 2 - 10Mhz clock out to drive WM8510
    T2CONbits.TON = 0; //Disable timer
    T2CONbits.TCS = 0; //Select internal instruction cycle clock
    T2CONbits.TGATE = 0; //Disable gated mode
    T2CONbits.TCKPS = 0; //1:1 Prescaler
    TMR2 = 0; //Clear timer register
    PR2 = 3; //Load the period value PWM freq = 10Mhz
    IPC1bits.T2IP = 1; //Set timer 2 priority level
    IFS0bits.T2IF = 0; //Clear timer 2 interrupt flag
    T2CONbits.TON = 1; //Start timer 2

    CLOCKOUT = 2;
}

void wm8510_config(void)
{
    //Configure WM8510
    wm8510_write(software_reset, 0); //reset
    wm8510_write(power_mngmnt1, 79); //mic2 on, bias on, tie off buffer enabled, vmid 5k
    wm8510_write(power_mngmnt2, 17); //Boost on, ADC on
    wm8510_write(power_mngmnt3, 137); //mono out on, mono mixer on, DAC on
    //wm8510_write(input_ctrl, 8); //mic2 = mixer

    //PLL setup to generate 8.192MHz clock for 32kHz sample rate
    wm8510_write(pll_n, 9); //PLL n = 9
    wm8510_write(pll_k1, 53);
    wm8510_write(pll_k2, 74);
    wm8510_write(pll_k3, 280);
    wm8510_write(clock_gen_ctrl, 361); //PLL output, PLL/3, BCCLK/4, Frame master
    wm8510_write(power_mngmnt1, 111); //PLL on

    wm8510_write(audio_interface, 208); //frame clock inverted, word length 24bits, I2S format
    wm8510_write(adc_boost_ctrl, 5); //input boost 0dB
    //wm8510_write(companding_ctrl, 1); //Loopback
    wm8510_write(dac_control, 24); //32kHz de-emphasis, 128x DAC oversample
    wm8510_write(adc_control, 264); //hipass on (removes any audio DC offset), 128x ADC oversample
    wm8510_write(additional_ctrl, 2); //32KHz sample rate
    wm8510_write(mono_mixer_ctrl, 1); //DAC to mono mixer
}

void wm8510_write(int reg, int data) {

    int temp;

    WM8510_SS = 0;

    SPI1BUF = (data | (reg << 9));
    while (!SPI1STATbits.SPIRBF);
    temp = SPI1BUF;
  
    WM8510_SS = 1;
}