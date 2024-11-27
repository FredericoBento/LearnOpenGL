#include <iostream>

#include "Camera.h"
#include "Shader.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/geometric.hpp"
#include "stb_image.h"
#include "functions.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void going_3d_plane(GLFWwindow* window)
{
    //9.7
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
         std::cout << "Failed to load texture" << std::endl;  
    }

    stbi_image_free(data);
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data2 = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
         std::cout << "Failed to load texture" << std::endl;  
    }

    stbi_image_free(data2);


    Shader shader("shaders/917/shader.vs", "shaders/917/shader.fs");

    
    float vertices[] = {
        0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
       -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


    shader.use();
    shader.setInt("texture2", 1);

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    float opacity = 0.1f;

    while(!glfwWindowShouldClose(window)) 
    {
        process_input(window);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            if (opacity < 1.0f)
            {
                opacity += 0.01f;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if (opacity > 0.0f)
            {
                opacity -= 0.01f;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // specify the color before clear/'painting'
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setFloat("opacity", opacity);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void smile_cube_3d(GLFWwindow* window)
{
    // 9.8    

    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
         std::cout << "Failed to load texture" << std::endl;  
    }

    stbi_image_free(data);
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data2 = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
         std::cout << "Failed to load texture" << std::endl;  
    }

    stbi_image_free(data2);


    Shader shader("shaders/918/shader.vs", "shaders/918/shader.fs");

    
    float vertices[] = {
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

       -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

       -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, 0.5f, -0.5f,  0.0f, 1.0f    
   };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


    shader.use();
    shader.setInt("texture2", 1);

    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    unsigned int projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    float opacity = 0.1f;

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) 
    {
        process_input(window);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            if (opacity < 1.0f)
            {
                opacity += 0.01f;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if (opacity > 0.0f)
            {
                opacity -= 0.01f;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // specify the color before clear/'painting'
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setFloat("opacity", opacity);

        model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
   
}

void smile_ten_cubes_3d(GLFWwindow* window)
{
    // 9.8.2   

    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glGenTextures(1, &texture2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
         std::cout << "Failed to load texture" << std::endl;  
    }

    stbi_image_free(data);
    
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data2 = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
         std::cout << "Failed to load texture" << std::endl;  
    }

    stbi_image_free(data2);


    Shader shader("shaders/918/shader.vs", "shaders/918/shader.fs");

    
    float vertices[] = {
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

       -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
       -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,
       -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

       -0.5f, 0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, 0.5f,  0.5f,  1.0f, 0.0f,
       -0.5f, 0.5f,  0.5f,  0.0f, 0.0f,
       -0.5f, 0.5f, -0.5f,  0.0f, 1.0f    
   };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,   0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f,  -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f,  -3.5f),
        glm::vec3(-1.7f,  3.0f,  -7.5f),
        glm::vec3( 1.3f, -2.0f,  -2.5f),
        glm::vec3( 1.5f,  2.0f,  -2.5f),
        glm::vec3( 1.5f,  0.2f,  -1.5f),
        glm::vec3(-1.3f,  1.0f,  -1.5f)    
    };


    shader.use();
    shader.setInt("texture2", 1);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    float opacity = 0.1f;

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) 
    {
        process_input(window);

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            if (opacity < 1.0f)
            {
                opacity += 0.01f;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            if (opacity > 0.0f)
            {
                opacity -= 0.01f;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // specify the color before clear/'painting'
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        shader.setFloat("opacity", opacity);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle),
                                glm::vec3(1.0f, 0.3f, 0.5f));

            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void rotating_cube(GLFWwindow* window)
{
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
    };

    unsigned int indices[] = {
        // Bottom Face
        0, 1, 2,
        2, 3, 1,        
        // Top Face
        4, 5, 6,
        6, 7, 5,
        // Left Face
        0, 1, 5,
        5, 0, 4,
        // Right Face
        2, 3, 7,
        7, 2, 6,
        // Front Face
        0, 2, 6,
        6, 0, 4,
        // Back Face
        1, 3, 7,
        7, 1, 5
    };

    unsigned int VAO, VBO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // glm::vec3 position = glm::vec3( -1.0f, 0.5f, -0.5f);
    glm::vec3 position = glm::vec3( 0.0f, 0.0f, -0.0f);
   
    Shader shader("shaders/multicolor_cubes/shader.vs", "shaders/multicolor_cubes/shader.fs");    

    shader.use();

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glEnable(GL_DEPTH_TEST);
    glLineWidth(3.0f);

    float rotationAngle = 0.0f;
    float rotationSpeed = 0.05f;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);

    while (!glfwWindowShouldClose(window))
    {

        process_input(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);

        rotationAngle += rotationSpeed * glfwGetTime();
        glfwSetTime(0.0);

        if (rotationAngle >= 360.0f)
        {
            rotationAngle = 0; 
        }

        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

        glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);

        shader.setMat4("model", model);
        shader.setVec3("color", color);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        color = glm::vec3(0.0f, 0.0f, 1.0f);
        shader.setVec3("color", color);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}
void rotating_multiple_cubes(GLFWwindow* window)
{
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
    };

    unsigned int indices[] = {
        // Bottom Face
        0, 1, 2,
        2, 3, 1,        
        // Top Face
        4, 5, 6,
        6, 7, 5,
        // Left Face
        0, 1, 5,
        5, 0, 4,
        // Right Face
        2, 3, 7,
        7, 2, 6,
        // Front Face
        0, 2, 6,
        6, 0, 4,
        // Back Face
        1, 3, 7,
        7, 1, 5
    };

    unsigned int VAO, VBO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // glm::mat4 view = glm::mat4(1.0f);
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -8.0f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    Shader shader("shaders/multicolor_cubes/shader.vs", "shaders/multicolor_cubes/shader.fs");    

    shader.use();

    // shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glEnable(GL_DEPTH_TEST);
    glLineWidth(3.0f);

    float rotationAngle = 0.0f;
    float rotationSpeed = 0.05f;


    glm::vec3 positions[] = {
        glm::vec3(-1.0f, 1.0f, 0.5f),
        glm::vec3( 0.0f, 1.0f, 0.5f),
        glm::vec3( 1.0f, 1.0f, 0.5f),

        glm::vec3(-1.0f, 0.0f, 0.5f),
        glm::vec3( 0.0f, 0.0f, 0.5f),
        glm::vec3( 1.0f, 0.0f, 0.5f),

        glm::vec3(-1.0f, -1.0f, 0.5f),
        glm::vec3( 0.0f, -1.0f, 0.5f),
        glm::vec3( 1.0f, -1.0f, 0.5f),
    };

    double time = 0.0f;

    while (!glfwWindowShouldClose(window))
    {

        process_input(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);

        time += glfwGetTime();
        rotationAngle += rotationSpeed * glfwGetTime()+ 1.0f;
        glfwSetTime(0.0);

        if (rotationAngle >= 360.0f)
        {
            rotationAngle = 0; 
        }

        float radius = 5.0f;
        float camX = sin(time) * radius;
        float camZ = cos(time) * radius;

        glm::mat4 view;
        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f)
                );

        shader.setMat4("view", view);

        for (int i = 0; i < 9; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, positions[i]);
            model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

            glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);

            shader.setMat4("model", model);
            shader.setVec3("color", color);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

            color = glm::vec3(0.0f, 0.0f, 1.0f);
            shader.setVec3("color", color);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void rotating_multiple_cubes_with_camera(GLFWwindow* window)
{
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
    };

    unsigned int indices[] = {
        // Bottom Face
        0, 1, 2,
        2, 3, 1,        
        // Top Face
        4, 5, 6,
        6, 7, 5,
        // Left Face
        0, 1, 5,
        5, 0, 4,
        // Right Face
        2, 3, 7,
        7, 2, 6,
        // Front Face
        0, 2, 6,
        6, 0, 4,
        // Back Face
        1, 3, 7,
        7, 1, 5
    };

    unsigned int VAO, VBO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    Shader shader("shaders/multicolor_cubes/shader.vs", "shaders/multicolor_cubes/shader.fs");    

    shader.use();

    // shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glEnable(GL_DEPTH_TEST);
    glLineWidth(3.0f);

    float rotationAngle = 0.0f;
    float rotationSpeed = 0.05f;


    glm::vec3 positions[] = {
        glm::vec3(-1.0f, 1.0f, 0.5f),
        glm::vec3( 0.0f, 1.0f, 0.5f),
        glm::vec3( 1.0f, 1.0f, 0.5f),

        glm::vec3(-1.0f, 0.0f, 0.5f),
        glm::vec3( 0.0f, 0.0f, 0.5f),
        glm::vec3( 1.0f, 0.0f, 0.5f),

        glm::vec3(-1.0f, -1.0f, 0.5f),
        glm::vec3( 0.0f, -1.0f, 0.5f),
        glm::vec3( 1.0f, -1.0f, 0.5f),
    };

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view;

    shader.setMat4("view", view);

    const float cameraSpeedScalar = 3.5f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    float cameraSpeed = 0.0f;
    

    while (!glfwWindowShouldClose(window))
    {
        currentFrame += glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cameraSpeed = cameraSpeedScalar * deltaTime;

        process_input(window);
      
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);

        rotationAngle += rotationSpeed * glfwGetTime()+ 1.0f;
        glfwSetTime(0.0);

        if (rotationAngle >= 360.0f)
        {
            rotationAngle = 0; 
        }


        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        shader.setMat4("view", view);

        for (int i = 0; i < 9; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, positions[i]);
            model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

            glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);

            shader.setMat4("model", model);
            shader.setVec3("color", color);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

            color = glm::vec3(0.0f, 0.0f, 1.0f);
            shader.setVec3("color", color);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

float lastX = 400, lastY = 300;
float yaw = 0, pitch = 0;
float fov = 45.0f;

void rotating_multiple_cubes_with_mouse_camera(GLFWwindow* window)
{
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
    };

    unsigned int indices[] = {
        // Bottom Face
        0, 1, 2,
        2, 3, 1,        
        // Top Face
        4, 5, 6,
        6, 7, 5,
        // Left Face
        0, 1, 5,
        5, 0, 4,
        // Right Face
        2, 3, 7,
        7, 2, 6,
        // Front Face
        0, 2, 6,
        6, 0, 4,
        // Back Face
        1, 3, 7,
        7, 1, 5
    };

    unsigned int VAO, VBO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


    Shader shader("shaders/multicolor_cubes/shader.vs", "shaders/multicolor_cubes/shader.fs");    

    shader.use();

    glEnable(GL_DEPTH_TEST);
    glLineWidth(3.0f);

    float rotationAngle = 0.0f;
    float rotationSpeed = 0.05f;


    glm::vec3 positions[] = {
        glm::vec3(-1.0f, 1.0f, 0.5f),
        glm::vec3( 0.0f, 1.0f, 0.5f),
        glm::vec3( 1.0f, 1.0f, 0.5f),

        glm::vec3(-1.0f, 0.0f, 0.5f),
        glm::vec3( 0.0f, 0.0f, 0.5f),
        glm::vec3( 1.0f, 0.0f, 0.5f),

        glm::vec3(-1.0f, -1.0f, 0.5f),
        glm::vec3( 0.0f, -1.0f, 0.5f),
        glm::vec3( 1.0f, -1.0f, 0.5f),
    };

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view;

    shader.setMat4("view", view);

    const float cameraSpeedScalar = 3.5f;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;
    float cameraSpeed = 0.0f;

   

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        currentFrame += glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        cameraSpeed = cameraSpeedScalar * deltaTime;

        process_input(window);
      
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);

        rotationAngle += rotationSpeed * glfwGetTime()+ 1.0f;
        glfwSetTime(0.0);

        if (rotationAngle >= 360.0f)
        {
            rotationAngle = 0; 
        }

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFront = glm::normalize(direction);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        glm::mat4 projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        for (int i = 0; i < 9; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, positions[i]);
            model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

            glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);

            shader.setMat4("model", model);
            shader.setVec3("color", color);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

            color = glm::vec3(0.0f, 0.0f, 1.0f);
            shader.setVec3("color", color);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void rotating_multiple_cubes_with_mouse_camera_class(GLFWwindow* window)
{
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
    };

    unsigned int indices[] = {
        // Bottom Face
        0, 1, 2,
        2, 3, 1,        
        // Top Face
        4, 5, 6,
        6, 7, 5,
        // Left Face
        0, 1, 5,
        5, 0, 4,
        // Right Face
        2, 3, 7,
        7, 2, 6,
        // Front Face
        0, 2, 6,
        6, 0, 4,
        // Back Face
        1, 3, 7,
        7, 1, 5
    };

    unsigned int VAO, VBO, EBO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);


    Shader shader("shaders/multicolor_cubes/shader.vs", "shaders/multicolor_cubes/shader.fs");    

    shader.use();

    glEnable(GL_DEPTH_TEST);
    glLineWidth(3.0f);

    float rotationAngle = 0.0f;
    float rotationSpeed = 0.05f;


    glm::vec3 positions[] = {
        glm::vec3(-1.0f, 1.0f, 0.5f),
        glm::vec3( 0.0f, 1.0f, 0.5f),
        glm::vec3( 1.0f, 1.0f, 0.5f),

        glm::vec3(-1.0f, 0.0f, 0.5f),
        glm::vec3( 0.0f, 0.0f, 0.5f),
        glm::vec3( 1.0f, 0.0f, 0.5f),

        glm::vec3(-1.0f, -1.0f, 0.5f),
        glm::vec3( 0.0f, -1.0f, 0.5f),
        glm::vec3( 1.0f, -1.0f, 0.5f),
    };

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float currentFrame = 0.0f;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, camera.MouseMovementCallback);
    glfwSetScrollCallback(window, camera.MouseScrollCallback);

    while (!glfwWindowShouldClose(window))
    {
        currentFrame += glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window);
      
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FOWARD, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);

        rotationAngle += rotationSpeed * glfwGetTime()+ 1.0f;
        glfwSetTime(0.0);

        if (rotationAngle >= 360.0f)
        {
            rotationAngle = 0; 
        }

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), 800.0f / 600.0f, 0.1f, 100.0f);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        for (int i = 0; i < 9; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(model, positions[i]);
            model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 1.0f));
            model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

            glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);

            shader.setMat4("model", model);
            shader.setVec3("color", color);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);

            color = glm::vec3(0.0f, 0.0f, 1.0f);
            shader.setVec3("color", color);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    (void)* window; // remove unused warning

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) 
        pitch = 89.0f;

    if (pitch < -89.0f) 
        pitch = -89.0f;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    (void) window;
    (void) xoffset;

    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;

}
