// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateTypes.h"

class AMusiumPlayerController;

class SPauseMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPauseMenuWidget)
		{
		}

		// 인자(Argument)는 여기에 선언합니다.
		SLATE_ARGUMENT(TWeakObjectPtr<AMusiumPlayerController>, OwningPlayerController)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	// 게임 재개 클릭
	FReply OnResumeClicked();

	// 게임 종료 클릭
	FReply OnQuitClicked();

	// 이 위젯을 소유한 플레이어 컨트롤러 참조
	TWeakObjectPtr<AMusiumPlayerController> OwningPlayerController;

	// 버튼의 외형 (보통, 마우스 올렸을 때, 눌렀을 때)
	FButtonStyle MuseumButtonStyle;

	// 제목 텍스트 스타일
	FTextBlockStyle TitleTextStyle;

	// 버튼 텍스트 스타일
	FTextBlockStyle ButtonTextStyle;

	// 패널 배경 스타일
	FSlateBrush PanelBackgroundBrush;

};
