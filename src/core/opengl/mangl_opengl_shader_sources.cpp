/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
// +++ DO NOT EDIT +++ Automatically Generated File +++
namespace mangl::opengl {
const char *shader_conditions_ = R"===(
#line 0

#ifdef COND_SHADING_ON
#define COND_SHADING_BEGIN  {
#elif defined COND_SHADING_OFF
#define COND_SHADING_BEGIN  if (false) {
#else
#define COND_SHADING_BEGIN  if (shading_enabled) {
#endif
#define COND_SHADING_END    }

//#define COND_SHADING_FLAT
//#define COND_SHADING_GOURAUD
#define COND_SHADING_PHONG



#ifdef COND_TEX_ON
#define COND_TEX_BEGIN      {
#elif defined COND_TEX_OFF
#define COND_TEX_BEGIN      if (false) {
#else
#define COND_TEX_BEGIN      if (tex_enabled) {
#endif
#define COND_TEX_END        }


#ifdef COND_FOG_ON
#define COND_FOG_BEGIN      {
#elif defined COND_FOG_OFF
#define COND_FOG_BEGIN      if (false) {
#else  // uniform
#define COND_FOG_BEGIN      if (fog_enabled) {
#endif
#define COND_FOG_END        }

#define FOG_UNI_PACKED      1


#ifdef COND_LINE_ON
#define COND_LINE_BEGIN     {
#elif defined COND_LINE_OFF
#define COND_LINE_BEGIN     if (false) {
#else
#define COND_LINE_BEGIN     if (prim_type == 2) {
#endif
#define COND_LINE_END       }

#define LINE_UNI_PACKED     1


#ifdef COND_POINT_ON
#define COND_POINT_BEGIN    {
#elif defined COND_POINT_OFF
#define COND_POINT_BEGIN    if (false) {
#else
#define COND_POINT_BEGIN    if (prim_type == 3) {
#endif
#define COND_POINT_END      }

)===" ;
const char *shader_fragment_ = R"===(
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
)===" ;
const char *shader_types_ = R"===(

#ifdef GL_ES

#ifdef IS_VERTEX
precision highp     float;
precision mediump   int;
#elif defined IS_FRAGMENT
precision mediump   float;
precision mediump   int;
#endif

#define ColorPrec   mediump
#define Byte        lowp int

#else  // Regular GLSL

// Desktop GLSL don't have precisions
#define mediump
#define lowp
#define highp

#define ColorPrec
#define Byte        int

#endif  // Regular GLSL vs ES


#define Color       ColorPrec vec4
#define ColorFloat  ColorPrec float



#if __VERSION__ < 130
#define InAtt       attribute
#define InVar       varying
#define OutVar      varying
#else  // Version >= 130
#define InAtt       in
#define InVar       in
#define OutVar      out
#endif  // Version >= 130

#define OutFloat    OutVar float
#define OutVec2     OutVar vec2
#define OutVec3     OutVar vec3
#define OutVec4     OutVar vec4
#define OutColor    OutVar Color
#define OutColorV   OutVar ColorPrec float

#define InVec2      InVar vec2
#define InVec3      InVar vec3
#define InVec4      InVar vec4
#define InColorV    InVar ColorPrec float
#define InColor     InVar Color


#define UniFloat    uniform mediump float
#define UniInt      uniform int
#define UniBool     uniform bool
#define UniVec2     uniform vec2
#define UniVec3     uniform vec3
#define UniVec4     uniform vec4
#define UniVec2I    uniform ivec2
#define UniVec3I    uniform ivec3
#define UniVec4I    uniform ivec4
#define UniMat4     uniform mat4
#define UniSampler2 uniform sampler2D

#define UniColor    uniform Color
#define UniByte     uniform Byte


#if __VERSION__ < 150
#define texture(_p1, _p2)       texture2D(_p1, _p2)
#define FRAG_OUT_DECL
#define out_frag_color          gl_FragColor
#else  // GLSL Version >= 150
#define FRAG_OUT_DECL           out Color out_frag_color;
#endif  // GLSL Version ~ 150

)===" ;
const char *shader_vertex_ = R"===(
#line 2

// Input Data
InAtt vec4  in_vtx_pos;
InAtt vec4  in_vtx_clr;
InAtt vec2  in_vtx_tex;
InAtt vec2  in_vtx_nrm;
InAtt vec4  in_vtx_p1;
InAtt vec2  in_vtx_p5;

// Parameters scaling
UniFloat    un_vtx_pos_scale;
UniFloat    un_vtx_nrm_scale;


// Environment/Effects
UniFloat    un_screen_scale;
UniVec2     un_viewport;
UniVec4     un_scene_color_effect;


// Primitive information
UniByte     un_prim_type;

// Texture
UniBool     un_texture_enabled;

// Line
//UniFloat    line_width;
OutVec2     fs_line_center;

// Fog
UniBool     un_fog_enabled;
UniColor    un_fog_color;
#ifdef FOG_UNI_PACKED
UniVec2I    un_fog_iparams;
UniVec3     un_fog_params;
#else
UniInt      un_fog_type;  // 0: linear, 1: exponential, 2: exp squared
UniInt      un_fog_depth; // 0: z plane, 1: distance
UniFloat    un_fog_density;
UniFloat    un_fog_near, un_fog_far;
#endif

OutColorV   fs_fog_factor;


// Shading
UniBool     un_shading_enabled;
UniVec3     un_light_pos;
UniColor    un_diff_mtl;  // diffuse material
UniColor    un_spec_mtl;  // specular material
UniFloat    un_shininess;
OutVec3     fs_light_pos;
OutVec3     fs_light_dir;

// Model/View/Projection Matrices
UniMat4     un_model_mtx;
UniMat4     un_view_mtx;
UniMat4     un_proj_mtx;


// Fragment shader outputs
OutVec4     fs_vtx_pos;
OutColor    fs_vtx_clr;
OutVec2     fs_vtx_tex;
OutVec3     fs_vtx_nrm;

OutVec3     fs_pos_view;


void main()
{
    vec4 pos_in = vec4(in_vtx_pos.xyz * un_vtx_pos_scale, 1.0);

    // vec4 in_pos = vec4(vtx_in_pos.xyz, 1.0);
    // in_pos.xyz *= vtx_pos_scale;

    // Vertex position in world space
    vec4 pos_model = un_model_mtx * pos_in;

    // Vertex Position in camera space
    vec4 pos_view = un_view_mtx * pos_model;

    // Vertex position on projected
    vec4 pos_camera = un_proj_mtx * pos_view;

    // Vertex position on the screen
    vec4 pos_screen = pos_camera * un_screen_scale;

    // Fragment position position on the screen
    vec3 pos_frag = pos_camera.xyz / pos_camera.w * un_screen_scale;
    //pos_screen *= un_screen_scale;

    // Scale camera space coordinates
    //frag_in_cam_space.xyz *= screen_scale;


    // Vertex Color
    vec4 color = in_vtx_clr * un_scene_color_effect;


    //vec3 frag_vtx = vtx_pos.xyz;

COND_TEX_BEGIN
    // Pass texture coordinates to FS
    fs_vtx_tex = in_vtx_tex;
COND_TEX_END


COND_SHADING_BEGIN
    // Ideally:
    // normal = mat3(transpose(inverse(model_mtx))) * vtx_in_nrm;
    // make uniform 3x3 normal_mtx

    vec3 nrm_in = vec3(in_vtx_nrm.xy, in_vtx_pos.w);

    // world light position
    //vec3 light_pos = un_light_pos;
    vec3 light_pos = vec3(un_view_mtx * vec4(un_light_pos, 1.0));
    //vec3 light_pos = vec3(un_view_mtx * (un_model_mtx * vec4(un_light_pos, 1.0)));
    //vec3 light_pos = vec3(un_proj_mtx * (un_view_mtx * vec4(un_light_pos.xyz, 1.0)));


    // Normal in the view world
    vec3 n = nrm_in;
    n = mat3(un_model_mtx) * n;
    n = mat3(un_view_mtx) * n;
    n = normalize(n);

    // Light direction
    //vec3 l = normalize(light_pos - pos_model.xyz);
    vec3 l = normalize(light_pos - pos_view.xyz);


#ifdef COND_SHADING_PHONG
    fs_light_pos = light_pos;
    fs_light_dir = l;
    fs_vtx_nrm = n;
    fs_pos_view = pos_view.xyz;
#endif

#if defined(COND_SHADING_FLAT) || defined(COND_SHADING_GOURAUD)
    // cosine theta
    float df = max(dot(n, l), 0.1);  // must be positive and a little

    color.rgb += df * un_diff_mtl.rgb;  // flat shading

    //vec3 nrm_cam_space = nrm_world;
    // vec3 N = nrm_world;
    // vec3 L = normalize(light_pos - vtx_pos);
    // vec3 H = normalize(L + vec3(0.0, 0.0, 1.0));
    // float df = clamp(dot(N,L), 0.0, 1.0);
    // float sf = max(0.0, dot(N, H));
    // sf = clamp(pow(sf, shininess), 0.0, 1.0);

    //fragInNormal = normalize(normalCameraSpace);
#endif

#ifdef COND_SHADING_GOURAUD
    // reflection direction
    vec3 r = reflect(-l, n);

    // viewer direction
    vec3 v = vec3(0.0, 0.0, 1.0);

    // specular parameters
    float spec_strength = 0.5;

    float sf = max(dot(v, r), 0.0);
    sf = pow(sf, un_shininess) * spec_strength;
    color.rgb += sf * un_spec_mtl.rgb;
    //color.a = sf;
#endif

COND_SHADING_END


COND_FOG_BEGIN
#ifdef FOG_UNI_PACKED
    int un_fog_type = un_fog_iparams.x;
    int un_fog_depth = un_fog_iparams.y;
    float un_fog_density = un_fog_params.x;
    float un_fog_near = un_fog_params.y;
#endif

    float fog_z = (un_fog_depth == 0)? pos_view.z : length(pos_view.xyz);
    float dist = un_fog_near - fog_z;
    float f = dist * un_fog_density;
    float factor = exp(-f);
    // if (fog_type == 0) factor = (fog_far - dist ) / (fog_far - fog_near) ; else
    // if (fog_type == 1) factor = exp(-f) ; else
    // if (fog_type == 2) factor = exp(-f*f) ;

    fs_fog_factor = clamp(factor, 0.0, 1.0);
COND_FOG_END


COND_LINE_BEGIN
/*
    Notes: gl_Position to frag conversion
        // Perspective divide
        vec3 ndc = gl_Position.xyz / gl_Position.w;

        // conversion from ndc[-1, 1] to [0, 1]
        vec2 viewport_coord = ndc.xy * 0.5 + 0.5;

        // adjust by the viewport
        vec2 frag_coord = viewport_coord * viewport;

*/
    //fs_line_center = ((pos_camera.xy / pos_camera.w) + 1.0) * 0.5 * un_viewport;
    fs_line_center = ((pos_screen.xy / pos_screen.w) + 1.0) * 0.5 * un_viewport;
    //fs_line_center = (pos_frag.xy + 1.0) * (un_viewport * 0.5) ;
COND_LINE_END

COND_POINT_BEGIN
    fs_line_center = ((pos_camera.xy / pos_camera.w) + 1.0) * (un_viewport * 0.5) ;
    //gl_PointSize = pointSize ;
COND_POINT_END

    // Sending the final result to FS
    // final vertex position sent to the FS
    fs_vtx_pos = pos_camera;
    fs_vtx_clr = color;

    // Vertex position on the screen
    gl_Position = pos_screen;
}

)===" ;
}  // namespace mangl::opengl
