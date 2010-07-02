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


using namespace SoftingOPCToolboxServer;
using namespace std;

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
			
			MyDaAddressSpaceElement* element = 
				(MyDaAddressSpaceElement*)m_requestList[i]->getAddressSpaceElement();
			
			if (element == NULL)
			{
				m_requestList[i]->setResult(E_FAIL);
			}
			else
			{
				if (m_requestList[i]->getPropertyId() == 0)
				{	
					std::cout << "MyTransaction getting value..." << std::endl;

					ValueQT vqtCacheValue;
					long nResult = element->GetExcelCellValue(vqtCacheValue)? S_OK: S_FALSE;
					m_requestList[i]->setValue(vqtCacheValue);
					m_requestList[i]->setResult(nResult);
/*					
					string sCellValue("woohoo");
					if(ExcelIntegration::GetInstance()->GetCellValue("A1", sCellValue))
					{
						cout << "Got cell value ["<< sCellValue <<"]" << endl;

						ValueQT cacheValue(Variant(sCellValue.c_str()), EnumQuality_GOOD, DateTime());					
						m_requestList[i]->setValue(cacheValue);

						nResult = S_OK;

					}
					else
					{
						cout << "Failed to get cell value" << endl;
					}
					
					m_requestList[i]->setResult(nResult);	
*/
				}
				else
				{
					// the element's property will handle this request
					element->getPropertyValue(m_requestList[i]);
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
