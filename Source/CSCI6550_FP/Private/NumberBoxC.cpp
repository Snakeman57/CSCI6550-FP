// Copyright 2021 Matthew Sentell


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
	num = tmp.EqualTo(FText::GetEmpty()) ? dflt : UKismetStringLibrary::Conv_StringToInt(UKismetTextLibrary::Conv_TextToString(Text));
}
void UNumberBoxC::HandleOnTextCommitted(FText const& t, ETextCommit::Type cm) {
	Super::HandleOnTextCommitted(t, cm);
	num = Text.EqualTo(FText::GetEmpty()) ? dflt : UKismetStringLibrary::Conv_StringToInt(UKismetTextLibrary::Conv_TextToString(Text));
}
void UNumberBoxC::reset() {
	num = dflt;
	SetText(FText::GetEmpty());
}