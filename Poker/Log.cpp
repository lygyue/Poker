/***********************************************
 * File: Log.cpp
 *
 * Author: LYG
 * Date: Ê®ÔÂ 2018
 *
 * Purpose:
 *
 * 
 **********************************************/
#include "Log.h"
#include <stdarg.h>

LogImpl::LogImpl(const char* FilePath)
{
	char FileName[256];
	memset(FileName, 0, sizeof(FileName));
	strcpy_s(FileName, 256, FilePath);
	strcat_s(FileName, 256, "/VirtualCity.Log");

	fopen_s(&(mLogFile), FileName, "w");
}

LogImpl::~LogImpl()
{
	fclose(mLogFile);
}

void LogImpl::Log(std::string Msg)
{
	fprintf(mLogFile, "%s\n", Msg.c_str());

	fflush(mLogFile);
}

void LogImpl::LogFormat(const char* Format, ...)
{
	char szMsg[256] = { 0 };
	va_list args;
	va_start(args, Format);
	vsprintf_s(szMsg, 256, Format, args);
	va_end(args);
	Log(szMsg);
}