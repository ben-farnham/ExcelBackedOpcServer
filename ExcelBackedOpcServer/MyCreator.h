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
//  Filename    : MyCreator.h                                                 |
//  Version     : 4.22                                                        |
//  Date        : 15-December-2008                                            |
//                                                                            |
//  Description : User's speciffic OPC Objects creator class                  |
//                                                                            |
//-----------------------------------------------------------------------------
#ifndef _MYCREATOR_H_
#define _MYCREATOR_H_


#include "ServerCreator.h"


#include "MyDaAddressSpaceRoot.h"
#include "MyDaAddressSpaceElement.h"
#include "MyRequest.h"
#include "MyTransaction.h"

using namespace SoftingOPCToolboxServer;

//-----------------------------------------------------------------------------
// MyCreator
//-----------------------------------------------------------------------------
class MyCreator : public Creator
{
protected: 

	
	virtual DaAddressSpaceElement* createInternalDaAddressSpaceElement(
		tstring& anItemId,
		tstring& aName,
		unsigned long anUserData,
		unsigned long anObjectHandle,
		unsigned long aParentHandle)
	{
		return new MyDaAddressSpaceElement(anItemId, aName, anUserData, anObjectHandle, aParentHandle);
	} // end createInternalDaAddressSpaceElement

public:

	virtual DaAddressSpaceRoot* createDaAddressSpaceRoot(void)
	{
		return new MyDaAddressSpaceRoot();
	} // end createDaAddressSpaceRoot

	virtual DaTransaction* createTransaction(
		EnumTransactionType aTransactionType, 
		std::vector<DaRequest*>& aRequestList,
		unsigned long aSessionKey)
	{
		return new MyTransaction(aTransactionType, aRequestList, aSessionKey);
	} // end createTransaction

	virtual DaRequest* createRequest(
		EnumTransactionType aTransactionType,
		unsigned long aSessionHandle,
		DaAddressSpaceElement* anElement,
		int aPropertyId,
		unsigned long aRequestHandle)
	{
		return new MyRequest(aTransactionType, aSessionHandle, anElement, aPropertyId, aRequestHandle);
	
	} // end createRequest

	virtual DaAddressSpaceElement* createMyDaAddressSpaceElement(void)
	{
		return new MyDaAddressSpaceElement();
	} // end createMyDaAddressSpaceElement
	

};	// end class MyCreator

#endif