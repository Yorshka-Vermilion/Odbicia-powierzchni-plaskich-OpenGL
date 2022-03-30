#include <iostream>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include "RenderObject.h"
#include "ShaderObj.h"

using namespace std;

// :: OKNO ::

GLFWwindow* okno;
glm::vec2 rozmiarOkna = glm::vec2(1024, 860);
string tytul = "Bartłomiej Marzec - Odbicia";


GLuint vertex_buffer, vertex_shader, fragment_shader, program;
GLint mvp_location, vpos_location, vcol_location;


// :: Obsluga klawiszy ::

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// :: Obsługa błedów ::

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


// :: INICJALIZACJA ::

bool init() {
    if (!glfwInit()) {
        cout << "Nie udało sie zainicjować GLFW!" << endl;
        glfwTerminate();
        return false;
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    okno = glfwCreateWindow(rozmiarOkna.x, rozmiarOkna.y, tytul.c_str(), NULL, NULL);
    if (!okno) {
        cout << "Nie udało się zainicjować okna!" << endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(okno);
    glfwSetKeyCallback(okno, key_callback);
    glfwSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    return true;
}

void exit() {
    glfwDestroyWindow(okno);
    glfwTerminate();
}



// :: RENDER ::

float tmp[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
};

std::vector<RenderObject> renderObjects;

void render() {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Update the uniforms


    //Render models
    for (size_t i = 0; i < renderObjects.size(); i++)
    {
        renderObjects.at(i).render();
    }

    //End Draw
    glfwSwapBuffers(okno);
    glFlush();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main()
{
    if (!init()) {
        cout << "Inicjalizacja nie powiodła sie" << endl;
        return -1;
    };

    
    ShaderObj shader = ShaderObj("shader1.vert", "shader1.frag");
    //shader.Use();

    renderObjects.push_back(RenderObject("test.obj"));

    while (!glfwWindowShouldClose(okno)) {
        render();
    }

    exit();
    exit(EXIT_SUCCESS);
}

