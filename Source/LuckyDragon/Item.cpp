// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

void UItem::UpdateAmount(const int32& Magnitude)
{
	Amount += Magnitude;
	Amount = FMath::Clamp(Amount, 0,UINT32_MAX);
}