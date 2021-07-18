#include <iostream>
#include <cstring>
#include <regex>
#include "ColorifyText.h"

inline static constexpr int kBUFFER_SIZE = 1024;

enum class DiffVariantType
{
	Unknown,
	Perforce
};

/**
 * Find whether argument list of the program contains the 'searchOption' as specified.
 *
 * @param argc number of arguments
 * @param argv argument list
 * @param targetOpt target command line's flag option
 * @param outFoundIndex return index of found flag option if specified as non-null. It will be ignored if not found.
 * @return True if found, otherwise return false.
 */
static bool FindCmdOption(int argc, char* argv[], const char* targetOpt, int* outFoundIndex=nullptr)
{
	for (int i=1; i<argc; ++i)
	{
		if (std::strcmp(argv[i], targetOpt) == 0)
		{
			if (outFoundIndex != nullptr)
				*outFoundIndex = i;
			return true;
		}
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
		ProcessingType type {ProcessingType::NONDU};
	};

	//static int isPrevProcessingFilePathLine = false;
	static ProcessingState state;

	const std::size_t len = strlen(lineBuffer);

	if (len <= 0)
		return;

	auto lineW = std::string_view(lineBuffer, len);
	bool doneProcessed = false;

	// first character checking for optimization not to execute regex every line
	if (lineW[0] != ' ' &&
		lineW[0] != '<' &&
		lineW[0] != '>' &&
		state.type == ProcessingType::NONDU)
	{
		std::string inputLine(lineBuffer);
		std::smatch regexMatchResult;
		// e.g. 14,16c14,19, 516c516, or 3c3
		// also note it can be 'a' for addition, 'c' for change, and 'd' for deletion
		std::regex rgx ("^[0-9]+,?[0-9]*[acd][0-9]+,?[0-9]*\\s*");
		if (std::regex_match(inputLine, regexMatchResult, rgx))
		{
			ColorifyText::Print(ColorifyText::Color::Cyan, lineW);
			doneProcessed = true;
		}
	}
	else if (lineW[0] != ' ' &&
			 lineW[0] != '-' &&
			 lineW[0] != '+')
	{
		std::string inputLine(lineBuffer);
		std::smatch regexMatchResult;
		// e.g. @@ -11,9 +11,12 @@, @@ -513,7 +513,7 @@, or @@ -1,6 +1,6 @@
		std::regex rgx ("^@@ -[0-9]+,[0-9]+ \\+[0-9]+,[0-9]+ @@\\s*");
		if (std::regex_match(inputLine, regexMatchResult, rgx))
		{
			ColorifyText::Print(ColorifyText::Color::Cyan, lineW);
			doneProcessed = true;
		}
	}

	// for non- "-du" flags used
	// target 4 chars included with 1 space
	if (!doneProcessed && len >= 5)
	{
		if (lineW.compare(0, 5, "==== ") == 0)
		{
			ColorifyText::Print(lineW);
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
			ColorifyText::Print(lineW);
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
			ColorifyText::Print(ColorifyText::Color::Red, lineW);
			doneProcessed = true;
		}
		// additional - color as green
		else if (lineBuffer[0] == '>' && lineBuffer[1] == ' ')
		{
			ColorifyText::Print(ColorifyText::Color::Green, lineW);
			doneProcessed = true;
		}
	}
	// for "-du" of diff
	if (!doneProcessed && len >= 1)
	{
		// removal - color as red
		if (lineBuffer[0] == '-' &&
			!(len >= 3 && lineW.compare(0, 3, "---") == 0))	// ignore change separator
		{
			ColorifyText::Print(ColorifyText::Color::Red, lineW);
			doneProcessed = true;
		}
		// additional - color as green
		else if (lineBuffer[0] == '+')
		{
			ColorifyText::Print(ColorifyText::Color::Green, lineW);
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
	if (!ColorifyText::Init())
		std::cerr << "Colorifying text sub-system initialization error\n";

	if (FindCmdOption(argc, argv, "--help"))
	{
		std::cout << "Usage: diffp4 < some-command\n";
		std::cout << "or\n";
		std::cout << "some-command | diffp4\n\n";
		return 0;
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
