// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberBoxC.h"

UNumberBoxC::UNumberBoxC(const FObjectInitializer& ObjectInitializer) : Super(), dflt(0), num(dflt), last(FText::GetEmpty()), min(INT_MIN), max(INT_MAX) {
}
void UNumberBoxC::HandleOnTextChanged(FText const& t) {
	Super::HandleOnTextChanged(t);
	FText tmp = t;
	int32 val = UKismetStringLibrary::Conv_StringToInt(UKismetTextLibrary::Conv_TextToString(t));
	if ((UKismetStringLibrary::IsNumeric(UKismetTextLibrary::Conv_TextToString(tmp)) || tmp.EqualTo(FText::GetEmpty())) && val >= min && val <= max)
			last = t;
	SetText(last);
	num = UKismetStringLibrary::Conv_StringToInt(UKismetTextLibrary::Conv_TextToString(last));
}
void UNumberBoxC::HandleOnTextCommitted(FText const& t, ETextCommit::Type cm) {
	Super::HandleOnTextCommitted(t, cm);
	num = UKismetStringLibrary::Conv_StringToInt(UKismetTextLibrary::Conv_TextToString(Text));
}
void UNumberBoxC::reset() {
	num = dflt;
	SetText(FText::GetEmpty());
}