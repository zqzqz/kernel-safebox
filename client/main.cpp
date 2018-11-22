#include <iostream>
#include "CLIHandler.h"

int main(void)
{
    CLIHandler cli = CLIHandler();
    printf("Note: Welcome to Interactive Command!\n");
    printf("      Press 'Quit'/'Q' to quit!\n\n");
    InitReadLine();
    while (1)
    {
        char *mCmdLine = cli.ReadCmdLine();
        if (cli.IsUserQuitCmd(mCmdLine))
        {
            break;
        }

        cli.ExecCmd(mCmdLine);
    }
    return 0;
}