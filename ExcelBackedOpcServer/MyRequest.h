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
//  Filename    : MyRequest.h                                                 |
//  Version     : 4.22                                                        |
//  Date        : 15-December-2008                                            |
//                                                                            |
//  Description : User's speciffic DataAccess OPC server                      |
//                DaRequest definition                                        |
//                                                                            |
//-----------------------------------------------------------------------------
#ifndef _MYREQUEST_H_
#define _MYREQUEST_H_

#include "Da/ServerDaRequest.h"

using namespace SoftingOPCToolboxServer;

class MyRequest : public DaRequest{

public:
	MyRequest(
		EnumTransactionType aTransactionType,
		unsigned long aSessionHandle,
		DaAddressSpaceElement* anElement,
		int aPropertyId,
		unsigned long aRequestHandle):
		DaRequest(aTransactionType, 
			aSessionHandle, 
			anElement, 
			aPropertyId, 
			aRequestHandle)
	{} // end ctor
	
	virtual ~MyRequest(void)
	{} // end dtor

};	// end class MyRequest

#endif