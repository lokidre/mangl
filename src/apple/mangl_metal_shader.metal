

#include <metal_stdlib>
#include <simd/simd.h>

#import "mangl_metal_types.h"

using namespace metal;
using namespace mangl::render;


//using SColor = float4;
using Color = half4;
using Vec2 = float2;
using Vec3 = float3;
using Vec4 = float4;

struct VertexInput {
    Vec3   position [[attribute(VertexAttributeIndexPosition)]];
    Color  color    [[attribute(VertexAttributeIndexColor)]];
    Vec2   texcoord [[attribute(VertexAttributeIndexTexCoord)]];
    half3  normal   [[attribute(VertexAttributeIndexNormal)]];
};

struct FragmentInput {
    Vec4 position [[position]];
    Color color;
    float2 tex;
    float3 normal;
    
    // Point
    float point_size [[point_size]];
    Vec2 point_center;
    
    // Line rendering
    
    // Texture
    Vec2 tex_coord;
    
    // Fog
    Color fog_color;
    Color fog_factor;
    
    // Shading
    Vec3 light_pos, light_dir;
    
    // Fragment shader stuff
    Vec3 fs_pos_view;
    Vec3 pos_camera;
};

struct SFragmentOutput {
    Color color [[color(0)]];
};


vertex FragmentInput
vertex_main(VertexInput in [[stage_in]],
            constant Uniforms& un [[buffer(BufferIndexUniforms)]],
            uint v_id [[vertex_id]])
{
    FragmentInput frag;
    
    //Vec4 pos_in = Vec4(in.position * un.vtx_pos_scale, 1);
    Vec4 pos_in = Vec4(in.position * un.vtx_pos_scale, 1);
    Vec4 pos_model = un.model_mtx * pos_in;
    Vec4 pos_view = un.view_mtx * pos_model;
    Vec4 pos_camera = un.proj_mtx * pos_view;
    Vec4 pos_screen = pos_camera * un.screen_scale;
    frag.position = pos_screen;
    
    Vec3 pos_frag = pos_camera.xyz / pos_camera.w * un.screen_scale;
    frag.pos_camera = pos_frag;

    
    //    frag.color = in.color * un.scene_color_effect;
    frag.color = in.color;

    
    //frag.position = {in.position.x, in.position.y, in.position.z, 1};
    if (PrimitivePoint == un.prim_type) {
        frag.point_size = un.point_diameter;
        //frag.point_center = ((pos_camera.xy / pos_camera.w) + 1.0) * (un.viewport * 0.5);
    }
    
    if (un.tex_enabled) {
        frag.tex_coord = in.texcoord;
    }

    if (un.fog_enabled) {
        float fog_z = (un.fog_depth == 0)? pos_view.z : length(pos_view.xyz);
        float dist = un.fog_z_near - fog_z;
        float f = dist * un.fog_density;
        float factor = exp(-f);
        frag.fog_factor = factor;
        frag.fog_color = half4(un.fog_color);
    }
    
    
    return frag;
}
                                                               

constexpr sampler textureSampler (mag_filter::linear, min_filter::linear);
                       
fragment SFragmentOutput
fragment_main(FragmentInput in [[stage_in]],
              constant Uniforms& un [[buffer(BufferIndexUniforms)]],
              float2 point_coord [[point_coord]],
              texture2d<half> texture [[texture(TextureIndexBaseColor)]]
              )
{
    SFragmentOutput out;
    
    Color color = in.color;

    if (PrimitivePoint == un.prim_type) {
        float dist = length(point_coord - float2(0.5));
        color.a *= 1.0 - smoothstep(0.4, 0.5, dist);
    }
    
    if (un.tex_enabled) {
        color *= texture.sample(textureSampler, in.tex_coord);
        //out.color.rgb = {1, 0, 0};
    }
    
    if (un.fog_enabled) {
        color = mix(in.fog_color, color, in.fog_factor);
    }
    
    out.color = color;
    return out;
}
