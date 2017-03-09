#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

// http://stackoverflow.com/questions/2048509/how-to-echo-with-different-colors-in-the-windows-command-line/38617204#38617204
// https://msdn.microsoft.com/en-us/library/windows/desktop/mt638032(v=vs.85).aspx

#define CTRC_ARG_VERBOSE L"-v"
#define CTRC_ARG_PAUSE L"-p"
#define CTRC_ARG_SCHEME L"-s"

namespace ctrc
{
	typedef const wchar_t* sz;
	static const char esc = 0x1B;
	static const char def = 0;
	static const wchar_t esc_def[] = { esc, def, 0 };

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	enum colorKey
	{
		COLOR_RED = 0,
		COLOR_GREEN,
		COLOR_YELLOW,
		COLOR_CYAN,
// 		COLOR_GRAY,

		COLOR_MAX
	};

	static const sz colorArg [] =
	{
		L"red",
		L"green",
		L"yellow",
		L"cyan",
// 		L"gray",
	};

	static const wchar_t colorVal [] =
	{
		31,
		32,
		33,
		36,
	};

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	struct scheme
	{
		scheme(colorKey color, sz start): m_color(color), m_start(start)
		{
			assert(color < COLOR_MAX);
			m_seq[0] = esc;
			m_seq[1] = colorVal[color];
			m_seq[2] = 0;
		}
		scheme(colorKey color, sz start, sz end): m_color(color), m_start(start), m_end(end) {}

		colorKey m_color;
		std::wstring m_start;
		std::wstring m_end;
		wchar_t m_seq [3];
	};
}

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
int wmain(int argc, wchar_t *argv[] /*, wchar_t *envp[]*/)
{
	// Test for pause argument
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if((arg != NULL) && (_wcsicmp(arg, CTRC_ARG_PAUSE) == 0))
		{
			system("pause");
			break;
		}
	}

	// Test for verbose argument
	bool verbose = false;
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if((arg != NULL) && (_wcsicmp(arg, CTRC_ARG_VERBOSE) == 0))
		{
			std::wcout << L"verbose mode detected" << std::endl;
			verbose = true;
		}
	}
// 	for(int i = 1; i < argc; ++i)
// 	{
// 		const wchar_t* arg = argv[i];
// 		if(arg == NULL)
// 		{
// 			continue;
// 		}
// 		else if(_wcsicmp(arg, CTRC_ARG_SCHEME) == 0)
// 		{
// 			continue;
// 		}
// 	}
	std::vector<ctrc::scheme> schemes;
	schemes.push_back(ctrc::scheme(ctrc::COLOR_RED, L"error:"));
	schemes.push_back(ctrc::scheme(ctrc::COLOR_YELLOW, L"warning:"));

	if(verbose)
	{
		std::wcout << L"ctrc start" << std::endl;
	}

	std::wstring line;
	while(std::wcin)
	{
		getline(std::wcin, line);

		for(size_t i = 0; i < schemes.size(); ++i)
		{
			const ctrc::scheme& s = schemes[i];
			ctrc::sz schemeStart = s.m_start.c_str();
			assert(schemeStart != NULL);
			std::wstring::size_type startFound = line.find(schemeStart);
			if(startFound != std::wstring::npos)
			{
				line.insert(startFound, ctrc::sz(s.m_seq));

				std::wstring::size_type endFound = std::wstring::npos;
				if(s.m_end.empty() == false)
				{
					ctrc::sz schemeEnd = s.m_end.c_str();
					endFound = line.find(schemeEnd);
				}
				if(endFound == std::wstring::npos)
				{
					line.push_back(ctrc::esc);
					line.push_back(ctrc::def);
					line.push_back(0);
				}
				else
				{
					line.insert(startFound, ctrc::sz(s.m_seq));
				}
			}
			break;
		}
		std::wcout << line.c_str() << std::endl;

		std::wcout << L"ctrc new line" << std::endl;
	};
	std::wcout << L"ctrc finished" << std::endl;

	 return 0;
}
