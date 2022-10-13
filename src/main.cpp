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


RenderCase choice = RenderCase::_values()[0];
#define GLDebug(expre) GLClearError();(expre);GLCheckError(#expre,__FILE__,__LINE__);
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
static void GLCheckError(const char* functionName, const char* file, int line) {
    if (GLenum error = glGetError())
    {
        std::cout << "[OpenGL ERROR] " << error << std::endl; //返回错误代码，请去glew.h找十六进制编号
        std::cout << "Function: " << functionName << std::endl;
        std::cout << "File: " << file << std::endl;
        std::cout << "Line: " << line << std::endl;
        std::cout << "Go glew.h to find the hex of the error code!" << line << std::endl;
        __debugbreak();//MSBC编译器特有函数
    }
}
int main() {
#pragma region Choose RenderCase


    goto OpenGLInit;
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
#pragma endregion

#pragma region OpenGLInit
    OpenGLInit:
    WindowGuard windowGuard(window, WIDTH, HEIGHT, "CS171 hw2");
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glEnable(GL_DEPTH_TEST);
#pragma endregion
	
    Shader simpleshader("shaders/vertex/onlyMVP.glsl", "shaders/fragment/colorIsNormal.glsl");

    auto vertexs= read("assets\\tea.bzs");
    Object testobj;
    testobj.draw_mode.primitive_mode = GL_TRIANGLES;
    for each (BezierSurface bs in vertexs)
    {
        for each (Vertex point in bs.generateObject())
        {
            testobj.vertices.push_back(point);

        }
    }
    testobj.init();
	
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        simpleshader.use();
        simpleshader.setMat4("MVP", mycamera.getVP());
        GLDebug(testobj.drawArrays());

		

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

    mycamera.processLookAround(x - lastX, -y + lastY);

    // update record
    lastX = x;
    lastY = y;
}