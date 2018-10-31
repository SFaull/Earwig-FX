#include "volume.h"

// This function attenuates the signal. Division of the sampled value is used to attenuate the sample
// and 16 linear discrete volume levels are defined 
signed int volume(signed int x, int level)
{
  switch (level)
  {              
     case  0: // 0% Volume
             return 0;   
             break;
     case  1: // 6.25% Volume
             return ( (x>>4) );          
             break;
     case  2: // 12.5% Volume
             return ( (x>>3) );          
             break;
     case  3:  // 18.75% Volume
             return ( (x>>4)+ (x>>3) );        
             break;         
     case  4:  // 25% Volume
             return ( (x>>2) );        
             break;
     case  5:  // 31.25% Volume
             return ( (x>>2)+(x>>4) );       
             break;
   	 case  6:  // 37.5% Volume
             return ( (x>>2)+(x>>3) );       
             break;
     case  7:  // 43.75% Volume
             return ( (x>>2)+(x>>4)+(x>>3) );     
             break;
     case  8:  // 50% Volume
             return ( (x>>1) );     
             break;        
     case  9: // 56.25% Volume
             return ( (x>>1)+(x>>4) );
             break;
     case 10: // 62.5% Volume
             return ( (x>>1)+(x>>3) );       
             break;
     case 11: // 68.75% Volume
             return ( (x>>1)+(x>>4)+(x>>3) );         
             break;
     case 12:  // 75% Volume
             return ( (x>>1)+ (x>>2) );        
             break;         
     case 13:  // 81.25% Volume
             return ( (x>>1)+(x>>2)+(x>>4) );          
             break;
     case 14:  // 87.5% Volume
             return ( (x>>1)+(x>>2)+(x>>3) );    
             break;
   	 case 15:  // 93.75% Volume
             return ( (x>>1)+(x>>2)+(x>>4)+(x>>3) );       
             break;
     case 16:  // 100% Volume
            return x; 
            break;
      default:  // should never happen...
          return 0;
          break;
  }  
}
