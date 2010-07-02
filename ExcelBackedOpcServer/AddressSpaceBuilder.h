#pragma once

#include "MyDaAddressSpaceElement.h"
#include "MyCreator.h"

using namespace SoftingOPCToolboxServer;

class AddressSpaceBuilder
{
public:
	AddressSpaceBuilder(DaAddressSpaceRoot* daRoot);
	~AddressSpaceBuilder(void);

	size_t Build(MyCreator* pCreator);

private:
	MyDaAddressSpaceElement* CreateAddressSpaceElement(MyCreator* pCreator, const tstring& sName, const unsigned int nExcelRowNumber);
	DaProperty* CreateDescriptionProperty(void) const;
	DaAddressSpaceRoot* m_pRoot;
};
