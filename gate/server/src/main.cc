#include "Common.h"
#include "Version.h"
#include "Gate.h"


void printGateVer(void)
{
    ILOG<<GREEN<<PROJECT_NAME<<" start."NONE;
    ILOG<<GREEN<<"     Programe Version V"<<PROGRAM_VERSION<<NONE;
    ILOG<<GREEN<<"Attention : this version is new gate"<<NONE;
}
void init(void)
{
	
	static GLogHelper glog((char*)PROJECT_NAME);
	printGateVer();
}
int main(int argc,char*argv[])
{
	init();
	Gate main_gate((char*)GATE_SOCKET_PATH,sizeof(GATE_SOCKET_PATH));
	main_gate.run();
}
