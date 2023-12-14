#version 330 core

uniform sampler2D u_texture;
uniform vec2 u_resolution;
uniform float u_coef;

bool do_postproc;

void main()
{
    vec2 uv = gl_FragCoord.xy / u_resolution;
    // gl_FragColor = vec4(texture(u_texture, uv).xyz, 1);
    vec3 sampled_color = texture(u_texture, uv).xyz * u_coef;
    if (do_postproc)
    {
        sampled_color = pow(sampled_color, vec3(0.45));
    }
    gl_FragColor = vec4(sampled_color, 1);
    // gl_FragColor = vec4(uv, 1, 1);
}