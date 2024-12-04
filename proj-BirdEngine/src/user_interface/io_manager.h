#pragma once
#include <iostream>
#include <string>
#include <map>

// Enums for warnings and errors
typedef enum {
    ALREADY_INITIALIZED,
} WarningType;

typedef enum {
    FAILED_TO_INITIALIZE,
} ErrorType;

class IOManager {
private:
    constexpr char* general_error_prefix = "Error: ";
    std::map<ErrorType, std::string> prefix_from_error_type = {
        {FAILED_TO_INITIALIZE, "Failed to initialize "}
    };

    constexpr char* general_warning_prefix = "Warning: ";
    std::map<WarningType, std::string> prefix_from_warning_type = {
        {ALREADY_INITIALIZED, "Already initialized "}
    };

public:
    void print(const std::string& msg) {
        std::cout << msg;
    }

    void print_line(const std::string& msg) {
        std::cout << msg;
        std::cout << std::endl;
    }

    // Error printing methods
    void print_line_error(const std::string& msg, ErrorType type) {
        std::cerr << general_error_prefix << prefix_from_error_type[type] << msg;
    }

    // Warning printing methods
    void print_line_warning(const std::string& msg, WarningType type) {
        std::cerr << general_warning_prefix << prefix_from_warning_type[type] << msg;
    }
};
