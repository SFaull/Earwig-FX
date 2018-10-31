#include "filter.h"

signed int lowpass(signed int lp_in) 
{
    static signed long lp_out = 0;  // variable persists from one call of a function to another

    lp_out = ((((lp_in - lp_out) * 8000) >> 15) + lp_out);   // shift by 15 to divide by approximate sampling freq

    return (lp_out);
}

