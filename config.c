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
    // read back all the data from NV
	uint32_t src = 0;
	
	// calculate the destination address 
	uint8_t * dest = (uint8_t*)&config_nv;
	
	// length of the data to read 
	int32_t length = sizeof(config_nv_t);
    
    printf("Getting %lu bytes from eeprom\n", (unsigned long)length);
	
	// Read the all the data from NV 
	eeprom_readSeq(src, dest, length);
    
   // load the config from nv storage
    bool valid_config = config_verify();
    
    // if config wasn't valid, write the defaults to NV
    // TODO: is this actually a good idea?
    if (!valid_config)
    {
        // set the config default
        config_defaults();
        // save the defaults
        config_save();
        // set all the patches to the default
        int i;
        for(i=0;i<MAX_PATCHES;i++)
            config_savePatch(i);
    }
    
    //config_print();
}

void config_print(void)
{
    
    printf("-------- Config  ------------------\n");
    printf("Header:             %d\n", sizeof(config_header_t));
    printf("Patch:              %d\n", sizeof(config_patch_t));
    printf("    FX:             %d\n", sizeof(config_fx_t));
    printf("Total:              %d\n", sizeof(config_nv_t));
    printf("-----------------------------------\n");
    
    printf("\n\n\n\n");
    
    printf("---Header---\n");
    printf("Signature:  0x%08lX\n", config_nv.Header.Signature);
    printf("Version:    v%d\n", config_nv.Header.Version);
    printf("---LUT------\n");
    

    int i;
    /*
    for (i=0; i<MAX_PATCHES; i++)
        printf("%d:         0x%08lX\n", i, config_nv.Lut.Address[i]);
    */
    
    
    
    printf("---Patch----\n");
    for (i=0; i<kEffectCount; i++)
    {
        int j;
        
        effectInfo_t *currentFx;
        currentFx = &fx[i];
        
        printf("\n");
        
        if(config_nv.Patch.Fx[i].Enabled)
            printf("%s: Enabled\n", currentFx->Name);
        else
            printf("%s: Disabled\n", currentFx->Name);
        
        for(j=0; j<MAX_PARAMETERS; j++)
        {
            printf("Param %d: %d\n", j, config_nv.Patch.Fx[i].ParamValue[j]);
        }
    }
    printf("--------------\n");
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
    return true;
}

void config_save(void)
{        
    nv_write(0, (uint8_t*)&config_nv, sizeof(config_nv_t));
}

void config_defaults(void)
{
    // first set the defaults
    effect_set_defaults();
    
    
    // set the header information
    config_nv.Header.Signature = CONFIG_SIGNATURE;
    config_nv.Header.Version = CONFIG_VERSION;
    
    // write the default effect settings
    config_applyEffects();
    //config_nv.Patch.Name = "Default";
    strcpy (config_nv.Patch.Name,"Default");
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
}
/*@brief save the current loadout into a patch location*/
void config_savePatch(uint8_t index)
{
    index++;
    config_patch_t tmp = config_nv.Patch;   // Copy the patch to a temporary location
    //tmp.Name = name;
    tmp.Index = index;
    
    uint32_t mem_location = index * ((EEPROM_SIZE-sizeof(config_nv_t))/MAX_PATCHES);
    mem_location+=sizeof(config_nv_t);
    
    printf("Writing patch [%i] to address [0x%08lX]\n", index, mem_location);
    nv_write(mem_location, (uint8_t*)&tmp, sizeof(config_patch_t));
}

void config_loadPatch(uint8_t index)
{    
    index++;
    // read back all the data from NV
	uint32_t src = sizeof(config_nv_t) + (index * ((EEPROM_SIZE-sizeof(config_nv_t))/MAX_PATCHES));
	
	// calculate the destination address 
	uint8_t * dest = (uint8_t*)&config_nv.Patch;
	
	// length of the data to read 
	int32_t length = sizeof(config_patch_t);
    
    printf("Reading patch [%i] from address [0x%08lX]\n", index, src);
	
	// Read the all the data from NV 
	eeprom_readSeq(src, dest, length);
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
