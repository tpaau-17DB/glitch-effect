#include <iostream>

#include "../src/Logger.h"

using namespace std;

int main()
{
    cout<<"----------test start----------"<<endl<<endl;
    
    for (int i = 0; i < 4; i++)
    {
        cout<<"Testing logging for verbosity = "<<i<<endl;
        Logger::SetVerbosity(i);

        Logger::PrintDebug("debug");
        Logger::PrintDebug("debug", 1);
        Logger::PrintLog("log");
        Logger::PrintLog("log", 1);
        Logger::PrintWarn("warning");
        Logger::PrintWarn("warning", 1);
        Logger::PrintErr("error");
        Logger::PrintErr("error", 1);

        cout<<endl;
    }
    
    cout<<"----------test end----------"<<endl;

    return 0;
}
