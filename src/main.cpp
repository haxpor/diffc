#include <iostream>
#include <cstring>
#include <string_view>
#include <regex>

using namespace std::literals;

inline static constexpr int kBUFFER_SIZE = 1024;
inline static constexpr std::string_view kPrefixRedTxt = "\033[1;31m"sv;
inline static constexpr std::string_view kPrefixGreenTxt = "\033[1;32m"sv;
inline static constexpr std::string_view kPrefixCyanTxt = "\033[1;36m"sv;
inline static constexpr std::string_view kPrefixWhiteTxt = "\033[1;37m"sv;
inline static constexpr std::string_view kPostfixTxt = "\033[0m"sv;
inline static constexpr std::string_view kPostfixWithNLTxt = "\033[0m\n"sv;

static bool FindCmdOption(int argc, char* argv[], const char* searchOption)
{
	for (int i=1; i<argc; ++i)
	{
		if (std::strcmp(argv[i], searchOption) == 0)
			return true;
	}
	return false;
}

static void colorify_perforce(const char* lineBuffer)
{
	enum ProcessingType
	{
		NONDU,
		DU
	};

	struct ProcessingState
	{
		bool isPrevProcessed {false};
		ProcessingType type {ProcessingType::NONDU};
	};

	//static int isPrevProcessingFilePathLine = false;
	static ProcessingState state;

	const std::size_t len = strlen(lineBuffer);

	if (len <= 0)
		return;

	auto lineW = std::string_view(lineBuffer, len);
	bool doneProcessed = false;

	// check to only do regex to color line range changes only when necessary
	if (state.isPrevProcessed)
	{
		if (state.type == ProcessingType::NONDU)
		{
			std::string inputLine(lineBuffer);
			std::smatch regexMatchResult;
			// e.g. 14,16c14,19, 516c516, or 3c3
			std::regex rgx ("[0-9]+,?[0-9]*c[0-9]+,?[0-9]*\\s");
			if (std::regex_match(inputLine, regexMatchResult, rgx))
			{
				std::cout << kPrefixCyanTxt << lineW << kPostfixWithNLTxt;
				doneProcessed = true;
			}
		}
		else
		{
			std::string inputLine(lineBuffer);
			std::smatch regexMatchResult;
			// e.g. @@ -11,9 +11,12 @@, @@ -513,7 +513,7 @@, or @@ -1,6 +1,6 @@
			std::regex rgx ("@@ -[0-9]+,[0-9]+ \\+[0-9]+,[0-9]+ @@\\s");
			if (std::regex_match(inputLine, regexMatchResult, rgx))
			{
				std::cout << kPrefixCyanTxt << lineW << kPostfixWithNLTxt;
				doneProcessed = true;
			}
		}
		state.isPrevProcessed = false;
	}

	// for non- "-du" flags used
	// target 4 chars included with 1 space
	if (!doneProcessed && len >= 5)
	{
		if (lineW.compare(0, 5, "==== ") == 0)
		{
			std::cout << kPrefixWhiteTxt << lineW << kPostfixWithNLTxt;
			state.isPrevProcessed = true;
			state.type = ProcessingType::NONDU;
			doneProcessed = true;
		}
	}
	// for "-du" flag used
	// target 4 chars included with 1 space
	if (!doneProcessed && len >= 4)
	{
		if (lineW.compare(0, 4, "--- ") == 0 ||
			lineW.compare(0, 4, "+++ ") == 0)
		{
			std::cout << kPrefixWhiteTxt << lineW << kPostfixWithNLTxt;
			state.isPrevProcessed = true;
			state.type = ProcessingType::DU;
			doneProcessed = true;
		}
	}

	//for non-"-du" of diff
	if (!doneProcessed && len >= 2)
	{
		// removal - color as red
		if (lineBuffer[0] == '<' && lineBuffer[1] == ' ')
		{
			std::cout << kPrefixRedTxt << lineW << kPostfixWithNLTxt;
			doneProcessed = true;
		}
		// additional - color as green
		else if (lineBuffer[0] == '>' && lineBuffer[1] == ' ')
		{
			std::cout << kPrefixGreenTxt << lineW << kPostfixWithNLTxt;
			doneProcessed = true;
		}
	}
	// for "-du" of diff
	if (!doneProcessed && len >= 1)
	{
		// removal - color as red
		if (lineBuffer[0] == '-' &&
			!(len == 4 && lineW.compare(0, len-1, "---") == 0))	// ignore change separator
		{
			std::cout << kPrefixRedTxt << lineW << kPostfixWithNLTxt;
			doneProcessed = true;
		}
		// additional - color as green
		else if (lineBuffer[0] == '+')
		{
			std::cout << kPrefixGreenTxt << lineW << kPostfixWithNLTxt;
			doneProcessed = true;
		}
	}
	
	if (!doneProcessed)
	{
		std::cout << lineW << "\n";
	}
}

int main(int argc, char* argv[])
{
	if (FindCmdOption(argc, argv, "--help"))
	{
		std::cout << "Usage: diffc -t <diff-variant>\n\n";
		std::cout << "Available diff-variant is as follows\n";
		std::cout << " - perforce\n";
		return 0;
	}

	// NOTE: for now we're being explicit, so it needs to know which diff variant
	// to be working on	
	if (argc < 3)
	{
		std::cerr << "See diffc --help\n";
		return -1;
	}
	char lineBuffer[kBUFFER_SIZE];

	while (true)
	{
		std::cin.getline(lineBuffer, kBUFFER_SIZE);
		auto state = std::cin.rdstate();
		if ((state & std::ios_base::eofbit) ||
			(state & std::ios_base::failbit) ||
			(state & std::ios_base::badbit))
		{
			break;
		}

		colorify_perforce(lineBuffer);
	}
	return 0;
}
