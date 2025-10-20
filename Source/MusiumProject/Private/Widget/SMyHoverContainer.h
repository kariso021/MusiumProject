// SMyHoverContainer.h

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Templates/SharedPointer.h"

class SMyHoverContainer : public SCompoundWidget, public TSharedFromThis<SMyHoverContainer>
{
public:
    SLATE_BEGIN_ARGS(SMyHoverContainer)
        : _Content() // 기본 콘텐츠 (예: 썸네일 이미지)
        , _OverlayContent() // 호버 시 나타날 오버레이 콘텐츠
        {
        }

        SLATE_DEFAULT_SLOT(FArguments, Content)
        SLATE_NAMED_SLOT(FArguments, OverlayContent)

    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

protected:
    // 이벤트 핸들러는 그대로 사용합니다.
    virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

    /** 오버레이 위젯의 Visibility 상태를 반환하는 함수 */
    EVisibility GetOverlayVisibility() const;

private:
    /** 현재 호버 상태인지를 저장하는 변수 */
    bool bIsHovered = false;
};