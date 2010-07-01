#include <vole/vole.hpp>
#include <vole/error/exceptions.hpp>
#include <iostream>
#include <ole2.h>
#include <comstl/util/initialisers.hpp>


#include "ExcelIntegration.h"

using namespace vole;

using namespace std;

ExcelIntegration* gspTheInstance = NULL;

ExcelIntegration::ExcelIntegration()
:m_ExcelApplication(GetExcelDispatchPtr(), false), m_pActiveSheet(NULL)
{
	cout << "ExcelIntegration::ExcelIntegration+" << endl;

	cout << "does the app have active sheet? ["<< m_ExcelApplication.has_member(L"Activesheet") <<"]" << endl;
	m_pActiveSheet = new object(m_ExcelApplication.get_property<object>(L"Activesheet"));
	string sSheetName = m_pActiveSheet->get_property<string>(L"Name");
	cout << "active sheet name ["<< sSheetName.c_str() <<"]" << endl;

	cout << "ExcelIntegration::ExcelIntegration-" << endl;
}

ExcelIntegration::~ExcelIntegration(void)
{
	cout << "ExcelIntegration::~ExcelIntegration+" << endl;
	cout << "ExcelIntegration::~ExcelIntegration-" << endl;
}

ExcelIntegration* ExcelIntegration::GetInstance()
{
	if(gspTheInstance == NULL)
	{
		gspTheInstance = new ExcelIntegration();
	}

	return gspTheInstance;
}

IDispatch* ExcelIntegration::GetExcelDispatchPtr() const
{
	CLSID sExcelClsid;
	CLSIDFromProgID(L"Excel.Application", &sExcelClsid);

	IUnknown* pExcelUnknown = NULL;
	IDispatch* pExcelDispatch = NULL;
	if(SUCCEEDED(GetActiveObject(sExcelClsid, NULL, &pExcelUnknown)))
	{
		cout << "GetExcelDispatchPtr - successfully connected to running excel instance" << endl;

		if(SUCCEEDED(pExcelUnknown->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&pExcelDispatch))))
		{
			cout << "GetExcelDispatchPtr - successfully got dispatch pointer instance" << endl;
		}
	}

	if(pExcelDispatch == NULL)
	{
		cerr << "GetExcelDispatchPtr - Failed to get automation pointer to a running instance of Excel - Is Excel running? This application will exit" << endl;
		Sleep(3000);
		exit(EXIT_FAILURE);
	}

	return pExcelDispatch;
}

bool ExcelIntegration::GetCellValue(const string& sCellId, string& sResult)
{
	char cRangeBuff[16];
	FormatRangeBuffer(cRangeBuff, sizeof(cRangeBuff), sCellId);
	try
	{
		object oRange = m_pActiveSheet->get_property<object>(L"Range", cRangeBuff);

		if(oRange.is_nothing())
		{
			cout << "Error cellId ["<< sCellId <<"] returned NULL range" << endl;
			return false;
		}

		sResult = oRange.get_property<string>(L"Value");
	}
	catch(vole_exception e)
	{
		cout << "Error: VOLE exception: " << e.what() << endl;
		return false;
	}

	return true;
}

const char* ExcelIntegration::FormatRangeBuffer(char* const pBuffer, size_t sz, const std::string sCellId) const
{
	memset(static_cast<void*>(pBuffer), 0, sz);
	sprintf_s(pBuffer, sz, "%s:%s", sCellId.c_str(), sCellId.c_str());
	cout << "formatted range is ["<< pBuffer <<"]" << endl;
	return pBuffer;
}