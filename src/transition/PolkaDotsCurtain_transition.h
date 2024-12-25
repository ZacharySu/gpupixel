
#pragma once
#include "transition.h"

NS_GPUPIXEL_BEGIN
class GPUPIXEL_API PolkaDotsCurtainTransition : public Transition {
public:
    static std::shared_ptr<PolkaDotsCurtainTransition> create(float dots = 20.0f, Vector2 center = Vector2(0.0f, 0.0f));
    ~PolkaDotsCurtainTransition();
    void setDots(float dots);
    void setCenter(Vector2 center);
};
NS_GPUPIXEL_END
