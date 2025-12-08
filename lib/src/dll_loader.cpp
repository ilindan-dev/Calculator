#include "dll_loader.hpp"

#include <stdexcept>
#include <utility> // for std::exchange, std::move

// Constructor: Loads the dynamic library from the specified path.
// Argument is passed by value to utilize move semantics (sink parameter).
// Throws std::runtime_error if the library cannot be loaded.
DllLoader::DllLoader(std::string libraryPath) : path(std::move(libraryPath)) {
#ifdef _WIN32
    handle = LoadLibraryA(path.c_str());
#else
    handle = dlopen(path.c_str(), RTLD_NOW);
#endif

    if (!handle) {
        std::string errorMsg = "Failed to load plugin: " + path;
#ifndef _WIN32
        if (const char* err = dlerror()) {
            errorMsg += " Error: ";
            errorMsg += err;
        }
#endif
        throw std::runtime_error(errorMsg);
    }
}

// Destructor: Automatically unloads the library when the object goes out of scope.
DllLoader::~DllLoader() {
    unload();
}

// Move Constructor: Transfers ownership of the library handle from 'other' to this object.
DllLoader::DllLoader(DllLoader&& other) noexcept
    : handle(std::exchange(other.handle, nullptr)),
      path(std::move(other.path)) {}

// Move Assignment Operator: Transfers ownership, unloading the current library if necessary.
DllLoader& DllLoader::operator=(DllLoader&& other) noexcept {
    if (this != &other) {
        unload();
        handle = std::exchange(other.handle, nullptr);
        path = std::move(other.path);
    }
    return *this;
}

// getPath returns the file path of the loaded library.
const std::string& DllLoader::getPath() const {
    return path;
}

// unload frees the system resource associated with the library handle.
void DllLoader::unload() {
    if (handle) {
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
        handle = nullptr;
    }
}