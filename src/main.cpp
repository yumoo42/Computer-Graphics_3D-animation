#include "../lib/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../lib/glm/glm.hpp"
#include "../lib/glm/gtc/matrix_transform.hpp"
#include "../lib/glm/gtc/type_ptr.hpp"

#include "../include/shader.h"
#include "../include/camera.h"

#include "../include/model.h"
#include "../include/circle.h"
#include "../include/SceneNode.h"

#include <iostream>


// Globals
bool animation = false;
float walkCircleRadius = 10.0f;
glm::mat4 legsBaseTransforms[4][2];

// Timing
float lastFrame = 0.0f;
float timeSinceLastToggle = 1.0f;

// Some settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 30.0f));
float cameraOrbitRadius = 30.0f;
float rotateAngle = 1.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
// void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scrollInput(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void updateWalkAnimation(SceneNode* legNodes[][2], float deltaTime);
void updateModelPosition(SceneNode* modelNode, float deltaTime);
glm::mat4 calculateOrientation(float angle);

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void keyboardInput(GLFWwindow * window, float deltaTime)
{
    // Exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Upwards Rotation
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Orbit(UP, cameraOrbitRadius, rotateAngle);

    // Downwards Rotation
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Orbit(DOWN, cameraOrbitRadius, rotateAngle);

    // Rightwards Rotation
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Orbit(RIGHT, cameraOrbitRadius, rotateAngle);

    // Leftwards Rotation
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Orbit(LEFT, cameraOrbitRadius, rotateAngle);

    // Pause / Start
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        if (timeSinceLastToggle > 0.2)
        {
            animation = !animation;
            timeSinceLastToggle = 0.0f;
        }
    }
    timeSinceLastToggle += deltaTime;
        
}

// Handles mouse scroll wheel. Supposed to be used as the glfw scroll callback.
void scrollInput(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.Zoom(yoffset);
}

void lerpRotation(SceneNode* node, glm::mat4 baseTransform, float startAngle, float endAngle, float t) {
    float angle = startAngle + t * (endAngle - startAngle);
    glm::mat4 rotationTransform = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    node->setTransformation(baseTransform * rotationTransform);
}


void updateWalkAnimation(SceneNode* legNodes[][2], float deltaTime) {
    static float walkCycleTime = 0.0f;

    float phaseDuration = 1.0f;
    walkCycleTime = fmod(walkCycleTime + deltaTime, phaseDuration);
    float t = fmod(walkCycleTime, phaseDuration) / phaseDuration;
    float upperRotations[12] = {15.0f, 30.0f, 45.0f, 30.0f, 15.0f, 0.0f, -15.0f, -30.0f, -45.0f, -30.0f, -15.0f, 0.0f};
    float lowerRotations[12] = {0.0f, 0.0f, 10.0f, 20.0f, 10.0f, 0.0f, 0.0f, -10.0f, -20.0f, -10.0f, 0.0f, 0.0f};
    for (int i = 0; i < 4; ++i) {
        int phase = static_cast<int>(t * 12) % 12;
        int nextPhase = (phase + 1) % 12;

        if (i == 0 || i == 2) {
            phase = (phase + 6) % 12; 
            nextPhase = (phase + 1) % 12;
        }

        float upperStartAngle = upperRotations[phase];
        float upperEndAngle = upperRotations[nextPhase];
        float lowerStartAngle = upperRotations[phase];
        float lowerEndAngle = upperRotations[nextPhase];

        lerpRotation(legNodes[i][0], legsBaseTransforms[i][0], upperStartAngle, upperEndAngle, t);
        lerpRotation(legNodes[i][1], legsBaseTransforms[i][1], lowerStartAngle, lowerEndAngle, t);
    }
}

glm::mat4 calculateOrientation(float angle) {
    glm::vec3 forwardDir(cos(angle), 0.0f, sin(angle));

    glm::mat4 orientation = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), forwardDir, glm::vec3(0.0f, 1.0f, 0.0f));
    return glm::inverse(orientation);
}

void updateModelPosition(SceneNode* modelNode, float deltaTime) {
    static float angle = 0.0f;
    
    float radius = 10.0f;
    float speed = 0.5f;

    if (animation) {
        angle += deltaTime * speed;
    }

    float x = radius * cos(-angle);
    float z = radius * sin(-angle);

    glm::mat4 modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, z));
    glm::mat4 rotation = calculateOrientation(-angle);
    modelNode->setTransformation(modelTransform * rotation);
}

int main()
{
    // Initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "hw3", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSetScrollCallback(window, scrollInput);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);    

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader("./src/skinning.vs", "./src/skinning.fs");

    // Load the models
    Model cubeModel("./models/cube.obj");
    glm::mat4 model(1.0);

    SceneNode bodyNode(&cubeModel);
    bodyNode.setTransformation(glm::scale(model, {2.0f, 2.0f, 2.0f}));
    
    SceneNode headNode(&cubeModel);
    headNode.setTransformation(
        glm::translate(model, {-1.7f, 0.0f, 0.0f}) *
        glm::scale(model, {0.7f, 0.7f, 0.7f})
    );
    bodyNode.addChild(&headNode);

    SceneNode* legNodes[4][2];
    for (int i = 0; i < 4; ++i) {
        float legXOffset = (i < 2) ? -1.0f : 1.0f;
        for (int j = 0; j < 2; ++j) {
            glm::mat4 localModel(1.0f);
            legNodes[i][j] = new SceneNode(&cubeModel);
            if (j == 0){
                if (i == 0){
                    legNodes[i][j]->setTransformation(
                        glm::translate(localModel, {-0.7f, -1.3f, 0.7f}) * 
                        glm::scale(localModel, {0.3f, 0.3f, 0.3f})
                    );
                } else if (i == 1){
                    legNodes[i][j]->setTransformation(
                        glm::translate(localModel, {-0.7f, -1.3f, -0.7f}) * 
                        glm::scale(localModel, {0.3f, 0.3f, 0.3f})
                    );
                } else if (i == 2){
                    legNodes[i][j]->setTransformation(
                        glm::translate(localModel, {0.7f, -1.3f, -0.7f}) * 
                        glm::scale(localModel, {0.3f, 0.3f, 0.3f})
                    );
                } else {
                    legNodes[i][j]->setTransformation(
                        glm::translate(localModel, {0.7f, -1.3f, 0.7f}) * 
                        glm::scale(localModel, {0.3f, 0.3f, 0.3f})
                    );
                }
                bodyNode.addChild(legNodes[i][j]); 
            } else {
                if (i == 0){
                    legNodes[i][j]->setTransformation(
                        glm::translate(localModel, {0.0f, -1.6f, 0.0f}) * 
                        glm::rotate(localModel, glm::radians(15.0f), {0, 0, 1})
                    );
                } else if (i == 1){
                    legNodes[i][j]->setTransformation(
                        glm::translate(localModel, {0.0f, -1.6f, 0.0f}) * 
                        glm::rotate(localModel, glm::radians(15.0f), {0, 0, 1})
                    );
                } else if (i == 2){
                    legNodes[i][j]->setTransformation(
                        glm::translate(localModel, {0.0f, -1.6f, 0.0f}) * 
                        glm::rotate(localModel, glm::radians(15.0f), {0, 0, 1})
                    );
                } else {
                    legNodes[i][j]->setTransformation(
                        glm::translate(localModel, {0.0f, -1.6f, 0.0f}) * 
                        glm::rotate(localModel, glm::radians(15.0f), {0, 0, 1})
                    );
                }
                legNodes[i][0]->addChild(legNodes[i][j]);
            }
        }
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 2; ++j) {
            legsBaseTransforms[i][j] = legNodes[i][j]->getTransformation();
        }
    }

    SceneNode tailNode(&cubeModel);
    tailNode.setTransformation(
        glm::translate(model, {1.3f, 0.3f, 0.0f}) * 
        glm::scale(model, {0.3f, 0.3f, 0.3f}) *
        glm::rotate(model, glm::radians(45.0f), {0, 0, 1})
    );
    bodyNode.addChild(&tailNode);

    float initialAngle = 0.0f;
    float initialX = walkCircleRadius * cos(-initialAngle);
    float initialZ = walkCircleRadius * sin(-initialAngle);
    glm::mat4 initialModelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(initialX, 0.0f, initialZ));
    glm::mat4 initialRotation = calculateOrientation(-initialAngle);
    bodyNode.setTransformation(initialModelTransform * initialRotation);

    shader.use();
    // Render Loop
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        keyboardInput(window, deltaTime);


        if (animation) {
            updateWalkAnimation(legNodes, deltaTime);
            updateModelPosition(&bodyNode, deltaTime);
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // view / projection
        shader.use();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);
        bodyNode.draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}