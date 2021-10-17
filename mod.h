#pragma once
#include <stdio.h>
using namespace std;
//输出日志
#define ERROR(t){printf(t+'\n')};
#define TPS(tick){printf("TPS: "+char(tick)+'\n')}

//全局
extern int tick；
