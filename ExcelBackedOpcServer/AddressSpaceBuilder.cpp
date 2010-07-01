#include "stdafx.h"
#include "AddressSpaceBuilder.h"
#include "ExcelIntegration.h"
#include <iostream>

using namespace std;

AddressSpaceBuilder::AddressSpaceBuilder(DaAddressSpaceRoot* daRoot)
:m_pRoot(daRoot)
{
}

AddressSpaceBuilder::~AddressSpaceBuilder(void)
{
}

/**
* Returns number of items built in address space.
*/
size_t AddressSpaceBuilder::Build(MyCreator* pCreator)
{
	ExcelIntegration* pExcel = ExcelIntegration::GetInstance();

	int nAddressSpaceSz = 0;
	for(unsigned int i=0; i<10; i++)
	{		
		stringstream sCellRow;
		sCellRow << i+1;
		string sCellId("A" + sCellRow.str());

		string sCellValue("undefined");
		if(pExcel->GetCellValue(sCellId, sCellValue))
		{
			MyDaAddressSpaceElement* pElement = CreateAddressSpaceElement(pCreator, sCellValue, i);
			m_pRoot->addChild(pElement);
			nAddressSpaceSz++;
		}				
	}

	return nAddressSpaceSz;
}

MyDaAddressSpaceElement* AddressSpaceBuilder::CreateAddressSpaceElement(MyCreator* pCreator, const tstring& sName, const unsigned int nIndex)
{
	MyDaAddressSpaceElement* pElement = static_cast<MyDaAddressSpaceElement*>(pCreator->createMyDaAddressSpaceElement());
	pElement->setName(const_cast<tstring&>(sName));
	pElement->setAccessRights(EnumAccessRights_READWRITEABLE);
	pElement->setDatatype(VT_BSTR);
	pElement->setIoMode(EnumIoMode_POLL);	

	ValueQT value(Variant(_T("Initial value ")), EnumQuality_GOOD, DateTime());
	pElement->valueChanged(value);

	DaProperty* pProperty = new DaProperty();
	pProperty->setId(nIndex);
	
	tstring propName(_T("Description"));
	pProperty->setName(propName);
	
	tstring propDescription(_T("Element Description"));
	pProperty->setDescription(propDescription);
	pProperty->setItemId(propName);
	pProperty->setDatatype(VT_BSTR);
	pProperty->setAccessRights(EnumAccessRights_READABLE);

	pElement->addProperty(pProperty);

	return pElement;
}