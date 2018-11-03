#include "config.h"
#include "eeprom.h"
#include "effect.h"

static void config_setDefaults(void);

void config_load(void)
{
    // read config from eeprom
    // perform validity test
    // if valid, use
    // else, set defaults
    
    int len = sizeof(fx[0]);
    printf("Reading fx config (expectd size is %d \n", len);
    eeprom_readSeq(0, &fx[0], len);
    printf("Read complete \n");
    
    printf("Config dump\n");
    int i;
    const unsigned char * const px = (unsigned char*)&fx[0];
    for (i = 0; i < sizeof(fx[0]); ++i) printf("%02X ", px[i]);
    printf("\n");
}

void config_save(void)
{
    // write config to eeprom
    int len = sizeof(fx[0]);
    printf("Config dump\n");
    int i;
    const unsigned char * const px = (unsigned char*)&fx[0];
    for (i = 0; i < len; ++i) printf("%02X ", px[i]);
    printf("\n");
    
    printf("Writing fx config \n");
    eeprom_writeSeq(0,&fx[0], len);
    printf("Write complete \n");
}

static void config_setDefaults(void)
{
    // use the default config (no effects selected)
}
