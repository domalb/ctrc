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
#define CTRC_ARG_DEFAULT L"-d="
static const size_t CTRC_ARG_DEFAULT_LENGTH = CTRC_STRING_LENGTH(CTRC_ARG_DEFAULT);
#define CTRC_ARG_TAG L"-c="
static const size_t CTRC_ARG_TAG_LENGTH = CTRC_STRING_LENGTH(CTRC_ARG_TAG);
#define CTRC_ARG_SEPARATOR L"-s="
static const size_t CTRC_ARG_SEPARATOR_LENGTH = CTRC_STRING_LENGTH(CTRC_ARG_SEPARATOR);

bool verbose = false;
#define CTRC_LOG_INFO(x_msg) if(verbose) { std::wcout << L"CTRC: " << x_msg << std::endl; }
#define CTRC_LOG_ERROR(x_msg) if(verbose) { std::wcerr << L"CTRC: " << x_msg << std::endl; }

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
		COLOR_BLACK = 0,
		COLOR_DARK_RED,
		COLOR_DARK_GREEN,
		COLOR_DARK_YELLOW,
		COLOR_DARK_BLUE,
		COLOR_DARK_MAGENTA,
		COLOR_DARK_CYAN,
		COLOR_DARK_GREY,
		COLOR_LIGHT_GREY,
		COLOR_LIGHT_RED,
		COLOR_LIGHT_GREEN,
		COLOR_LIGHT_YELLOW,
		COLOR_LIGHT_BLUE,
		COLOR_LIGHT_MAGENTA,
		COLOR_LIGHT_CYAN,
		COLOR_WHITE,

		COLOR_MAX
	};

	static const sz colorName [] =
	{
		L"black",
		L"d_red",
		L"d_green",
		L"d_yellow",
		L"d_blue",
		L"d_magenta",
		L"d_cyan",
		L"d_gray",
		L"gray",
		L"red",
		L"green",
		L"yellow",
		L"blue",
		L"magenta",
		L"cyan",
		L"white",
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
		// black
		0,
		// d_red
		FOREGROUND_RED,
		// d_green
		FOREGROUND_GREEN,
		// d_yellow
		FOREGROUND_RED | FOREGROUND_GREEN,
		// d_blue
		FOREGROUND_BLUE,
		// d_magenta
		FOREGROUND_RED | FOREGROUND_BLUE,
		// d_cyan
		FOREGROUND_BLUE | FOREGROUND_GREEN,
		// d_gray
		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN,
		// l_gray
		FOREGROUND_INTENSITY,
		// red
		FOREGROUND_INTENSITY | FOREGROUND_RED,
		// green
		FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		// yellow
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		// blue
		FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		// magenta
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		// cyan
		FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN,
		// white
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN,
	};

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	struct tag
	{
		tag(colorKey color, sz start): m_colorKey(color), m_start(start)
		{
			assert(color < COLOR_MAX);
			m_seq[0] = esc;
			m_seq[1] = colorVal[color];
			m_seq[2] = 0;
			m_attribs = colorConsoleAttrib[color];
		}
		tag(colorKey color, sz start, sz end): m_colorKey(color), m_start(start), m_end(end) {}

		colorKey m_colorKey;
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
				CTRC_LOG_INFO(L"invalid argument length");
				return -1;
			}
			else if(val[argLength - 1] != L'"')
			{
				CTRC_LOG_INFO(L"quote detection error for argument " << arg);
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
			CTRC_LOG_INFO(L"verbose mode detected");
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
				CTRC_LOG_INFO(L"invalid separator : " << arg);
				return -1;
			}
			else
			{
				CTRC_LOG_INFO(L"separtor : " << newSeparator);
				ctrc::tagSeparator = newSeparator;

			}
		}
	}

	// Test for default color
	static const WORD invalidConsoleAttrib = WORD(-1);
	WORD defaultConsoleAttrib = invalidConsoleAttrib;
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if((arg != NULL) && (_wcsnicmp(arg, CTRC_ARG_DEFAULT, CTRC_ARG_DEFAULT_LENGTH) == 0))
		{
			ctrc::sz argVal = arg + CTRC_ARG_SEPARATOR_LENGTH;
			for(size_t j = 0; j < ctrc::COLOR_MAX; ++j)
			{
				if(_wcsicmp(argVal, ctrc::colorName[j]) == 0)
				{
					defaultConsoleAttrib = ctrc::colorConsoleAttrib[j];
					break;
				}
			}
		}
	}

	// Get initial console text attributes
	HANDLE stdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if(stdOut == INVALID_HANDLE_VALUE)
	{
		CTRC_LOG_INFO(L"could not get output handle");
		return  -1;
	}
	CONSOLE_SCREEN_BUFFER_INFO info;
	memset(&info, 0, sizeof(info));
	BOOL getInfo = GetConsoleScreenBufferInfo(stdOut, &info);
	if(getInfo == FALSE)
	{
		CTRC_LOG_INFO(L"could not get consule screen buffer info");
		return  -1;
	}
	WORD initConsoleAttribs = info.wAttributes;

	// Detect schemes
	std::vector<ctrc::tag> tags;
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
						tags.push_back(ctrc::tag(ctrc::colorKey(j), start));
						CTRC_LOG_INFO(L"tag : " << colorArgVal << L" : " << start);
					}
				}
			}
		}
	}

	CTRC_LOG_INFO(L"start");

	std::wstring line;
	while(std::wcin)
	{
		getline(std::wcin, line);
		CTRC_LOG_INFO(L"new line");

		WORD consoleAttrib = defaultConsoleAttrib;
		std::wstring::size_type startFound = std::wstring::npos;
		for(size_t i = 0; i < tags.size(); ++i)
		{
			const ctrc::tag& s = tags[i];
			startFound = line.find(s.m_start.c_str());
			if(startFound != std::wstring::npos)
			{
				CTRC_LOG_INFO(L"tag found : " << ctrc::colorName[s.m_colorKey]);
				consoleAttrib = s.m_attribs;
				break;
			}
		}
		if(consoleAttrib != invalidConsoleAttrib)
		{
			SetConsoleTextAttribute(stdOut, consoleAttrib);
			std::wcout << line.c_str() << std::endl;
			SetConsoleTextAttribute(stdOut, initConsoleAttribs);
		}
		else
		{
			std::wcout << line.c_str() << std::endl;
		}
	}
	CTRC_LOG_INFO(L"finished");

	 return 0;
}
