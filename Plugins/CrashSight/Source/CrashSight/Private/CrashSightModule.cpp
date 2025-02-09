#include "CrashSightModule.h"

#define LOCTEXT_NAMESPACE "FCrashSightModule"

void FCrashSightModule::StartupModule()
{
    // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#if defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5)
    SceKernelModule mod = sceKernelLoadStartModule("/app0/prx/libcs.prx", 0, NULL, 0, NULL, NULL);
    if (mod < 0)
    {
        // error
        csPSMod = -1;
    }
    else
    {
        csPSMod = mod;
    }
#endif
}

void FCrashSightModule::ShutdownModule()
{
#if defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5)
    // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
    // we call this function before unloading the module.
    SceKernelModule mod = csPSMod;
    if (csPSMod > 0)
    {
        int iRet = sceKernelStopUnloadModule(mod, 0, NULL, 0, NULL, NULL);
        if (iRet < 0)
        {
            // error
        }
        csPSMod = -1;
    }
#endif
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCrashSightModule, CrashSight)