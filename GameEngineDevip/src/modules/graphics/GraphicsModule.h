#pragma once
// Define this when building the DLL
#define BUILDING_DLL
#include "Core/IGraphics.h"
#include "Core/CommonTypes.h"

// Forward declarations for internal implementation
struct ID3D11Device;
struct ID3D11DeviceContext;

class GraphicsModule : public Engine::IGraphics {
private:
    // PRIVATE implementation details
    ID3D11Device* m_Device;
    ID3D11DeviceContext* m_Context;
    bool m_Initialized;
    
    // Internal methods that aren't part of the interface
    bool InitializeDirectX();
    void CleanupDirectX();
    
public:
    // PUBLIC interface implementation
    GraphicsModule();
    virtual ~GraphicsModule();
    
    // IModule interface
    Engine::Result Initialize(Engine::IApplication* app) override;
    void Update(Engine::float32 deltaTime) override;
    void Shutdown() override;
    Engine::String GetName() const override { return "DirectX11 Graphics"; }
    Engine::String GetVersion() const override { return "1.0.0"; }
    Engine::ModulePriority GetPriority() const override { return Engine::ModulePriority::System; }
    bool IsInitialized() const override { return m_Initialized; }
    Engine::ModuleInfo GetModuleInfo() const override;
    
    // IGraphics interface
    Engine::TextureHandle CreateTexture(const Engine::TextureDesc& desc) override;
    Engine::TextureHandle LoadTexture(const Engine::String& filePath, Engine::TextureUsage usage) override;
    void DestroyTexture(Engine::TextureHandle texture) override;
    // ... implement ALL other IGraphics methods
};