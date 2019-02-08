#include "config.h"
#include "effect.h"
#include "eeprom.h"
#include <stddef.h>

static void config_generateLUT(void);

static uint16_t patch_lut[MAX_PATCHES] = {0};

static config_nv_t config_nv;

/**
 * @brief Get a reference to the configuration data
 *
 * The obtained reference is intentionally read only
 * Use this for normally access
 */
const config_nv_t * config_get_reference(void)
{
	return &config_nv;
}


/**
 * @brief Get a reference to the configuration data
 *
 * The obtained reference has read and write access
 * 
 * @warning Use this only in calibration and set up routines
 *
 * @return A read and write pointer to the configuration data;
 */
config_nv_t * config_get_writable_reference(void)
{
	return &config_nv;
}

void config_init(void)
{
    // print the config size (debug)
    printf("-------- CONFIG SIZE INFO ---------\n");
    
    
    printf("Header:             %d\n", sizeof(config_header_t));
    printf("LUT:                %d\n", sizeof(config_lut_t));
    printf("Patch:              %d\n", sizeof(config_patch_t));
    printf("    FX:             %d\n", sizeof(config_fx_t));
    printf("Total:              %d\n", sizeof(config_nv_t));
    printf("-----------------------------------\n");
    
    
    // read back all the data from NV
	uint32_t src = 0;
	
	// calculate the destination address 
	uint8_t * dest = (uint8_t*)&config_nv;
	
	// length of the data to read 
	int32_t length = sizeof(config_nv_t);
	
	// Read the all the data from NV 
	eeprom_readSeq(src, dest, length);
    
   // load the config from nv storage
    bool valid_config = config_verify();
    
    // if config wasn't valid, write the defaults to NV
    // TODO: is this actually a good idea?
    if (!valid_config)
        config_defaults();
}

bool config_verify(void)
{
    uint32_t sig = config_nv.Header.Signature;
    uint32_t vers = config_nv.Header.Version;
    
    if (sig != CONFIG_SIGNATURE)
    {
        printf("CONFIG SIGNATURE INVALID: Expected 0x%08lX, got 0x%08lX\n",CONFIG_SIGNATURE,sig);
        return false;
    }
    if (vers != CONFIG_VERSION)
    {
        printf("CONFIG VERSION INVALID: Expected 0x%08lX, got 0x%08lX\n",CONFIG_VERSION,vers);
        return false;
    }
    printf("Config OK\n");
}

void config_save(void)
{    
    eeprom_writeSeq(0, (uint8_t*)&config_nv/*, sizeof(config_nv_t)*/);
}

void config_defaults(void)
{
    // apply the default config... 
    
    // set the header information
    config_nv.Header.Signature = CONFIG_SIGNATURE;
    config_nv.Header.Version = CONFIG_VERSION;
    
    // clear the look up table
    int i;
    for (i=0; i<MAX_PATCHES; i++)
        config_nv.Lut.Address[i] = 0;
    
    // write the default effect settings
    config_applyEffects();
}

void config_applyEffects()
{
    int i;
        // write the default effect settings
    for (i=0; i<kEffectCount; i++)
    {
        int j;
        
        effectInfo_t *currentFx;
        currentFx = &fx[i];
        
        config_nv.Patch.Fx[i].Enabled = currentFx->Enabled;
        for(j=0; j<MAX_PARAMETERS; j++)
        {
            config_nv.Patch.Fx[i].ParamValue[j] = currentFx->Parameter[j].Value;
        }
    }
    
    config_save();
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
