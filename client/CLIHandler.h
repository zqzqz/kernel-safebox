#ifndef _READLINE_CLI
#define _READLINE_CLI

#include <readline/readline.h>
#include <readline/history.h>
#include "command.h"

class CLIHandler: public Command
{
  public:
    CLIHandler();
    const char *const mCmdPrompt = "manager>> ";

    // Quit command
    char **mQuitCmd;
    unsigned char mQuitCmdNum;

    char *mLineRead;  //command input
    char *mStripLine; //command input without leading spaces

    int IsUserQuitCmd(char *mCmd);

    // remove spaces in commands
    char *StripWhite(char *mOrig);

    char *ReadCmdLine();
};

char *CmdGenerator(const char *mText, int dwState);

char **CmdCompletion(const char *mText, int dwStart, int dwEnd);

// initialize command completion
void InitReadLine(void);

#endif