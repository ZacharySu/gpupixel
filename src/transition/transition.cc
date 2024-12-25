
#include "transition.h"
#include "gpupixel_context.h"

NS_GPUPIXEL_BEGIN

const std::string kTransitionBaseVertexShaderString = R"(
    attribute vec4 position; 
    attribute vec4 inputTextureCoordinate;

    varying vec2 textureCoordinate;

    void main() {
      gl_Position = position;
      textureCoordinate = inputTextureCoordinate.xy;
    })";
#if defined(GPUPIXEL_IOS) || defined(GPUPIXEL_ANDROID)
const std::string kTransitionBaseFragmentShaderStringHead = R"(
    precision highp float;
    varying vec2 textureCoordinate;
    uniform sampler2D inputImageTexture;
    uniform sampler2D inputImageTexture2;
    uniform float progress;

    vec4 getFromColor(highp vec2 uv){
        return texture2D(inputImageTexture, uv);
    }
    vec4 getToColor(vec2 uv){
        return texture2D(inputImageTexture2, uv);
    })";
const std::string kTransitionBaseString = R"(
    vec4 transition (vec2 uv) {
        return mix(
            getFromColor(uv),
            getToColor(uv),
            progress
        );
    })";
const std::string kTransitionBaseFragmentShaderStringTail = R"(
    void main() {
      gl_FragColor = transition(textureCoordinate);
    })";
#elif defined(GPUPIXEL_MAC) || defined(GPUPIXEL_WIN) || defined(GPUPIXEL_LINUX)
#error "Not implemented"
#endif

Transition::Transition() {}

Transition::~Transition() {}

std::shared_ptr<Transition> Transition::create() {
  auto ret = std::shared_ptr<Transition>(new Transition());
  if (ret && !ret->init()) {
    ret.reset();
  }
  return ret;
}
std::shared_ptr<Transition> Transition::create(const std::string transitionStr) {
  auto ret = std::shared_ptr<Transition>(new Transition());
  if (ret && !ret->init(transitionStr)) {
    ret.reset();
  }
  return ret;
}
bool Transition::init(const std::string transitionStr){
  std::string kTransitionBaseFragmentShaderString = kTransitionBaseFragmentShaderStringHead + transitionStr + kTransitionBaseFragmentShaderStringTail;
  if (!Filter::initWithShaderString(kTransitionBaseVertexShaderString,
                                    kTransitionBaseFragmentShaderString, 2)) {
    return false;
  }

  // texcoord attribute
  filterTexCoordAttribute_ =
      _filterProgram->getAttribLocation("inputTextureCoordinate");
  // filterTexCoordAttribute2_ =
      // _filterProgram->getAttribLocation("inputTextureCoordinate2");

  setProgress(0.0f);
  return true;
}
bool Transition::init() {
  return init(kTransitionBaseString);
}
void Transition::updateProperty(std::string name, int value) {
  auto it = _intPropertys.find(name);
  if (it != _intPropertys.end()) {
    it->second = value;
  } else {
    _intPropertys[name] = value;
  }
}
void Transition::updateProperty(std::string name, float value) {
  auto it = _floatPropertys.find(name);
  if (it != _floatPropertys.end()) {
    it->second = value;
  } else {
    _floatPropertys[name] = value;
  }
}
void Transition::updateProperty(std::string name, Vector2 value) {
  auto it = _vec2Propertys.find(name);
  if (it != _vec2Propertys.end()) {
    it->second = value;
  } else {
    _vec2Propertys[name] = value;
  }
}
void Transition::updateProperty(std::string name, Vector3 value) {
  auto it = _vec3Propertys.find(name);
  if (it != _vec3Propertys.end()) {
    it->second = value;
  } else {
    _vec3Propertys[name] = value;
  }
}

bool Transition::proceed(bool bUpdateTargets, int64_t frameTime) {
  static const GLfloat imageVertices[] = {
      -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
  };

  GPUPixelContext::getInstance()->setActiveShaderProgram(_filterProgram);
  _framebuffer->active();
  CHECK_GL(glClearColor(_backgroundColor.r, _backgroundColor.g,
                        _backgroundColor.b, _backgroundColor.a));
  CHECK_GL(glClear(GL_COLOR_BUFFER_BIT));

  // Texture 0
  CHECK_GL(glActiveTexture(GL_TEXTURE0));
  CHECK_GL(glBindTexture(GL_TEXTURE_2D,
                         _inputFramebuffers[0].frameBuffer->getTexture()));
  _filterProgram->setUniformValue("inputImageTexture", 0);

  // Texture 1
  CHECK_GL(glActiveTexture(GL_TEXTURE1));
  CHECK_GL(glBindTexture(GL_TEXTURE_2D,
                         _inputFramebuffers[1].frameBuffer->getTexture()));
  _filterProgram->setUniformValue("inputImageTexture2", 1);

  CHECK_GL(glEnableVertexAttribArray(filterTexCoordAttribute_));
  CHECK_GL(glVertexAttribPointer(
      filterTexCoordAttribute_, 2, GL_FLOAT, 0, 0,
      _getTexureCoordinate(_inputFramebuffers[0].rotationMode)));

  // vertex position
  CHECK_GL(glVertexAttribPointer(_filterPositionAttribute, 2, GL_FLOAT, 0, 0,
                                 imageVertices));

  // update uniform
  _filterProgram->setUniformValue("progress", _progress);
  for (auto property : _intPropertys){
    _filterProgram->setUniformValue(property.first, property.second);
  }
  for (auto property : _floatPropertys){
    _filterProgram->setUniformValue(property.first, property.second);
  }
  for (auto property : _vec2Propertys){
    _filterProgram->setUniformValue(property.first, property.second);
  }
  // draw
  CHECK_GL(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

  _framebuffer->inactive();

  return Source::proceed(bUpdateTargets, frameTime);
}

void Transition::setProgress(float progress) {
  this->_progress = progress;
}
NS_GPUPIXEL_END
