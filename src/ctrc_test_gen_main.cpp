#include <windows.h>
#include <iostream>
#include <string>
#include <assert.h>

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
int wmain(int, char *[])
{
	std::cout << "hello world!" << std::endl;
	std::cout << "error: this is bad" << std::endl;
	std::cout << "encountered an error: this is bad. hoping for no other error." << std::endl;
	std::cout << "warning: reaching the end." << std::endl;
	std::cout << "trace: finished!" << std::endl;

	 return 0;
}
