#ifndef UTILS_HPP
#define UTILS_HPP
// some utility functions that model compute shader functions
#include <glm/glm.hpp>

#include <cmath>
#include <cstdlib>
#include <functional>
#include <numeric>
#include <random>

using vec3 = glm::vec3;
using vec4 = glm::vec4;
using vec2 = glm::vec2;

float PI = 3.1415926535;
inline float degree_to_radian(float degree) {
  //
  return degree * PI / 180;
}

//
//
inline float random_double(float min, float max) {
  // random double number in range [min, max]
  static std::uniform_real_distribution<float> distr(min, max);
  static thread_local std::mt19937 gen;
  static std::function<float()> rand_gen = std::bind(distr, gen);
  return rand_gen();
}

inline float random_double() { return random_double(0, 1); }
inline int random_int() { return static_cast<int>(random_double()); }
inline int random_int(int min, int max) {
  return static_cast<int>(random_double(min, max));
}
vec3 random_vec() {
  // random vector
  return vec3(random_double(), random_double(), random_double());
}
vec3 random_vec(float mi, float ma) {
  // random vector in given seed
  return vec3(random_double(mi, ma), random_double(mi, ma),
              random_double(mi, ma));
}
vec3 random_in_unit_sphere() {
  // random in unit sphere
  while (true) {
    //
    vec3 v = random_vec(-1, 1);
    if (dot(v, v) >= 1) {
      continue;
    }
    return v;
  }
}
vec3 random_unit_vector() {
  // unit vector
  float a = random_double(0, 2 * PI);
  float z = random_double(-1, 1);
  float r = sqrt(1 - z * z);
  return vec3(r * cos(a), r * sin(a), z);
}
vec3 random_in_hemisphere(vec3 normal) {
  // normal ekseninde dagilan yon
  vec3 unit_sphere_dir = random_in_unit_sphere();
  if (dot(unit_sphere_dir, normal) > 0.0) {
    return unit_sphere_dir;
  } else {
    return -1.0f * unit_sphere_dir;
  }
}
vec3 random_in_unit_disk() {
  // lens yakinsamasi için gerekli
  while (true) {
    vec3 point = vec3(random_double(-1, 1), random_double(-1, 1), 0);
    if (dot(point, point) >= 1) {
      continue;
    }
    return point;
  }
}

// -------------- making of structs ------------------
struct Ray {
  /*N = C + mY
N: nokta
C: cikis yeri
m: mesafe/buyukluk
Y: yonu
   */
  vec3 direction;
  vec3 origin;
};
Ray makeRay(vec3 orig, vec3 dir) {
  Ray r;
  r.origin = orig;
  r.direction = dir;
  return r;
}

struct RayCameraLens {

  vec3 origin;
  vec3 horizontal;
  vec3 vertical;
  vec3 lower_left_corner;
  vec3 u;
  vec3 v;
  vec3 w;
  float lens_radius;
};
RayCameraLens makeCamera(vec3 pos, vec3 target, vec3 up, float vfov,
                         float aspect_ratio, float aperture, float focus_dist) {
  // make camera struct
  RayCameraLens cam;
  cam.origin = pos;
  cam.lens_radius = aperture / 2;

  float theta = degree_to_radian(vfov);
  float half_height = tan(theta / 2);
  float half_width = aspect_ratio * half_height;

  // w, v, u eksenleri
  cam.w = glm::normalize(pos - target);
  cam.u = glm::normalize(cross(up, cam.w));
  cam.v = cross(cam.w, cam.u);

  cam.lower_left_corner = cam.origin - half_width * focus_dist * cam.u -
                          half_height * focus_dist * cam.v - cam.w * focus_dist;
  cam.horizontal = 2 * half_width * focus_dist * cam.u;
  cam.vertical = 2 * half_height * focus_dist * cam.v;
  return cam;
}

struct NHittable {
  int hittable_type;
  vec4 sphere_center;
  float sphere_radius;
  int material_type; // 0: lambert, 1: metal, 2: dielectric;
  vec4 lambert_albedo;
  vec4 metal_albedo;
  float metal_roughness;
  float dielectric_ref_idx;
};
NHittable makeNHittable(int hittable_type, vec3 sphere_center,
                        float sphere_radius, int material_type,
                        vec3 lambert_albedo, vec3 metal_albedo,
                        float metal_fuzz, float dielectric_ref_idx) {
  NHittable nhit;
  nhit.hittable_type = hittable_type;
  nhit.sphere_center = vec4(sphere_center, 0);
  nhit.sphere_radius = sphere_radius;
  nhit.material_type = material_type;
  nhit.lambert_albedo = vec4(lambert_albedo, 0);
  nhit.metal_albedo = vec4(metal_albedo, 0);
  nhit.metal_roughness = metal_fuzz;
  nhit.dielectric_ref_idx = dielectric_ref_idx;
  return nhit;
}

struct NHittableVecs {
  std::vector<int> hittable_type_v;
  std::vector<vec3> sphere_center_v; // vec3
  std::vector<float> sphere_radius_v;
  std::vector<int> material_type_v;   // 0: lambert, 1: metal, 2: dielectric;
  std::vector<vec3> lambert_albedo_v; // vec3
  std::vector<vec3> metal_albedo_v;   // vec3
  std::vector<float> metal_roughness_v;
  std::vector<float> dielectric_ref_idx_v;
};

void addVecComponents(std::vector<float> &vs, vec4 v) {
  vs.push_back(v.x);
  vs.push_back(v.y);
  vs.push_back(v.z);
  vs.push_back(v.w);
}
void addVecComponents(std::vector<float> &vs, vec3 v) {
  vs.push_back(v.x);
  vs.push_back(v.y);
  vs.push_back(v.z);
}

NHittableVecs makeNHittableVecs(std::vector<NHittable> nhtvs) {
  //
  NHittableVecs vs;
  for (int i = 0; i < nhtvs.size(); i++) {
    vs.hittable_type_v.push_back(nhtvs[i].hittable_type);
    vs.material_type_v.push_back(nhtvs[i].material_type);
    vs.dielectric_ref_idx_v.push_back(nhtvs[i].dielectric_ref_idx);

    vs.metal_roughness_v.push_back(nhtvs[i].metal_roughness);
    vs.sphere_radius_v.push_back(nhtvs[i].sphere_radius);
    //
    // addVecComponents(vs.metal_albedo_v, nhtvs[i].metal_albedo);
    // addVecComponents(vs.sphere_center_v, nhtvs[i].sphere_center);
    // addVecComponents(vs.metal_albedo_v, nhtvs[i].metal_albedo);
  }
  return vs;
}
int sizeofNHittableVecs(NHittableVecs vs) {
  // get size of underlying arrays
  return sizeof(vs.hittable_type_v.data()) + sizeof(vs.sphere_center_v.data()) +
         sizeof(vs.sphere_radius_v.data()) + sizeof(vs.material_type_v.data()) +
         sizeof(vs.lambert_albedo_v.data()) +
         sizeof(vs.metal_roughness_v.data()) +
         sizeof(vs.metal_albedo_v.data()) +
         sizeof(vs.dielectric_ref_idx_v.data());
}
//
#endif
