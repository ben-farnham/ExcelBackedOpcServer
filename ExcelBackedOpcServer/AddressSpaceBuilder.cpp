#include "stdafx.h"
#include "AddressSpaceBuilder.h"
#include "ExcelIntegration.h"
#include <iostream>
#include <pantheios/pantheios.hpp>
#include <pantheios/inserters.hpp>

using namespace std;
using namespace pantheios;

AddressSpaceBuilder::AddressSpaceBuilder(DaAddressSpaceRoot* daRoot)
:m_pRoot(daRoot)
{
	log_NOTICE("AddressSpaceBuilder::AddressSpaceBuilder+");
	log_NOTICE("AddressSpaceBuilder::AddressSpaceBuilder-");
}

AddressSpaceBuilder::~AddressSpaceBuilder(void)
{
	log_NOTICE("AddressSpaceBuilder::~AddressSpaceBuilder+");
	log_NOTICE("AddressSpaceBuilder::~AddressSpaceBuilder-");
}

/**
* Returns number of items built in address space.
*/
size_t AddressSpaceBuilder::Build(MyCreator* pCreator)
{
	log_NOTICE("AddressSpaceBuilder::Build+");
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
			MyDaAddressSpaceElement* pElement = CreateAddressSpaceElement(pCreator, sCellValue, i+1);
			m_pRoot->addChild(pElement);
			nAddressSpaceSz++;
		}				
	}

	log_NOTICE("AddressSpaceBuilder::Build- result [", integer(nAddressSpaceSz),"]");
	return nAddressSpaceSz;
}

MyDaAddressSpaceElement* AddressSpaceBuilder::CreateAddressSpaceElement(MyCreator* pCreator, const tstring& sName, const unsigned int nExcelRowNumber)
{
	log_NOTICE("AddressSpaceBuilder::CreateAddressSpaceElement+ name [", sName,"]");
	MyDaAddressSpaceElement* pElement = static_cast<MyDaAddressSpaceElement*>(pCreator->createMyDaAddressSpaceElement());
	pElement->setName(const_cast<tstring&>(sName));
	pElement->setAccessRights(EnumAccessRights_READWRITEABLE);
	pElement->setDatatype(VT_BSTR);
	pElement->setIoMode(EnumIoMode_POLL);	
	pElement->SetExcelRowNumber(nExcelRowNumber);

	ValueQT value(Variant(_T("Initial value ")), EnumQuality_GOOD, DateTime());
	pElement->valueChanged(value);

	pElement->addProperty(CreateDescriptionProperty());

	log_NOTICE("AddressSpaceBuilder::CreateAddressSpaceElement-");
	return pElement;
}

DaProperty* AddressSpaceBuilder::CreateDescriptionProperty(void) const
{
	DaProperty* pProperty = new DaProperty();
	pProperty->setId(EnumPropertyId_ITEM_DESCRIPTION);
	
	tstring propName(_T("Description"));
	pProperty->setName(propName);
	
	pProperty->setDescription(tstring(_T("Element Description")));
	pProperty->setItemId(propName);
	pProperty->setDatatype(VT_BSTR);
	pProperty->setAccessRights(EnumAccessRights_READABLE);

	return pProperty;
}