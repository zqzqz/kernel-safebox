#include "command.h"
#include <vector>
#define MAX_COMMAND_LEN 128

Command::Command()
{
    char *command_name = "test";
    cmdMap[command_name] = test;
}

int Command::ExecCmd(char *mCmdLine)
{
    try
    {
        if (mCmdLine == NULL)
        {
            return -1;
        }
        // place arguments
        std::vector<char *> cmdArgs;
        // place the command name
        char *cmdName = new char[MAX_COMMAND_LEN];
        bool spaceFlag = false;
        bool cmdFlag = true;
        char delimiter = ' ';
        int start = 0;
        for (int i = 0; i <= strlen(mCmdLine); i++)
        {
            if (i == strlen(mCmdLine) || mCmdLine[i] == delimiter)
            {
                if (!spaceFlag)
                {
                    spaceFlag = true;
                    if (cmdFlag) {
                        strncpy(cmdName, mCmdLine+start, i-start);
                        cmdFlag = false;
                    }
                    else {
                        char * arg = new char[MAX_COMMAND_LEN];
                        strncpy(arg, mCmdLine+start, i-start);
                        cmdArgs.push_back(arg);
                    }
                }
            }
            else
            {
                if (spaceFlag)
                {
                    start = i;
                    spaceFlag = false;
                }
            }
        }
        char ** mArgs = new char* [cmdArgs.size()];
        for (int i=0; i<cmdArgs.size(); i++) {
            mArgs[i] = new char[MAX_COMMAND_LEN];
            strcpy(mArgs[i], cmdArgs[i]);
            delete cmdArgs[i];
        }
        CmdProcFunc func = cmdMap[cmdName];
        if (func)
        {
            return func(mArgs, cmdArgs.size());
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

int test(char **args, int argCnt)
{
    std::cout << "test one" << std::endl;
    int i = 0;
    for (int i=0; i< argCnt; i++) {
        std::cout<<"argument "<<i<<": "<<*(args+i)<<std::endl;
    }
    return 0;
}

int put(char **args, int argCnt)
{
    return 0;
}

int get(char **args, int argCnt)
{
    return 0;
}

int list(char **args, int argCnt)
{
    return 0;
}