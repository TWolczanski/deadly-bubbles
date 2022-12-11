#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <AGL3Window.hpp>
#include <aquarium.hpp>
#include <player.hpp>
#include <bubbles.hpp>
#include <light.hpp>
#include <constants.hpp>

class MyWin : public AGLWindow
{
    float fov;
    int viewport;
public:
    MyWin(){};
    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr = 0)
        : AGLWindow(_wd, _ht, name, vers, fullscr) {};
    virtual void Resize(int _wd, int _ht);
    virtual void KeyCB(int key, int scancode, int action, int mods);
    virtual void ScrollCB(double xp, double yp);
    void MainLoop();
};

void MyWin::Resize(int _wd, int _ht) {
   wd    = _wd;
   ht    = _ht;
   aspect= float(ht)/float(wd);
}

void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        viewport = viewport % 2 + 1;
    }
}

void MyWin::ScrollCB(double xp, double yp) {
    fov -= yp;
    if (fov < 1.0) {
        fov = 1.0;
    }
    if (fov > 90.0) {
        fov = 90.0;
    }
}

void MyWin::MainLoop()
{
    glEnable(GL_DEPTH_TEST);

    const float playerRadius = 1.0 / 5.0 / 6.0;
    fov = 45.0;
    viewport = 1;

    Aquarium aquarium;
    Bubbles bubblesLevel1(20, 0.3, 0.02);
    Bubbles bubblesLevel2(35, 0.5, 0.03);
    Player player(playerRadius);
    Observer outsideCamera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    outsideCamera.rotateY(180.0 + 45.0);
    outsideCamera.rotateX(-35.0);
    outsideCamera.moveZ(-1.0);

    PointLight pointLight;
    pointLight.position = glm::vec3(AQUARIUM_SIZE_X - 0.3f, 0.05f, AQUARIUM_SIZE_Z / 2);
    pointLight.quadratic = 0.20;
    pointLight.linear = 0.22;
    pointLight.constant = 1.0;
    pointLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    pointLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    DirectionalLight directionalLight;
    directionalLight.direction = glm::vec3(-0.5f, -1.0f, -0.5f);
    directionalLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    directionalLight.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    directionalLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    double now = glfwGetTime();
    double last = glfwGetTime();

    do
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 view, projection;
        now = glfwGetTime();

        AGLErrors("main-loopbegin");
        // =====================================================        Drawing
        ViewportOne(0, 0, wd, ht);
        if (viewport == 1) {
            view = player.getViewMatrix();
            projection = glm::perspective(glm::radians(fov), (float)wd / (float)ht, playerRadius, 100.0f);
            aquarium.draw(view, projection, pointLight, directionalLight, player.getPosition());
            if (player.level == 1) {
                bubblesLevel1.draw(view, projection, pointLight, directionalLight, player.getPosition(), now, now - last);
            }
            else if (player.level == 2) {
                bubblesLevel2.draw(view, projection, pointLight, directionalLight, player.getPosition(), now, now - last);
            }
        }
        else {
            view = outsideCamera.getViewMatrix();
            projection = glm::ortho(-1.0f * wd / 800, 1.0f * wd / 800, -0.1f * ht / 600, 1.75f * ht / 600, 0.1f, 100.0f);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            aquarium.draw(view, projection, pointLight, directionalLight, outsideCamera.getPosition());
            glDisable(GL_CULL_FACE);
            player.draw(view, projection, pointLight, directionalLight, outsideCamera.getPosition());
            if (player.level == 1) {
                bubblesLevel1.draw(view, projection, pointLight, directionalLight, outsideCamera.getPosition(), now, now - last);
            }
            else if (player.level == 2) {
                bubblesLevel2.draw(view, projection, pointLight, directionalLight, outsideCamera.getPosition(), now, now - last);
            }
        }
        
        AGLErrors("main-afterdraw");

        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();
        // glfwWaitEvents();

        if (player.level == 1) {
            player.handleInput(win(), now - last, bubblesLevel1);
        }
        else if (player.level == 2) {
            player.handleInput(win(), now - last, bubblesLevel2);
        }

        if (player.level == 0) {
            std::cout << "You've touched a deadly bubble! Game time: " << now << std::endl;
            break;
        }
        else if (player.level == 3) {
            std::cout << "Congratulations! It took you " << now << "s to twice get through to the other side of the aquarium." << std::endl;
            break;
        }

        last = now;

    } while (glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(win()) == 0);
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    MyWin win;
    win.Init(800, 600, "Deadly bubbles", 0, 33);
    win.MainLoop();
    return 0;
}
