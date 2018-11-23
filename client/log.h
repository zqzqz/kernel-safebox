#ifndef LOG_H
#define LOG_H
#include <iostream>
#define LOG(msg) std::cout<<"[LOG] ("<<__FILE__<<":"<<__LINE__<<")"<<msg<<std::endl;
#define ERROR(msg) std::cout<<"[ERROR] ("<<__FILE__<<":"<<__LINE__<<")"<<msg<<std::endl;
#endif // LOG_H