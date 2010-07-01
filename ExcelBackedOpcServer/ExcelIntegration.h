#pragma once

/*
* Class covers excel integration: Note initializing COM/OLE is the calling
* environment's responsibility.
*/
class ExcelIntegration
{
private:
	ExcelIntegration();
	~ExcelIntegration(void);

public:
	static ExcelIntegration* GetInstance();

	bool GetCellValue(const std::string& sCellId, std::string& sResult);

private:
	void InitialiseExcelApplication(void);
	void InitialiseActiveSheet(void);
	const char* FormatRangeBuffer(char* const pBuffer, size_t sz, const std::string sCellId) const;
	
	IDispatch* GetExcelDispatchPtr() const;

private:
	vole::object m_ExcelApplication;
	vole::object* m_pActiveSheet;
};
