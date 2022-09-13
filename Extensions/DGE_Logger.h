#pragma once

#pragma region license
/***
*	BSD 3-Clause License
	Copyright (c) 2021, 2022 Alex
	All rights reserved.
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.
	3. Neither the name of the copyright holder nor the names of its
	   contributors may be used to endorse or promote products derived from
	   this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***/
#pragma endregion

#pragma region includes

#include <iostream>
#include <fstream>
#include <ctime>

#pragma endregion

#ifdef DGE_LOGGER

namespace def
{
	class DGE_Logger
	{
	public:
		DGE_Logger()
		{
			sFilename = "DGE.log";
		}

		DGE_Logger(const char* file_logs)
		{
			if (!OpenFile(file_logs))
				std::cerr << "[DGE_Logger] Can not open file for writing logs!\n";
		}

		~DGE_Logger()
		{
			CloseFile();
		}

	public:

		bool OpenFile(const char* filename)
		{
			ofLogFile.open(filename, std::ios::out);

			return ofLogFile.good() && !ofLogFile.fail();
		}

		void CloseFile()
		{
			ofLogFile.close();
		}

		void GetTimestamp()
		{
			time_t tCurrentTime;
			tm* tmCurrentTimeMark = new tm;

			std::time(&tCurrentTime);

			localtime_s(tmCurrentTimeMark, &tCurrentTime);

			strftime(date, 32, "[ %B %d, %Y ]", tmCurrentTimeMark);
			strftime(time, 16, " [ %T ] ", tmCurrentTimeMark);
		}

		bool LogInfo(rcode& rc)
		{
			GetTimestamp();

			if (rc.info != "ok" && rc.info != "Ok" && rc.info != "oK")
				ofLogFile << (rc.ok ? "[ Info ] " : "[ Error ] ") << date << time << rc.info << "\n";

			return rc.ok;
		}

		void LogExit()
		{
			rcode rc;

			rc.ok = true;
			rc.info = "End running of application";

			LogInfo(rc);
		}

	private:
		std::string sFilename;

		std::ofstream ofLogFile;

		char date[64];
		char time[32];

	};
}

#endif
