#pragma once
#include <string>

class GapBuffer
{
	char* buffer = "";
	size_t bufSize = 0;
	int gapStart = 0;
	int gapEnd = 0;

public:
	GapBuffer();
	GapBuffer(int n_bytes);

	void shiftGapTo(int cursorPos);
	void checkGapSize(int n_required);
	void insertChar(int cursorPos, char c);
	void removeChar(int cursorPos);
	std::string getLeftString();
	std::string getRightString();
};

