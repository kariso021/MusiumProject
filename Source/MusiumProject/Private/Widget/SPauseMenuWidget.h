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

		// ����(Argument)�� ���⿡ �����մϴ�.
		SLATE_ARGUMENT(TWeakObjectPtr<AMusiumPlayerController>, OwningPlayerController)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	// ���� �簳 Ŭ��
	FReply OnResumeClicked();

	// ���� ���� Ŭ��
	FReply OnQuitClicked();

	// �� ������ ������ �÷��̾� ��Ʈ�ѷ� ����
	TWeakObjectPtr<AMusiumPlayerController> OwningPlayerController;

	// ��ư�� ���� (����, ���콺 �÷��� ��, ������ ��)
	FButtonStyle MuseumButtonStyle;

	// ���� �ؽ�Ʈ ��Ÿ��
	FTextBlockStyle TitleTextStyle;

	// ��ư �ؽ�Ʈ ��Ÿ��
	FTextBlockStyle ButtonTextStyle;

	// �г� ��� ��Ÿ��
	FSlateBrush PanelBackgroundBrush;

};
