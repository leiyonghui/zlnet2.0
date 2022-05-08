#ifdef _WIN32

#include <stdio.h>
#include "network/Network.h"
#include "network/Common.h"
#include <iostream>
#include <windows.h>
#pragma comment(lib, "wsock32.lib")
using namespace network;
using namespace std;

int main()
{
	core_log_trace(hostToNetwork32(1), ::htonl(1), hostToNetwork16(1), ::htons(1));
	core_log_trace(networkToHost32(1), ::ntohl(1), networkToHost16(1), ::ntohs(1));
	system("pause");
	return 0;
}

#endif //  _WIN32

