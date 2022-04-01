#include <iostream>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include "RenderObject.h"
#include "ShaderObj.h"
#include "Camera.h"

using namespace std;

// :: OKNO ::

GLFWwindow* okno;
glm::vec2 rozmiarOkna = glm::vec2(1024, 860);
string tytul = "Bartłomiej Marzec - Odbicia";


GLuint vertex_buffer, vertex_shader, fragment_shader, program;
GLint mvp_location, vpos_location, vcol_location;


float rotation = 0.5f;

float currTime, lastTime, dt;

Camera camera;
bool cameraRotActive;

// :: Obsluga klawiszy ::

void rotateCamera() {
    glm::dvec2 mousePos;
    glfwGetCursorPos(okno, &mousePos.x, &mousePos.y);
    glm::vec2 mouseRot = glm::vec2(mousePos.y - (rozmiarOkna.y / 2), mousePos.x - (rozmiarOkna.x / 2));


    camera.Rotate(mouseRot, dt);
    glfwSetCursorPos(okno, (rozmiarOkna.x / 2), (rozmiarOkna.y / 2));
}

void updateKeyboard() {
    if (glfwGetKey(okno,GLFW_KEY_ESCAPE)== GLFW_PRESS)
        glfwSetWindowShouldClose(okno, GLFW_TRUE);

    if (glfwGetKey(okno, GLFW_KEY_W) == GLFW_PRESS)
        camera.Move(FORWARD, dt);
    if (glfwGetKey(okno, GLFW_KEY_S) == GLFW_PRESS)
        camera.Move(BACK, dt);
    if (glfwGetKey(okno, GLFW_KEY_A) == GLFW_PRESS)
        camera.Move(LEFT, dt);
    if (glfwGetKey(okno, GLFW_KEY_D) == GLFW_PRESS)
        camera.Move(RIGHT, dt);
    if (glfwGetKey(okno, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.Move(UP, dt);
    if (glfwGetKey(okno, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.Move(DOWN, dt);
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (!cameraRotActive) {
            glfwSetCursorPos(window, (rozmiarOkna.x / 2), (rozmiarOkna.y / 2));
            cameraRotActive = true;
        }
        
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cameraRotActive = false;
    }
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
    glfwSetMouseButtonCallback(okno, mouse_callback);
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

std::vector<RenderObject*> renderObjects;

void render(ShaderObj *shader) {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    //Update the uniforms


    //Render models
    for (size_t i = 0; i < renderObjects.size(); i++)
    {
        renderObjects.at(i)->render(shader);
    }

    //End Draw
    glfwSwapBuffers(okno);
    glFlush();

    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void update(ShaderObj* shader) {
    glfwPollEvents();
    updateKeyboard();

    currTime = static_cast<float>(glfwGetTime());
    dt = currTime - lastTime;
    lastTime = currTime;
    
    camera.UpdateMatrix(shader);
    if (cameraRotActive) {
        rotateCamera();
    }
}


int main()
{
    if (!init()) {
        cout << "Inicjalizacja nie powiodła sie" << endl;
        return -1;
    };

    
    ShaderObj shader = ShaderObj("shader1.vert", "shader1.frag");
    //shader.Use();



    camera = Camera(rozmiarOkna, "cameraMatrix");

    renderObjects.push_back(new RenderObject("Obiekty/test.obj", glm::vec3(1.f, 1.f, 1.f)));
    renderObjects.at(0)->SetTexture("Tekstury/Skala.jpg");

    renderObjects.push_back(new RenderObject("Obiekty/Floor_square.obj", glm::vec3(0.f, 0.f, 0.f)));
    renderObjects.at(1)->SetTexture("Tekstury/Patrick.jpg");


    while (!glfwWindowShouldClose(okno)) {
        
        update(&shader);
        
        
        render(&shader);
        
    }

    exit();
    exit(EXIT_SUCCESS);
}

