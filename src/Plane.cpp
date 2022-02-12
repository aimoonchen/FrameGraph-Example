#include "Plane.hpp"
#include "glm/geometric.hpp"

void Plane::normalize() {
  const auto magnitude = glm::length(normal);
  normal /= magnitude;
  distance /= magnitude;
}
float Plane::distanceTo(const glm::vec3 &p) const {
  return glm::dot(normal, p) + distance;
}
