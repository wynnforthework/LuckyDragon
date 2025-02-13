// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "VaRestJsonObject.h"
#include "GoogleUtilityWidget.generated.h"

USTRUCT(BlueprintType)
struct FDataTableAndSheetConfigData
{
	GENERATED_USTRUCT_BODY()

	FDataTableAndSheetConfigData()
	{
		DataTable = nullptr;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* DataTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SpreadsheetId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SheetTitle;
};
/**
 * 
 */
UCLASS()
class LUCKYDRAGON_API UGoogleUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:
	UGoogleUtilityWidget(){}
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	class UUserWidget* DownloadSheetWidget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	class UTextBlock* TextLogs;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TArray<FDataTableAndSheetConfigData> DataTableAndSheetConfigDatas;

	UFUNCTION()
	void OnDownloadSheetButtonClicked();
	virtual bool Initialize() override;

	UFUNCTION()
	void OnRequestComplete(UVaRestRequestJSON* Result);
	void RequestSheetData(FString SheetId,FString SheetTitle);
	TSharedPtr<FJsonObject> ParseJsonString(const FString& JsonString);
	void AccessJsonData(const TSharedPtr<FJsonObject>& JsonObject);

private:
	int CurrentSheetIndex;
	UVaRestJsonObject* ResultObject;
};
