/*
 * File:        version.h
 * Author:      Sam Faull
 * Description:
 */

#ifndef VERSION_H
#define VERSION_H

/** @brief String representing the device part number */
#define MODEL_STRING		"Earwig"	
#define BUILD_DATE          __DATE__
#define BUILD_TIME          __TIME__

#define STRINGIFY(x)			#x
#define TOSTRING(x)				STRINGIFY(x)

/** @brief Firmware Version String */
#define FW_VERSION_STR 	"v"		TOSTRING(FW_VERSION_MAJOR) "." \
								TOSTRING(FW_VERSION_MINOR) "." \
								TOSTRING(FW_VERSION_PATCH)



/** @brief Firmware major version number (max 255)*/
#define FW_VERSION_MAJOR		0

/** @brief Firmware minor version number (max 255)*/
#define FW_VERSION_MINOR		0

/** @brief Firmware patch version number (max 65535) */
#define FW_VERSION_PATCH		1

/** @brief Firmware version as a 32-bit number */
#define FW_VERSION_NUMBER		(((uint32_t)FW_VERSION_MAJOR<<24)|((uint32_t)FW_VERSION_MINOR<<16)|(FW_VERSION_PATCH))


#endif /* VERSION_H */

