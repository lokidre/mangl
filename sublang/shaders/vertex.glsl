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

