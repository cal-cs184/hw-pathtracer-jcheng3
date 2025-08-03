#include "triangle.h"

#include "CGL/CGL.h"
#include "GL/glew.h"

namespace CGL {
namespace SceneObjects {

Triangle::Triangle(const Mesh *mesh, size_t v1, size_t v2, size_t v3) {
  p1 = mesh->positions[v1];
  p2 = mesh->positions[v2];
  p3 = mesh->positions[v3];
  n1 = mesh->normals[v1];
  n2 = mesh->normals[v2];
  n3 = mesh->normals[v3];
  bbox = BBox(p1);
  bbox.expand(p2);
  bbox.expand(p3);

  bsdf = mesh->get_bsdf();
}

BBox Triangle::get_bbox() const { return bbox; }

bool Triangle::has_intersection(const Ray &r) const {
  // Part 1, Task 3: implement ray-triangle intersection
  // The difference between this function and the next function is that the next
  // function records the "intersection" while this function only tests whether
  // there is a intersection.

  // vertices and edges
  Vector3D v0 = p1;
  Vector3D v1 = p2;
  Vector3D v2 = p3;
  Vector3D e1 = v1 - v0;
  Vector3D e2 = v2 - v0;

  Vector3D s = r.o - v0;
  Vector3D s1 = cross(r.d, e2);
  Vector3D s2 = cross(s, e1);

  double det = dot(s1, e1);

  // check if det is close to 0 so dont divide by 0 next
  const double EPSILON = 1e-8;
  if (det > -EPSILON && det < EPSILON) {
    return false;
  }

  double inv_det = 1.0 / det;

  double u = dot(s2, e2);
  double v = dot(s1, s);
  double w = dot(s2, r.d);

  double t = inv_det * (u);
  double b1 = inv_det * (v); //barycentric
  double b2 = inv_det * (w); // barycentric

  // check barycentric cords inside triangle
  if (b1 < 0.0 || b1 > 1.0) {
    return false;
  }

  if (b2 < 0.0 || b1 + b2 > 1.0) {
    return false;
  }

  // check intersection within range
  if (t >= r.min_t && t <= r.max_t) {
    return true;
  }

  return false;

}

bool Triangle::intersect(const Ray &r, Intersection *isect) const {
  // Part 1, Task 3:
  // implement ray-triangle intersection. When an intersection takes
  // place, the Intersection data should be updated accordingly

  // vertices and edges
  Vector3D v0 = p1;
  Vector3D v1 = p2;
  Vector3D v2 = p3;
  Vector3D e1 = v1 - v0;
  Vector3D e2 = v2 - v0;

  Vector3D s = r.o - v0;
  Vector3D s1 = cross(r.d, e2);
  Vector3D s2 = cross(s, e1);

  double det = dot(s1, e1);

  // check if det is close to 0 so dont divide by 0 next
  const double EPSILON = 1e-8;
  if (det > -EPSILON && det < EPSILON) {
    return false;
  }

  double inv_det = 1.0 / det;

  double u = dot(s2, e2);
  double v = dot(s1, s);
  double w = dot(s2, r.d);

  double t = inv_det * (u);
  double b1 = inv_det * (v); //barycentric
  double b2 = inv_det * (w); // barycentric

  // check barycentric cords inside triangle
  if (b1 < 0.0 || b1 > 1.0) {
    return false;
  }

  if (b2 < 0.0 || b1 + b2 > 1.0) {
    return false;
  }

  // check intersection is within valid t range
  if (t >= r.min_t && t <= r.max_t) {

    // calculate barycentric coords
    double b0 = 1.0 - b1 - b2;  // barycentric coordinate for v0

    isect->t = t;

    // interpolate normal with barycentric coordinates
    isect->n = b0 * n1 + b1 * n2 + b2 * n3;
    isect->n.normalize();

    // primitive points to the primitive that was intersected (use this pointer).
    isect->primitive = this;
    isect->bsdf = get_bsdf();

    const_cast<Ray&>(r).max_t = t;

    return true;
  }

  return false;

}

void Triangle::draw(const Color &c, float alpha) const {
  glColor4f(c.r, c.g, c.b, alpha);
  glBegin(GL_TRIANGLES);
  glVertex3d(p1.x, p1.y, p1.z);
  glVertex3d(p2.x, p2.y, p2.z);
  glVertex3d(p3.x, p3.y, p3.z);
  glEnd();
}

void Triangle::drawOutline(const Color &c, float alpha) const {
  glColor4f(c.r, c.g, c.b, alpha);
  glBegin(GL_LINE_LOOP);
  glVertex3d(p1.x, p1.y, p1.z);
  glVertex3d(p2.x, p2.y, p2.z);
  glVertex3d(p3.x, p3.y, p3.z);
  glEnd();
}

} // namespace SceneObjects
} // namespace CGL
