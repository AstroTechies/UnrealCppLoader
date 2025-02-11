#include "GameInfo/GameInfo.h"
#include "GameInfo/GameSettings.h"
#include "Utilities/MinHook.h"
#include <filesystem>
#include <fstream>
#include <Log/Log.h>
#include <stdexcept>
#include <nlohmann/json.hpp>

DWORD WINAPI MainThread(LPVOID lParam)
{
    try
    {
        auto ConfigPath = std::filesystem::temp_directory_path() / "unrealmodding" / "cpp_loader" / "config.json";
        auto InputStream = std::ifstream(ConfigPath);
        auto Json = nlohmann::json::parse(InputStream);
    
        auto Settings = Json.get<GameSettings::GameSettings>();

        GameProfile::CreateGameProfile(Settings);
    }
    catch (std::exception &e)
    {
        LOG_CRITICAL("Failed to load: {}", e.what());
    }

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Log::Init();
        CreateThread(NULL, NULL, MainThread, NULL, NULL, NULL);
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        break;
    }
    return TRUE;
}