#pragma once

#include <cppfs/FilePath.h>
#include <string>
#include <vector>

namespace at {
class AnythingConfig {
public:
    void reset();
    bool load(const cppfs::FilePath& path);

    const std::vector<std::string>& searchPaths() const
    {
        return mSearchPaths;
    }

    const std::vector<std::string>& excludePaths() const
    {
        return mExcludePaths;
    }

    void addSearchPath(const std::string& path)
    {
        mSearchPaths.push_back(path);
        strip();
    }

private:
    bool loadJson(const cppfs::FilePath& path);

    void strip();

private:
    std::vector<std::string> mSearchPaths;
    std::vector<std::string> mExcludePaths;
};
}
