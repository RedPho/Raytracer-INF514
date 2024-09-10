#define _USE_MATH_DEFINES
#include "phong.h"

#include <cmath>
#include <iostream> //for debugging
#include "colors.h"
#include "constantcolorsource.h"
//==============================================================================
namespace cg {
//==============================================================================
Phong::Phong(const ColorSource& albedo_color, double shininess)
    : Material{albedo_color, 0.5, ConstantColorSource{0.95, 0.95, 0.95}},
      m_shininess{shininess} {}
//------------------------------------------------------------------------------
Phong::Phong(const ColorSource& albedo_color, const double reflectance,
             const ColorSource& reflective_color, double shininess)
    : Material{albedo_color, reflectance, reflective_color},
      m_shininess{shininess} {}
//------------------------------------------------------------------------------
Phong::Phong(const ColorSource& albedo_color, const double reflectance,
             const ColorSource& reflective_color, const double refractance,
             const double       index_of_refraction,
             const ColorSource& refractive_color, double shininess)
    : Material{albedo_color, reflectance,         reflective_color,
               refractance,  index_of_refraction, refractive_color},
      m_shininess{shininess} {}
//------------------------------------------------------------------------------
Phong::Phong(const color& col, double shininess)
    : Material{ConstantColorSource{col}}, m_shininess{shininess} {}
//------------------------------------------------------------------------------
Phong::Phong(const color& albedo_color, const double reflectance,
             const color& reflective_color, double shininess)
    : Material{ConstantColorSource{albedo_color}, reflectance,
               ConstantColorSource{reflective_color}},
      m_shininess{shininess} {}
//------------------------------------------------------------------------------
Phong::Phong(const color& albedo_color, const double reflectance,
             const color& reflective_color, const double refractance,
             const double index_of_refraction, const color& refractive_color,
             double shininess)
    : Material{ConstantColorSource{albedo_color},
               reflectance,
               ConstantColorSource{reflective_color},
               refractance,
               index_of_refraction,
               ConstantColorSource{refractive_color}},
      m_shininess{shininess} {}
//==============================================================================
color Phong::brdf_phong(const vec3& V, const vec3& L, const vec3& N,
                        const vec2& uv) const {

  /*
  ============================================================================
   Task 3
  ============================================================================
   Implement the physically plausible Phong BRDF function f_r.
   The diffuse and specular reflectance coefficients rho_d and rho_s are
   colors instead of scalar values (see task description). See material.h for
   what colors are available.

   The test assumes that the parameters L and V point towards the surface and
   N away from it.

   The number pi is given by M_PI.

   See header file for a description of the parameters of the function.
  */

  // calculate diffuse
  vec3 rho_d = sample_albedo_color(uv);
  vec3 diffuse = rho_d/M_PI;

  // calculate specular
  vec3 R = 2 * dot(L, N) * N - L;
  double cos_phi = dot(V, R);
  double s = shininess();
  vec3 rho_s = sample_reflective_color(uv);
  vec3 specular = ((s+2)/(2*M_PI)) * rho_s * std::pow(std::max(0.0, cos_phi), s); //

  return diffuse + specular;
  }
//------------------------------------------------------------------------------
vec3 Phong::shade(const Light& light_source, const Intersection& hit) const {

  /*
  ============================================================================
   Task 3
  ============================================================================
   Implement the outgoing radiance f_r * E. Use the function brdf_phong.
   Also check what methods and members the light_source (light.h) and 
   intersection (intersection.h) offer for accessing the needed values to
   calculate the value.

   The vector class (vec.h) already implements functions such as dot and cross 
   product as well as basic arithmetic operations such as + and -.
  */
  vec3 p = hit.position;
  vec3 incident_radiance = light_source.incident_radiance_at(p);
  vec3 n = hit.normal;
  vec3 light_ray = normalize(light_source.pos() - p);
  double cos_theta = dot(light_ray, n);
  vec3 E = incident_radiance*cos_theta;
  vec3 view_ray = normalize(hit.incident_ray.origin() - p);

  return E * brdf_phong(view_ray, light_ray, n, hit.uv);

  }
//==============================================================================
}  // namespace cg
//==============================================================================
//Bearbeitet von Mehmet Emin Daşdan, Mehmet Duman, Zeynep Saraoğlu und İpek Semra Süthan

