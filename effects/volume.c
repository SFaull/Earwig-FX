#include <stdbool.h>
#include "volume.h"

// This function attenuates the signal. Division of the sampled value is used to attenuate the sample
// and 16 linear discrete volume levels are defined 
/** @brief level can be -16 to 16*/
signed int volume(signed int x, int level)
{
  signed int sample = 0;
  bool gain = false;
  
  // cap the upper and lower levels
  if(level > VOLUME_MAX) level = VOLUME_MAX;
  if(level < VOLUME_MIN) level = VOLUME_MIN;
  
  // check if gain or attenuation
  if(level > 0)
  {
      gain = true;
      level -= 16;
  }
  
  switch (level)
  {              
     case -16:  sample = 0;                             break;  // 0% Volume
     case -15:  sample = (x>>4);                        break;  // 6.25% Volume        
     case -14:  sample = (x>>3);                        break;  // 12.5% Volume
     case -13:  sample = (x>>4) + (x>>3);               break;  // 18.75% Volume       
     case -12:  sample = (x>>2);                        break;  // 25% Volume
     case -11:  sample = (x>>2)+(x>>4);                 break;  // 31.25% Volume
   	 case -10:  sample = (x>>2)+(x>>3);                 break;  // 37.5% Volume
     case -9:   sample = (x>>2)+(x>>4)+(x>>3);          break;  // 43.75% Volume
     case -8:   sample = (x>>1);                        break;  // 50% Volume
     case -7:   sample = (x>>1)+(x>>4);                 break;  // 56.25% Volume
     case -6:   sample = (x>>1)+(x>>3);                 break;  // 62.5% Volume
     case -5:   sample = (x>>1)+(x>>4)+(x>>3);          break;  // 68.75% Volume
     case -4:   sample = (x>>1)+ (x>>2);                break;  // 75% Volume        
     case -3:   sample = (x>>1)+(x>>2)+(x>>4);          break;  // 81.25% Volume
     case -2:   sample = (x>>1)+(x>>2)+(x>>3);          break;  // 87.5% Volume
   	 case -1:   sample = (x>>1)+(x>>2)+(x>>4)+(x>>3);   break;  // 93.75% Volume
     case 0:    /* do nothing */                        break;  // 100% Volume
     default:   /* do nothing */                        break;
  }  
  
  if(gain)
      sample += x;
  
  return sample;
}
