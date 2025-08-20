// Fill out your copyright notice in the Description page of Project Settings.


#include "SPauseMenuWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/CoreStyle.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Styling/SlateColor.h"
#include "Widgets/Layout/SBorder.h"
#include "MusiumPlayerController.h"
#include <Brushes/SlateColorBrush.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SPauseMenuWidget::Construct(const FArguments& InArgs)
{
	OwningPlayerController = InArgs._OwningPlayerController;

	// --- 1. [����] ������ ���� �� ��Ÿ�� ���� ---

	// ��� ������ ���İ�(������ ����)�� 1.0f�� �����Ͽ� ������ �������ϰ�
	const FLinearColor PanelColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);        // ��ο� �г� ���� (������)
	const FLinearColor TextColor = FLinearColor(0.95f, 0.95f, 0.95f, 1.0f);      // ���� �ؽ�Ʈ ����
	const FLinearColor ButtonNormalColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f); // ��ư ��� ���� (������)
	const FLinearColor ButtonHoverColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);  // ��ư ȣ�� ���� (������)
	const FLinearColor ButtonPressedColor = FLinearColor(0.4f, 0.4f, 0.4f, 1.0f); // ��ư ������ �� ���� (������)

	PanelBackgroundBrush = FSlateColorBrush(PanelColor);

	TitleTextStyle = FTextBlockStyle()
		.SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 48, "Light"))
		.SetColorAndOpacity(TextColor);

	ButtonTextStyle = FTextBlockStyle()
		.SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 24))
		.SetColorAndOpacity(TextColor);

	MuseumButtonStyle = FButtonStyle()
		.SetNormal(FSlateColorBrush(ButtonNormalColor))
		.SetHovered(FSlateColorBrush(ButtonHoverColor))
		.SetPressed(FSlateColorBrush(ButtonPressedColor))
		.SetNormalPadding(FMargin(15.f))
		.SetPressedPadding(FMargin(15.f));

	const FText TitleText = FText::FromString(TEXT("PAUSED"));
	const FText ResumeText = FText::FromString(TEXT("RESUME"));
	const FText QuitText = FText::FromString(TEXT("QUIT"));
	const FMargin ButtonPadding = FMargin(0.f, 15.f);

	ChildSlot
		[
			SNew(SBox)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
						.WidthOverride(1000.0f)
						.HeightOverride(600.0f)
						[
							SNew(SBorder)
								.BorderImage(&PanelBackgroundBrush)
								.Padding(FMargin(40.f))
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot()
										.AutoHeight()
										.HAlign(HAlign_Center)
										.Padding(FMargin(0.f, 0.f, 0.f, 30.f))
										[
											SNew(STextBlock)
												.TextStyle(&TitleTextStyle)
												.Text(TitleText)
										]
										+ SVerticalBox::Slot()
										.FillHeight(1.0f)
										.VAlign(VAlign_Center)
										.Padding(ButtonPadding)
										[
											SNew(SButton)
												.ButtonStyle(&MuseumButtonStyle)
												.OnClicked(this, &SPauseMenuWidget::OnResumeClicked)
												[
													SNew(STextBlock)
														.Text(ResumeText)
														.ColorAndOpacity(TextColor) // TextColor�� ���� ����
														.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 24))
														.Justification(ETextJustify::Center)
												]
										]
									+ SVerticalBox::Slot()
										.FillHeight(1.0f)
										.VAlign(VAlign_Center)
										.Padding(ButtonPadding)
										[
											SNew(SButton)
												.ButtonStyle(&MuseumButtonStyle)
												.OnClicked(this, &SPauseMenuWidget::OnQuitClicked)
												[
													SNew(STextBlock)
														.Text(QuitText)
														.ColorAndOpacity(TextColor) // TextColor�� ���� ����
														.Font(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 24))
														.Justification(ETextJustify::Center)
												]
										]
								]
						]
				]
		];
}


FReply SPauseMenuWidget::OnResumeClicked()
{
	// OwningPlayerController�� ��ȿ���� Ȯ��
	if (OwningPlayerController.IsValid())
	{
		OwningPlayerController.Get()->ResumeGame();
	}

	return FReply::Handled();
}
FReply SPauseMenuWidget::OnQuitClicked()
{
	if (OwningPlayerController.IsValid())
	{
		// ���� ����
		UKismetSystemLibrary::QuitGame(OwningPlayerController.Get(), OwningPlayerController.Get(), EQuitPreference::Quit, true);
	}

	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
