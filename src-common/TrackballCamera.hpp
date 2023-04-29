#ifndef __TCAMERA__
#define __TCAMERA__

#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/quaternion.hpp"
#include <algorithm>

class TrackballCamera {
private:
  float _distance;
  float _angleX;
  float _angleY;

public:
  TrackballCamera(float distance = 5, float angleX = 0, float angleY = 0);

  inline void moveFront(float distance) {
    _distance = std::min(std::max(_distance - distance, 0.f), 6.f);
  };
  inline void rotateLeft(float angle) { _angleX += angle; };
  inline void rotateUp(float angle) { _angleY += angle; };

  glm::mat4 getViewMatrix() const;
};

#endif
