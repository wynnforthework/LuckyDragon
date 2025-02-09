#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
#pragma warning (disable:4191)
#include "CrashSightBridgeWin.h"
#include "Windows/WindowsHWrapper.h"

#define CS_INVOKE_CHECK(is_inited, function_ptr) \
    if (!is_inited) {                            \
        InitBridge();                            \
    }                                            \
    if (!is_inited || function_ptr == nullptr) { \
        return;                                  \
    }

#define CS_INVOKE_CHECK_WITH_RETURN(is_inited, function_ptr, ret) \
    if (!is_inited) {                            \
        InitBridge();                            \
    }                                            \
    if (!is_inited || function_ptr == nullptr) { \
        return ret;                              \
    }

namespace CrashSight {
    typedef void (*CS_InitContext)(const char* id, const char* version, const char* key);
    typedef void (*CS_ReportException)(int type, const char* name, const char* message, const char* stackTrace,
                                       const char* extras, bool is_async, const char *error_attach_path);
    typedef void (*CS_ReportExceptionW)(int type, const char *name, const char *message, const char *stackTrace,
                                        const char *extras, bool is_async, const wchar_t *error_attach_path);
    typedef void (*CS_ReportCrash)();
    typedef void (*CS_ReportDump)(const char* dump_path, bool is_async);
    typedef void (*CS_SetCrashCallback)(CrashCallbackFuncPtr callback);
    typedef void (*CS_SetUserValue)(const char* key, const char* value);
    typedef void (*CS_SetVehEnable)(bool enable);
    typedef void (*CS_SetExtraHandler)(bool extra_handle_enable);
    typedef void (*CS_SetCustomLogDir)(const char* log_path);
    typedef void (*CS_SetCustomLogDirW)(const wchar_t *log_path);
    typedef void (*CS_SetUserId)(const char* user_id);
    typedef void (*CS_PrintLog)(LogSeverity level, const char* tag, const char* format, ...);
    typedef void (*CS_UploadGivenPathDump)(const char* dump_dir, bool is_extra_check);
    typedef void (*CS_UnrealCriticalErrorEnable)(bool enable);
    typedef void (*CS_InitWithAppId)(const char* app_id);
    typedef void (*CS_SetAppVersion)(const char* app_version);
    typedef void (*CS_ConfigCrashServerUrl)(const char* crash_server_url);
    typedef void (*CS_ConfigDebugMode)(bool enable);
    typedef void (*CS_SetDeviceId)(const char* device_id);
    typedef void (*CS_ConfigCrashReporter)(int log_level);
    typedef void (*CS_TestNativeCrash)();
    typedef void (*CS_SetEnvironmentName)(const char* name);
    typedef void (*CS_SetCrashObserver)(UQM::UQMCrashObserver* crashObserver);
    typedef void (*CS_OnlyUploadFirstCrash)(bool enable);
    typedef void (*CS_SetDumpType)(int dump_type);
    typedef void (*CS_AddValidExpCode)(unsigned long exp_code);
    typedef void (*CS_UploadCrashWithGuid)(const char *guid);
    typedef void (*CS_GetSessionId)(char *session_id);
    typedef void (*CS_SetCrashUploadEnable)(bool enable);
    typedef void (*CS_SetWorkSpace)(const char *workspace);
    typedef void (*CS_SetWorkSpaceW)(const wchar_t *workspace);
    typedef void (*CS_SetEngineInfo)(const char *version, const char *buildConfig, const char *language, const char *locale);
    typedef void (*CS_SetCustomAttachDir)(const char* attach_path);
    typedef void (*CS_SetCustomAttachDirW)(const wchar_t* attach_path);
    typedef int (*CS_GetCrashThreadID)();

    static CS_InitContext     init_context_ptr_ = nullptr;
    static CS_ReportException report_exception_ptr_ = nullptr;
    static CS_ReportExceptionW report_exception_w_ptr_ = nullptr;
    static CS_ReportCrash     report_crash_ptr_ = nullptr;
    static CS_ReportDump      report_dump_ptr_ = nullptr;
    static CS_SetCrashCallback set_crash_callback_ptr_ = nullptr;
    static CS_SetUserValue     set_user_value_ptr_ = nullptr;
    static CS_SetVehEnable     set_veh_enable_ptr_ = nullptr;
    static CS_SetExtraHandler  set_extra_handler_ptr_ = nullptr;
    static CS_SetCustomLogDir  set_custom_log_dir_ptr_ = nullptr;
    static CS_SetCustomLogDirW set_custom_log_dir_w_ptr_ = nullptr;
    static CS_SetUserId        set_user_id_ptr_ = nullptr;
    static CS_PrintLog            print_log_ptr_ = nullptr;
    static CS_UploadGivenPathDump upload_given_path_dump_ptr_ = nullptr;
    static CS_UnrealCriticalErrorEnable unreal_critical_error_enable_ptr_ = nullptr;
    static CS_InitWithAppId init_with_appid_ = nullptr;
    static CS_ConfigCrashServerUrl config_crash_sever_url_ = nullptr;
    static CS_ConfigDebugMode config_debug_mode_ = nullptr;
    static CS_SetDeviceId set_device_id_ = nullptr;
    static CS_ConfigCrashReporter config_crash_reporter_ = nullptr;
    static CS_TestNativeCrash test_native_crash_ = nullptr;
    static CS_SetAppVersion set_app_version_ = nullptr;
    static CS_SetEnvironmentName set_environment_name_ = nullptr;
    static CS_SetCrashObserver set_crash_observer_ = nullptr;
    static CS_OnlyUploadFirstCrash      only_upload_first_crash_ptr_ = nullptr;
    static CS_SetDumpType               set_dump_type_ptr_ = nullptr;
    static CS_AddValidExpCode           add_vaild_exp_code_ptr_ = nullptr;
    static CS_UploadCrashWithGuid       upload_crash_with_guid_ptr_ = nullptr;
    static CS_GetSessionId              get_session_id_ptr_ = nullptr;
    static CS_SetCrashUploadEnable      set_crash_upload_enable_ptr_ = nullptr;
    static CS_SetWorkSpace              set_work_space_ptr_ = nullptr;
    static CS_SetWorkSpaceW             set_work_space_w_ptr_ = nullptr;
    static CS_SetEngineInfo             set_engine_info_ptr_ = nullptr;
    static CS_SetCustomAttachDir        set_custom_attach_dir_ptr_ = nullptr;
    static CS_SetCustomAttachDirW       set_custom_attach_dir_w_ptr_ = nullptr;
    static CS_GetCrashThreadID          get_crash_thread_id_ptr_ = nullptr;

    bool CrashSightBridge::is_inited_ = false;

    int CrashSightBridge::InitBridge() {
        HINSTANCE crashsight_dll_ = LoadLibrary(L"CrashSight64.dll");
        if (crashsight_dll_ != nullptr) {
                init_context_ptr_ = (CS_InitContext)GetProcAddress(crashsight_dll_, "CS_InitContext");
                report_exception_ptr_ = (CS_ReportException)GetProcAddress(crashsight_dll_, "CS_ReportException");
                report_exception_w_ptr_ = (CS_ReportExceptionW)GetProcAddress(crashsight_dll_, "CS_ReportExceptionW");
                report_crash_ptr_ = (CS_ReportCrash)GetProcAddress(crashsight_dll_, "CS_ReportCrash");
                report_dump_ptr_ = (CS_ReportDump)GetProcAddress(crashsight_dll_, "CS_ReportDump");
                set_crash_callback_ptr_ = (CS_SetCrashCallback)GetProcAddress(crashsight_dll_, "CS_SetCrashCallback");
                set_user_value_ptr_ = (CS_SetUserValue)GetProcAddress(crashsight_dll_, "CS_SetUserValue");
                set_veh_enable_ptr_ = (CS_SetVehEnable)GetProcAddress(crashsight_dll_, "CS_SetVehEnable");
                set_extra_handler_ptr_ = (CS_SetExtraHandler)GetProcAddress(crashsight_dll_, "CS_SetExtraHandler");
                set_custom_log_dir_ptr_ = (CS_SetCustomLogDir)GetProcAddress(crashsight_dll_, "CS_SetCustomLogDir");
                set_custom_log_dir_w_ptr_ = (CS_SetCustomLogDirW)GetProcAddress(crashsight_dll_, "CS_SetCustomLogDirW");
                set_user_id_ptr_ = (CS_SetUserId)GetProcAddress(crashsight_dll_, "CS_SetUserId");
                print_log_ptr_ = (CS_PrintLog)GetProcAddress(crashsight_dll_, "CS_PrintLog");
                upload_given_path_dump_ptr_ = (CS_UploadGivenPathDump)GetProcAddress(crashsight_dll_, "CS_UploadGivenPathDump");
                unreal_critical_error_enable_ptr_ =
                    (CS_UnrealCriticalErrorEnable)GetProcAddress(crashsight_dll_, "CS_UnrealCriticalErrorEnable");
                init_with_appid_ = (CS_InitWithAppId)GetProcAddress(crashsight_dll_, "CS_InitWithAppId");
                set_app_version_ = (CS_SetAppVersion)GetProcAddress(crashsight_dll_, "CS_SetAppVersion");
                config_crash_sever_url_ = (CS_ConfigCrashServerUrl)GetProcAddress(crashsight_dll_, "CS_ConfigCrashServerUrl");
                config_debug_mode_ = (CS_ConfigDebugMode)GetProcAddress(crashsight_dll_, "CS_ConfigDebugMode");
                set_device_id_ = (CS_SetDeviceId)GetProcAddress(crashsight_dll_, "CS_SetDeviceId");
                config_crash_reporter_ = (CS_ConfigCrashReporter)GetProcAddress(crashsight_dll_, "CS_ConfigCrashReporter");
                test_native_crash_ = (CS_TestNativeCrash)GetProcAddress(crashsight_dll_, "CS_TestNativeCrash");
                set_environment_name_ = (CS_SetEnvironmentName)GetProcAddress(crashsight_dll_, "CS_SetEnvironmentName");
                set_crash_observer_ = (CS_SetCrashObserver)GetProcAddress(crashsight_dll_, "CS_SetCrashObserver");
                only_upload_first_crash_ptr_ = (CS_OnlyUploadFirstCrash)GetProcAddress(crashsight_dll_, "CS_OnlyUploadFirstCrash");
                set_dump_type_ptr_              = (CS_SetDumpType)GetProcAddress(crashsight_dll_, "CS_SetDumpType");
                add_vaild_exp_code_ptr_         = (CS_AddValidExpCode)GetProcAddress(crashsight_dll_, "CS_AddValidExpCode");
                upload_crash_with_guid_ptr_     = (CS_UploadCrashWithGuid)GetProcAddress(crashsight_dll_, "CS_UploadCrashWithGuid");
                get_session_id_ptr_             = (CS_GetSessionId)GetProcAddress(crashsight_dll_, "CS_GetSessionId");
                set_crash_upload_enable_ptr_    = (CS_SetCrashUploadEnable)GetProcAddress(crashsight_dll_, "CS_SetCrashUploadEnable");
                set_work_space_ptr_             = (CS_SetWorkSpace)GetProcAddress(crashsight_dll_, "CS_SetWorkSpace");
                set_work_space_w_ptr_           = (CS_SetWorkSpaceW)GetProcAddress(crashsight_dll_, "CS_SetWorkSpaceW");
                set_engine_info_ptr_            = (CS_SetEngineInfo)GetProcAddress(crashsight_dll_, "CS_SetEngineInfo");
                set_custom_attach_dir_ptr_      = (CS_SetCustomAttachDir)GetProcAddress(crashsight_dll_, "CS_SetCustomAttachDir");
                set_custom_attach_dir_w_ptr_    = (CS_SetCustomAttachDirW)GetProcAddress(crashsight_dll_, "CS_SetCustomAttachDirW");
                get_crash_thread_id_ptr_        = (CS_GetCrashThreadID)GetProcAddress(crashsight_dll_, "CS_GetCrashThreadID");

            is_inited_ = true;
            return 0;
        } else {
            return -1;
        }
    }
    void CrashSightBridge::MonitorEnable(bool enable) {}
    int CrashSightBridge::InitContext(const char *user_id, const char *version, const char *key) {
        CS_INVOKE_CHECK_WITH_RETURN(is_inited_, init_context_ptr_, -1)
        init_context_ptr_(user_id, version, key);
        return 0;
    }
    void CrashSightBridge::ReportException(int type, const char *exp_name, const char *exp_message, const char *stack_trace,
                                           const char *extras, bool is_async, const char *error_attach_path) {
        CS_INVOKE_CHECK(is_inited_, report_exception_ptr_)
        report_exception_ptr_(type, exp_name, exp_message, stack_trace, extras, is_async, error_attach_path);
    }
    void CrashSightBridge::ReportExceptionW(int type, const char *exp_name, const char *exp_message, const char *stack_trace,
                                           const char *extras, bool is_async, const wchar_t *error_attach_path) {
        CS_INVOKE_CHECK(is_inited_, report_exception_w_ptr_)
        report_exception_w_ptr_(type, exp_name, exp_message, stack_trace, extras, is_async, error_attach_path);
    }
    void CrashSightBridge::ReportCrash() {
        CS_INVOKE_CHECK(is_inited_, report_crash_ptr_)
        report_crash_ptr_();
    }
    void CrashSightBridge::ReportDump(const char *dump_path, bool is_async) {
        CS_INVOKE_CHECK(is_inited_, report_dump_ptr_)
        report_dump_ptr_(dump_path, is_async);
    }
    void CrashSightBridge::SetCrashCallback(CrashCallbackFuncPtr callback) {
        CS_INVOKE_CHECK(is_inited_, set_crash_callback_ptr_)
        set_crash_callback_ptr_(callback);
    }
    void CrashSightBridge::SetUserValue(const char *key, const char *value) {
        CS_INVOKE_CHECK(is_inited_, set_user_value_ptr_)
        set_user_value_ptr_(key, value);
    }
    void CrashSightBridge::SetVehEnable(bool enable) {
        CS_INVOKE_CHECK(is_inited_, set_veh_enable_ptr_)
        set_veh_enable_ptr_(enable);
    }
    void CrashSightBridge::SetExtraHandler(bool extra_handle_enable) {
        CS_INVOKE_CHECK(is_inited_, set_extra_handler_ptr_)
        set_extra_handler_ptr_(extra_handle_enable);
    }
    void CrashSightBridge::SetCustomLogDir(const char *log_path) {
        CS_INVOKE_CHECK(is_inited_, set_custom_log_dir_ptr_)
        set_custom_log_dir_ptr_(log_path);
    }
    void CrashSightBridge::SetCustomLogDirW(const wchar_t *log_path) {
        CS_INVOKE_CHECK(is_inited_, set_custom_log_dir_w_ptr_)
        set_custom_log_dir_w_ptr_(log_path);
    }
    void CrashSightBridge::SetUserId(const char *user_id) {
        CS_INVOKE_CHECK(is_inited_, set_user_id_ptr_)
        set_user_id_ptr_(user_id);
    }
    void CrashSightBridge::PrintLog(LogSeverity level, const char *tag, const char *format, ...) {
        CS_INVOKE_CHECK(is_inited_, print_log_ptr_)
        print_log_ptr_(level, tag, format);
    }
    void CrashSightBridge::UploadGivenPathDump(const char *dump_dir, bool is_extra_check) {
        CS_INVOKE_CHECK(is_inited_, upload_given_path_dump_ptr_)
        upload_given_path_dump_ptr_(dump_dir, is_extra_check);
    }
    void CrashSightBridge::UnrealCriticalErrorEnable(bool enable) {
        CS_INVOKE_CHECK(is_inited_, unreal_critical_error_enable_ptr_)
        unreal_critical_error_enable_ptr_(enable);
    }

    void CrashSightBridge::ConfigDebugMode(bool enable) {
        CS_INVOKE_CHECK(is_inited_, config_debug_mode_)
        config_debug_mode_(enable);
    }

    void CrashSightBridge::ConfigCrashServerUrl(const char* crash_server_url) {
        CS_INVOKE_CHECK(is_inited_, config_crash_sever_url_)
        config_crash_sever_url_(crash_server_url);
    }

    void CrashSightBridge::SetAppVersion(const char* app_version) {
        CS_INVOKE_CHECK(is_inited_, set_app_version_)
        set_app_version_(app_version);
    }

    void CrashSightBridge::ConfigCrashReporter(int log_level) {
        CS_INVOKE_CHECK(is_inited_, config_crash_reporter_)
        config_crash_reporter_(log_level);
    }

    void CrashSightBridge::SetDeviceId(const char* device_id) {
        CS_INVOKE_CHECK(is_inited_, set_device_id_)
        set_device_id_(device_id);
    }

    void CrashSightBridge::GetSDKSessionID(void* data, int len) {
        if (len > 32) {
            CS_INVOKE_CHECK(is_inited_, get_session_id_ptr_)
            get_session_id_ptr_((char*)data);
        }
    }

    void CrashSightBridge::InitWithAppId(const char* app_id) {
        CS_INVOKE_CHECK(is_inited_, init_with_appid_)
        init_with_appid_(app_id);
    }

    void CrashSightBridge::TestNativeCrash() {
        CS_INVOKE_CHECK(is_inited_, test_native_crash_)
        test_native_crash_();
    }

    void CrashSightBridge::SetEnvironmentName(const char *serverEnv) {
        CS_INVOKE_CHECK(is_inited_, set_environment_name_)
        set_environment_name_(serverEnv);
    }

    void CrashSightBridge::SetCrashObserver(UQM::UQMCrashObserver* crashObserver) {
        CS_INVOKE_CHECK(is_inited_, set_crash_observer_)
        set_crash_observer_(crashObserver);
    }

    void CrashSightBridge::OnlyUploadFirstCrash(bool enable) {
        CS_INVOKE_CHECK(is_inited_, only_upload_first_crash_ptr_)
        only_upload_first_crash_ptr_(enable);
    }

    void CrashSightBridge::SetDumpType(int dump_type) {
        CS_INVOKE_CHECK(is_inited_, set_dump_type_ptr_)
        set_dump_type_ptr_(dump_type);
    }

    void CrashSightBridge::AddValidExpCode(unsigned long exp_code) {
        CS_INVOKE_CHECK(is_inited_, add_vaild_exp_code_ptr_)
        add_vaild_exp_code_ptr_(exp_code);
    }

    void CrashSightBridge::UploadCrashWithGuid(const char *guid) {
        CS_INVOKE_CHECK(is_inited_, upload_crash_with_guid_ptr_)
        upload_crash_with_guid_ptr_(guid);
    }

    void CrashSightBridge::SetCrashUploadEnable(bool enable) {
        CS_INVOKE_CHECK(is_inited_, set_crash_upload_enable_ptr_)
        set_crash_upload_enable_ptr_(enable);
    }

    void CrashSightBridge::SetWorkSpace(const char *workspace) {
        CS_INVOKE_CHECK(is_inited_, set_work_space_ptr_)
        set_work_space_ptr_(workspace);
    }

    void CrashSightBridge::SetWorkSpaceW(const wchar_t *workspace) {
        CS_INVOKE_CHECK(is_inited_, set_work_space_w_ptr_)
        set_work_space_w_ptr_(workspace);
    }

    void CrashSightBridge::SetEngineInfo(const char *version, const char *buildConfig, const char *language, const char *locale) {
        CS_INVOKE_CHECK(is_inited_, set_engine_info_ptr_)
        set_engine_info_ptr_(version, buildConfig, language, locale);
    }
    void CrashSightBridge::SetCustomAttachDir(const char* attach_path) {
        CS_INVOKE_CHECK(is_inited_, set_custom_attach_dir_ptr_)
        set_custom_attach_dir_ptr_(attach_path);
    }
    void CrashSightBridge::SetCustomAttachDirW(const wchar_t* attach_path) {
        CS_INVOKE_CHECK(is_inited_, set_custom_attach_dir_w_ptr_)
        set_custom_attach_dir_w_ptr_(attach_path);
    }
    int CrashSightBridge::GetCrashThreadID() {
        CS_INVOKE_CHECK_WITH_RETURN(is_inited_, get_crash_thread_id_ptr_, -1)
        return get_crash_thread_id_ptr_();
    }
}  // namespace CrashSight
#endif
