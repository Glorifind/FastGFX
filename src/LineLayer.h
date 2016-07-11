//
// Created by Michał Łaszczewski on 20/04/16.
//

#ifndef ENGINE_VECTORLAYER_H
#define ENGINE_VECTORLAYER_H

#include "Layer.h"
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>
#include "shaders.h"

namespace fgfx {

  class LineLayer : public Layer {
  protected:
    std::vector<glm::vec3> lineVertices;
    std::vector<glm::vec4> lineColors;
    int linePointsCount;

    GLuint positionBuffer;
    GLuint colorBuffer;

    friend class Engine;

    static GLuint lineProgram;
    static GLuint lineProgramAttribPosition;
    static GLuint lineProgramAttribColor;
    static GLuint lineProgramUniformCameraMatrix;

    static void initializeLineProgram();
    void beginDraw(glm::mat4 cameraMatrix);
    void endDraw();
  public:
    int composition;
    bool wireframe;
    bool uploaded;

    LineLayer(Engine* enginep);
    ~LineLayer();

    void reserve(int pointCount);
    void bufferPoint(glm::vec3 pos, glm::vec4 color);
    void bufferLine(glm::vec3 p1, glm::vec3 p2, glm::vec4 color);

    virtual void reset() override;
    virtual void upload() override;
    virtual void render(glm::mat4 cameraMatrix) override;
  };

}


#endif //ENGINE_VECTORLAYER_H
