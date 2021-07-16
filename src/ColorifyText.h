#pragma once

#include <ostream>
#include <string_view>
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif

struct ColorifyText
{
private:
	struct Internal
	{
#ifdef _WIN32
		inline static HANDLE s_hNormalOutput = INVALID_HANDLE_VALUE;
		inline static HANDLE s_hErrorOutput = INVALID_HANDLE_VALUE;
#endif

		enum class StdOutput
		{
			Normal,
			Error
		};

		static constexpr std::ostream& GetOutputS_CompileTime(StdOutput so)
		{
			return so == StdOutput::Normal ? std::cout : std::cerr;
		}

#ifdef _WIN32
		static constexpr HANDLE GetOutputSHandle_CompileTime(StdOutput so)
		{
			return so == StdOutput::Normal ? s_hNormalOutput : s_hErrorOutput;
		}
#endif

		inline static constexpr std::string_view kPrefixRedTxt = std::string_view("\033[1;31m");
		inline static constexpr std::string_view kPrefixGreenTxt = std::string_view("\033[1;32m");
		inline static constexpr std::string_view kPrefixCyanTxt = std::string_view("\033[1;36m");
		inline static constexpr std::string_view kPrefixWhiteTxt = std::string_view("\033[1;37m");
		inline static constexpr std::string_view kPostfixWithNLTxt = std::string_view("\033[0m\n");
	};

public:
	enum class Color
	{
		Red,
		Green,
		Cyan,
		White,	// white is different from normal un-colored print
	};

	static bool Init()
	{
#ifdef _WIN32
		// enable vt console from specified handle
		// return 0 for success
		auto enableVTConsole = [](HANDLE handle) -> DWORD {
			DWORD dwMode = 0;
			if (!GetConsoleMode(handle, &dwMode))
				return GetLastError();
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			if (!SetConsoleMode(handle, dwMode))
				return GetLastError();
			return 0;
		};

		Internal::s_hNormalOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		if (Internal::s_hNormalOutput == INVALID_HANDLE_VALUE)
		{
			std::cerr << "GetStdHandle for STD_OUTPUT_HANDLE failed\n";
			return false;
		}

		Internal::s_hErrorOutput = GetStdHandle(STD_ERROR_HANDLE);
		if (Internal::s_hErrorOutput == INVALID_HANDLE_VALUE)
		{
			std::cerr << "GetStdHandle for STD_ERROR_HANDLE failed\n";
			return false;
		}

		// enabling VT console most likely affects only for old terminal like command prompt
		// even if we didn't enable it, Powershell can handle coloring text just fine.
		if (enableVTConsole(Internal::s_hNormalOutput) != 0)
		{
			std::cerr << "Enable Virtual console for STD_OUTPUT_HANDLE failed\n";
			return false;
		}
		if (enableVTConsole(Internal::s_hErrorOutput) != 0)
		{
			std::cerr << "Enable Virtual console for STD_ERROR_HANDLE failed\n";
			return false;
		}

		return true;
#else
		return true;
#endif
	}

	static void Print(std::string_view text)
	{
		PrintImpl(Internal::StdOutput::Normal, Color::White, text);
	}

	static void Print(Color color, std::string_view text)
	{
		PrintImpl(Internal::StdOutput::Normal, color, text);
	}

	static void PrintErr(std::string_view text)
	{
		PrintImpl(Internal::StdOutput::Error, Color::White, text);
	}

	static void PrintErr(Color color, std::string_view text)
	{
		PrintImpl(Internal::StdOutput::Error, color, text);
	}

private:
	static void PrintImpl(Internal::StdOutput so, Color color, std::string_view text)
	{
		switch (color)
		{
			case Color::Red:
				Internal::GetOutputS_CompileTime(so) << Internal::kPrefixRedTxt << text << Internal::kPostfixWithNLTxt;
				break;
			case Color::Green:
				Internal::GetOutputS_CompileTime(so) << Internal::kPrefixGreenTxt << text << Internal::kPostfixWithNLTxt;
				break;
			case Color::Cyan:
				Internal::GetOutputS_CompileTime(so) << Internal::kPrefixCyanTxt << text << Internal::kPostfixWithNLTxt;
				break;
			case Color::White:
			default:	// default to white color
				Internal::GetOutputS_CompileTime(so) << Internal::kPrefixWhiteTxt << text << Internal::kPostfixWithNLTxt;
				break;
		}
	}
};
