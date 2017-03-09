#include <windows.h>
#include <iostream>
#include <string>
#include <assert.h>

//
//----------------------------------------------------------------------------------------------------------------------
int wmain(int, wchar_t *[] /*, wchar_t *envp[]*/)
{
	std::wcout << L"hello world!" << std::endl;
	std::wcout << L"error: this is bad" << std::endl;
	std::wcout << L"encountered an error: this is bad. hoping for no other error." << std::endl;
	std::wcout << L"warning: reaching the end." << std::endl;
	std::wcout << L"trace: finished!" << std::endl;

	 return 0;
}
