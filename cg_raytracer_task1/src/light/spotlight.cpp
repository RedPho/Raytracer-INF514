#include "spotlight.h"

#include <iostream>

//==============================================================================
namespace cg {
//==============================================================================
  SpotLight::SpotLight(const vec3& direction, double angle) : 
    m_direction(normalize(direction)), m_angle(angle) {};
  SpotLight::SpotLight(const vec3& direction, double angle, 
                       double r, double g, double b) :
    Light{ r, g, b }, m_direction(normalize(direction)), m_angle(angle) {};
  SpotLight::SpotLight(const vec3& direction, double angle, 
                       const vec3& spectral_intensity) :
    Light{ spectral_intensity }, 
    m_direction(normalize(direction)), 
    m_angle(angle) {};
  //----------------------------------------------------------------------------
  vec3 SpotLight::incident_radiance_at(const vec3& p) const {
    /*
    ============================================================================
     Task 1
    ============================================================================
     Calculate the incident radiance (i.e. the incoming light intensity) that
     a point p receives from this light source.
     See task description on how a spotlight is defined.

     Hint: 
     The light intensity (spectral_intensity) decreases quadratically with the 
     distance to the light source (inverse square law).
     See also pointlight.cpp.
    */
    //quadratic fall-off since receiver area decreases quadratically
    vec3 vecToPos = light_direction_to(p);
    double cos = dot(m_direction, vecToPos) / (m_direction.length() * vecToPos.length());
    //std::cout << "cos: " << cos << std::endl;
    double angleBetweenSpotlightVecAndP = std::acos((float)cos);
    //std::cout << "angle between spotlight and p: " << angleBetweenSpotlightVecAndP << " m_angle: " <<  m_angle << std::endl;
    if (angleBetweenSpotlightVecAndP > m_angle) {
      //std::cout << "not in light!" << std::endl;
      return vec3::zeros();
    }
    return spectral_intensity() / light_direction_to(p).squared_length();
  }
  //----------------------------------------------------------------------------
  vec3 SpotLight::light_direction_to(const vec3& p) const {
    return p - pos();
  }
  //----------------------------------------------------------------------------
  double SpotLight::distance_to(const vec3& p) const {
    return (p - pos()).length();
  }
//==============================================================================
}  // namespace cg
//==============================================================================

//Bearbeitet von Mehmet Emin Daşdan, Mehmet Duman, İpek Semra Süthan und Zeynep Saraoğlu
