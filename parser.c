#include "commands.h"
#include <string.h>
#include <stdio.h>
#include "parser.h"
#include <ctype.h>

char inChar;          // A character read from the serial stream 
char serial_buffer[SERIALCOMMANDBUFFER];   // Buffer of stored characters while waiting for terminator character
int  bufPos;                        // Current position in the buffer
char delim[MAXDELIMETER];           // null-terminated list of character to be used as delimeters for tokenizing (default " ")
char term;                          // Character that signals end of command (default '\r')
char *token;                        // Returned token from the command buffer as returned by strtok_r
char *last;                         // State variable used by strtok_r during processing

typedef struct _callback {
    char command[SERIALCOMMANDBUFFER];
    void (*function)();
} SerialCommandCallback;            // Data structure to hold Command/Handler function key-value pairs

int numCommand;
SerialCommandCallback CommandList[MAXSERIALCOMMANDS];   // Actual definition for command/handler array
void (*defaultHandler)();           // Pointer to the default handler function 



static char *
strtok_r (char *s, const char *delim, char **save_ptr);
static void clearBuffer();

void parser_printCommandList(void)
{
    int i;
    for(i=0; i<MAXSERIALCOMMANDS; i++)
    {
        printf("%s\n", CommandList[i].command);
    }
}

// Constructor makes sure some things are set. 
void parser_init()
{
	strncpy(delim," ",MAXDELIMETER);  // strtok_r needs a null-terminated string
	term='\r';   // return character, default terminator for commands
	numCommand=0;    // Number of callback handlers installed
	clearBuffer(); 
}

// Retrieve the next token ("word" or "argument") from the Command buffer.  
// returns a NULL if no more tokens exist.   
char *parser_next() 
{
	char *nextToken;
	nextToken = strtok_r(NULL, delim, &last); 
	return nextToken; 
}

// This checks the Serial stream for characters, and assembles them into a buffer.  
// When the terminator character (default '\r') is seen, it starts parsing the 
// buffer for a prefix command, and calls handlers setup by addCommand() member
void parser_process(char *c) 
{
    int i; 
    bool matched; 
    
    inChar = toupper(*c);
    
    if (inChar==term) {     // Check for the terminator (default '\r') meaning end of command
        bufPos=0;           // Reset to start of buffer
        token = strtok_r(serial_buffer,delim,&last);   // Search for command at start of buffer
        if (token == NULL) return; 
        matched=false; 
        for (i=0; i<numCommand; i++) {
            // Compare the found command against the list of known commands for a match
            if (strncmp(token,CommandList[i].command,SERIALCOMMANDBUFFER) == 0) 
            {
                // Execute the stored handler function for the command
                (*CommandList[i].function)(); 
                clearBuffer(); 
                matched=true; 
                break; 
            }
        }
        if (matched==false) {
            (*defaultHandler)(); 
            clearBuffer(); 
        }

    }
    //Only printable characters into the buffer
    if(isprint(inChar))
    {
        serial_buffer[bufPos++]=inChar;   // Put character into buffer
        serial_buffer[bufPos]='\0';  // Null terminate
        if (bufPos > SERIALCOMMANDBUFFER-1) bufPos=0; // wrap buffer around if full  
    }


}

// Adds a "command" and a handler function to the list of available commands.  
// This is used for matching a found token in the buffer, and gives the pointer
// to the handler function to deal with it. 
void parser_addCommand(const char *command, void (*function)())
{
	if (numCommand < MAXSERIALCOMMANDS) {		
		strncpy(CommandList[numCommand].command,command,SERIALCOMMANDBUFFER); 
		CommandList[numCommand].function = function; 
		numCommand++; 
	} else {
		// In this case, you tried to push more commands into the buffer than it is compiled to hold.  
		// Not much we can do since there is no real visible error assertion, we just ignore adding
		// the command
	}
}

// This sets up a handler to be called in the event that the receveived command string
// isn't in the list of things with handlers.
void parser_addDefaultHandler(void (*function)())
{
	defaultHandler = function;
}


//
// Initialize the command buffer being processed to all null characters
//
static void clearBuffer()
{
    int i;
	for (i=0; i<SERIALCOMMANDBUFFER; i++) 
	{
		serial_buffer[i]='\0';
	}
	bufPos=0; 
}


static char *
strtok_r (char *s, const char *delim, char **save_ptr)
{
  char *end;
  if (s == NULL)
    s = *save_ptr;
  if (*s == '\0')
    {
      *save_ptr = s;
      return NULL;
    }
  /* Scan leading delimiters.  */
  s += strspn (s, delim);
  if (*s == '\0')
    {
      *save_ptr = s;
      return NULL;
    }
  /* Find the end of the token.  */
  end = s + strcspn (s, delim);
  if (*end == '\0')
    {
      *save_ptr = end;
      return s;
    }
  /* Terminate the token and make *SAVE_PTR point past it.  */
  *end = '\0';
  *save_ptr = end + 1;
  return s;
}