// simple specular vertex shader

// These are values written by this vertex shader
// and passed to the fragment shader
varying vec3 normal;
varying vec4 pos;
varying vec2 uv;
varying float onRight;

uniform sampler2D permTexture;          // Permutation texture
const float permTexUnit = 1.0/256.0;        // Perm texture texel-size
const float permTexUnitHalf = 0.5/256.0;    // Half perm texture texel-size
 
float fade(in float t)
{
    return t*t*t*(t*(t*6.0-15.0)+10.0);
}
 
float pnoise3D(in vec3 p)
{
    vec3 pi = permTexUnit*floor(p)+permTexUnitHalf; // Integer part, scaled so +1 moves permTexUnit texel
    // and offset 1/2 texel to sample texel centers
    
    //vec3 pf = vec3(p[0] - floor(p[0]), p[1] - floor(p[1]), p[2] - floor(p[2]));
    vec3 pf = fract(p);     // Fractional part for interpolation
	
	// Noise contributions from (x=0, y=0), z=0 and z=1
    float perm00 = texture2D(permTexture, pi.xy).a ;
	vec3  grad000 = texture2D(permTexture, vec2(perm00, pi.z)).rgb * 4.0 - 1.0;
    float n000 = dot(grad000, pf);
    vec3  grad001 = texture2D(permTexture, vec2(perm00, pi.z + permTexUnit)).rgb * 4.0 - 1.0;
    float n001 = dot(grad001, pf - vec3(0.0, 0.0, 1.0));
 
    // Noise contributions from (x=0, y=1), z=0 and z=1
    float perm01 = texture2D(permTexture, pi.xy + vec2(0.0, permTexUnit)).a ;
    vec3  grad010 = texture2D(permTexture, vec2(perm01, pi.z)).rgb * 4.0 - 1.0;
    float n010 = dot(grad010, pf - vec3(0.0, 1.0, 0.0));
    vec3  grad011 = texture2D(permTexture, vec2(perm01, pi.z + permTexUnit)).rgb * 4.0 - 1.0;
    float n011 = dot(grad011, pf - vec3(0.0, 1.0, 1.0));
 
    // Noise contributions from (x=1, y=0), z=0 and z=1
    float perm10 = texture2D(permTexture, pi.xy + vec2(permTexUnit, 0.0)).a ;
    vec3  grad100 = texture2D(permTexture, vec2(perm10, pi.z)).rgb * 4.0 - 1.0;
    float n100 = dot(grad100, pf - vec3(1.0, 0.0, 0.0));
    vec3  grad101 = texture2D(permTexture, vec2(perm10, pi.z + permTexUnit)).rgb * 4.0 - 1.0;
    float n101 = dot(grad101, pf - vec3(1.0, 0.0, 1.0));
 
    // Noise contributions from (x=1, y=1), z=0 and z=1
    float perm11 = texture2D(permTexture, pi.xy + vec2(permTexUnit, permTexUnit)).a ;
    vec3  grad110 = texture2D(permTexture, vec2(perm11, pi.z)).rgb * 4.0 - 1.0;
    float n110 = dot(grad110, pf - vec3(1.0, 1.0, 0.0));
    vec3  grad111 = texture2D(permTexture, vec2(perm11, pi.z + permTexUnit)).rgb * 4.0 - 1.0;
    float n111 = dot(grad111, pf - vec3(1.0, 1.0, 1.0));
 
    // Blend contributions along x
    vec4 n_x = mix(vec4(n000, n001, n010, n011),
            vec4(n100, n101, n110, n111), fade(pf.x));
    
    // Blend contributions along y
    vec2 n_xy = mix(n_x.xy, n_x.zw, fade(pf.y));
 
    // Blend contributions along z
    float n_xyz = mix(n_xy.x, n_xy.y, fade(pf.z));
    
    // We're done, return the final noise value.
    return n_xyz;
}
 
/////////////////////
// Sphere Function //
/////////////////////
 
const float PI = 3.14159265;
const float TWOPI = 6.28318531;
const float BaseRadius = 0.6;
 
vec4 sphere(in float u, in float v)
{
	u *= PI;
	v *= TWOPI;
	vec4 pSphere;
	pSphere.z = BaseRadius * cos(v) * sin(u);
	pSphere.y = BaseRadius * sin(v) * sin(u);
	pSphere.x = BaseRadius * cos(u);
	pSphere.w = 1.0;
	return pSphere;
}
 
///////////////////////////
// Apply 3D Perlin Noise //
///////////////////////////

const vec3 NoiseScale = vec3(2.0,2.0,2.0);    // Noise scale, 0.01 > 8
const float Sharpness = 0.3;    // Displacement 'sharpness', 0.1 > 5
const float Displacement = 0.6; // Displcement amount, 0 > 2
const float Speed = 0.1;        // Displacement rate, 0.01 > 1
uniform float Timer;        // Feed incrementing value, infinite
uniform float SwapLeftRight; // Whether to left / right swapped
//uniform float UOffset;

vec4 perlinSphere(in float u, in float v)
{	
	vec4 sPoint = sphere(u, v);
	
	// The rest of this function is mainly from vBomb shader from NVIDIA Shader Library
	
	float timer = Timer;
	vec4 noisePos = vec4(NoiseScale.xyz,1.0) * (sPoint + (Speed * timer));
	
	float noise = (pnoise3D(noisePos.xyz) + 1.0) * 0.5;
	float ni = pow(abs(noise),Sharpness) - 0.25;
	vec4 nn = vec4(normalize(sPoint.xyz),0.0);
	
	return (sPoint - (nn * (ni-0.5) * Displacement));
}
 
////////////////////////////////
// Calculate Position, Normal //
////////////////////////////////
 
const float grid = 0.01;    // Grid offset for normal-estimation
 
vec4 posNorm(in float u, in float v)
{
	// Vertex position
	vec4 vPosition = perlinSphere(u, v);
	
	// Estimate normal by 'neighbour' technique
	// with thanks to tonfilm
	
	vec3 tangent = (perlinSphere(u + grid, v) - vPosition).xyz;
	vec3 bitangent = (perlinSphere(u, v + grid) - vPosition).xyz;
	normal = gl_NormalMatrix * normalize(cross(tangent, bitangent));
	normal = vec3(-normal.z, -normal.y, -normal.x);
	
	// Return vertex position
	return vPosition;
}


void main()
{
	uv = gl_MultiTexCoord0.xy;
	
	onRight = step(2.0, uv[0]);
	uv[0] = uv[0] - (onRight * 3.0);
 	
	// Offset XY mesh coords to 0 > 1 range
	//uv += 0.5;
	
	// Pre-scale and transform mesh
	//uv *= 0.999;
	//uv+= 0.1;
	uv*= 1.001;
	uv-= 0.0022;

	// Calculate new vertex position and normal
	//vec4 spherePos = gl_Vertex;
	vec4 spherePos = posNorm(uv[0], uv[1]);
	
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	
	gl_Position = gl_ModelViewProjectionMatrix * spherePos;//ftransform();
}
