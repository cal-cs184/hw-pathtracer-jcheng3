#include "sphere.h"

#include <cmath>

#include "pathtracer/bsdf.h"
#include "util/sphere_drawing.h"

namespace CGL {
namespace SceneObjects {

bool Sphere::test(const Ray &r, double &t1, double &t2) const {

  // TODO (Part 1.4):
  // Implement ray - sphere intersection test.
  // Return true if there are intersections and writing the
  // smaller of the two intersection times in t1 and the larger in t2.

  // sphere center to ray origin
  Vector3D oc = r.o - o;

  // at^2 + bt + c = 0
  double a = dot(r.d, r.d);
  double b = 2.0 * dot(oc, r.d);
  double c = dot(oc, oc) - r2;

  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0) {
    return false;
  }

  // intersection points
  double sqrt_discriminant = sqrt(discriminant);
  double denominator = 1.0 / (2.0 * a);

  // quadratic formula
  t1 = (-b - sqrt_discriminant) * denominator;
  t2 = (-b + sqrt_discriminant) * denominator;

  // t1 should be smaller than t2
  if (t1 > t2) {
    std::swap(t1, t2);
  }

  return true;
}

bool Sphere::has_intersection(const Ray &r) const {

  // TODO (Part 1.4):
  // Implement ray - sphere intersection.
  // Note that you might want to use the the Sphere::test helper here.
  double t1, t2;

  // run test for intersection check
  if (!test(r, t1, t2)) {
    return false;
  }

  // check if t1 is closest valid intersection
  if (t1 >= r.min_t && t1 <= r.max_t) {
    return true;
  }

  // same for t2
  if (t2 >= r.min_t && t2 <= r.max_t) {
    return true;
  }

  return false;
}

bool Sphere::intersect(const Ray &r, Intersection *i) const {

  // TODO (Part 1.4):
  // Implement ray - sphere intersection.
  // Note again that you might want to use the the Sphere::test helper here.
  // When an intersection takes place, the Intersection data should be updated
  // correspondingly.

  double t1, t2;

  // run test for intersection check
  if (!test(r, t1, t2)) {
    return false;
  }

  // check if t1 is closest valid intersection
  if (t1 >= r.min_t && t1 <= r.max_t) {
    return true;
  } else if (t2 >= r.min_t && t2 <= r.max_t) {
    return true;
  }

  // closest valid intersection
  double t_intersect = -1;

  // check if t1 or t2 is closest valid intersection
  if (t1 >= r.min_t && t1 <= r.max_t) {
    t_intersect = t1;
  } else if (t2 >= r.min_t && t2 <= r.max_t) {
    t_intersect = t2;
  } else {
    return false;
  }

  // populate the intersection structure
  isect->t = t_intersect;

  Vector3D intersection_point = r.o + t_intersect * r.d;

  // surface normal (normalized vector from sphere center to intersection point)
  isect->n = (intersection_point - o);  // vector from center to intersection
  isect->n.normalize();

  isect->primitive = this;
  isect->bsdf = get_bsdf();

  const_cast<Ray&>(r).max_t = t_intersect;

  return true;
}

void Sphere::draw(const Color &c, float alpha) const {
  Misc::draw_sphere_opengl(o, r, c);
}

void Sphere::drawOutline(const Color &c, float alpha) const {
  // Misc::draw_sphere_opengl(o, r, c);
}

} // namespace SceneObjects
} // namespace CGL
