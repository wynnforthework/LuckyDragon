// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VaRestRequestJSON.h"
#include "GoogleSheetMenu.generated.h"

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

UCLASS()
class LUCKYDRAGON_API UGoogleSheetMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	class UUserWidget* DownloadSheetWidget;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	class UUserWidget* CloseSheetWidget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	class UTextBlock* TextLogs;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(BindWidget))
	TArray<FDataTableAndSheetConfigData> DataTableAndSheetConfigDatas;

	UFUNCTION()
	void OnDownloadSheetButtonClicked();
	UFUNCTION()
	void OnCloseButtonClicked();
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
