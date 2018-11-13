#include "config.h"
#include "effect.h"
#include "eeprom.h"

static void config_setDefaults(void);

void config_load(void)
{
    // read config from eeprom
    // perform validity test
    // if valid, use
    // else, set defaults
    
    int address;
    int fxIndex;
    
    printf("Reading fx config \n");
    
    for(fxIndex=0; fxIndex<kEffectCount; fxIndex++)
    {
        address = fxIndex*4;
        fx[fxIndex].Enabled = eeprom_readByte(address);
        fx[fxIndex].Parameter[0].Value = eeprom_readByte(address+1);
        fx[fxIndex].Parameter[1].Value = eeprom_readByte(address+2);
        fx[fxIndex].Parameter[2].Value = eeprom_readByte(address+3);
    }
    
    printf("Read complete \n");
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
        eeprom_writeByte(address, fx[fxIndex].Enabled);
        eeprom_writeByte(address+1, fx[fxIndex].Parameter[0].Value);
        eeprom_writeByte(address+2, fx[fxIndex].Parameter[1].Value);
        eeprom_writeByte(address+3, fx[fxIndex].Parameter[2].Value);
    }
    
    printf("Write complete \n");
}


static void config_setDefaults(void)
{
    // use the default config (no effects selected)
}
