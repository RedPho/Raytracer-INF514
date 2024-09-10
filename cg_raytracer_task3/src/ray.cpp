#include "ray.h"

#include <iostream>

namespace cg{
  std::optional<vec3> refract(const vec3& d, const vec3& n, double ior){
    /*
    ============================================================================
     Task 3
    ============================================================================
     Calculate the refracted ray. See task description for more information.

     Be aware that the ray enters but also exits an object! The normal n will 
     always point away from the object. ior is always the index of refraction for
     the object material. We assume the ior for air is always 1 and that the 
     refraction always happens at an air-to-material or material-to-air boundary.
    */

    double cos_theta_1;
    double sin_theta_1;
    double sin_theta_2;
    double cos_theta_2;
    vec3 m;
    if (dot(d, n) < 0) { // air to material
                         // enters to object
      cos_theta_1 = dot(-d, n);
      sin_theta_1 = sqrt(1 - std::pow(cos_theta_1, 2));
      //std::cout << "cos_theta_1: " << cos_theta_1 << "\n";

      sin_theta_2 = sin_theta_1/ior;
      if (pow(sin_theta_2, 2) >= 1) {
        return vec3(0,0,0);
      }
      cos_theta_2 = sqrt(1 - std::pow(sin_theta_2, 2));
      m = d - ( dot(d, n) * n );
      if (m.length() != 0 ) { // total internal reflection
        m = normalize(m);
      }
      //std::cout << "air to material\n";
      return sin_theta_2 * m - cos_theta_2 * n;

    } else { // material to air
      //exits the object
      cos_theta_1 = dot(d, n);
      sin_theta_1 = sqrt(1 - std::pow(cos_theta_1, 2));
      sin_theta_2 = sin_theta_1 * ior;
      if (pow(sin_theta_2, 2) >= 1) {
        return vec3(0,0,0);
      }

      cos_theta_2 = sqrt(1 - std::pow(sin_theta_2, 2));
      m = d - ( dot(d, n) * n );
      if (m.length() != 0 ) { // total internal reflection
        m = normalize(m);
      }
/*    // used for debugging
      std::cout << "material to air\n";
      std::cout << "sin_theta_1: " << sin_theta_1 << "\ncos_theta_1: " << cos_theta_1 << "\nsin_theta_2: " << sin_theta_2 << "\ncos_theta_2: " << cos_theta_2 << "\nn: " << n << "\n" << "m: " << m << "\n";
*/
      return sin_theta_2 * m - cos_theta_2 * -n;
    }

      }
}