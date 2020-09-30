#pragma once
#include <codecvt>

#define ENGINE_ERROR_MESSAGE(hr) { EngineException::EngineErrorMessage(hr, __LINE__, __FUNCTION__, __FILE__);  }
#define ENGINE_HR_BOOL(hr) { HRESULT hresult = hr; if(FAILED(hresult)){ENGINE_ERROR_MESSAGE(hresult); return false;}}
#define ENGINE_HR(hr) { HRESULT hresult = hr; if (FAILED(hresult)) { ENGINE_ERROR_MESSAGE(hresult); }}

class EngineException {
public:
	EngineException() {

	}

	static void EngineErrorMessage(HRESULT result, const int line, const char* function, const char* /*file*/, ...) {
		_com_error err(result); 
		LPCTSTR error_msg = err.ErrorMessage();
		//std::wstring errorStr(L"FUNCTION: ");

		std::wstring str;
		str.append(L"\nERROR: ");
		str.append(error_msg);
		std::wstring aFunctionString = stringToWstring(function);
		str.append(L"\nFUNCTION: ");
		str.append(aFunctionString);
		//str.append(L"\nLINE: ");
		str.append(L"(");
		str.append(std::to_wstring(line));
		str.append(L")");
		
		MessageBox(0, str.c_str(), 0, MB_ICONERROR);
	}

	static std::wstring stringToWstring(const std::string& t_str)
	{
		//setup converter
		typedef std::codecvt_utf8<wchar_t> convert_type;
		std::wstring_convert<convert_type, wchar_t> converter;

		//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
		return converter.from_bytes(t_str);
	}

private:

};

// MessageBox(0, error_msg, 0, MB_ICONERROR);
