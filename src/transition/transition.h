/*** 
 * @Author: suzhou suzhou@360.cn
 * @Date: 2024-07-30 18:32:00
 * @LastEditors: suzhou suzhou@360.cn
 * @LastEditTime: 2024-12-25 21:57:52
 * @FilePath: /GPUPixel/src/transition/transition.h
 * @Description: 
 * @
 * @Copyright (c) 2024 by Qihoo 360, All Rights Reserved. 
 */


#pragma once

#include "filter.h"
#include "gpupixel_macros.h"

NS_GPUPIXEL_BEGIN
class GPUPIXEL_API Transition : public Filter {
 public:
  static std::shared_ptr<Transition> create();
  static std::shared_ptr<Transition> create(const std::string transitionStr);
  ~Transition();
  bool init();
  bool init(const std::string transitionStr);
  bool proceed(bool bUpdateTargets = true, int64_t frameTime = 0) override;

  //
  void setProgress(float progress);
  
  void updateProperty(std::string name, int value);
  void updateProperty(std::string name, float value);
  void updateProperty(std::string name, Vector2 value);
  void updateProperty(std::string name, Vector3 value);

 protected:
  Transition();
  float _progress;
  GLuint filterTexCoordAttribute_;
  std::map<std::string, int> _intPropertys;
  std::map<std::string, float> _floatPropertys;
  std::map<std::string, Vector2> _vec2Propertys;
  std::map<std::string, Vector3> _vec3Propertys;
  // GLuint filterTexCoordAttribute2_;
};

NS_GPUPIXEL_END
