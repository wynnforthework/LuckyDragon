#include "CrashSightAgent.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Internationalization.h"
#include "Misc/EngineVersion.h"

#ifdef CRASHSIGHT_OHOS
#include "aki/jsbind.h"
#endif
#include "CrashSightBridgeWin.h"

extern "C"
{
#if defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
    UQM_EXPORT void CS_InitWithAppId(const char* appId);
    UQM_EXPORT void CS_SetAppVersion(const char* appVersion);
    UQM_EXPORT void CS_ReportException(int type, const char* name, const char* message, const char* stack_trace,
        const char* extras, bool quit);
    UQM_EXPORT void CS_SetUserId(const char* userId);
    UQM_EXPORT void CS_EnableDebugMode(bool isDebug);
    UQM_EXPORT void CS_SetErrorUploadInterval(int interval);
    UQM_EXPORT void CS_SetCrashServerUrl(const char* crashServerUrl);
    UQM_EXPORT void CS_SetUserValue(const char* key, const char* value);
    UQM_EXPORT void CS_SetErrorUploadEnable(bool enable);
    UQM_EXPORT void CS_PrintLog(int level, const char* tag, const char* data);
    UQM_EXPORT void CS_SetDeviceId(const char* deviceId);
    UQM_EXPORT void CS_ConfigCrashReporter(int log_level);
    UQM_EXPORT void CS_TestNativeCrash();
#endif

#if defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5)
    UQM_EXPORT void CS_SetLogPath(const char* path);
#endif

#if PLATFORM_LINUX
    UQM_EXPORT void CS_ConfigCrashServerUrl(const char* user_id);  //需要传入Uid
    UQM_EXPORT void CS_Init(const char* app_id, const char* app_key, const char* app_version);
    UQM_EXPORT void CS_SetUserId(const char* user_id);
    UQM_EXPORT void CS_SetDeviceId(const char* device_id);
    UQM_EXPORT void CS_SetAppVersion(const char* app_version);
    UQM_EXPORT void CS_PrintLog(int level, const char* format, ...);
    UQM_EXPORT void CS_SetRecordFileDir(const char* record_dir);
    UQM_EXPORT void CS_SetUserValue(const char* key, const char* value);
    UQM_EXPORT void CS_SetLogPath(const char* log_path);
    UQM_EXPORT void CS_ReportException(int type, const char* name, const char* reason, const char* stackTrace,
        const char* extras, bool quit, int dumpNativeType = 0);
    UQM_EXPORT void CS_InitWithAppId(const char* app_id);
    UQM_EXPORT void CS_ConfigDebugMode(bool enable);
    UQM_EXPORT void CS_ConfigCrashReporter(int log_level);
    UQM_EXPORT void CS_TestNativeCrash();
    UQM_EXPORT void CS_CloseCrashReport();
#endif
}

#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
using CrashSight::CrashSightBridge;
#endif

namespace GCloud {
    namespace CrashSight {
        bool CrashSightAgent::is_log_path_setted_ = false;

        char* FStringToCharPtr(const FString& String)
        {
            FTCHARToUTF8 Converter(*String);
            const char* UTF8Str = (ANSICHAR*)Converter.Get();
            int32 Length = strlen(UTF8Str);
            char* Result = new char[Length + 1];
            strncpy(Result, UTF8Str, Length);
            Result[Length] = '\0';
            return Result;
        }

        void CrashSightAgent::SetEngineInfo() {
            FString fVersion = FEngineVersion::Current().ToString();
#if UE_BUILD_DEBUG
            FString fBuildConfig = TEXT("debug");
#elif UE_BUILD_DEVELOPMENT
            FString fBuildConfig = TEXT("development");
#elif UE_BUILD_TEST
            FString fBuildConfig = TEXT("test");
#elif UE_BUILD_SHIPPING
            FString fBuildConfig = TEXT("shipping");
#else
            FString fBuildConfig = TEXT("unknown");
#endif
            FString fLanguage = FInternationalization::Get().GetDefaultLanguage()->GetTwoLetterISOLanguageName();
            FString fLocale = FInternationalization::Get().GetDefaultCulture()->GetName();

            char* version = FStringToCharPtr(fVersion);
            char* buildConfig = FStringToCharPtr(fBuildConfig);
            char* language = FStringToCharPtr(fLanguage);
            char* locale = FStringToCharPtr(fLocale);

#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::setEngineInfo(version, buildConfig, language, locale);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetEngineInfo(version, buildConfig, language, locale);
#endif

            delete[] version;
            delete[] buildConfig;
            delete[] language;
            delete[] locale;
        }

        int CrashSightAgent::InitContext(const char* user_id, const char* version, const char* key) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            if (CrashSightBridge::InitContext(user_id, version, key) == 0) {
#ifdef ENGINE_MAJOR_VERSION
#ifdef ENGINE_MINOR_VERSION
#if (ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 24)) && PLATFORM_WINDOWS
            if (!is_log_path_setted_) {
                FString ProjectName = FApp::GetProjectName();
                FString LogDir = FPaths::ProjectLogDir();
                FString LogFilePath = IPlatformFile::GetPlatformPhysical().ConvertToAbsolutePathForExternalAppForRead(*LogDir);
                FString LogFileName = LogFilePath + ProjectName + ".log";

                int32 Length = LogFileName.Len() + 1;
                if (Length <= MAX_PATH) {
                    wchar_t* WideLogFileName = new wchar_t[Length];
                    wcscpy_s(WideLogFileName, Length, *LogFileName);
                    SetLogPathW(WideLogFileName);
                    delete[] WideLogFileName;
                }
            }
#endif
#endif
#endif
                SetEngineInfo();
                return 0;
            }
            return -1;
#else
            return -1;
#endif
        }

        void CrashSightAgent::ReportException(int type, const char* name, const char* reason, const char* stack_trace,
                                              const char* extras, bool quit, int dump_native_type, bool is_async, const char *error_attach_path) {
#if PLATFORM_IOS || PLATFORM_ANDROID  || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::ReportException(type, name, reason, stack_trace, extras, quit, dump_native_type, error_attach_path);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::ReportException(type, name, reason, stack_trace, extras, is_async, error_attach_path);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_ReportException(type, name, reason, stack_trace, extras, quit);
#elif PLATFORM_LINUX
            CS_ReportException(type, name, reason, stack_trace, extras, quit, dump_native_type);
#endif
        }

        void CrashSightAgent::ReportExceptionW(int type, const char* name, const char* reason, const char* stack_trace,
                                               const char* extras, bool quit, int dump_native_type, bool is_async, const wchar_t *error_attach_path) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::ReportExceptionW(type, name, reason, stack_trace, extras, is_async, error_attach_path);
#endif
        }

        void CrashSightAgent::ReportCrash() {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::ReportCrash();
#endif
        }

        void CrashSightAgent::ReportDump(const char* dump_path, bool is_async) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::ReportDump(dump_path, is_async);
#endif
        }

        void CrashSightAgent::SetCrashCallback(CrashCallbackFuncPtr callback) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetCrashCallback(callback);
#endif
        }

        void CrashSightAgent::AddSceneData(const char* key, const char* value) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::AddSceneData(key, value);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetUserValue(key, value);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_SetUserValue(key, value);
#elif PLATFORM_LINUX
            CS_SetUserValue(key, value);
#endif
        }

        void CrashSightAgent::SetUserValue(const char* key, int value) {
            int modified_key_len = strlen(key) + 3;
            char* modified_key = (char*)malloc(modified_key_len);
            if (modified_key == NULL) {
                return;
            }
            snprintf(modified_key, modified_key_len, "I#%s", key);
            int value_str_len = snprintf(NULL, 0, "%d", value) + 1;
            char* value_str = (char*)malloc(value_str_len);
            if (value_str == NULL) {
                free(modified_key);
                return;
            }
            snprintf(value_str, value_str_len, "%d", value);
            AddSceneData(modified_key, value_str);
            free(modified_key);
            free(value_str);
        }

        void CrashSightAgent::SetUserValue(const char* key, double value) {
            int modified_key_len = strlen(key) + 3;
            char* modified_key = (char*)malloc(modified_key_len);
            if (modified_key == NULL) {
                return;
            }
            snprintf(modified_key, modified_key_len, "I#%s", key);
            int value_str_len = snprintf(NULL, 0, "%f", value) + 1; // Adjust precision as needed
            char* value_str = (char*)malloc(value_str_len);
            if (value_str == NULL) {
                free(modified_key);
                return;
            }
            snprintf(value_str, value_str_len, "%f", value); // Adjust precision as needed
            AddSceneData(modified_key, value_str);
            free(modified_key);
            free(value_str);
        }

        void CrashSightAgent::SetUserValue(const char* key, const char* value) {
            int modified_key_len = strlen(key) + 3;
            char* modified_key = (char*)malloc(modified_key_len);
            if (modified_key == NULL) {
                return;
            }
            snprintf(modified_key, modified_key_len, "K#%s", key);
            AddSceneData(modified_key, value);
            free(modified_key);
        }

        void CrashSightAgent::SetUserValue(const char* key, const char** value, int count) {
            int modified_key_len = strlen(key) + 3;
            char* modified_key = (char*)malloc(modified_key_len);
            if (modified_key == NULL) {
                return;
            }
            snprintf(modified_key, modified_key_len, "S#%s", key);
            int value_str_len = 0;
            for (int i = 0; i < count; ++i) {
                value_str_len += strlen(value[i]) + 1;
            }
            char* value_str = (char*)malloc(value_str_len);
            if (value_str == NULL) {
                free(modified_key);
                return;
            }
            value_str[0] = '\0';
            for (int i = 0; i < count; ++i) {
                strcat(value_str, value[i]);
                if (i < count - 1) {
                    strcat(value_str, "#");
                }
            }
            AddSceneData(modified_key, value_str);
            free(modified_key);
            free(value_str);
        }

        void CrashSightAgent::SetVehEnable(bool enable) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetVehEnable(enable);
#endif
        }

        void CrashSightAgent::SetExtraHandler(bool extra_handle_enable) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetExtraHandler(extra_handle_enable);
#endif
        }

        void CrashSightAgent::SetLogPath(const char* log_path) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::SetLogPath(log_path);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetCustomLogDir(log_path);
#elif PLATFORM_LINUX
            CS_SetLogPath(log_path);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5)
            CS_SetLogPath(log_path);
#endif
            is_log_path_setted_ = true;
        }

        void CrashSightAgent::SetLogPathW(const wchar_t* log_path) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetCustomLogDirW(log_path);
#endif
            is_log_path_setted_ = true;
        }

        void CrashSightAgent::SetUserId(const char* user_id) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::SetUserId(user_id);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetUserId(user_id);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH) || PLATFORM_LINUX
            CS_SetUserId(user_id);
#endif
        }

        void CrashSightAgent::MonitorEnable(bool enable) {}

        void CrashSightAgent::PrintLog(LogSeverity level, const char* format, ...) {
            char log_str[4 * 1024];
            va_list list;
            va_start(list, format);
            vsnprintf(log_str, 4 * 1024 - 1, format, list);
            va_end(list);
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::PrintLog(level, log_str);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            ::CrashSight::LogSeverity logLevel = ::CrashSight::LogSeverity::LogSilent;
            switch (level) {
            case GCloud::CrashSight::LogSeverity::LogSilent:
                logLevel = ::CrashSight::LogSeverity::LogSilent;
                break;
            case GCloud::CrashSight::LogSeverity::LogError:
                logLevel = ::CrashSight::LogSeverity::LogError;
                break;
            case GCloud::CrashSight::LogSeverity::LogWarning:
                logLevel = ::CrashSight::LogSeverity::LogWarning;
                break;
            case GCloud::CrashSight::LogSeverity::LogInfo:
                logLevel = ::CrashSight::LogSeverity::LogInfo;
                break;
            case GCloud::CrashSight::LogSeverity::LogDebug:
                logLevel = ::CrashSight::LogSeverity::LogDebug;
                break;
            case GCloud::CrashSight::LogSeverity::LogVerbose:
                logLevel = ::CrashSight::LogSeverity::LogVerbose;
                break;
            }
            CrashSightBridge::PrintLog(logLevel, "Log", log_str);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_PrintLog((int)level, "Log", log_str);
#elif PLATFORM_LINUX
            CS_PrintLog(level, log_str);
#endif
        }

        void CrashSightAgent::UploadGivenPathDump(const char* dump_dir, bool is_extra_check) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::UploadGivenPathDump(dump_dir, is_extra_check);
#endif
        }

        void CrashSightAgent::UnrealCriticalErrorEnable(bool enable) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::UnrealCriticalErrorEnable(enable);
#endif
        }

        void CrashSightAgent::ConfigDebugMode(bool enable) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::ConfigDebugMode(enable);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::ConfigDebugMode(enable);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_EnableDebugMode(enable);
#elif PLATFORM_LINUX
            CS_ConfigDebugMode(enable);
#endif
        }

        void CrashSightAgent::SetErrorUploadInterval(int interval) {
#if defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_SetErrorUploadInterval(interval);
#endif
        }

        void CrashSightAgent::SetErrorUploadEnable(bool enable) {
#if defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_SetErrorUploadEnable(enable);
#endif
        }

        void CrashSightAgent::ConfigCrashServerUrl(const char* crash_server_url) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::ConfigCrashServerUrl(crash_server_url);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::ConfigCrashServerUrl(crash_server_url);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_SetCrashServerUrl(crash_server_url);
#elif PLATFORM_LINUX
            CS_ConfigCrashServerUrl(crash_server_url);
#endif
        }

        void CrashSightAgent::ConfigDefault(const char* channel, const char* version, const char* user, long delay) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::ConfigDefault(channel, version, user, delay);
#endif
        }

        void CrashSightAgent::SetAppVersion(const char* app_version) {
#if PLATFORM_IOS || PLATFORM_ANDROID|| defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::SetAppVersion(app_version);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetAppVersion(app_version);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH) || PLATFORM_LINUX
            CS_SetAppVersion(app_version);
#endif
        }

        void CrashSightAgent::SetGameType(int game_type) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::SetGameType(game_type);
#endif
        }

        void CrashSightAgent::ConfigCallbackType(int32_t callback_type) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::ConfigCallbackType(callback_type);
#endif
        }

        void CrashSightAgent::ConfigCrashReporter(int log_level) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::ConfigCrashReporter(log_level);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::ConfigCrashReporter(log_level);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_ConfigCrashReporter(log_level);
#elif PLATFORM_LINUX
            CS_ConfigCrashReporter(log_level);
#endif
        }

        void CrashSightAgent::SetDeviceId(const char* device_id) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::SetDeviceId(device_id);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetDeviceId(device_id);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_SetDeviceId(device_id);
#elif PLATFORM_LINUX
            CS_SetDeviceId(device_id);
#endif
        }

        void CrashSightAgent::SetCustomizedDeviceID(const char* device_id) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::SetCustomizedDeviceID(device_id);
#endif
        }

        void CrashSightAgent::GetSDKDefinedDeviceID(void* data, int len) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::GetSDKDefinedDeviceID(data, len);
#endif
        }

        void CrashSightAgent::SetCustomizedMatchID(const char* match_id) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::SetCustomizedMatchID(match_id);
#endif
        }

        void CrashSightAgent::GetSDKSessionID(void* data, int len) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::GetSDKSessionID(data, len);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::GetSDKSessionID(data, len);
#endif
        }

        void CrashSightAgent::SetDeviceModel(const char* device_model) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::SetDeviceModel(device_model);
#endif
        }

        void CrashSightAgent::InitWithAppId(const char* app_id) {
#if PLATFORM_ANDROID
            CrashSightMobileAgent::setIsOpengles(!FAndroidMisc::ShouldUseVulkan());
#endif
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::InitWithAppId(app_id);
#elif defined(CRASHSIGHT_OHOS)
            auto jsFunc = aki::JSBind::GetJSFunction("CrashSightClass.initContext");
            jsFunc->Invoke<int>();
            CrashSightMobileAgent::InitWithAppId(app_id);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::InitWithAppId(app_id);
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_InitWithAppId(app_id);
#elif PLATFORM_LINUX
            CS_InitWithAppId(app_id);
#endif
#ifdef ENGINE_MAJOR_VERSION
            #ifdef ENGINE_MINOR_VERSION
#if (ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 24)) && PLATFORM_WINDOWS
            if (!is_log_path_setted_) {
                FString ProjectName = FApp::GetProjectName();
                FString LogDir = FPaths::ProjectLogDir();
                FString LogFilePath = IPlatformFile::GetPlatformPhysical().ConvertToAbsolutePathForExternalAppForRead(*LogDir);
                FString LogFileName = LogFilePath + ProjectName + ".log";

                int32 Length = LogFileName.Len() + 1;
                if (Length <= MAX_PATH) {
                    wchar_t* WideLogFileName = new wchar_t[Length];
                    wcscpy_s(WideLogFileName, Length, *LogFileName);
                    SetLogPathW(WideLogFileName);
                    delete[] WideLogFileName;
                }
            }
#endif
#endif
#endif
            SetEngineInfo();
        }

        void CrashSightAgent::ReportExceptionJson(int type, const char* exception_name, const char* exception_msg, const char* exception_stack, const char* params_json, int dump_native_type, const char* errorAttachmentPath) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::ReportExceptionJson(type, exception_name, exception_msg, exception_stack, params_json, dump_native_type, errorAttachmentPath);
#endif
        }

        void CrashSightAgent::ReportLogInfo(const char* msg_type, const char* msg) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::ReportLogInfo(msg_type, msg);
#endif
        }

        void CrashSightAgent::SetScene(const char *scene_id, bool upload) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::SetScene(scene_id, upload);
#endif
        }

        void CrashSightAgent::SetScene(int scene_id) {
            int value_str_len = snprintf(NULL, 0, "%d", scene_id) + 1;
            char* value_str = (char*)malloc(value_str_len);
            if (value_str == NULL) {
                return;
            }
            snprintf(value_str, value_str_len, "%d", scene_id);
            SetScene(value_str, false);
            free(value_str);
        }

        void CrashSightAgent::SetCatchMultiSignal(bool enable) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::SetCatchMultiSignal(enable);
#endif
        }

        void CrashSightAgent::SetUnwindExtraStack(bool enable) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::SetUnwindExtraStack(enable);
#endif
        }

        long CrashSightAgent::GetCrashThreadId() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            return CrashSightMobileAgent::GetCrashThreadId();
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            return CrashSightBridge::GetCrashThreadID();
#else
            return -1;
#endif
        }

        bool CrashSightAgent::IsLastSessionCrash() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            return CrashSightMobileAgent::IsLastSessionCrash();
#else
            return false;
#endif
        }

        void CrashSightAgent::GetLastSessionUserId(void* data, int len) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::GetLastSessionUserId(data, len);
#endif
        }

        void CrashSightAgent::SetUploadThreadNum(int num) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::SetUploadThreadNum(num);
#endif
        }

        void CrashSightAgent::TestOomCrash() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::TestOomCrash();
#endif
        }

        void CrashSightAgent::TestJavaCrash() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::TestJavaCrash();
#endif
        }

        void CrashSightAgent::TestOcCrash() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::TestOcCrash();
#endif
        }

        void CrashSightAgent::TestNativeCrash() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::TestNativeCrash();
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::TestNativeCrash();
#elif defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5) || defined(CRASHSIGHT_SWITCH)
            CS_TestNativeCrash();
#elif PLATFORM_LINUX
            CS_TestNativeCrash();
#endif
        }

        void CrashSightAgent::TestANR() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::TestANR();
#endif
        }

        void CrashSightAgent::GetCrashUuid(void* data, int len) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::GetCrashUuid(data, len);
#endif
        }

        void CrashSightAgent::SetLogcatBufferSize(int size) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::SetLogcatBufferSize(size);
#endif
        }

        void CrashSightAgent::StartDumpRoutine(int dumpMode, int startTimeMode, long startTime, long dumpInterval, int dumpTimes, bool saveLocal, const char *savePath) {
#if PLATFORM_ANDROID
            CrashSightMobileAgent::startDumpRoutine(dumpMode, startTimeMode, startTime, dumpInterval, dumpTimes, saveLocal, savePath);
#endif
        }

        void CrashSightAgent::StartMonitorFdCount(int interval, int limit, int dumpType) {
#if PLATFORM_ANDROID
            CrashSightMobileAgent::startMonitorFdCount(interval, limit, dumpType);
#endif
        }

        int CrashSightAgent::GetExceptionType(const char *name) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            return  CrashSightMobileAgent::getExceptionType(name);
#else
            return 0;
#endif
        }

        void CrashSightAgent::TestUseAfterFree() {
#if PLATFORM_ANDROID
            CrashSightMobileAgent::testUseAfterFree();
#endif
        }

        void CrashSightAgent::setEnableGetPackageInfo(bool enable) {
#if PLATFORM_ANDROID
            CrashSightMobileAgent::setEnableGetPackageInfo(enable);
#endif
        }

        void CrashSightAgent::SetEnvironmentName(const char *serverEnv) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::setServerEnv(serverEnv);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetEnvironmentName(serverEnv);
#endif
        }

        void CrashSightAgent::setServerEnv(const char *serverEnv) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::setServerEnv(serverEnv);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetEnvironmentName(serverEnv);
#endif
        }

        void CrashSightAgent::SetRecordFileDir(const char* record_dir) {
#if PLATFORM_LINUX
            CS_SetRecordFileDir(record_dir);
#endif
        }

        void CrashSightAgent::Init(const char* app_id, const char* app_key, const char* app_version) {
#if PLATFORM_LINUX
            CS_Init(app_id, app_key, app_version);
#endif
        }

        void CrashSightAgent::SetCrashObserver(UQM::UQMCrashObserver* crashObserver) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            UQM::UQMCrash::SetCrashObserver(crashObserver);
#elif PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetCrashObserver(crashObserver);
#endif
        }

        int CrashSightAgent::GetPlatformCode() {
            return -1;
        }

        void CrashSightAgent::OnlyUploadFirstCrash(bool enable) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::OnlyUploadFirstCrash(enable);
#endif
        }

        void CrashSightAgent::SetDumpType(int dump_type) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetDumpType(dump_type);
#endif
        }

        void CrashSightAgent::AddValidExpCode(unsigned long exp_code) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::AddValidExpCode(exp_code);
#endif
        }

        void CrashSightAgent::UploadCrashWithGuid(const char *guid) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::UploadCrashWithGuid(guid);
#endif
        }

        void CrashSightAgent::SetCrashUploadEnable(bool enable) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetCrashUploadEnable(enable);
#endif
        }

        void CrashSightAgent::SetWorkSpace(const char *workspace) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetWorkSpace(workspace);
#endif
        }

        void CrashSightAgent::SetWorkSpaceW(const wchar_t *workspace) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetWorkSpaceW(workspace);
#endif
        }

        void CrashSightAgent::SetAttachPath(const char* attach_path) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetCustomAttachDir(attach_path);
#endif
        }

        void CrashSightAgent::SetAttachPathW(const wchar_t* attach_path) {
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
            CrashSightBridge::SetCustomAttachDirW(attach_path);
#endif
        }

        bool CrashSightAgent::checkFdCount(int limit, int dumpType, bool upload) {
#if PLATFORM_IOS || PLATFORM_ANDROID
            return CrashSightMobileAgent::checkFdCount(limit, dumpType, upload);
#endif
            return false;
        }

        void CrashSightAgent::SetOomLogPath(const char* log_path) {
#if PLATFORM_IOS || PLATFORM_ANDROID || defined(CRASHSIGHT_OHOS)
            CrashSightMobileAgent::SetOomLogPath(log_path);
#endif
        }

        void CrashSightAgent::CloseCrashReport() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::CloseCrashReport();
#endif
        }

        void CrashSightAgent::StartCrashReport() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::StartCrashReport();
#endif
        }

        void CrashSightAgent::RestartCrashReport() {
#if PLATFORM_IOS || PLATFORM_ANDROID
            CrashSightMobileAgent::RestartCrashReport();
#endif
        }
    }  // namespace CrashSight
}  // namespace GCloud
