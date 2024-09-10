Bearbeitet von Mehmet Emin Daşdan, Mehmet Duman, Zeynep Saraoğlu und İpek Semra Süthan  

# Erläuterung zu den Ansätzen  

# Aufgabe -- Kompilieren & Ausführen
We used those commands to compile the project in a system that operates with Arch Linux:  
mkdir build(create build directory)  
cd build(change directory into the build directory)  
cmake ../ (create the make files using cmake tool (cmakelist.txt was already given))  
make all (compile using make)  


# Aufgabe -- Phong--Lighting
## BRDF–Phong
in phong brdf function: pd/pi + ((s+2)/2pi) * ps * cos(phi)^s, the term pd/pi is the diffuse term, because it is related to diffuse value of the object,  
the rest is the specular term because it is all related to specular value.  
Calculated the diffuse factor using the rho_d(it was called diffuser Reflexionsgrad in our classes and slide materials). we simply divide it by pi then get the diffuse part of the formula.  
To calculate the specular, first calculated the perfect reflection of the light to point ray(R, it was given as wr in our class materials, calculated with: 2 * <wi, n> * n - wi ) wi is the light ray(from point to light source)  
then applied the specular part of the formula. Then summed diffuse and specular part so we could get the whole brdf calculation.  
```c++
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
```
## Shade
light_source.incident_radiance_at(p) gives the light intensity in a spesific point so we used it directly.  
What we meant by light_ray and view_ray are "point to light" vector and "point to camera" vector.  
Normalized them so we could directly use them to calculate the cosine of the angle between light_ray and surface normal in that point and in brdf_phong function we used those vectors as it is(without normalization) so we need to make sure that we pass a vector with a length 1.  

```c++
  vec3 p = hit.position;
  vec3 incident_radiance = light_source.incident_radiance_at(p);
  vec3 n = hit.normal;
  vec3 light_ray = normalize(light_source.pos() - p);
  double cos_theta = dot(light_ray, n);
  vec3 E = incident_radiance*cos_theta;
  vec3 viewRay = normalize(hit.incident_ray.origin() - p);

  return E * brdf_phong(view_ray, light_ray, n, hit.uv);
```

# Aufgabe -- Spiegelung

using shade_closest_intersection(incident_ray.reflect(hit->position, hit->normal), 0.00001), we pass the reflected ray(using ray reflect member function) to the next recursive step of the function.  
Just like the above one, using shade_closest_intersection(incident_ray.refract(hit->position, hit->normal, renderable->index_of_refraction()).value(), 0.00001) we pass the refracted ray to the next recursive step.  

```c++
vec3 Scene::shade_closest_intersection(const Ray& incident_ray,
                                       double     min_t) const {
  const auto hit = closest_intersection(incident_ray, min_t);
  if (hit) {
    auto renderable   = dynamic_cast<const Renderable*>(hit->intersectable);
    auto shaded_color = vec3::zeros();
    // first apply the lighting model for each light source
    for (const auto& light_source : lights()) {
      if (!in_shadow(*light_source, hit->position + 1e-6*hit->normal)) {
          shaded_color +=
            renderable->shade(*light_source, *hit) *
            (1 - renderable->reflectance() - renderable->refractance());
            // only a part of the total incoming light is used for shading the
            // actual object. The rest is reflected or refracted. The fraction
            // of light that is transported away is given by the reflectance
            // and refractance constants of the material.
      }
    }

    /*
    ============================================================================
     Task 3
    ============================================================================
     Implement the calculation of the color that is showing in the reflection on
     this object.
     Remember that ray tracing is recursive.
    */
    if (renderable->reflectance() > 0 &&
        incident_ray.num_reflections() < max_num_reflections) {
      vec3 reflected_color = shade_closest_intersection(incident_ray.reflect(hit->position, hit->normal), 0.00001);

      shaded_color +=
        reflected_color *
        renderable->sample_reflective_color(hit->uv) *
        renderable->reflectance();
    }

    /*
    ============================================================================
     Task 3
    ============================================================================
     Implement refraction analogously to reflection above.
    */
    if (renderable->refractance() > 0 &&
        incident_ray.num_reflections() < max_num_reflections) {
      vec3 refracted_color = shade_closest_intersection(incident_ray.refract(hit->position, hit->normal, renderable->index_of_refraction()).value(), 0.00001);

      shaded_color +=
        refracted_color *
        renderable->sample_refractive_color(hit->uv) *
        renderable->refractance();
    }
        return shaded_color;
  }
  return m_background_color;
}
```


# Aufgabe -- Material

Fully diffuse materials have no reflectance and shininess.  
Perfect mirrors have 100% (so 1) reflectence.  
We couldn't figure out the redblack one, actually the code renders the ppm image quite right but it couldn't pass the tests.  
```c++
  Phong phong_redblack{black, 0.0, cyan, 0.0};
  Phong phong_diffuse{white, 0.0, offwhite, 0.0};
  Phong phong_mirror{black, 1.00, white, 0.0};
```

# Aufgabe -- Refraktion

in ray.cpp:  
Here, we declared the variables before calculations because the ray can enter or exit the object, we done different calculations for those two situation.  
d is d1 from the image given in task.  
the other variable names are taken from the task description.
d is the ray direction. if dot product of d and n is less than 0, angle between them are bigger than pi/2. It means the ray enters the object, else, ray exits the object. Because the normal always points away from the object.  

The Snell’s law tells:  
sin(theta_1)/sin(theta_2) = material 2 index of refraction / material 1 index of refraction  

to find cos_theta_1, in air to material we calculate the dot product of -d and n.  
-d because we need the opposite direction of d (again, because the normal always points away from the object.) in material to air part we can simply use dot product of d and n.  
We calculated m with d - ( dot(d, n) * n ) because cosine(angle between d and n) * n gives the projection of d in n.  
Again, d and n are already normalized so we could just use dot product as cosine. Then we subtracted this value from d to get the orthogonal vector to n.  
sin_theta_2^2 > 1 is mathematically and physically impossible, so from this value, it becomes a total internal reflection. At this angle, the ray is totally reflected, so refraction is 0.  
Then used the formula given in tast information.  

```c++
    double cos_theta_1;
    double sin_theta_1;
    double sin_theta_2;
    double cos_theta_2;
    vec3 m;
    if (dot(d, n) < 0) { // air to material
                         // enters to object
      cos_theta_1 = dot(-d, n);
      sin_theta_1 = sqrt(1 - std::pow(cos_theta_1, 2));

      sin_theta_2 = sin_theta_1/ior;
      if (pow(sin_theta_2, 2) >= 1) { // total internal reflection
        return vec3(0,0,0);
      }
      cos_theta_2 = sqrt(1 - std::pow(sin_theta_2, 2));
      m = d - ( dot(d, n) * n );
      if (m.length() != 0 ) { // we can't normalize a vector that has length of 0
        m = normalize(m);
      }
      return sin_theta_2 * m - cos_theta_2 * n;

    } else { // material to air
      //exits the object
      cos_theta_1 = dot(d, n);
      sin_theta_1 = sqrt(1 - std::pow(cos_theta_1, 2));
      sin_theta_2 = sin_theta_1 * ior;
      if (pow(sin_theta_2, 2) >= 1) { // total internal reflection
        return vec3(0,0,0);
      }

      cos_theta_2 = sqrt(1 - std::pow(sin_theta_2, 2));
      m = d - ( dot(d, n) * n );
      if (m.length() != 0 ) { // we can't normalize a vector that has length of 0
        m = normalize(m);
      }
      return sin_theta_2 * m - cos_theta_2 * -n; // this is the d2(from task description)
    }
```
