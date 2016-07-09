#ifndef VECTORS_H
#define VECTORS_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace fgfx {

  class Vec2 : public glm::vec2 {
  public:
    Vec2(const glm::vec2 src) {
      x=src.x;
      y=src.y;
    }
    Vec2(double xp, double yp) {
      x=xp;
      y=yp;
    }
    Vec2() {
      x = 0;
      y = 0;
    }
  };

  class Vec3 : public glm::vec3 {
  public:
    Vec3(const glm::vec3 src) {
      x=src.x;
      y=src.y;
      z=src.z;
    }
    Vec3(double xp, double yp, double zp) {
      x=xp;
      y=yp;
      z=zp;
    }
    Vec3() {
      x = 0;
      y = 0;
      z = 0;
    }
  };

};

#endif // VECTORS_H
