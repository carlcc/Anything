#include "anything/FileEnumerator.h"
#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>
#include <cppfs/fs.h>

namespace at {

void FileEnumerator::enumerateFiles(const std::string& root, const std::function<void(cppfs::FileHandle& file)>& callback)
{
    cppfs::FileHandle rootHandle = cppfs::fs::open(root);
    if (rootHandle.exists()) {
        callback(rootHandle);
        if (rootHandle.isDirectory() && !rootHandle.isSymbolicLink()) {
            auto dir = (root == "/" ? "" : root) + '/';
            for (cppfs::FileIterator it = rootHandle.begin(); it != rootHandle.end(); ++it) {
                std::string path = dir + *it;
                if (mStop) {
                    return;
                }
                enumerateFilesInternal(path, callback);
            }
        }
    }
}

void FileEnumerator::enumerateFilesInternal(const std::string& root, const std::function<void(cppfs::FileHandle& file)>& callback)
{
    cppfs::FileHandle rootHandle = cppfs::fs::open(root);
    if (mStop) {
        return;
    }
    if (rootHandle.exists()) {
        callback(rootHandle);

        if (rootHandle.isDirectory() && !rootHandle.isSymbolicLink()) {
            auto dir = root + '/';
            for (cppfs::FileIterator it = rootHandle.begin(); it != rootHandle.end(); ++it) {
                std::string path = dir + *it;
                enumerateFilesInternal(path, callback);
            }
        }
    }
}

}
