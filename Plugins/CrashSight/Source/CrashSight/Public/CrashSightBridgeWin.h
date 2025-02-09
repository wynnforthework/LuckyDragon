#pragma once
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
#include "UQMMacros.h"
#include "UQMCrash.h"

namespace CrashSight {
enum LogSeverity
{
    LogSilent,
    LogError,
    LogWarning,
    LogInfo,
    LogDebug,
    LogVerbose
};
typedef void (*CrashCallbackFuncPtr)(int type, const char* guid);
class CRASHSIGHT_API CrashSightBridge {
private:
    static bool is_inited_;
public:
    static int  InitBridge();
    CS_DEPRECATED("MonitorEnable is deprecated since version 2.1.1")
    static void MonitorEnable(bool enable);
    static void SetVehEnable(bool enable);

    static int  InitContext(const char *user_id, const char *version, const char *key);
    static void ReportException(int type, const char *exp_name, const char *exp_message, const char *stack_trace,
                                const char *extras, bool is_async = true, const char *error_attach_path = nullptr);
    static void ReportExceptionW(int type, const char *exp_name, const char *exp_message, const char *stack_trace,
                                const char *extras, bool is_async = true, const wchar_t *error_attach_path = nullptr);
    static void ReportCrash();
    static void ReportDump(const char *dump_path, bool is_async);
    static void SetCrashCallback(CrashCallbackFuncPtr callback);
    static void SetUserValue(const char *key, const char *value);
    static void SetExtraHandler(bool extra_handle_enable);
    static void SetCustomLogDir(const char *log_path);
    static void SetCustomLogDirW(const wchar_t *log_path);
    static void SetUserId(const char *user_id);
    static void PrintLog(LogSeverity level, const char *tag, const char *format, ...);
    static void UploadGivenPathDump(const char *dump_dir, bool is_extra_check);
    static void UnrealCriticalErrorEnable(bool enable);
    static void ConfigDebugMode(bool enable);
    static void ConfigCrashServerUrl(const char* crash_server_url);
    static void SetAppVersion(const char* app_version);
    static void ConfigCrashReporter(int log_level);
    static void SetDeviceId(const char* device_id);
    static void GetSDKSessionID(void* data, int len);
    static void InitWithAppId(const char* app_id);
    static void TestNativeCrash();
    static void SetEnvironmentName(const char *serverEnv);
    static void SetCrashObserver(UQM::UQMCrashObserver* crashObserver);
    static void OnlyUploadFirstCrash(bool enable);
    static void SetDumpType(int dump_type);
    static void AddValidExpCode(unsigned long exp_code);
    static void UploadCrashWithGuid(const char *guid);
    static void SetCrashUploadEnable(bool enable);
    static void SetWorkSpace(const char *workspace);
    static void SetWorkSpaceW(const wchar_t *workspace);
    static void SetEngineInfo(const char* version, const char* buildConfig, const char* language, const char* locale);
    static void SetCustomAttachDir(const char* attach_path);
    static void SetCustomAttachDirW(const wchar_t* attach_path);
    static int GetCrashThreadID();
};

}  // namespace CrashSight
#endif
