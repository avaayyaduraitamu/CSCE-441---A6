//
//
//
//
//#include <iostream>
//#include <vector>
//#include <string>
//#include <memory>
//#include <cmath>
//#include <algorithm>
//
//#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"
//#include "Image.h"
//#include "Camera.h"
//
//using namespace std;
//
//// --- Constants ---
//const float EPS = 0.0001f;
//const int MAX_DEPTH = 4;
//
//// --- Minimal Vector Math Helper ---
//struct vec3 {
//    float x, y, z;
//    vec3(float v = 0) : x(v), y(v), z(v) {}
//    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
//    vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
//    vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
//    vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }
//    vec3 operator*(const vec3& v) const { return vec3(x * v.x, y * v.y, z * v.z); }
//};
//
//inline float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
//inline vec3 cross(vec3 a, vec3 b) { return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
//inline float length(vec3 v) { return sqrt(dot(v, v)); }
//inline vec3 normalize(vec3 v) {
//    float len = length(v);
//    if (len < 1e-8f) return vec3(0);
//    return v * (1.0f / len);
//}
//vec3 reflect(const vec3& I, const vec3& N) { return I - N * (2.0f * dot(I, N)); }
//
//struct Hit {
//    float t;
//    vec3 p, n, ka, kd, ks;
//    float exp;
//    float reflectivity = 0.0f;
//};
//
//class Shape {
//public:
//    virtual bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const = 0;
//};
//
//// --- Shapes Implementation (Sphere, Plane, Ellipsoid) ---
//class Sphere : public Shape {
//public:
//    vec3 center; float radius, exp; vec3 ka, kd, ks;
//    Sphere(vec3 c, float r, vec3 a, vec3 d, vec3 s, float e) : center(c), radius(r), ka(a), kd(d), ks(s), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz), oc = ro - center;
//        float a = dot(rd, rd), b = 2.0f * dot(oc, rd), c = dot(oc, oc) - radius * radius;
//        float disc = b * b - 4 * a * c;
//        if (disc < 0) return false;
//        float t = (-b - sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = ro + rd * t; hit.n = normalize(hit.p - center);
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp;
//        hit.reflectivity = (kd.x == 0 && kd.y == 0 && kd.z == 0) ? 1.0f : 0.0f;
//        return true;
//    }
//};
//
//class Plane : public Shape {
//public:
//    vec3 point, normal, ka, kd, ks; float exp;
//    Plane(vec3 p, vec3 n, vec3 a, vec3 d, vec3 s, float e) : point(p), normal(normalize(n)), ka(a), kd(d), ks(s), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz);
//        float denom = dot(rd, normal);
//        if (fabs(denom) < 1e-6) return false;
//        float t = dot(point - ro, normal) / denom;
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = ro + rd * t; hit.n = normal;
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.reflectivity = 0.0f;
//        return true;
//    }
//};
//
//class Ellipsoid : public Shape {
//public:
//    vec3 center, scale, ka, kd, ks; float exp;
//    Ellipsoid(vec3 c, vec3 s, vec3 a, vec3 d, vec3 sp, float e) : center(c), scale(s), ka(a), kd(d), ks(sp), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro_L = (vec3(ray.ox, ray.oy, ray.oz) - center);
//        ro_L.x /= scale.x; ro_L.y /= scale.y; ro_L.z /= scale.z;
//        vec3 rd_L(ray.dx / scale.x, ray.dy / scale.y, ray.dz / scale.z);
//        float a = dot(rd_L, rd_L), b = 2.0f * dot(ro_L, rd_L), c = dot(ro_L, ro_L) - 1.0f;
//        float disc = b * b - 4 * a * c;
//        if (disc < 0) return false;
//        float t = (-b - sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = vec3(ray.ox, ray.oy, ray.oz) + vec3(ray.dx, ray.dy, ray.dz) * t;
//        vec3 p_L = ro_L + rd_L * t;
//        hit.n = normalize(vec3(p_L.x / scale.x, p_L.y / scale.y, p_L.z / scale.z));
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.reflectivity = 0.0f;
//        return true;
//    }
//};
//
//
//class Mesh : public Shape {
//public:
//    vector<float> verts;
//    vector<float> normals; // New: Store normals for interpolation
//    vec3 ka{ 0.1f }, kd{ 0.0f, 0.0f, 1.0f }, ks{ 1.0f, 1.0f, 0.5f };
//    float exp = 100.0f;
//    float E[16], Ei[16], EiT[16];
//    vec3 sphereCenter;
//    float sphereRadius;
//
//    Mesh(string file, float* _E, float* _Ei, float* _EiT) {
//        memcpy(E, _E, 16 * sizeof(float));
//        memcpy(Ei, _Ei, 16 * sizeof(float));
//        memcpy(EiT, _EiT, 16 * sizeof(float));
//
//        tinyobj::attrib_t a; vector<tinyobj::shape_t> s; vector<tinyobj::material_t> m; string w, e;
//        if (!tinyobj::LoadObj(&a, &s, &m, &w, &e, file.c_str())) cout << "OBJ LOAD FAILED\n";
//
//        vec3 minB(1e10f), maxB(-1e10f);
//        for (auto& sh : s) {
//            for (auto& idx : sh.mesh.indices) {
//                // Positions
//                float vx = a.vertices[3 * idx.vertex_index + 0];
//                float vy = a.vertices[3 * idx.vertex_index + 1];
//                float vz = a.vertices[3 * idx.vertex_index + 2];
//                verts.push_back(vx); verts.push_back(vy); verts.push_back(vz);
//
//                // Normals: Load vertex normals if they exist
//                if (!a.normals.empty()) {
//                    normals.push_back(a.normals[3 * idx.normal_index + 0]);
//                    normals.push_back(a.normals[3 * idx.normal_index + 1]);
//                    normals.push_back(a.normals[3 * idx.normal_index + 2]);
//                }
//
//                minB.x = min(minB.x, vx); minB.y = min(minB.y, vy); minB.z = min(minB.z, vz);
//                maxB.x = max(maxB.x, vx); maxB.y = max(maxB.y, vy); maxB.z = max(maxB.z, vz);
//            }
//        }
//        sphereCenter = (minB + maxB) * 0.5f;
//        sphereRadius = length(maxB - sphereCenter);
//    }
//
//    vec3 mult(const float* M, vec3 v, bool isPt) const {
//        float x = M[0] * v.x + M[4] * v.y + M[8] * v.z + (isPt ? M[12] : 0);
//        float y = M[1] * v.x + M[5] * v.y + M[9] * v.z + (isPt ? M[13] : 0);
//        float z = M[2] * v.x + M[6] * v.y + M[10] * v.z + (isPt ? M[14] : 0);
//        return vec3(x, y, z);
//    }
//
//    bool intersect(const Ray& worldRay, float tMin, float tMax, Hit& h) const override {
//        vec3 ro_world(worldRay.ox, worldRay.oy, worldRay.oz), rd_world(worldRay.dx, worldRay.dy, worldRay.dz);
//        vec3 ro_L = mult(Ei, ro_world, true);
//        vec3 rd_L = normalize(mult(Ei, rd_world, false));
//
//        // Bounding Sphere optimization
//        vec3 oc = ro_L - sphereCenter;
//        float b = dot(oc, rd_L), c = dot(oc, oc) - sphereRadius * sphereRadius;
//        if (c > 0.0f && b > 0.0f) return false;
//        if (b * b - c < 0.0f) return false;
//
//        bool hit = false;
//        float best_t_local = 1e20f;
//        vec3 best_n_local;
//
//        for (size_t i = 0; i < verts.size(); i += 9) {
//            vec3 v0(verts[i], verts[i + 1], verts[i + 2]);
//            vec3 v1(verts[i + 3], verts[i + 4], verts[i + 5]);
//            vec3 v2(verts[i + 6], verts[i + 7], verts[i + 8]);
//
//            vec3 e1 = v1 - v0, e2 = v2 - v0, pv = cross(rd_L, e2);
//            float det = dot(e1, pv);
//            if (fabs(det) < 1e-8) continue;
//
//            float invDet = 1.0f / det;
//            vec3 tv = ro_L - v0;
//            float u = dot(tv, pv) * invDet;
//            if (u < 0 || u > 1) continue;
//
//            vec3 qv = cross(tv, e1);
//            float v = dot(rd_L, qv) * invDet;
//            if (v < 0 || u + v > 1) continue;
//
//            float tHit = dot(e2, qv) * invDet;
//            if (tHit > 0.001f && tHit < best_t_local) {
//                best_t_local = tHit;
//
//                // INTERPOLATED NORMAL CALCULATION
//                if (!normals.empty()) {
//                    float w = 1.0f - u - v; // Barycentric coordinate for v0
//                    vec3 n0(normals[i], normals[i + 1], normals[i + 2]);
//                    vec3 n1(normals[i + 3], normals[i + 4], normals[i + 5]);
//                    vec3 n2(normals[i + 6], normals[i + 7], normals[i + 8]);
//
//                    // Blend the three normals based on the hit point position
//                    best_n_local = normalize(n0 * w + n1 * u + n2 * v);
//                }
//                else {
//                    best_n_local = normalize(cross(e1, e2)); // Fallback to flat shading
//                }
//
//                hit = true;
//            }
//        }
//
//        if (hit) {
//            vec3 worldP = mult(E, ro_L + rd_L * best_t_local, true);
//            float worldT = length(worldP - ro_world);
//            if (worldT < tMax && worldT > tMin) {
//                h.t = worldT;
//                h.p = worldP;
//                // Transform the local normal back to world space using Inverse-Transpose
//                h.n = normalize(mult(EiT, best_n_local, false));
//                h.ka = ka; h.kd = kd; h.ks = ks; h.exp = exp;
//                return true;
//            }
//        }
//        return false;
//    }
//};
//struct Light { vec3 pos; float intensity; };
//
//// --- TRACE FUNCTION ---
//vec3 trace(const Ray& ray, const vector<shared_ptr<Shape>>& scene, const vector<Light>& lights, int depth, int maxDepth, const vec3& eye, int scnNum) {
//    if (depth > maxDepth) return vec3(0);
//    Hit bestHit; bestHit.t = 1e20f; bool found = false;
//    for (auto& obj : scene) {
//        Hit h; if (obj->intersect(ray, EPS, bestHit.t, h)) { bestHit = h; found = true; }
//    }
//    if (!found) return vec3(0.0f);
//
//    vec3 color = bestHit.ka;
//    for (const auto& light : lights) {
//        vec3 L_vec = light.pos - bestHit.p;
//        float distToLight = length(L_vec);
//        vec3 L = normalize(L_vec);
//        bool inShadow = false;
//
//        if (scnNum >= 2) {
//            Ray sRay;
//            sRay.ox = bestHit.p.x + bestHit.n.x * EPS;
//            sRay.oy = bestHit.p.y + bestHit.n.y * EPS;
//            sRay.oz = bestHit.p.z + bestHit.n.z * EPS;
//            sRay.dx = L.x; sRay.dy = L.y; sRay.dz = L.z;
//            Hit sHit;
//            for (auto& obj : scene) {
//                if (obj->intersect(sRay, EPS, distToLight - EPS, sHit)) {
//                    inShadow = true; break;
//                }
//            }
//        }
//
//        if (!inShadow) {
//            vec3 V = normalize(eye - bestHit.p), H = normalize(L + V);
//            vec3 diff = (bestHit.kd * light.intensity) * max(0.0f, dot(bestHit.n, L));
//            float ndh = max(0.0f, dot(bestHit.n, H));
//            vec3 spec = (bestHit.ks * light.intensity) * pow(ndh, bestHit.exp);
//            color = color + diff + spec;
//        }
//    }
//
//    if (bestHit.reflectivity > 0.0f) {
//        vec3 I(ray.dx, ray.dy, ray.dz); vec3 R = reflect(normalize(I), bestHit.n);
//        Ray reflRay;
//        reflRay.ox = bestHit.p.x + R.x * EPS; reflRay.oy = bestHit.p.y + R.y * EPS; reflRay.oz = bestHit.p.z + R.z * EPS;
//        reflRay.dx = R.x; reflRay.dy = R.y; reflRay.dz = R.z;
//        color = color + trace(reflRay, scene, lights, depth + 1, maxDepth, eye, scnNum) * bestHit.reflectivity;
//    }
//    return color;
//}
//
//
//int main(int argc, char** argv) {
//    if (argc < 4) return 0;
//    int sceneNum = stoi(argv[1]), width = stoi(argv[2]); string outFilename(argv[3]);
//
//    // --- 1. DECLARE CAMERA AND SCENE GLOBALS FIRST ---
//    Camera cam;
//    vector<shared_ptr<Shape>> scene;
//    vector<Light> lights;
//    vec3 eye(0, 0, 5); // Default eye position
//
//    float E[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//    float Ei[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//    float EiT[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//
//    // --- 2. DEFINE SCENE GEOMETRY ---
//    // Scene 1, 2, and 8 use the same spheres
//    if (sceneNum <= 2 || sceneNum == 8) {
//        lights.push_back({ vec3(-2,1,1), 1.0f });
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, -1, 1), 1, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(0.5, -1, -1), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(0, 1, 0), 1, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
//    }
//    else if (sceneNum == 3) {
//        lights.push_back({ vec3(1,2,2), 0.5f }); lights.push_back({ vec3(-1,2,-1), 0.5f });
//        scene.push_back(make_shared<Ellipsoid>(vec3(0.5, 0, 0.5), vec3(0.5, 0.6, 0.2), vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, 0, -0.5), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1, 1, 1), vec3(0), 0));
//    }
//    else if (sceneNum <= 5) {
//        lights.push_back({ vec3(-1,2,1), 0.5f }); lights.push_back({ vec3(0.5,-0.5,0), 0.5f });
//        scene.push_back(make_shared<Sphere>(vec3(0.5, -0.7, 0.5), 0.3, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(1.0, -0.7, 0.0), 0.3, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1, 1, 1), vec3(0), 0));
//        scene.push_back(make_shared<Plane>(vec3(0, 0, -3), vec3(0, 0, 1), vec3(0.1), vec3(1, 1, 1), vec3(0), 0));
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, 0, -0.5), 1, vec3(0), vec3(0), vec3(0), 0));
//        scene.push_back(make_shared<Sphere>(vec3(1.5, 0, -1.5), 1, vec3(0), vec3(0), vec3(0), 0));
//    }
//    else if (sceneNum <= 7) {
//        if (sceneNum == 7) {
//            float E7[16] = { 1.5f, 0, 0, 0, 0, 1.4095f, 0.5130f, 0, 0, -0.5130f, 1.4095f, 0, 0.3f, -1.5f, 0.0f, 1.0f };
//            float Ei7[16] = { 0.6667f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6264f, -0.2280f, 0.0f, 0.0f, 0.2280f, 0.6264f, 0.0f, -0.2000f, 0.9397f, -0.1170f, 1.0f };
//            float EiT7[16] = { 0.6667f, 0.0f, 0.0f, -0.2000f, 0.0f, 0.6264f, 0.2280f, 0.9397f, 0.0f, -0.2280f, 0.6264f, -0.1170f, 0.0f, 0.0f, 0.0f, 1.0f };
//            lights.push_back({ vec3(1.0, 1.0, 2.0), 1.0f });
//            scene.push_back(make_shared<Mesh>(argv[4], E7, Ei7, EiT7));
//        }
//        else {
//            lights.push_back({ vec3(-1.0, 1.0, 1.0), 1.0f });
//            scene.push_back(make_shared<Mesh>(argv[4], E, Ei, EiT));
//        }
//    }
//
//    // --- 3. CONFIGURE THE CAMERA ---
//    if (sceneNum == 8) {
//        vec3_cam eyePos(-3.0f, 0.0f, 0.0f);
//        vec3_cam lookAtPoint(1.0f, 0.0f, 0.0f);
//        vec3_cam upVector(0.0f, 1.0f, 0.0f);
//        float fov = 60.0f;
//        cam.setup(width, width, eyePos, lookAtPoint, upVector, fov);
//
//        // Update world-space eye for the trace function
//        eye = vec3(-3.0f, 0.0f, 0.0f);
//    }
//    else {
//        // Default camera for all other scenes
//        cam.setup(width, width);
//    }
//
//    // --- 4. RENDER ---
//    auto image = make_shared<Image>(width, width);
//    int maxDepth = (sceneNum == 4) ? 1 : (sceneNum == 5 ? MAX_DEPTH : 0);
//
//    for (int j = 0; j < width; ++j) {
//        for (int i = 0; i < width; ++i) {
//            Ray ray = cam.getRay(i, (width - 1) - j);
//            vec3 color = trace(ray, scene, lights, 0, maxDepth, eye, sceneNum);
//            image->setPixel(i, j, clamp((int)(color.x * 255), 0, 255), clamp((int)(color.y * 255), 0, 255), clamp((int)(color.z * 255), 0, 255));
//        }
//    }
//    image->writeToFile(outFilename);
//    return 0;
//}



//
//
//#include <iostream>
//#include <vector>
//#include <string>
//#include <memory>
//#include <cmath>
//#include <algorithm>
//#include <random>
//
//#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"
//#include "Image.h"
//#include "Camera.h"
//
//using namespace std;
//
//// --- Constants ---
//const float EPS = 0.0001f;
//
//// --- Custom Clamp Helper (Fixes "no instance of clamp" error) ---
//inline float my_clamp(float n, float lower, float upper) {
//    return max(lower, min(n, upper));
//}
//
//// --- Vector Math Structure ---
//struct vec3 {
//    float x, y, z;
//    vec3(float v = 0) : x(v), y(v), z(v) {}
//    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
//    vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
//    vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
//    vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }
//    vec3 operator*(const vec3& v) const { return vec3(x * v.x, y * v.y, z * v.z); }
//};
//
//inline float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
//inline vec3 cross(vec3 a, vec3 b) { return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
//inline float length(vec3 v) { return sqrt(dot(v, v)); }
//inline vec3 normalize(vec3 v) {
//    float len = length(v);
//    return (len < 1e-8f) ? vec3(0) : v * (1.0f / len);
//}
//inline vec3 reflect(const vec3& I, const vec3& N) { return I - N * (2.0f * dot(I, N)); }
//
//// --- Randomness for Scene 9 ---
//static std::mt19937 generator;
//static std::uniform_real_distribution<float> distribution(0.0, 1.0);
//inline float get_random() { return distribution(generator); }
//
//vec3 random_in_hemisphere(vec3 normal) {
//    float u = get_random();
//    float v = get_random();
//    float theta = 2.0f * 3.14159f * u;
//    float phi = acos(2.0f * v - 1.0f);
//    vec3 randDir(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
//    if (dot(randDir, normal) < 0) return randDir * -1.0f;
//    return randDir;
//}
//
//// --- Hit and Light structures ---
//struct Hit {
//    float t;
//    vec3 p, n, ka, kd, ks;
//    float exp;
//    float reflectivity = 0.0f;
//};
//
//struct Light { vec3 pos; float intensity; };
//
//class Shape {
//public:
//    virtual bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const = 0;
//};
//
//// --- Shape Implementations ---
//class Sphere : public Shape {
//public:
//    vec3 center; float radius, exp; vec3 ka, kd, ks;
//    Sphere(vec3 c, float r, vec3 a, vec3 d, vec3 s, float e) : center(c), radius(r), ka(a), kd(d), ks(s), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz), oc = ro - center;
//        float a = dot(rd, rd), b = 2.0f * dot(oc, rd), c = dot(oc, oc) - radius * radius;
//        float disc = b * b - 4 * a * c;
//        if (disc < 0) return false;
//        float t = (-b - sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = ro + rd * t; hit.n = normalize(hit.p - center);
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp;
//        hit.reflectivity = (kd.x == 0 && kd.y == 0 && kd.z == 0) ? 1.0f : 0.0f;
//        return true;
//    }
//};
//
//class Plane : public Shape {
//public:
//    vec3 point, normal, ka, kd, ks; float exp;
//    Plane(vec3 p, vec3 n, vec3 a, vec3 d, vec3 s, float e) : point(p), normal(normalize(n)), ka(a), kd(d), ks(s), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz);
//        float denom = dot(rd, normal);
//        if (fabs(denom) < 1e-6) return false;
//        float t = dot(point - ro, normal) / denom;
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = ro + rd * t; hit.n = normal;
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.reflectivity = 0.0f;
//        return true;
//    }
//};
//
//class Ellipsoid : public Shape {
//public:
//    vec3 center, scale, ka, kd, ks; float exp;
//    Ellipsoid(vec3 c, vec3 s, vec3 a, vec3 d, vec3 sp, float e) : center(c), scale(s), ka(a), kd(d), ks(sp), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro_L = (vec3(ray.ox, ray.oy, ray.oz) - center);
//        ro_L.x /= scale.x; ro_L.y /= scale.y; ro_L.z /= scale.z;
//        vec3 rd_L(ray.dx / scale.x, ray.dy / scale.y, ray.dz / scale.z);
//        float a = dot(rd_L, rd_L), b = 2.0f * dot(ro_L, rd_L), c = dot(ro_L, ro_L) - 1.0f;
//        float disc = b * b - 4 * a * c;
//        if (disc < 0) return false;
//        float t = (-b - sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = vec3(ray.ox, ray.oy, ray.oz) + vec3(ray.dx, ray.dy, ray.dz) * t;
//        vec3 p_L = ro_L + rd_L * t;
//        hit.n = normalize(vec3(p_L.x / scale.x, p_L.y / scale.y, p_L.z / scale.z));
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.reflectivity = 0.0f;
//        return true;
//    }
//};
//
//class Mesh : public Shape {
//public:
//    vector<float> verts; vector<float> normals;
//    vec3 ka{ 0.1f }, kd{ 0.0f, 0.0f, 1.0f }, ks{ 1.0f, 1.0f, 0.5f }; float exp = 100.0f;
//    float E[16], Ei[16], EiT[16]; vec3 sphereCenter; float sphereRadius;
//
//    Mesh(string file, float* _E, float* _Ei, float* _EiT) {
//        memcpy(E, _E, 16 * sizeof(float)); memcpy(Ei, _Ei, 16 * sizeof(float)); memcpy(EiT, _EiT, 16 * sizeof(float));
//        tinyobj::attrib_t a; vector<tinyobj::shape_t> s; vector<tinyobj::material_t> m; string w, e;
//        if (!tinyobj::LoadObj(&a, &s, &m, &w, &e, file.c_str())) cout << "OBJ LOAD FAILED\n";
//        vec3 minB(1e10f), maxB(-1e10f);
//        for (auto& sh : s) {
//            for (auto& idx : sh.mesh.indices) {
//                float vx = a.vertices[3 * idx.vertex_index + 0], vy = a.vertices[3 * idx.vertex_index + 1], vz = a.vertices[3 * idx.vertex_index + 2];
//                verts.push_back(vx); verts.push_back(vy); verts.push_back(vz);
//                if (!a.normals.empty()) {
//                    normals.push_back(a.normals[3 * idx.normal_index + 0]);
//                    normals.push_back(a.normals[3 * idx.normal_index + 1]);
//                    normals.push_back(a.normals[3 * idx.normal_index + 2]);
//                }
//                minB.x = min(minB.x, vx); minB.y = min(minB.y, vy); minB.z = min(minB.z, vz);
//                maxB.x = max(maxB.x, vx); maxB.y = max(maxB.y, vy); maxB.z = max(maxB.z, vz);
//            }
//        }
//        sphereCenter = (minB + maxB) * 0.5f; sphereRadius = length(maxB - sphereCenter);
//    }
//
//    vec3 mult(const float* M, vec3 v, bool isPt) const {
//        float x = M[0] * v.x + M[4] * v.y + M[8] * v.z + (isPt ? M[12] : 0);
//        float y = M[1] * v.x + M[5] * v.y + M[9] * v.z + (isPt ? M[13] : 0);
//        float z = M[2] * v.x + M[6] * v.y + M[10] * v.z + (isPt ? M[14] : 0);
//        return vec3(x, y, z);
//    }
//
//    bool intersect(const Ray& worldRay, float tMin, float tMax, Hit& h) const override {
//        vec3 ro_world(worldRay.ox, worldRay.oy, worldRay.oz), rd_world(worldRay.dx, worldRay.dy, worldRay.dz);
//        vec3 ro_L = mult(Ei, ro_world, true); vec3 rd_L = normalize(mult(Ei, rd_world, false));
//        vec3 oc = ro_L - sphereCenter;
//        float b = dot(oc, rd_L), c = dot(oc, oc) - sphereRadius * sphereRadius;
//        if (c > 0.0f && b > 0.0f) return false;
//        if (b * b - c < 0.0f) return false;
//        bool hit = false; float best_t_local = 1e20f; vec3 best_n_local;
//        for (size_t i = 0; i < verts.size(); i += 9) {
//            vec3 v0(verts[i], verts[i + 1], verts[i + 2]), v1(verts[i + 3], verts[i + 4], verts[i + 5]), v2(verts[i + 6], verts[i + 7], verts[i + 8]);
//            vec3 e1 = v1 - v0, e2 = v2 - v0, pv = cross(rd_L, e2);
//            float det = dot(e1, pv); if (fabs(det) < 1e-8) continue;
//            float invDet = 1.0f / det; vec3 tv = ro_L - v0;
//            float u = dot(tv, pv) * invDet; if (u < 0 || u > 1) continue;
//            vec3 qv = cross(tv, e1); float v = dot(rd_L, qv) * invDet; if (v < 0 || u + v > 1) continue;
//            float tHit = dot(e2, qv) * invDet;
//            if (tHit > 0.001f && tHit < best_t_local) {
//                best_t_local = tHit;
//                if (!normals.empty()) {
//                    float w = 1.0f - u - v;
//                    vec3 n0(normals[i], normals[i + 1], normals[i + 2]), n1(normals[i + 3], normals[i + 4], normals[i + 5]), n2(normals[i + 6], normals[i + 7], normals[i + 8]);
//                    best_n_local = normalize(n0 * w + n1 * u + n2 * v);
//                }
//                else { best_n_local = normalize(cross(e1, e2)); }
//                hit = true;
//            }
//        }
//        if (hit) {
//            vec3 worldP = mult(E, ro_L + rd_L * best_t_local, true);
//            float worldT = length(worldP - ro_world);
//            if (worldT < tMax && worldT > tMin) {
//                h.t = worldT; h.p = worldP; h.n = normalize(mult(EiT, best_n_local, false));
//                h.ka = ka; h.kd = kd; h.ks = ks; h.exp = exp; return true;
//            }
//        }
//        return false;
//    }
//};
//
//// --- Main Trace Function ---
//vec3 trace(const Ray& ray, const vector<shared_ptr<Shape>>& scene, const vector<Light>& lights, int depth, int maxDepth, const vec3& eye, int scnNum) {
//    if (depth > maxDepth) return vec3(0);
//    Hit bestHit; bestHit.t = 1e20f; bool found = false;
//    for (auto& obj : scene) {
//        Hit h; if (obj->intersect(ray, EPS, bestHit.t, h)) { bestHit = h; found = true; }
//    }
//    if (!found) return vec3(0.0f);
//
//    if (scnNum == 9) {
//        if (length(bestHit.ka) > 5.0f) return bestHit.ka;
//        if (bestHit.reflectivity > 0.0f) {
//            vec3 R = reflect(normalize(vec3(ray.dx, ray.dy, ray.dz)), bestHit.n);
//            Ray reflRay; reflRay.ox = bestHit.p.x + R.x * EPS; reflRay.oy = bestHit.p.y + R.y * EPS; reflRay.oz = bestHit.p.z + R.z * EPS;
//            reflRay.dx = R.x; reflRay.dy = R.y; reflRay.dz = R.z;
//            return trace(reflRay, scene, lights, depth + 1, maxDepth, eye, scnNum);
//        }
//        vec3 randDir = random_in_hemisphere(bestHit.n);
//        Ray bounceRay; bounceRay.ox = bestHit.p.x + randDir.x * EPS; bounceRay.oy = bestHit.p.y + randDir.y * EPS; bounceRay.oz = bestHit.p.z + randDir.z * EPS;
//        bounceRay.dx = randDir.x; bounceRay.dy = randDir.y; bounceRay.dz = randDir.z;
//        return trace(bounceRay, scene, lights, depth + 1, maxDepth, eye, scnNum) * bestHit.kd;
//    }
//
//    vec3 color = bestHit.ka;
//    for (const auto& light : lights) {
//        vec3 L_vec = light.pos - bestHit.p; float distToLight = length(L_vec); vec3 L = normalize(L_vec);
//        bool inShadow = false;
//        if (scnNum >= 2) {
//            Ray sRay; sRay.ox = bestHit.p.x + bestHit.n.x * EPS; sRay.oy = bestHit.p.y + bestHit.n.y * EPS; sRay.oz = bestHit.p.z + bestHit.n.z * EPS;
//            sRay.dx = L.x; sRay.dy = L.y; sRay.dz = L.z;
//            Hit sHit;
//            for (auto& obj : scene) { if (obj->intersect(sRay, EPS, distToLight - EPS, sHit)) { inShadow = true; break; } }
//        }
//        if (!inShadow) {
//            vec3 V = normalize(eye - bestHit.p), H = normalize(L + V);
//            vec3 diff = (bestHit.kd * light.intensity) * max(0.0f, dot(bestHit.n, L));
//            float ndh = max(0.0f, dot(bestHit.n, H));
//            vec3 spec = (bestHit.ks * light.intensity) * pow(ndh, bestHit.exp);
//            color = color + diff + spec;
//        }
//    }
//    if (bestHit.reflectivity > 0.0f) {
//        vec3 R = reflect(normalize(vec3(ray.dx, ray.dy, ray.dz)), bestHit.n);
//        Ray reflRay; reflRay.ox = bestHit.p.x + R.x * EPS; reflRay.oy = bestHit.p.y + R.y * EPS; reflRay.oz = bestHit.p.z + R.z * EPS;
//        reflRay.dx = R.x; reflRay.dy = R.y; reflRay.dz = R.z;
//        color = color + trace(reflRay, scene, lights, depth + 1, maxDepth, eye, scnNum) * bestHit.reflectivity;
//    }
//    return color;
//}
//
//int main(int argc, char** argv) {
//    if (argc < 4) return 0;
//    int sceneNum = stoi(argv[1]), width = stoi(argv[2]); string outFilename(argv[3]);
//    Camera cam; vector<shared_ptr<Shape>> scene; vector<Light> lights; vec3 eye(0, 0, 5);
//    float E[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }, Ei[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 }, EiT[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
//
//    if (sceneNum <= 2 || sceneNum == 8) {
//        lights.push_back({ vec3(-2,1,1), 1.0f });
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, -1, 1), 1, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(0.5, -1, -1), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(0, 1, 0), 1, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
//    }
//    else if (sceneNum == 3) {
//        lights.push_back({ vec3(1,2,2), 0.5f }); lights.push_back({ vec3(-1,2,-1), 0.5f });
//        scene.push_back(make_shared<Ellipsoid>(vec3(0.5, 0, 0.5), vec3(0.5, 0.6, 0.2), vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, 0, -0.5), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1, 1, 1), vec3(0), 0));
//    }
//    else if (sceneNum <= 5) {
//        lights.push_back({ vec3(-1,2,1), 0.5f }); lights.push_back({ vec3(0.5,-0.5,0), 0.5f });
//        scene.push_back(make_shared<Sphere>(vec3(0.5, -0.7, 0.5), 0.3, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(1.0, -0.7, 0.0), 0.3, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1, 1, 1), vec3(0), 0));
//        scene.push_back(make_shared<Plane>(vec3(0, 0, -3), vec3(0, 0, 1), vec3(0.1), vec3(1, 1, 1), vec3(0), 0));
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, 0, -0.5), 1, vec3(0), vec3(0), vec3(0), 0));
//        scene.push_back(make_shared<Sphere>(vec3(1.5, 0, -1.5), 1, vec3(0), vec3(0), vec3(0), 0));
//    }
//    else if (sceneNum <= 7) {
//        if (sceneNum == 7) {
//            float E7[16] = { 1.5f, 0, 0, 0, 0, 1.4095f, 0.5130f, 0, 0, -0.5130f, 1.4095f, 0, 0.3f, -1.5f, 0.0f, 1.0f };
//            float Ei7[16] = { 0.6667f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6264f, -0.2280f, 0.0f, 0.0f, 0.2280f, 0.6264f, 0.0f, -0.2000f, 0.9397f, -0.1170f, 1.0f };
//            float EiT7[16] = { 0.6667f, 0.0f, 0.0f, -0.2000f, 0.0f, 0.6264f, 0.2280f, 0.9397f, 0.0f, -0.2280f, 0.6264f, -0.1170f, 0.0f, 0.0f, 0.0f, 1.0f };
//            lights.push_back({ vec3(1.0, 1.0, 2.0), 1.0f });
//            scene.push_back(make_shared<Mesh>(argv[4], E7, Ei7, EiT7));
//        }
//        else {
//            lights.push_back({ vec3(-1.0, 1.0, 1.0), 1.0f });
//            scene.push_back(make_shared<Mesh>(argv[4], E, Ei, EiT));
//        }
//    }
//    else if (sceneNum == 9) {
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0), vec3(1), vec3(0), 0)); // Floor
//        scene.push_back(make_shared<Plane>(vec3(0, 1, 0), vec3(0, -1, 0), vec3(0), vec3(1), vec3(0), 0));  // Ceiling
//        scene.push_back(make_shared<Plane>(vec3(-1, 0, 0), vec3(1, 0, 0), vec3(0), vec3(1, 0, 0), vec3(0), 0)); // Left Red
//        scene.push_back(make_shared<Plane>(vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0), vec3(0, 0, 1), vec3(0), 0)); // Right Blue
//        scene.push_back(make_shared<Plane>(vec3(0, 0, -1), vec3(0, 0, 1), vec3(0), vec3(1), vec3(0), 0));  // Back
//        scene.push_back(make_shared<Sphere>(vec3(0, 0.9, -0.5), 0.15, vec3(20), vec3(0), vec3(0), 0)); // Light
//        scene.push_back(make_shared<Sphere>(vec3(-0.4, -0.7, -0.5), 0.3, vec3(0), vec3(0), vec3(1), 0)); // Mirror
//        scene.push_back(make_shared<Sphere>(vec3(0.4, -0.7, -0.8), 0.3, vec3(0), vec3(1), vec3(0), 0));  // Diffuse
//    }
//
//    if (sceneNum == 8) { cam.setup(width, width, vec3_cam(-3, 0, 0), vec3_cam(1, 0, 0), vec3_cam(0, 1, 0), 60.0f); eye = vec3(-3, 0, 0); }
//    else if (sceneNum == 9) { cam.setup(width, width, vec3_cam(0, 0, 2.5), vec3_cam(0, 0, -1), vec3_cam(0, 1, 0), 45.0f); eye = vec3(0, 0, 2.5); }
//    else { cam.setup(width, width); }
//
//    auto image = make_shared<Image>(width, width);
//    int maxD = (sceneNum >= 5) ? 5 : ((sceneNum == 4) ? 1 : 0);
//    int spp = (sceneNum == 9) ? 256 : 1;
//
//    for (int j = 0; j < width; ++j) {
//        for (int i = 0; i < width; ++i) {
//            vec3 col(0);
//            for (int s = 0; s < spp; ++s) col = col + trace(cam.getRay(i, width - 1- j), scene, lights, 0, maxD, eye, sceneNum);
//            col = col * (1.0f / (float)spp);
//            if (sceneNum == 9) {
//                col.x = pow(col.x, 1 / 2.2f); col.y = pow(col.y, 1 / 2.2f); col.z = pow(col.z, 1 / 2.2f);
//            }
//            image->setPixel(i, j, (int)my_clamp(col.x * 255, 0, 255), (int)my_clamp(col.y * 255, 0, 255), (int)my_clamp(col.z * 255, 0, 255));
//        }
//    }
//    image->writeToFile(outFilename);
//    return 0;
//}


//#include <iostream>
//#include <vector>
//#include <string>
//#include <memory>
//#include <cmath>
//#include <algorithm>
//#include <random>
//
//#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"
//#include "Image.h"
//#include "Camera.h"
//
//using namespace std;
//
//const float EPS = 0.0001f;
//
//inline float my_clamp(float n, float lower, float upper) {
//    return max(lower, min(n, upper));
//}
//
//struct vec3 {
//    float x, y, z;
//    vec3(float v = 0) : x(v), y(v), z(v) {}
//    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
//    vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
//    vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
//    vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }
//    vec3 operator*(const vec3& v) const { return vec3(x * v.x, y * v.y, z * v.z); }
//};
//
//inline float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
//inline vec3 cross(vec3 a, vec3 b) { return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
//inline float length(vec3 v) { return sqrt(dot(v, v)); }
//inline vec3 normalize(vec3 v) {
//    float len = length(v);
//    return (len < 1e-8f) ? vec3(0) : v * (1.0f / len);
//}
//inline vec3 reflect(const vec3& I, const vec3& N) { return I - N * (2.0f * dot(I, N)); }
//
//// --- Faster Randomness ---
//inline float get_random() {
//    static thread_local std::mt19937 generator(std::random_device{}());
//    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
//    return distribution(generator);
//}
//
//vec3 random_in_hemisphere(vec3 normal) {
//    float u = get_random();
//    float v = get_random();
//    float theta = 2.0f * 3.1415926f * u;
//    float phi = acos(2.0f * v - 1.0f);
//    vec3 randDir(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
//    return (dot(randDir, normal) < 0) ? randDir * -1.0f : randDir;
//}
//
//struct Hit {
//    float t;
//    vec3 p, n, ka, kd, ks;
//    float exp;
//    float reflectivity = 0.0f;
//};
//
//struct Light { vec3 pos; float intensity; };
//
//class Shape {
//public:
//    virtual bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const = 0;
//};
//
//class Sphere : public Shape {
//public:
//    vec3 center; float radius, exp; vec3 ka, kd, ks;
//    Sphere(vec3 c, float r, vec3 a, vec3 d, vec3 s, float e) : center(c), radius(r), ka(a), kd(d), ks(s), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz), oc = ro - center;
//        float a = dot(rd, rd), b = 2.0f * dot(oc, rd), c = dot(oc, oc) - radius * radius;
//        float disc = b * b - 4 * a * c;
//        if (disc < 0) return false;
//        float t = (-b - sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = ro + rd * t; hit.n = (hit.p - center) * (1.0f / radius);
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp;
//        hit.reflectivity = (kd.x == 0 && kd.y == 0 && kd.z == 0) ? 1.0f : 0.0f;
//        return true;
//    }
//};
//
//class Plane : public Shape {
//public:
//    vec3 point, normal, ka, kd, ks; float exp;
//    Plane(vec3 p, vec3 n, vec3 a, vec3 d, vec3 s, float e) : point(p), normal(normalize(n)), ka(a), kd(d), ks(s), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz);
//        float denom = dot(rd, normal);
//        if (fabs(denom) < 1e-6) return false;
//        float t = dot(point - ro, normal) / denom;
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = ro + rd * t; hit.n = normal;
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.reflectivity = 0.0f;
//        return true;
//    }
//};
//
//class Ellipsoid : public Shape {
//public:
//    vec3 center, scale, ka, kd, ks; float exp;
//    Ellipsoid(vec3 c, vec3 s, vec3 a, vec3 d, vec3 sp, float e) : center(c), scale(s), ka(a), kd(d), ks(sp), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro_L = (vec3(ray.ox, ray.oy, ray.oz) - center);
//        ro_L.x /= scale.x; ro_L.y /= scale.y; ro_L.z /= scale.z;
//        vec3 rd_L(ray.dx / scale.x, ray.dy / scale.y, ray.dz / scale.z);
//        float a = dot(rd_L, rd_L), b = 2.0f * dot(ro_L, rd_L), c = dot(ro_L, ro_L) - 1.0f;
//        float disc = b * b - 4 * a * c;
//        if (disc < 0) return false;
//        float t = (-b - sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = vec3(ray.ox, ray.oy, ray.oz) + vec3(ray.dx, ray.dy, ray.dz) * t;
//        vec3 p_L = ro_L + rd_L * t;
//        hit.n = normalize(vec3(p_L.x / scale.x, p_L.y / scale.y, p_L.z / scale.z));
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.reflectivity = 0.0f;
//        return true;
//    }
//};
//
//class Mesh : public Shape {
//public:
//    vector<float> verts; vector<float> normals;
//    vec3 ka{ 0.1f }, kd{ 0.0f, 0.0f, 1.0f }, ks{ 1.0f, 1.0f, 0.5f }; float exp = 100.0f;
//    float E[16], Ei[16], EiT[16]; vec3 sphereCenter; float sphereRadius;
//    Mesh(string file, float* _E, float* _Ei, float* _EiT) {
//        memcpy(E, _E, 16 * sizeof(float)); memcpy(Ei, _Ei, 16 * sizeof(float)); memcpy(EiT, _EiT, 16 * sizeof(float));
//        tinyobj::attrib_t a; vector<tinyobj::shape_t> s; vector<tinyobj::material_t> m; string w, e;
//        if (!tinyobj::LoadObj(&a, &s, &m, &w, &e, file.c_str())) cout << "OBJ LOAD FAILED\n";
//        vec3 minB(1e10f), maxB(-1e10f);
//        for (auto& sh : s) {
//            for (auto& idx : sh.mesh.indices) {
//                float vx = a.vertices[3 * idx.vertex_index + 0], vy = a.vertices[3 * idx.vertex_index + 1], vz = a.vertices[3 * idx.vertex_index + 2];
//                verts.push_back(vx); verts.push_back(vy); verts.push_back(vz);
//                if (!a.normals.empty()) {
//                    normals.push_back(a.normals[3 * idx.normal_index + 0]);
//                    normals.push_back(a.normals[3 * idx.normal_index + 1]);
//                    normals.push_back(a.normals[3 * idx.normal_index + 2]);
//                }
//                minB.x = min(minB.x, vx); minB.y = min(minB.y, vy); minB.z = min(minB.z, vz);
//                maxB.x = max(maxB.x, vx); maxB.y = max(maxB.y, vy); maxB.z = max(maxB.z, vz);
//            }
//        }
//        sphereCenter = (minB + maxB) * 0.5f; sphereRadius = length(maxB - sphereCenter);
//    }
//    vec3 mult(const float* M, vec3 v, bool isPt) const {
//        float x = M[0] * v.x + M[4] * v.y + M[8] * v.z + (isPt ? M[12] : 0);
//        float y = M[1] * v.x + M[5] * v.y + M[9] * v.z + (isPt ? M[13] : 0);
//        float z = M[2] * v.x + M[6] * v.y + M[10] * v.z + (isPt ? M[14] : 0);
//        return vec3(x, y, z);
//    }
//    bool intersect(const Ray& worldRay, float tMin, float tMax, Hit& h) const override {
//        vec3 ro_world(worldRay.ox, worldRay.oy, worldRay.oz), rd_world(worldRay.dx, worldRay.dy, worldRay.dz);
//        vec3 ro_L = mult(Ei, ro_world, true); vec3 rd_L = normalize(mult(Ei, rd_world, false));
//        vec3 oc = ro_L - sphereCenter;
//        float b = dot(oc, rd_L), c = dot(oc, oc) - sphereRadius * sphereRadius;
//        if (c > 0.0f && b > 0.0f) return false;
//        if (b * b - c < 0.0f) return false;
//        bool hit = false; float best_t_local = 1e20f; vec3 best_n_local;
//        for (size_t i = 0; i < verts.size(); i += 9) {
//            vec3 v0(verts[i], verts[i + 1], verts[i + 2]), v1(verts[i + 3], verts[i + 4], verts[i + 5]), v2(verts[i + 6], verts[i + 7], verts[i + 8]);
//            vec3 e1 = v1 - v0, e2 = v2 - v0, pv = cross(rd_L, e2);
//            float det = dot(e1, pv); if (fabs(det) < 1e-8) continue;
//            float invDet = 1.0f / det; vec3 tv = ro_L - v0;
//            float u = dot(tv, pv) * invDet; if (u < 0 || u > 1) continue;
//            vec3 qv = cross(tv, e1); float v = dot(rd_L, qv) * invDet; if (v < 0 || u + v > 1) continue;
//            float tHit = dot(e2, qv) * invDet;
//            if (tHit > 0.001f && tHit < best_t_local) {
//                best_t_local = tHit;
//                if (!normals.empty()) {
//                    float w = 1.0f - u - v;
//                    vec3 n0(normals[i], normals[i + 1], normals[i + 2]), n1(normals[i + 3], normals[i + 4], normals[i + 5]), n2(normals[i + 6], normals[i + 7], normals[i + 8]);
//                    best_n_local = normalize(n0 * w + n1 * u + n2 * v);
//                }
//                else { best_n_local = normalize(cross(e1, e2)); }
//                hit = true;
//            }
//        }
//        if (hit) {
//            vec3 worldP = mult(E, ro_L + rd_L * best_t_local, true);
//            float worldT = length(worldP - ro_world);
//            if (worldT < tMax && worldT > tMin) {
//                h.t = worldT; h.p = worldP; h.n = normalize(mult(EiT, best_n_local, false));
//                h.ka = ka; h.kd = kd; h.ks = ks; h.exp = exp; return true;
//            }
//        }
//        return false;
//    }
//};
//
//vec3 trace(const Ray& ray, const vector<shared_ptr<Shape>>& scene, const vector<Light>& lights, int depth, int maxDepth, const vec3& eye, int scnNum) {
//    if (depth > maxDepth) return vec3(0);
//    Hit bestHit; bestHit.t = 1e20f; bool found = false;
//    for (const auto& obj : scene) {
//        Hit h; if (obj->intersect(ray, EPS, bestHit.t, h)) { bestHit = h; found = true; }
//    }
//    if (!found) return vec3(0.0f);
//
//    if (scnNum == 9) {
//        if (length(bestHit.ka) > 5.0f) return bestHit.ka;
//        if (bestHit.reflectivity > 0.0f) {
//            vec3 R = reflect(normalize(vec3(ray.dx, ray.dy, ray.dz)), bestHit.n);
//            Ray reflRay; reflRay.ox = bestHit.p.x + R.x * EPS; reflRay.oy = bestHit.p.y + R.y * EPS; reflRay.oz = bestHit.p.z + R.z * EPS;
//            reflRay.dx = R.x; reflRay.dy = R.y; reflRay.dz = R.z;
//            return trace(reflRay, scene, lights, depth + 1, maxDepth, eye, scnNum);
//        }
//        vec3 randDir = random_in_hemisphere(bestHit.n);
//        Ray bounceRay; bounceRay.ox = bestHit.p.x + randDir.x * EPS; bounceRay.oy = bestHit.p.y + randDir.y * EPS; bounceRay.oz = bestHit.p.z + randDir.z * EPS;
//        bounceRay.dx = randDir.x; bounceRay.dy = randDir.y; bounceRay.dz = randDir.z;
//        return trace(bounceRay, scene, lights, depth + 1, maxDepth, eye, scnNum) * bestHit.kd;
//    }
//
//    vec3 color = bestHit.ka;
//    for (const auto& light : lights) {
//        vec3 L_vec = light.pos - bestHit.p; float distToLight = length(L_vec); vec3 L = L_vec * (1.0f / distToLight);
//        bool inShadow = false;
//        if (scnNum >= 2) {
//            Ray sRay; sRay.ox = bestHit.p.x + bestHit.n.x * EPS; sRay.oy = bestHit.p.y + bestHit.n.y * EPS; sRay.oz = bestHit.p.z + bestHit.n.z * EPS;
//            sRay.dx = L.x; sRay.dy = L.y; sRay.dz = L.z;
//            Hit sHit;
//            for (const auto& obj : scene) { if (obj->intersect(sRay, EPS, distToLight - EPS, sHit)) { inShadow = true; break; } }
//        }
//        if (!inShadow) {
//            vec3 V = normalize(eye - bestHit.p), H = normalize(L + V);
//            color = color + (bestHit.kd * light.intensity) * max(0.0f, dot(bestHit.n, L)) + (bestHit.ks * light.intensity) * pow(max(0.0f, dot(bestHit.n, H)), bestHit.exp);
//        }
//    }
//    if (bestHit.reflectivity > 0.0f) {
//        vec3 R = reflect(normalize(vec3(ray.dx, ray.dy, ray.dz)), bestHit.n);
//        Ray reflRay; reflRay.ox = bestHit.p.x + R.x * EPS; reflRay.oy = bestHit.p.y + R.y * EPS; reflRay.oz = bestHit.p.z + R.z * EPS;
//        reflRay.dx = R.x; reflRay.dy = R.y; reflRay.dz = R.z;
//        color = color + trace(reflRay, scene, lights, depth + 1, maxDepth, eye, scnNum) * bestHit.reflectivity;
//    }
//    return color;
//}
//
//int main(int argc, char** argv) {
//    if (argc < 4) return 0;
//    int scn = stoi(argv[1]), w = stoi(argv[2]); string out = argv[3];
//    Camera cam; vector<shared_ptr<Shape>> scene; vector<Light> lts; vec3 eye(0, 0, 5);
//    float E[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 }, Ei[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 }, EiT[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
//
//    if (scn <= 2 || scn == 8) {
//        lts.push_back({ vec3(-2,1,1), 1.0f });
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, -1, 1), 1, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(0.5, -1, -1), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(0, 1, 0), 1, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
//    }
//    else if (scn == 3) {
//        lts.push_back({ vec3(1,2,2), 0.5f }); lts.push_back({ vec3(-1,2,-1), 0.5f });
//        scene.push_back(make_shared<Ellipsoid>(vec3(0.5, 0, 0.5), vec3(0.5, 0.6, 0.2), vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, 0, -0.5), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1, 1, 1), vec3(0), 0));
//    }
//    else if (scn <= 5) {
//        lts.push_back({ vec3(-1,2,1), 0.5f }); lts.push_back({ vec3(0.5,-0.5,0), 0.5f });
//        scene.push_back(make_shared<Sphere>(vec3(0.5, -0.7, 0.5), 0.3, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(1.0, -0.7, 0.0), 0.3, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1, 1, 1), vec3(0), 0));
//        scene.push_back(make_shared<Plane>(vec3(0, 0, -3), vec3(0, 0, 1), vec3(0.1), vec3(1, 1, 1), vec3(0), 0));
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, 0, -0.5), 1, vec3(0), vec3(0), vec3(0), 0));
//        scene.push_back(make_shared<Sphere>(vec3(1.5, 0, -1.5), 1, vec3(0), vec3(0), vec3(0), 0));
//    }
//    else if (scn <= 7) {
//        if (scn == 7) {
//            float E7[16] = { 1.5f, 0, 0, 0, 0, 1.4095f, 0.5130f, 0, 0, -0.5130f, 1.4095f, 0, 0.3f, -1.5f, 0.0f, 1.0f };
//            float Ei7[16] = { 0.6667f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6264f, -0.2280f, 0.0f, 0.0f, 0.2280f, 0.6264f, 0.0f, -0.2000f, 0.9397f, -0.1170f, 1.0f };
//            float EiT7[16] = { 0.6667f, 0.0f, 0.0f, -0.2000f, 0.0f, 0.6264f, 0.2280f, 0.9397f, 0.0f, -0.2280f, 0.6264f, -0.1170f, 0.0f, 0.0f, 0.0f, 1.0f };
//            lts.push_back({ vec3(1.0, 1.0, 2.0), 1.0f });
//            scene.push_back(make_shared<Mesh>(argv[4], E7, Ei7, EiT7));
//        }
//        else {
//            lts.push_back({ vec3(-1.0, 1.0, 1.0), 1.0f });
//            scene.push_back(make_shared<Mesh>(argv[4], E, Ei, EiT));
//        }
//    }
//    else if (scn == 9) {
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0), vec3(1), vec3(0), 0));
//        scene.push_back(make_shared<Plane>(vec3(0, 1, 0), vec3(0, -1, 0), vec3(0), vec3(1), vec3(0), 0));
//        scene.push_back(make_shared<Plane>(vec3(-1, 0, 0), vec3(1, 0, 0), vec3(0), vec3(1, 0, 0), vec3(0), 0));
//        scene.push_back(make_shared<Plane>(vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0), vec3(0, 0, 1), vec3(0), 0));
//        scene.push_back(make_shared<Plane>(vec3(0, 0, -1), vec3(0, 0, 1), vec3(0), vec3(1), vec3(0), 0));
//        scene.push_back(make_shared<Sphere>(vec3(0, 0.95, -0.5), 0.1, vec3(30), vec3(0), vec3(0), 0));
//        scene.push_back(make_shared<Sphere>(vec3(-0.4, -0.7, -0.5), 0.3, vec3(0), vec3(0), vec3(1), 0));
//        scene.push_back(make_shared<Sphere>(vec3(0.4, -0.7, -0.8), 0.3, vec3(0), vec3(1), vec3(0), 0));
//    }
//
//    if (scn == 8) { cam.setup(w, w, vec3_cam(-3, 0, 0), vec3_cam(1, 0, 0), vec3_cam(0, 1, 0), 60.0f); eye = vec3(-3, 0, 0); }
//    else if (scn == 9) { cam.setup(w, w, vec3_cam(0, 0, 2.5), vec3_cam(0, 0, -1), vec3_cam(0, 1, 0), 45.0f); eye = vec3(0, 0, 2.5); }
//    else { cam.setup(w, w); }
//
//    auto img = make_shared<Image>(w, w);
//    int maxD = (scn >= 5) ? 5 : ((scn == 4) ? 1 : 0);
//    int spp = (scn == 9) ? 256 : 1;
//
//#pragma omp parallel for schedule(dynamic) // Add this for 10x speedup if using OpenMP
//    for (int j = 0; j < w; ++j) {
//        for (int i = 0; i < w; ++i) {
//            vec3 col(0);
//            for (int s = 0; s < spp; ++s) {
//                // FIXED: width - 1 - j handles vertical flip
//                col = col + trace(cam.getRay(i, w - 1 - j), scene, lts, 0, maxD, eye, scn);
//            }
//            col = col * (1.0f / (float)spp);
//            if (scn == 9) {
//                col.x = pow(my_clamp(col.x, 0, 1), 1 / 2.2f);
//                col.y = pow(my_clamp(col.y, 0, 1), 1 / 2.2f);
//                col.z = pow(my_clamp(col.z, 0, 1), 1 / 2.2f);
//            }
//            img->setPixel(i, j, (int)(my_clamp(col.x, 0, 1) * 255), (int)(my_clamp(col.y, 0, 1) * 255), (int)(my_clamp(col.z, 0, 1) * 255));
//        }
//    }
//    img->writeToFile(out);
//    return 0;
//}


//#include <iostream>
//#include <vector>
//#include <string>
//#include <memory>
//#include <cmath>
//#include <algorithm>
//#include <random>
//
//#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"
//#include "Image.h"
//#include "Camera.h"
//
//using namespace std;
//
//const float EPS = 0.0001f;
//
//// --- Utilities ---
//inline float my_clamp(float n, float lower, float upper) {
//    return max(lower, min(n, upper));
//}
//
//// Thread-local random for speed and noise reduction
//inline float get_random() {
//    static thread_local std::mt19937 generator(std::random_device{}());
//    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
//    return distribution(generator);
//}
//
//struct vec3 {
//    float x, y, z;
//    vec3(float v = 0) : x(v), y(v), z(v) {}
//    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
//    vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
//    vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
//    vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }
//    vec3 operator*(const vec3& v) const { return vec3(x * v.x, y * v.y, z * v.z); }
//};
//
//inline float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
//inline vec3 cross(vec3 a, vec3 b) { return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
//inline float length(vec3 v) { return sqrt(dot(v, v)); }
//inline vec3 normalize(vec3 v) {
//    float len = length(v);
//    return (len < 1e-8f) ? vec3(0) : v * (1.0f / len);
//}
//inline vec3 reflect(const vec3& I, const vec3& N) { return I - N * (2.0f * dot(I, N)); }
//
//// Importance sampling for Scene 9
//vec3 random_in_hemisphere(vec3 normal) {
//    float u1 = get_random();
//    float u2 = get_random();
//    float r = sqrt(u1);
//    float theta = 2.0f * 3.14159265f * u2;
//    float x = r * cos(theta);
//    float y = r * sin(theta);
//    float z = sqrt(max(0.0f, 1.0f - u1));
//    vec3 w = normal;
//    vec3 a = (fabs(w.x) > 0.9f) ? vec3(0, 1, 0) : vec3(1, 0, 0);
//    vec3 v = normalize(cross(a, w));
//    vec3 u = cross(w, v);
//    return u * x + v * y + w * z;
//}
//
//struct Hit {
//    float t;
//    vec3 p, n, ka, kd, ks;
//    float exp;
//    bool isMirror = false;
//};
//
//struct Light { vec3 pos; float intensity; };
//
//class Shape {
//public:
//    virtual bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const = 0;
//};
//
//class Sphere : public Shape {
//public:
//    vec3 center; float radius, exp; vec3 ka, kd, ks;
//    bool isMirror = false; // Added missing member
//
//    Sphere(vec3 c, float r, vec3 a, vec3 d, vec3 s, float e)
//        : center(c), radius(r), ka(a), kd(d), ks(s), exp(e) {
//    }
//
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz), oc = ro - center;
//        float a = dot(rd, rd), b = 2.0f * dot(oc, rd), c = dot(oc, oc) - radius * radius;
//        float disc = b * b - 4 * a * c;
//        if (disc < 0) return false;
//        float t = (-b - sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = ro + rd * t; hit.n = (hit.p - center) * (1.0f / radius);
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp;
//        hit.isMirror = this->isMirror;
//        return true;
//    }
//};
//
//class Plane : public Shape {
//public:
//    vec3 point, normal, ka, kd, ks; float exp;
//    Plane(vec3 p, vec3 n, vec3 a, vec3 d, vec3 s, float e) : point(p), normal(normalize(n)), ka(a), kd(d), ks(s), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz);
//        float d = dot(rd, normal);
//        if (fabs(d) < 1e-6) return false;
//        float t = dot(point - ro, normal) / d;
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = ro + rd * t; hit.n = normal;
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.isMirror = false;
//        return true;
//    }
//};
//
//class Ellipsoid : public Shape {
//public:
//    vec3 center, scale, ka, kd, ks; float exp;
//    Ellipsoid(vec3 c, vec3 s, vec3 a, vec3 d, vec3 sp, float e) : center(c), scale(s), ka(a), kd(d), ks(sp), exp(e) {}
//    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
//        vec3 ro_L = (vec3(ray.ox, ray.oy, ray.oz) - center);
//        ro_L.x /= scale.x; ro_L.y /= scale.y; ro_L.z /= scale.z;
//        vec3 rd_L(ray.dx / scale.x, ray.dy / scale.y, ray.dz / scale.z);
//        float a = dot(rd_L, rd_L), b = 2.0f * dot(ro_L, rd_L), c = dot(ro_L, ro_L) - 1.0f;
//        float disc = b * b - 4 * a * c;
//        if (disc < 0) return false;
//        float t = (-b - sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
//        if (t < tMin || t > tMax) return false;
//        hit.t = t; hit.p = vec3(ray.ox, ray.oy, ray.oz) + vec3(ray.dx, ray.dy, ray.dz) * t;
//        vec3 p_L = ro_L + rd_L * t;
//        hit.n = normalize(vec3(p_L.x / scale.x, p_L.y / scale.y, p_L.z / scale.z));
//        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.isMirror = false;
//        return true;
//    }
//};
//
//class Mesh : public Shape {
//public:
//    vector<float> verts; vector<float> normals;
//    vec3 ka{ 0.1f }, kd{ 0.0f, 0.0f, 1.0f }, ks{ 1.0f, 1.0f, 0.5f }; float exp = 100.0f;
//    float E[16], Ei[16], EiT[16]; vec3 sphereCenter; float sphereRadius;
//    Mesh(string file, float* _E, float* _Ei, float* _EiT) {
//        memcpy(E, _E, 16 * sizeof(float)); memcpy(Ei, _Ei, 16 * sizeof(float)); memcpy(EiT, _EiT, 16 * sizeof(float));
//        tinyobj::attrib_t a; vector<tinyobj::shape_t> s; vector<tinyobj::material_t> m; string w, e;
//        tinyobj::LoadObj(&a, &s, &m, &w, &e, file.c_str());
//        vec3 minB(1e10f), maxB(-1e10f);
//        for (auto& sh : s) {
//            for (auto& idx : sh.mesh.indices) {
//                float vx = a.vertices[3 * idx.vertex_index + 0], vy = a.vertices[3 * idx.vertex_index + 1], vz = a.vertices[3 * idx.vertex_index + 2];
//                verts.push_back(vx); verts.push_back(vy); verts.push_back(vz);
//                if (!a.normals.empty()) {
//                    normals.push_back(a.normals[3 * idx.normal_index + 0]);
//                    normals.push_back(a.normals[3 * idx.normal_index + 1]);
//                    normals.push_back(a.normals[3 * idx.normal_index + 2]);
//                }
//                minB.x = min(minB.x, vx); minB.y = min(minB.y, vy); minB.z = min(minB.z, vz);
//                maxB.x = max(maxB.x, vx); maxB.y = max(maxB.y, vy); maxB.z = max(maxB.z, vz);
//            }
//        }
//        sphereCenter = (minB + maxB) * 0.5f; sphereRadius = length(maxB - sphereCenter);
//    }
//    vec3 mult(const float* M, vec3 v, bool isPt) const {
//        float x = M[0] * v.x + M[4] * v.y + M[8] * v.z + (isPt ? M[12] : 0);
//        float y = M[1] * v.x + M[5] * v.y + M[9] * v.z + (isPt ? M[13] : 0);
//        float z = M[2] * v.x + M[6] * v.y + M[10] * v.z + (isPt ? M[14] : 0);
//        return vec3(x, y, z);
//    }
//    bool intersect(const Ray& worldRay, float tMin, float tMax, Hit& h) const override {
//        vec3 ro_world(worldRay.ox, worldRay.oy, worldRay.oz), rd_world(worldRay.dx, worldRay.dy, worldRay.dz);
//        vec3 ro_L = mult(Ei, ro_world, true); vec3 rd_L = normalize(mult(Ei, rd_world, false));
//        vec3 oc = ro_L - sphereCenter;
//        float b = dot(oc, rd_L), c = dot(oc, oc) - sphereRadius * sphereRadius;
//        if (c > 0.0f && b > 0.0f) return false;
//        if (b * b - c < 0.0f) return false;
//        bool hit = false; float best_t_local = 1e20f; vec3 best_n_local;
//        for (size_t i = 0; i < verts.size(); i += 9) {
//            vec3 v0(verts[i], verts[i + 1], verts[i + 2]), v1(verts[i + 3], verts[i + 4], verts[i + 5]), v2(verts[i + 6], verts[i + 7], verts[i + 8]);
//            vec3 e1 = v1 - v0, e2 = v2 - v0, pv = cross(rd_L, e2);
//            float det = dot(e1, pv); if (fabs(det) < 1e-8) continue;
//            float invDet = 1.0f / det; vec3 tv = ro_L - v0;
//            float u = dot(tv, pv) * invDet; if (u < 0 || u > 1) continue;
//            vec3 qv = cross(tv, e1); float v = dot(rd_L, qv) * invDet; if (v < 0 || u + v > 1) continue;
//            float tHit = dot(e2, qv) * invDet;
//            if (tHit > 0.001f && tHit < best_t_local) {
//                best_t_local = tHit;
//                if (!normals.empty()) {
//                    float w = 1.0f - u - v;
//                    vec3 n0(normals[i], normals[i + 1], normals[i + 2]), n1(normals[i + 3], normals[i + 4], normals[i + 5]), n2(normals[i + 6], normals[i + 7], normals[i + 8]);
//                    best_n_local = normalize(n0 * w + n1 * u + n2 * v);
//                }
//                else { best_n_local = normalize(cross(e1, e2)); }
//                hit = true;
//            }
//        }
//        if (hit) {
//            vec3 worldP = mult(E, ro_L + rd_L * best_t_local, true);
//            float worldT = length(worldP - ro_world);
//            if (worldT < tMax && worldT > tMin) {
//                h.t = worldT; h.p = worldP; h.n = normalize(mult(EiT, best_n_local, false));
//                h.ka = ka; h.kd = kd; h.ks = ks; h.exp = exp; h.isMirror = false; return true;
//            }
//        }
//        return false;
//    }
//};
//vec3 trace(const Ray& ray, const vector<shared_ptr<Shape>>& scene, const vector<Light>& lts, int depth, int maxD, vec3 eye, int scn) {
//    if (depth > maxD) return vec3(0);
//
//    Hit best;
//    best.t = 1e20f;
//    bool found = false;
//    for (auto& obj : scene) {
//        Hit h;
//        if (obj->intersect(ray, EPS, best.t, h)) {
//            best = h;
//            found = true;
//        }
//    }
//
//    if (!found) return vec3(0);
//
//    // --- Scene 9 Logic (Keep as provided in your original code) ---
//    if (scn == 9) {
//        // ... (Keep your existing Scene 9 implementation here)
//    }
//
//    // --- Local Lighting (Standard Blinn-Phong for all Scenes) ---
//    vec3 color = best.ka;
//    for (auto& l : lts) {
//        vec3 Ldir = normalize(l.pos - best.p);
//        bool shadowed = false;
//
//        if (scn >= 2) {
//            Ray sRay;
//            // BIAS: Use 0.001f to fix the "zebra stripes" seen in your previous Scene 4
//            float bias = 0.001f;
//            sRay.ox = best.p.x + best.n.x * bias;
//            sRay.oy = best.p.y + best.n.y * bias;
//            sRay.oz = best.p.z + best.n.z * bias;
//            sRay.dx = Ldir.x; sRay.dy = Ldir.y; sRay.dz = Ldir.z;
//
//            Hit sHit;
//            float distToLight = length(l.pos - best.p);
//            for (auto& obj : scene) {
//                if (obj->intersect(sRay, bias, distToLight, sHit)) {
//                    shadowed = true;
//                    break;
//                }
//            }
//        }
//
//        if (!shadowed) {
//            vec3 V = normalize(eye - best.p), H = normalize(Ldir + V);
//            color = color + (best.kd * l.intensity) * max(0.0f, dot(best.n, Ldir)) +
//                (best.ks * l.intensity) * pow(max(0.0f, dot(best.n, H)), best.exp);
//        }
//    }
//
//    // --- Scene 5 REFLECTION LOGIC ---
//    // Perfect mirrors in Scene 5 have no diffuse component (kd is zero).
//    if (scn == 5 && (best.kd.x == 0 && best.kd.y == 0 && best.kd.z == 0)) {
//        vec3 I = normalize(vec3(ray.dx, ray.dy, ray.dz));
//        vec3 R = reflect(I, best.n);
//
//        Ray rRay;
//        float rBias = 0.001f;
//        rRay.ox = best.p.x + R.x * rBias;
//        rRay.oy = best.p.y + R.y * rBias;
//        rRay.oz = best.p.z + R.z * rBias;
//        rRay.dx = R.x; rRay.dy = R.y; rRay.dz = R.z;
//
//        // Recursive call: add the reflected color multiplied by the specular coefficient
//        color = color + trace(rRay, scene, lts, depth + 1, maxD, eye, scn) * best.ks;
//    }
//
//    return color;
//}
//int main(int argc, char** argv) {
//    if (argc < 4) return 0;
//    int scn = stoi(argv[1]), w = stoi(argv[2]); string out = argv[3];
//    Camera cam; vector<shared_ptr<Shape>> scene; vector<Light> lts; vec3 eye(0, 0, 5);
//    float E[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 }, Ei[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 }, EiT[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
//
//    if (scn <= 2 || scn == 8) {
//        lts.push_back({ vec3(-2,1,1), 1.0f });
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, -1, 1), 1, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(0.5, -1, -1), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(0, 1, 0), 1, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
//    }
//    else if (scn == 3) {
//        lts.push_back({ vec3(1,2,2), 0.5f }); lts.push_back({ vec3(-1,2,-1), 0.5f });
//        scene.push_back(make_shared<Ellipsoid>(vec3(0.5, 0, 0.5), vec3(0.5, 0.6, 0.2), vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(-0.5, 0, -0.5), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1), vec3(0), 0));
//    }
//    else if (scn <= 5) {
//        lts.push_back({ vec3(-1,2,1), 0.5f }); lts.push_back({ vec3(0.5,-0.5,0), 0.5f });
//        scene.push_back(make_shared<Sphere>(vec3(0.5, -0.7, 0.5), 0.3, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Sphere>(vec3(1.0, -0.7, 0.0), 0.3, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1), vec3(0), 0));
//        scene.push_back(make_shared<Plane>(vec3(0, 0, -3), vec3(0, 0, 1), vec3(0.1), vec3(1), vec3(0), 0));
//    }
//
//    else if (scn <= 7) {
//                if (scn == 7) {
//                    float E7[16] = { 1.5f, 0, 0, 0, 0, 1.4095f, 0.5130f, 0, 0, -0.5130f, 1.4095f, 0, 0.3f, -1.5f, 0.0f, 1.0f };
//                    float Ei7[16] = { 0.6667f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6264f, -0.2280f, 0.0f, 0.0f, 0.2280f, 0.6264f, 0.0f, -0.2000f, 0.9397f, -0.1170f, 1.0f };
//                    float EiT7[16] = { 0.6667f, 0.0f, 0.0f, -0.2000f, 0.0f, 0.6264f, 0.2280f, 0.9397f, 0.0f, -0.2280f, 0.6264f, -0.1170f, 0.0f, 0.0f, 0.0f, 1.0f };
//                    lts.push_back({ vec3(1.0, 1.0, 2.0), 1.0f });
//                    scene.push_back(make_shared<Mesh>(argv[4], E7, Ei7, EiT7));
//                }
//                else {
//                    lts.push_back({ vec3(-1.0, 1.0, 1.0), 1.0f });
//                    scene.push_back(make_shared<Mesh>(argv[4], E, Ei, EiT));
//                }
//            }
//
//    else if (scn == 9) {
//        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0), vec3(1), vec3(0), 0)); // floor
//        scene.push_back(make_shared<Plane>(vec3(0, 1, 0), vec3(0, -1, 0), vec3(0), vec3(1), vec3(0), 0)); // ceiling
//        scene.push_back(make_shared<Plane>(vec3(-1, 0, 0), vec3(1, 0, 0), vec3(0), vec3(1, 0, 0), vec3(0), 0)); // red
//        scene.push_back(make_shared<Plane>(vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0), vec3(0, 0, 1), vec3(0), 0)); // blue
//        scene.push_back(make_shared<Plane>(vec3(0, 0, -1), vec3(0, 0, 1), vec3(0), vec3(1), vec3(0), 0)); // back
//        scene.push_back(make_shared<Sphere>(vec3(0, 0.99, -0.5), 0.15, vec3(15), vec3(0), vec3(0), 0)); // light
//        auto mirr = make_shared<Sphere>(vec3(-0.4, -0.7, -0.4), 0.3, vec3(0), vec3(0), vec3(1), 100);
//        mirr->isMirror = true; // Correctly using the class member
//        scene.push_back(mirr);
//        scene.push_back(make_shared<Sphere>(vec3(0.4, -0.7, -0.7), 0.3, vec3(0), vec3(1), vec3(0), 0));
//    }
//
//    if (scn == 8) { cam.setup(w, w, vec3_cam(-3, 0, 0), vec3_cam(1, 0, 0), vec3_cam(0, 1, 0), 60.0f); eye = vec3(-3, 0, 0); }
//    else if (scn == 9) { cam.setup(w, w, vec3_cam(0, 0, 2.6), vec3_cam(0, 0, -1), vec3_cam(0, 1, 0), 45.0f); eye = vec3(0, 0, 2.6); }
//    else { cam.setup(w, w); }
//
//    auto img = make_shared<Image>(w, w);
//    int spp = (scn == 9) ? 256 : 1;
//    int maxDepth = 5;
//#pragma omp parallel for schedule(dynamic)
//    for (int j = 0; j < w; ++j) {
//        for (int i = 0; i < w; ++i) {
//            vec3 col(0);
//            for (int s = 0; s < spp; ++s) {
//                // Using your requested coordinate logic
//                //col = col + trace(cam.getRay(i, j - 1 - w), scene, lts, 0, maxDepth, eye, scn);
//                col = col + trace(cam.getRay(i, w - 1 - j), scene, lts, 0, maxDepth, eye, scn);
//            }
//
//            col = col * (1.0f / (float)spp);
//
//            if (scn == 9) {
//                // FIX: Use 1.0f to ensure floating point division
//                col.x = pow(my_clamp(col.x, 0.0f, 1.0f), 1.0f / 2.2f);
//                col.y = pow(my_clamp(col.y, 0.0f, 1.0f), 1.0f / 2.2f);
//                col.z = pow(my_clamp(col.z, 0.0f, 1.0f), 1.0f / 2.2f);
//            }
//
//            // Final output clamp to prevent wrap-around colors
//            int r = (int)(my_clamp(col.x, 0.0f, 1.0f) * 255.0f);
//            int g = (int)(my_clamp(col.y, 0.0f, 1.0f) * 255.0f);
//            int b = (int)(my_clamp(col.z, 0.0f, 1.0f) * 255.0f);
//
//            img->setPixel(i, j, r, g, b);
//        }
//    }
//    img->writeToFile(out);
// 
//    return 0;
//}


#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <algorithm>
#include <random>
#include <cstring>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Image.h"
#include "Camera.h"

using namespace std;

const float EPS = 0.0001f;

// --- Utilities ---
inline float my_clamp(float n, float lower, float upper) {
    return max(lower, min(n, upper));
}

inline float get_random() {
    static thread_local std::mt19937 generator(std::random_device{}());
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution(generator);
}

struct vec3 {
    float x, y, z;
    vec3(float v = 0) : x(v), y(v), z(v) {}
    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    vec3 operator+(const vec3& v) const { return vec3(x + v.x, y + v.y, z + v.z); }
    vec3 operator-(const vec3& v) const { return vec3(x - v.x, y - v.y, z - v.z); }
    vec3 operator*(float s) const { return vec3(x * s, y * s, z * s); }
    vec3 operator*(const vec3& v) const { return vec3(x * v.x, y * v.y, z * v.z); }
};

inline float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline vec3 cross(vec3 a, vec3 b) { return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
inline float length(vec3 v) { return sqrt(dot(v, v)); }
inline vec3 normalize(vec3 v) {
    float len = length(v);
    return (len < 1e-8f) ? vec3(0) : v * (1.0f / len);
}
inline vec3 reflect(const vec3& I, const vec3& N) { return I - N * (2.0f * dot(I, N)); }

vec3 random_in_hemisphere(vec3 normal) {
    float u1 = get_random();
    float u2 = get_random();
    float r = sqrt(u1);
    float theta = 2.0f * 3.14159265f * u2;
    float x = r * cos(theta);
    float y = r * sin(theta);
    float z = sqrt(max(0.0f, 1.0f - u1));
    vec3 w = normal;
    vec3 a = (fabs(w.x) > 0.9f) ? vec3(0, 1, 0) : vec3(1, 0, 0);
    vec3 v = normalize(cross(a, w));
    vec3 u = cross(w, v);
    return u * x + v * y + w * z;
}

struct Hit {
    float t;
    vec3 p, n, ka, kd, ks;
    float exp;
    bool isMirror = false;
};

struct Light { vec3 pos; float intensity; };

class Shape {
public:
    virtual bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const = 0;
};

class Sphere : public Shape {
public:
    vec3 center; float radius, exp; vec3 ka, kd, ks;
    bool isMirror = false;
    Sphere(vec3 c, float r, vec3 a, vec3 d, vec3 s, float e) : center(c), radius(r), ka(a), kd(d), ks(s), exp(e) {}
    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz), oc = ro - center;
        float a = dot(rd, rd), b = 2.0f * dot(oc, rd), c = dot(oc, oc) - radius * radius;
        float disc = b * b - 4 * a * c;
        if (disc < 0) return false;
        float t = (-b - sqrt(disc)) / (2.0f * a);
        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
        if (t < tMin || t > tMax) return false;
        hit.t = t; hit.p = ro + rd * t; hit.n = (hit.p - center) * (1.0f / radius);
        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.isMirror = isMirror;
        return true;
    }
};

class Plane : public Shape {
public:
    vec3 point, normal, ka, kd, ks; float exp;
    Plane(vec3 p, vec3 n, vec3 a, vec3 d, vec3 s, float e) : point(p), normal(normalize(n)), ka(a), kd(d), ks(s), exp(e) {}
    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
        vec3 ro(ray.ox, ray.oy, ray.oz), rd(ray.dx, ray.dy, ray.dz);
        float d = dot(rd, normal);
        if (fabs(d) < 1e-6) return false;
        float t = dot(point - ro, normal) / d;
        if (t < tMin || t > tMax) return false;
        hit.t = t; hit.p = ro + rd * t; hit.n = normal;
        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.isMirror = false;
        return true;
    }
};

class Ellipsoid : public Shape {
public:
    vec3 center, scale, ka, kd, ks; float exp;
    Ellipsoid(vec3 c, vec3 s, vec3 a, vec3 d, vec3 sp, float e) : center(c), scale(s), ka(a), kd(d), ks(sp), exp(e) {}
    bool intersect(const Ray& ray, float tMin, float tMax, Hit& hit) const override {
        vec3 ro_L = (vec3(ray.ox, ray.oy, ray.oz) - center);
        ro_L.x /= scale.x; ro_L.y /= scale.y; ro_L.z /= scale.z;
        vec3 rd_L(ray.dx / scale.x, ray.dy / scale.y, ray.dz / scale.z);
        float a = dot(rd_L, rd_L), b = 2.0f * dot(ro_L, rd_L), c = dot(ro_L, ro_L) - 1.0f;
        float disc = b * b - 4 * a * c;
        if (disc < 0) return false;
        float t = (-b - sqrt(disc)) / (2.0f * a);
        if (t < tMin || t > tMax) t = (-b + sqrt(disc)) / (2.0f * a);
        if (t < tMin || t > tMax) return false;
        hit.t = t; hit.p = vec3(ray.ox, ray.oy, ray.oz) + vec3(ray.dx, ray.dy, ray.dz) * t;
        vec3 p_L = ro_L + rd_L * t;
        hit.n = normalize(vec3(p_L.x / scale.x, p_L.y / scale.y, p_L.z / scale.z));
        hit.ka = ka; hit.kd = kd; hit.ks = ks; hit.exp = exp; hit.isMirror = false;
        return true;
    }
};

class Mesh : public Shape {
public:
    vector<float> verts; vector<float> normals;
    vec3 ka{ 0.1f }, kd{ 0.0f, 0.0f, 1.0f }, ks{ 1.0f, 1.0f, 0.5f }; float exp = 100.0f;
    float E[16], Ei[16], EiT[16]; vec3 sphereCenter; float sphereRadius;
    Mesh(string file, float* _E, float* _Ei, float* _EiT) {
        memcpy(E, _E, 16 * sizeof(float)); memcpy(Ei, _Ei, 16 * sizeof(float)); memcpy(EiT, _EiT, 16 * sizeof(float));
        tinyobj::attrib_t a; vector<tinyobj::shape_t> s; vector<tinyobj::material_t> m; string w, e;
        tinyobj::LoadObj(&a, &s, &m, &w, &e, file.c_str());
        vec3 minB(1e10f), maxB(-1e10f);
        for (auto& sh : s) {
            for (auto& idx : sh.mesh.indices) {
                float vx = a.vertices[3 * idx.vertex_index + 0], vy = a.vertices[3 * idx.vertex_index + 1], vz = a.vertices[3 * idx.vertex_index + 2];
                verts.push_back(vx); verts.push_back(vy); verts.push_back(vz);
                if (!a.normals.empty()) {
                    normals.push_back(a.normals[3 * idx.normal_index + 0]);
                    normals.push_back(a.normals[3 * idx.normal_index + 1]);
                    normals.push_back(a.normals[3 * idx.normal_index + 2]);
                }
                minB.x = min(minB.x, vx); minB.y = min(minB.y, vy); minB.z = min(minB.z, vz);
                maxB.x = max(maxB.x, vx); maxB.y = max(maxB.y, vy); maxB.z = max(maxB.z, vz);
            }
        }
        sphereCenter = (minB + maxB) * 0.5f; sphereRadius = length(maxB - sphereCenter);
    }
    vec3 mult(const float* M, vec3 v, bool isPt) const {
        float x = M[0] * v.x + M[4] * v.y + M[8] * v.z + (isPt ? M[12] : 0);
        float y = M[1] * v.x + M[5] * v.y + M[9] * v.z + (isPt ? M[13] : 0);
        float z = M[2] * v.x + M[6] * v.y + M[10] * v.z + (isPt ? M[14] : 0);
        return vec3(x, y, z);
    }
    bool intersect(const Ray& worldRay, float tMin, float tMax, Hit& h) const override {
        vec3 ro_world(worldRay.ox, worldRay.oy, worldRay.oz), rd_world(worldRay.dx, worldRay.dy, worldRay.dz);
        vec3 ro_L = mult(Ei, ro_world, true); vec3 rd_L = normalize(mult(Ei, rd_world, false));
        vec3 oc = ro_L - sphereCenter;
        float b = dot(oc, rd_L), c = dot(oc, oc) - sphereRadius * sphereRadius;
        if (c > 0.0f && b > 0.0f) return false;
        if (b * b - c < 0.0f) return false;
        bool hit = false; float best_t_local = 1e20f; vec3 best_n_local;
        for (size_t i = 0; i < verts.size(); i += 9) {
            vec3 v0(verts[i], verts[i + 1], verts[i + 2]), v1(verts[i + 3], verts[i + 4], verts[i + 5]), v2(verts[i + 6], verts[i + 7], verts[i + 8]);
            vec3 e1 = v1 - v0, e2 = v2 - v0, pv = cross(rd_L, e2);
            float det = dot(e1, pv); if (fabs(det) < 1e-8) continue;
            float invDet = 1.0f / det; vec3 tv = ro_L - v0;
            float u = dot(tv, pv) * invDet; if (u < 0 || u > 1) continue;
            vec3 qv = cross(tv, e1); float v = dot(rd_L, qv) * invDet; if (v < 0 || u + v > 1) continue;
            float tHit = dot(e2, qv) * invDet;
            if (tHit > 0.001f && tHit < best_t_local) {
                best_t_local = tHit;
                if (!normals.empty()) {
                    float w = 1.0f - u - v;
                    vec3 n0(normals[i], normals[i + 1], normals[i + 2]), n1(normals[i + 3], normals[i + 4], normals[i + 5]), n2(normals[i + 6], normals[i + 7], normals[i + 8]);
                    best_n_local = normalize(n0 * w + n1 * u + n2 * v);
                }
                else { best_n_local = normalize(cross(e1, e2)); }
                hit = true;
            }
        }
        if (hit) {
            vec3 worldP = mult(E, ro_L + rd_L * best_t_local, true);
            float worldT = length(worldP - ro_world);
            if (worldT < tMax && worldT > tMin) {
                h.t = worldT; h.p = worldP; h.n = normalize(mult(EiT, best_n_local, false));
                h.ka = ka; h.kd = kd; h.ks = ks; h.exp = exp; h.isMirror = false; return true;
            }
        }
        return false;
    }
};

//vec3 trace(const Ray& ray, const vector<shared_ptr<Shape>>& scene, const vector<Light>& lts, int depth, int maxD, vec3 eye, int scn) {
//    if (depth > maxD) return vec3(0);
//
//    Hit best; best.t = 1e20f; bool found = false;
//    for (auto& obj : scene) {
//        Hit h;
//        if (obj->intersect(ray, EPS, best.t, h)) { best = h; found = true; }
//    }
//    if (!found) return vec3(0);
//
//    // --- Scene 9 Recursive Logic ---
//    if (scn == 9) {
//        if (best.ka.x > 1.0f) return best.ka; // Hit light source
//        if (best.isMirror) {
//            vec3 R = reflect(normalize(vec3(ray.dx, ray.dy, ray.dz)), best.n);
//            Ray rRay;
//            rRay.ox = best.p.x + R.x * EPS; rRay.oy = best.p.y + R.y * EPS; rRay.oz = best.p.z + R.z * EPS;
//            rRay.dx = R.x; rRay.dy = R.y; rRay.dz = R.z;
//            return trace(rRay, scene, lts, depth + 1, maxD, eye, scn);
//        }
//        vec3 dir = random_in_hemisphere(best.n);
//        Ray bRay;
//        bRay.ox = best.p.x + dir.x * EPS; bRay.oy = best.p.y + dir.y * EPS; bRay.oz = best.p.z + dir.z * EPS;
//        bRay.dx = dir.x; bRay.dy = dir.y; bRay.dz = dir.z;
//        return trace(bRay, scene, lts, depth + 1, maxD, eye, scn) * best.kd;
//    }
//
//    // --- Standard Lighting (Fixes Scene 4 Shadow Acne) ---
//    vec3 color = best.ka;
//    for (auto& l : lts) {
//        vec3 Ldir = normalize(l.pos - best.p);
//        bool shadowed = false;
//        if (scn >= 2) {
//            Ray sRay;
//            float bias = 0.001f; // Standard bias to remove zebra stripes
//            sRay.ox = best.p.x + best.n.x * bias;
//            sRay.oy = best.p.y + best.n.y * bias;
//            sRay.oz = best.p.z + best.n.z * bias;
//            sRay.dx = Ldir.x; sRay.dy = Ldir.y; sRay.dz = Ldir.z;
//            Hit sHit;
//            float distToLight = length(l.pos - best.p);
//            for (auto& obj : scene) {
//                if (obj->intersect(sRay, bias, distToLight, sHit)) { shadowed = true; break; }
//            }
//        }
//        if (!shadowed) {
//            vec3 V = normalize(eye - best.p), H = normalize(Ldir + V);
//            color = color + (best.kd * l.intensity) * max(0.0f, dot(best.n, Ldir)) +
//                (best.ks * l.intensity) * pow(max(0.0f, dot(best.n, H)), best.exp);
//        }
//    }
//
//    // --- Scene 5 Perfect Reflection Logic ---
//    if (scn == 5 && (best.kd.x == 0 && best.kd.y == 0 && best.kd.z == 0)) {
//        vec3 I = normalize(vec3(ray.dx, ray.dy, ray.dz));
//        vec3 R = reflect(I, best.n);
//        Ray rRay;
//        float rBias = 0.001f;
//        rRay.ox = best.p.x + R.x * rBias; rRay.oy = best.p.y + R.y * rBias; rRay.oz = best.p.z + R.z * rBias;
//        rRay.dx = R.x; rRay.dy = R.y; rRay.dz = R.z;
//        color = color + trace(rRay, scene, lts, depth + 1, maxD, eye, scn) * best.ks;
//    }
//
//    return color;
//}

vec3 trace(const Ray& ray, const vector<shared_ptr<Shape>>& scene, const vector<Light>& lts, int depth, int maxD, vec3 eye, int scn) {
    if (depth > maxD) return vec3(0);

    Hit best; best.t = 1e20f; bool found = false;
    for (auto& obj : scene) {
        Hit h;
        if (obj->intersect(ray, EPS, best.t, h)) { best = h; found = true; }
    }

    // 1. THIS FIXES THE BLACK TOP: If no object is hit, return pure black
    if (!found) return vec3(0);

    // --- Scene 9 Logic (Keep as is) ---
    if (scn == 9) {
        if (best.ka.x > 1.0f) return best.ka;
        if (best.isMirror) {
            vec3 R = reflect(normalize(vec3(ray.dx, ray.dy, ray.dz)), best.n);
            Ray rRay;
            rRay.ox = best.p.x + R.x * EPS; rRay.oy = best.p.y + R.y * EPS; rRay.oz = best.p.z + R.z * EPS;
            rRay.dx = R.x; rRay.dy = R.y; rRay.dz = R.z;
            return trace(rRay, scene, lts, depth + 1, maxD, eye, scn);
        }
        vec3 dir = random_in_hemisphere(best.n);
        Ray bRay;
        bRay.ox = best.p.x + dir.x * EPS; bRay.oy = best.p.y + dir.y * EPS; bRay.oz = best.p.z + dir.z * EPS;
        bRay.dx = dir.x; bRay.dy = dir.y; bRay.dz = dir.z;
        return trace(bRay, scene, lts, depth + 1, maxD, eye, scn) * best.kd;
    }

    // 2. THIS FIXES THE YELLOW: If it's a mirror in Scene 5, return ONLY reflection
    // Move this logic ABOVE the lighting loop
    if ((scn == 5 || scn == 4) && (best.kd.x == 0 && best.kd.y == 0 && best.kd.z == 0)) {
        
        vec3 I = normalize(vec3(ray.dx, ray.dy, ray.dz));
        vec3 R = reflect(I, best.n);
        Ray rRay;
        float rBias = 0.001f;
        rRay.ox = best.p.x + R.x * rBias;
        rRay.oy = best.p.y + R.y * rBias;
        rRay.oz = best.p.z + R.z * rBias;
        rRay.dx = R.x; rRay.dy = R.y; rRay.dz = R.z;

        // Return only the reflection; don't add local lighting colors
        return trace(rRay, scene, lts, depth + 1, maxD, eye, scn) * best.ks;
    }

    // --- Standard Lighting Loop (Only runs for non-mirror objects) ---
    vec3 color = best.ka;
    for (auto& l : lts) {
        vec3 Ldir = normalize(l.pos - best.p);
        bool shadowed = false;
        if (scn >= 2) {
            Ray sRay;
            float bias = 0.001f;
            sRay.ox = best.p.x + best.n.x * bias;
            sRay.oy = best.p.y + best.n.y * bias;
            sRay.oz = best.p.z + best.n.z * bias;
            sRay.dx = Ldir.x; sRay.dy = Ldir.y; sRay.dz = Ldir.z;
            Hit sHit;
            float distToLight = length(l.pos - best.p);
            for (auto& obj : scene) {
                if (obj->intersect(sRay, bias, distToLight, sHit)) { shadowed = true; break; }
            }
        }
        if (!shadowed) {
            vec3 V = normalize(eye - best.p), H = normalize(Ldir + V);
            color = color + (best.kd * l.intensity) * max(0.0f, dot(best.n, Ldir)) +
                (best.ks * l.intensity) * pow(max(0.0f, dot(best.n, H)), best.exp);
        }
    }

    return color;
}

int main(int argc, char** argv) {
    if (argc < 4) return 0;
    int scn = stoi(argv[1]), w = stoi(argv[2]); string out = argv[3];
    Camera cam; vector<shared_ptr<Shape>> scene; vector<Light> lts; vec3 eye(0, 0, 5);
    float E[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 }, Ei[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 }, EiT[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };

    if (scn <= 2 || scn == 8) {
        lts.push_back({ vec3(-2,1,1), 1.0f });
        scene.push_back(make_shared<Sphere>(vec3(-0.5, -1, 1), 1, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
        scene.push_back(make_shared<Sphere>(vec3(0.5, -1, -1), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
        scene.push_back(make_shared<Sphere>(vec3(0, 1, 0), 1, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
    }
    else if (scn == 3) {
        lts.push_back({ vec3(1,2,2), 0.5f }); lts.push_back({ vec3(-1,2,-1), 0.5f });
        scene.push_back(make_shared<Ellipsoid>(vec3(0.5, 0, 0.5), vec3(0.5, 0.6, 0.2), vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
        scene.push_back(make_shared<Sphere>(vec3(-0.5, 0, -0.5), 1, vec3(0.1), vec3(0, 1, 0), vec3(1, 1, 0.5), 100));
        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1), vec3(0), 0));
    }
    else if (scn <= 5) {
        lts.push_back({ vec3(-1,2,1), 0.5f }); lts.push_back({ vec3(0.5,-0.5,0), 0.5f });
        scene.push_back(make_shared<Sphere>(vec3(0.5, -0.7, 0.5), 0.3, vec3(0.1), vec3(1, 0, 0), vec3(1, 1, 0.5), 100));
        scene.push_back(make_shared<Sphere>(vec3(1.0, -0.7, 0.0), 0.3, vec3(0.1), vec3(0, 0, 1), vec3(1, 1, 0.5), 100));
        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.1), vec3(1), vec3(0), 0));
        scene.push_back(make_shared<Plane>(vec3(0, 0, -3), vec3(0, 0, 1), vec3(0.1), vec3(1), vec3(0), 0));
        if (scn == 5 || scn == 4 ) {
            // Perfect Mirror Spheres for Scene 5
            scene.push_back(make_shared<Sphere>(vec3(-0.5, 0, -0.5), 1, vec3(0), vec3(0), vec3(1, 1, 1), 100));
            scene.push_back(make_shared<Sphere>(vec3(1.5, 0, -1.5), 1.0f, vec3(0), vec3(0), vec3(1, 1, 1), 100));
        }
    }
    else if (scn <= 7) {
        if (scn == 7) {
            float E7[16] = { 1.5f, 0, 0, 0, 0, 1.4095f, 0.5130f, 0, 0, -0.5130f, 1.4095f, 0, 0.3f, -1.5f, 0.0f, 1.0f };
            float Ei7[16] = { 0.6667f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6264f, -0.2280f, 0.0f, 0.0f, 0.2280f, 0.6264f, 0.0f, -0.2000f, 0.9397f, -0.1170f, 1.0f };
            float EiT7[16] = { 0.6667f, 0.0f, 0.0f, -0.2000f, 0.0f, 0.6264f, 0.2280f, 0.9397f, 0.0f, -0.2280f, 0.6264f, -0.1170f, 0.0f, 0.0f, 0.0f, 1.0f };
            lts.push_back({ vec3(1.0, 1.0, 2.0), 1.0f });
            scene.push_back(make_shared<Mesh>(argv[4], E7, Ei7, EiT7));
        }
        else {
            lts.push_back({ vec3(-1.0, 1.0, 1.0), 1.0f });
            scene.push_back(make_shared<Mesh>(argv[4], E, Ei, EiT));
        }
    }
    else if (scn == 9) {
        scene.push_back(make_shared<Plane>(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0), vec3(1), vec3(0), 0));
        scene.push_back(make_shared<Plane>(vec3(0, 1, 0), vec3(0, -1, 0), vec3(0), vec3(1), vec3(0), 0));
        scene.push_back(make_shared<Plane>(vec3(-1, 0, 0), vec3(1, 0, 0), vec3(0), vec3(1, 0, 0), vec3(0), 0));
        scene.push_back(make_shared<Plane>(vec3(1, 0, 0), vec3(-1, 0, 0), vec3(0), vec3(0, 0, 1), vec3(0), 0));
        scene.push_back(make_shared<Plane>(vec3(0, 0, -1), vec3(0, 0, 1), vec3(0), vec3(1), vec3(0), 0));
        scene.push_back(make_shared<Sphere>(vec3(0, 0.99, -0.5), 0.15, vec3(15), vec3(0), vec3(0), 0));
        auto mirr = make_shared<Sphere>(vec3(-0.4, -0.7, -0.4), 0.3, vec3(0), vec3(0), vec3(1), 100);
        mirr->isMirror = true; scene.push_back(mirr);
        scene.push_back(make_shared<Sphere>(vec3(0.4, -0.7, -0.7), 0.3, vec3(0), vec3(1), vec3(0), 0));
    }

    if (scn == 8) { cam.setup(w, w, vec3_cam(-3, 0, 0), vec3_cam(1, 0, 0), vec3_cam(0, 1, 0), 60.0f); eye = vec3(-3, 0, 0); }
    else if (scn == 9) { cam.setup(w, w, vec3_cam(0, 0, 2.6), vec3_cam(0, 0, -1), vec3_cam(0, 1, 0), 45.0f); eye = vec3(0, 0, 2.6); }
    else { cam.setup(w, w); }

    auto img = make_shared<Image>(w, w);
    int spp = (scn == 9) ? 256 : 1;
    int maxDepth = (scn == 5 || scn == 4 || scn == 9) ? 5 : 1;

#pragma omp parallel for schedule(dynamic)
    for (int j = 0; j < w; ++j) {
        for (int i = 0; i < w; ++i) {
            vec3 col(0);
            for (int s = 0; s < spp; ++s) {
                col = col + trace(cam.getRay(i, w - 1 - j), scene, lts, 0, maxDepth, eye, scn);
            }
            col = col * (1.0f / (float)spp);
            if (scn == 9) {
                col.x = pow(my_clamp(col.x, 0.0f, 1.0f), 1.0f / 2.2f);
                col.y = pow(my_clamp(col.y, 0.0f, 1.0f), 1.0f / 2.2f);
                col.z = pow(my_clamp(col.z, 0.0f, 1.0f), 1.0f / 2.2f);
            }
            img->setPixel(i, j, (int)(my_clamp(col.x, 0, 1) * 255), (int)(my_clamp(col.y, 0, 1) * 255), (int)(my_clamp(col.z, 0, 1) * 255));
        }
    }
    img->writeToFile(out);
    return 0;
}


