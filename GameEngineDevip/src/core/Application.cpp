#include "Core/IModule.h"
#include "Core/IGraphics.h"
#include <windows.h>
#include <iostream>

class Application {
    HMODULE graphicsDLL;
    Engine::IGraphics* graphics;
    
public:
    void Run() {
        std::cout << "Loading graphics module...\n";
        
        // Load the graphics DLL
        graphicsDLL = LoadLibraryA("GraphicsModule.dll");
        if (!graphicsDLL) {
            std::cout << "Failed to load GraphicsModule.dll!\n";
            return;
        }
        
        // Get the CreateModule function from the DLL
        auto createFunc = (Engine::IModule*(*)())GetProcAddress(graphicsDLL, "CreateModule");
        if (!createFunc) {
            std::cout << "DLL doesn't have CreateModule function!\n";
            return;
        }
        
        // Create the graphics module
        graphics = dynamic_cast<Engine::IGraphics*>(createFunc());
        if (graphics) {
            graphics->Initialize();
            
            // Main game loop
            bool running = true;
            while (running) {
                graphics->Clear(Engine::Color{0.1f, 0.2f, 0.3f, 1.0f});
                graphics->DrawText("Hello Modular Engine!", Engine::Vector3{10, 10, 0});
                graphics->Update(0.016f); // 60 FPS
                //this is where you shove your code
                
                // In real game, check for quit condition
            }
            
            graphics->Shutdown();
            
            // Clean up
            auto destroyFunc = (void(*)(Engine::IModule*))GetProcAddress(graphicsDLL, "DestroyModule");
            if (destroyFunc) {
                destroyFunc(graphics);
            }
        }
        
        FreeLibrary(graphicsDLL);
    }
};

int main() {
    Application app;
    app.Run();
    return 0;
}