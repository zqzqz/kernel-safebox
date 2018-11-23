#ifndef _COMMAND_H
#define _COMMAND_H

#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include "log.h"

typedef int (*CmdProcFunc)(char ** args, int argCnt);
struct cmp_str
{
   bool operator()(char const *a, char const *b) const
   {
      return std::strcmp(a, b) < 0;
   }
};

int test(char ** args, int argCnt);
int put(char ** args, int argCnt);
int get(char ** args, int argCnt);
int list(char ** args, int argCnt);

class Command
{
public:
    std::map<char*, CmdProcFunc, cmp_str> cmdMap;
    Command();
    int ExecCmd(char *mCmdLine);
};

#endif