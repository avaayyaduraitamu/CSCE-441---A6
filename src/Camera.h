


#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Ray {
    float ox, oy, oz;
    float dx, dy, dz;
};

struct vec3_cam {
    float x, y, z;
    vec3_cam(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {}
    vec3_cam operator+(const vec3_cam& v) const { return vec3_cam(x + v.x, y + v.y, z + v.z); }
    vec3_cam operator-(const vec3_cam& v) const { return vec3_cam(x - v.x, y - v.y, z - v.z); }
    vec3_cam operator*(float s) const { return vec3_cam(x * s, y * s, z * s); }
};

inline vec3_cam c_cross(vec3_cam a, vec3_cam b) {
    return vec3_cam(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline vec3_cam c_norm(vec3_cam v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return (len > 0) ? v * (1.0f / len) : v;
}

class Camera {
public:
    Camera() : width(0), height(0) {}

    void setup(int w_px, int h_px, vec3_cam _eye, vec3_cam _lookAt, vec3_cam _up, float _fov) {
        width = w_px;
        height = h_px;
        eye = _eye;

        float aspect = (float)width / (float)height;
        float theta = _fov * (float)M_PI / 180.0f;
        halfHeight = tan(theta / 2.0f);
        halfWidth = aspect * halfHeight;

        // Orthonormal Basis
        w = c_norm(eye - _lookAt);      // Backward
        u = c_norm(c_cross(_up, w));    // Right
        v = c_norm(c_cross(w, u));      // Up
    }

    void setup(int w_px, int h_px) {
        // Legacy scenes 1-7
        setup(w_px, h_px, vec3_cam(0, 0, 5), vec3_cam(0, 0, 0), vec3_cam(0, 1, 0), 45.0f);
    }

    Ray getRay(int i, int j) const {
        float x = (-1.0f + 2.0f * (i + 0.5f) / (float)width) * halfWidth;

        // --- THE FIX ---
        // Flip the vertical coordinate: 1.0 at j=0 (top), -1.0 at j=height (bottom)
        float y = (1.0f - 2.0f * (j + 0.5f) / (float)height) * halfHeight;

        vec3_cam dir = c_norm((u * x) + (v * y) - w);

        Ray r;
        r.ox = eye.x; r.oy = eye.y; r.oz = eye.z;
        r.dx = dir.x; r.dy = dir.y; r.dz = dir.z;
        return r;
    }

private:
    int width, height;
    float halfWidth, halfHeight;
    vec3_cam eye, u, v, w;
};

#endif