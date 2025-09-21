#import "mangl_metal_render_view.h"


#if MANGL_METAL

@import Metal;
@import QuartzCore;

#include "../apple/mangl_metal_types.h"
#include "../apple/mangl_imacos_util.h"

#include "../core/mangl_framework.h"
#include "../core/mangl_render.h"
#include "../core/render/mangl_pl_util_global.h"



namespace mangl::render {
static RealGL cacheScreenScale_{};
static RealGL cacheLineWidth_{}, cachePointSize_{};
static bool cacheVtxTexEnabled_{};
static int cacheTexSlot_{-1};

static bool cacheScissorEnabled_{};
static RectGL cacheScissorRect_{};
static MTLScissorRect cacheScissorViewport_{};


static MatrixHashedGL::Hash modelMtxCacheHash_{}, viewMtxHashCache_{}, projMtxHashCache_{};
}  // namespace mangl::render;


using namespace mangl;

constexpr static int NumInflightBuffers{3};//, NumUniformBuffers{3};

static ManglMetalRenderView* renderViewInstance__;

template <int N>
struct BufferProvider {
    id<MTLBuffer> buffers[N];
    int index{};
    
    auto getAvail() noexcept {
        auto i = index++;
        if (index >= N)
            index = 0;
        return buffers[i];
    }
       
};

@interface ManglMetalRenderView()
@property (nonatomic) CAMetalLayer* metalLayer;

@property (nonatomic) id<MTLDepthStencilState> depthStencilState;
@property (nonatomic) id<MTLDepthStencilState> depthStencilStateOff;
@property (nonatomic) id<MTLTexture> depthTexture;
@end

@implementation ManglMetalRenderView {
    dispatch_semaphore_t inflightSemaphore_;
    id<MTLDevice> device_;
    BufferProvider<NumInflightBuffers> vertexBuffers_;
    BufferProvider<NumInflightBuffers> indexBuffers_;
    id<MTLCommandQueue> commandQueue_;
    MTLRenderPipelineDescriptor* renderPipelineDesc_;
    id<MTLRenderPipelineState> renderPipelineState_;
    
    //id<MTLDepthStencilState> depthState_;
    
    mangl::render::Uniforms uniforms_;
    
    mangl::Rect viewport_;
    bool cacheDepthTest_;
    mangl::render::Uniforms cacheUniforms_;
}




+(Class)layerClass {  return [CAMetalLayer class];  }

-(CALayer *)makeBackingLayer {  return [CAMetalLayer layer]; }


- (id<MTLDevice>)getDevice { return device_; }

- (void)ctor
{
    //frameDrawable_ = nil;
    //renderPassDesc_ = nil;
    uniforms_ = {};
    
    cacheUniforms_ = {};
    cacheUniforms_.prim_type = -1;
}



- (id)initWithFrame:(CGRect)rect
{
    self = [super initWithFrame:rect];
    renderViewInstance__ = self;
    [self ctor];
    return self;
}

- (void)dealloc
{
    [self dtor];
}

- (void)dtor{}


- (void)attachRender:(SceneRenderer *)render
{
    render_ = render;
    
    using namespace mangl::render;
    
    NSError* error{};
    
    //auto depthFormat = MTLPixelFormatDepth32Float;
//#if APEAL_OS_IPHONE
//    auto colorFormat = MTLPixelFormatBGRA8Unorm_sRGB;
//#else
    auto colorFormat = MTLPixelFormatBGRA8Unorm;
//#endif
    
    device_ = MTLCreateSystemDefaultDevice();
    
    MDebugPrint("Metal device name: %s", fromNSString(device_.name));
    auto maxBufferLength = device_.maxBufferLength;
    MDebugPrint("   max buffer length: %d", maxBufferLength);
    
    //MDebugPrint("Metal device name: %s", [device_ name]);
    
    //viewport_ = {.w = self.frame.size.width, .h = self.frame.size.height};
    //viewport_ = {.w = self.frame.size.width, .h = self.frame.size.height};
    //viewport_ = Env::physScreenSize.rect<MRect>();
    viewport_ = Env::applicationRect;
    
    //auto layerFrame = self.layer.frame;
    //auto viewFrame = self.frame;
    
    //self.wantsLayer = YES;
    _metalLayer = (CAMetalLayer *)self.layer;
    //_metalLayer = self.layer;
    [_metalLayer setDevice: device_];
    [_metalLayer setPixelFormat: colorFormat];
    _metalLayer.framebufferOnly = YES;
    _metalLayer.drawableSize = CGSizeMake(viewport_.w, viewport_.h);
    
    cacheScissorViewport_.x = cacheScissorViewport_.y = 0;
    cacheScissorViewport_.width = viewport_.w;
    cacheScissorViewport_.height = viewport_.h;
    
    
    inflightSemaphore_ = dispatch_semaphore_create(NumInflightBuffers);
    commandQueue_ = [device_ newCommandQueue];
    
   
    auto storageMode = MTLResourceStorageModeShared;
    
    maxRenderBufferSize_ = 1024*1024; //*16; //256000; //16000;
    //maxRenderBufferSize_ = 1024;
    maxBatchVtxCount_ = maxRenderBufferSize_ / SizeOfVertexPack;
    maxBatchIdxCount_ = maxRenderBufferSize_ / SizeOfIndexBufferIndex;

    if (maxRenderBufferSize_ > maxBufferLength)
        maxRenderBufferSize_ = (int)maxBufferLength;

    int indexBufferSize = maxRenderBufferSize_;
    int vertexBufferSize = maxRenderBufferSize_;
    
    int index{};

    for (index = 0; auto& buffer: vertexBuffers_.buffers) {
        buffer = [device_ newBufferWithLength:vertexBufferSize options:storageMode];
        buffer.label = [NSString stringWithFormat:@"vertex_buffer_%i", index++];
    }

    for (index = 0; auto& buffer: indexBuffers_.buffers) {
        buffer = [device_ newBufferWithLength:indexBufferSize options:storageMode];
        buffer.label = [NSString stringWithFormat:@"index_buffer_%i", index++];
    }
    
//    for (index = 0; auto& buffer: uniformBuffers_.buffers) {
//        buffer = [device_ newBufferWithLength:uniformBufferSize options:MTLResourceCPUCacheModeDefaultCache];
//        buffer.label = [NSString stringWithFormat:@"uniform_buffer_%i", index++];
//    }


//    for (int i = 0 ; i < NumInflightBuffers; ++i) {
//        vertexBuffers_[i] = [device_ newBufferWithLength:vertexBufferSize options:storageMode];
//        vertexBuffers_[i].label = [NSString stringWithFormat:@"RenderBuffer_%i", i];
//
//        indexBuffers_[i] = [device_ newBufferWithLength:indexBufferSize options:storageMode];
//        indexBuffers_[i].label = [NSString stringWithFormat:@"IndexBuffer_%i", i];
//
//        uniformBuffers_[i] = [device_ newBufferWithLength:uniformBufferSize options:storageMode];
//        uniformBuffers_[i].label = [NSString stringWithFormat:@"UniformBuffer_%i", i];
//
//        //auto un = static_cast<Uniforms*>(uniformBuffers_[i].contents);
//        //*un = {};
//    }
    
    
    auto vtxDesc = [[MTLVertexDescriptor alloc] init];
    
    index = VertexAttributeIndexPosition;
    vtxDesc.attributes[index].format = MtlVertexFormatPosition;
    vtxDesc.attributes[index].offset = MtlVertexOffsetPosition;
    vtxDesc.attributes[index].bufferIndex = BufferIndexVertices;
    
    index = VertexAttributeIndexColor;
    vtxDesc.attributes[index].format = MtlVertexFormatColor;
    vtxDesc.attributes[index].offset = MtlVertexOffsetColor;
    vtxDesc.attributes[index].bufferIndex = BufferIndexVertices;
    
    index = VertexAttributeIndexTexCoord;
    vtxDesc.attributes[index].format = MtlVertexFormatTexCoord;
    vtxDesc.attributes[index].offset = MtlVertexOffsetTexCoord;
    vtxDesc.attributes[index].bufferIndex = BufferIndexVertices;
    
    index = VertexAttributeIndexNormal;
    vtxDesc.attributes[index].format = MtlVertexFormatNormal;
    vtxDesc.attributes[index].offset = MtlVertexOffsetNormal;
    vtxDesc.attributes[index].bufferIndex = BufferIndexVertices;
    
    index = BufferIndexVertices;
    vtxDesc.layouts[index].stride = sizeof(VertexData);
    vtxDesc.layouts[index].stepRate = 1;
    vtxDesc.layouts[index].stepFunction = MTLVertexStepFunctionPerVertex;
    
    
    
    renderPipelineDesc_ = [MTLRenderPipelineDescriptor new];
    renderPipelineDesc_.label = @"RenderPipeline";
    renderPipelineDesc_.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float;
    renderPipelineDesc_.vertexBuffers[BufferIndexVertices].mutability = MTLMutabilityImmutable;
    
    auto library = [device_ newDefaultLibrary];
    
    auto fragmentFunction = [library newFunctionWithName:@"fragment_main"];
    auto vertexFunction = [library newFunctionWithName:@"vertex_main"];
    
    renderPipelineDesc_.vertexDescriptor = vtxDesc;
    renderPipelineDesc_.vertexFunction = vertexFunction;
    renderPipelineDesc_.fragmentFunction = fragmentFunction;
    
    auto att = renderPipelineDesc_.colorAttachments[0];
    att.pixelFormat = colorFormat;
    
    att.blendingEnabled = YES;
    att.rgbBlendOperation = MTLBlendOperationAdd;
    att.alphaBlendOperation = MTLBlendOperationAdd;
    
    att.sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    att.sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
    
    att.destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    att.destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    
//    let depthStencilDesc = MTLDepthStencilDescriptor()
//    depthStencilDesc.depthCompareFunction = .Always
//    depthStencilDesc.depthWriteEnabled = false
//    mNoDepthTest = device.newDepthStencilStateWithDescriptor(depthStencilDesc)
//    encoder.setDepthStencilState(mNoDepthTest!)
    
    //renderPipelineDesc_.colorAttachments[0].pixelFormat = colorFormat;
    //pipelineDesc.depthAttachmentPixelFormat = depthFormat;
    //pipelineDesc.depthAttachmentPixelFormat = MTLPixelFormatInvalid;
    
    renderPipelineState_ = [device_ newRenderPipelineStateWithDescriptor:renderPipelineDesc_ error:&error];
    
    NSAssert(renderPipelineState_, @"Metal: Failed to create pipeline: %@", error);
    
    
    

    MTLDepthStencilDescriptor *depthStencilDescriptor = [MTLDepthStencilDescriptor new];
    depthStencilDescriptor.depthCompareFunction = MTLCompareFunctionLess;
    depthStencilDescriptor.depthWriteEnabled = YES;
    _depthStencilState = [device_ newDepthStencilStateWithDescriptor:depthStencilDescriptor];

    depthStencilDescriptor = [MTLDepthStencilDescriptor new];
    depthStencilDescriptor.depthCompareFunction = MTLCompareFunctionAlways;
    //depthStencilDescriptor.depthWriteEnabled = YES;
    _depthStencilStateOff = [device_ newDepthStencilStateWithDescriptor:depthStencilDescriptor];

    
    //
    // Initialize uniforms
    //
    //uniforms_.model_mtx = uniforms_.view_mtx = uniforms_.proj_mtx = matrix_identity_float4x4;
    
    Framework::instance()->onInitialized();
}


// Used from MacOS
- (void)updateViewport {
    using namespace mangl::render;
    
    _depthTexture = nil;
    
    viewport_ = Env::applicationRect;
    _metalLayer.drawableSize = CGSizeMake(viewport_.w, viewport_.h);
    cacheScissorViewport_.width = viewport_.w;
    cacheScissorViewport_.height = viewport_.h;
}

- (void)renderScene
{
    // Timestamp
    // CACurrentMediaTime
    
    auto drawable = [_metalLayer nextDrawable];
        
    if (!drawable)
        return;
       
    [self submitRender:drawable];
}


//
// submibRender supporting stuff
//

- (void)sendBuffersData:(id<MTLRenderCommandEncoder>)renderEncoder
           vertexBuffer:(id<MTLBuffer>)vertexBuffer
            indexBuffer:(id<MTLBuffer>)indexBuffer
                  batch:(const render::DrawBatch *)batch
{
    using namespace render;
    
    auto vtxData = batch->vtxData;
    auto vtxSize = batch->vtxCount * SizeOfVertexPack;
    
    auto idxData = batch->idxData;
    auto idxSize = batch->idxCount * SizeOfIndexBufferIndex;
    
//    auto vertexBuffer = vertexBuffers_.getAvail();
//    auto indexBuffer = indexBuffers_.getAvail();

    std::memcpy(vertexBuffer.contents, vtxData, vtxSize);
    std::memcpy(indexBuffer.contents, idxData, idxSize);
    
    [renderEncoder setVertexBuffer:vertexBuffer offset:0 atIndex:BufferIndexVertices];
}


- (void)assignMatrix:(render::UniMat4&) unMat pmtx:(const MatrixHashedGL*) pmtx hash:(MatrixHashedGL::Hash*) hash
{
    if (*hash == pmtx->hash)
        return;
    
    auto& mtx = *pmtx;
    
    unMat.columns[0] = {mtx.a[0][0], mtx.a[1][0], mtx.a[2][0], mtx.a[3][0]};
    unMat.columns[1] = {mtx.a[0][1], mtx.a[1][1], mtx.a[2][1], mtx.a[3][1]};
    unMat.columns[2] = {mtx.a[0][2], mtx.a[1][2], mtx.a[2][2], mtx.a[3][2]};
    unMat.columns[3] = {mtx.a[0][3], mtx.a[1][3], mtx.a[2][3], mtx.a[3][3]};
    
    *hash = mtx.hash;
}

- (void)updateLineWidth:(RealGL)width
{
    using namespace render;
    
    cacheLineWidth_ = width;
    
    auto scaled = width * cacheScreenScale_;
    
    //auto edge = scaled > 1.5 ? scaled - 1.5f : scaled - 0.5f;
    auto outerEdge = scaled;// + 0.2f;
    auto innerEdge = outerEdge / 4;// * 0.5f;

    uniforms_.line_width = scaled;
    uniforms_.line_edge_inner = innerEdge;
    uniforms_.line_edge_outer = outerEdge;
}

- (void)updatePointSize:(RealGL)size {
    using namespace render;
    
    cachePointSize_ = size;
    auto scaled = size * cacheScreenScale_;
    uniforms_.point_diameter = scaled;
}

- (void)setUnColor:(render::UniColor&)unc color:(ColorA)color {
    unc[0] = color.r;
    unc[1] = color.g;
    unc[2] = color.b;
    unc[3] = color.a;
}

- (void) renderCommand:(id<MTLRenderCommandEncoder>)renderEncoder 
           indexBuffer:(id<MTLBuffer>)indexBuffer
                   cmd:(render::DrawPrimitive*)cmd
{
    using namespace render;
    //dispatch_semaphore_wait(inflightSemaphore_, DISPATCH_TIME_FOREVER);

    const auto& state = cmd->state;
    auto& un = uniforms_;
    bool skipRender = false;


    MTLPrimitiveType primitiveType{};
    
    switch(cmd->primitiveType) {
    case PrimitiveTriangle:
        primitiveType = MTLPrimitiveTypeTriangle;
        break;
        
    case PrimitiveLine:
        primitiveType = MTLPrimitiveTypeLine;
        if (cacheLineWidth_ != state.lineWidth)
            [self updateLineWidth:state.lineWidth];
        break;
        
    case PrimitivePoint:
        primitiveType = MTLPrimitiveTypePoint;
        if (cachePointSize_ != state.lineWidth)
            [self updatePointSize:state.lineWidth];
        break;
        
    default:
        MDebugTrap();
        break;
    }

    // Shader selection parameters
    un.prim_type = cmd->primitiveType;
    un.vtx_pos_scale = state.vtxPosScale;
    //un.scene_color_effect = state.col
    
    
    un.tex_enabled = state.texture ? 1: 0; //Enabled;
    
    if (state.texture) {
        if (!cacheVtxTexEnabled_ || cacheTexSlot_ != state.texture->glTextureSlot) {
            [renderEncoder setFragmentTexture:state.texture->textureMetal atIndex:TextureIndexBaseColor];
            cacheVtxTexEnabled_ = true;
            cacheTexSlot_ = state.texture->glTextureSlot;
        }
    } else {
        if (cacheVtxTexEnabled_) {
            [renderEncoder setFragmentTexture:nil atIndex:TextureIndexBaseColor];
            cacheVtxTexEnabled_ = false;
        }

    }
    
    auto updateScissor{false};
    if (state.scissor) {
        if (!cacheScissorEnabled_)
            updateScissor = true;
        else
            if (cacheScissorRect_ != state.scissorRect)
                updateScissor = true;
    } else {
        if (state.scissor != cacheScissorEnabled_)
            updateScissor = true;
    }
    
    if (updateScissor) {
        cacheScissorEnabled_ = state.scissor;
        cacheScissorRect_ = state.scissorRect;
        
        if (state.scissor) {
            auto a2 = state.scissorRect.pos();
            auto c2 = a2 + state.scissorRect.size().point();
            
            Point3DGL a{a2.x, a2.y, 1}, c{c2.x, c2.y, 1};
            
            a = *state.viewMtx * (*state.modelMtx * a);
            a *= cacheScreenScale_;
            
            c = *state.viewMtx * (*state.modelMtx * c);
            c *= cacheScreenScale_;
            
            MTLScissorRect scissorRect;
            scissorRect.width = c.x - a.x;
            scissorRect.height = c.y - a.y;

            scissorRect.x = a.x;
            scissorRect.y = viewport_.h - scissorRect.height - a.y;
            
            // Check if the scissor rect fits the viewport
            // The system throws the error for the wrong scissor rect
            // Check if the scissor rect is completely out of bounds
            // then we just don't render it
            if (scissorRect.x >= viewport_.w || scissorRect.y >= viewport_.h) {
//                MDebugFunc("Scissor is out of bounds");
                return;
            }
            
            if (scissorRect.x + scissorRect.width > viewport_.w) {
                scissorRect.width = viewport_.w - scissorRect.x;
            }
            
            if (scissorRect.y + scissorRect.height > viewport_.h) {
                scissorRect.height = viewport_.h - scissorRect.y;
            }


            [renderEncoder setScissorRect:scissorRect];
        } else {
            [renderEncoder setScissorRect:cacheScissorViewport_];
        }
    }

    [self assignMatrix:un.model_mtx pmtx:state.modelMtx hash:&modelMtxCacheHash_];
    [self assignMatrix:un.view_mtx  pmtx:state.viewMtx  hash:&viewMtxHashCache_];
    [self assignMatrix:un.proj_mtx  pmtx:state.projMtx  hash:&projMtxHashCache_];

    
    un.fog_enabled = state.fog ? 1: 0; //Enabled;
    if (state.fog) {
        auto fog = state.fog;
        un.fog_type = (int)fog->type;
        un.fog_depth = (int)fog->depth;
        un.fog_density = fog->density;
        un.fog_z_near = fog->znear;
        un.fog_z_far = fog->zfar;
        [self setUnColor:un.fog_color color:fog->color];
    }

    
    if (cacheDepthTest_ != state.depthTest) {
        if (state.depthTest) {
            [renderEncoder setDepthStencilState:self.depthStencilState];
            //[renderEncoder setFrontFacingWinding:MTLWindingCounterClockwise];
            //[renderEncoder setCullMode:MTLCullModeBack];
        } else {
            [renderEncoder setDepthStencilState:self.depthStencilStateOff];
        }
        cacheDepthTest_ = state.depthTest;
    }

    
    //[renderEncoder setCullMode:MTLCullModeNone];

//    if (std::memcmp(&uniforms_, &cacheUniforms_, sizeof(uniforms_)) != 0) {
    // TODO: bring back uniform checks
        [renderEncoder setVertexBytes:&uniforms_ length:sizeof(uniforms_) atIndex:BufferIndexUniforms];
        [renderEncoder setFragmentBytes:&uniforms_ length:sizeof(uniforms_) atIndex:BufferIndexUniforms];
        
//        cacheUniforms_ = uniforms_;
//    }

    [renderEncoder drawIndexedPrimitives:primitiveType
                              indexCount:cmd->idxCount
                               indexType:MtlBufferIndexType
                             indexBuffer:indexBuffer
                       indexBufferOffset:cmd->idxStart * SizeOfIndexBufferIndex];
};



- (void)submitRender:(id<CAMetalDrawable>)drawable
{
    using namespace render;
    
    const auto& batchBegin = drawBatches_.data();
    const auto& batchEnd = batchBegin + pipelineStatics_.batchHead;
    
    auto batchesCount = std::distance(batchBegin, batchEnd);
    
    if (batchesCount == 0)  // Nothing to render
        return;
    
//    ADebugTrapAssert(batchesCount >= 1);


    auto& un = uniforms_;
    Uniforms lastUniforms;
    
    MTLRenderPassDescriptor* renderPassDesc{};
    id<MTLRenderCommandEncoder> renderEncoder{};
    
    cacheDepthTest_ = false;
    
    dispatch_semaphore_wait(inflightSemaphore_, DISPATCH_TIME_FOREVER);
    
    id<MTLCommandBuffer> commandBuffer = commandQueue_.commandBuffer;
    commandBuffer.label = @"manglCommandBuffer";
    
    __block auto blockSemaphore = inflightSemaphore_;
    [commandBuffer addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull) {
        dispatch_semaphore_signal(blockSemaphore);
    }];

    
    

    if (_depthTexture == nil) {
        auto td = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatDepth32Float
                                                                     width:viewport_.w
                                                                    height:viewport_.h
                                                                 mipmapped:NO];
        td.storageMode = MTLStorageModePrivate;
        td.usage = MTLTextureUsageRenderTarget;
        _depthTexture = [_metalLayer.device newTextureWithDescriptor:td];
        _depthTexture.label = @"depth_stencil";
    }
    
    if (!renderPassDesc) {
        renderPassDesc = MTLRenderPassDescriptor.renderPassDescriptor;
        
        auto att = renderPassDesc.colorAttachments[0];
    
        att.texture = drawable.texture;
        att.loadAction = MTLLoadActionClear;
        att.clearColor = MTLClearColorMake(0, 0, 0, 1);
        att.storeAction = MTLStoreActionStore;
        
        auto da = renderPassDesc.depthAttachment;
        da.texture = _depthTexture;
        
        renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDesc];
        //[renderEncoder setViewport:{0, 0, viewport_.w, viewport_.h, 0, 1}];
        
        [renderEncoder setRenderPipelineState:renderPipelineState_];
        [renderEncoder setDepthStencilState:_depthStencilStateOff];
        cacheDepthTest_ = false;
        cacheVtxTexEnabled_ = false;
        cacheTexSlot_ = -1;
    }

   
    
    for (auto batch = batchBegin; batch != batchEnd; ++batch) {
        
        auto vertexBuffer = vertexBuffers_.getAvail();
        auto indexBuffer = indexBuffers_.getAvail();

        [self sendBuffersData:renderEncoder vertexBuffer:vertexBuffer indexBuffer:indexBuffer batch:batch];
        
        auto cmdBegin = batch->cmdData;
        auto cmdEnd = cmdBegin + batch->cmdCount;
        
        for (auto cmd = cmdBegin; cmd != cmdEnd; ++cmd) {
            [self renderCommand:renderEncoder indexBuffer:indexBuffer cmd:cmd];
        }  // for all commands

        //[renderEncoder endEncoding];
    }  // for all batches
    
    // Clear everything once we are done
    render_statics_reset();
    
    [renderEncoder endEncoding];

    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
}


@end

namespace mangl {

// MARK: Renderer implementation
void SceneRenderer::prepareViewportMetal()
{
    using namespace render;

    cacheScreenScale_ = screenScale_;

    // Metal adjustment matrix
    MatrixHashedGL adjMtx;
    adjMtx.eye();
    adjMtx.a[2][2] = 0.5;
    adjMtx.a[2][3] = 0.5;
    
    //orthoProjMatrix_ = adjMtx * orthoProjMatrix_;
    auto aa = adjMtx * orthoProjMatrix_;
    orthoProjMatrix_.assign(aa);
    orthoProjMatrix_.updateHash();
    currentProjMtx_ = orthoProjMatrix_;
}

void SceneRenderer::prepareSceneMetal()
{
    using namespace render;
    
    if (scenePrepared_)
        return;

    scenePrepared_ = true;
    
    prepareViewport();
   
    //defaultModelMatrixSet_ = true;
    //defaultViewMatrixSet_ = true;
    //defaultProjMatrixSet_ = true;
}

//void SceneRenderer::renderCommitMetal() noexcept
//{
//    [renderViewInstance__ renderCommit];
//}


void SceneRenderer::glLoadTextureMetal(RenderTextureInfo* ti)
{
    using namespace apl;
    
    // Load texture from file
    Image image;
    loadTextureData(ti->imageFile, &image);
    
    ti->size = image.size.convert<SizeI>();
    
    ti->glTextureSlot = ti->index;
    
    auto texDesc = [[MTLTextureDescriptor alloc] init];
    
    texDesc.pixelFormat = MTLPixelFormatBGRA8Unorm;
    
    auto bgra = image.clone(Image::Model::BGRA);
    
    auto [width, height] = ti->size;
    
    texDesc.width = width;
    texDesc.height = height;
    
    ti->textureMetal = [renderViewInstance__.getDevice newTextureWithDescriptor:texDesc];
    
    MTLRegion region {
        {0, 0, 0},         // MTLOrigin
        {texDesc.width, texDesc.height, 1} // MTLSize
    };

    [ti->textureMetal replaceRegion:region
                        mipmapLevel:0
                          withBytes:bgra.data()
                        bytesPerRow:bgra.bytesPerRow];
}

void SceneRenderer::glReleaseTextureMetal(RenderTextureInfo* ti)
{
    ti->textureMetal = nil;
}


} // namespace mangl


#endif  // if Metal

