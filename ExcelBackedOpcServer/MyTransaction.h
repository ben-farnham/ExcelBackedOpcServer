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
//  Filename    : MyTransaction.h                                             |
//  Version     : 4.22                                                        |
//  Date        : 15-December-2008                                            |
//                                                                            |
//  Description : User's speciffic DataAccess OPC server                      |
//                DaTransaction definition                                    |
//                                                                            |
//-----------------------------------------------------------------------------
#ifndef _MYTRANSACTION_H_
#define _MYTRANSACTION_H_

#include "Da/ServerDaTransaction.h"
#include "MyDaAddressSpaceElement.h"
#include "ExcelIntegration.h"
#include <iostream>

#include <pantheios/pantheios.hpp>


using namespace SoftingOPCToolboxServer;
using namespace std;
using namespace pantheios;

class MyTransaction : public DaTransaction {

public:
	
	MyTransaction(
		EnumTransactionType aTransactionType, 
		std::vector<DaRequest*>& aRequestList,
		unsigned long aSessionKey) : 
		DaTransaction(
			aTransactionType,
			aRequestList,
			aSessionKey)
	{
	}
	
	virtual ~MyTransaction()
	{
	}

	virtual long handleReadRequests(void){

		size_t count = m_requestList.size();
		
		for(size_t i = 0; i < count; i++){

			DaRequest* pReq = m_requestList[i];
			
			MyDaAddressSpaceElement* pElem = (MyDaAddressSpaceElement*)pReq->getAddressSpaceElement();
			
			if (pElem == NULL)
			{
				pReq->setResult(E_FAIL);
			}
			else
			{
				if (pReq->getPropertyId() == 0)
				{	
					log_NOTICE("MyTransaction::handleReadRequests getting value for element [", pElem->getName(),"]");

					ValueQT vqtCacheValue;
					long nResult = pElem->GetExcelCellValue(vqtCacheValue)? S_OK: S_FALSE;
					pReq->setValue(vqtCacheValue);
					pReq->setResult(nResult);
				}
				else
				{
					// the element's property will handle this request
					pElem->getPropertyValue(pReq);
				}
			}
		}

		return completeRequests();

	} // end HandleReadRequests
	
	virtual long handleWriteRequests(void){

		size_t count = m_requestList.size();

		for(size_t i = 0; i < count; i++){

			DaRequest* pRequest = m_requestList[i];
			if (pRequest != NULL)
			{
				MyDaAddressSpaceElement* pElement = 
					(MyDaAddressSpaceElement*)pRequest->getAddressSpaceElement();
				if (pElement != NULL) {
					
					ValueQT* pValue = pRequest->getValue();
					pRequest->setResult(pElement->valueChanged(*pValue));
				} // end if
			} // end if
		} // end  for
		
		return completeRequests();

	} // end handleWriteRequests

};	// end class MyTransaction

#endif	// _MYTRANSACTION_H_
