// SMyHoverContainer.cpp

#include "SMyHoverContainer.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"

void SMyHoverContainer::Construct(const FArguments& InArgs)
{
    // SOverlay를 사용하여 위젯들을 겹치게 배치합니다.
    ChildSlot
        [
            SNew(SOverlay)
        ];

}

void SMyHoverContainer::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
    UE_LOG(LogTemp, Display, TEXT("MouseEntered"));
    bIsHovered = true; // 호버 상태를 true로 변경
}

void SMyHoverContainer::OnMouseLeave(const FPointerEvent& MouseEvent)
{
    SCompoundWidget::OnMouseLeave(MouseEvent);
    bIsHovered = false; // 호버 상태를 false로 변경
    UE_LOG(LogTemp, Display, TEXT("MouseLeave"));
}

EVisibility SMyHoverContainer::GetOverlayVisibility() const
{
    // bIsHovered가 true이면 Visible, false이면 Hidden을 반환합니다.
    // Hidden: 보이지 않지만 공간은 차지함 (레이아웃 유지에 유리)
    // Collapsed: 보이지 않고 공간도 차지하지 않음
    return bIsHovered ? EVisibility::Visible : EVisibility::Hidden;
}