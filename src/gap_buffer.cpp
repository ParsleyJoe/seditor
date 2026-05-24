#include "gap_buffer.hpp"

// for memcpy
#include <cstring>
#include <assert.h>
#include <algorithm>
#include <string>

GapBuffer::GapBuffer()
{
	
}

GapBuffer::GapBuffer(int n_bytes)
{
	buffer = new char[n_bytes];
	bufSize = n_bytes;

	// empty buffer
	gapStart = 0;
	gapEnd = n_bytes - 1;
}

void GapBuffer::shiftGapTo(int cursorPos)
{
	if (gapStart <= 0)
	{
		// can (probably?) start modifying
		return;
	}
	if (gapStart == cursorPos)
		return;

	if (gapStart == gapEnd)
	{
		// Full buffer need to reallocate
	}

	if (cursorPos < gapStart) // Gap is after cursor
	{
		size_t size = gapStart - cursorPos;
		void* dst = (void*)(buffer + (gapEnd - size) + 1);
		std::memmove(dst, (void*)(buffer + cursorPos), size * sizeof(char));
		gapStart -= size;
		gapEnd -= size;
	}
	else if (cursorPos > gapStart) // Gap is before cursor
	{
		size_t size = cursorPos - gapStart;
		void* dst = (void*)(buffer + gapStart);
		std::memmove(dst, (void*)(buffer + (gapEnd + 1)), size * sizeof(char));
		gapStart += size;
		gapEnd += size;
	}
}

// Reallocate if neccessary
void GapBuffer::checkGapSize(int n_required)
{
	size_t size = gapEnd - gapStart;
	if (size < n_required)
	{
		// shiftGapTo(bufSize - gapEnd);
		int newN = std::max((int)(bufSize * 2), n_required);
		char* newBuf = new char[newN];

		// Create a wider gap
		int validElements = bufSize - gapEnd - 1;
		std::memmove(newBuf, buffer, gapStart * sizeof(char));
		std::memmove(newBuf + (newN - validElements), buffer + (gapEnd + 1), validElements * sizeof(char));

		// Modify after copying is complete
		bufSize = newN;
		delete[] buffer;
		buffer = newBuf;
		gapEnd = newN - validElements - 1;
	}
}

void GapBuffer::insertChar(int cursorPos, char c)
{
	checkGapSize(1);
	shiftGapTo(cursorPos);

	// TODO: probably don't need these as we already checked the size
	assert(gapStart < bufSize);
	assert(gapStart >= 0);

	buffer[gapStart] = c;
	gapStart++;
	
}

void GapBuffer::removeChar(int cursorPos)
{
	shiftGapTo(cursorPos);

	gapStart--;
}

std::string GapBuffer::getLeftString()
{
	std::string str{""};

	// Gap starts AT gapStart inclusive, so buf[gapStart] would be invalid
	for (int i = 0; i < gapStart; i++)
	{
		str += buffer[i];
	}

	return str;
}

std::string GapBuffer::getRightString()
{
	std::string str{""};

	// Buffer ends BEFORE bufSize so bufSize - 1 is the last valid char, and gapEnd is THE END, so buf[gapEnd] is invalid
	for (int i = gapEnd + 1; i < bufSize; i++)
	{
		str += buffer[i];
	}

	return str;
}