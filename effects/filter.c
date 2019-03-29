#include <dsp.h>
#include "filter.h"
#include "../effect.h"

#define LOG2_BLOCK_LENGTH  		7            
#define FFT_BLOCK_LENGTH  		SAMPLE_BUFFER_SIZE          
#define IFFT_BLOCK_LENGTH  		SAMPLE_BUFFER_SIZE          
#define FRAME_SIZE 				SAMPLE_BUFFER_SIZE			/* Each audio frame will have these many samples	*/


fractcomplex fft_input[FFT_BLOCK_LENGTH] __attribute__((space(ymemory),   aligned(FFT_BLOCK_LENGTH*2*2))); 

fractional window[FFT_BLOCK_LENGTH] __attribute__((space(ymemory),  aligned(FFT_BLOCK_LENGTH*2))); 

fractcomplex twiddleFactorsFFT[FFT_BLOCK_LENGTH/2] 	/* Declare Twiddle Factor array in X-space*/
__attribute__ ((section (".xbss, bss, xmemory"), aligned (FFT_BLOCK_LENGTH*2)));

fractcomplex twiddleFactorsIFFT[IFFT_BLOCK_LENGTH/2] 	/* Declare Twiddle Factor array in X-space*/
__attribute__ ((section (".xbss, bss, xmemory"), aligned (IFFT_BLOCK_LENGTH*2)));

/* A simple lowpass filter */
signed int lowpass(signed int lp_in) 
{
    static signed long lp_out = 0;  // variable persists from one call of a function to another

    lp_out = ((((lp_in - lp_out) * 8000) >> 15) + lp_out);   // shift by 15 to divide by approximate sampling freq

    return (lp_out);
}

/* A complicated lowpass filer using fft*/
signed int unknown(signed int *samples) 
{
    int i;
    
    /* first time in the function initialse the FFT stuff*/
    static bool first = true;
    if(first)
    {
        first = false;
        TwidFactorInit (LOG2_BLOCK_LENGTH, &twiddleFactorsFFT[0], 0);
        TwidFactorInit (LOG2_BLOCK_LENGTH, &twiddleFactorsIFFT[0], 1); 
    }
    
    // copy sample buffer into 
    for(i=0; i<FRAME_SIZE; i++)
    {
        fft_input[i].real = samples[i]; 
    }	
    
    VectorWindow(FFT_BLOCK_LENGTH,&fft_input[FFT_BLOCK_LENGTH].real,&fft_input[FFT_BLOCK_LENGTH].real,(fractional*)&window[0]);

    for(i=0; i<FFT_BLOCK_LENGTH; i++)
    {
        fft_input[i].real = fft_input[i].real>>1;  
        fft_input[i].imag = 0x0000;
    }	

    /* Perform FFT operation */

    FFTComplexIP (LOG2_BLOCK_LENGTH, &fft_input[0], &twiddleFactorsFFT[0], COEFFS_IN_DATA);

    /* Store output samples in bit-reversed order of their addresses */
    BitReverseComplex (LOG2_BLOCK_LENGTH, &fft_input[0]);

    IFFTComplexIP (LOG2_BLOCK_LENGTH, &fft_input[0], &twiddleFactorsIFFT[0], COEFFS_IN_DATA);

    for(i=0; i<FRAME_SIZE; i++)
    {
        samples[i] = (fft_input[i].real<<8);         //  
    }

    // write everything to the codec
    /* Write the frame to the output	*/
    //WM8510Write (codecHandle,samples,FRAME_SIZE);    
}
