#include "Configs.h"
#include "TestApp.h"

using namespace app;
using namespace net;

int main(int argc, char** argv)
{
	if (CheckCPUendian())
		std::cout << " big-endian  " << std::endl;
	else
		std::cout << " little-endian  " << std::endl;

	SSeting st1 = {
		1,1,"",9802,"server1"
	};
	SSeting st2 = {
		2,2,"",9803,"server2"
	};
	if (argc == 2) 
	{
		if (argv[1] == "1")
		{

		}
		else if (argv[1] == "2")
		{

		}
	}
	return 0;
}