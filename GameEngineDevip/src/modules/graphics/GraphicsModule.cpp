#include "modules/graphics/GraphicsModule.h"
#include <d3d11.h>
#include <vector>

// Internal data structures (hidden from interface)
struct TextureData {
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* view;
    uint32_t width, height;
};

// Global list of loaded textures (implementation detail)
static std::vector<TextureData> s_Textures;

GraphicsModule::GraphicsModule() 
    : m_Device(nullptr), m_Context(nullptr), m_Initialized(false) {
}

GraphicsModule::~GraphicsModule() {
    Shutdown();
}

Engine::Result GraphicsModule::Initialize(Engine::IApplication* app) {
    if (m_Initialized) {
        return Engine::Result::AlreadyInitialized;
    }
    
    // Initialize DirectX 11 (IMPLEMENTATION SPECIFIC)
    if (!InitializeDirectX()) {
        return Engine::Result::Failure;
    }
    
    m_Initialized = true;
    return Engine::Result::Success;
}

bool GraphicsModule::InitializeDirectX() {
    // ↓↓↓ THIS IS IMPLEMENTATION-SPECIFIC CODE ↓↓↓
    D3D_FEATURE_LEVEL featureLevel;
    HRESULT hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION,
        &m_Device, &featureLevel, &m_Context
    );
    
    return SUCCEEDED(hr);
    // ↑↑↑ ONLY THE DIRECTX IMPLEMENTATION CARES ABOUT THIS ↑↑↑
}

Engine::TextureHandle GraphicsModule::CreateTexture(const Engine::TextureDesc& desc) {
    if (!m_Initialized) return Engine::TextureHandle();
    
    // ↓↓↓ ACTUAL DIRECTX TEXTURE CREATION ↓↓↓
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = desc.width;
    texDesc.Height = desc.height;
    texDesc.MipLevels = desc.mipLevels;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Convert from our format
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    
    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = m_Device->CreateTexture2D(&texDesc, nullptr, &texture);
    
    if (SUCCEEDED(hr)) {
        TextureData data;
        data.texture = texture;
        data.width = desc.width;
        data.height = desc.height;
        s_Textures.push_back(data);
        
        return Engine::TextureHandle(s_Textures.size()); // Return handle
    }
    
    return Engine::TextureHandle(); // Invalid handle
}

// ... implement dozens more methods

// DLL exports - this makes it a loadable module
extern "C" {
    ENGINE_API Engine::IModule* ENGINE_CALL CreateModule() {
        return new GraphicsModule();
    }
    
    ENGINE_API void ENGINE_CALL DestroyModule(Engine::IModule* module) {
        delete static_cast<GraphicsModule*>(module);
    }
    
    ENGINE_API Engine::ModuleInfo ENGINE_CALL GetModuleInfo() {
        return {
            "DirectX11 Graphics", 
            "1.0.0", 
            "DirectX 11 rendering implementation",
            Engine::ModulePriority::System,
            1
        };
    }
}