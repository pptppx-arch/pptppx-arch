#pragma once
#include "IModule.h"

namespace Engine {
    //! @brief Rendering API types
    enum class RenderAPI {
        DirectX11,
        DirectX12,
        OpenGL,
        Vulkan,
        Metal
    };

    //! @brief Texture formats
    enum class TextureFormat {
        R8_UNorm,
        RGBA8_UNorm,
        RGBA16_Float,
        Depth32_Float,
        BC1_UNorm,  // DXT1
        BC3_UNorm   // DXT5
    };

    //! @brief Texture usage flags
    enum class TextureUsage {
        Static = 0,     // Loaded once, rarely changed
        Dynamic = 1,    // Updated frequently
        RenderTarget = 2 // Used as render target
    };

    //! @brief Shader types
    enum class ShaderType {
        Vertex,
        Pixel,
        Geometry,
        Compute
    };

    //! @brief Mesh primitive types
    enum class PrimitiveType {
        Triangles,
        Lines,
        Points
    };

    //! @brief Graphics resource handle
    struct TextureHandle {
        uint64 id;
        
        TextureHandle() : id(0) {}
        explicit TextureHandle(uint64 id) : id(id) {}
        
        bool IsValid() const { return id != 0; }
        bool operator==(const TextureHandle& other) const { return id == other.id; }
    };

    struct ShaderHandle {
        uint64 id;
        
        ShaderHandle() : id(0) {}
        explicit ShaderHandle(uint64 id) : id(id) {}
        
        bool IsValid() const { return id != 0; }
        bool operator==(const ShaderHandle& other) const { return id == other.id; }
    };

    struct MeshHandle {
        uint64 id;
        
        MeshHandle() : id(0) {}
        explicit MeshHandle(uint64 id) : id(id) {}
        
        bool IsValid() const { return id != 0; }
        bool operator==(const MeshHandle& other) const { return id == other.id; }
    };

    //! @brief Texture creation parameters
    struct TextureDesc {
        uint32 width;
        uint32 height;
        TextureFormat format;
        TextureUsage usage;
        uint32 mipLevels;
        const char* debugName;
        
        TextureDesc() : width(0), height(0), format(TextureFormat::RGBA8_UNorm), 
                       usage(TextureUsage::Static), mipLevels(1), debugName(nullptr) {}
    };

    //! @brief Shader creation parameters
    struct ShaderDesc {
        ShaderType type;
        const char* sourceCode;
        size_t sourceLength;
        const char* entryPoint;
        const char* debugName;
        
        ShaderDesc() : type(ShaderType::Vertex), sourceCode(nullptr), 
                      sourceLength(0), entryPoint("main"), debugName(nullptr) {}
    };

    //! @brief Mesh creation parameters
    struct MeshDesc {
        PrimitiveType primitiveType;
        const void* vertexData;
        uint32 vertexCount;
        uint32 vertexStride;
        const uint32* indexData;
        uint32 indexCount;
        const char* debugName;
        
        MeshDesc() : primitiveType(PrimitiveType::Triangles), vertexData(nullptr),
                    vertexCount(0), vertexStride(0), indexData(nullptr),
                    indexCount(0), debugName(nullptr) {}
    };

    //! @brief Rendering statistics
    struct RenderStats {
        uint32 drawCalls;
        uint32 trianglesDrawn;
        uint32 texturesLoaded;
        uint32 shadersLoaded;
        float32 frameTime;
        
        RenderStats() : drawCalls(0), trianglesDrawn(0), texturesLoaded(0), 
                       shadersLoaded(0), frameTime(0.0f) {}
    };

    //! @brief Camera parameters for rendering
    struct Camera {
        Vector3 position;
        Vector3 direction;
        Vector3 up;
        float32 fov;
        float32 nearPlane;
        float32 farPlane;
        
        Camera() : position(0, 0, 0), direction(0, 0, 1), up(0, 1, 0),
                  fov(60.0f), nearPlane(0.1f), farPlane(1000.0f) {}
    };

    //! @brief Graphics module interface
    //! @details Handles all rendering operations and graphics resource management
    class IGraphics : public IModule {
    public:
        virtual ~IGraphics() = default;

        // Resource Management
        //! @brief Create a texture from description
        //! @param desc Texture description
        //! @return Handle to created texture, or invalid handle on failure
        virtual TextureHandle CreateTexture(const TextureDesc& desc) = 0;

        //! @brief Create a texture from file
        //! @param filePath Path to texture file
        //! @param usage Texture usage flags
        //! @return Handle to created texture
        virtual TextureHandle LoadTexture(const String& filePath, TextureUsage usage = TextureUsage::Static) = 0;

        //! @brief Destroy a texture
        //! @param texture Handle to texture to destroy
        virtual void DestroyTexture(TextureHandle texture) = 0;

        //! @brief Create a shader
        //! @param desc Shader description
        //! @return Handle to created shader
        virtual ShaderHandle CreateShader(const ShaderDesc& desc) = 0;

        //! @brief Load a shader from file
        //! @param filePath Path to shader file
        //! @param type Shader type
        //! @return Handle to created shader
        virtual ShaderHandle LoadShader(const String& filePath, ShaderType type) = 0;

        //! @brief Destroy a shader
        //! @param shader Handle to shader to destroy
        virtual void DestroyShader(ShaderHandle shader) = 0;

        //! @brief Create a mesh
        //! @param desc Mesh description
        //! @return Handle to created mesh
        virtual MeshHandle CreateMesh(const MeshDesc& desc) = 0;

        //! @brief Load a mesh from file
        //! @param filePath Path to mesh file
        //! @return Handle to created mesh
        virtual MeshHandle LoadMesh(const String& filePath) = 0;

        //! @brief Destroy a mesh
        //! @param mesh Handle to mesh to destroy
        virtual void DestroyMesh(MeshHandle mesh) = 0;

        // Rendering Commands
        //! @brief Begin a new frame
        virtual void BeginFrame() = 0;

        //! @brief End the current frame and present to screen
        virtual void EndFrame() = 0;

        //! @brief Clear the screen
        //! @param color Clear color
        virtual void Clear(const Color& color) = 0;

        //! @brief Set the active camera for rendering
        //! @param camera Camera parameters
        virtual void SetCamera(const Camera& camera) = 0;

        //! @brief Draw a mesh
        //! @param mesh Mesh to draw
        //! @param worldTransform World transformation matrix
        virtual void DrawMesh(MeshHandle mesh, const Vector3& position, const Vector3& rotation, const Vector3& scale) = 0;

        //! @brief Draw 2D texture
        //! @param texture Texture to draw
        //! @param destination Destination rectangle
        virtual void DrawTexture(TextureHandle texture, const Rectangle& destination) = 0;

        // State Management
        //! @brief Set viewport
        //! @param x Viewport X position
        //! @param y Viewport Y position
        //! @param width Viewport width
        //! @param height Viewport height
        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;

        //! @brief Get the rendering API being used
        //! @return RenderAPI value
        virtual RenderAPI GetRenderAPI() const = 0;

        //! @brief Get rendering statistics for the last frame
        //! @return RenderStats structure with statistics
        virtual RenderStats GetStats() const = 0;

        //! @brief Check if a specific texture format is supported
        //! @param format Texture format to check
        //! @return true if supported, false otherwise
        virtual bool IsTextureFormatSupported(TextureFormat format) const = 0;

        //! @brief Get maximum texture size supported by the hardware
        //! @return Maximum texture dimension in pixels
        virtual uint32 GetMaxTextureSize() const = 0;

        //! @brief Reload all shaders (for hot-reloading during development)
        virtual void ReloadShaders() = 0;
    };
}