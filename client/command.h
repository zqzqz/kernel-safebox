#ifndef _COMMAND_H
#define _COMMAND_H

#include <iostream>
#include <cstring>
#include <vector>
#include <map>

typedef int (*CmdProcFunc)(char ** args);
struct cmp_str
{
   bool operator()(char const *a, char const *b) const
   {
      return std::strcmp(a, b) < 0;
   }
};

int test(char ** args);
int put(char ** args);
int get(char ** args);
int list(char ** args);

class Command
{
public:
    std::map<char*, CmdProcFunc, cmp_str> cmdMap;
    Command();
    int ExecCmd(char *mCmdLine);
};

#endif