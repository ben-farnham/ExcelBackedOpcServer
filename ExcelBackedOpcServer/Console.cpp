//-----------------------------------------------------------------------------
//                                                                            |
//                               Softing AG                                   |
//                        Richard-Reitzner-Allee 6                            |
//                           85540 Haar, Germany                              |
//                                                                            |
//                 This is a part of the Softing OPC Toolbox                  |
//                   Copyright (C) Softing AG 1998 - 2008                     |
//                           All Rights Reserved                              |
//                                                                            |
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//                             OPC Toolbox C++                                |
//                                                                            |
//  Filename    : Console.cpp                                                 |
//  Version     : 4.22                                                        |
//  Date        : 15-December-2008                                            |
//                                                                            |
//  Description : Console application main implementation                     |
//                                                                            |
//-----------------------------------------------------------------------------
#include "stdafx.h"

#include "OpcServer.h"
#include <comstl/util/initialisers.hpp> // for comstl::com_initialiser;
#include <pantheios/pantheios.hpp>
#include <pantheios/backends/bec.file.h>


HANDLE g_endEvent = 0;


extern "C" const char PANTHEIOS_FE_PROCESS_IDENTITY[] = "ExcelBackedOpcServer";
char gscLogFilePath[MAX_PATH];

BOOL WINAPI ControlHandler(IN unsigned long crtlType)	// control type
{
	switch(crtlType)
	{
		case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to end server
		case CTRL_C_EVENT:   
		case CTRL_CLOSE_EVENT:
				
		if (g_endEvent)
		{
			SetEvent(g_endEvent);
		}
		return TRUE;
    }
    return FALSE;

} // end controlHandler

const char* const GetLogFilePath()
{
	time_t tmNow = time(NULL);
	
	char cTmString[MAX_PATH];
	memset(cTmString, MAX_PATH, 0);
	strftime(cTmString, MAX_PATH, "%Y_%m_%d___%H_%M_%S", localtime(&tmNow));

	memset(gscLogFilePath, 0, MAX_PATH);
	sprintf_s(gscLogFilePath, MAX_PATH, "C:\\TEMP\\%s___ExcelBackedOpcServer.log", cTmString);

	std::cout << "logfile ["<< gscLogFilePath <<"]" << std::endl;	
	return gscLogFilePath;
}

void InitialiseLogging()
{
	std::cout << "initialising logging..." << std::endl;

	try
	{
		if(pantheios::pantheios_init() < 0)
		{
			std::cout << "ERROR failed to initialise logger" << std::endl;
		}
		else
		{
			pantheios_be_file_setFilePath(GetLogFilePath(), PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BEID_LOCAL);
		}
		pantheios::log_NOTICE("InitialiseLogging: logging initialised");
		
		std::cout << "logging initialised" << std::endl;
	}
	catch(...)
	{
		pantheios::logputs(pantheios::emergency, "Unexpected unknown error occurred whilst initialising logging");
		std::cout << "logging failed" << std::endl;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	long result = S_OK;
	//comstl::com_initialiser coinit;
	InitialiseLogging();

	g_endEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	SetConsoleCtrlHandler(ControlHandler, TRUE);

	// create and initialize the OpcServer instance
	createOpcServer();
	OpcServer* pServer = getOpcServer();
	pServer->initialize();


	// provide the server with the proper custom creator
	MyCreator creator;
	if (!SUCCEEDED(pServer->prepare(&creator)))
	{
		pServer->terminate();
		destroyOpcServer();
	
		CloseHandle(g_endEvent);
		return 1;
	}

	// handle the command line arguments (register/unregister, etc)
	tstring commandLine(GetCommandLine());
	result = pServer->processCommandLine(commandLine);
	
	if (result != S_OK)
	{	
		long returnResult = 0;
		if (result == S_FALSE)
		{
			// registration operation successful
			pServer->trace(EnumTraceLevel_INF, EnumTraceGroup_USER1, _T("_tWinMain"),
				_T("Registration succeeded"));
			returnResult = 0;
		}
		else
		{
			// registration operation has failed
			pServer->trace(EnumTraceLevel_INF, EnumTraceGroup_USER1, _T("_tWinMain"),
				_T("Registration failed"));
			returnResult = 1;
		}
		
		// no matter what close the application if processCommandLine returned 
		// something different of S_OK
		pServer->terminate();
		destroyOpcServer();

		CloseHandle(g_endEvent);
		return returnResult;
	} // end if


	// Start the OPC server's I/O internal mechanism 
	if (SUCCEEDED(pServer->start()))
	{	
		// build the addressSpace
		pServer->buildAddressSpace();

		// declare the namespaces built and the server ready for clients to connect
		pServer->ready();
	}

		
	_tprintf(_T("Press Ctrl-C to exit\n"));
	while (true)
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(g_endEvent, 100))
		{
			break;
		}
	}


	pServer->stop();
	pServer->terminate();	


	destroyOpcServer();


	CloseHandle(g_endEvent);	
	return 0;

} // end main
