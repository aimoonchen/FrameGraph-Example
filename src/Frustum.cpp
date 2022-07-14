#include "Frustum.hpp"

// https://www.lighthouse3d.com/tutorials/view-frustum-culling/

namespace {

[[nodiscard]] bool coneBehindPlane(const Cone &cone, const Plane &plane) {
  const auto m = glm::cross(glm::cross(plane.normal, cone.d), cone.d);
  const auto Q = cone.T + cone.d * cone.h - m * cone.r;
  return plane.distanceTo(cone.T) < 0 && plane.distanceTo(Q) < 0;
}

} // namespace

enum FrustumSide {
  Right = 0,
  Left = 1,
  Bottom = 2,
  Top = 3,
  Near = 4, // Back
  Far = 5   // Front
};
enum class IntersectionResult { Outside, Intersect, Inside };

//
// Frustum class:
//

Frustum::Frustum(const glm::mat4 &m) { update(m); }

void Frustum::update(const glm::mat4 &m) {
  // NOTE: All planes faces inwards

  m_planes[FrustumSide::Left] = Plane{
    .normal = {m[0][3] + m[0][0], m[1][3] + m[1][0], m[2][3] + m[2][0]},
    .distance = m[3][3] + m[3][0],
  };
  m_planes[FrustumSide::Right] = Plane{
    .normal = {m[0][3] - m[0][0], m[1][3] - m[1][0], m[2][3] - m[2][0]},
    .distance = m[3][3] - m[3][0],
  };
  m_planes[FrustumSide::Bottom] = Plane{
    .normal = {m[0][3] + m[0][1], m[1][3] + m[1][1], m[2][3] + m[2][1]},
    .distance = m[3][3] + m[3][1],
  };
  m_planes[FrustumSide::Top] = Plane{
    .normal = {m[0][3] - m[0][1], m[1][3] - m[1][1], m[2][3] - m[2][1]},
    .distance = m[3][3] - m[3][1],
  };
  m_planes[FrustumSide::Near] = Plane{
    .normal = {m[0][3] + m[0][2], m[1][3] + m[1][2], m[2][3] + m[2][2]},
    .distance = m[3][3] + m[3][2],
  };
  m_planes[FrustumSide::Far] = Plane{
    .normal = {m[0][3] - m[0][2], m[1][3] - m[1][2], m[2][3] - m[2][2]},
    .distance = m[3][3] - m[3][2],
  };

  for (auto &plane : m_planes)
    plane.normalize();
}

bool Frustum::testPoint(const glm::vec3 &point) const {
  for (const auto &plane : m_planes)
    if (plane.distanceTo(point) < 0) return false; // Outside

  return true; // Inside
}
bool Frustum::testSphere(const Sphere &sphere) const {
  auto result{IntersectionResult::Inside};
  for (const auto &plane : m_planes) {
    const auto distance = plane.distanceTo(sphere.c);
    if (distance < -sphere.r) return false; // Outside
    if (glm::abs(distance) < sphere.r) result = IntersectionResult::Intersect;
  }
  return result != IntersectionResult::Outside;
}
bool Frustum::testCone(const Cone &cone) const {
  bool inside = true;
  if (coneBehindPlane(cone, m_planes[FrustumSide::Near]) ||
      coneBehindPlane(cone, m_planes[FrustumSide::Far])) {
    inside = false;
  }
  for (uint32_t i{0}; i < 4 && inside; ++i)
    inside = !coneBehindPlane(cone, m_planes[i]);

  return inside;
}
bool Frustum::testAABB(const AABB &aabb) const {
  auto result{IntersectionResult::Inside};
  for (const auto &plane : m_planes) {
    auto pv = aabb.min;
    auto nv = aabb.max;
    if (plane.normal.x >= 0) {
      pv.x = aabb.max.x;
      nv.x = aabb.min.x;
    }
    if (plane.normal.y >= 0) {
      pv.y = aabb.max.y;
      nv.y = aabb.min.y;
    }
    if (plane.normal.z >= 0) {
      pv.z = aabb.max.z;
      nv.z = aabb.min.z;
    }
    if (plane.distanceTo(pv) < 0) return false;
    if (plane.distanceTo(nv) < 0) result = IntersectionResult::Intersect;
  }
  return result != IntersectionResult::Outside;
}
