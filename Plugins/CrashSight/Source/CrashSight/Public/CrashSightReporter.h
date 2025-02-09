#pragma once
#if PLATFORM_WINDOWS
#include "Windows/WindowsHWrapper.h"
#include "Windows/WindowsSystemIncludes.h"
#include <string>
#    pragma warning(disable : 4191)

class FCrashSightReporter {
	typedef void (*CS_ReportCrashWithExpInfo)(LPEXCEPTION_POINTERS ExceptionInfo);
	typedef void (*CS_ReportException)(int type, const char* name, const char* message, const char* stackTrace,
		const char* extras, bool is_async, const char* error_attach_path);

	static HINSTANCE CrashSightHandle;
public:
	static void ReportCrashToCrashSight(LPEXCEPTION_POINTERS ExceptionInfo) {
		if (CrashSightHandle == nullptr) {
			CrashSightHandle = LoadLibraryW(L"CrashSight64.dll");
		}
		if (CrashSightHandle != nullptr) {
			CS_ReportCrashWithExpInfo     report_crash_ptr_ = nullptr;
			report_crash_ptr_ = (CS_ReportCrashWithExpInfo)GetProcAddress(CrashSightHandle, "CS_ReportCrashWithExpInfo");
			if (report_crash_ptr_ != nullptr) {
				report_crash_ptr_(ExceptionInfo);
			}
		}
	}

	static void ReportEnsureToCrashSight(TCHAR* ErrorMsg, TCHAR* StackTrace) {
		if (CrashSightHandle == nullptr) {
			CrashSightHandle = LoadLibraryW(L"CrashSight64.dll");
		}
		if (CrashSightHandle != nullptr) {
			CS_ReportException     report_ensure_ptr_ = nullptr;
			report_ensure_ptr_ = (CS_ReportException)GetProcAddress(CrashSightHandle, "CS_ReportException");
			if (report_ensure_ptr_ != nullptr) {
				std::string cErrorMsg(TCHAR_TO_UTF8(ErrorMsg));
				std::string cStackTrace(TCHAR_TO_UTF8(StackTrace));
				report_ensure_ptr_(7, "Ensure failed", cErrorMsg.c_str(), cStackTrace.c_str(), "", false, "");
			}
		}
	}
};
HINSTANCE FCrashSightReporter::CrashSightHandle = nullptr;
#    pragma warning(default : 4191)
#endif