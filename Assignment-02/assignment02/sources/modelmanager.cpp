//
// Created by inessa on 13/04/16.
//

#include "modelmanager.hpp"

void ModelManager ()
{
    // Initialize the GUI
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(g_nWidth, g_nHeight);

    // Set GLFW event callbacks. I removed glfwSetWindowSizeCallback for conciseness
    glfwSetMouseButtonCallback(g_pWindow, (GLFWmousebuttonfun)TwEventMouseButtonGLFW); // - Directly redirect GLFW mouse button events to AntTweakBar
    glfwSetCursorPosCallback(g_pWindow, (GLFWcursorposfun)TwEventMousePosGLFW);          // - Directly redirect GLFW mouse position events to AntTweakBar
    glfwSetScrollCallback(g_pWindow, (GLFWscrollfun)TwEventMouseWheelGLFW);    // - Directly redirect GLFW mouse wheel events to AntTweakBar
    glfwSetKeyCallback(g_pWindow, (GLFWkeyfun)TwEventKeyGLFW);                         // - Directly redirect GLFW key events to AntTweakBar
    glfwSetCharCallback(g_pWindow, (GLFWcharfun)TwEventCharGLFW);                      // - Directly redirect GLFW char events to AntTweakBar
    glfwSetWindowSizeCallback(g_pWindow, WindowSizeCallBack);

    //create the toolbar
    g_pToolBar = TwNewBar("CG UFPel ToolBar");
    // Add 'speed' to 'bar': it is a modifable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
    double speed = 0.0;
    TwAddVarRW(g_pToolBar, "speed", TW_TYPE_DOUBLE, &speed, " label='Rot speed' min=0 max=2 step=0.01 keyIncr=s keyDecr=S help='Rotation speed (turns/second)' ");
    // Add 'bgColor' to 'bar': it is a modifable variable of type TW_TYPE_COLOR3F (3 floats color)
    vec3 oColor(0.0f);
    TwAddVarRW(g_pToolBar, "bgColor", TW_TYPE_COLOR3F, &oColor[0], " label='Background color' ");

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(g_pWindow, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPos(g_pWindow, g_nWidth / 2, g_nHeight / 2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}