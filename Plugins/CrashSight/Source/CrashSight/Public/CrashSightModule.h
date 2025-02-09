// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#if defined(_GPM_PS4_) || defined(_GPM_PS5_)
#include <kernel.h>
#endif
class FCrashSightModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
#if defined(CRASHSIGHT_PS4) || defined(CRASHSIGHT_PS5)
	int csPSMod = -1;
#endif
	
};
