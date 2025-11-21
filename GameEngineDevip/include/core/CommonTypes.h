#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define ENGINE_PLATFORM_WINDOWS 1
    #ifdef _WIN64
        #define ENGINE_PLATFORM_WINDOWS_64 1
    #else
        #define ENGINE_PLATFORM_WINDOWS_32 1
    #endif
#elif defined(__linux__)
    #define ENGINE_PLATFORM_LINUX 1
#elif defined(__APPLE__)
    #define ENGINE_PLATFORM_MACOS 1
#else
    #error "Unknown platform"
#endif

// DLL export/import macros
#if ENGINE_PLATFORM_WINDOWS
    #ifdef BUILDING_DLL
        #define ENGINE_API __declspec(dllexport)
    #else
        #define ENGINE_API __declspec(dllimport)
    #endif
    #define ENGINE_CALL __cdecl
#else
    #define ENGINE_API __attribute__((visibility("default")))
    #define ENGINE_CALL
#endif

// Basic type definitions
namespace Engine {
    // Primitive types
    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;
    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
    using float32 = float;
    using float64 = double;

    // String types
    using String = std::string;
    using WString = std::wstring;

    // Container types
    template<typename T>
    using Array = std::vector<T>;

    template<typename Key, typename Value>
    using Map = std::unordered_map<Key, Value>;

    // Smart pointers
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using WeakRef = std::weak_ptr<T>;

    template<typename T>
    using UniqueRef = std::unique_ptr<T>;

    // Math types
    struct Vector2 {
        float32 x, y;
        
        Vector2() : x(0), y(0) {}
        Vector2(float32 x, float32 y) : x(x), y(y) {}
        
        // Basic operations
        Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
        Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
        Vector2 operator*(float32 scalar) const { return Vector2(x * scalar, y * scalar); }
        
        float32 Length() const { return sqrtf(x*x + y*y); }
        Vector2 Normalized() const { float32 len = Length(); return len > 0 ? Vector2(x/len, y/len) : Vector2(); }
    };

    struct Vector3 {
        float32 x, y, z;
        
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float32 x, float32 y, float32 z) : x(x), y(y), z(z) {}
        
        Vector3 operator+(const Vector3& other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
        Vector3 operator-(const Vector3& other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
        Vector3 operator*(float32 scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
        
        float32 Length() const { return sqrtf(x*x + y*y + z*z); }
        Vector3 Normalized() const { float32 len = Length(); return len > 0 ? Vector3(x/len, y/len, z/len) : Vector3(); }
    };

    struct Vector4 {
        float32 x, y, z, w;
        
        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float32 x, float32 y, float32 z, float32 w) : x(x), y(y), z(z), w(w) {}
    };

    struct Color {
        float32 r, g, b, a;
        
        Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
        Color(float32 r, float32 g, float32 b, float32 a = 1.0f) : r(r), g(g), b(b), a(a) {}
        
        static Color Red() { return Color(1.0f, 0.0f, 0.0f); }
        static Color Green() { return Color(0.0f, 1.0f, 0.0f); }
        static Color Blue() { return Color(0.0f, 0.0f, 1.0f); }
        static Color White() { return Color(1.0f, 1.0f, 1.0f); }
        static Color Black() { return Color(0.0f, 0.0f, 0.0f); }
    };

    struct Rectangle {
        float32 x, y, width, height;
        
        Rectangle() : x(0), y(0), width(0), height(0) {}
        Rectangle(float32 x, float32 y, float32 width, float32 height) : x(x), y(y), width(width), height(height) {}
        
        bool Contains(const Vector2& point) const {
            return point.x >= x && point.x <= x + width && point.y >= y && point.y <= y + height;
        }
    };

    // Result codes
    enum class Result {
        Success = 0,
        Failure = 1,
        FileNotFound = 2,
        InvalidParameter = 3,
        OutOfMemory = 4,
        NotInitialized = 5,
        AlreadyInitialized = 6,
        PlatformError = 7
    };

    // Module priority for initialization order
    enum class ModulePriority {
        Core = 0,      // Must load first
        System = 100,  // Low-level systems
        Engine = 200,  // Engine core systems  
        Game = 300,    // Gameplay systems
        UI = 400       // User interface (load last)
    };
}