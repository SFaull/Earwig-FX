#include "patch.h"
#include "config.h"

bool patch_exists(int index)
{
    uint16_t address = config_get_reference()->Lut.Address[index];
    return (address > sizeof(config_nv_t)); // valid patches have an address greater than 
}

// load a patch at a given address
void patch_load(uint16_t address)
{
    
}