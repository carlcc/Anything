#include <cppfs/FileHandle.h>
#include <cppfs/FileWatcher.h>
#include <cppfs/fs.h>

#include <iostream>

int main()
{
    cppfs::FileHandle handle = cppfs::fs::open("/home/carl/");

    auto p = handle.permissions();
    std::cout << p << std::endl;
    std::cout << " is dir " << handle.isDirectory() << std::endl;
    std::cout << " is exists " << handle.exists() << std::endl;
    for (auto& f : handle.listFiles()) {
        std::cout << "=== " << f << std::endl;
    }

    // cppfs::FileWatcher watcher(handle.fs());
    // watcher.add(handle, cppfs::FileEvent::FileCreated | cppfs::FileEvent::FileRemoved | cppfs::FileEvent::FileModified | cppfs::FileEvent::FileAttrChanged, cppfs::RecursiveMode::Recursive);
    // watcher.addHandler([](cppfs::FileHandle &fh, cppfs::FileEvent event) {
    //     if (event & cppfs::FileEvent::FileCreated) {
    //         std::cout << "Filecreated: " << fh.path() << std::endl;
    //     }
    //     if (event & cppfs::FileEvent::FileRemoved) {
    //         std::cout << "FileRemoved: " << fh.path() << std::endl;
    //     }
    //     if (event & cppfs::FileEvent::FileModified) {
    //         std::cout << "FileModified: " << fh.path() << std::endl;
    //     }
    //     if (event & cppfs::FileEvent::FileAttrChanged) {
    //         std::cout << "FileAttrChanged: " << fh.path() << std::endl;
    //     }
    // });
    // while (true)
    // {
    //     std::cout << "===" << std::endl;
    //     watcher.watch(500);
    // }
    
    return 0;
}