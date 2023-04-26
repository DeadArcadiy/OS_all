#include <windows.h>

struct ThreadInfo {
    HANDLE h;
    int num;
    bool flag = false;
    int marked_num = 0;
};