#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include "shader.h"
#include "camera.h"
#include "skybox.h"
#include "terrain_manager.h"

#include <iostream>

class Window
{
public:
    static Window* getInstance();
    int init();
    int shouldClose();
    void render();

private:
    static Window* window_instance;
    Window();
    GLFWwindow* window;

    // Window constants 
    const unsigned int WINDOW_WIDTH = 1440;
    const unsigned int WINDOW_HEIGHT = 1080;
    const char* WINDOW_TITLE = "Terrain Generator";


    // Pointer to different objects
    Skybox* skybox_ptr;
    TerrainManager* terrain_manager_ptr;

    // Camera 
    Camera camera;
    float last_x;
    float last_y;
    bool first_mouse;
    float camera_near;
    float camera_far;

    // Timing 
    float delta_time;
    float last_frame;

    double last_time;
    int nbFrames;

    // Process Input 
    void processInput(GLFWwindow* window);

    //Callback functions 
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

};