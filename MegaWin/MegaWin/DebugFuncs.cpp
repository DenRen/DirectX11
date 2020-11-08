#define _CRT_SECURE_NO_WARNINGS

#include "DebugFuncs.h"
#include <cstdio>
#include <stdexcept>
#include <windows.h>

void NOP ()
	{}

void PrintLogInfo (const char file[], int line, bool inFile)
{
	const int sizeLog = 512;
	char log[sizeLog] = "";
	snprintf (log, sizeLog, R"(/////////////////////____________DUMP ERROR____________\\\\\\\\\\\\\\\\\\\\\)");
	snprintf (log, sizeLog, "%s\n\n"
							"FILE: %s\n"
							"LINE: %d\n", log, file, line);

	snprintf (log, sizeLog, "%s\nerror:\n%s\n\n", log, std::strerror (errno));
	snprintf (log, sizeLog, R"(%s\\\\\\\\\\\\\\\\\\\\\_______________END________________/////////////////////)", log);

	FILE *outputStream = nullptr;

	if (inFile)
	{
		const char logFileName[] = "log.txt";
		outputStream = fopen (logFileName, "w");
		if (outputStream == nullptr)
		{
			snprintf (log, sizeLog, "Failed to create log file!\n%s", log);
			MessageBox (nullptr, log, 0, MB_OK);
			
			return;
		}
	}
	else
	{
		outputStream = stdout;
	}

	fprintf (outputStream, "%s", log);


	if (inFile)
	{
		fclose (outputStream);
	}
}


#undef _CRT_SECURE_NO_WARNINGS
