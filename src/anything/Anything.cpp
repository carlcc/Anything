#include "anything/Anything.h"
#include "anything/FileEnumerator.h"
#include "anything/Log.h"
#include "anything/StringUtil.h"

namespace at {

Anything::Anything(const AnythingConfig& config)
    : mConfig(config)
{
}

void Anything::reload()
{
    mAllFiles.clear();

    bool stop = false;
    FileEnumerator enumerator(stop);
    for (auto& path : mConfig.searchPaths()) {
        enumerator.enumerateFiles(path, [this](const cppfs::FileHandle& handle) {
            auto filePath = handle.path();
            for (auto& excludePath : mConfig.excludePaths()) {
                if (str::starts_with(filePath, excludePath)) {
                    return;
                }
            }
            // DEBUG("Enum: {}", filePath);
            mAllFiles.pushBack(filePath);
        });
    }

    DEBUG("All files size = {}", mAllFiles.size());
}

void Anything::reload(bool& stop)
{
    mAllFiles.clear();

    FileEnumerator enumerator(stop);
    for (auto& path : mConfig.searchPaths()) {
        enumerator.enumerateFiles(path, [this](const cppfs::FileHandle& handle) {
            auto filePath = handle.path();
            for (auto& excludePath : mConfig.excludePaths()) {
                if (str::starts_with(filePath, excludePath)) {
                    return;
                }
            }
            // DEBUG("Enum: {}", filePath);
            mAllFiles.pushBack(filePath);
        });
        if (stop) {
            break;
        }
    }

    DEBUG("All files size = {}", mAllFiles.size());
}

void Anything::writeCache()
{
}

}