#include "CLIHandler.h"

CLIHandler::CLIHandler() : Command()
{
    mQuitCmdNum = 2;
    mQuitCmd = new char *[mQuitCmdNum];
    mQuitCmd[0] = "Quit";
    mQuitCmd[1] = "Q";
}

int CLIHandler::IsUserQuitCmd(char *mCmd)
{
    unsigned char mQuitCmdIdx = 0;
    for (; mQuitCmdIdx < mQuitCmdNum; mQuitCmdIdx++)
    {
        if (!strcasecmp(mCmd, mQuitCmd[mQuitCmdIdx]))
            return 1;
    }

    return 0;
}

// remove spaces in commands
char *CLIHandler::StripWhite(char *mOrig)
{
    if (NULL == mOrig)
        return "NUL";

    char *mStripHead = mOrig;
    while (isspace(*mStripHead))
        mStripHead++;

    if ('\0' == *mStripHead)
        return mStripHead;

    char *mStripTail = mStripHead + strlen(mStripHead) - 1;
    while (mStripTail > mStripHead && isspace(*mStripTail))
        mStripTail--;
    *(++mStripTail) = '\0';

    return mStripHead;
}

char *CLIHandler::ReadCmdLine()
{
    if (mLineRead)
    {
        mLineRead = NULL;
    }

    mLineRead = readline(mCmdPrompt);

    mStripLine = StripWhite(mLineRead);
    if (mStripLine && *mStripLine)
        add_history(mStripLine);

    return mStripLine;
}

char *CmdGenerator(const char *mText, int dwState)
{
    Command cmd = Command();
    int dwTextLen = 0;
    if (!dwState)
    {
        dwTextLen = strlen(mText);
    }

    // found matched command history
    std::map<char *, CmdProcFunc, cmp_str>::iterator iter;
    if (dwState >= cmd.cmdMap.size())
    {
        return NULL;
    }
    iter = cmd.cmdMap.begin();
    for (int i = 0; i < dwState; i++)
    {
        iter++;
    }
    for (; iter != cmd.cmdMap.end(); iter++)
    {
        if (!strncmp(iter->first, mText, dwTextLen))
            return strdup(iter->first);
    }

    return NULL;
}

char **CmdCompletion(const char *mText, int dwStart, int dwEnd)
{
    //rl_attempted_completion_over = 1;
    char **pMatches = NULL;
    if (0 == dwStart)
        pMatches = rl_completion_matches(mText, CmdGenerator);

    return pMatches;
}

// initialize command completion
void InitReadLine(void)
{
    rl_attempted_completion_function = CmdCompletion;
}