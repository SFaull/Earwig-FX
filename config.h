/*
 * File:        config.h
 * Author:      Sam Faull
 * Description: 
 */

#include <stdbool.h>
#include "effect.h"

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef CONFIG_H
#define	CONFIG_H


#define CONFIG_SIGNATURE		0xDEADBEEF
#define CONFIG_VERSION          3

// Support for upto 99 patches
#define MAX_PATCHES 99

/** @brief Configuration header structure */
typedef struct {
	uint32_t Signature;														// 4 bytes
	uint8_t Version;														// 1 byte
    uint8_t Pad[11];                                                        // 11 bytes
} config_header_t;                                                          // TOTAL = 16 bytes
                                                      // TOTAL = 200 bytes	

/** @brief Stripped down parameter for config structure */
typedef struct{
    uint8_t Enabled;                                                        // 1 byte
    uint16_t ParamValue[MAX_PARAMETERS];                                    // 2 * 3 = 6 bytes
    uint8_t Pad;                                                            // 1 byte
} config_fx_t;                                                              // TOTAL = 8 bytes

/** @brief Patch structure */
typedef struct {
	uint8_t Index;                                                          // 1 bytes
    char Name[10];                                                          // 10 bytes
    config_fx_t Fx[kEffectCount];                                           // 8 * 6 = 48 
    uint8_t Pad[5];                                                         // 5 bytes
} config_patch_t;                                                           // TOTAL = 64 

/**
 * @brief Configuration data in NV memory
 */
typedef struct {
	config_header_t Header;												// 16 bytes
	config_patch_t Patch;                                               // 64 bytes
} config_nv_t;                                                          // TOTAL = 280 bytes

void config_print(void);
bool config_verify(void);
void config_save(void);
void config_init(void);
void config_defaults(void);
void config_applyEffects();
void config_loadPatch(uint8_t index);
void config_savePatch(uint8_t index);

const config_nv_t * config_get_reference(void);
config_nv_t * config_get_writable_reference(void);

#endif	/* CONFIG_H */