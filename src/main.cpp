#include <windows.h>
#include <iostream>
#include <string>

// http://stackoverflow.com/questions/2048509/how-to-echo-with-different-colors-in-the-windows-command-line/38617204#38617204

typedef const wchar_t* sz;

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
enum cadbColor
{
	CADB_RED = 0,
	CADB_GREEN,
	CADB_YELLOW,
	CADB_CYAN,
};

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
struct key
{
	std::wstring start;
	std::wstring end;
	char color;
};

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
//int wmain(int argc, wchar_t *argv[] /*, wchar_t *envp[]*/)
int wmain(int, wchar_t *[])
{
	std::vector<std::wstring>
	// https://msdn.microsoft.com/en-us/library/windows/desktop/mt638032(v=vs.85).aspx
	static const char esc = 0x1B;
	static const char def = 0;
	static const char red = 31;
	static const char green = 32;
	static const char yellow = 33;
	static const char cyan = 36;

	static const wchar_t esc_yellow[] = { esc, yellow, 0 };
	static const wchar_t esc_def[] = { esc, def, 0 };

	std::wcout << L"cadb start" << std::endl;

	std::wstring line;
	while(std::wcin)
	{
		getline(std::wcin, line);

		std::wstring::size_type foundI = line.find(L" I ");
		if(foundI != std::wstring::npos)
		{
			line.insert(foundI, sz(esc_yellow));
			line.push_back(esc);
			line.push_back(def);
		}
		else if(line == L"- waiting for device -")
		{
			std::wcout << sz(esc_yellow) << L"-- waiting for device --" << sz(esc_def) << std::endl;

		}
		std::wcout << line.c_str() << std::endl;
		std::wcout << L"cadb new line" << std::endl;
	};
	std::wcout << L"cadb finished" << std::endl;

	 return 0;
}
