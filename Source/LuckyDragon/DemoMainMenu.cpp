// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoMainMenu.h"

#include "GameSubsystem.h"
#include "MySaveGame.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"


bool UDemoMainMenu::Initialize()
{
	return Super::Initialize();
}

void UDemoMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	StartButton->OnClicked.AddDynamic(this, &UDemoMainMenu::StartGame);
	ContinueButton->OnClicked.AddDynamic(this, &UDemoMainMenu::ContinueGame);
	EndButton->OnClicked.AddDynamic(this, &UDemoMainMenu::QuitGame);

	UButton* SureButton = Cast<UButton>(PopupSureWidget->GetWidgetFromName(TEXT("Button")));
	SureButton->OnClicked.AddDynamic(this,&UDemoMainMenu::ClosePopup);

	UButton* NextDayButton = Cast<UButton>(NextDayWidget->GetWidgetFromName(TEXT("Button")));
	NextDayButton->OnClicked.AddDynamic(this,&UDemoMainMenu::NextDay);

	UButton* GiftButton = Cast<UButton>(GiftWidget->GetWidgetFromName(TEXT("Button")));
	GiftButton->OnClicked.AddDynamic(this,&UDemoMainMenu::OpenGacha);
	
	UButton* GachaBackButton = Cast<UButton>(GachaBackWidget->GetWidgetFromName(TEXT("ButtonIcon")));
	GachaBackButton->OnClicked.AddDynamic(this,&UDemoMainMenu::CloseGacha);

	UButton* GachaOneButton = Cast<UButton>(GachaOneWidget->GetWidgetFromName(TEXT("Button")));
	if (GachaOneButton != nullptr)
	{
		GachaOneButton->OnClicked.AddDynamic(this,&UDemoMainMenu::GachaOne);
	}
	UButton* GachaTenButton = Cast<UButton>(GachaTenWidget->GetWidgetFromName(TEXT("Button")));
	if (GachaTenButton != nullptr)
	{
		GachaTenButton->OnClicked.AddDynamic(this,&UDemoMainMenu::GachaTen);
	}
	GiftSlotWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Widgets/w_inventory_gift_slot_template.w_inventory_gift_slot_template_C"));
	UpSlotWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/Widgets/w_shop_categorie_up_template.w_shop_categorie_up_template_C"));
	HideAllPanel();
	LoadBag();
	LoadUp();
	NewGameState = 0;
}

void UDemoMainMenu::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"), *FString(__FUNCTION__));

	GetGameInstance()->GetSubsystem<UGameSubsystem>()->NewSave();
	RefreshUI();
	LoginPanel->SetVisibility(ESlateVisibility::Hidden);
	PlayStory();
	RequestAIData();
}

void UDemoMainMenu::ContinueGame()
{
	GetGameInstance()->GetSubsystem<UGameSubsystem>()->LoadSaveGame();
	RefreshUI();
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
	StartTypewriterEffect(TEXT("醒醒          \n醒醒          \n龙泡泡：爸，一天只有一万块钱生活费，你让我怎么活啊。          \n大龙跨国集团董事长龙王：以前太惯着你了，不让你吃点苦看来你是不会听话的，好好反省！"), 0.1f);
}

void UDemoMainMenu::HideAllPanel()
{
	LogoPanel->SetVisibility(ESlateVisibility::Hidden);
	LoginPanel->SetVisibility(ESlateVisibility::Hidden);
	StoryPanel->SetVisibility(ESlateVisibility::Hidden);
	HomePanel->SetVisibility(ESlateVisibility::Hidden);
	PopupPanel->SetVisibility(ESlateVisibility::Hidden);
	GachaPanel->SetVisibility(ESlateVisibility::Hidden);
	auto a = PopupPanel->GetVisibility();
	if (a == ESlateVisibility::Hidden)
	{
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s] PopupPanel隐藏 "), *FString(__FUNCTION__));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s] PopupPanel显示 "), *FString(__FUNCTION__));

	}
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
		GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,content);
		TextAI->SetText(FText::FromString(TEXT("创世成功！")));
		CheckGameState();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s]"), *FString(__FUNCTION__));
		CheckGameState();
	}
}

void UDemoMainMenu::OnRequestFail(UVaRestRequestJSON* Request)
{
	FString content = Request->GetResponseContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s] content:%s"), *FString(__FUNCTION__), *content);
	TextAI->SetText(FText::FromString(TEXT("创世失败，游戏功能将被限制。")));
	FMessageDialog::Open( EAppMsgCategory::Warning, EAppMsgType::Ok, FText::FromString(TEXT("创世失败，游戏功能将被限制")));
	CheckGameState();
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
	JsonObject->SetStringField(TEXT("content"), TEXT("“生成10个游戏NPC数据，每个NPC需包含以下字段：1.name：中文姓名2.type：主播类型（具体领域+直播形式）3.unlock_level：整数解锁等级（1-20）4.tags：3个身份标签（用中文短词描述）5.ability：与角色设定相关的游戏能力（含数值/触发条件）6.specialty：2个特长描述（动词开头短语）7.relationships：至少1个与其他NPC/势力的关联按严格JSON格式返回，键名使用蛇形命名法，禁用注释。”"));
	const TArray<UVaRestJsonObject*> JsonArray = {JsonObject};
	UVaRestJsonObject* JsonObject2 = UVaRestSubsystem::StaticConstructVaRestJsonObject();
	JsonObject2->SetStringField(TEXT("model"), TEXT("deepseek-chat"));
	JsonObject2->SetObjectArrayField(TEXT("messages"),JsonArray);
	UVaRestJsonObject* JsonObject3 = UVaRestSubsystem::StaticConstructVaRestJsonObject();
	JsonObject3->SetStringField(TEXT("type"), TEXT("json_object"));
	JsonObject2->SetObjectField("response_format",JsonObject3);

	RequestJSON->SetRequestObject(JsonObject2);
	
	FString ApiKey;
	const FString DefaultGamePath = FConfigCacheIni::NormalizeConfigIniPath(FString::Printf(TEXT("%sDeepSeek.ini"), *FPaths::SourceConfigDir()));
	GConfig->GetString(TEXT("DeepSeek"), TEXT("ApiKey"), ApiKey, DefaultGamePath);
	RequestJSON->SetHeader(TEXT("Authorization"), ApiKey);
	RequestJSON->SetHeader(TEXT("Content-Type"),TEXT("application/json"));
	RequestJSON->OnRequestComplete.AddDynamic(this, &UDemoMainMenu::OnRequestComplete);
	RequestJSON->OnRequestFail.AddDynamic(this, &UDemoMainMenu::OnRequestFail);
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
	auto a = PopupPanel->GetVisibility();
	if (a == ESlateVisibility::Hidden)
	{
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s] PopupPanel隐藏 "), *FString(__FUNCTION__));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s] PopupPanel显示 "), *FString(__FUNCTION__));

	}
}
void UDemoMainMenu::ClosePopup()
{
	PopupPanel->SetVisibility(ESlateVisibility::Hidden);
	auto a = PopupPanel->GetVisibility();
	if (a == ESlateVisibility::Hidden)
	{
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s] PopupPanel隐藏 "), *FString(__FUNCTION__));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[wyh] [%s] PopupPanel显示 "), *FString(__FUNCTION__));

	}

	GetGameInstance()->GetSubsystem<UGameSubsystem>()->UpdateGold(10000);
	GetGameInstance()->GetSubsystem<UGameSubsystem>()->WriteSaveGame();
	RefreshUI();
}
void UDemoMainMenu::NextDay()
{
	GetGameInstance()->GetSubsystem<UGameSubsystem>()->UpdateDay();
	ShowPopup();
}
void UDemoMainMenu::OpenGacha()
{
	HomePanel->SetVisibility(ESlateVisibility::Hidden);
	GachaPanel->SetVisibility(ESlateVisibility::Visible);
	RefreshUI();
	RandomGiftState = 0;
	GachaPanelShowTime = 0;
	RandomGiftTexts.Empty();
	AllGiftNames.Empty();
	CurrentGroupIndex = 0;
	CurrentGiftNameIndex = 0;
	IsFirstCircle = true;
	GachaAnimationDuration = 0;
	GachaAnimationTime = 0;
	for (UWidget* Widget : RandomGiftPanel->GetAllChildren())
	{
		USizeBox* SizeBox = Cast<USizeBox>(Widget);
		UTextBlock* TextBlock = Cast<UTextBlock>(SizeBox->GetChildAt(0));
        TextBlock->SetOpacity(0);
		RandomGiftTexts.Add(TextBlock);
	}
	// 将TextBlock随机分成3组
	TextBlockGroups.SetNum(3);
	TArray<UTextBlock*> ShuffledTextBlocks = RandomGiftTexts;
	for (int32 i = ShuffledTextBlocks.Num() - 1; i > 0; --i)
	{
		int32 j = FMath::Rand() % (i + 1);
		ShuffledTextBlocks.Swap(i, j);
	}

	for (int32 i = 0; i < ShuffledTextBlocks.Num(); ++i)
	{
		TextBlockGroups[i % 3].Add(ShuffledTextBlocks[i]);
	}

	if (GetGameInstance()->GetSubsystem<UGameSubsystem>()->DT_Gift)
	{
		for (auto iter : GetGameInstance()->GetSubsystem<UGameSubsystem>()->DT_Gift->GetRowMap())
		{
			// FName RowName = iter.Key;
			FItemData* Row = (FItemData*)iter.Value;
			AllGiftNames.Add(Row->ItemName);
		}
	}

	// 打乱FName的顺序
	for (int32 i = AllGiftNames.Num() - 1; i > 0; --i)
	{
		int32 j = FMath::Rand() % (i + 1);
		AllGiftNames.Swap(i, j);
	}
	RandomGiftState = 1;
	UpdateBag();
}
void UDemoMainMenu::CloseGacha()
{
	HomePanel->SetVisibility(ESlateVisibility::Visible);
	GachaPanel->SetVisibility(ESlateVisibility::Hidden);
	RandomGiftState = 0;
}

void UDemoMainMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (RandomGiftState>0)
	{
		if (RandomGiftState == 1)
		{
			if (GachaPanelShowTime == 0)
			{
				for (UTextBlock* TextBlock : TextBlockGroups[CurrentGroupIndex])
				{
					// 从FNames中获取下一个FName
					FName NextFName = AllGiftNames[CurrentGiftNameIndex];
					CurrentGiftNameIndex = (CurrentGiftNameIndex + 1) % AllGiftNames.Num();

					// 设置TextBlock的文本
					TextBlock->SetText(FText::FromName(NextFName));

					TextBlock->SetOpacity(1);
				}
			}
			GachaPanelShowTime += InDeltaTime;
			if (GachaPanelShowTime>=1.0f)
			{
				GachaPanelShowTime = 0;
				CurrentGroupIndex++;
				if (CurrentGroupIndex>=3)
				{
					IsFirstCircle = false;
					CurrentGroupIndex = 0;
				}
			}
			for(int32 Index = 0;Index<TextBlockGroups.Num();Index++)
			{
				for (UTextBlock* TextBlock : TextBlockGroups[Index])
				{
					// 设置TextBlock的透明度
					if (Index == CurrentGroupIndex)
					{
						if (GachaPanelShowTime == 0)
						{
							TextBlock->SetOpacity(1);

						}
						else
						{
							TextBlock->SetOpacity(GachaPanelShowTime);
						}
					}
					else
					{
						if (!IsFirstCircle)
						{
							// TextBlock->SetOpacity(1 - GachaPanelShowTime);
						}
					}
				}

			}
		}
		else if (RandomGiftState == 2)
		{
			if (GachaAnimationDuration == 0)
			{
				GachaAnimationDuration = 0.5f;
				GachaAnimationTime = 0.0f;
				for (UTextBlock* TextBlock : TextBlockGroups[CurrentGroupIndex])
				{
					TextBlock->SetOpacity(1);
				}
			}
			else
			{
				GachaAnimationTime += InDeltaTime;

				if (GachaAnimationTime>GachaAnimationDuration)
				{
					GachaAnimationDuration += 0.3f;
					GachaAnimationTime = 0.0f;
					if (GachaAnimationDuration>=2.0f)
					{
						FString _result_name = "";
						int8 _result_count = 0;
						for(int32 Index = 0;Index<TextBlockGroups.Num();Index++)
						{
							for (UTextBlock* TextBlock : TextBlockGroups[Index])
							{
								// 从FNames中获取下一个FName
								FName NextFName = AllGiftNames[CurrentGiftNameIndex];
								CurrentGiftNameIndex = (CurrentGiftNameIndex + 1) % AllGiftNames.Num();

								// 设置TextBlock的文本
								TextBlock->SetText(FText::FromName(NextFName));
							
								_result_count++;
								if (_result_count<=GachaResultCount)
								{
									TextBlock->SetOpacity(1);
									_result_name.Append(TEXT("【"));
									_result_name.Append(NextFName.ToString());
									_result_name.Append(TEXT("】"));
									for (auto iter : GetGameInstance()->GetSubsystem<UGameSubsystem>()->DT_Gift->GetRowMap())
									{
										// FName RowName = iter.Key;
										FItemData* Row = (FItemData*)iter.Value;
										if (Row->ItemName == NextFName)
										{
											GetGameInstance()->GetSubsystem<UGameSubsystem>()->UpdateAmount(Row->ID,1);
										}
									}
								}
								else
								{
									TextBlock->SetOpacity(0);
								}
							}
						}
						GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red,_result_name);
						UE_LOG(LogTemp, Display, TEXT("恭喜抽中：%s"),*_result_name);
						UpdateBag();
						UButton* GachaOneButton = Cast<UButton>(GachaOneWidget->GetWidgetFromName(TEXT("Button")));
						GachaOneButton->SetIsEnabled(false);
						UButton* GachaTenButton = Cast<UButton>(GachaTenWidget->GetWidgetFromName(TEXT("Button")));
						GachaTenButton->SetIsEnabled(false);
						GetGameInstance()->GetSubsystem<UGameSubsystem>()->WriteSaveGame();
						RandomGiftState = 0;
					}
					else
					{
						for(int32 Index = 0;Index<TextBlockGroups.Num();Index++)
						{
							for (UTextBlock* TextBlock : TextBlockGroups[Index])
							{
								// 从FNames中获取下一个FName
								FName NextFName = AllGiftNames[CurrentGiftNameIndex];
								CurrentGiftNameIndex = (CurrentGiftNameIndex + 1) % AllGiftNames.Num();

								// 设置TextBlock的文本
								TextBlock->SetText(FText::FromName(NextFName));

								TextBlock->SetOpacity(1);
							}
						}
					}
				}
				else
				{
					float _alpha = 1-FMath::Clamp(GachaAnimationTime / GachaAnimationDuration, 0.0f, 1.0f);
					for(int32 Index = 0;Index<TextBlockGroups.Num();Index++)
					{
						for (UTextBlock* TextBlock : TextBlockGroups[Index])
						{
							TextBlock->SetOpacity(_alpha);
						}
					}
				}
			}
		}
	}
}

void UDemoMainMenu::GachaOne()
{
	if (GetGameInstance()->GetSubsystem<UGameSubsystem>()->UpdateGold(-1000))
	{
		RandomGiftState = 2;
		GachaResultCount = 1;
		GachaAnimationDuration = 0;
		GachaAnimationTime = 0;
		RefreshUI();
		UButton* GachaOneButton = Cast<UButton>(GachaOneWidget->GetWidgetFromName(TEXT("Button")));
		GachaOneButton->SetIsEnabled(false);
		UButton* GachaTenButton = Cast<UButton>(GachaTenWidget->GetWidgetFromName(TEXT("Button")));
		GachaTenButton->SetIsEnabled(false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Red,TEXT("财富不足"));
		FText MsgTitle = FText::FromString(TEXT("Warning"));
		FMessageDialog::Open( EAppMsgCategory::Error, EAppMsgType::Ok, FText::FromString(TEXT("财富不足")));
	}
}

void UDemoMainMenu::GachaTen()
{
	if (GetGameInstance()->GetSubsystem<UGameSubsystem>()->UpdateGold(-10000))
	{
		RandomGiftState = 2;
		GachaResultCount = 10;
		GachaAnimationDuration = 0;
		GachaAnimationTime = 0;
		RefreshUI();
		UButton* GachaOneButton = Cast<UButton>(GachaOneWidget->GetWidgetFromName(TEXT("Button")));
		GachaOneButton->SetIsEnabled(false);
		UButton* GachaTenButton = Cast<UButton>(GachaTenWidget->GetWidgetFromName(TEXT("Button")));
		GachaTenButton->SetIsEnabled(false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Red,TEXT("财富不足"));
		FMessageDialog::Open( EAppMsgCategory::Error, EAppMsgType::Ok, FText::FromString(TEXT("财富不足")));
	}
}

void UDemoMainMenu::LoadBag()
{
	if (BagGiftPanel && GiftSlotWidgetClass!=nullptr)
	{
		BagGiftPanel->ClearChildren();
		auto MaxCol = ceil(GetGameInstance()->GetSubsystem<UGameSubsystem>()->DT_Gift->GetRowMap().Num()/3);
		for (int32 i=0;i<3;i++)
		{
			for (int32 j=0;j<MaxCol;j++)
			{
				auto GiftSlot = CreateWidget<UUserWidget>(this, GiftSlotWidgetClass);
				if (GiftSlot!=nullptr)
				{
					GiftSlot->SetDesiredSizeInViewport(FVector2D(160.0f,160.0f));
					BagGiftPanel->AddChildToUniformGrid(GiftSlot,i,j);
				}
			}
		}
	}
}

void UDemoMainMenu::UpdateBag()
{
	int32 Index = 0;
	for (auto iter : GetGameInstance()->GetSubsystem<UGameSubsystem>()->DT_Gift->GetRowMap())
	{
		FItemData* ItemData = (FItemData*)iter.Value;
		if (ItemData == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("是空的：%s"),*iter.Key.ToString());
			continue;
		}
		FString _name = ItemData->ItemName.ToString();
		UE_LOG(LogTemp, Display, TEXT("%s"),*_name);
		UUserWidget* GiftSlot = Cast<UUserWidget>(BagGiftPanel->GetChildAt(Index++));
		if (GiftSlot)
		{
			UWidget* SlotLocked = GiftSlot->GetWidgetFromName(TEXT("SlotLocked"));
			UTextBlock* ItemName = Cast<UTextBlock>(GiftSlot->GetWidgetFromName(TEXT("ItemName")));
			UTextBlock* Count = Cast<UTextBlock>(GiftSlot->GetWidgetFromName(TEXT("Count")));
			if (ItemName==nullptr || Count==nullptr)
			{
				UE_LOG(LogTemp, Display, TEXT("GiftSlot是空的"));
				continue;
			}
			ItemName->SetText(FText::FromName(ItemData->ItemName));
			Count->SetText(FText::FromString(FString::FromInt(0)));
			SlotLocked->SetVisibility(ESlateVisibility::Visible);
			for (UItem* PlayerItem : GetGameInstance()->GetSubsystem<UGameSubsystem>()->PlayerItems)
			{
				if (PlayerItem->ID == ItemData->ID)
				{
					Count->SetText(FText::FromString(FString::FromInt(PlayerItem->Amount)));
					if (PlayerItem->Amount>0)
					{
						SlotLocked->SetVisibility(ESlateVisibility::Hidden);
					}
					break;
				}
			}
		}
		else
		{
			break;
		}
	}
}

void UDemoMainMenu::RefreshUI()
{
	auto PlayerData = GetGameInstance()->GetSubsystem<UGameSubsystem>()->GetPlayerData();
	if (TextGold != nullptr)
	{
		TextGold->SetText(FText::FromString(LexToString(PlayerData.Gold)));
	}
	if (TextDay != nullptr)
	{
		TextDay->SetText(FText::FromString(LexToString(PlayerData.Day)));
	}
	if (TextLevel != nullptr)
	{
		TextLevel->SetText(FText::FromString(LexToString(PlayerData.Level)));
	}
	if (GachaTextGold!=nullptr)
	{
		GachaTextGold->SetText(FText::FromString(LexToString(PlayerData.Gold)));
	}
}

void UDemoMainMenu::LoadUp()
{
	if (UpGridPanel && UpSlotWidgetClass!=nullptr)
	{
		UpGridPanel->ClearChildren();
		for (int32 i=0;i<5;i++)
		{
			for (int32 j=0;j<2;j++)
			{
				auto UpSlot = CreateWidget<UUserWidget>(this, UpSlotWidgetClass);
				if (UpSlot!=nullptr)
				{
					UpGridPanel->AddChildToUniformGrid(UpSlot,i,j);
					UWidget* SlotLocked = UpSlot->GetWidgetFromName(TEXT("SlotLocked"));
					if (i==0 && j == 0)
					{
						SlotLocked->SetVisibility(ESlateVisibility::Hidden);
					}
					else
					{
						SlotLocked->SetVisibility(ESlateVisibility::Visible);
					}
				}
			}
		}
	}
}
