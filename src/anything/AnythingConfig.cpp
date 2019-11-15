#include "anything/AnythingConfig.h"
#include "JsonUtil.h"
#include "anything/Log.h"
#include "anything/StringUtil.h"
#include <cppfs/InputStream.h>
#include <fstream>
#include <json/reader.h>
#include <json/value.h>

#define ANYTHING_CONFIG_DEBUG

namespace at {

void AnythingConfig::reset()
{
    mSearchPaths.clear();
    mExcludePaths.clear();
}

bool AnythingConfig::load(const cppfs::FilePath& path)
{
    reset();

    auto extension = str::to_lower(path.extension());
    auto ok = false;
    if (extension == ".json") {
        ok = loadJson(path);
    }

    if (ok) {
        strip();
    }

    return ok;
}

bool AnythingConfig::loadJson(const cppfs::FilePath& path)
{
    Json::Value root;
    Json::Reader reader;

    std::ifstream ifs(path.fullPath(), std::ios::in | std::ios::binary);
    if (!reader.parse(ifs, root, false)) {
        FATAL("Load config: {} failed, parse json failed: {}", path.fullPath(), reader.getFormattedErrorMessages());
        return false;
    }

    {
        std::vector<std::string> searchPaths;
        if (!JsonUtil::getStringArray(searchPaths, root, "main/searchPaths")) {
            FATAL("Load config: main/searchPaths is not a string array");
            return false;
        }
        mSearchPaths.reserve(searchPaths.size());
        for (auto& p : searchPaths) {
            cppfs::FilePath path(p);
            path = path.fullPath();
            mSearchPaths.push_back(path.resolved());
        }
    }
    {
        std::vector<std::string> excludePaths;
        if (!JsonUtil::getStringArray(excludePaths, root, "main/excludePaths")) {
            FATAL("Load config: main/excludePaths is not a string array");
            return false;
        }
        mSearchPaths.reserve(excludePaths.size());
        for (auto& p : excludePaths) {
            cppfs::FilePath path(p);
            path = path.fullPath();
            mExcludePaths.push_back(path.resolved());
        }
    }
    return true;
}

void AnythingConfig::strip()
{
    std::sort(mSearchPaths.begin(), mSearchPaths.end());
    std::sort(mExcludePaths.begin(), mExcludePaths.end());
    auto removeSubPaths = [](std::vector<std::string>& paths) {
        if (paths.empty()) {
            return;
        }
        std::size_t i = 0, j = 1;
        for (; i < paths.size() && j < paths.size(); ++j) {
            if (str::starts_with(paths[j], paths[i])) {
                continue;
            }
            ++i;
            if (i != j) {
                paths[i] = paths[j];
            }
        }
        paths.resize(i + 1);
    };
    removeSubPaths(mSearchPaths);
    removeSubPaths(mExcludePaths);

    {
        std::size_t i = 0, j = 0;
        auto isSubDirOfSearchPaths = [this](const std::string& path) -> bool {
            for (auto& p : mSearchPaths) {
                if (str::starts_with(path, p)) {
                    return true;
                }
            }
            return false;
        };
        for (; j < mExcludePaths.size(); ++j) {
            auto& excludePath = mExcludePaths[j];
            if (!isSubDirOfSearchPaths(excludePath)) {
                continue;
            }
            
            if (i != j) {
                mExcludePaths[i] = excludePath;
            }
            ++i;
        }
        mExcludePaths.resize(i);
    }

#ifdef ANYTHING_CONFIG_DEBUG
    for (auto& s : mSearchPaths) {
        DEBUG("Search: {}", s);
    }
    for (auto& s : mExcludePaths) {
        DEBUG("exclude: {}", s);
    }
#endif
}

}