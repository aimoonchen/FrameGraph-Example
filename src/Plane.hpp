#pragma once

#include "glm/vec3.hpp"

struct Plane {
  glm::vec3 normal{0.0f};
  float distance{0.0f};

  void normalize();
  [[nodiscard]] float distanceTo(const glm::vec3 &) const;
};
