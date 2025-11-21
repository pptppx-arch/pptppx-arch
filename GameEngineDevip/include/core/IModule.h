#pragma once
#include "CommonTypes.h"

namespace Engine {
    // Forward declarations
    class IApplication;

    // Module information structure
    struct ModuleInfo {
        const char* name;
        const char* version;
        const char* description;
        ModulePriority priority;
        uint32 minimumApiVersion;
    };

    //! @brief Base interface for all engine modules
    //! @details All modules (Graphics, Audio, Physics, etc.) must implement this interface
    class IModule {
    public:
        virtual ~IModule() = default;

        //! @brief Called when the module is loaded and should initialize itself
        //! @param app Pointer to the main application instance
        //! @return Result::Success if initialization succeeded, error code otherwise
        virtual Result Initialize(IApplication* app) = 0;

        //! @brief Called every frame to update the module
        //! @param deltaTime Time in seconds since last frame
        virtual void Update(float32 deltaTime) = 0;

        //! @brief Called when the module is about to be unloaded
        virtual void Shutdown() = 0;

        //! @brief Get the name of this module
        //! @return Module name as string
        virtual String GetName() const = 0;

        //! @brief Get the version of this module
        //! @return Version string (e.g., "1.2.3")
        virtual String GetVersion() const = 0;

        //! @brief Get the priority of this module for initialization order
        //! @return ModulePriority value
        virtual ModulePriority GetPriority() const = 0;

        //! @brief Check if this module is currently initialized and running
        //! @return true if initialized, false otherwise
        virtual bool IsInitialized() const = 0;

        //! @brief Get detailed information about this module
        //! @return ModuleInfo structure with module metadata
        virtual ModuleInfo GetModuleInfo() const = 0;
    };

    //! @brief Application interface that modules can use to interact with the engine
    class IApplication {
    public:
        virtual ~IApplication() = default;

        //! @brief Get another module by name
        //! @param moduleName Name of the module to retrieve
        //! @return Pointer to the module, or nullptr if not found
        virtual IModule* GetModule(const String& moduleName) = 0;

        //! @brief Check if a module is currently loaded
        //! @param moduleName Name of the module to check
        //! @return true if module is loaded, false otherwise
        virtual bool HasModule(const String& moduleName) = 0;

        //! @brief Get the root directory for game data
        //! @return Path to data directory
        virtual String GetDataPath() const = 0;

        //! @brief Get the application window width
        //! @return Window width in pixels
        virtual uint32 GetWindowWidth() const = 0;

        //! @brief Get the application window height
        //! @return Window height in pixels
        virtual uint32 GetWindowHeight() const = 0;

        //! @brief Check if application is running
        //! @return true if application should continue running
        virtual bool IsRunning() const = 0;

        //! @brief Request application to shutdown
        virtual void RequestShutdown() = 0;

        //! @brief Log a message to the engine's log system
        //! @param message The message to log
        virtual void LogMessage(const String& message) = 0;
    };
}

// Standardized export functions - every DLL MUST implement these
extern "C" {
    //! @brief Create an instance of the module
    //! @return Pointer to new module instance
    ENGINE_API Engine::IModule* ENGINE_CALL CreateModule();

    //! @brief Destroy a module instance
    //! @param module Pointer to module instance to destroy
    ENGINE_API void ENGINE_CALL DestroyModule(Engine::IModule* module);

    //! @brief Get module metadata
    //! @return ModuleInfo structure with module information
    ENGINE_API Engine::ModuleInfo ENGINE_CALL GetModuleInfo();
}