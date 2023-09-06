#include "window.h"

int main()
{
    int success = Window::getInstance()->init();
    if (success == -1)
    {
        return -1;
    }

    while (!Window::getInstance()->shouldClose())
    {
        Window::getInstance()->render();
    }

    return 0;
}