/*
 * File:        parser.h
 * Author:      Sam Faull
 * Description: 
 */

#ifndef PARSER_H
#define	PARSER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#define SERIALCOMMANDBUFFER 16
#define MAXSERIALCOMMANDS	10
#define MAXDELIMETER 2

#define SERIALCOMMANDDEBUG 1
#undef SERIALCOMMANDDEBUG      // Comment this out to run  library in debug mode (verbose messages)


void initSerialCommand();
void clearBuffer();   // Sets the command buffer to all '\0' (nulls)
char *next();         // returns pointer to next token found in command buffer (for getting arguments to commands)
void readSerial(char c);    // Main entry point.  
void addCommand(const char *command, void (*function)());
//void addCommand(const char *, void(*)());   // Add commands to processing dictionary
void addDefaultHandler(void (*function)());    // A handler to call when no valid command received. 


#endif	/* PARSER_H */

