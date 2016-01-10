#ifndef PERLINNOISE__H
#define PERLINNOISE__H


#include <GL/glew.h>
#include <GL/glut.h>

#include <cstdio>
#include <math.h>
#include <string>
#include <vector>
#include <process.h>
#include <windows.h>


typedef void(*BuildNotifier)(void);


class PerlinNoise
{
  public:
	PerlinNoise(void);

	~PerlinNoise(void);

  public:

	static void DrawPotato(int num, float timer);

	static void DrawPotato(int num);

	static void SaveObj(float timer, float scale, std::string filename);

  private:

	static void GetVertPosition(float u, float v, float timer, float *pos, float *normal);
	
	static float pnoise3D(float *p);

	static void sphere(float u, float v, float *pSphere);

	static void perlinSphere(float u, float v, float timer, float *pos);

  private:

	static float texture2D(float x, float y); // Just grab alpha component

	static void texture2D(float x, float y, float *rgb);

	static float texture2DN(float x, float y); // Just grab alpha component

	static void texture2DN(float x, float y, float *rgb);

  private:

	static float fade(float t);

	static float fract(float v);
	
	static void fract(float *p, float *res);

	static void normalize(float *pos);

	static void normalize(float *pos, float *res);

	static float dot(float *p1, float *p2);

	static void cross(float *p1, float *p2, float *res);

	static void sub(float *p1, float *p2, float *res);

	static void scaleOffset(float *p, float scale, float offset);

	static float mix(float v1, float v2, float a);

	static void mix(float *p1, float *p2, float a, int n, float *res);

	static float clamp(float v, float minv, float maxv);

	static void wrapUV(float &u, float &v, float texsize);

	static void copy(float *p, float *res, int n);

  public:

	static int NumSegs;
	static float Timer;
	static float Timer2;
	static float Timer3;
	static bool NewDisplayList;
	static bool NewDisplayList2;
	static bool NewDisplayList3;
	static GLuint DisplayList;
	static GLuint DisplayList2;
	static GLuint DisplayList3;
	static bool SwapUV;
	static BuildNotifier OnBuild;

  private:

	const static int perm[256];
	const static int grad3[16][3];
	const static int grad4[32][4];

	const static float permTexUnit;
	const static float permTexUnitHalf;
	const static float PI;
	const static float TWOPI;
	const static float BaseRadius;

	const static float NoiseScale; // Noise scale, 0.01 > 8
	const static float Sharpness;    // Displacement 'sharpness', 0.1 > 5
	const static float Displacement; // Displcement amount, 0 > 2
	const static float Speed;        // Displacement rate, 0.01 > 1

	const static float grid;    // Grid offset for normal-estimation
};


#endif
