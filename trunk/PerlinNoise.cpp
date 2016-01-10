#ifndef PERLINNOISE__H
	#include "PerlinNoise.h"
#endif


int PerlinNoise::NumSegs = 8;
float PerlinNoise::Timer = 0.0f;
float PerlinNoise::Timer2 = 0.0f;
float PerlinNoise::Timer3 = 0.0f;
bool PerlinNoise::NewDisplayList = true;
bool PerlinNoise::NewDisplayList2 = true;
bool PerlinNoise::NewDisplayList3 = true;
GLuint PerlinNoise::DisplayList = 0;
GLuint PerlinNoise::DisplayList2 = 0;
GLuint PerlinNoise::DisplayList3 = 0;
bool PerlinNoise::SwapUV = false;
BuildNotifier PerlinNoise::OnBuild = NULL;

const int PerlinNoise::perm[256] = {151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};

/* These are Ken Perlin's proposed gradients for 3D noise. I kept them for
   better consistency with the reference implementation, but there is really
   no need to pad this to 16 gradients for this particular implementation.
   If only the "proper" first 12 gradients are used, they can be extracted
   from the grad4[][] array: grad3[i][j] == grad4[i*2][j], 0<=i<=11, j=0,1,2
*/
const int PerlinNoise::grad3[16][3] = {{0,1,1},{0,1,-1},{0,-1,1},{0,-1,-1},
				   {1,0,1},{1,0,-1},{-1,0,1},{-1,0,-1},
				   {1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0}, // 12 cube edges
				   {1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}}; // 4 more to make 16

/* These are my own proposed gradients for 4D noise. They are the coordinates
   of the midpoints of each of the 32 edges of a tesseract, just like the 3D
   noise gradients are the midpoints of the 12 edges of a cube.
*/
const int PerlinNoise::grad4[32][4] = {{0,1,1,1}, {0,1,1,-1}, {0,1,-1,1}, {0,1,-1,-1}, // 32 tesseract edges
				   {0,-1,1,1}, {0,-1,1,-1}, {0,-1,-1,1}, {0,-1,-1,-1},
				   {1,0,1,1}, {1,0,1,-1}, {1,0,-1,1}, {1,0,-1,-1},
				   {-1,0,1,1}, {-1,0,1,-1}, {-1,0,-1,1}, {-1,0,-1,-1},
				   {1,1,0,1}, {1,1,0,-1}, {1,-1,0,1}, {1,-1,0,-1},
				   {-1,1,0,1}, {-1,1,0,-1}, {-1,-1,0,1}, {-1,-1,0,-1},
				   {1,1,1,0}, {1,1,-1,0}, {1,-1,1,0}, {1,-1,-1,0},
				   {-1,1,1,0}, {-1,1,-1,0}, {-1,-1,1,0}, {-1,-1,-1,0}};
 
const float PerlinNoise::permTexUnit = 1.0f / 256.0f;        // Perm texture texel-size
const float PerlinNoise::permTexUnitHalf = 0.5f / 256.0f;    // Half perm texture texel-size

const float PerlinNoise::PI = 3.14159265f;
const float PerlinNoise::TWOPI = 6.28318531f;
const float PerlinNoise::BaseRadius = 0.6f;

const float PerlinNoise::NoiseScale = 2.0f;//2.3f;    // Noise scale, 0.01 > 8
const float PerlinNoise::Sharpness = 0.3f;    // Displacement 'sharpness', 0.1 > 5
const float PerlinNoise::Displacement = 0.6f; // Displcement amount, 0 > 2
const float PerlinNoise::Speed = 0.1f;        // Displacement rate, 0.01 > 1

const float PerlinNoise::grid = 0.01f;    // Grid offset for normal-estimation


PerlinNoise::PerlinNoise(void)
{
}


PerlinNoise::~PerlinNoise(void)
{
}


//////////////////////
// Public Functions //
//////////////////////

void PerlinNoise::DrawPotato(int num, float timer)
{
	switch (num)
	{
	case 0:
		if (timer != Timer)
			NewDisplayList = true;

		Timer = timer;
		break;
	case 1:
		if (timer != Timer2)
			NewDisplayList2 = true;

		Timer2 = timer;
		break;
	case 2:
		if (timer != Timer3)
			NewDisplayList3 = true;

		Timer3 = timer;
		break;
	}

	DrawPotato(num);
}


void PerlinNoise::DrawPotato(int num)
{
	GLuint displaylist = 0;
	bool newdisplaylist = true;
	float timer = 0.0f;
	
	switch (num)
	{
	case 0:
		displaylist = DisplayList;
		newdisplaylist = NewDisplayList;
		timer = Timer;
		break;
	case 1:
		displaylist = DisplayList2;
		newdisplaylist = NewDisplayList2;
		timer = Timer2;
		break;
	case 2:
		displaylist = DisplayList3;
		newdisplaylist = NewDisplayList3;
		timer = Timer3;
		break;
	}

	if (!newdisplaylist)
	{
		glCallList(displaylist);
		return;
	}

	if (displaylist > -1)
		glDeleteLists(displaylist, 1);

	displaylist = glGenLists(1);

	glNewList(displaylist, GL_COMPILE);

	int i, j;
	float x, y;
	float normal[3];
	float pos[4];

	float segs = (float) NumSegs;
	
	for(i = 0; i < NumSegs; i++)
	{
		glBegin(GL_QUAD_STRIP);

			for(j = 0; j <= NumSegs; j++)
			{
				x = ((float) i) / segs;
				y = ((float) j) / segs;
				
				x*= 1.001f; x-= 0.0022f;
				y*= 1.001f; y-= 0.0022f;

				GetVertPosition(x, y, timer, pos, normal);

				glNormal3f(normal[0], normal[1], normal[2]);
				//glColor4f(LastColour[0], LastColour[1], LastColour[2], LastColour[3]);
				glTexCoord2f(x, y);
				glVertex3f(pos[0], pos[1], pos[2]);

				x = ((float) (i + 1)) / segs;
				y = ((float) j) / segs;

				x*= 1.001f; x-= 0.0022f;
				y*= 1.001f; y-= 0.0022f;

				GetVertPosition(x, y, timer, pos, normal);

				glNormal3f(normal[0], normal[1], normal[2]);
				//glColor4f(LastColour[0], LastColour[1], LastColour[2], LastColour[3]);
				glTexCoord2f(x, y);
				glVertex3f(pos[0], pos[1], pos[2]);
			}

		glEnd();

		if (OnBuild)
			OnBuild();
	}
	
	glEndList();

	switch (num)
	{
	case 0:
		DisplayList = displaylist;
		NewDisplayList = false;
		break;
	case 1:
		DisplayList2 = displaylist;
		NewDisplayList2 = false;
		break;
	case 2:
		DisplayList3 = displaylist;
		NewDisplayList3 = false;
		break;
	}
}


void PerlinNoise::SaveObj(float timer, float scale, std::string filename)
{
	FILE* fp = NULL;

	errno_t e = fopen_s(&fp, filename.c_str(), "w");

	if (e == 0)
	{
		int i, j;
		float x, y;
		float normal[3];
		float pos[4];
		std::vector<float> tempvecf;
		std::vector<int> tempveci_pos;
		std::vector<int> tempveci_tex;
		std::vector<int> tempveci_norm;
		std::vector< std::vector<float> > vpos;
		std::vector< std::vector<float> > vtex;
		std::vector< std::vector<float> > vnorm;
		std::vector< std::vector<int> > vipos;
		std::vector< std::vector<int> > vitex;
		std::vector< std::vector<int> > vinorm;

		float segs = (float) NumSegs;

		// Define the verts first
		for (i = 0; i < NumSegs; i++)
		{
			for (j = 0; j < NumSegs; j++)
			{
				x = ((float) i) / segs;
				y = ((float) j) / segs;

				x*= 1.001f; x-= 0.0022f;
				y*= 1.001f; y-= 0.0022f;

				GetVertPosition(x, y, timer, pos, normal);

				tempvecf.clear();
				tempvecf.push_back(pos[0]*scale);tempvecf.push_back(-pos[1]*scale);tempvecf.push_back(pos[2]*scale);
				vpos.push_back(tempvecf);

				tempvecf.clear();
				tempvecf.push_back(x);tempvecf.push_back(y);
				vtex.push_back(tempvecf);

				tempvecf.clear();
				tempvecf.push_back(normal[2]);tempvecf.push_back(-normal[1]);tempvecf.push_back(normal[0]);
				vnorm.push_back(tempvecf);
			}

			// Keep same vert for pos (to weld seams) but keep separate texture coords
			j = NumSegs;

			x = ((float) i) / segs;
			y = ((float) j) / segs;

			x*= 1.001f; x-= 0.0022f;
			y*= 1.001f; y-= 0.0022f;

			tempvecf.clear();
			tempvecf.push_back(x);tempvecf.push_back(y);
			vtex.push_back(tempvecf);
		}
		
		// Now define the indices
		for (i = 0; i < (NumSegs-1); i++)
		{
			for (j = 0; j < (NumSegs-1); j++)
			{
				tempveci_pos.clear();
				tempveci_tex.clear();
				tempveci_norm.clear();

				tempveci_pos.push_back(1+j+(i*NumSegs));
				tempveci_tex.push_back(1+j+(i*(NumSegs+1)));
				tempveci_norm.push_back(1+j+(i*NumSegs));

				tempveci_pos.push_back(1+j+((i+1)*NumSegs));
				tempveci_tex.push_back(1+j+((i+1)*(NumSegs+1)));
				tempveci_norm.push_back(1+j+((i+1)*NumSegs));

				tempveci_pos.push_back(1+(j+1)+(i*NumSegs));
				tempveci_tex.push_back(1+(j+1)+(i*(NumSegs+1)));
				tempveci_norm.push_back(1+(j+1)+(i*NumSegs));

				vipos.push_back(tempveci_pos);
				vitex.push_back(tempveci_tex);
				vinorm.push_back(tempveci_norm);

				tempveci_pos.clear();
				tempveci_tex.clear();
				tempveci_norm.clear();

				tempveci_pos.push_back(1+(j+1)+(i*NumSegs));
				tempveci_tex.push_back(1+(j+1)+(i*(NumSegs+1)));
				tempveci_norm.push_back(1+(j+1)+(i*NumSegs));

				tempveci_pos.push_back(1+j+((i+1)*NumSegs));
				tempveci_tex.push_back(1+j+((i+1)*(NumSegs+1)));
				tempveci_norm.push_back(1+j+((i+1)*NumSegs));

				tempveci_pos.push_back(1+(j+1)+((i+1)*NumSegs));
				tempveci_tex.push_back(1+(j+1)+((i+1)*(NumSegs+1)));
				tempveci_norm.push_back(1+(j+1)+((i+1)*NumSegs));

				vipos.push_back(tempveci_pos);
				vitex.push_back(tempveci_tex);
				vinorm.push_back(tempveci_norm);
			}

			j = NumSegs - 1;

			tempveci_pos.clear();
			tempveci_tex.clear();
			tempveci_norm.clear();

			tempveci_pos.push_back(1+j+(i*NumSegs));
			tempveci_tex.push_back(1+j+(i*(NumSegs+1)));
			tempveci_norm.push_back(1+j+(i*NumSegs));

			tempveci_pos.push_back(1+j+((i+1)*NumSegs));
			tempveci_tex.push_back(1+j+((i+1)*(NumSegs+1)));
			tempveci_norm.push_back(1+j+((i+1)*NumSegs));

			tempveci_pos.push_back(1+(i*NumSegs));
			tempveci_tex.push_back(1+(j+1)+(i*(NumSegs+1)));
			tempveci_norm.push_back(1+(i*NumSegs));

			vipos.push_back(tempveci_pos);
			vitex.push_back(tempveci_tex);
			vinorm.push_back(tempveci_norm);

			tempveci_pos.clear();
			tempveci_tex.clear();
			tempveci_norm.clear();

			tempveci_pos.push_back(1+(i*NumSegs));
			tempveci_tex.push_back(1+(j+1)+(i*(NumSegs+1)));
			tempveci_norm.push_back(1+(i*NumSegs));

			tempveci_pos.push_back(1+j+((i+1)*NumSegs));
			tempveci_tex.push_back(1+j+((i+1)*(NumSegs+1)));
			tempveci_norm.push_back(1+j+((i+1)*NumSegs));

			tempveci_pos.push_back(1+(i+1)*NumSegs);
			tempveci_tex.push_back(1+(j+1)+(i+1)*(NumSegs+1));
			tempveci_norm.push_back(1+(i+1)*NumSegs);

			vipos.push_back(tempveci_pos);
			vitex.push_back(tempveci_tex);
			vinorm.push_back(tempveci_norm);
		}

		fprintf(fp, "############################################\n");
		fprintf(fp, "# Wavefront OBJ file\n");
		fprintf(fp, "# Filename: %s\n", filename.c_str());
		fprintf(fp, "############################################\n");
		fprintf(fp, "\n\n");

		// Vertex positions
		fprintf(fp, "# Verts\n");

		for (i=0;i<(int)vpos.size();i++)
		{
			tempvecf = vpos[i];

			if (tempvecf.size() > 2)
				fprintf(fp, "v %.6f %.6f %.6f\n",tempvecf[0],tempvecf[1],tempvecf[2]);
		}

		fprintf(fp, "# %i vertices\n\n",vpos.size());

		// Texture coordinates
		fprintf(fp, "# UVs\n");

		for (i=0;i<(int)vtex.size();i++)
		{
			tempvecf = vtex[i];

			if (tempvecf.size() > 1)
				fprintf(fp, "vt %.6f %.6f\n",tempvecf[0],tempvecf[1]);
		}

		fprintf(fp, "# %i texture vertices\n\n",vtex.size());

		// Normals
		fprintf(fp, "# Normals\n");

		for (i=0;i<(int)vnorm.size();i++)
		{
			tempvecf = vnorm[i];

			if (tempvecf.size() > 2)
				fprintf(fp, "vn %.6f %.6f %.6f\n",tempvecf[0],tempvecf[1],tempvecf[2]);
		}

		fprintf(fp, "# %i vertex normals\n\n",vnorm.size());

		// Face indices
		fprintf(fp, "# Faces\n");

		for (i=0;i<(int)vipos.size();i++)
		{
			if ((i < (int)vitex.size()) && (i < (int)vinorm.size()))
			{
				tempveci_pos = vipos[i];
				tempveci_tex = vitex[i];
				tempveci_norm = vinorm[i];

				if ((tempveci_pos.size() > 2) && (tempveci_tex.size() > 2) && (tempveci_norm.size() > 2))
					fprintf(fp, "f %i/%i/%i %i/%i/%i %i/%i/%i\n",tempveci_pos[0],tempveci_tex[0],tempveci_norm[0],tempveci_pos[1],tempveci_tex[1],tempveci_norm[1],tempveci_pos[2],tempveci_tex[2],tempveci_norm[2]);
			}
		}

		fprintf(fp, "# %i faces\n\n",vipos.size());

		fclose(fp);
	}
}


void PerlinNoise::GetVertPosition(float u, float v, float timer, float *pos, float *normal)
{
	if (pos == NULL)
		return;

	// Vertex position
	perlinSphere(u, v, timer, pos);

	//texture2DN(u, v, LastColour);

	if (normal == NULL)
		return;
	
	// Estimate normal by 'neighbour' technique
	// with thanks to tonfilm

	float tangent[3];
	float bitangent[3];
	
	perlinSphere(u + grid, v, timer, tangent);
	tangent[0]-= pos[0]; tangent[1]-= pos[1]; tangent[2]-= pos[2];

	perlinSphere(u, v + grid, timer, bitangent);
	bitangent[0]-= pos[0]; bitangent[1]-= pos[1]; bitangent[2]-= pos[2];

	cross(tangent, bitangent, normal);
	normalize(normal);
	float tempnorm = normal[0];
	normal[0] = -normal[2]; normal[1] = -normal[1]; normal[2] = -tempnorm;
}


////////////////////////////////
// Noise Calculation Function //
////////////////////////////////

float PerlinNoise::pnoise3D(float *p)
{
    float floorp[3];
	floorp[0] = floor(p[0]);
	floorp[1] = floor(p[1]);
	floorp[2] = floor(p[2]);

    float pi[3];
	pi[0] = permTexUnit*floorp[0]+permTexUnitHalf; // Integer part, scaled so +1 moves permTexUnit texel
	pi[1] = permTexUnit*floorp[1]+permTexUnitHalf;
	pi[2] = permTexUnit*floorp[2]+permTexUnitHalf;
	
    // and offset 1/2 texel to sample texel centers
    float pf[3];
	fract(p, pf); // Fractional part for interpolation
	//float intf;
	//pf[0] = modf(p[0],&intf);   // This doesn't wrap to negative numbers like GLSL fract() function 
	//pf[1] = modf(p[1],&intf);
	//pf[2] = modf(p[2],&intf);

	float grad000[3];
	float grad001[3];
	float grad010[3];
	float grad011[3];
	float grad100[3];
	float grad101[3];
	float grad110[3];
	float grad111[3];
	float pfTemp[3];

	float unitZ[3] = {0.0f, 0.0f, 1.0f};
	float unitY[3] = {0.0f, 1.0f, 0.0f};
	float unitYZ[3] = {0.0f, 1.0f, 1.0f};
	float unitX[3] = {1.0f, 0.0f, 0.0f};
	float unitXZ[3] = {1.0f, 0.0f, 1.0f};
	float unitXY[3] = {1.0f, 1.0f, 0.0f};
	float unitXYZ[3] = {1.0f, 1.0f, 1.0f};
 
    // Noise contributions from (x=0, y=0), z=0 and z=1
    float perm00 = texture2DN(pi[0], pi[1]);
    texture2DN(perm00, pi[2], grad000);
	scaleOffset(grad000, 4.0f, -1.0f);
    float n000 = dot(grad000, pf);
    texture2DN(perm00, pi[2] + permTexUnit, grad001);
	scaleOffset(grad001, 4.0f, -1.0f);
	sub(pf, unitZ, pfTemp);
    float n001 = dot(grad001, pfTemp);
 
    // Noise contributions from (x=0, y=1), z=0 and z=1
    float perm01 = texture2DN(pi[0], pi[1] + permTexUnit);
    texture2DN(perm01, pi[2], grad010);
	scaleOffset(grad010, 4.0f, -1.0f);
	sub(pf, unitY, pfTemp);
    float n010 = dot(grad010, pfTemp);
    texture2DN(perm01, pi[2] + permTexUnit, grad011);
	scaleOffset(grad011, 4.0f, -1.0f);
	sub(pf, unitYZ, pfTemp);
    float n011 = dot(grad011, pfTemp);
 
    // Noise contributions from (x=1, y=0), z=0 and z=1
    float perm10 = texture2DN(pi[0] + permTexUnit, pi[1]);
    texture2DN(perm10, pi[2], grad100);
	scaleOffset(grad100, 4.0f, -1.0f);
	sub(pf, unitX, pfTemp);
    float n100 = dot(grad100, pfTemp);
    texture2DN(perm10, pi[2] + permTexUnit, grad101);
	scaleOffset(grad101, 4.0f, -1.0f);
	sub(pf, unitXZ, pfTemp);
    float n101 = dot(grad101, pfTemp);
 
    // Noise contributions from (x=1, y=1), z=0 and z=1
    float perm11 = texture2DN(pi[0] + permTexUnit, pi[1] + permTexUnit);
    texture2DN(perm11, pi[2], grad110);
	scaleOffset(grad110, 4.0f, -1.0f);
	sub(pf, unitXY, pfTemp);
    float n110 = dot(grad110, pfTemp);
    texture2DN(perm11, pi[2] + permTexUnit, grad111);
	scaleOffset(grad111, 4.0f, -1.0f);
	sub(pf, unitXYZ, pfTemp);
    float n111 = dot(grad111, pfTemp);
 
	float n_x[4];
	float n_xy[2];

    // Blend contributions along x
	float nn1[4] = {n000, n001, n010, n011};
	float nn2[4] = {n100, n101, n110, n111};

    mix(nn1, nn2, fade(pf[0]), 4, n_x);	

	// Blend contributions along y
	float nn3[2] = {n_x[0], n_x[1]};
	float nn4[2] = {n_x[2], n_x[3]};
 
    mix(nn3, nn4, fade(pf[1]), 2, n_xy);
 
    // Blend contributions along z
    float n_xyz = mix(n_xy[0], n_xy[1], fade(pf[2]));
	
    // We're done, return the final noise value.
    return n_xyz;
}

 
/////////////////////
// Sphere Function //
/////////////////////
 
void PerlinNoise::sphere(float u, float v, float *pSphere)
{
	if (pSphere == NULL)
		return;

	u *= PI;
	v *= TWOPI;

	pSphere[2] = BaseRadius * cos(v) * sin(u);
	pSphere[1] = BaseRadius * sin(v) * sin(u);
	pSphere[0] = BaseRadius * cos(u);
	pSphere[3] = 1.0;
}


///////////////////////////
// Apply 3D Perlin Noise //
///////////////////////////

void PerlinNoise::perlinSphere(float u, float v, float timer, float *psphere)
{
	if (psphere == NULL)
		return;

	float sPoint[4];
	sphere(u, v, sPoint);
	
	// The rest of this function is mainly from vBomb shader from NVIDIA Shader Library
	float noisePos[3] = {(sPoint[0] + (Speed * timer)) * NoiseScale, (sPoint[1] + (Speed * timer)) * NoiseScale, (sPoint[2] + (Speed * timer)) * NoiseScale};
	
	float noise = (pnoise3D(noisePos) + 1.0f) * 0.5f;
	float ni = pow(abs(noise),Sharpness) - 0.25f;

	float nn[4];
	normalize(sPoint, nn);
	
	psphere[0] = (sPoint[0] - (nn[0] * (ni-0.5f) * Displacement));
	psphere[1] = (sPoint[1] - (nn[1] * (ni-0.5f) * Displacement));
	psphere[2] = (sPoint[2] - (nn[2] * (ni-0.5f) * Displacement));
	//psphere[3] = (sPoint[3] - (0.0f * (ni-0.5f) * Displacement));
}


///////////////////////
// Texture Functions //
///////////////////////

float PerlinNoise::texture2D(float x, float y)
{
	int lx, hx, ly, hy;
	float intx;
	float fracx, fracy;

	wrapUV(x, y, 256.0f);

	if (x < 0)
	{
		lx = (int) (floor(x) + 1.0f);
		hx = lx - 1;
		
		fracx = abs(modf(x, &intx));
	}
	else
	{
		lx = (int) floor(x);
		hx = lx + 1;
		
		fracx = abs(modf(x, &intx));
	}

	lx = lx % 256;
	hx = hx % 256;

	if (y < 0)
	{
		ly = (int) (floor(y) + 1.0f);
		hy = ly - 1;
		
		fracy = abs(modf(y, &intx));
	}
	else
	{
		ly = (int) floor(y);
		hy = ly + 1;
		
		fracy = abs(modf(y, &intx));
	}

	ly = ly % 256;
	hy = hy % 256;

	unsigned char vlxly = perm[(ly+perm[lx]) & 0xFF];
	unsigned char vlxhy = perm[(hy+perm[lx]) & 0xFF];
	unsigned char vhxly = perm[(ly+perm[hx]) & 0xFF];
	unsigned char vhxhy = perm[(hy+perm[hx]) & 0xFF];
	
	float flxly = ((float) vlxly) / 255.0f;
	float flxhy = ((float) vlxhy) / 255.0f;
	float fhxly = ((float) vhxly) / 255.0f;
	float fhxhy = ((float) vhxhy) / 255.0f;

	float fxly = mix(flxly, fhxly, fracx);
	float fxhy = mix(flxhy, fhxhy, fracx);

	return mix(fxly, fxhy, fracy);
}


void PerlinNoise::texture2D(float x, float y, float *rgb)
{
	int lx, hx, ly, hy;
	float intx;
	float fracx, fracy;

	wrapUV(x, y, 256.0f);

	if (x < 0)
	{
		lx = (int) (floor(x) + 1.0f);
		hx = lx - 1;
		
		fracx = abs(modf(x, &intx));
	}
	else
	{
		lx = (int) floor(x);
		hx = lx + 1;
		
		fracx = abs(modf(x, &intx));
	}

	lx = lx % 256;
	hx = hx % 256;

	if (y < 0)
	{
		ly = (int) (floor(y) + 1.0f);
		hy = ly - 1;
		
		fracy = abs(modf(y, &intx));
	}
	else
	{
		ly = (int) floor(y);
		hy = ly + 1;
		
		fracy = abs(modf(y, &intx));
	}

	ly = ly % 256;
	hy = hy % 256;

	unsigned char vlxly = perm[(ly+perm[lx]) & 0xFF];
	unsigned char vlxhy = perm[(hy+perm[lx]) & 0xFF];
	unsigned char vhxly = perm[(ly+perm[hx]) & 0xFF];
	unsigned char vhxhy = perm[(hy+perm[hx]) & 0xFF];

	unsigned char rlxly = grad3[vlxly & 0x0F][0] * 64 + 64;   // Gradient x
	unsigned char rlxhy = grad3[vlxhy & 0x0F][0] * 64 + 64;   // Gradient x
	unsigned char rhxly = grad3[vhxly & 0x0F][0] * 64 + 64;   // Gradient x
	unsigned char rhxhy = grad3[vhxhy & 0x0F][0] * 64 + 64;   // Gradient x

	unsigned char glxly = grad3[vlxly & 0x0F][1] * 64 + 64;   // Gradient y
	unsigned char glxhy = grad3[vlxhy & 0x0F][1] * 64 + 64;   // Gradient y
	unsigned char ghxly = grad3[vhxly & 0x0F][1] * 64 + 64;   // Gradient y
	unsigned char ghxhy = grad3[vhxhy & 0x0F][1] * 64 + 64;   // Gradient y

	unsigned char blxly = grad3[vlxly & 0x0F][2] * 64 + 64;   // Gradient z
	unsigned char blxhy = grad3[vlxhy & 0x0F][2] * 64 + 64;   // Gradient z
	unsigned char bhxly = grad3[vhxly & 0x0F][2] * 64 + 64;   // Gradient z
	unsigned char bhxhy = grad3[vhxhy & 0x0F][2] * 64 + 64;   // Gradient z
	
	float frlxly = ((float) rlxly) / 255.0f;
	float frlxhy = ((float) rlxhy) / 255.0f;
	float frhxly = ((float) rhxly) / 255.0f;
	float frhxhy = ((float) rhxhy) / 255.0f;
	
	float fglxly = ((float) glxly) / 255.0f;
	float fglxhy = ((float) glxhy) / 255.0f;
	float fghxly = ((float) ghxly) / 255.0f;
	float fghxhy = ((float) ghxhy) / 255.0f;
	
	float fblxly = ((float) blxly) / 255.0f;
	float fblxhy = ((float) blxhy) / 255.0f;
	float fbhxly = ((float) bhxly) / 255.0f;
	float fbhxhy = ((float) bhxhy) / 255.0f;

	float frxly = mix(frlxly, frhxly, fracx);
	float frxhy = mix(frlxhy, frhxhy, fracx);

	float fgxly = mix(fglxly, fghxly, fracx);
	float fgxhy = mix(fglxhy, fghxhy, fracx);

	float fbxly = mix(fblxly, fbhxly, fracx);
	float fbxhy = mix(fblxhy, fbhxhy, fracx);

	rgb[0] = mix(frxly, frxhy, fracy);
	rgb[1] = mix(fgxly, fgxhy, fracy);
	rgb[2] = mix(fbxly, fbxhy, fracy);
}


float PerlinNoise::texture2DN(float x, float y)
{
	int lx, hx, ly, hy;
	float intx;
	float fracx, fracy;

	int truex, truey;

	wrapUV(x, y, 256.0f);

	if (x < 0)
	{
		lx = (int) (floor(x) + 20.0f);
		hx = lx - 1;
		
		fracx = abs(modf(x, &intx));
	}
	else
	{
		lx = (int) floor(x);
		hx = lx + 1;
		
		fracx = x - floor(x);//abs(modf(x, &intx));
	}
	
	if (fracx < 0.5f)
		truex = lx;
	else
		truex = hx;

	truex = truex % 256;

	if (y < 0)
	{
		ly = (int) (floor(y) + 1.0f);
		hy = ly - 1;
		
		fracy = abs(modf(y, &intx));
	}
	else
	{
		ly = (int) floor(y);
		hy = ly + 1;
		
		fracy = y - floor(y);//abs(modf(y, &intx));
	}
	
	if (fracy < 0.5f)
		truey = ly;
	else
		truey = hy;

	truey = truey % 256;

	unsigned char v = perm[(truex+perm[truey]) & 0xFF];
	
	return (((float) v) / 255.0f);
}


void PerlinNoise::texture2DN(float x, float y, float *rgb)
{
	int lx, hx, ly, hy;
	float intx;
	float fracx, fracy;

	int truex, truey;

	wrapUV(x, y, 256.0f);

	if (x < 0)
	{
		lx = (int) (floor(x) + 1.0f);
		hx = lx - 1;
		
		fracx = abs(modf(x, &intx));
	}
	else
	{
		lx = (int) floor(x);
		hx = lx + 1;
		
		fracx = x - floor(x);//abs(modf(x, &intx));
	}
	
	if (fracx < 0.5f)
		truex = lx;
	else
		truex = hx;

	truex = truex % 256;

	if (y < 0)
	{
		ly = (int) (floor(y) + 1.0f);
		hy = ly - 1;
		
		fracy = abs(modf(y, &intx));
	}
	else
	{
		ly = (int) floor(y);
		hy = ly + 1;
		
		fracy = y - floor(y); //abs(modf(y, &intx));
	}
	
	if (fracy < 0.5f)
		truey = ly;
	else
		truey = hy;

	truey = truey % 256;

	unsigned char v = perm[(truex+perm[truey]) & 0xFF];

	unsigned char r = grad3[v & 0x0F][0] * 64 + 64;   // Gradient x
	unsigned char g = grad3[v & 0x0F][1] * 64 + 64;   // Gradient y
	unsigned char b = grad3[v & 0x0F][2] * 64 + 64;   // Gradient z
	
	rgb[0] = ((float) r) / 255.0f;
	rgb[1] = ((float) g) / 255.0f;
	rgb[2] = ((float) b) / 255.0f;
}


///////////////////////////////
// Private Utility Functions //
///////////////////////////////

float PerlinNoise::fade(float t)
{
    return t*t*t*(t*(t*6.0f-15.0f)+10.0f);
}


float PerlinNoise::fract(float v)
{
	return (v - floor(v));
}


void PerlinNoise::fract(float *p, float *res)
{
	if ((p == NULL) || (res == NULL))
		return;

	res[0] = fract(p[0]);
	res[1] = fract(p[1]);
	res[2] = fract(p[2]);
}


void PerlinNoise::normalize(float *pos)
{
	if (pos == NULL)
		return;

	float len = (pos[0] * pos[0]) + (pos[1] * pos[1]) + (pos[2] * pos[2]);
	len = sqrt(len);

	pos[0]/= len;
	pos[1]/= len;
	pos[2]/= len;
}


void PerlinNoise::normalize(float *pos, float *res)
{
	if ((pos == NULL) || (res == NULL))
		return;

	float len = (pos[0] * pos[0]) + (pos[1] * pos[1]) + (pos[2] * pos[2]);
	len = sqrt(len);

	res[0] = pos[0] / len;
	res[1] = pos[1] / len;
	res[2] = pos[2] / len;
}


float PerlinNoise::dot(float *p1, float *p2)
{
	if ((p1 == NULL) || (p2 == NULL))
		return 0.0f;

	return ((p1[0] * p2[0]) + (p1[1] * p2[1]) + (p1[2] * p2[2]));
}


void PerlinNoise::cross(float *p1, float *p2, float *res)
{
	if ((p1 == NULL) || (p2 == NULL) || (res == NULL))
		return;

	res[0] = (p1[1] * p2[2]) - (p1[2] * p2[1]);
	res[1] = (p1[2] * p2[0]) - (p1[0] * p2[2]);
	res[2] = (p1[0] * p2[1]) - (p1[1] * p2[0]);
}


void PerlinNoise::sub(float *p1, float *p2, float *res)
{
	if ((p1 == NULL) || (p2 == NULL) || (res == NULL))
		return;

	res[0] = p1[0] - p2[0];
	res[1] = p1[1] - p2[1];
	res[2] = p1[2] - p2[2];
}


void PerlinNoise::scaleOffset(float *p, float scale, float offset)
{
	if (p == NULL)
		return;

	p[0] = (p[0] * scale) + offset;
	p[1] = (p[1] * scale) + offset;
	p[2] = (p[2] * scale) + offset;
}

float PerlinNoise::mix(float v1, float v2, float a)
{
	return ((v1 * (1.0f - a)) + (v2 * a));
}

void PerlinNoise::mix(float *p1, float *p2, float a, int n, float *res)
{
	if ((p1 == NULL) || (p2 == NULL) || (res == NULL))
		return;

	int counter1;
	for (counter1=0;counter1<n;counter1++)
	{
		res[counter1] = ((p1[counter1] * (1.0f - a)) + (p2[counter1] * a));
	}
}

float PerlinNoise::clamp(float v, float minv, float maxv)
{
	if (v < minv)
		return minv;

	if (v > maxv)
		return maxv;

	return v;
}


void PerlinNoise::wrapUV(float &u, float &v, float texsize)
{
	// U and V should be wrapped to 0->1 space,
	// then scaled back up to texsize

	u = (u - floor(u)) * texsize;
	v = (v - floor(v)) * texsize;
	u-= 0.5f;
	v-= 0.5f;
}


void PerlinNoise::copy(float *p, float *res, int n)
{
	if ((p == NULL) || (res == NULL))
		return;

	int counter1;

	for (counter1=0;counter1<n;counter1++)
	{
		res[counter1] = p[counter1];
	}
}