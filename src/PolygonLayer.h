//
// Created by Michał Łaszczewski on 20/04/16.
//

#ifndef ENGINE_POLYGONLAYER_H
#define ENGINE_POLYGONLAYER_H

#include "Layer.h"
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>
#include "shaders.h"

namespace fgfx {

  class PolygonLayer : public Layer {
  protected:
    std::vector<glm::vec3> polygonVertices;
    std::vector<glm::vec4> polygonColors;
    int polygonPointsCount;

    GLuint positionBuffer;
    GLuint colorBuffer;

    friend class Engine;

    static GLuint polygonProgram;
    static GLuint polygonProgramAttribPosition;
    static GLuint polygonProgramAttribColor;
    static GLuint polygonProgramUniformCameraMatrix;

    static void initializePolygonProgram();
    void beginDraw(glm::mat4 cameraMatrix);
    void endDraw();
  public:
    int composition;
    bool wireframe;
    bool uploaded;

    PolygonLayer(Engine* enginep, std::string namep);
    ~PolygonLayer();

    void reserve(int pointCount);

    void bufferPoint(glm::vec3 pos, glm::vec4 color);
    void bufferTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec4 color);
    void bufferTriangle(glm::vec3 p[3], glm::vec4 color);
    void bufferQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec4 color);
    void bufferQuad(glm::vec3 p[4], glm::vec4 color);

    /// TODO: Buffer polygon, buffer line(with weight)

    virtual void reset() override;
    virtual void upload() override;
    virtual void render(glm::mat4 cameraMatrix) override;
  };

}


#endif //ENGINE_POLYGONLAYER_H
