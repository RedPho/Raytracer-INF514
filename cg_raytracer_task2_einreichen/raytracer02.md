Bearbeitet von Mehmet Emin Daşdan, Mehmet Duman, Zeynep Saraoğlu und İpek Semra Süthan

# Erläuterung zu den Ansätzen

## Aufgabenteil 1: Schnittpunkte mit der Kugel

### solve_quadratic:
We find the roots of the quadratic with discriminant method.
```c++
  double delta{b*b - 4*a*c};
  if (delta < 0) {
    return {};
  }

  std::pair<double, double> solutions;
  solutions.first = (-b - sqrt(delta))/(2*a);
  solutions.second = (-b + sqrt(delta))/(2*a);

  return solutions; 
```
### check_intersection:
. is dot product, * is multiplication.  
d is the direction of ray,  
p is the position of the ray origin,  
ce is the center of the sphere.
to find the t values of intersection of ray and sphere, we solve the equation:  
when we set x = p + t*d (ray equation) in the (x-c).(x-c) - r^2 = 0 (sphere equation), we get:  
(d.d)t^2 + 2d.(p-ce)t + (p-ce).(p-ce) - r^2 = 0

for a*x^2 + b*x + c = 0:  
our x is r  
a is (d.d)  
b is 2d.(p-c)  
c is (p-ce).(p-ce) - r^2  
we used these values as solve_quadratic parameters.  
if first t_value is bigger than 0: this is the first intersection.  
else: if second t_value is bigger than 0, this is the first intersection.
```c++
  vec3 d = r.direction();
  vec3 p = r.origin();

  std::optional<std::pair<double, double>> t_values = solve_quadratic(dot(d, d), dot(2*d, (p-center)), dot(p-center, p-center) - radius*radius);
  if (t_values.has_value()) {
    vec3 intersection_pos;
    double t{};
    if (t_values->first > 0) {
      intersection_pos = r.origin() + r.direction()*t_values->first;
      t = t_values->first;
    } else if (t_values->second > 0) {
      intersection_pos = r.origin() + r.direction()*t_values->second;
      t = t_values->second;
    } else {
      return {};
    }
    return Intersection{this, r, t, intersection_pos, normalize(intersection_pos-center),  vec2{0.0, 0.0}};
  }
  return {};
```

## Aufgabenteil 2: Schnittpunkte mit einem Dreieck

We used [this approach](https://courses.cs.washington.edu/courses/csep557/10au/lectures/triangle_intersection.pdf)  
To find the intersection, first we find the supporting plane of the triangle.  
Then calculate where the plane and ray intersect.  
if they intersect, then check if the intersection position lies inside of triangle.  
```c++
  /*
    Plane: dot(n,x) = c
    n is the normal and it is given.
    x is a vec3 where plane equation is true.
    we need to find c.
    to find: we set x = v0 in plane equation. It works with all vertices of triangle. We used v0 here.
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
```


## Bonus Aufgabenteil 3: Schnittpunkte mit mit einem Bézier-Patch
We used a recursive approach.  
In every call, we calculate the b^r,0 to b^r,i for the next r for next r value. r and i are used same as in lecture.  
if the next r has only one i for b^r,i, then it is the approximated point.  
The points before that give the tangent.
```c++
  std::pair<vec3, vec3> BezierPatchMesh::de_casteljau(std::vector<vec3> control_points, double t) const {
    std::vector<vec3> nextOrderControlPoints{};
    for (int i = 0; i < control_points.size()-1; i++) {
      vec3 p = (1-t)*control_points[i] + t*control_points[i+1];
      nextOrderControlPoints.push_back(p);
    }
    if (nextOrderControlPoints.size() == 1) {
      return {nextOrderControlPoints[0], control_points[1] - control_points[0]};
    }
    return de_casteljau(nextOrderControlPoints, t);
  }
```