#include "GL.h"
#include <string>

namespace GL {
    inline GLFWwindow* _window;
    inline GLFWmonitor* _monitor;
    inline const GLFWvidmode* _mode;
    inline int _currentWidth = 0;
    inline int _currentHeight = 0;
    inline int _windowedWidth = 0;
    inline int _windowedHeight = 0;
    inline int _fullscreenWidth = 0;
    inline int _fullscreenHeight = 0;
    inline int _mouseScreenX = 0;
    inline int _mouseScreenY = 0;
    inline int _windowHasFocus = true;
    inline bool _forceCloseWindow = false;
    inline int _scrollWheelYOffset = 0;
    inline WindowMode _windowMode = WINDOWED;// FULLSCREEN;
}