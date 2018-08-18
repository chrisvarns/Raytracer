/*
See LICENSE folder for this sample’s licensing information.

Abstract:
Implementation of our platform independent renderer class, which performs Metal setup and per frame rendering
*/
#import <simd/simd.h>
#import <MetalKit/MetalKit.h>
#import <ImageIO/ImageIO.h>
#import <CoreGraphics/CoreGraphics.h>

#include <vector>

#import "AAPLRenderer.h"

// Header shared between C code here, which executes Metal API commands, and .metal files, which
//   uses these types as inputs to the shaders
#import "AAPLShaderTypes.h"

#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])

// Main class performing the rendering
@implementation AAPLRenderer
{
    // The device (aka GPU) we're using to render
    id<MTLDevice> _device;

    // Our render pipeline composed of our vertex and fragment shaders in the .metal shader file
    id<MTLRenderPipelineState> _pipelineState;

    // The command Queue from which we'll obtain command buffers
    id<MTLCommandQueue> _commandQueue;

    // The current size of our view so we can use this in our render pipeline
    vector_uint2 _viewportSize;
    
    bool _redrawBackBuffer;
    
    std::vector<uint8>  _backBuffer;
    uint8*              _backBufferPtr;
    id<MTLTexture>      _backBufferTex;
    
    NSString* _outputPath;
}

/// Initialize with the MetalKit view from which we'll obtain our Metal device
- (nonnull instancetype)initWithMetalKitView:(nonnull MTKView *)mtkView
{
    self = [super init];
    if(self)
    {
        NSError *error = NULL;

        _device = mtkView.device;

        // Load all the shader files with a .metal file extension in the project
        id<MTLLibrary> defaultLibrary = [_device newDefaultLibrary];

        // Load the vertex function from the library
        id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexShader"];

        // Load the fragment function from the library
        id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentShader"];

        // Configure a pipeline descriptor that is used to create a pipeline state
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
        pipelineStateDescriptor.label = @"Simple Pipeline";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = mtkView.colorPixelFormat;

        _pipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineStateDescriptor
                                                                 error:&error];
        if (!_pipelineState)
        {
            // Pipeline State creation could fail if we haven't properly set up our pipeline descriptor.
            //  If the Metal API validation is enabled, we can find out more information about what
            //  went wrong.  (Metal API validation is enabled by default when a debug build is run
            //  from Xcode)
            NSLog(@"Failed to created pipeline state, error %@", error);
            return nil;
        }

        // Create the command queue
        _commandQueue = [_device newCommandQueue];
        
        _outputPath = [[NSProcessInfo processInfo] arguments][1];
        _outputPath = [_outputPath stringByAppendingString:@"/output.png"];
    }

    return self;
}

- (void)createBackBufferTex {
    _backBuffer.resize(_viewportSize.x * _viewportSize.y * sizeof(uint8) * 4);
    _backBufferPtr = _backBuffer.data();
    
    MTLTextureDescriptor* texDesc = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:MTLPixelFormatRGBA8Unorm width:_viewportSize.x height:_viewportSize.y mipmapped:NO];
    
    _backBufferTex = [_device newTextureWithDescriptor:texDesc];
}

/// Called whenever view changes orientation or is resized
- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size
{
    // Save the size of the drawable as we'll pass these
    //   values to our vertex shader when we draw
    _viewportSize.x = size.width;
    _viewportSize.y = size.height;
    [self createBackBufferTex];
    _redrawBackBuffer = true;
}

- (void)redrawBackBuffer {
    uint8* outPtr = _backBufferPtr;
    for(auto j = 0; j < _viewportSize.y; j++) {
        for(auto i = 0; i < _viewportSize.x; i++) {
            float r = float(i) / _viewportSize.x;
            float g = float(j) / _viewportSize.y;
            float b = 0.2f;
            *(outPtr++) = uint8(255.99 * r);
            *(outPtr++) = uint8(255.99 * g);
            *(outPtr++) = uint8(255.99 * b);
            *(outPtr++) = 255;
        }
    }
    
    [self writeImageToDisk];
    
    [_backBufferTex replaceRegion:MTLRegionMake2D(0, 0, _viewportSize.x, _viewportSize.y) mipmapLevel:0 withBytes:_backBufferPtr bytesPerRow:_viewportSize.x*4];
}



- (void)writeImageToDisk {
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    //NSData* nsData = [NSData dataWithBytes:_backBufferPtr length:_backBuffer.size()];
    CFDataRef cfDataRef = CFDataCreate(nil, _backBuffer.data(), _backBuffer.size());
    CGDataProviderRef provider = CGDataProviderCreateWithCFData(cfDataRef);
    
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault | kCGImageAlphaNoneSkipLast;
    CGImageRef cgImage = CGImageCreate(_viewportSize.x, _viewportSize.y, 8, 32, 4*_viewportSize.x, colorSpace, bitmapInfo, provider, nil, NO, kCGRenderingIntentDefault);
    
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(provider);
    
    NSURL* fileUrl = [NSURL fileURLWithPath:_outputPath];
    CGImageDestinationRef imageDest = CGImageDestinationCreateWithURL((CFURLRef)fileUrl, kUTTypePNG, 1, nil);
    CGImageDestinationAddImage(imageDest, cgImage, nil);
    CGImageDestinationFinalize(imageDest);
    
    CGImageRelease(cgImage);
    CFRelease(imageDest);
}

/// Called whenever the view needs to render a frame
- (void)drawInMTKView:(nonnull MTKView *)view
{
    static const AAPLVertex triangleVertices[] =
    {
        { {  1,  -1 }, { 1, 0 } },
        { { -1,  -1 }, { 0, 0 } },
        { { -1,   1 }, { 0, 1 } },
        { {  1,  -1 }, { 1, 0 } },
        { { -1,   1 }, { 0, 1 } },
        { {  1,   1 }, { 1, 1 } },
    };
    
    if(_redrawBackBuffer) {
        _redrawBackBuffer = false;
        [self redrawBackBuffer];
    }

    // Create a new command buffer for each render pass to the current drawable
    id<MTLCommandBuffer> commandBuffer = [_commandQueue commandBuffer];
    commandBuffer.label = @"MyCommand";

    // Obtain a renderPassDescriptor generated from the view's drawable textures
    MTLRenderPassDescriptor *renderPassDescriptor = view.currentRenderPassDescriptor;

    if(renderPassDescriptor != nil)
    {
        // Create a render command encoder so we can render into something
        id<MTLRenderCommandEncoder> renderEncoder =
        [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
        renderEncoder.label = @"MyRenderEncoder";

        [renderEncoder setRenderPipelineState:_pipelineState];

        // We call -[MTLRenderCommandEncoder setVertexBytes:length:atIndex:] to send data from our
        //   Application ObjC code here to our Metal 'vertexShader' function
        // This call has 3 arguments
        //   1) A pointer to the memory we want to pass to our shader
        //   2) The memory size of the data we want passed down
        //   3) An integer index which corresponds to the index of the buffer attribute qualifier
        //      of the argument in our 'vertexShader' function

        // You send a pointer to the `triangleVertices` array also and indicate its size
        // The `AAPLVertexInputIndexVertices` enum value corresponds to the `vertexArray`
        // argument in the `vertexShader` function because its buffer attribute also uses
        // the `AAPLVertexInputIndexVertices` enum value for its index
        [renderEncoder setVertexBytes:triangleVertices
                               length:sizeof(triangleVertices)
                              atIndex:AAPLVertexInputIndexVertices];
        
        [renderEncoder setFragmentTexture:_backBufferTex
                                  atIndex:0];

        // Draw the 3 vertices of our triangle
        [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle
                          vertexStart:0
                          vertexCount:ARRAYSIZE(triangleVertices)];

        [renderEncoder endEncoding];

        // Schedule a present once the framebuffer is complete using the current drawable
        [commandBuffer presentDrawable:view.currentDrawable];
    }

    // Finalize rendering here & push the command buffer to the GPU
    [commandBuffer commit];
}

@end