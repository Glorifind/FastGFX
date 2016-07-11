#ifndef LAYER_H
#define LAYER_H

#include <string>

#include <glm/glm.hpp>

namespace fgfx {

  class Engine;

  enum LayerComposition {
    Additive,
    Transparent
  };

  class Layer {
  public:
    Engine* engine;
    int composition;
    bool frozen;
    bool visible;
    bool camera;
    glm::mat4 cameraMatrix;
    Layer(Engine* enginep);
    virtual ~Layer() = 0;

    void setFrozen(bool frozenp);
    void setComposition(int compositionp);
    void setCamera(bool camerap);
    void setVisible(bool visiblep);

    std::string getName();
    bool isVisible();

    virtual void reset() = 0;
    virtual void upload() = 0;
    virtual void render(glm::mat4 cameraMatrix) = 0;

  };

};

#endif // LAYER_H
