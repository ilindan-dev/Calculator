#ifndef CALCULATOR_DLL_LOADER_HPP
#define CALCULATOR_DLL_LOADER_HPP

#include <string>

// Platform-specific headers for dynamic library loading.
#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

// DllLoader manages the lifetime of a dynamic library handle using the RAII idiom.
// It abstracts platform differences between Windows (LoadLibrary) and Linux (dlopen).
class DllLoader {
public:
    // HandleType defines the underlying raw handle type based on the operating system.
#ifdef _WIN32
    using HandleType = HMODULE;
#else
    using HandleType = void*;
#endif

    // Constructor: Loads the dynamic library from the specified path.
    // Argument is passed by value to utilize move semantics (sink parameter).
    // Throws std::runtime_error if the library cannot be loaded.
    explicit DllLoader(std::string libraryPath);

    // Destructor: Automatically unloads the library when the object goes out of scope.
    ~DllLoader();

    // --- Rule of 5 Implementation ---

    // Copy Constructor is deleted to prevent multiple objects from owning the same raw handle (Unique Ownership).
    DllLoader(const DllLoader&) = delete;

    // Copy Assignment Operator is deleted for the same reason.
    DllLoader& operator=(const DllLoader&) = delete;

    // Move Constructor: Transfers ownership of the library handle from 'other' to this object.
    DllLoader(DllLoader&& other) noexcept;

    // Move Assignment Operator: Transfers ownership, unloading the current library if necessary.
    DllLoader& operator=(DllLoader&& other) noexcept;

    // getPath returns the file path of the loaded library.
    [[nodiscard]] const std::string& getPath() const;

    // getSymbol retrieves a function pointer (symbol) from the loaded library.
    // T must be a function pointer type.
    // Returns nullptr if the symbol is not found.
    template <typename T>
    T getSymbol(const std::string& name) const {
#ifdef _WIN32
        // GetProcAddress returns FARPROC, requiring a reinterpret_cast.
        auto ptr = reinterpret_cast<T>(GetProcAddress(handle, name.c_str()));
#else
        // dlsym returns void*, requiring a reinterpret_cast.
        auto ptr = reinterpret_cast<T>(dlsym(handle, name.c_str()));
#endif
        return ptr;
    }

private:
    // unload frees the system resource associated with the library handle.
    void unload();

    // handle is the raw OS-specific handle to the dynamic library.
    HandleType handle{nullptr};

    // path stores the file path for debugging or info purposes.
    std::string path;
};

#endif //CALCULATOR_DLL_LOADER_HPP