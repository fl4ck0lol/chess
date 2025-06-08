#include "raylib.h"
#include "math.h"
#include <stdlib.h>

#define M_PI 3.14159265358979323846

typedef struct vec2
{
	float x, y;
}vec2;

typedef struct vec3
{
	float x, y, z;
}vec3;

typedef struct CubeData
{
	vec3* vertices;
	unsigned short* indices;
}CubeData;

typedef struct SimulatedCamera
{
	float fov, aspect, near, far;
}SimulatedCamera;

SimulatedCamera* createCamera(float fov, float aspect, float near, float far)
{
	SimulatedCamera* sc = (SimulatedCamera*)malloc(sizeof(SimulatedCamera));

	sc->fov = fov;
	sc->aspect = aspect;
	sc->near = near;
	sc->far = far;

	return sc;
}


CubeData* createCubeData(vec3* vertices, unsigned short* indices)
{
	CubeData* cd = (CubeData*)malloc(sizeof(CubeData));

	cd->vertices = vertices;
	cd->indices = indices;

	return cd;
}

vec3 rotatePointByX(vec3 point, float angle)
{
    angle *= M_PI / 180.f;

    float s = sinf(angle);
    float c = cosf(angle);

    return (vec3){point.x, point.y * c - point.z * s, point.y * s + point.z * c};
}

void rotateAllByX(vec3* vertices, float angle)
{
    for (int i = 0; i < 8; i++)
        vertices[i] = rotatePointByX(vertices[i], angle);
}

vec3 rotatePointByY(vec3 point, float angle)
{
	angle *= M_PI / 180.f;

	float s = sinf(angle);
	float c = cosf(angle);

	return (vec3){point.x * c - point.z * s, point.y, point.x * s + point.z * c};
}

void rotateAllByY(vec3* vertices, float angle)
{
	for(int i = 0; i < 8; i++)
		vertices[i] = rotatePointByY(vertices[i], angle);
}

vec3 rotatePointByZ(vec3 point, float angle)
{
    angle *= M_PI / 180.f;

    float s = sinf(angle);
    float c = cosf(angle);

    return (vec3){point.x * c - point.y * s, point.x * s + point.y * c, point.z};
}

void rotateAllByZ(vec3* vertices, float angle)
{
    for (int i = 0; i < 8; i++)
        vertices[i] = rotatePointByZ(vertices[i], angle);
}

void move(CubeData* cd, float angle)
{
	rotateAllByX(cd->vertices, angle);
	rotateAllByY(cd->vertices, angle);
	rotateAllByZ(cd->vertices, angle);
}

vec2 project(vec3 point, SimulatedCamera* sc)
{
	float f = 1.f / tanf(sc->fov / 2.f);

	return (vec2){(point.x * f / point.z) * sc->aspect, point.y * f / point.z};
}