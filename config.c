#include "config.h"
#include "effect.h"
#include "eeprom.h"
#include <stddef.h>


static void config_setDefaults(void);
static void config_generateLUT(void);

static uint16_t patch_lut[MAX_PATCHES] = {0};

void config_init(void)
{
    printf("-------- CONFIG SIZE INFO ---------\n");
    
    
    printf("Header:             %d\n", sizeof(config_header_t));
    printf("LUT:                %d\n", sizeof(config_lut_t));
    printf("Patch:              %d\n", sizeof(config_patch_t));
    printf("    FX:             %d\n", sizeof(config_fx_t));
    printf("Total:              %d\n", sizeof(config_nv_t));
    printf("-----------------------------------\n");
}

bool config_load(void)
{
    config_init();
    config_generateLUT();
    // read signature
    uint8_t a = eeprom_readByte(EEPROM_SIZE-4);
    uint8_t b = eeprom_readByte(EEPROM_SIZE-3);
    uint8_t c = eeprom_readByte(EEPROM_SIZE-2);
    uint8_t d = eeprom_readByte(EEPROM_SIZE-1);
    
    uint16_t dead = ((uint32_t)a << 8) | (uint32_t)b;
    uint16_t beef = ((uint32_t)c << 8) | (uint32_t)d;
    
    printf("dead: 0x%04X \n", dead);
    printf("beef: 0x%04X \n", beef);
    
    if((dead == 0xDEAD) && (beef == 0xBEEF))
    {    
        printf("Signature match SUCCESS! \n");
        
        int address;
        int fxIndex;

        printf("Reading fx config \n");

        for(fxIndex=0; fxIndex<kEffectCount; fxIndex++)
        {
            address = fxIndex*4;
            fx[fxIndex].Enabled = eeprom_readByte(address);
            eeprom_readSeq(address+1, &fx[fxIndex].Parameter[0].Value, 2);
            eeprom_readSeq(address+3, &fx[fxIndex].Parameter[1].Value, 2);
            eeprom_readSeq(address+5, &fx[fxIndex].Parameter[2].Value, 2);
        }

        printf("Read complete \n");
        return true;
    }

    printf("Signature match FAILED \n");
    return false;
}

void config_save(void)
{
    // write config to eeprom
    int address;
    int fxIndex;
    
    printf("Writing fx config \n");
    
    for(fxIndex=0; fxIndex<kEffectCount; fxIndex++)
    {
        
        address = fxIndex*4;
        printf("%i, %i \n", fxIndex, address);
        eeprom_writeByte(address, fx[fxIndex].Enabled);
        printf("%i, %i \n", fxIndex, address+1);
        eeprom_writeByte(address+1, fx[fxIndex].Parameter[0].Value);
        eeprom_writeByte(address+2, fx[fxIndex].Parameter[0].Value >> 8);
        printf("%i, %i \n", fxIndex, address+3);
        eeprom_writeByte(address+3, fx[fxIndex].Parameter[1].Value);
        eeprom_writeByte(address+4, fx[fxIndex].Parameter[1].Value >> 8);
        printf("%i, %i \n", fxIndex, address+5);
        eeprom_writeByte(address+5, fx[fxIndex].Parameter[2].Value);
        eeprom_writeByte(address+6, fx[fxIndex].Parameter[2].Value >> 8);
    }
    eeprom_writeByte(EEPROM_SIZE-4, 0xDE);
    eeprom_writeByte(EEPROM_SIZE-3, 0xAD);
    eeprom_writeByte(EEPROM_SIZE-2, 0xBE);
    eeprom_writeByte(EEPROM_SIZE-1, 0xEF);
    
    printf("Write complete \n");
}


static void config_setDefaults(void)
{
    // use the default config (no effects selected)
}

static void config_generateLUT(void)
{
    // get the start address of the look up table
    //uint8_t start_address = offsetof(config_nv_t, Lut);
    
    uint16_t start_address = sizeof(config_nv_t);
    
    int i;
    for(i=0; i<MAX_PATCHES; i++)
    {
        patch_lut[i] = start_address + (i * sizeof(config_patch_t));
        //printf("Index: %i, Address: %d\n", i, patch_lut[i]);
    }
}
