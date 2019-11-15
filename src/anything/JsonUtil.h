#include "anything/StringUtil.h"
#include <json/value.h>

namespace at {
class JsonUtil {
public:
    static bool getString(std::string& outValue, const Json::Value& root, const std::string& path)
    {
        bool ok { false };
        const Json::Value& node = getNode(root, path, ok);
        if (!node.isString()) {
            return false;
        }
        outValue = node.asString();
        return true;
    }

    static bool getStringArray(std::vector<std::string>& outValue, const Json::Value& root, const std::string& path) {
        bool ok { false };
        const Json::Value& node = getNode(root, path, ok);
        if (!node.isArray()) {
            return false;
        }
        for (auto& n : node) {
            if (!n.isString()) {
                return false;
            }
        }
        outValue.reserve(node.size());
        for (auto& n : node) {
            outValue.push_back(n.asString());
        }
        return true;
    }

private:
    static const Json::Value& getNode(const Json::Value& root, const std::string& path, bool& ok)
    {
        auto paths = str::split(path, '/');
        auto* pNode = &root;
        for (auto& p : paths) {
            auto& node = *pNode;
            if (!node.isObject()) {
                ok = false;
                return root;
            }
            if (node[p].isNull()) {
                ok = false;
                return root;
            }
            pNode = &node[p];
        }
        ok = true;
        return *pNode;
    }
};
}