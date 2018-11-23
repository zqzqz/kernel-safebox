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
                ERROR("Execution of command is failed");
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
    if (argCnt != 2)
    {
        ERROR("Unvalid number of arguments (required 2)");
        return 3;
    }
    // fetch arguments
    char *outFileName = *(args);
    char *inFileName = *(args + 1);
    char *password = *(args + 2);
    int res = 0;

    // read input file
    std::ifstream in(outFileName);
    if (!in)
    {
        ERROR("Cannot open file" + std::string(outFileName));
        return 1;
    }
    std::string text_str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    char *text = (char *)text_str.c_str();

    size_t cipherLen = strlen(text);

    // encrypt
    char *cipher = new char[strlen(text)];
    CryptoUtils utils = CryptoUtils();
    if (utils.AESEncrypt(text, cipher, password, NULL) != 0)
    {
        ERROR("AES Encryption failed");
        return 2;
    }
    char *cipherFileName = new char[strlen(inFileName)];
    if (utils.AES_base64_encrypt(inFileName, cipherFileName, password, NULL) != 0)
    {
        ERROR("AES Encryption failed");
        return 2;
    }
    std::string cipher_str = std::string(cipher);

    // write output file
    std::string fullFileName = std::string(BOX_PATH) + std::string(cipherFileName);
    std::ofstream out(fullFileName.c_str());
    if (!out)
    {
        ERROR("Cannot open file" + std::string(inFileName));
        return 1;
    }
    out << cipher_str;

    // clear job
    delete cipher;
    delete cipherFileName;
    return 0;
}

int get(char **args, int argCnt)
{
    if (argCnt != 2)
    {
        ERROR("Unvalid number of arguments (required 2)");
        return 3;
    }
    // fetch arguments
    char *inFileName = *(args);
    char *outFileName = *(args + 1);
    char *password = *(args + 2);
    int res = 0;

    // read input file
    CryptoUtils utils = CryptoUtils();
    char *cipherFileName = new char[strlen(inFileName)];
    if (utils.AES_base64_encrypt(inFileName, cipherFileName, password, NULL) != 0)
    {
        ERROR("AES Encryption failed");
        return 2;
    }
    std::string fullFileName = std::string(BOX_PATH) + std::string(cipherFileName);
    std::ifstream in(fullFileName.c_str());
    if (!in)
    {
        ERROR("Cannot open file" + std::string(inFileName));
        return 1;
    }
    std::string cipher_str((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    char *cipher = (char *)cipher_str.c_str();

    size_t cipherLen = strlen(cipher);

    // edecrypt
    char *text = new char[strlen(cipher)];
    if (utils.AESDecrypt(cipher, text, password, NULL) != 0)
    {
        ERROR("AES Decryption failed");
        return 2;
    }
    std::string text_str = std::string(text);

    // write output file
    std::ofstream out(outFileName);
    if (!out)
    {
        ERROR("Cannot open file" + std::string(outFileName));
        return 1;
    }
    out << text_str;

    // clear job
    delete text;
    delete cipherFileName;
    return 0;
}

int list(char **args, int argCnt)
{
    char *match = NULL;
    if (argCnt > 0) {
        match = *(args);
    }
    DIR* dirp;
    struct dirent *dp;
    dirp = opendir(BOX_PATH);
    while ((dp = readdir(dirp)) != NULL) {
        if (match != NULL && strncmp(match, dp->d_name, strlen(match) != 0)) {
            continue;
        }
        std::cout<<dp->d_name<<" ";
    }
    std::cout<<std::endl;
    (void)closedir(dirp);
    return 0;
}