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
    sStream << fixed << setprecision(1) << tpsns;
    sStream >> tpsns;
    return tpsns;
}

std::string Convert(float Num)
{
    std::ostringstream oss;
    oss << Num;
    std::string str(oss.str());
    return str;
}

bool oncmd_tps(CommandOrigin const& ori, CommandOutput& outp) {
    std::string tpsString = Convert(getTps());
    outp.addMessage("TPS:" + tpsString);
    return true;
}

void entry() {
    //std::cout << "Hello World\n";
    //赋值变量
    long long timeTemp = getTimeStamp();
    time_old = timeTemp;
    time_new = timeTemp;
    printf("[INFO] TPS Loaded. By HuoHuaX\n");

    //注册指令
    Event::addEventListener([](RegCmdEV ev) {
        CMDREG::SetCommandRegistry(ev.CMDRg);
        MakeCommand("tps", "show tps", 0);//注册指令
        CmdOverload(tps, oncmd_tps);//重载指令
        });
}

//tick
THook(void, "?tick@ServerLevel@@UEAAXXZ",
    void* _this) {
    ++tick;
    if (tick == reset_ticks) {
        tick = 0;
        time_old = time_new;
        time_new = getTimeStamp();
    }
    return original(_this);
}

//控制台输入
THook(bool, "??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
    void* _this, std::string* cmd) {
    if(*cmd == "tps") {
        std::string tpsString = Convert(getTps());
        cout << "[INFO] TPS:" << tpsString << '\n';
        return false;
    }
    return original(_this, cmd);
}

