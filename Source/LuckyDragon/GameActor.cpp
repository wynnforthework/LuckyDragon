// Fill out your copyright notice in the Description page of Project Settings.


#include "GameActor.h"

#include "DemoMainMenu.h"
#include "GameSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "CrashSightAgent.h" //请包含该头文件
using namespace GCloud::CrashSight;


void AGameActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"),*FString(__FUNCTION__));

	FString AppID;
    const FString DefaultGamePath = FString::Printf(TEXT("%sDeepSeek.ini"), *FPaths::SourceConfigDir());
    const FString NormalizePath = FConfigCacheIni::NormalizeConfigIniPath(DefaultGamePath);
    GConfig->GetString(TEXT("CrashSight"), TEXT("AppID"), AppID, NormalizePath);
	AppIDChar = TCHAR_TO_ANSI(*AppID);
	// 设置上报域名，请根据项目发行需求进行填写。（必填）
	CrashSightAgent::ConfigCrashServerUrl("https://android.crashsight.wetest.net/pb/async");
	// 设置上报所指向的APP ID, 并进行初始化。APP ID可以在管理端更多->产品设置->产品信息中找到。（必填）
	CrashSightAgent::InitWithAppId(AppIDChar);
	// 设置游戏类型，Cocos=1，Unity=2，UE4=3，UE5=4
	CrashSightAgent::SetGameType(4);
#if DEBUG	
	CrashSightAgent::ConfigDebugMode(true);
#endif
	// CrashSightAgent::TestNativeCrash();
	// CrashSightAgent::TestJavaCrash();
	// Shut Mouse Cursor
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	check(PlayerController);
	PlayerController->SetShowMouseCursor(true);

	bool IsSavedExist = GetGameInstance()->GetSubsystem<UGameSubsystem>()->HasSaveData();

	if (UIMainMenu)
	{
		UDemoMainMenu* MainMenu = CreateWidget<UDemoMainMenu>(GetWorld(), UIMainMenu);
		MainMenu->AddToViewport();
		if (IsSavedExist)
		{
			MainMenu->ContinueButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			MainMenu->ContinueButton->SetVisibility(ESlateVisibility::Hidden);
		}
		//播放开始动画
		MainMenu->PlayEnterAnimation();
	}
}
