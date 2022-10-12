#include <utils.h>
#include <camera.h>
#include "enum.h"
#include <object.h>
#include <shader.h>
#include <bezier.h>

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, double x, double y);

const int WIDTH = 800;
const int HEIGHT = 600;

bool firstMouse = true;
float lastX = WIDTH / 2.0;
float lastY = HEIGHT / 2.0;

GLFWwindow *window;
Camera mycamera;

BETTER_ENUM(RenderCase, int,
            uniform_single_bezier_surface,
            uniform_multi_bezier_surface)
//            adaptive_single_bezier_surface,
//            adaptive_multi_bezier_surface,
//            single_spline_surface
/**
 * BETTER_ENUM is from https://aantron.github.io/better-enums/
 * You can add any render case as you like,
 * this is to allow you to demonstrate multi task in single executable file.
 * */


RenderCase choice = RenderCase::_values()[0];

int main() {
    for (size_t index = 0; index < RenderCase::_size(); ++index) {
        RenderCase render_case = RenderCase::_values()[index];
        std::cout << index << ". " << +render_case << "\n";
    }
    while (true) {
        std::cout << "choose a rendering case from [0:" << RenderCase::_size() - 1 << "]" << "\n";
        std::string input;
        std::cin >> input;
        if (isNumber(input) &&
            std::stoi(input) >= 0 &&
            std::stoi(input) < RenderCase::_size()) {
            choice = RenderCase::_values()[std::stoi(input)];
            break;
        } else {
            std::cout << "Wrong input.\n";
        }
    }
    /**
     * Choose a rendering case in the terminal.
     * */
    WindowGuard windowGuard(window, WIDTH, HEIGHT, "CS171 hw2");
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glEnable(GL_DEPTH_TEST);

    switch (choice) {
        case RenderCase::uniform_single_bezier_surface: {
            std::cout << +RenderCase::uniform_multi_bezier_surface << "do something\n";
            break;
        }
        case RenderCase::uniform_multi_bezier_surface: {
            std::cout << +RenderCase::uniform_multi_bezier_surface << "do something\n";
            break;
        }
    }


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        mycamera.processWalkAround(Forward);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        mycamera.processWalkAround(Backward);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        mycamera.processWalkAround(Leftward);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        mycamera.processWalkAround(Rightward);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        std::cout << "Camera position: (" << mycamera.Position.x << ", "
                  << mycamera.Position.y << ", " << mycamera.Position.z << ")" << std::endl;
    }
}

void mouse_callback(GLFWwindow *window, double x, double y) {
    x = (float) x;
    y = (float) y;
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    mycamera.processLookAround(x - lastX, y - lastY);

    // update record
    lastX = x;
    lastY = y;
}