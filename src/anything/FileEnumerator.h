#pragma once

#include <cppfs/FileHandle.h>
#include <cppfs/FilePath.h>
#include <functional>

namespace at {

class FileEnumerator {
public:
    FileEnumerator(bool& stop): mStop(stop)
    {
    }
    
    void enumerateFiles(const std::string& root, const std::function<void(cppfs::FileHandle& file)>& callback);

private:
    void enumerateFilesInternal(const std::string& root, const std::function<void(cppfs::FileHandle& file)>& callback);

    bool& mStop;
};

}