#include "pch.h"
#include <mc/Player.h>
#include <time.h>
#include <sys/timeb.h>
#include <mc/Command.h>
#include <mc/CommandReg.h>
#include <api/regCommandHelper.h>
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;
//全局变量
int tick = 0;
const float reset_ticks = 20.0;
long long time_old = 0;
long long time_new = 0;


long long getTimeStamp()
{
    timeb t;
    ftime(&t);
    return t.time * 1000 + t.millitm;
}

float getTps() {
    float w = 1000.0;
    float tpsns = w * (reset_ticks / (time_new - time_old));
    stringstream sStream;
    sStream << fixed << setprecision(2) << tpsns;
    sStream >> tpsns;
    if (tpsns > 20) {
        tpsns = 20.0;
    }
    return tpsns;
}

float getSelfTps(int Num) {
    float w = 1000.0;
    float tpsns = w * (reset_ticks / (time_new - time_old));
    stringstream sStream;
    if (Num >= 0) {
        sStream << fixed << setprecision(Num) << tpsns;
        sStream >> tpsns;
    }
    if (tpsns > 20) {
        tpsns = 20.0;
    }
    return tpsns;
}

std::string Convert(float Num)
{
    std::ostringstream oss;
    oss << Num;
    std::string str(oss.str());
    return str;
}

bool oncmd_tps(CommandOrigin const& ori, CommandOutput& outp, optional<int>& str2) {
    if (str2.set) {
        std::string tpsString = Convert(getSelfTps(str2.val()));
        outp.addMessage("[INFO] TPS:" + tpsString);
    }
    else {
        std::string tpsString = Convert(getTps());
        outp.addMessage("[INFO] TPS:" + tpsString);
    }
    return true;
}

void entry() {
    //赋值变量
    long long timeTemp = getTimeStamp();
    time_old = timeTemp;
    time_new = timeTemp;
    printf("[INFO] TPS Loaded. By HuoHuaX\n");

    //注册指令
    Event::addEventListener([](RegCmdEV ev) {
        CMDREG::SetCommandRegistry(ev.CMDRg);
        MakeCommand("tps", "show tps", 0);//注册指令
        CmdOverload(tps, oncmd_tps,"point");//重载指令
        });
}

//tick
THook(void, "?tick@ServerLevel@@UEAAXXZ",
    void* _this) {
    try {
        ++tick;
        if (tick == reset_ticks) {
            tick = 0;
            time_old = time_new;
            time_new = getTimeStamp();
            //cout << time_old << " " << time_new << " " << time_new-time_old <<"\n";
        }
    }
    catch (const char*& e) {
        ERROR(e);
    }
    
    return original(_this);
}

