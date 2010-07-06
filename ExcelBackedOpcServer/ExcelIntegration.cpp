#include <vole/vole.hpp>
#include <vole/error/exceptions.hpp>
#include <iostream>
#include <ole2.h>
#include <comstl/util/initialisers.hpp>
#include <pantheios/pantheios.hpp>

#include "ExcelIntegration.h"

using namespace vole;
using namespace std;
using namespace pantheios;

ExcelIntegration* gspTheInstance = NULL;

ExcelIntegration::ExcelIntegration()
:m_ExcelApplication(GetExcelDispatchPtr(), true), m_pActiveSheet(NULL)
{
	log_NOTICE("ExcelIntegration::ExcelIntegration+");
	log_NOTICE("does the app have active sheet? [", (m_ExcelApplication.has_member(L"Activesheet")?"YES":"NO"), "]");

	object oSheet = m_ExcelApplication.get_property<object>(L"Activesheet");

	if(oSheet.is_nothing())
	{
		pantheios::log_CRITICAL("ExcelIntegration::ExcelIntegration failed to get valid Activesheet property");		
		exit(EXIT_FAILURE);
	}

	m_pActiveSheet = new object(oSheet);
	string sSheetName = m_pActiveSheet->get_property<string>(L"Name");
	log_NOTICE("active sheet name [", sSheetName.c_str(), "]");

	log_NOTICE("ExcelIntegration::ExcelIntegration-");
}

ExcelIntegration::~ExcelIntegration(void)
{
	log_NOTICE("ExcelIntegration::~ExcelIntegration+");
	log_NOTICE("ExcelIntegration::~ExcelIntegration-");
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
		log_NOTICE("GetExcelDispatchPtr - successfully connected to running excel instance");

		if(SUCCEEDED(pExcelUnknown->QueryInterface(IID_IDispatch, reinterpret_cast<void**>(&pExcelDispatch))))
		{
			log_NOTICE("GetExcelDispatchPtr - successfully got dispatch pointer instance");
		}
	}

	if(pExcelDispatch == NULL)
	{
		log_CRITICAL("GetExcelDispatchPtr - Failed to get automation pointer to a running instance of Excel - Is Excel running? This application will exit");
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
			log_WARNING("Error cellId [", sCellId, "] returned NULL range");
			return false;
		}

		sResult = oRange.get_property<string>(L"Value");
	}
	catch(vole_exception e)
	{
		log_ERROR("Error: VOLE exception: ", e.what());
		return false;
	}

	return true;
}

const char* ExcelIntegration::FormatRangeBuffer(char* const pBuffer, size_t sz, const std::string sCellId) const
{
	memset(static_cast<void*>(pBuffer), 0, sz);
	sprintf_s(pBuffer, sz, "%s:%s", sCellId.c_str(), sCellId.c_str());
	log_NOTICE("formatted range is [", pBuffer, "]");
	return pBuffer;
}