#version 330 core
// УПРОЩЕННЫЙ ШЕЙДЕР ДЛЯ РАССТАНОВКИ ОБЪЕКТОВ
#define eps 0.00001

#define LIGHTCOLOR vec3(16.86, 10.76, 8.2)*200.

uniform float u_time;
uniform vec2 u_resolution;
uniform int u_samples;
uniform int u_samples_perframe;
uniform vec3 u_campos;
uniform mat4 u_raydirmod;

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

float hash1(inout float seed) {
    return fract(sin(seed += 0.1)*43758.5453123);
}

vec2 hash2(inout float seed) {
    return fract(sin(vec2(seed+=0.1,seed+=0.1))*vec2(43758.5453123,22578.1459123));
}

vec3 hash3(inout float seed) {
    return fract(sin(vec3(seed+=0.1,seed+=0.1,seed+=0.1))*vec3(43758.5453123,22578.1459123,19642.3490423));
}

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

vec4 iHexPrism( in vec3  ro, in vec3  rd, in float ra, in float he)
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

const vec4 lightSphere = vec4( 3.0,7.5,2.5, .5 );

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

bool matIsLight( const in float mat ) {
    return mat < 0.5;
}


void main() {
	vec2 q = gl_FragCoord.xy / u_resolution.xy;
    vec2 p = -1.0 + 2.0 * (gl_FragCoord.xy) / u_resolution.xy;
    p.x *= u_resolution.x/u_resolution.y;

    float seed = p.x + p.y * 3.43121412313 + fract(1.12345314312*u_time);
    
    vec3 ro = u_campos;
    // vec3 ta = vec3(2.78, 2.73,  0.00) + u_campos;
    vec3 ww = normalize( vec4(0, 0, 1, 1) * u_raydirmod ).xyz;
    vec3 uu = normalize( cross(ww,vec3(0.0,1.0,0.0) ) );
    vec3 vv = normalize( cross(uu,ww));

    vec3 col = vec3(0.0);
    vec3 tot = vec3(0.0);
    vec3 uvw = vec3(0.0);
    

    vec3 rd = normalize(p.x*uu + p.y*vv + 3.0*ww );

    
    vec3 norm;
    vec2 dist = intersect(ro, rd, norm, col);
    if(dist.x > 1e19)
    {
        gl_FragColor = vec4(0.0);
        return;
    }

    vec3 point = ro + rd * dist.x;


    vec3 col_res = vec3(0);

    for(int i = 0; i < light_n_sources; ++i)
    {
    // Модель освещения Блина-Фонга
        vec3 to_light_t = normalize(lightSources[i].pos_radius.xyz - point);
        vec3 to_view_t = normalize(ro - point);
        vec3 midnorm_t = normalize(to_light_t + to_view_t);

        float k = 1;
        if(intersectShadow(point, to_light_t, length(lightSources[i].pos_radius.xyz - point)))
        {
            k = 0.5;
        }
        col_res += col * max(0, dot(to_light_t, norm)) * k;
    }
    // k /= 10;
    // col = vec3(to_light.z > 0, to_light.z < 0, 1);
    // tot = col;
    tot = col_res / light_n_sources;
    // gl_FragColor = vec4( (tot / u_samples) + texture(u_buffer_tex, q).xyz, 1.0 );
    gl_FragColor = vec4(tot, 1.0);
}