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
//  Filename    : OpcServer.cpp                                               |
//  Version     : 4.22                                                        |
//  Date        : 15-December-2008                                            |
//                                                                            |
//  Description : OPC Server template class implementation                    |
//                                                                            |
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "OpcServer.h"
#include "ServerCommon.h"
#include "AddressSpaceBuilder.h"

OpcServer* instance = NULL;


OpcServer* getOpcServer(void)
{
	return instance; 
} // end getOpcServer


void createOpcServer(void)
{
	if (instance == NULL)
	{
		instance = new OpcServer();
	} // end if

} // end createOpcServer


void destroyOpcServer(void)
{
	if (instance != NULL){
		delete instance;
		instance = NULL;
	} // end if

} // end destroyOpcServer


long API_CALL handleShutdownRequest(void)
{
	//	TODO: add your Appliction termination code here
	return S_OK;
} // end handleShutdownRequest


OpcServer::OpcServer(void)
{
	m_pDaSimulationElement = NULL;
} // end constructor


OpcServer::~OpcServer(void)
{
} // end destructor

 
long OpcServer::initialize(void)
{
	getApp()->setVersionOtb(422);
	getApp()->setAppType(EnumApplicationType_EXECUTABLE);

	getApp()->setClsidDa(_T("{3159A5CD-309F-4A9F-9DC0-23E672E7C0FB}"));
	getApp()->setProgIdDa(_T("CERN.ExcelBackedOpcServer.DA.1"));
	getApp()->setVerIndProgIdDa(_T("CERN.ExcelBackedOpcServer.DA"));
	getApp()->setDescription(_T("CERN simulation OPC server - fronts excel spreadsheet"));
	
	getApp()->setMajorVersion(0);
	getApp()->setMinorVersion(0);
	getApp()->setBuildNumber(0);
	getApp()->setVendorInfo(_T("Vendor"));
	getApp()->setMinUpdateRateDa(1000);
	getApp()->setClientCheckPeriod(30000);
	getApp()->setAddressSpaceDelimiter(_T('.'));
	getApp()->setPropertyDelimiter(_T('/'));
	getApp()->ShutdownRequest = handleShutdownRequest;

	return S_OK;

} // end initialize

void OpcServer::setServiceName(tstring serviceName)
{
	getApp()->setServiceName(serviceName);	
} // end setServiceName

long OpcServer::prepare(MyCreator* creator)
{	    
	long result = S_OK;

	// TODO - design time license activation
	// Fill in your design time license activation keys here
	//
	// NOTE: you can activate one or all of the features at the same time
	
	// activate the COM-DA Server feature
	// result = getApp()->activate(EnumFeature_DA_SERVER, _T("XXXX-XXXX-XXXX-XXXX-XXXX"));
	if (!SUCCEEDED(result))
	{
		return result;
	}

	// END TODO - design time license activation

	// a valid creator must be provided to the initialize
	result = getApp()->initialize(creator);
	if (!SUCCEEDED(result))
	{
		return result;
	}

	// proceed with initializing of the Toolbox tracing mechanism
	getApp()->enableTracing(
			EnumTraceGroup_ALL,
			EnumTraceGroup_ALL,
			EnumTraceGroup_SERVER,
			EnumTraceGroup_SERVER,
			_T("Trace.txt"),
			1000000,
			0);

	return result;

} // end prepare

long OpcServer::start(void) 
{
	return getApp()->start(); 
} // end start

long OpcServer::ready(void)
{
	return getApp()->ready();
} // end ready

long OpcServer::stop(void)
{
	return getApp()->stop(); 
} // end stop

long OpcServer::terminate(void) 
{	
	long result = getApp()->terminate(); 
	releaseApp();
	return result;
} // end terminate

long OpcServer::processCommandLine(tstring command)
{
	return getApp()->processCommandLine(command);
} // end processCommandLine


long OpcServer::buildAddressSpace(void)
{
	SoftingOPCToolboxServer::Application* pApp = getApp();
	AddressSpaceBuilder objBuilder(pApp->getDaAddressSpaceRoot());
	objBuilder.Build(static_cast<MyCreator*>(pApp->getCreator()));

	return S_OK;

}


void OpcServer::changeSimulationValues(void)
{
	if (m_pDaSimulationElement != NULL)
	{
		DateTime now;
		Variant aVariant(::rand());
		ValueQT value(aVariant, EnumQuality_GOOD, now);
		m_pDaSimulationElement->valueChanged(value);
	}
} // end changeSimulationValues

void OpcServer::trace(
	EnumTraceLevel aLevel, 
	EnumTraceGroup aMask,
	const TCHAR* anObjectID,
	const TCHAR* aMessage,
	...)
{
	getApp()->trace(aLevel, aMask, anObjectID, aMessage);
} // end trace
