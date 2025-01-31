#include "triangle.h"

#include <iostream>
#include <limits>

//==============================================================================
namespace cg {
//==============================================================================
Triangle::Triangle(const vec3& x0, const vec3& x1, const vec3& x2,
                   const Material& mat)
    : Renderable{mat},
      m_vertices{x0, x1, x2},
      m_uv_coordinates{vec2{0.0, 0.0}, vec2{1.0, 0.0}, vec2{0.0, 1.0}},
      m_normal{normalize(cross(x1 - x0, x2 - x0))} {}

std::array<vec3, 3> Triangle::vertices() const {
  std::array<vec3, 3> trVertices;
  std::transform(m_vertices.begin(), m_vertices.end(),
                 trVertices.begin(),
                 [this](const vec3& v) {return transform_point(v);});
  return trVertices;
}
//------------------------------------------------------------------------------
std::optional<Intersection> Triangle::check_intersection(const Ray& r,
                                                         double min_t) const {

  /*
  ============================================================================
   Task 2
  ============================================================================
   Implement a ray-triangle intersection.
   
   The triangle vertices are given below as v0, v1 and v2. The normal is given
   by n.

   The class vec3 already implements basic vector operations like + and - as
   well as the dot and cross product and length. You don't need to implement 
   it yourself. (see vec.h)

   The return type is std::optional<>. This means you can return
   an Intersection if the ray is intersecting or {} if there is no 
   intersection.

   See e.g.
   https://en.cppreference.com/w/cpp/utility/optional
   and
   intersection.h
  */
  const vec3 v0 = transform_point(m_vertices[0]);
  const vec3 v1 = transform_point(m_vertices[1]);
  const vec3 v2 = transform_point(m_vertices[2]);

  const vec3 n = normal();

  // you can return an intersection like this
  // return Intersection{this, r, t, pos, normal, uv};
  /* where
      this     : is a pointer to the triangle
      r        : the ray intersecting
      t, pos   : t value and world space position of the intersection
                 (needs to be calculated by you)
      normal   : the surface normal in world space coordinates
                 (given) 
      uv       : local uv texture coordinates of the triangle 
                 (needs to be calculated by you, 
                 hint: barycentric coordinates)
  */

  /*
    Plane: dot(n,x) = c
    n is given.
    x is a vec3 where
    we need to find c.
    to find -> equ is true for all vertices of triange. We used v0 here.
  */
  double c = dot(n, v0);
  if (dot(n, r.direction()) == 0) { //no intersection, ray and plane is parallel.
    return {};
  }
  double t = (c - dot(n, r.origin())) / dot(n, r.direction());
  if (t <= 0) {
    return {};
  }
  vec<3> pos = r.origin() + r.direction()*t;

  //check if pos in triangle:
  if ((dot(cross(v1-v0, pos-v0), n) >= 0) && (dot(cross(v2-v1, pos-v1), n) >= 0) && (dot(cross(v0-v2, pos-v2), n) >= 0)) {
    return Intersection{this, r, t, pos, n, vec2{0.0, 0.0}};
  }
  return {};
  }
//==============================================================================
}  // namespace cg
//==============================================================================
//Bearbeitet von Mehmet Emin Daşdan, Mehmet Duman, Zeynep Saraoğlu und İpek Semra Süthan