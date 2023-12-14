#version 330 core

#define eps 0.00001

#define MAX_LIGHTPATH_LEN 10

#define LIGHTCOLOR vec3(16.86, 10.76, 8.2)//*200.

uniform float u_time;
uniform vec2 u_resolution;
uniform int u_samples;
uniform int u_samples_perframe;
uniform sampler2D u_buffer_tex;
uniform vec3 u_campos;
uniform mat4 u_raydirmod;

uniform int u_lightpathlen;
uniform int u_eyepathlen;

struct MapObjectInfo
{
    int object_type;
    float attr_extra_1;
    float attr_extra_2;
    bool attr_flag;
    vec4 attr_1;
    vec4 attr_2;
    vec4 attr_3;
};

struct LightSourceInfo
{
    vec4 pos_radius;
    vec4 intensity;
};
// Вся сцена передается через UniformBuffer

layout (std140) uniform MapData
{
    int map_n_objects;
    MapObjectInfo map_objects[100];
    int light_n_sources;
    LightSourceInfo lightSources[10];
};

// Генерация псевдослучайных чисел

float hash1(inout float seed) {
    return fract(sin(seed += 0.1)*43758.5453123);
}

vec2 hash2(inout float seed) {
    return fract(sin(vec2(seed+=0.1,seed+=0.1))*vec2(43758.5453123,22578.1459123));
}

vec3 hash3(inout float seed) {
    return fract(sin(vec3(seed+=0.1,seed+=0.1,seed+=0.1))*vec3(43758.5453123,22578.1459123,19642.3490423));
}

// Функции пересечения и нормалей в точке, в дальнейшем их станет больше

vec3 nSphere( in vec3 pos, in vec4 sph ) {
    return (pos-sph.xyz)/sph.w;
}

float iSphere( in vec3 ro, in vec3 rd, in vec4 sph ) {
    vec3 oc = ro - sph.xyz;
    float b = dot(oc, rd);
    float c = dot(oc, oc) - sph.w * sph.w;
    float h = b * b - c;
    if (h < 0.0) return -1.0;

	float s = sqrt(h);
	float t1 = -b - s;
	float t2 = -b + s;
	
	return t1 < 0.0 ? t2 : t1;
}

vec3 nPlane( in vec3 ro, in vec4 obj ) {
    return obj.xyz;
}

float iPlane( in vec3 ro, in vec3 rd, in vec4 pla ) {
    return (-pla.w - dot(pla.xyz,ro)) / dot( pla.xyz, rd );
}

vec4 iHexPrism( in vec3  ro, in vec3  rd, in float ra, in float he )
{
    const float ks3 = 0.866025;

    // normals
    const vec3 n1 = vec3( 1.0,0.0,0.0);
    const vec3 n2 = vec3( 0.5,0.0,ks3);
    const vec3 n3 = vec3(-0.5,0.0,ks3);
    const vec3 n4 = vec3( 0.0,1.0,0.0);

    // slabs intersections
    vec3 t1 = vec3((vec2(ra,-ra)-dot(ro,n1))/dot(rd,n1), 1.0);
    vec3 t2 = vec3((vec2(ra,-ra)-dot(ro,n2))/dot(rd,n2), 1.0);
    vec3 t3 = vec3((vec2(ra,-ra)-dot(ro,n3))/dot(rd,n3), 1.0);
    vec3 t4 = vec3((vec2(he,-he)-dot(ro,n4))/dot(rd,n4), 1.0);
    
    // inetsection selection
    if( t1.y<t1.x ) t1=vec3(t1.yx,-1.0);
    if( t2.y<t2.x ) t2=vec3(t2.yx,-1.0);
    if( t3.y<t3.x ) t3=vec3(t3.yx,-1.0);
    if( t4.y<t4.x ) t4=vec3(t4.yx,-1.0);
   
    vec4            tN=vec4(t1.x,t1.z*n1);
    if( t2.x>tN.x ) tN=vec4(t2.x,t2.z*n2);
    if( t3.x>tN.x ) tN=vec4(t3.x,t3.z*n3);
    if( t4.x>tN.x ) tN=vec4(t4.x,t4.z*n4);
    
    float tF = min(min(t1.y,t2.y),min(t3.y,t4.y));
    
    // no intersection
    if( tN.x>tF || tF<0.0) return vec4(-1.0);

    return tN;  // return tF too for exit point
}
float torIntersect( in vec3 ro, in vec3 rd, in vec2 tor )
{
    float po = 1.0;
    float Ra2 = tor.x*tor.x;
    float ra2 = tor.y*tor.y;
    float m = dot(ro,ro);
    float n = dot(ro,rd);
    float k = (m + Ra2 - ra2)/2.0;
    float k3 = n;
    float k2 = n*n - Ra2*dot(rd.xy,rd.xy) + k;
    float k1 = n*k - Ra2*dot(rd.xy,ro.xy);
    float k0 = k*k - Ra2*dot(ro.xy,ro.xy);
    
    if( abs(k3*(k3*k3-k2)+k1) < 0.01 )
    {
        po = -1.0;
        float tmp=k1; k1=k3; k3=tmp;
        k0 = 1.0/k0;
        k1 = k1*k0;
        k2 = k2*k0;
        k3 = k3*k0;
    }
    
    float c2 = k2*2.0 - 3.0*k3*k3;
    float c1 = k3*(k3*k3-k2)+k1;
    float c0 = k3*(k3*(c2+2.0*k2)-8.0*k1)+4.0*k0;
    c2 /= 3.0;
    c1 *= 2.0;
    c0 /= 3.0;
    float Q = c2*c2 + c0;
    float R = c2*c2*c2 - 3.0*c2*c0 + c1*c1;
    float h = R*R - Q*Q*Q;
    
    if( h>=0.0 )  
    {
        h = sqrt(h);
        float v = sign(R+h)*pow(abs(R+h),1.0/3.0); // cube root
        float u = sign(R-h)*pow(abs(R-h),1.0/3.0); // cube root
        vec2 s = vec2( (v+u)+4.0*c2, (v-u)*sqrt(3.0));
        float y = sqrt(0.5*(length(s)+s.x));
        float x = 0.5*s.y/y;
        float r = 2.0*c1/(x*x+y*y);
        float t1 =  x - r - k3; t1 = (po<0.0)?2.0/t1:t1;
        float t2 = -x - r - k3; t2 = (po<0.0)?2.0/t2:t2;
        float t = 1e20;
        if( t1>0.0 ) t=t1;
        if( t2>0.0 ) t=min(t,t2);
        return t;
    }
    
    float sQ = sqrt(Q);
    float w = sQ*cos( acos(-R/(sQ*Q)) / 3.0 );
    float d2 = -(w+c2); if( d2<0.0 ) return -1.0;
    float d1 = sqrt(d2);
    float h1 = sqrt(w - 2.0*c2 + c1/d1);
    float h2 = sqrt(w - 2.0*c2 - c1/d1);
    float t1 = -d1 - h1 - k3; t1 = (po<0.0)?2.0/t1:t1;
    float t2 = -d1 + h1 - k3; t2 = (po<0.0)?2.0/t2:t2;
    float t3 =  d1 - h2 - k3; t3 = (po<0.0)?2.0/t3:t3;
    float t4 =  d1 + h2 - k3; t4 = (po<0.0)?2.0/t4:t4;
    float t = 1e20;
    if( t1>0.0 ) t=t1;
    if( t2>0.0 ) t=min(t,t2);
    if( t3>0.0 ) t=min(t,t3);
    if( t4>0.0 ) t=min(t,t4);
    return t;
}

vec3 torNormal( vec3 pos, vec2 tor )
{
    return normalize( pos*(dot(pos,pos)-tor.y*tor.y - tor.x*tor.x*vec3(1.0,1.0,-1.0)));
}
// Случайное направление на полусфере
vec3 cosWeightedRandomHemisphereDirection( const vec3 n, inout float seed ) {
  	vec2 r = hash2(seed);
    
	vec3  uu = normalize( cross( n, vec3(0.0,1.0,1.0) ) );
	vec3  vv = cross( uu, n );
	
	float ra = sqrt(r.y);
	float rx = ra*cos(6.2831*r.x); 
	float ry = ra*sin(6.2831*r.x);
	float rz = sqrt( 1.0-r.y );
	vec3  rr = vec3( rx*uu + ry*vv + rz*n );
    
    return normalize( rr );
}

vec2 boxIntersection( in vec3 ro, in vec3 rd, vec3 boxSize, out vec3 outNormal ) 
{
    vec3 m = 1.0/rd; // can precompute if traversing a set of aligned boxes
    vec3 n = m*ro;   // can precompute if traversing a set of aligned boxes
    vec3 k = abs(m)*boxSize;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );
    if( tN>tF || tF<0.0) return vec2(-1.0); // no intersection
    outNormal = (tN>0.0) ? step(vec3(tN),t1) : // ro ouside the box
                           step(t2,vec3(tF));  // ro inside the box
    outNormal *= -sign(rd);
    return vec2( tN, tF );
}

vec3 triIntersect( in vec3 ro, in vec3 rd, in vec3 v0, in vec3 v1, in vec3 v2, inout vec3 n )
{
    vec3 v1v0 = v1 - v0;
    vec3 v2v0 = v2 - v0;
    vec3 rov0 = ro - v0;
    n = cross( v1v0, v2v0 );
    vec3  q = cross( rov0, rd );
    float d = 1.0/dot( rd, n );
    float u = d*dot( -q, v2v0 );
    float v = d*dot(  q, v1v0 );
    float t = d*dot( -n, rov0 );
    if( u<0.0 || v<0.0 || (u+v)>1.0 ) t = -1.0;
    return vec3( t, u, v );
}

// Случайное направление на сфере
vec3 randomSphereDirection(inout float seed) {
    vec2 h = hash2(seed) * vec2(2.,6.28318530718)-vec2(1,0);
    float phi = h.y;
	return vec3(sqrt(1.-h.x*h.x)*vec2(sin(phi),cos(phi)),h.x);
}

// Источник света

// const vec4 lightSphere = vec4( 3.0,7.5,2.5, .5 );

vec3 sampleLight( const in vec3 ro, inout float seed, int ind ) {
    vec3 n = randomSphereDirection(seed) * lightSources[ind].pos_radius.w;
    return lightSources[ind].pos_radius.xyz + n;
}

vec2 intersect( in vec3 ro, in vec3 rd, inout vec3 normal, inout vec3 color ) {
	vec2 res = vec2( 1e20, -1.0 );
    float t;
	
    for (int i = 0; i < map_n_objects; ++i)
    {
        if(map_objects[i].object_type == 0)
        {
            vec4 position = map_objects[i].attr_1;
            t = iSphere( ro, rd, position);
            if( t>eps && t<res.x )
            {
                res = vec2( t, map_objects[i].attr_2.w );
                normal = nSphere( ro+t*rd, position );
                color = map_objects[i].attr_2.xyz;
            }
        }
        else if(map_objects[i].object_type == 1)
        {
            t = iPlane( ro, rd, map_objects[i].attr_1 );
            if( t>eps && t<res.x )
            {
                res = vec2( t, map_objects[i].attr_3.w );
                normal = map_objects[i].attr_1.xyz * map_objects[i].attr_extra_1;
                color = map_objects[i].attr_3.xyz;
            }
        }
        else if(map_objects[i].object_type == 2)
        {
            vec3 new_norm;
            t = boxIntersection(ro - map_objects[i].attr_3.xyz, rd, map_objects[i].attr_1.xyz, new_norm).x;
            if( t>eps && t<res.x )
            {
                res = vec2(t, map_objects[i].attr_2.w);
                normal = new_norm;
                color = map_objects[i].attr_2.xyz;
            }
        }
        else if(map_objects[i].object_type == 3)
        {
            vec4 tN = iHexPrism(ro - map_objects[i].attr_3.xyz, rd, map_objects[i].attr_1.x, map_objects[i].attr_1.y);
            t = tN.x;
            if( t>eps && t<res.x )
            {
                res = vec2( t, map_objects[i].attr_2.w);
                normal = tN.yzw;;
                color = map_objects[i].attr_2.xyz;
            }
        }
        else if(map_objects[i].object_type == 4)
        {
            vec3 tmp_n;
            vec3 tuv = triIntersect(ro, rd, map_objects[i].attr_1.xyz, map_objects[i].attr_2.xyz, map_objects[i].attr_3.xyz, tmp_n);
            if (tuv.x > eps && tuv.x < res.x)
            {
                res = vec2(tuv.x, map_objects[i].attr_extra_1);
                normal = normalize(tmp_n);
                color = vec3(map_objects[i].attr_1.w, map_objects[i].attr_2.w, map_objects[i].attr_3.w);
            }
        }
        else if(map_objects[i].object_type == 5)
        {
            t = torIntersect(ro - map_objects[i].attr_3.xyz, rd, map_objects[i].attr_1.xy);
            if( t>eps && t<res.x )
            {
                res = vec2( t, map_objects[i].attr_2.w);
                normal = torNormal(map_objects[i].attr_3.xyz, map_objects[i].attr_1.xy);
                color = map_objects[i].attr_2.xyz;
            }
        }
    }

    // См в intersectShadow что это за плоскость
    // t = iPlane( ro, rd, vec4( 0.0,-1.0, 0.0,5.49) ); if( t>eps && t<res.x && ro.z+rd.z*t < 5.5 ) { res = vec2( t, 1. ); normal = vec3( 0.,-1., 0.); }
    // Источник света пока захардкожен
    for(int i = 0; i < light_n_sources; ++i)
    {
        t = iSphere( ro, rd, lightSources[i].pos_radius ); if( t>eps && t<res.x ) { res = vec2( t, -1 );  normal = nSphere( ro+t*rd, lightSources[i].pos_radius ); }
    }
					  
    return res;					  
}

bool intersectShadow( in vec3 ro, in vec3 rd, in float dist ) {
    float t;
	for (int i = 0; i < map_n_objects; ++i)
    {
        if(map_objects[i].object_type == 0)
        {
            vec4 position = map_objects[i].attr_1;
            t = iSphere( ro, rd, position);
            if( t > eps && t < dist ) { return true; }
        }
        else if(map_objects[i].object_type == 1)
        {
            t = iPlane( ro, rd, map_objects[i].attr_1 );
            if( t > eps && t < dist ) { return true; }
        }
        else if(map_objects[i].object_type == 2)
        {
            vec3 new_norm;
            t = boxIntersection(ro - map_objects[i].attr_3.xyz, rd, map_objects[i].attr_1.xyz, new_norm).x;
            if( t>eps && t < dist ) { return true; }
        }
        else if(map_objects[i].object_type == 3)
        {
            vec4 tN = iHexPrism(ro - map_objects[i].attr_3.xyz, rd, map_objects[i].attr_1.x, map_objects[i].attr_1.y);
            t = tN.x;
            if( t > eps && t < dist ) { return true; }
        }
        else if(map_objects[i].object_type == 4)
        {
            vec3 tmp_n;
            vec3 tuv = triIntersect(ro, rd, map_objects[i].attr_1.xyz, map_objects[i].attr_2.xyz, map_objects[i].attr_3.xyz, tmp_n);
            if (tuv.x > eps && tuv.x < dist) {return true;}
        }
        else if(map_objects[i].object_type == 5)
        {
            t = torIntersect(ro - map_objects[i].attr_3.xyz, rd, map_objects[i].attr_1.xy);
            if( t > eps && t < dist ) { return true; }
        }
    }

    // Пока захардкожено, потому что в сцене демонстрируется непрямое освещение и нужна выемка,
    // а т.к. поддерживаются пока только сферы и плоскости, плоскости бесконечные и их надо ограничить
    // t = iPlane( ro, rd, vec4( 0.0,-1.0, 0.0,5.49) ); if( t>eps && t<dist && ro.z+rd.z*t < 5.5 ) { return true; }

    return false;
}

// Материалы пока захардкожены потом будет лучше

bool matIsSpecular(const in float mat)
{
    return mat >= 1;
}

bool matIsLight(const in float mat)
{
    return mat == -1;
}

bool matIsMirror(const in float mat)
{
    return mat <= -2;
}

// Функция выбора направления отражения луча на основе того, какой материал там используется

vec3 getBRDFRay( in vec3 n, const in vec3 rd, const in float m, inout bool specularBounce, inout float seed ) {
    if(matIsMirror(m))
    {
        specularBounce = true;
        return reflect( rd, n );
    }
    specularBounce = false;

    
    vec3 r = cosWeightedRandomHemisphereDirection( n, seed );
    if(  !matIsSpecular( m ) ) {
        return r;
    } else {
        specularBounce = true;
        
        float n1, n2, ndotr = dot(rd,n);
        
        if( ndotr > 0. ) {
            n1 = 1./m; n2 = 1.;
            n = -n;
        } else {
            n2 = 1./m; n1 = 1.;
        }
                
        float r0 = (n1-n2)/(n1+n2); r0 *= r0;
		float fresnel = r0 + (1.-r0) * pow(1.0-abs(ndotr),5.);
        
        vec3 ref = refract( rd, n, n2/n1 );        
        if( ref == vec3(0) || hash1(seed) < fresnel ) {
            ref = reflect( rd, n );
        }
        
        return ref;
	}
}

// ТРАССИРОВКА ОТ ИСТОЧНИКА

struct LightPathNode {
    vec3 color;
    vec3 position;
    vec3 normal;
};

LightPathNode lpNodes_[MAX_LIGHTPATH_LEN * 10];

void constructLightPath( inout float seed, int ind ) {
    vec3 ro = randomSphereDirection( seed );
    vec3 rd = cosWeightedRandomHemisphereDirection( ro, seed );
    ro = lightSources[ind].pos_radius.xyz - ro*lightSources[ind].pos_radius.w;
    vec3 color = LIGHTCOLOR * lightSources[ind].intensity.x;

    int lp_offs = ind * MAX_LIGHTPATH_LEN;
 
    for( int i=lp_offs; i<u_lightpathlen + lp_offs; ++i ) {
        lpNodes_[i].position = lpNodes_[i].color = lpNodes_[i].normal = vec3(0.);
    }
    
    bool specularBounce;
    float w = 0.;
    
    for( int i=lp_offs; i<u_lightpathlen + lp_offs; i++ ) {
		vec3 normal;

        vec3 cur_color;
        vec2 res = intersect( ro, rd, normal, cur_color );
        
        if( !matIsLight(res.y) && dot( rd, normal ) < 0. ) {
            ro = ro + rd*res.x;            
            // color *= matColor( res.y );
            color *= cur_color;
            
            lpNodes_[i].position = ro;
            if( !matIsSpecular( res.y ) ) lpNodes_[i].color = color;// * clamp( dot( normal, -rd ), 0., 1.);
            lpNodes_[i].normal = normal;
            
            rd = getBRDFRay( normal, rd, res.y, specularBounce, seed );
        } else break;
    }
}

// ТРАССИРОВКА ОТ НАБЛЮДАТЕЛЯ

float getWeightForPath( int e, int l ) {
    return float(e + l + 2);
}

vec3 traceEyePath( in vec3 ro, in vec3 rd, inout float seed ) {
    vec3 tcol = vec3(0.);
    vec3 fcol  = vec3(1.);
    
    bool specularBounce = true; 
	int jdiff = 0;
    
    for( int j=0; j<u_eyepathlen; ++j ) {
        vec3 normal;

        vec3 target_color;
        vec2 res = intersect( ro, rd, normal, target_color );
        // if( res.y < -0.5 ) {
        //     return tcol;
        // }
        
        if( matIsLight( res.y ) ) {
            if( specularBounce ) tcol += fcol * LIGHTCOLOR;
            return tcol;
        }
        
        ro = ro + res.x * rd;   
        vec3 rdi = rd;
        rd = getBRDFRay( normal, rd, res.y, specularBounce, seed );

        if(!specularBounce || dot(rd,normal) < 0.) {  
        	fcol *= target_color;
        	// fcol *= matColor( res.y );
        }

        for(int lsrci = 0; lsrci < light_n_sources; ++lsrci)
        {
            int lp_offs = lsrci * MAX_LIGHTPATH_LEN;
            vec3 ld = sampleLight( ro, seed, lsrci ) - ro;
            
            vec3 nld = normalize(ld);

            if( !specularBounce && !intersectShadow( ro, nld, length(ld)) ) {
                float cos_a_max = sqrt(1. - clamp(lightSources[lsrci].pos_radius.w * lightSources[lsrci].pos_radius.w / dot(lightSources[lsrci].pos_radius.xyz-ro, lightSources[lsrci].pos_radius.xyz-ro), 0., 1.));
                float weight = 2. * (1. - cos_a_max);


                // float k = 1 - 0.5 * float(matIsSpecular( res.y ) );
                tcol += (fcol * LIGHTCOLOR * lightSources[lsrci].intensity.x) * (weight * clamp(dot( nld, normal ), 0., 1.))
                    / getWeightForPath(jdiff,-1);

                
                if( !matIsSpecular( res.y ) ) {
                    for( int i=lp_offs; i<u_lightpathlen + lp_offs; ++i ) {
                        vec3 lp = lpNodes_[i].position - ro;
                        vec3 lpn = normalize( lp );
                        vec3 lc = lpNodes_[i].color;

                        if( !intersectShadow(ro, lpn, length(lp)) ) {
                            // веса для j+1 точки от наблюдателя к i+2 точке пути
                            float weight = 
                                    clamp( dot( lpn, normal ), 0.0, 1.) 
                                * clamp( dot( -lpn, lpNodes_[i].normal ), 0., 1.)
                                * clamp(1. / dot(lp, lp), 0., 1.)
                                ;

                            tcol += lc * fcol * weight / getWeightForPath(jdiff,i);
                            // tcol = vec3(0, 1, 0);
                        }
                    }
                }
            }
        }
        
        if( !specularBounce) jdiff++; else jdiff = 0;
    }  
    
    return tcol;
}

void main() {
    // x: [0, 1], y: [0, 1]
	vec2 q = gl_FragCoord.xy / u_resolution.xy;

    // x: [-1, 1], y: [-1, 1]
    vec2 p = -1.0 + 2.0 * (gl_FragCoord.xy) / u_resolution.xy;
    p.x *= u_resolution.x/u_resolution.y;

    float seed = p.x + p.y * 3.43121412313 + fract(1.12345314312*u_time);
    
    // ro и вектора forward, right, up
    vec3 ro = u_campos;
    vec3 ww = normalize( vec4(0, 0, 1, 1) * u_raydirmod ).xyz;
    vec3 uu = normalize( cross(ww,vec3(0.0,1.0,0.0) ) );
    vec3 vv = normalize( cross(uu,ww));

    vec3 col = vec3(0.0);
    vec3 tot = vec3(0.0);

    for(int i = 0; i < light_n_sources; ++i)
    {
        constructLightPath( seed, i );
    }

    for( int a=0; a<u_samples_perframe; a++ ) {

        // небольшой разброс луча
        vec2 rpof = 4.*(hash2(seed)-vec2(0.5)) / u_resolution.xy;
        // rpof /= 4;
	    vec3 rd = normalize( (p.x+rpof.x)*uu + (p.y+rpof.y)*vv + 3.0*ww );        

        // пока захардкожен фокус через 12
	    vec3 fp = ro + rd * 12.0;
   		vec3 rof = ro + (uu*(hash1(seed)-0.5) + vv*(hash1(seed)-0.5))*0.125;
    	rd = normalize( fp - rof );

        
        // процедура построения пути от источника
        // constructLightPath( seed );
        
        // функция трассировки пути от наблюдателя
        col = traceEyePath( rof, rd, seed );

        tot += col;
        
        seed = mod( seed*1.1234567893490423, 13. );
    }
    
    tot /= float(u_samples_perframe);
    
    
	// tot = pow( clamp(tot,0.0,1.0), vec3(0.45) );
    tot = clamp(tot, 0.0, 1.0);

    vec3 cur_color = tot / u_samples;
    vec3 saved_color = texture(u_buffer_tex, q).xyz;

    gl_FragColor = vec4(cur_color + saved_color, 1.0 );
}