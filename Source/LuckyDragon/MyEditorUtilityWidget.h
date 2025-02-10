// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "VaRestRequestJSON.h"
#include "MyEditorUtilityWidget.generated.h"

USTRUCT(BlueprintType)
struct FDataTableAndSheetConfigData
{
	GENERATED_USTRUCT_BODY()

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
class LUCKYDRAGON_API UMyEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UMyEditorUtilityWidget(){}
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
