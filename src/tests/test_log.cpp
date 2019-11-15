#include <anything/Log.h>

int main()
{
    INFO("{2}{1}{0}", "hello", 1, "world");
    return 0;
}