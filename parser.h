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

#define SERIALCOMMANDBUFFER 64
#define MAXSERIALCOMMANDS	10
#define MAXDELIMETER 2

void parser_printCommandList(void);
void parser_init();
char *parser_next();         // returns pointer to next token found in command buffer (for getting arguments to commands)
void parser_process(char *c);    // Main entry point.  
void parser_addCommand(const char *, void(*)());   // Add commands to processing dictionary
void parser_addDefaultHandler(void (*function)());    // A handler to call when no valid command received. 


#endif	/* PARSER_H */

