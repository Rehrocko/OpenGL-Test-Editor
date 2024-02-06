#include "GL.h"
#include <string>

namespace GL {
    GLFWwindow* _window;
    GLFWmonitor* _monitor;
    const GLFWvidmode* _mode;
    int _currentWidth = 0;
    int _currentHeight = 0;
    int _windowedWidth = 0;
    int _windowedHeight = 0;
    int _fullscreenWidth = 0;
    int _fullscreenHeight = 0;
    int _mouseScreenX = 0;
    int _mouseScreenY = 0;
    int _windowHasFocus = true;
    bool _forceCloseWindow = false;
    bool showCursor = false;
    int _scrollWheelYOffset = 0;
    WindowMode _windowMode = WINDOWED;// FULLSCREEN;
}

// Callbacks and other GLFW stuff

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void window_focus_callback(GLFWwindow* window, int focused);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei /*length*/, const char* message, const void* /*userParam*/) {
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes
    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;
    switch (source) {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } 
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
}


void GL::ToggleFullscreen() {
    if (_windowMode == WINDOWED)
        SetWindowMode(FULLSCREEN);
    else
        SetWindowMode(WINDOWED);
}

void GL::CreateWindow(WindowMode windowMode) {
    if (windowMode == WINDOWED) {
        _currentWidth = _windowedWidth;
        _currentHeight = _windowedHeight;
        _window = glfwCreateWindow(_windowedWidth, _windowedHeight, "Scene Test", NULL, NULL);
    }
    else if (windowMode == FULLSCREEN) {
        _currentWidth = _fullscreenWidth;
        _currentHeight = _fullscreenHeight;
        _window = glfwCreateWindow(_fullscreenWidth, _fullscreenHeight, "Scene Test", _monitor, NULL);
    }
    _windowMode = windowMode;
}

void GL::SetWindowMode(WindowMode windowMode) {
    if (windowMode == WINDOWED) {
        _currentWidth = _windowedWidth;
        _currentHeight = _windowedHeight;
        glfwSetWindowMonitor(_window, nullptr, 0, 30, _windowedWidth, _windowedHeight, 0);
    }
    else if (windowMode == FULLSCREEN) {
        _currentWidth = _fullscreenWidth;
        _currentHeight = _fullscreenHeight;
        glfwSetWindowMonitor(_window, _monitor, 0, 0, _fullscreenWidth, _fullscreenHeight, _mode->refreshRate);
    }
    _windowMode = windowMode;
}

void GL::Init(int width, int height) {

    glfwInit();

    glfwSetErrorCallback([](int error, const char* description) {
        std::cout << "GLFW Error (" << std::to_string(error) << "): " << description << "\n";
    });

    // We are using 4.6 OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    // Resolution and window size
    _monitor = glfwGetPrimaryMonitor();
    _mode = glfwGetVideoMode(_monitor);
    glfwWindowHint(GLFW_RED_BITS, _mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, _mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, _mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, _mode->refreshRate);
    _fullscreenWidth = _mode->width;
    _fullscreenHeight = _mode->height;
    _windowedWidth = width;
    _windowedHeight = height;
    if (_windowedWidth > _fullscreenWidth || _windowedHeight > _fullscreenHeight) {
        _windowedWidth = (int)(_fullscreenWidth * 0.75f);
        _windowedHeight = (int)(_fullscreenHeight * 0.75f);
    }
    if (_windowMode == FULLSCREEN) {
        _currentWidth = _fullscreenWidth;
        _currentHeight = _fullscreenHeight;
        _window = glfwCreateWindow(_fullscreenWidth, _fullscreenHeight, "Scene test", _monitor, NULL);
    }
    else {
        _currentWidth = _windowedWidth;
        _currentHeight = _windowedHeight;
        _window = glfwCreateWindow(_windowedWidth, _windowedHeight, "Scene Test", NULL, NULL);
        glfwSetWindowPos(_window, 0, 30);
    }
    if (_window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(_window);
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glfwSetWindowFocusCallback(_window, window_focus_callback);
    glfwSetScrollCallback(_window, scroll_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    std::cout << "\nGPU: " << renderer << "\n";
    std::cout << "GL version: " << major << "." << minor << "\n\n";

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        //std::cout << "Debug GL context enabled\n";
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
        glDebugMessageCallback(glDebugOutput, nullptr);
    }
    else {
        std::cout << "Debug GL context not available\n";
    }
    //glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(_window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // Clear screen to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void GL::ProcessInput() {
    processInput(_window);
}

void GL::SwapBuffersPollEvents() {
    glfwSwapBuffers(_window);
    glfwPollEvents();
}

void GL::Terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}

bool GL::WindowIsOpen() {
    return !glfwWindowShouldClose(_window);
}

int GL::GetWindowWidth() {
    return _currentWidth;
}

int GL::GetWindowHeight() {
    return _currentHeight;
}

int GL::GetCursorX() {
    double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);
    return int(xpos);
}

int GL::GetCursorY() {
    double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);
    return int(ypos);
}

void GL::DisableCursor() {
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void GL::HideCursor() {
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    showCursor = false;
}

void GL::ShowCursor() {
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    showCursor = true;
}

void GL::ToggleCursor() {
    if (!showCursor) {
        ShowCursor();
    }
    else {
        HideCursor();
    }
}

GLFWwindow* GL::GetWindowPtr() {
    return _window;
}

int GL::GetCursorScreenX() {
    return _mouseScreenX;
}

int GL::GetCursorScreenY() {
    return _mouseScreenY;
}

bool GL::WindowHasFocus() {
    return _windowHasFocus;
}

bool GL::WindowHasNotBeenForceClosed() {
    return !_forceCloseWindow;
}

void GL::ForceCloseWindow() {
    _forceCloseWindow = true;
}

// Callbacks

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

void window_focus_callback(GLFWwindow* /*window*/, int focused) {
    if (focused) {
        GL::_windowHasFocus = true;
    }
    else {
        GL::_windowHasFocus = false;
    }
}

void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset) {
    GL::_scrollWheelYOffset = (int)yoffset;
}