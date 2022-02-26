#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#define STB_IMAGE_IMPLEMENTATION


#include <iostream>
#include <cstdio>
#include <algorithm>


#include "core/Shader.h"
#include "core/Texture.h"


#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 990


float mix_value = 0.2f;
float texScale = 1.0f;
float currentFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main()
{
    //-----intializations

    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game Engine", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to created glfwWindow\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    bool err = glewInit() != GLEW_OK;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader\n");
        glfwTerminate();
        return 1;
    }

    
    //------buffers and data
    
    /*float geometry[] = {
        -0.5f, -0.5f, -0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };*/

    float vertices[] = {
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 
        0.5f,  -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    GLuint VBO, VAO, EBO;
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    //-----settings
    glEnable(GL_DEPTH_TEST);


    //-----shaders
    engine::Shader shader("vertex.glsl", "fragment.glsl");


    //------textures
    engine::Texture containerTexture("resources/textures/container.jpg", GL_RGB);
    engine::Texture brikwallTexture("resources/textures/brikwall.jpg", GL_RGB);
    engine::Texture faceTexture("resources/textures/awesomeface.png", GL_RGBA);


    shader.Use();
    shader.setInt("texture0", 0);
    shader.setInt("texture1", 1);
    shader.setInt("texture2", 2);

    //render loop
    while (!glfwWindowShouldClose(window))
    {
        float lastFrame = currentFrame;
        float time = glfwGetTime();
        currentFrame = time;
        std::cout << "Elapsed time: " << currentFrame - lastFrame << std::endl;

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput(window);

        shader.Use();
        glActiveTexture(GL_TEXTURE0);
        containerTexture.Bind();
        glActiveTexture(GL_TEXTURE1);
        brikwallTexture.Bind();
        glActiveTexture(GL_TEXTURE2);
        faceTexture.Bind();

        mix_value = std::clamp(mix_value, 0.0f, 1.0f);
        shader.setFloat("mix_value", mix_value);
        shader.setFloat("texScale", texScale);
        shader.setFloat("texShift", time);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glfwTerminate();
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        mix_value += 0.01f;
    
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        mix_value -= 0.01f;
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        texScale += 0.01f;
    
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        texScale -= 0.01f;
}