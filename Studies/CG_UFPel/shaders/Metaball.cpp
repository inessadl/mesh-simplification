uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iGlobalTime;           // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform int       iFrame;                // shader playback frame
uniform float     iChannelTime[4];       // channel playback time (in seconds)
uniform vec3      iChannelResolution[4]; // channel resolution (in pixels)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform samplerXX iChannel0..3;          // input channel. XX = 2D/Cube
uniform vec4      iDate;                 // (year, month, day, time in seconds)
uniform float     iSampleRate;           // sound sample rate (i.e., 44100)

/*
"Sine metaballs" by Emmanuel Keller aka Tambako - December 2015
License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
Contact: tamby@tambako.ch
*/

#define pi 3.141593

struct Lamp
{
  vec3 position;
  vec3 color;
  float intensity;
  float attenuation;
};

struct RenderData
{
  vec3 col;
  vec3 pos;
  vec3 norm;
  int objnr;
};

const int nbBalls = 10;
const float bSize0 = 1.;
const float btShift = 0.26;
const float btSFact = 0.94;
const float bmovamp = 1.8;

Lamp lamps[3];

vec3 campos = vec3(0., 3., 15.);
vec3 camdir = vec3(0., -0.29, -1.);
float fov = 2.8;

const vec3 ambientColor = vec3(0.1, 0.4, 0.9);
const float ambientint = 0.17;

vec3 colors[3];

//#define shadow
#define floorref
#define ambocc
#define specular
const float specint = 0.27;
const float specshin = 11.;
const float aoint = 1.5;
const float shi = 0.8;
const float shf = 0.9;

const float normdelta = 0.002;
const float maxdist = 80.;

const float flpos = 3.2;

vec4 hex;
const float hexts = 1.9;
const float hexth = 0.1;
const float hexbw = 0.15;

// Antialias. Change from 1 to 2 or more AT YOUR OWN RISK! It may CRASH your browser while compiling!
const float aawidth = 0.8;
const int aasamples = 1;

vec2 rotateVec(vec2 vect, float angle)
{
    vec2 rv;
    rv.x = vect.x*cos(angle) + vect.y*sin(angle);
    rv.y = vect.x*sin(angle) - vect.y*cos(angle);
    return rv;
}

// From https://www.shadertoy.com/view/Xd2GR3
vec4 hexagon(vec2 p)
{
	vec2 q = vec2( p.x*2.0*0.5773503, p.y + p.x*0.5773503 );

	vec2 ppi = floor(q);
	vec2 pf = fract(q);

	float v = mod(ppi.x + ppi.y, 3.0);

	float ca = step(1.0,v);
	float cb = step(2.0,v);
	vec2  ma = step(pf.xy,pf.yx);

    // distance to borders
	float e = dot( ma, 1.0-pf.yx + ca*(pf.x+pf.y-1.0) + cb*(pf.yx-2.0*pf.xy) );

	// distance to center
	p = vec2( q.x + floor(0.5+p.y/1.5), 4.0*p.y/3.0 )*0.5 + 0.5;
	float f = length( (fract(p) - 0.5)*vec2(1.0,0.85) );

	return vec4( ppi + ca - cb*ma, e, f );
}

vec3 getHexagonColor(vec4 hex, float nbcols, vec3 pos)
{
    colors[0] = vec3(.95, .88, .75);
    colors[1] = vec3(.6, .7, .8);
    colors[2] = vec3(.75, .95, .3);

    int colnr = int(mod(hex.x, nbcols));
    /*vec3 color = vec3(mod(colnr, 3.)==0., mod(colnr, 3.)==1., mod(colnr, 3.)==2.);
    if (colnr>2.)
        color = vec3(1.) - color;

    vec3 color2 = vec3(0.5*color.r + 0.3*color.g + 0.7*color.b,
                       0.3*color.r + 0.7*color.g + 0.4*color.b,
                       0.8*color.r + 0.5*color.g + 0.3*color.b);*/

    vec3 color2 = colnr==0?colors[0]*(0.25 + 0.75*texture2D(iChannel0, 0.2*pos.xz).rgb):(colnr==1?
                           colors[1]*(0.25 + 0.75*texture2D(iChannel1, 0.2*pos.xz).rgb):
                           colors[2]*(0.25 + 0.75*texture2D(iChannel2, 0.2*pos.xz).rgb));

    return color2;
}

float getHexagonHeight(vec4 hex, float borderw)
{
    return -1. + 2.*smoothstep(-borderw, borderw, hex.z);
    //return -2.5 + 2.*smoothstep(-borderw, borderw, hex.z) + 0.5*pow(smoothstep(0., borderw*2.7, hex.w), 2.);
}

vec3 getObjPosition(float time)
{
   float x = sin(time*1.05);
   float y = .8*sin(time);
   float z = sin(time*1.15 + radians(90.));

   x+= .2*sin(time*2.5 + radians(45.));
   y+= .4*sin(time*1.5 + radians(20.));
   z+= .3*sin(time*2.2);

   x+= .03*sin(time*4.5 + radians(10.));
   y+= .05*sin(time*4.2 + radians(90.));
   z+= .07*sin(time*5.5);

   return vec3(x, y, z);
}

float floorm(vec3 pos)
{
  	hex = hexagon(pos.xz/hexts);
   	float hexy = hex.x-2.*hex.y;
   	float h = hexth*(getHexagonHeight(hex, hexbw) - 1.);
    return hexy<32.?pos.y + flpos - h:500.;
}

// From https://www.shadertoy.com/view/ld2GRz
float sdMetaBalls(vec3 pos, bool nofloor)
{
	float m = 0.0;
	float p = 0.0;
	float dmin = 1e20;

	float h = 1.0; // track Lipschitz constant

	for( int i=0; i<nbBalls; i++ )
	{
		// bounding sphere for ball
        float fact = pow(btSFact, float(i));
        vec3 oPos = bmovamp*getObjPosition(iGlobalTime - float(i)*btShift*fact);
        float db = distance( oPos , pos );
        float size = bSize0*fact;
        if( db < size )
    	{
    		float x = db/size;
    		p += 1.0 - x*x*x*(x*(x*6.0-15.0)+10.0);
	    	m += 1.0;
    		h = max( h, 0.5333*size );
	    }
	    else // bouncing sphere distance
	    {
    		dmin = min( dmin, db - size );
    	}
	}
    float d = dmin + 0.1;

	if( m>0.5 )
	{
		float th = 0.2;
		d = h*(th-p);
	}

	return min(d, nofloor?100.:floorm(pos));
}

float map(vec3 pos, bool nofloor)
{
    return sdMetaBalls(pos, nofloor);

    float d = 0.;
    for (int b=0; b<nbBalls; b++)
    {
        vec3 oPos = bmovamp*getObjPosition(iGlobalTime - float(b)*btShift); //iGlobalTime
        float size = (bSize0 - 1.)*pow(btSFact, float(b));
        d+= size/dot(pos - oPos, pos - oPos);
    }
    return min(1. - d, nofloor?100.:floorm(pos));
}

float trace(vec3 cam, vec3 ray, float maxdist, bool nofloor)
{
    float t = 2.;
  	for (int i = 0; i < 75; ++i)
    {
    	vec3 pos = ray*t + cam;
    	float dist = map(pos, nofloor);
        if (dist>maxdist || abs(dist)<0.02)
            break;
        t+= dist*(t>20.?1.:0.6);
  	}
  	return t;
}

// From https://www.shadertoy.com/view/ld2GRz
vec3 norMetaBalls(vec3 pos, float e)
{
	vec3 nor = vec3( 0.0, e, 0.0 );

	for( int i=0; i<nbBalls; i++ )
	{
        float fact = pow(btSFact, float(i));
        vec3 oPos = bmovamp*getObjPosition(iGlobalTime - float(i)*btShift*fact);
        float db = distance( oPos , pos );
        float size = bSize0*fact;
		float x = clamp( db/size, 0.0, 1.0 );
		float p = x*x*(30.0*x*x - 60.0*x + 30.0);
		nor += normalize( pos - oPos ) * p / size;
	}

	return normalize( nor );
}

// From https://www.shadertoy.com/view/MstGDM
vec3 getNormal(vec3 pos, float e)
{
    if (pos.y>-flpos + 0.1)
        return norMetaBalls(pos, e);

    vec2 q = vec2(0, e);
    return normalize(vec3(floorm(pos + q.yxx) - floorm(pos - q.yxx),
                          floorm(pos + q.xyx) - floorm(pos - q.xyx),
                          floorm(pos + q.xxy) - floorm(pos - q.xxy)));
}

vec3 floor_color(vec3 pos)
{
    return getHexagonColor(hex, 3., pos);
}

vec3 obj_color(vec3 norm, vec3 pos)
{
    float cp = dot(cross(norm.xyz, norm.zxy), norm.yzx) - 0.2;
    return mix(vec3(1., 1., 0.), vec3(1., 0., 1.), cp);
}

vec3 sky_color(vec3 ray)
{
    return ambientColor + vec3(0.4, 0.3, 0.05)*2.8*(0.53-atan(ray.y));
}

// From https://www.shadertoy.com/view/Xds3zN;
float softshadow(vec3 ro, vec3 rd, float mint, float tmax)
{
	float res = 1.0;
    float t = mint;
    for(int i=0; i<16; i++)
    {
    	float h = map(ro + rd*t, true);
        res = min( res, 8.0*h/t );
        t += clamp( h, 0.02, 0.10 );
        if( h<0.001 || t>tmax ) break;
    }
    return clamp( res, 0.0, 1.0 );
}

float hash( float n ){
	return fract(sin(n)*3538.5453);
}

float calcAO( in vec3 p, in vec3 n, float maxDist, float falloff ){
	float ao = 0.0;
	const int nbIte = 5;
	for( int i=0; i<nbIte; i++ )
	{
		float l = hash(float(i))*maxDist;
		vec3 rd = n*l;
		ao += (l - map( p + rd, false )) / pow(1.+l, falloff);
	}
	return clamp( 1.35*(1.-ao/float(nbIte)), 0., 1.);
}

// Fresnel reflectance factor through Schlick's approximation: https://en.wikipedia.org/wiki/Schlick's_approximation
float fresnel(vec3 ray, vec3 norm, float n2)
{
   float n1 = 1.; // air
   float angle = acos(-dot(ray, norm));
   float r0 = dot((n1-n2)/(n1+n2), (n1-n2)/(n1+n2));
   float r = r0 + (1. - r0)*pow(1. - cos(angle), 5.);
   return r;
}

vec3 lampShading(Lamp lamp, vec3 norm, vec3 pos, vec3 ocol)
{
	vec3 pl = normalize(lamp.position - pos);
    float dlp = distance(lamp.position, pos);
    vec3 pli = pl/pow(1. + lamp.attenuation*dlp, 2.);

    // Diffuse shading
    vec3 col = ocol*normalize(lamp.color)*lamp.intensity*clamp(dot(norm, pli), 0., 1.);

    // Specular shading
    #ifdef specular
    if (dot(norm, lamp.position - pos) > 0.0)
        col+= vec3(1., 0.7, 0.3)*normalize(lamp.color)*lamp.intensity*specint*pow(max(0.0, dot(reflect(pl, norm), normalize(pos - campos))), specshin);
    #endif

    // Softshadow
    #ifdef shadow
    col*= shi*softshadow(pos, normalize(lamp.position - pos), shf, 6.) + 1. - shi;
    #endif

    return col;
}

vec3 lampsShading(vec3 norm, vec3 pos, vec3 ocol)
{
    vec3 col = vec3(0.);
    for (int l=0; l<3; l++) // lamps.length()
        col+= lampShading(lamps[l], norm, pos, ocol);

    return col;
}

// From https://www.shadertoy.com/view/lsSXzD, modified
vec3 GetCameraRayDir(vec2 vWindow, vec3 vCameraDir, float fov)
{
	vec3 vForward = normalize(vCameraDir);
	vec3 vRight = normalize(cross(vec3(0.0, 1.0, 0.0), vForward));
	vec3 vUp = normalize(cross(vForward, vRight));

	vec3 vDir = normalize(vWindow.x * vRight + vWindow.y * vUp + vForward * fov);

	return vDir;
}

RenderData trace0(vec3 tpos, vec3 ray, bool nofloor)
{
    float tx = trace(tpos, ray, maxdist, nofloor);
    vec3 col;
    int objnr;

    vec3 pos = tpos + tx*ray;
    vec3 norm;
    if (tx<maxdist*0.65)
    {
        norm = getNormal(pos, normdelta);

        // Coloring
        if (pos.y<=-flpos+0.025)
        {
           col = floor_color(pos);
           objnr = 1;
        }
        else
        {
           col = obj_color(norm, pos);
           objnr = 2;
        }

        // Shading
        col = ambientColor*ambientint + lampsShading(norm, pos, col);

        // Ambient occlusion
        #ifdef ambocc
        col*= 1. - aoint + aoint*vec3(calcAO(pos, norm, 7., 1.1));
        //col = vec3(calcAO(pos, norm, 7., 1.1));
        #endif
  }
  else
  {
      // Sky
      col = sky_color(ray);
      objnr = 3;
  }
  return RenderData(col, pos, norm, objnr);
}

vec4 render(vec2 fragCoord)
{
  lamps[0] = Lamp(vec3(-1., 4.5, 10.), vec3(1., 1., 1.), 7.5, 0.1);
  lamps[1] = Lamp(vec3(9., -0.5, 6.), vec3(1.0, 0.98, 0.9), 3.8, 0.1);
  lamps[2] = Lamp(vec3(-8., 1.8, -5.), vec3(1.0, 0.7, 0.7), 2., 0.1);

  vec2 uv = fragCoord.xy / iResolution.xy;
  uv = uv*2.0 - 1.0;
  uv.x*= iResolution.x / iResolution.y;

  vec3 ray = GetCameraRayDir(uv, camdir, fov);

  RenderData traceinf = trace0(campos, ray, false);
  vec3 col = traceinf.col;
  #ifdef floorref
  if (traceinf.objnr==1)
  {
     RenderData traceinf_floor = trace0(traceinf.pos, reflect(ray, traceinf.norm), true);
     float r = 1.2*clamp(fresnel(ray, traceinf.norm, 2.1), 0.0, 0.3);
     col = mix(col, traceinf_floor.col, r);
     //col = traceinf_floor.col;
  }
  #endif

  return vec4(col, 1.0);
}

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    // Antialiasing
    vec4 vs = vec4(0.);
    for (int j=0;j<aasamples ;j++)
    {
       float oy = float(j)*aawidth/max(float(aasamples-1), 1.);
       for (int i=0;i<aasamples ;i++)
       {
          float ox = float(i)*aawidth/max(float(aasamples-1), 1.);
          vs+= render(fragCoord + vec2(ox, oy));
       }
    }
    fragColor = vs/vec4(aasamples*aasamples);
}
