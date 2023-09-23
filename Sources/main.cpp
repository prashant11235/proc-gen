#include "window.h"

int main()
{
    int success = proc_gen::Window::getInstance()->init();
    if (success == -1)
    {
        return -1;
    }

    while (!proc_gen::Window::getInstance()->shouldClose())
    {
        proc_gen::Window::getInstance()->render();
    }

    return 0;
}