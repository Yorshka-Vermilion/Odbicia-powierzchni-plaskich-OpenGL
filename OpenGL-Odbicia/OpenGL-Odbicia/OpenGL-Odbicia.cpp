#include <iostream>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include "RenderObject.h"
#include "ShaderObj.h"
#include "Camera.h"
#include "Cubemap.h"

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

Cubemap mainCubemap;

std::vector<RenderObject*> renderObjects;
std::vector<RenderObject*> reflectiveRenderObjects;

// :: Obsluga sterowania ::

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




void render(ShaderObj *shader, ShaderObj *cubemapShader, ShaderObj *reflectionShader) {
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // Render odbić
    for (size_t j = 0; j < reflectiveRenderObjects.size(); j++) {
        glm::vec3 distance = (camera.position - reflectiveRenderObjects.at(j)->position) * 2.f;
        camera.flip(distance);
        camera.UpdateMatrix(shader);
        mainCubemap.updateMatrix(camera);
        reflectiveRenderObjects.at(j)->bindReflections();
        for (size_t i = 0; i < renderObjects.size(); i++)
        {
            if (!renderObjects.at(i)->reflective)
                renderObjects.at(i)->render(shader);
        }

        mainCubemap.render(cubemapShader);
        reflectiveRenderObjects.at(j)->unbindReflections(rozmiarOkna);
        camera.flip(-distance);
        camera.UpdateMatrix(shader);
        mainCubemap.updateMatrix(camera);
    }
    // Render głowny
    for (size_t i = 0; i < renderObjects.size(); i++)
    {
        if (!renderObjects.at(i)->reflective)
            renderObjects.at(i)->render(shader);
        else renderObjects.at(i)->render(reflectionShader,&camera);
    }
    mainCubemap.render(cubemapShader);
    
    //std::cout << camera.orientation.x << " : " << camera.orientation.y << " : " << camera.orientation.z << endl;
        
    // Koniec renderu
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
    float fps = 1 / dt;
    std::cout << fps << std::endl;

    renderObjects.at(0)->mesh->rotate(glm::vec3(0.f, 90.f * dt, 0.f));
    
    if (cameraRotActive) {
        rotateCamera();
    }
    camera.UpdateMatrix(shader);

    mainCubemap.updateMatrix(camera);

}

void addRenderObject(RenderObject* object, string filename, bool reflective = false) {
    renderObjects.push_back(object);
    object->SetTexture(filename);
    if (reflective) {
        reflectiveRenderObjects.push_back(object);
        object->initReflections();
    }
};

int main()
{
    if (!init()) {
        cout << "Inicjalizacja nie powiodła sie" << endl;
        return -1;
    };

    
    ShaderObj shader = ShaderObj("shader1.vert", "shader1.frag");
    ShaderObj cubemapShader = ShaderObj("cubemaps.vert", "cubemaps.frag");
    ShaderObj reflectionShader = ShaderObj("odbicie.vert", "odbicie.frag");

    mainCubemap = Cubemap(&cubemapShader);

    camera = Camera(rozmiarOkna, "cameraMatrix");

    //::Obiekty::

    addRenderObject(new RenderObject("Obiekty/test.obj", glm::vec3(0.f, 1.05f, 0.f), glm::vec3(0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Skala.jpg");

    addRenderObject(new RenderObject("Obiekty/Table.obj", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.01f, 0.01f, 0.01f)), "Tekstury/table.tga");

    addRenderObject(new RenderObject("Obiekty/Doniczka.obj", glm::vec3(1.f, 2.25f, 0.f), glm::vec3(0.f), glm::vec3(1.f, 1.f, 1.f)), "Tekstury/Doniczka.jpg");


    addRenderObject(new RenderObject("Obiekty/Krzeslo.obj", glm::vec3(1.5f, 1.25f, 0.45f), glm::vec3(0.f,90.f,0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Krzeslo.jpg");
    addRenderObject(new RenderObject("Obiekty/Krzeslo.obj", glm::vec3(1.5f, 1.25f, -0.45f), glm::vec3(0.f, 90.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Krzeslo.jpg");
    addRenderObject(new RenderObject("Obiekty/Krzeslo.obj", glm::vec3(1.5f, 1.25f, 0.45f), glm::vec3(0.f, -90.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Krzeslo.jpg");
    addRenderObject(new RenderObject("Obiekty/Krzeslo.obj", glm::vec3(1.5f, 1.25f, -0.45f), glm::vec3(0.f, -90.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Krzeslo.jpg");

    addRenderObject(new RenderObject("Obiekty/Sciana.obj", glm::vec3(3.43f, 1.32f, -1.695f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Sciana.jpg");
    addRenderObject(new RenderObject("Obiekty/Sciana.obj", glm::vec3(3.43f, 1.32f, 1.695f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Sciana.jpg");
    addRenderObject(new RenderObject("Obiekty/Sciana.obj", glm::vec3(3.43f, 1.32f, -1.695f), glm::vec3(0.f, 90.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Sciana.jpg");
    addRenderObject(new RenderObject("Obiekty/Sciana.obj", glm::vec3(3.43f, 1.32f, 1.695f), glm::vec3(0.f, 90.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Sciana.jpg");

    addRenderObject(new RenderObject("Obiekty/kanapa.obj", glm::vec3(5.5f, 1.75f, 0.0f), glm::vec3(0.f, 45.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Kanapa.jpg");

    addRenderObject(new RenderObject("Obiekty/Polka.obj", glm::vec3(3.43f, 1.2f, 1.695f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Polka.jpg");
    addRenderObject(new RenderObject("Obiekty/Polka.obj", glm::vec3(3.43f, 1.0f, -1.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Polka.jpg");
    addRenderObject(new RenderObject("Obiekty/Polka.obj", glm::vec3(3.43f, 0.9f, 1.2f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Polka.jpg");
    addRenderObject(new RenderObject("Obiekty/Polka.obj", glm::vec3(3.43f, 1.32f, 1.695f), glm::vec3(0.f, 90.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Polka.jpg");
    addRenderObject(new RenderObject("Obiekty/Polka.obj", glm::vec3(3.43f, 1.0f, -1.3f), glm::vec3(0.f, 90.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Polka.jpg");
    addRenderObject(new RenderObject("Obiekty/Polka.obj", glm::vec3(3.43f, 0.8f, 1.4f), glm::vec3(0.f, 90.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f)), "Tekstury/Polka.jpg");



    addRenderObject(new RenderObject("Obiekty/Plane.obj", glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f,0.f,0.f), glm::vec3(1.5f, 1.5f, 1.5f)), "Tekstury/Gradient.jpg", true);

    //::KONIEC OBIEKTOW::

    while (!glfwWindowShouldClose(okno)) {
        update(&shader);
        render(&shader,&cubemapShader,&reflectionShader);
    }

    exit();
    exit(EXIT_SUCCESS);
}

