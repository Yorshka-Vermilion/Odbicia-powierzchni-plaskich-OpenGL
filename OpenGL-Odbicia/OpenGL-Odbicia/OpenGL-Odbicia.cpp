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

    glewInit();
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


int main()
{
    if (!init()) {
        cout << "Inicjalizacja nie powiodła sie" << endl;
        return -1;
    };

    
    RenderObject testObj = RenderObject("test.obj");
    ShaderObj shader = ShaderObj("shader1.vert", "shader1.frag");
    //shader.Use();

    while (!glfwWindowShouldClose(okno)) {
        float ratio;
        int width, height;

        glfwGetFramebufferSize(okno, &width, &height);
        ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        // :: test ::

        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 View = glm::lookAt(
            glm::vec3(3, 3, 3), // Camera is at (4,3,3), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 mvp = Projection * View * Model; // Our ModelViewProjection : multiplication of our 3 matrices
        GLuint MatrixID = glGetUniformLocation(shader.ID, "MVP");
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

        // :: end ::
        
        testObj.render();


        glfwSwapBuffers(okno);
        glfwPollEvents();
    }

    exit();
    exit(EXIT_SUCCESS);
}