#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <regex>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#include <unistd.h>
#endif

class SettingsReader {
public:
    struct RustSettings {
        float fov;
        float sensitivity;
        bool valid;
        
        RustSettings() : fov(90.0f), sensitivity(0.5f), valid(false) {}
    };

    static RustSettings GetRustSettings() {
        RustSettings settings;
        try {
            std::string rustPath = GetRustPath();
            if (rustPath.empty()) {
                return settings;
            }

            // Read client.cfg
            std::string configPath = rustPath + "/cfg/client.cfg";
            std::ifstream file(configPath);
            if (!file.is_open()) {
                return settings;
            }

            std::string line;
            bool foundFov = false;
            bool foundSens = false;

            while (std::getline(file, line)) {
                if (!foundFov && line.find("graphics.fov") != std::string::npos) {
                    size_t pos = line.find("\"");
                    if (pos != std::string::npos) {
                        size_t end = line.find("\"", pos + 1);
                        if (end != std::string::npos) {
                            settings.fov = std::stof(line.substr(pos + 1, end - pos - 1));
                            foundFov = true;
                        }
                    }
                }
                if (!foundSens && line.find("input.sensitivity") != std::string::npos) {
                    size_t pos = line.find("\"");
                    if (pos != std::string::npos) {
                        size_t end = line.find("\"", pos + 1);
                        if (end != std::string::npos) {
                            settings.sensitivity = std::stof(line.substr(pos + 1, end - pos - 1));
                            foundSens = true;
                        }
                    }
                }
                if (foundFov && foundSens) {
                    settings.valid = true;
                    break;
                }
            }
        }
        catch (...) {
            // Silently fail
        }
        return settings;
    }

private:
    static std::string GetRustPath() {
#ifdef _WIN32
        HKEY hKey;
        const char* keyPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Steam App 252490";
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, keyPath, 0, KEY_READ | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS) {
            return "";
        }

        char value[1024];
        DWORD valueSize = sizeof(value);
        DWORD valueType;
        
        if (RegQueryValueExA(hKey, "InstallLocation", nullptr, &valueType, (LPBYTE)value, &valueSize) != ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return "";
        }

        RegCloseKey(hKey);
        return std::string(value);
#else
        // On Linux, try common Steam library locations
        const char* home = getenv("HOME");
        if (!home) return "";

        std::string steamPath = std::string(home) + "/.steam/steam";
        std::string rustPath = steamPath + "/steamapps/common/Rust";
        
        std::ifstream rustCheck((rustPath + "/cfg/client.cfg").c_str());
        if (rustCheck.good()) {
            return rustPath;
        }

        // Try alternative Steam library locations
        std::string libraryPath = steamPath + "/steamapps/libraryfolders.vdf";
        std::ifstream libraryFile(libraryPath.c_str());
        if (libraryFile.good()) {
            std::string line;
            while (std::getline(libraryFile, line)) {
                if (line.find("path") != std::string::npos) {
                    size_t start = line.find('"', line.find('"') + 1) + 1;
                    size_t end = line.find('"', start);
                    if (start != std::string::npos && end != std::string::npos) {
                        std::string path = line.substr(start, end - start);
                        rustPath = path + "/steamapps/common/Rust";
                        std::ifstream altCheck((rustPath + "/cfg/client.cfg").c_str());
                        if (altCheck.good()) {
                            return rustPath;
                        }
                    }
                }
            }
        }
        return "";
#endif
    }
};
