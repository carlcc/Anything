#pragma once

#include "anything/AnythingConfig.h"
#include "anything/ATVector.h"
#include "anything/StringUtil.h"
#include <pcre.h>

namespace at {

struct MatchResult {
    std::string& fullPath;
    uint32_t startIndex;
    uint32_t  endIndex;
};

class Anything {
public:
    explicit Anything(const AnythingConfig& config);

    void reload();

    void reload(bool& stop);

    void writeCache();

    const ATVector<std::string>& allFiles() const {
        return mAllFiles;
    }

    template <uint64_t TBLOCK_NUM>
    void matchSubString(ATVector<MatchResult, TBLOCK_NUM>& result, const std::string& subStr)
    {
        result.clear();
        for (uint64_t i = 0; i < mAllFiles.size(); ++i) {
            auto& f = mAllFiles[i];
            auto index = f.find(subStr);
            if (index != std::string::npos) {
                result.pushBack({f, uint32_t (index), uint32_t (index + subStr.length())});
            }
        }
    }

    template <uint64_t TBLOCK_NUM>
    void matchSubString(ATVector<MatchResult, TBLOCK_NUM>& result, const std::string& subStr, bool& stop)
    {
        result.clear();
        for (uint64_t i = 0; i < mAllFiles.size(); ++i) {
            auto& f = mAllFiles[i];
            auto index = f.find(subStr);
            if (index != std::string::npos) {
                result.pushBack({f, uint32_t (index), uint32_t (index + subStr.length())});
            }
            if (stop) {
                break;
            }
        }
    }

    template <uint64_t TBLOCK_NUM>
    void matchRegex(ATVector<MatchResult, TBLOCK_NUM>& result, const std::string& regex)
    {
        // use pcre, std::regex seems to be too slow...
        result.clear();
        const char* error;
        int error_offset;
        auto* reg = pcre_compile(regex.c_str(), 0, &error, &error_offset, nullptr);
        for (uint64_t i = 0; i < mAllFiles.size(); ++i) {
            auto& f = mAllFiles[i];
            int ovec[2];
            int regRet = pcre_exec(reg, nullptr, f.c_str(), f.length(), 0, 0, ovec, 2);
            if (regRet >= 0) {
                result.pushBack({f, ovec[0], ovec[1]});
            }
        }
        pcre_free(reg);
    }

    template <uint64_t TBLOCK_NUM>
    void matchRegex(ATVector<MatchResult, TBLOCK_NUM>& result, const std::string& regex, bool& stop)
    {
        // use pcre, std::regex seems to be too slow...
        result.clear();
        const char* error;
        int error_offset;
        auto* reg = pcre_compile(regex.c_str(), 0, &error, &error_offset, nullptr);
        for (uint64_t i = 0; i < mAllFiles.size(); ++i) {
            auto& f = mAllFiles[i];
            int ovec[2];
            int regRet = pcre_exec(reg, nullptr, f.c_str(), f.length(), 0, 0, ovec, 2);
            if (regRet >= 0) {
                result.pushBack({f, uint32_t(ovec[0]), uint32_t(ovec[1])});
            }
            if (stop) {
                break;
            }
        }
        pcre_free(reg);
    }

private:
    AnythingConfig mConfig;

    ATVector<std::string> mAllFiles;
};
}
