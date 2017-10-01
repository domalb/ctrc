#if defined _XBOX
#	define CCTX_PLATFORM_X360
#	define CCTX_BIG_ENDIAN
#	define CCTX_PTR_SIZE 4
#elif defined _DURANGO
#	define CCTX_PLATFORM_XONE
#	define CCTX_LITTLE_ENDIAN
#	define CCTX_PTR_SIZE 8
#elif defined WIN32
#	define CCTX_PLATFORM_WINDOWS
#	define CCTX_LITTLE_ENDIAN
#	ifdef _WIN64
#		define CCTX_PLATFORM_WINDOWS_X64
#		define CCTX_PTR_SIZE 8
#	else //_WIN64
#		define CCTX_PLATFORM_WINDOWS_X86
#		define CCTX_PTR_SIZE 4
#	endif // _WIN64
#elif defined __DOS__
#	define CCTX_PLATFORM_DOS
#	define CCTX_LITTLE_ENDIAN
#elif defined __ANDROID__
#	define CCTX_PLATFORM_ANDROID
#	define CCTX_LITTLE_ENDIAN
#	ifdef __ARM_ARCH
#		define CCTX_PLATFORM_ANDROID_ARM
#		if defined __ARM_ARCH_7A__
#			define CCTX_PLATFORM_ANDROID_ARM7A
#			define CCTX_PTR_SIZE 4
#		elif defined __ARM_ARCH_7S__
#			define CCTX_PLATFORM_ANDROID_ARM7S
#			define CCTX_PTR_SIZE 4
#		elif defined __ARM_ARCH_ISA_A64
#			define CCTX_PLATFORM_ANDROID_ARMA64
#			define CCTX_PTR_SIZE 8
#		else // __ARM_ARCH_XXX
#			error unsupported Android ARM platform
#		endif // __ARM_ARCH_XXX
#	elif defined __i386__ // or __i386 or i386
#		define CCTX_PLATFORM_ANDROID_INTEL
#		define CCTX_PLATFORM_ANDROID_X86
#		define CCTX_PTR_SIZE 4
#	elif defined __amd64__ // or __amd64
#		define CCTX_PLATFORM_ANDROID_INTEL
#		define CCTX_PLATFORM_ANDROID_X64
#		define CCTX_PTR_SIZE 8
#	else // Android architecture
#		error unsupported Android architecture
#	endif // Android architecture
#elif defined __gnu_linux__ // or __linux
	// Warning Android is considered to be Linux, so Android test must be kept higher
#	define CCTX_PLATFORM_LINUX
#	define CCTX_LITTLE_ENDIAN
#	ifdef __LP64__
#		define CCTX_PLATFORM_LINUX_X64
#		define CCTX_PTR_SIZE 8
#	else // __LP64__
#		define CCTX_PLATFORM_LINUX_X32
#		define CCTX_PTR_SIZE 4
#	endif // __LP64__
#elif defined ARM9
#	define CCTX_PLATFORM_DS
#	define CCTX_PTR_SIZE 4
#elif defined SN_TARGET_PS3
#	define CCTX_PLATFORM_PS3
#	define CCTX_PLATFORM_PS3_PPU
#	define CCTX_BIG_ENDIAN
#	define CCTX_PTR_SIZE 4
#elif defined SN_TARGET_PS3_SPU
#	define CCTX_PLATFORM_PS3
#	define CCTX_PLATFORM_PS3_SPU
#	define CCTX_BIG_ENDIAN
#	define CCTX_PTR_SIZE 4
#elif defined __ORBIS__
#	define CCTX_PLATFORM_PS4
#	define CCTX_LITTLE_ENDIAN
#	define CCTX_PTR_SIZE 8
#elif defined __APPLE_CC__
//#	define CCTX_APPLE
#	include <TargetConditionals.h>
#	if TARGET_RT_LITTLE_ENDIAN
#		define CCTX_LITTLE_ENDIAN
#	elif TARGET_RT_BIG_ENDIAN
#		define CCTX_BIG_ENDIAN
#	endif // TARGET_RT_LITTLE_ENDIAN
#	if TARGET_RT_64_BIT
#		define CCTX_PTR_SIZE 8
#	else // TARGET_RT_64_BIT
#		define CCTX_PTR_SIZE 4
#	endif // TARGET_RT_64_BIT
#	if TARGET_OS_IPHONE
#		define CCTX_IOS
#		define CCTX_IOS_HACK
#		if TARGET_IPHONE_SIMULATOR
#			define CCTX_IOS_SIMULATOR
#			if TARGET_CPU_X86
#				define CCTX_PLATFORM_IOS_SIMULATOR_X86
#			elif TARGET_CPU_X86_64
#				define CCTX_PLATFORM_IOS_SIMULATOR_X64
#			elif TARGET_CPU_PPC
#				define CCTX_PLATFORM_IOS_SIMULATOR_PPC32
#			elif TARGET_CPU_PPC64
#				define CCTX_PLATFORM_IOS_SIMULATOR_PPC64
#			else // TARGET_CPU
#				error unsupported IPhone simulator platform
#			endif // TARGET_CPU
#		elif TARGET_PLATFORM_IPHONE
#			define CCTX_PLATFORM_IOS_DEVICE
#			if defined __ARM_ARCH_7A__
#				define CCTX_PLATFORM_IOS_DEVICE_ARM7A
#			elif defined __ARM_ARCH_7S__
#				define CCTX_PLATFORM_IOS_DEVICE_ARM7S
#			elif defined __aarch64__
#				define CCTX_PLATFORM_IOS_DEVICE_ARM64
#			else // __ARM_ARCH_XXX
#				error unsupported IOS device platform
#			endif // __ARM_ARCH_XXX
#		endif // TARGET_IPHONE_SIMULATOR
#	elif TARGET_OS_MAC
#		define CCTX_PLATFORM_MAC
#		if TARGET_CPU_X86
#			define CCTX_PLATFORM_MAC_X86
#			define CCTX_PLATFORM_MAC_INTEL
#		elif TARGET_CPU_X86_64
#			define CCTX_PLATFORM_MAC_X64
#			define CCTX_PLATFORM_MAC_INTEL
#		elif TARGET_CPU_PPC
#			define CCTX_PLATFORM_MAC_PPC32
#			define CCTX_PLATFORM_MAC_PPC
#		elif TARGET_CPU_PPC64
#			define CCTX_PLATFORM_MAC_PPC64
#			define CCTX_PLATFORM_MAC_PPC
#		else // TARGET_CPU
#			error unsupported Mac platform
#		endif // TARGET_CPU
#	else // TARGET_OS
#		error unsupported Apple platform
#	endif // TARGET_OS
#else
#	error unsupported plateform
#endif

#if defined CCTX_PLATFORM_WINDOWS
#   define CCTX_MAX_PATH MAX_PATH
#else // CCTX_PLATFORM_XXX
#   define CCTX_MAX_PATH PATH_MAX
#endif // CCTX_PLATFORM_XXX

#if defined CCTX_PLATFORM_MACOS
#elif defined CCTX_PLATFORM_WINDOWS
#   include <windows.h>
#   define wcscasecmp _wcsicmp
#   define wcsncasecmp _wcsnicmp
#endif // CCTX_PLATFORM_XXX

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

	static const wchar_t tagDefaultSeparator = L'~';
	wchar_t tagSeparator = tagDefaultSeparator;

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

	static const size_t invalidColorKey = size_t(ctrc::COLOR_MAX);

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
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

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	sz getColorName(size_t key)
	{
		assert(key < COLOR_MAX);
		assert(CTRC_ARRAY_LENGTH(colorName) == COLOR_MAX);

		return colorName[key];
	}

#if defined CCTX_PLATFORM_WINDOWS
    
	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	static const wchar_t winColorVal [] =
	{
		// black
		30,
		// d_red
		31,
		// d_green
		32,
		// d_yellow
		33,
		// d_blue
		34,
		// d_magenta
		35,
		// d_cyan
		36,
		// d_gray
		37,
		// l_gray
		90,
		// red
		91,
		// green
		92,
		// yellow
		93,
		// blue
		94,
		// magenta
		95,
		// cyan
		96,
		// white
		97,
	};

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	wchar_t getWinColorVal(size_t key)
	{
		assert(key < COLOR_MAX);
		assert(CTRC_ARRAY_LENGTH(winColorVal) == COLOR_MAX);

		return winColorVal[key];
	}

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	static const WORD winColorConsoleAttrib [] =
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
	WORD getWinColorConsoleAttrib(size_t key)
	{
		assert(key < COLOR_MAX);
		assert(CTRC_ARRAY_LENGTH(winColorConsoleAttrib) == COLOR_MAX);

		return winColorConsoleAttrib[key];
	}

#elif defined CCTX_PLATFORM_MACOS

	static const sz ansiResetColorEsSeq = "\033[0m";

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	static const sz ansiColorEscapeSeq[] =
	{
		"\033[30m",
		"\033[31m",
		"\033[32m",
		"\033[33m",
		"\033[34m",
		"\033[35m",
		"\033[36m",
		"\033[37m",
		"\033[30;1m",
		"\033[31;1m",
		"\033[32;1m",
		"\033[33;1m",
		"\033[34;1m",
		"\033[35;1m",
		"\033[36;1m",
		"\033[37;1m",
	};

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	sz getAnsiEscapeSequence(size_t key)
	{
		assert(key < COLOR_MAX);
		assert(CTRC_ARRAY_LENGTH(ansiColorEscapeSeq) == COLOR_MAX);

		return ansiColorEscapeSeq[key];
	}

#endif // CCTX_PLATFORM_XXX

	//----------------------------------------------------------------------------------------------------------------------
	//
	//----------------------------------------------------------------------------------------------------------------------
	struct tag
	{
		tag(colorKey key, sz start) : m_colorKey(key), m_start(start)
		{
			assert(key < COLOR_MAX);
			m_seq[0] = esc;
			m_seq[1] = getWinColorVal(key);
			m_seq[2] = 0;
		}

		colorKey m_colorKey;
		std::wstring m_start;
		std::wstring m_end;
		wchar_t m_seq[3];
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

#ifdef CCTX_PLATFORM_WINDOWS
		wcscpy_s(buffer, CCTX_MAX_PATH, val);
#else // CCTX_PLATFORM_XXX
		wcscpy(buffer, val);
#endif // CCTX_PLATFORM_XXX

		if(quotes)
		{
			size_t argLength = wcslen(val);
			if(argLength < 2)
			{
				CTRC_LOG_ERROR(L"invalid argument length");
				return -1;
			}
			else if(val[argLength - 1] != L'"')
			{
				CTRC_LOG_ERROR(L"quote detection error for argument " << arg);
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
		colorArgValLengths[i] = wcslen(ctrc::getColorName(i));
	}

	// Test for pause argument
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if((arg != NULL) && (wcscasecmp(arg, CTRC_ARG_PAUSE) == 0))
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
		if((arg != NULL) && (wcscasecmp(arg, CTRC_ARG_VERBOSE) == 0))
		{
			verbose = true;
			CTRC_LOG_INFO(L"verbose mode detected");
			break;
		}
	}

	// Test for separator
	for(int i = 1; i < argc; ++i)
	{
        const wchar_t* arg = argv[i];
		if((arg != NULL) && (wcsncasecmp(arg, CTRC_ARG_SEPARATOR, CTRC_ARG_SEPARATOR_LENGTH) == 0))
		{
			wchar_t newSeparator = arg[CTRC_ARG_SEPARATOR_LENGTH];
			if(newSeparator == 0)
			{
				CTRC_LOG_ERROR(L"invalid separator : " << arg);
				return -1;
			}
			else
			{
				CTRC_LOG_INFO(L"separtor : " << newSeparator);
				ctrc::tagSeparator = newSeparator;
				break;

			}
		}
	}

	// Test for default color
	size_t defaultColorKey = ctrc::invalidColorKey;
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if((arg != NULL) && (wcsncasecmp(arg, CTRC_ARG_DEFAULT, CTRC_ARG_DEFAULT_LENGTH) == 0))
		{
			ctrc::sz argVal = arg + CTRC_ARG_SEPARATOR_LENGTH;
			for(size_t j = 0; j < ctrc::COLOR_MAX; ++j)
			{
				if(wcscasecmp(argVal, ctrc::colorName[j]) == 0)
				{
					defaultColorKey = j;
					break;
				}
			}
		}
	}

	// Detect tags
	std::vector<ctrc::tag> tags;
	for(int i = 1; i < argc; ++i)
	{
		const wchar_t* arg = argv[i];
		if(arg == NULL)
		{
			continue;
		}
		else if(wcsncasecmp(arg, CTRC_ARG_TAG, CTRC_ARG_TAG_LENGTH) == 0)
		{
			wchar_t argVal [MAX_PATH] = { 0 };
			int unquote = ctrc::getUnquoted(arg + CTRC_ARG_TAG_LENGTH, argVal);
			if(unquote != 0)
			{
				return unquote;
			}
			for(size_t j = 0; j < ctrc::COLOR_MAX; ++j)
			{
				ctrc::sz argValColorName = ctrc::getColorName(j);
				size_t colorArgValLength = colorArgValLengths[j];
				if((wcsncasecmp(argVal, argValColorName, colorArgValLength) == 0) && (argVal[colorArgValLength] == ctrc::tagSeparator))
				{
					ctrc::sz start = argVal + colorArgValLength + 1;
					if(start[0] != 0)
					{
						tags.push_back(ctrc::tag(ctrc::colorKey(j), start));
						CTRC_LOG_INFO(L"tag : " << argValColorName << L" : " << start);
					}
				}
			}
		}
	}

#if defined CCTX_PLATFORM_WINDOWS

	// Get initial console text attributes
	HANDLE stdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if(stdOut == INVALID_HANDLE_VALUE)
	{
		CTRC_LOG_ERROR(L"could not get output handle");
		return  -1;
	}
	CONSOLE_SCREEN_BUFFER_INFO info;
	memset(&info, 0, sizeof(info));
	BOOL getInfo = ::GetConsoleScreenBufferInfo(stdOut, &info);
	if(getInfo == FALSE)
	{
		CTRC_LOG_ERROR(L"could not get consule screen buffer info");
		return  -1;
	}
	WORD initConsoleAttribs = info.wAttributes;

#elif defined CCTX_PLATFORM_MAC

#endif // CCTX_PLATFORM_XXX

	CTRC_LOG_INFO(L"start");

	std::wstring line;
	while(std::wcin)
	{
		std::getline(std::wcin, line);
		CTRC_LOG_INFO(L"new line");

		size_t lineColorKey = defaultColorKey;
		for(size_t i = 0; i < tags.size(); ++i)
		{
			const ctrc::tag& s = tags[i];
			std::wstring::size_type startFound = line.find(s.m_start.c_str());
			if(startFound != std::wstring::npos)
			{
				CTRC_LOG_INFO(L"tag found : " << ctrc::colorName[s.m_colorKey]);
				lineColorKey = s.m_colorKey;
				break;
			}
		}

#if defined CCTX_PLATFORM_WINDOWS

		WORD lineConsoleAttrib = ctrc::getWinColorConsoleAttrib(lineColorKey);
		if(lineConsoleAttrib != initConsoleAttribs)
		{
			::SetConsoleTextAttribute(stdOut, lineConsoleAttrib);
			std::wcout << line.c_str() << std::endl;
			::SetConsoleTextAttribute(stdOut, initConsoleAttribs);
		}
		else
		{
			std::wcout << line.c_str() << std::endl;
		}

#elif defined CCTX_PLATFORM_MAC

		if(lineColorKey != defaultColorKey)
		{
			sz ansiLineColorEscSeq = getAnsiEscapeSequence(lineColorKey);
			std::wcout << ansiLineColorEscSeq;
			std::wcout << line.c_str();
			std::wcout << ansiResetColorEsSeq << std::endl;
		}
		else
		{
			std::wcout << line.c_str() << std::endl;
		}

#endif // CCTX_PLATFORM_XXX

	}

	CTRC_LOG_INFO(L"finished");

	return 0;
}
