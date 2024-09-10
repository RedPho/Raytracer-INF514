Bearbeitet von Mehmet Emin Daşdan, Mehmet Duman, İpek Semra Süthan und Zeynep Saraoğlu


# Erläuterung zu den Ansätzen
ToDo: Füge hier eine Erklärung Deiner Ansätze ein!

## Aufgabenteil 1: Nähester Schnittpunkt

We iterate through all the renderables and call the check_intersection method.  
if check_intersection has a value and its time < time of current closest_hit : We assign closest_hit to this intersection.

```c++
std::optional<Intersection> closest_hit;
for (const auto& obj : m_renderables) {
    std::optional<Intersection> intersection = obj->check_intersection(r, min_t);
    if (!intersection.has_value()) {
      continue;
    }
    if (!closest_hit.has_value()) {
      closest_hit = intersection;
    }
    if (intersection.value().t < closest_hit.value().t) {
      closest_hit = intersection;
    }
}
return closest_hit;
```
## Aufgabenteil 2: Gibt es einen Schnittpunkt in einem bestimmten Intervall?

We iterate through all the elements of m_renderables and call the check_intersection method.  
if check_intersection(std::optional) has a value and time of the intersection is between the intervall, return true, else false.

```c++
for (auto &renderable : m_renderables) { 
    std::optional<Intersection> intersection = renderable->check_intersection(r, min_t);
    if (intersection.has_value() && intersection.value().t > min_t && intersection.value().t < max_t) {
        return true;  
    }  
}
`return false;
```

## Aufgabenteil 3: Liegt ein Punkt im Schatten?
We generate a Ray from position directed to the opposite of lights direction. If any intersectable is in between the light and position, return true, else false.
```c++
Ray positionToLightRay{position, -light_source.light_direction_to(position)};
if(any_intersection(positionToLightRay, 0.000001, light_source.distance_to(position))) {
    return true;
}
return false;
```

## Aufgabenteil 4: Beleuchtung mit einem Spot-Licht

We calculate the angle between light vector and light_to_position vector. 
if this angle is bigger than spotlights angle, the position is not in the spotlights area.

We used (float) casting in cos value because of [this problem](https://stackoverflow.com/questions/52138147/why-is-acos-resulting-in-nanind-when-using-the-result-of-a-dot-product)
```c++
vec3 vecToPos = light_direction_to(p);
double cos = dot(m_direction, vecToPos) / (m_direction.length() * vecToPos.length());
double angleBetweenSpotlightVecAndP = std::acos((float)cos);
if (angleBetweenSpotlightVecAndP > m_angle) {
  return vec3::zeros();
}
return spectral_intensity() / light_direction_to(p).squared_length();
```