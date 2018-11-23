#include "command.h"
#include "cryptoUtils.h"
#include <fstream>
#include <vector>
#define MAX_COMMAND_LEN 128

Command::Command()
{
    cmdMap["test"] = test;
    cmdMap["put"] = put;
    cmdMap["get"] = get;
    cmdMap["list"] = list;
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
                    if (cmdFlag)
                    {
                        strncpy(cmdName, mCmdLine + start, i - start);
                        cmdFlag = false;
                    }
                    else
                    {
                        char *arg = new char[MAX_COMMAND_LEN];
                        strncpy(arg, mCmdLine + start, i - start);
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
        char **mArgs = new char *[cmdArgs.size()];
        for (int i = 0; i < cmdArgs.size(); i++)
        {
            mArgs[i] = new char[MAX_COMMAND_LEN];
            strcpy(mArgs[i], cmdArgs[i]);
            delete cmdArgs[i];
        }
        CmdProcFunc func = cmdMap[cmdName];
        if (func)
        {
            int res = func(mArgs, cmdArgs.size());
            if (res != 0)
            {
                ERROR("Execution of command is failed (code " + std::to_string(res) + ")");
            }
        }
        else
        {
            ERROR("The command is not found");
            return -1;
        }
    }
    catch (...)
    {
        ERROR("Unexpected failure");
        return -1;
    }
}

int test(char **args, int argCnt)
{
    std::cout << "test one" << std::endl;
    int i = 0;
    for (int i = 0; i < argCnt; i++)
    {
        std::cout << "argument " << i << ": " << *(args + i) << std::endl;
    }
    return 0;
}

int put(char **args, int argCnt)
{
    // fetch arguments
    char *filePath = *(args);
    char *password = *(args + 1);
    // read input file
    std::ifstream in(filePath);
    if (!in)
    {
        ERROR("Cannot open file" + std::string(filePath));
        return 1;
    }
    std::string text_str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    char *text = (char *)text_str.c_str();

    size_t cipherLen = strlen(text);
    // encrypt
    char *cipher;
    CryptoUtils utils = CryptoUtils();
    if (utils.AESEncrypt(text, cipher, password) != 0)
    {
        ERROR("AES Encryption failed");
        return 2;
    }
    // write output file
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