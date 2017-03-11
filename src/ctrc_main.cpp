#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

// https://en.wikipedia.org/wiki/ANSI_escape_code#Windows_and_DOS
// http://stackoverflow.com/questions/2048509/how-to-echo-with-different-colors-in-the-windows-command-line/38617204#38617204
// https://msdn.microsoft.com/en-us/library/windows/desktop/mt638032(v=vs.85).aspx

#define CTRC_ARRAY_LENGTH(val) (sizeof(val) / sizeof(val[0]))
#define CTRC_STRING_LENGTH(val) (CTRC_ARRAY_LENGTH(val) - 1)

#define CTRC_ARG_VERBOSE L"-v"
#define CTRC_ARG_PAUSE L"-p"
#define CTRC_ARG_TAG L"-t="
static const size_t CTRC_ARG_TAG_LENGTH = CTRC_STRING_LENGTH(CTRC_ARG_TAG);
#define CTRC_ARG_SEPARATOR L"-s="
static const size_t CTRC_ARG_SEPARATOR_LENGTH = CTRC_STRING_LENGTH(CTRC_ARG_SEPARATOR);

bool verbose = false;
#define CTRC_LOG(x_msg) if(verbose) { std::wcout << L"CTRC: " << x_msg << std::endl; }

namespace ctrc
{
	typedef const wchar_t* sz;
	static const char esc = 0x1B;
	static const char def = 0;
	static const wchar_t esc_def[] = { esc, def, 0 };

	wchar_t tagSeparator = L'~';

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

	static const sz colorName [] =
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

	static const WORD colorConsoleAttrib [] =
	{
		// red
		FOREGROUND_INTENSITY | FOREGROUND_RED,
		// green
		FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		// yellow
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		// cyan
		FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	struct tag
	{
		tag(colorKey color, sz start): m_color(color), m_start(start)
		{
			assert(color < COLOR_MAX);
			m_seq[0] = esc;
			m_seq[1] = colorVal[color];
			m_seq[2] = 0;
			m_attribs = colorConsoleAttrib[color];
		}
		tag(colorKey color, sz start, sz end): m_color(color), m_start(start), m_end(end) {}

		colorKey m_color;
		std::wstring m_start;
		std::wstring m_end;
		wchar_t m_seq [3];
		WORD m_attribs;
	};

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	int getUnquoted(sz arg, wchar_t* buffer)
	{
		const wchar_t* val = arg;

		bool quotes = (val[0] == L'"');
		if(quotes)
		{
			++val;
		}

		wcscpy_s(buffer, MAX_PATH, val);

		if(quotes)
		{
			size_t argLength = wcslen(val);
			if(argLength < 2)
			{
				CTRC_LOG(L"invalid argument length");
				return -1;
			}
			else if(val[argLength - 1] != L'"')
			{
				CTRC_LOG(L"quote detection error for argument " << arg);
				return -1;
			}
			else
			{
				buffer[argLength - 1] = 0;
			}
		}

		return 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------------
int wmain(int argc, wchar_t *argv[] /*, wchar_t *envp[]*/)
{
	size_t colorArgValLengths[ctrc::COLOR_MAX];
	assert(CTRC_ARRAY_LENGTH(ctrc::colorName) == CTRC_ARRAY_LENGTH(colorArgValLengths));
	for(size_t i = 0; i < ctrc::COLOR_MAX; ++i)
	{
		colorArgValLengths[i] = wcslen(ctrc::colorName[i]);
	}

	// Test for pause argument
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if((arg != NULL) && (_wcsicmp(arg, CTRC_ARG_PAUSE) == 0))
		{
			static int j = 0;
			while(true)
			{
				++j;
			}
			break;
		}
	}

	// Test for verbose argument
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if((arg != NULL) && (_wcsicmp(arg, CTRC_ARG_VERBOSE) == 0))
		{
			verbose = true;
			CTRC_LOG(L"verbose mode detected");
		}
	}

	// Test for separator
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if((arg != NULL) && (_wcsnicmp(arg, CTRC_ARG_SEPARATOR, CTRC_ARG_SEPARATOR_LENGTH) == 0))
		{
			wchar_t newSeparator = arg[CTRC_ARG_SEPARATOR_LENGTH];
			if(newSeparator == 0)
			{
				CTRC_LOG(L"invalid separator : " << arg);
				return -1;
			}
			else
			{
				CTRC_LOG(L"separtor : " << newSeparator);
				ctrc::tagSeparator = newSeparator;

			}
		}
	}

	HANDLE stdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if(stdOut == INVALID_HANDLE_VALUE)
	{
		CTRC_LOG(L"could not get output handle");
		return  -1;
	}
	CONSOLE_SCREEN_BUFFER_INFO info;
	memset(&info, 0, sizeof(info));
	BOOL getInfo = GetConsoleScreenBufferInfo(stdOut, &info);
	if(getInfo == FALSE)
	{
		CTRC_LOG(L"could not get consule screen buffer info");
		return  -1;
	}
	WORD defaultAttribs = info.wAttributes;

	// Detect schemes
	std::vector<ctrc::tag> schemes;
// 	schemes.push_back(ctrc::scheme(ctrc::COLOR_RED, L"error:"));
// 	schemes.push_back(ctrc::scheme(ctrc::COLOR_YELLOW, L"warning:"));
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if(arg == NULL)
		{
			continue;
		}
		else if(_wcsnicmp(arg, CTRC_ARG_TAG, CTRC_ARG_TAG_LENGTH) == 0)
		{
			wchar_t argVal [MAX_PATH] = { 0 };
			int unquote = ctrc::getUnquoted(arg + CTRC_ARG_TAG_LENGTH, argVal);
			if(unquote != 0)
			{
				return unquote;
			}
			for(size_t j = 0; j < ctrc::COLOR_MAX; ++j)
			{
				ctrc::sz colorArgVal = ctrc::colorName[j];
				size_t colorArgValLength = colorArgValLengths[j];
				if((_wcsnicmp(argVal, colorArgVal, colorArgValLength) == 0) && (argVal[colorArgValLength] == ctrc::tagSeparator))
				{
					ctrc::sz start = argVal + colorArgValLength + 1;
					if(start[0] != 0)
					{
						schemes.push_back(ctrc::tag(ctrc::colorKey(j), start));
						CTRC_LOG(L"scheme : " << colorArgVal << L" : " << start);
					}
				}
			}
		}
	}

	CTRC_LOG(L"start");

	std::wstring line;
	while(std::wcin)
	{
		getline(std::wcin, line);

		for(size_t i = 0; i < schemes.size(); ++i)
		{
			const ctrc::tag& s = schemes[i];
			std::wstring::size_type startFound = line.find(s.m_start.c_str());
			if(startFound != std::wstring::npos)
			{
// 				std::wstring::size_type endFound = std::wstring::npos;
// 				if(s.m_end.empty() == false)
// 				{
// 					endFound = line.find(s.m_end.c_str());
// 				}
// 
// 				if(startFound != 0)
// 				{
// 					printf("%.")
// 				}
				SetConsoleTextAttribute(stdOut, s.m_attribs);
				std::wcout << line.c_str() << std::endl;
				SetConsoleTextAttribute(stdOut, defaultAttribs);
				
				break;
			}
		}

		CTRC_LOG(L"new line");
	}
	CTRC_LOG(L"finished");

	 return 0;
}
