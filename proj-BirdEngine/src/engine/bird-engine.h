#pragma once
#include <dll/export_def.h>
#include <memory>

// Exported class from the DLL
class BIRDENGINE_API BirdEngine {
public:
    // Deleted copy constructor and assignment operator
    BirdEngine(const BirdEngine&) = delete;
    BirdEngine& operator=(const BirdEngine&) = delete;

    // Static method to access the singleton instance
    static BirdEngine& instance();

    void init();
    void run();

private:
    // Private constructor and destructor
    BirdEngine();
    ~BirdEngine();

    // Pimpl idiom: Pointer to the implementation
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};
