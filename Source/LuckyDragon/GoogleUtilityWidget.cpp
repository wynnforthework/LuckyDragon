// Fill out your copyright notice in the Description page of Project Settings.


#include "GoogleUtilityWidget.h"
#include "VaRestSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "LuckyDragon/Item.h"

bool UGoogleUtilityWidget::Initialize()
{
	Super::Initialize();

	if (DownloadSheetWidget!=nullptr)
	{
		UButton* DownloadSheetButton = Cast<UButton>(DownloadSheetWidget->GetWidgetFromName(TEXT("Button")));
		if (DownloadSheetButton)
		{
			DownloadSheetButton->OnClicked.AddDynamic(this,&UGoogleUtilityWidget::OnDownloadSheetButtonClicked);
		}
	}

	CurrentSheetIndex = 0;
	return true;
}


void UGoogleUtilityWidget::OnDownloadSheetButtonClicked()
{
	if (DataTableAndSheetConfigDatas.IsEmpty())
	{
		TextLogs->SetText(FText::FromString(TEXT("No more sheet to download")));
	}
	else if (DataTableAndSheetConfigDatas.Num()<=CurrentSheetIndex)
	{
		auto logs = TextLogs->GetText().ToString();
		logs.Append(TEXT("导入结束！"));
		TextLogs->SetText(FText::FromString(logs));
	}
	else
	{
		auto Config = DataTableAndSheetConfigDatas[CurrentSheetIndex];
		RequestSheetData(Config.SpreadsheetId,Config.SheetTitle);
	}
}

void UGoogleUtilityWidget::RequestSheetData(FString SheetId,FString SheetTitle)
{
	UVaRestSubsystem* VaRest = GEngine->GetEngineSubsystem<UVaRestSubsystem>();
	UVaRestRequestJSON* Request = VaRest->ConstructVaRestRequestExt(EVaRestRequestVerb::GET,EVaRestRequestContentType::x_www_form_urlencoded_url);
	FString Key;
	const FString DefaultGamePath = FConfigCacheIni::NormalizeConfigIniPath(FString::Printf(TEXT("%sDeepSeek.ini"), *FPaths::SourceConfigDir()));
	GConfig->GetString(TEXT("GoogleSheet"),TEXT("ApiKey2"),Key,DefaultGamePath);
	const FString Url = FString::Printf(TEXT("https://sheets.googleapis.com/v4/spreadsheets/%s/values/%s?key=%s"),*SheetId,*SheetTitle,*Key);
	Request->OnRequestComplete.AddDynamic(this,&UGoogleUtilityWidget::OnRequestComplete);
	FLatentActionInfo LatentActionInfo = FLatentActionInfo();
	Request->ApplyURL(Url,ResultObject,this,LatentActionInfo);
}

void UGoogleUtilityWidget::OnRequestComplete(UVaRestRequestJSON* Result)
{
	auto logs = TextLogs->GetText().ToString();
	if (Result->GetResponseCode() == 200)
	{
		auto a = Result->GetResponseObject();
		auto b = a->EncodeJson();
		// 清理 JSON 字符串
		b.ReplaceInline(TEXT("L\""), TEXT("\""));
		b.ReplaceInline(TEXT("\r"), TEXT(""));
		b.ReplaceInline(TEXT("\n"), TEXT(""));
		b.ReplaceInline(TEXT("\t"), TEXT(""));
		b.TrimStartAndEndInline();

		// 解析 JSON 字符串
		TSharedPtr<FJsonObject> JsonObject = ParseJsonString(b);

		logs.Append(TEXT("\n下载完成："));
		logs.Append(FString::FromInt(CurrentSheetIndex));
	
		logs.Append(TEXT("\n开始导入："));
		if (!JsonObject.IsValid())
		{
			logs.Append(TEXT("JSON 对象无效！"));
			TextLogs->SetText(FText::FromString(logs));
		}
		else
		{
			bool ParseSuccess = true;
			// 访问数组字段
			const TArray<TSharedPtr<FJsonValue>>* ValuesArray;
			if (JsonObject->TryGetArrayField(TEXT("values"), ValuesArray))
			{
				auto Config = DataTableAndSheetConfigDatas[CurrentSheetIndex];
				Config.DataTable->EmptyTable();
				for (int32 Index = 1;Index<ValuesArray->Num();Index++)
				{
					auto d = *ValuesArray;
					const TSharedPtr<FJsonValue>& RowValue = d[Index];
					if (RowValue->Type == EJson::Array)
					{
						const TArray<TSharedPtr<FJsonValue>> RowArray = RowValue->AsArray();
						FItemData NewRow = FItemData();
						FName RowName = FName(*RowArray[0]->AsString());
						NewRow.ID = RowArray[1]->AsNumber();
						NewRow.ItemName = FName(*RowArray[2]->AsString());
						NewRow.Price = RowArray[3]->AsNumber();
						Config.DataTable->AddRow(RowName,NewRow);
					}
					else
					{
						logs.Append(TEXT("解析失败"));
						TextLogs->SetText(FText::FromString(logs));
						ParseSuccess = false;
						break;
					}
				}
			}
			else
			{
				logs.Append(TEXT("导入失败，表格中没有数据"));
				TextLogs->SetText(FText::FromString(logs));
			}
			if (ParseSuccess)
			{
				logs.Append(TEXT("导入成功"));
				logs.Append(TEXT("\n尝试导入下一个"));
				TextLogs->SetText(FText::FromString(logs));
				CurrentSheetIndex++;
				OnDownloadSheetButtonClicked();
			}
		}
	}
	else
	{
		logs.Append(TEXT("\n"));
		logs.Append(TEXT("下载失败，请检查配置"));
		TextLogs->SetText(FText::FromString(logs));
	}
}
TSharedPtr<FJsonObject> UGoogleUtilityWidget::ParseJsonString(const FString& JsonString)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("JSON 解析成功！"));
		return JsonObject;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON 解析失败：%s"), *JsonReader->GetErrorMessage());
		return nullptr;
	}
}
void UGoogleUtilityWidget::AccessJsonData(const TSharedPtr<FJsonObject>& JsonObject)
{
	if (!JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("JSON 对象无效！"));
		return;
	}

	// 访问字符串字段
	FString Range;
	if (JsonObject->TryGetStringField(TEXT("range"), Range))
	{
		UE_LOG(LogTemp, Log, TEXT("range: %s"), *Range);
	}

	// 访问字符串字段
	FString MajorDimension;
	if (JsonObject->TryGetStringField(TEXT("majorDimension"), MajorDimension))
	{
		UE_LOG(LogTemp, Log, TEXT("majorDimension: %s"), *MajorDimension);
	}

	// 访问数组字段
	const TArray<TSharedPtr<FJsonValue>>* ValuesArray;
	if (JsonObject->TryGetArrayField(TEXT("values"), ValuesArray))
	{
		for (const TSharedPtr<FJsonValue>& RowValue : *ValuesArray)
		{
			if (RowValue->Type == EJson::Array)
			{
				const TArray<TSharedPtr<FJsonValue>>* RowArray = &RowValue->AsArray();
				for (const TSharedPtr<FJsonValue>& CellValue : *RowArray)
				{
					if (CellValue->Type == EJson::String)
					{
						UE_LOG(LogTemp, Log, TEXT("单元格值: %s"), *CellValue->AsString());
					}
					else if (CellValue->Type == EJson::Number)
					{
						UE_LOG(LogTemp, Log, TEXT("单元格值: %f"), CellValue->AsNumber());
					}
				}
			}
		}
	}
}

