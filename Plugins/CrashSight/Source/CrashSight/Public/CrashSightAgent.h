#pragma once
#include "UQMMacros.h"
#include "UQMCrash.h"
#if PLATFORM_WINDOWS || defined(CRASHSIGHT_XSX)
#include "Windows/WindowsHWrapper.h"
#endif

namespace GCloud {
    namespace CrashSight {
        class CRASHSIGHT_API CrashSightAgent {
            typedef void (*CrashCallbackFuncPtr)(int type, const char* guid);
        private:
            static bool is_log_path_setted_;
        public:
            /************************************全平台接口************************************/

            // 初始化CrashSight
            // appId：app_id
            static void InitWithAppId(const char* app_id);

            // 上报错误（wchar_t版本仅供Windows端使用）
            // type：异常类型
            // name：异常名称
            // reason：异常信息
            // stack_trace：堆栈
            // extras：其他信息
            // quit：是否退出
            // dump_native_type：0：关闭，1：调用系统接口dump，3：minidump（仅移动端）
            // is_async：是否异步（Win、Xbox有效）
            // error_attach_path：日志附件路径（Win、Xbox有效）
            static void ReportException(int type, const char* name, const char* reason, const char* stack_trace,
                                        const char* extras, bool quit, int dump_native_type = 0, bool is_async = true,
                                        const char *error_attach_path = nullptr);

            static void ReportExceptionW(int type, const char* name, const char* reason, const char* stack_trace,
                                         const char* extras, bool quit, int dump_native_type = 0, bool is_async = true,
                                         const wchar_t *error_attach_path = nullptr);

            // 设置用户ID
            // user_id：用户ID
            static void SetUserId(const char* user_id);

            // 添加自定义数据
            // key：键
            // value：值
            static void AddSceneData(const char* key, const char* value);
            static void SetUserValue(const char* key, int value);
            static void SetUserValue(const char* key, double value);
            static void SetUserValue(const char* key, const char* value);
            static void SetUserValue(const char* key, const char** values, int count);

            // 设置应用版本号
            // app_version：版本号
            static void SetAppVersion(const char* app_version);

            // 设置上报域名
            // crash_server_url：上报域名
            static void ConfigCrashServerUrl(const char* crash_server_url);

            // 设置自定义日志路径（Switch不可用）（wchar_t版本仅供Windows端使用）
            // logPath：日志路径
            static void SetLogPath(const char* log_path);

            static void SetLogPathW(const wchar_t* log_path);

            // debug开关
            static void ConfigDebugMode(bool enable);

            // 设置deviceId
            // device_id：设备唯一标识
            static void SetDeviceId(const char* device_id);

            // 设置日志上报等级
            // log_level：Off=0,Error=1,Warn=2,Info=3,Debug=4
            static void ConfigCrashReporter(int log_level);

            // 添加自定义日志
            // level：日志级别
            // format：日志格式
            // args：可变参数
            static void PrintLog(LogSeverity level, const char* format, ...);

            // 测试native崩溃
            static void TestNativeCrash();

            // 设置子场景（PS4、PS5、Switch、Linux暂不可用）
            // serverEnv:子场景名称
            static void SetEnvironmentName(const char *serverEnv);
            static void setServerEnv(const char *serverEnv);

            // 设置回调（PS4、PS5、Switch、Linux暂不可用）
            // crashObserver:回调类，需自行定义，继承UQMCrashObserver
            static void SetCrashObserver(UQM::UQMCrashObserver* crashObserver);

            // 设置引擎信息（PS4、PS5、Switch、Linux暂不可用）
            // 插件自动调用
            static void SetEngineInfo();

            // 当崩溃发生时，获取崩溃线程ID，失败时返回-1（PS4、PS5、Switch、Linux暂不可用）
            static long GetCrashThreadId();

            /************************************移动端接口************************************/

            // 各类上报的回调开关（Android、iOS）
            // callback_type：目前是5种类型，用5位表示。第一位表示crash，第二位表示anr，第三位表示u3d c#
            // error，第四位表示js，第五位表示lua
            static void ConfigCallbackType(int32_t callback_type);

            // 设置deviceModel（Android、iOS）
            // device_model：手机型号
            static void SetDeviceModel(const char* device_model);

            // 上报错误（Android、iOS）
            // type：异常类型
            // exception_name：异常名
            // exception_msg：异常消息
            // exception_stack：堆栈
            // params_json：map序列化后的JSON字符串
            // dump_native_type：0：关闭，1：调用系统接口dump，3：minidump
            static void ReportExceptionJson(int type, const char* exception_name, const char* exception_msg,
                                            const char* exception_stack, const char* params_json, int dump_native_type = 0, const char* errorAttachmentPath = "");

            // Report log statistics（Android、iOS）
            // msg_type：消息类型
            // msg：消息详情
            static void ReportLogInfo(const char* msg_type, const char* msg);

            // 设置场景（Android、iOS）
            // scene_id：场景ID
            // upload：是否上报
            static void SetScene(const char *scene_id, bool upload = false);
            static void SetScene(int scene_id);

            // 捕捉来自不同线程的多个信号，并上传第一个信号的信息（Android、iOS）
            static void SetCatchMultiSignal(bool enable);

            // 最多回溯256帧，即使栈字符串已满，也会上报最后一帧（Android、iOS）
            static void SetUnwindExtraStack(bool enable);

            // 设置自定义device ID（Android、iOS）
            // device_id:device ID
            static void SetCustomizedDeviceID(const char* device_id);

            // 获取SDK生成的device ID（Android、iOS）
            // data:指向用于存放device ID的内存
            // len:data的长度
            static void GetSDKDefinedDeviceID(void* data, int len);

            // 设置自定义match ID（Android、iOS）
            // match_id:match ID
            static void SetCustomizedMatchID(const char* match_id);

            // 获取SDK生成的session ID（Android、iOS）
            // data:指向用于存放session ID的内存
            // len:data的长度
            static void GetSDKSessionID(void* data, int len);

            // 上次是否为崩溃退出（Android、iOS）
            static bool IsLastSessionCrash();

            // 获取上次的用户ID（Android、iOS）
            // data:指向用于存放用户ID的内存
            // len:data的长度
            static void GetLastSessionUserId(void* data, int len);

            // 设置上报线程数，默认为 3（Android、iOS）
            static void SetUploadThreadNum(int num);

            // 测试oom（Android、iOS）
            static void TestOomCrash();

            // 设置game type（仅Android）
            // game_type：COCOS=1, UNITY=2, UNREAL=3
            static void SetGameType(int game_type);

            // 测试java崩溃（Android）
            static void TestJavaCrash();

            // 测试ANR（Android）
            static void TestANR();

            // 获取崩溃UUID（Android）
            // data:指向用于存放崩溃UUID的内存
            // len:data的长度
            static void GetCrashUuid(void* data, int len);

            // 设置logcat缓存大小（Android）
            static void SetLogcatBufferSize(int size);

            // 测试objective-C崩溃（iOS）
            static void TestOcCrash();

            // 启动定时dump（Android）
            // dumpMode:dump模式，1：dump，2：minidump
            // startTimeMode:启动时间模式，0：绝对时间，1：相对时间，单位：毫秒
            // startTime:启动时间
            // dumpInterval:dump间隔，单位：毫秒
            // dumpTimes:dump次数
            // saveLocal:是否保存本地
            // savePath:本地保存路径
            static void StartDumpRoutine(int dumpMode, int startTimeMode, long startTime, long dumpInterval,
                                         int dumpTimes, bool saveLocal, const char *savePath);

            // 监控FD数量（Android）
            // interval:扫描间隔，单位：毫秒
            // limit:FD数量限制，超过将触发一次上报
            // dumpType:dump方式，0：不上报dump，1：系统接口dump，3：minidump
            static void StartMonitorFdCount(int interval, int limit, int dumpType);

            // 获取异常类型编号（Android、iOS）
            // name:异常类型名，如“c#”、“js”、“lua”、“custom1”等
            // 返回值:异常类型编号，可用于填写ReportException接口的type
            static int GetExceptionType(const char *name);

            // 测试释放后使用内存（Android）
            static void TestUseAfterFree();

            // 是否允许获取PackageInfo（Android）
            // 默认开，推荐开启，仅在遇到合规问题时关闭。关闭会导致无法自动获取版本号。
            static void setEnableGetPackageInfo(bool enable);

            static bool checkFdCount(int limit, int dumpType, bool upload);

            // 设置oom日志路径（Android、iOS）
            // logPath：日志路径
            static void SetOomLogPath(const char* log_path);

            static void CloseCrashReport();

            static void StartCrashReport();

            static void RestartCrashReport();

            /************************************PC、Xbox端接口************************************/

            // VEH异常捕获开关（Win、Xbox）
            static void SetVehEnable(bool enable);

            // 主动上报崩溃（Win、Xbox）
            static void ReportCrash();

            // 主动上报dump（Win、Xbox）
            // dump_path：dump目录
            // is_async：是否异步
            static void ReportDump(const char* dump_path, bool is_async);

            // 设置崩溃回调（Win、Xbox）
            // callback：崩溃回调函数指针
            static void SetCrashCallback(CrashCallbackFuncPtr callback);

            // 额外异常捕获开关（Win、Xbox）
            static void SetExtraHandler(bool extra_handle_enable);

            // 上传dump文件（Win、Xbox）
            // dump_dir：dump文件目录
            // is_extra_check：默认填false即可
            static void UploadGivenPathDump(const char* dump_dir, bool is_extra_check);

            // UE Critical Error上报开关（Win、Xbox）
            static void UnrealCriticalErrorEnable(bool enable);

            // 仅上报首个捕获的崩溃，开启VEH捕获后才生效（Win、Xbox）
            static void OnlyUploadFirstCrash(bool enable);

            // 设置dump类型（Win、Xbox）
            // dump_type：dump类型，详见Windows官方定义
            static void SetDumpType(int dump_type);

            // 设置可用的异常类型，使用前请咨询CrashSight开发人员（Win、Xbox）
            // exp_code：异常代码，详见Windows官方定义
            static void AddValidExpCode(unsigned long exp_code);

            // 根据问题GUID上报（Win、Xbox）
            // guid：指向唯一问题的代码，可通过回调获取
            static void UploadCrashWithGuid(const char *guid);

            // 崩溃上报开关（Win、Xbox）
            static void SetCrashUploadEnable(bool enable);

            // 设置工作空间（Win、Xbox）
            // workspace：工作空间的绝对路径
            static void SetWorkSpace(const char *workspace);

            static void SetWorkSpaceW(const wchar_t *workspace);

            // 设置自定义附件路径（Switch不可用）（Win、Xbox）
            // attach_path：附件路径
            static void SetAttachPath(const char* attach_path);

            static void SetAttachPathW(const wchar_t* attach_path);

            /************************************PS4、PS5、Switch、Linux端接口************************************/

            // 设置错误上报间隔（PS4、PS5、Switch）
            // interval：错误上报间隔（单位：秒）
            static void SetErrorUploadInterval(int interval);

            // 错误上报开关（PS4、PS5、Switch）
            static void SetErrorUploadEnable(bool enable);

            // 设置所有记录文件的路径(Linux)
            // record_dir:记录文件的路径
            static void SetRecordFileDir(const char* record_dir);

            /************************************已废弃接口************************************/

            // 初始化（Win、Xbox）
            // user_id：用户id（Win、Xbox）
            // version：应用版本号
            // key：appkey
            CS_DEPRECATED("InitContext is deprecated. Use InitWithAppId instead")
            static int InitContext(const char* user_id, const char* version, const char* key);

            // 初始化（Linux）
            // app_id:appid
            // app_key:appkey
            // app_version:版本号
            CS_DEPRECATED("Init is deprecated. Use InitWithAppId instead")
            static void Init(const char* app_id, const char* app_key, const char* app_version);

            // 崩溃捕获开关（Win、Xbox，2.1.1后无效）
            CS_DEPRECATED("MonitorEnable is deprecated since version 2.1.1")
            static void MonitorEnable(bool enable);

            // 初始化前填写配置信息（Android、iOS、Switch可用，已弃用）
            // channel：已弃用，默认填“”即可
            // version：应用版本号
            // user：用户id
            // delay：延时
            CS_DEPRECATED("ConfigDefault is deprecated. Use SetUserId and SetAppVersion instead")
            static void ConfigDefault(const char* channel, const char* version, const char* user, long delay);

            CS_DEPRECATED("GetPlatformCode is deprecated.")
            static int GetPlatformCode();
        };
    }  // namespace CrashSight
}  // namespace GCloud
