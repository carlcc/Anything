#include <anything/Anything.h>
#include <anything/AnythingConfig.h>
#include <anything/StringUtil.h>
#include <anything/Log.h>
#include <iostream>

int main(int argc, char** argv)
{
    using namespace at;
    AnythingConfig config;
    
    bool configureSet = false;
    for (int i = 0; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--config" || arg == "-c") {
            if (configureSet) {
                FATAL("--config, -c: was set more than once");
                return -1;
            }
            ++i;
            if (i >= argc || str::starts_with(argv[i], "-")) {
                FATAL("--config, -c: missing argument");
                return -1;
            }
            configureSet = true;
            if (!config.load(argv[i])) {
                FATAL("--config, -c: load config file {} failed", argv[i]);
                return -1;
            }
        }
    }

    bool stop = false;
    Anything anything(config);
    anything.reload(stop);

    ATVector<MatchResult, 4096> result;
    while (true) {
        std::string line;
        std::cout << "> ";
        if (!std::getline(std::cin, line)) {
            break;
        }

        line = str::trim_copy(line);
//        anything.matchSubString(result, line);
        anything.matchRegex(result, line);

        for (uint64_t i = 0; i < result.size(); ++i) {
            std::cout << result[i].fullPath << ", " << result[i].startIndex << ", " << result[i].endIndex << std::endl;
        }
    }
    
    return 0;
}