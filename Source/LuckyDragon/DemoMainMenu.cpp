// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoMainMenu.h"

#include "GameSubsystem.h"
#include "MySaveGame.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UDemoMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UDemoMainMenu::Initialize()
{
	Super::Initialize();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UDemoMainMenu::StartGame);
	}
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UDemoMainMenu::ContinueGame);
	}
	if (EndButton)
	{
		EndButton->OnClicked.AddDynamic(this, &UDemoMainMenu::QuitGame);
	}

	UButton* SureButton = Cast<UButton>(PopupSureWidget->GetWidgetFromName(TEXT("Button")));
	if (SureButton)
	{
		SureButton->OnClicked.AddDynamic(this,&UDemoMainMenu::ClosePopup);
	}
	UButton* NextDayButton = Cast<UButton>(NextDayWidget->GetWidgetFromName(TEXT("Button")));
	if (NextDayButton)
	{
		NextDayButton->OnClicked.AddDynamic(this,&UDemoMainMenu::NextDay);
	}
	HideAllPanel();
	NewGameState = 0;
	return true;
}

void UDemoMainMenu::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"), *FString(__FUNCTION__));

	GetGameInstance()->GetSubsystem<UGameSubsystem>()->NewSave();

	LoginPanel->SetVisibility(ESlateVisibility::Hidden);
	PlayStory();
	RequestAIData();
}

void UDemoMainMenu::ContinueGame()
{
	LoginPanel->SetVisibility(ESlateVisibility::Hidden);
	HomePanel->SetVisibility(ESlateVisibility::Visible);
}

void UDemoMainMenu::QuitGame()
{
	UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"), *FString(__FUNCTION__));

	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,false);
}

void UDemoMainMenu::AfterDelay()
{
	UE_LOG(LogTemp,Warning,TEXT("[wyh] [%s] Delay Complete"),*FString(__FUNCTION__));
}

void UDemoMainMenu::PlayEnterAnimation()
{
	if (enter != nullptr)
	{
		LogoPanel->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(enter,0.0f,1,EUMGSequencePlayMode::Forward,1.0f);
		FTimerHandle DelayTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle,[this]()
		{
			LogoPanel->SetVisibility(ESlateVisibility::Hidden);
			LoginPanel->SetVisibility(ESlateVisibility::Visible);
		},2.0f,false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("enter is not found!"));	
	}
}
void UDemoMainMenu::PlayStory()
{
	StoryPanel->SetVisibility(ESlateVisibility::Visible);
	StartTypewriterEffect(TEXT("醒醒\n醒醒\n大龙集团法人龙爷：从今天开始，冻结你的所有银行卡，每天只给你10000元生活费，不让你吃点苦看来你是不会听话的。"), 0.1f);
}

void UDemoMainMenu::HideAllPanel()
{
	LogoPanel->SetVisibility(ESlateVisibility::Hidden);
	LoginPanel->SetVisibility(ESlateVisibility::Hidden);
	StoryPanel->SetVisibility(ESlateVisibility::Hidden);
	HomePanel->SetVisibility(ESlateVisibility::Hidden);
	PopupPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UDemoMainMenu::OnRequestComplete(UVaRestRequestJSON * Result) {
	UE_LOG(LogTemp, Display, TEXT("Response Complete"));
	TArray<UVaRestJsonObject*> VaRestJsonObjects = Result->GetResponseObject()->GetObjectArrayField(TEXT("choices"));
	if (VaRestJsonObjects.Num()>0)
	{
		UVaRestJsonObject* VaRestJsonObject = VaRestJsonObjects[0]->GetObjectField(TEXT("message"));
		FString role = VaRestJsonObject->GetStringField(TEXT("role"));
		FString content = VaRestJsonObject->GetStringField(TEXT("content"));
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s] role:%s content:%s"), *FString(__FUNCTION__), *role, *content);
		GEngine->AddOnScreenDebugMessage(1,2,FColor::Red,content);
		TextAI->SetText(FText::FromString("创建完成"));
		CheckGameState();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"), *FString(__FUNCTION__));
		CheckGameState();
	}
}
void UDemoMainMenu::StartTypewriterEffect(const FString& TextToType, float Interval)
{
	FullText = TextToType;
	DisplayedText = "";
	CurrentCharacterIndex = 0;

	GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		TypingTimerHandle,
		this,
		&UDemoMainMenu::TypeNextCharacter,
		Interval,
		true
	);
}

void UDemoMainMenu::TypeNextCharacter()
{
	if (CurrentCharacterIndex < FullText.Len())
	{
		DisplayedText += FullText.Mid(CurrentCharacterIndex, 1);
		CurrentCharacterIndex++;
		TextStory->SetText(FText::FromString(DisplayedText));
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
		CheckGameState();
	}
}
void UDemoMainMenu::RequestAIData()
{
	UVaRestSubsystem * VaRestSubsystem = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
	UVaRestRequestJSON * RequestJSON = VaRestSubsystem->ConstructVaRestRequestExt(EVaRestRequestVerb::POST,EVaRestRequestContentType::json);
	UVaRestJsonObject* JsonObject = UVaRestSubsystem::StaticConstructVaRestJsonObject();
	JsonObject->SetStringField(TEXT("role"), TEXT("user"));
	JsonObject->SetStringField(TEXT("content"), TEXT("Hello!"));
	const TArray<UVaRestJsonObject*> JsonArray = {JsonObject};
	UVaRestJsonObject* JsonObject2 = UVaRestSubsystem::StaticConstructVaRestJsonObject();
	JsonObject2->SetStringField(TEXT("model"), TEXT("deepseek-chat"));
	JsonObject2->SetObjectArrayField(TEXT("messages"),JsonArray);
	RequestJSON->SetRequestObject(JsonObject2);
	FString ApiKey;
	const FString DefaultGamePath = FString::Printf(TEXT("%sDeepSeek.ini"), *FPaths::SourceConfigDir());
	GConfig->GetString(TEXT("DeepSeek"), TEXT("ApiKey"), ApiKey, DefaultGamePath);
	RequestJSON->SetHeader(TEXT("Authorization"), ApiKey);
	RequestJSON->SetHeader(TEXT("Content-Type"),TEXT("application/json"));
	RequestJSON->OnRequestComplete.AddDynamic(this, &UDemoMainMenu::OnRequestComplete);
	FLatentActionInfo LatentInfo = FLatentActionInfo();
	RequestJSON->ApplyURL(TEXT("https://api.deepseek.com/chat/completions"),ResultObject,this,LatentInfo);
}
void UDemoMainMenu::CheckGameState()
{
	NewGameState++;
	if (NewGameState == 2)
	{
		StoryPanel->SetVisibility(ESlateVisibility::Hidden);
		HomePanel->SetVisibility(ESlateVisibility::Visible);
		ShowPopup();
	}
}
void UDemoMainMenu::ShowPopup()
{
	PopupPanel->SetVisibility(ESlateVisibility::Visible);
}
void UDemoMainMenu::ClosePopup()
{
	PopupPanel->SetVisibility(ESlateVisibility::Hidden);

	auto PlayerData = GetGameInstance()->GetSubsystem<UGameSubsystem>()->PlayerData;
	PlayerData->Gold += 10000;
	if (TextGold != nullptr)
	{
		TextGold->SetText(FText::FromString(LexToString(PlayerData->Gold)));
	}
	if (TextDay != nullptr)
	{
		TextDay->SetText(FText::FromString(LexToString(PlayerData->Day)));
	}
}
void UDemoMainMenu::NextDay()
{
	auto PlayerData = GetGameInstance()->GetSubsystem<UGameSubsystem>()->PlayerData;
	PlayerData->Day += 1;
	ShowPopup();
}
