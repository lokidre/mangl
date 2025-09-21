#line 2

// Inputs from Vertex shader
InVec4      fs_vtx_pos;
InColor     fs_vtx_clr;
InVec2      fs_vtx_tex;
InVec3      fs_vtx_nrm;

InVec3      fs_pos_view;

// Smooth line drawing
#ifdef LINE_UNI_PACKED // Group line params in one vector
UniVec3     un_line_params;
#else
UniFloat    un_line_width;
UniFloat    un_line_edge_inner;
UniFloat    un_line_edge_outer;
#endif
InVec2      fs_line_center;

// Primitive information
UniByte     un_prim_type;

// Shading
UniBool     un_shading_enabled;
UniColor    un_diff_mtl;  // diffuse material
UniColor    un_spec_mtl;  // specular material
UniFloat    un_shininess;

InVec3      fs_light_pos;  // light position
InVec3      fs_light_dir;  // light direction

// Texture
UniBool     un_tex_enabled;
UniSampler2 un_tex_sampler;

// Fog
UniBool     un_fog_enabled;
UniColor    un_fog_color;
InColorV    fs_fog_factor;


// Output fragment
FRAG_OUT_DECL

void main()
{
    Color color = fs_vtx_clr;


COND_SHADING_BEGIN
#ifdef COND_SHADING_PHONG
    //vec3 l = normalize(fs_light_pos - fs_pos_view);
    vec3 l = fs_light_dir;
    vec3 n = fs_vtx_nrm;
    float df = max(dot(n, l), 0.1);

    vec3 r = reflect(-l, n);

    // viewer direction
    vec3 v = vec3(0.0, 0.0, 1.0);
    //vec3 v = normalize(-fs_pos_view);
    float spec_strength = 0.5;
    float sf = pow(max(dot(v, r), 0.0), un_shininess) * spec_strength;

    color.rgb += df * un_diff_mtl.rgb + sf * un_spec_mtl.rgb;
    
    //color = vec4(1.0, 0.0, 0.0, 1.0);
#endif
COND_SHADING_END

COND_TEX_BEGIN  // Texture
    color *= texture(un_tex_sampler, fs_vtx_tex);
COND_TEX_END


COND_FOG_BEGIN  // Fog
    //color.rgb = mix(un_fog_color.rgb, color.rgb, fs_fog_factor);
    color = mix(un_fog_color, color, fs_fog_factor);
COND_FOG_END


COND_LINE_BEGIN  // Line drawing
//     float dist = distance(gl_FragCoord.xy, fs_line_center);

// #ifdef LINE_UNI_PACKED
//     float un_line_width = un_line_params.x;
//     float un_line_edge_inner = un_line_params.y;
//     float un_line_edge_outer = un_line_params.z;
// #endif

//     color.a *= 1.0 - smoothstep(un_line_edge_inner, un_line_edge_outer, dist);

    //color.a *= pow((un_line_width + 1.5 - dist)/un_line_width, 1.5); //blend_factor)
    //color.a *= pow((un_line_edge_outer + 1.5 - dist)/un_line_width, 1.8); //blend_factor)
COND_LINE_END

COND_POINT_BEGIN  // Point drawing
    float dist = distance(gl_FragCoord.xy, fs_line_center);
#ifdef LINE_UNI_PACKED
    float un_line_edge_inner = un_line_params.y;
    float un_line_edge_outer = un_line_params.z;
#endif
    color.a *= 1.0 - smoothstep(un_line_edge_inner, un_line_edge_outer, dist);
COND_POINT_END


    out_frag_color = color;
}
