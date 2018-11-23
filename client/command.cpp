#include <fstream>
#include <vector>
#include <dirent.h>
#include "command.h"
#include "cryptoUtils.h"
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
                        cmdName[i-start] = '\0';
                        cmdFlag = false;
                    }
                    else
                    {
                        char *arg = new char[MAX_COMMAND_LEN];
                        strncpy(arg, mCmdLine + start, i - start);
                        arg[i-start] = '\0';
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
                ERROR("Execution of command is failed");
            }
        }
        else
        {
            ERROR("The command is not found");
            return -1;
        }
    }
    catch (const std::exception &exc)
    {
        ERROR(exc.what());
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
    if (argCnt != 3)
    {
        ERROR("Unvalid number of arguments (required 3)");
        return 3;
    }
    // fetch arguments
    char *outFileName = *(args);
    char *inFileName = *(args + 1);
    char *password = *(args + 2);
    int res = 0;
    CryptoUtils utils = CryptoUtils();

    std::string key = utils.SHA256(std::string(password)).substr(0, 16);
    // read input file
    std::ifstream in(outFileName);
    if (!in)
    {
        ERROR("Cannot open file " + std::string(outFileName));
        return 1;
    }
    std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    // encrypt
    std::string cipher = utils.AESEncrypt(text, key, std::string());
    std::string cipherFileName = utils.AESEncrypt(inFileName, key, std::string());

    // write output file
    std::string fullFileName = std::string(BOX_PATH) + cipherFileName;
    std::ofstream out(fullFileName.c_str());
    if (!out)
    {
        ERROR("Cannot open file" + std::string(inFileName));
        return 1;
    }
    out << cipher;
    return 0;
}

int get(char **args, int argCnt)
{
    if (argCnt != 3)
    {
        ERROR("Unvalid number of arguments (required 3)");
        return 3;
    }
    // fetch arguments
    char *inFileName = *(args);
    char *outFileName = *(args + 1);
    char *password = *(args + 2);
    int res = 0;
    CryptoUtils utils = CryptoUtils();

    std::string key = utils.SHA256(std::string(password)).substr(0, 16);
    // read input file
    std::string cipherFileName = utils.AESEncrypt(inFileName, key, std::string());
    std::string fullFileName = std::string(BOX_PATH) + std::string(cipherFileName);
    std::ifstream in(fullFileName.c_str());
    if (!in)
    {
        ERROR("Cannot open file " + std::string(inFileName));
        return 1;
    }
    std::string cipher((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

    // edecrypt
    std::string text = utils.AESDecrypt(cipher, key, std::string());

    // write output file
    std::ofstream out(outFileName);
    if (!out)
    {
        ERROR("Cannot open file " + std::string(outFileName));
        return 1;
    }
    out << text;
    return 0;
}

int list(char **args, int argCnt)
{
    char *match = NULL;
    char *password = *(args);
    if (argCnt < 1)
    {
        ERROR("Unvalid number of arguments (required 1)");
        return 3;
    }
    else if (argCnt > 1)
    {
        match = *(args + 1);
    }
    DIR *dirp;
    struct dirent *dp;
    dirp = opendir(BOX_PATH);
    CryptoUtils utils = CryptoUtils();

    std::string key = utils.SHA256(std::string(password)).substr(0, 16);
    while ((dp = readdir(dirp)) != NULL)
    {
        std::string cipherFileName = utils.AESDecrypt(std::string(dp->d_name), key, std::string());
        if (match != NULL && strncmp(match, cipherFileName.c_str(), strlen(match) != 0))
        {
            continue;
        }
        std::cout << cipherFileName << " ";
    }
    std::cout << std::endl;
    (void)closedir(dirp);
    return 0;
}