#include "command.h"
#define MAX_COMMAND_LEN 256

Command::Command()
{
    char *command_name = "test";
    cmdMap[command_name] = test;
}

int Command::ExecCmd(char *mCmdLine)
{
    try
    {
        char **mArgs = NULL;
        CmdProcFunc func = cmdMap[mCmdLine];
        if (func)
        {
            return func(mArgs);
        }
        else
        {
            std::cout << "Error" << std::endl;
            return -1;
        }
    }
    catch (...)
    {
        return -1;
    }
}

int test(char **args)
{
    std::cout << "test one" << std::endl;
    return 0;
}

int put(char **args)
{
    return 0;
}

int get(char ** args)
{
    return 0;
}

int list(char ** args)
{
    return 0;
}