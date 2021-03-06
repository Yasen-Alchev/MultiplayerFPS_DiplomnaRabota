// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveStatsWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UObjectiveStatsWidget::UObjectiveStatsWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

void UObjectiveStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ResetStats();
}

void UObjectiveStatsWidget::UpdateStats(int RedScore, int BlueScore)
{
	if (IsValid(TXTBlock_RedScoreStat) && IsValid(TXTBlock_BlueScoreStat) && RedScore >= 0 && BlueScore >= 0)
	{
		TXTBlock_RedScoreStat->SetText(FText::FromString("Red: " + FString::FromInt(RedScore)));
		TXTBlock_BlueScoreStat->SetText(FText::FromString("Blue: " + FString::FromInt(BlueScore)));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectiveStatsWidget::UpdateStats(int32 RedScore, int32 BlueScore) -> TXTBlock_RedScoreStat or  TXTBlock_BlueScoreStat is Invalid or RedScore or BlueScore is less than Zero !!!"));
	}
}

void UObjectiveStatsWidget::ResetStats()
{
	if (IsValid(TXTBlock_RedScoreStat) && IsValid(TXTBlock_BlueScoreStat))
	{
		TXTBlock_RedScoreStat->SetText(FText::FromString("Red: 0"));
		TXTBlock_BlueScoreStat->SetText(FText::FromString("Blue: 0"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UObjectiveStatsWidget::ResetStats() -> TXTBlock_RedScoreStat or  TXTBlock_BlueScoreStat is not Valid!!!"));
	}
}