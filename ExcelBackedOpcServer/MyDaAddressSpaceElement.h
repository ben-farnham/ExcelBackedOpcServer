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
//  Filename    : MyAddressSpaceElement.h                                     |
//  Version     : 4.22                                                        |
//  Date        : 15-December-2008                                            |
//                                                                            |
//  Description : User's speciffic DataAccess Server OPC Server               |
//                address space element definition                            |
//                                                                            |
//-----------------------------------------------------------------------------
#ifndef _MYDAADDRESSSPACEELEMENT_H_
#define _MYDAADDRESSSPACEELEMENT_H_

#include "Da/ServerDaAddressSpaceElement.h"
#include "Da/ServerDaProperty.h"
#include "ExcelIntegration.h"

using namespace SoftingOPCToolboxServer;

//-----------------------------------------------------------------------------
// MyDaAddressSpaceElement
//-----------------------------------------------------------------------------
class MyDaAddressSpaceElement : public DaAddressSpaceElement
{

public:
	MyDaAddressSpaceElement(
		tstring& anItemID,
		tstring& aName,
		unsigned long anUserData,
		unsigned long anObjectHandle, 
		unsigned long aParentHandle) : 
		DaAddressSpaceElement(anItemID, aName, anUserData, anObjectHandle, aParentHandle),
		m_nExcelRowNumber(0)
	{
	}	
	
	MyDaAddressSpaceElement(void)
	{		
	}
	
	virtual ~MyDaAddressSpaceElement(void)
	{
		size_t count = m_properties.size();
		for (size_t index = 0; index < count; index++){
			DaProperty* property = m_properties[index];
			if (property != NULL){
				delete property;
			}
		}
	}

	
	virtual void addedToAddressSpace(void)
	{
		// add your notification code here
	}

	virtual void removedFromAddressSpace(void)
	{
		// add your notification code here
	}
	
	void getPropertyValue(DaRequest* aRequest)
	{
		if (aRequest->getPropertyId() == EnumPropertyId_ITEM_DESCRIPTION){
			DateTime now;
			now.now();

			Variant aVariant(_T("description"));
			ValueQT aValue(aVariant, EnumQuality_GOOD, now);
	
			aRequest->setValue(aValue);
			aRequest->setResult(S_OK);
		}
	}
	
	long queryProperties(std::vector<DaProperty*>& aPropertyList)
	{
		aPropertyList = m_properties;
		return S_OK;
	}
	
	long addProperty(DaProperty* aProperty)
	{
		if (aProperty == NULL)
		{
			return S_FALSE;
		}

		m_properties.push_back(aProperty);
		return S_OK;
	}

	void SetExcelRowNumber(const unsigned int nExcelRowNumber)
	{
		m_nExcelRowNumber = nExcelRowNumber;
	}

	bool GetValueCellId(std::string& sResult)
	{
		static const std::string sExcelValuesColumn = "B";

		if(m_nExcelRowNumber < 1)
		{
			sResult = "No Excel Row Specified";
			return false;
		}

		std::stringstream sExcelCellRow;
		sExcelCellRow << m_nExcelRowNumber;
		sResult = (sExcelValuesColumn + sExcelCellRow.str());
		return true;
	}

	bool GetExcelCellValue(ValueQT& rValue)
	{
		long nResult = S_FALSE;
		std::string sCellId;
		if(GetValueCellId(sCellId))
		{
			std::string sCellValue;
			if(ExcelIntegration::GetInstance()->GetCellValue(sCellId, sCellValue))
			{
				rValue.setData(Variant(sCellValue.c_str()), EnumQuality_GOOD, DateTime());
				return true;
			}			
		}

		rValue.setData(Variant("Excel Integration Error"), EnumQuality_BAD, DateTime());
		return false;
	}
	
	private:
		std::vector<DaProperty*> m_properties;
		unsigned int m_nExcelRowNumber;
};

#endif