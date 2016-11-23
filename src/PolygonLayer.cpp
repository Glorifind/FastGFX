//
// Created by Michał Łaszczewski on 20/04/16.
//

#include "PolygonLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include "config.h"

namespace fgfx {

  GLuint PolygonLayer::polygonProgram = 0;
  GLuint PolygonLayer::polygonProgramAttribColor = 0;
  GLuint PolygonLayer::polygonProgramAttribPosition = 0;
  GLuint PolygonLayer::polygonProgramUniformCameraMatrix = 0;
  
  void PolygonLayer::initializePolygonProgram() {
    
    const char vShaderStr[] =
      "attribute vec3 aVertexPosition;\n"\
      "attribute vec4 aVertexColor;\n"
      "uniform mat4 uCameraMatrix;\n"
      "varying vec4 vColor;\n"
      "void main() {\n"
      "  gl_Position = uCameraMatrix*vec4(aVertexPosition, 1.0);\n"
      "  vColor = aVertexColor;\n"
      "}\n";

    const char fShaderStr[] =
      "precision mediump float;\n"\
      "varying vec4 vColor;\n"
      "void main() {\n"
      "  gl_FragColor = vColor;\n"
      "}\n";

    auto vertexShader = fgfx::loadShader(GL_VERTEX_SHADER, vShaderStr);
    auto fragmentShader = fgfx::loadShader(GL_FRAGMENT_SHADER, fShaderStr);

    auto polygonProgram = fgfx::createProgram(vertexShader, fragmentShader);
    if (!polygonProgram) throw "nie ma programu!";
    PolygonLayer::polygonProgram = polygonProgram;

    PolygonLayer::polygonProgramAttribPosition = glGetAttribLocation(polygonProgram, "aVertexPosition");
    PolygonLayer::polygonProgramAttribColor = glGetAttribLocation(polygonProgram, "aVertexColor");
    PolygonLayer::polygonProgramUniformCameraMatrix = glGetUniformLocation(polygonProgram, "uCameraMatrix");
  }

  void PolygonLayer::beginDraw(glm::mat4 cameraMatrix) {
    glUseProgram(PolygonLayer::polygonProgram);
    glEnableVertexAttribArray(PolygonLayer::polygonProgramAttribPosition);
    glEnableVertexAttribArray(PolygonLayer::polygonProgramAttribColor);

    glUniformMatrix4fv(PolygonLayer::polygonProgramUniformCameraMatrix, 1, GL_FALSE, (GLfloat*)glm::value_ptr(cameraMatrix));
  }

  void PolygonLayer::endDraw() {
    glDisableVertexAttribArray(PolygonLayer::polygonProgramAttribPosition);
    glDisableVertexAttribArray(PolygonLayer::polygonProgramAttribColor);
  }

  PolygonLayer::PolygonLayer(Engine* enginep) : Layer(enginep), polygonPointsCount(0) {
    glGenBuffers(1,&positionBuffer);
    glGenBuffers(1,&colorBuffer);
  }
  PolygonLayer::~PolygonLayer() {
    glDeleteBuffers(1,&positionBuffer);
    glDeleteBuffers(1,&colorBuffer);
  }

  void PolygonLayer::reserve(int pointCount) {
    polygonVertices.reserve(pointCount);
    polygonColors.reserve(pointCount);
  }
  void PolygonLayer::reserveMore(int pointCount) {
    polygonVertices.reserve(pointCount + polygonPointsCount);
    polygonColors.reserve(pointCount + polygonPointsCount);
  }


  void PolygonLayer::bufferPoint(glm::vec3 pos, glm::vec4 color) {
    if(frozen && uploaded) return;
    /*emscripten_log(EM_LOG_ERROR,"BUFFER POLYGON POINT %d (%f %f %f) COLOR (%f %f %f %f)\n",polygonPointsCount,
                   pos.x,pos.y,pos.z,color.r,color.g,color.b,color.a);*/
    polygonVertices.push_back(pos);
    polygonColors.push_back(color);
    polygonPointsCount++;
  }
  void PolygonLayer::bufferTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec4 color) {
    bufferPoint(p1,color);
    bufferPoint(p2,color);
    bufferPoint(p3,color);
  }
  void PolygonLayer::bufferTriangle(glm::vec3 p[3], glm::vec4 color) {
    bufferPoint(p[0],color);
    bufferPoint(p[1],color);
    bufferPoint(p[2],color);
  }
  void PolygonLayer::bufferQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec4 color) {
    bufferPoint(p1,color);
    bufferPoint(p2,color);
    bufferPoint(p3,color);
    bufferPoint(p1,color);
    bufferPoint(p3,color);
    bufferPoint(p4,color);
  }
  void PolygonLayer::bufferQuad(glm::vec3 p[4], glm::vec4 color) {
    bufferPoint(p[0],color);
    bufferPoint(p[1],color);
    bufferPoint(p[2],color);
    bufferPoint(p[0],color);
    bufferPoint(p[2],color);
    bufferPoint(p[3],color);
  }

  void PolygonLayer::reset() {
    if(!visible) return;
    if(frozen) return;
    polygonVertices.clear();
    polygonColors.clear();
    polygonPointsCount = 0;
    uploaded=false;
  }
  void PolygonLayer::upload() {
    if(!visible) return;
    if(uploaded) return;
    if(!polygonPointsCount) return;

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, polygonPointsCount * sizeof(glm::vec3),polygonVertices.data(),
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, polygonPointsCount * sizeof(glm::vec4),polygonColors.data(),
                 GL_DYNAMIC_DRAW);

    uploaded=true;
  }
  void PolygonLayer::render(glm::mat4 cameraMatrix) {
    if(!polygonPointsCount) return;

    beginDraw(cameraMatrix);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glVertexAttribPointer(PolygonLayer::polygonProgramAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(PolygonLayer::polygonProgramAttribColor, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, polygonPointsCount);

    endDraw();
  }
}