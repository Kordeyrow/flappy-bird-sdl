#pragma once
#include <iostream>
#include <string>
#include <map>

namespace WING {
    typedef enum {
        FAILED_TO_CREATE,
        FAILED_TO_INITIALIZE,
        FAILED_LOADING,
    } ErrorType;

    typedef enum {
        ALREADY_CREATED,
        ALREADY_INITIALIZED,
        ALREADY_LOADED,
    } WarningType;


    class IOManager {
    private:
        const char* general_error_prefix = "Error: ";
        std::map<ErrorType, std::string> prefix_from_error_type = {
            {FAILED_TO_CREATE, "Failed to create "},
            {FAILED_TO_INITIALIZE, "Failed to initialize "},
            {FAILED_LOADING, "Failed to load "}
        };

        const char* general_warning_prefix = "Warning: ";
        std::map<WarningType, std::string> prefix_from_warning_type = {
            {ALREADY_CREATED, "Already created "},
            {ALREADY_INITIALIZED, "Already initialized "},
            {ALREADY_LOADED, "Already loaded "}
        };

    public:
        IOManager() {
        }
        ~IOManager() {
        }

        void print(const std::string& msg) {
            std::cout << msg;
        }

        void print_line(const std::string& msg) {
            std::cout << msg << std::endl;
        }

        // Error printing methods
        void print_error(const std::string& msg, ErrorType type) {
            std::cerr << general_error_prefix << prefix_from_error_type[type] << msg << std::endl;
        }

        // Warning printing methods
        void print_warning(const std::string& msg, WarningType type) {
            std::cerr << general_warning_prefix << prefix_from_warning_type[type] << msg << std::endl;
        }
    };
}