/*
 * @Author: suzhou suzhou@360.cn
 * @Date: 2024-12-25 21:33:14
 * @LastEditors: suzhou suzhou@360.cn
 * @LastEditTime: 2024-12-25 21:53:22
 * @FilePath: /GPUPixel/src/transition/PolkaDotsCurtain_transition.cc
 * @Description: 
 * 
 * Copyright (c) 2024 by Qihoo 360, All Rights Reserved. 
 */

#include "PolkaDotsCurtain_transition.h"

NS_GPUPIXEL_BEGIN
#if defined(GPUPIXEL_IOS) || defined(GPUPIXEL_ANDROID)

const std::string kPolkaDotsCurtainTransitionString = R"(
    const float SQRT_2 = 1.414213562373;
    uniform float dots;// = 20.0;
    uniform vec2 center;// = vec2(0, 0);

    vec4 transition(vec2 uv) {
        bool nextImage = distance(fract(uv * dots), vec2(0.5, 0.5)) < ( progress / distance(uv, center));
        return nextImage ? getToColor(uv) : getFromColor(uv);
    })";
#elif defined(GPUPIXEL_MAC) || defined(GPUPIXEL_WIN) || defined(GPUPIXEL_LINUX)
#error "Not implemented"
#endif
PolkaDotsCurtainTransition::~PolkaDotsCurtainTransition(){}

std::shared_ptr<PolkaDotsCurtainTransition> PolkaDotsCurtainTransition::create(float dots, Vector2 center) {
    auto ret = std::shared_ptr<PolkaDotsCurtainTransition>(new PolkaDotsCurtainTransition());
    if (ret && !ret->init(kPolkaDotsCurtainTransitionString)) {
        ret.reset();
    } else {
        ret->updateProperty("dots", dots);
        ret->updateProperty("center", center);
    }
    return ret;
}

void PolkaDotsCurtainTransition::setDots(float dots){
    updateProperty("dots", dots);
}
void PolkaDotsCurtainTransition::setCenter(Vector2 center){
    updateProperty("center", center);
}
NS_GPUPIXEL_END
