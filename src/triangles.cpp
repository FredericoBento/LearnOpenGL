#include <iostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "functions.h"

int draw_triangle(GLFWwindow* window, unsigned int shaderProgram)
{
    float vertices[] = {
       0.0f,  0.5f, 0.0f,
       0.5f,  -0.5f, 0.0f,
       -0.5f,  -0.5f, 0.0f,
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* 
        1st Arg: The location we set on the shader
        2nd Arg: Size of the vertex attribute in this case vec3
        3rd Arg: Type of the data
        4th Arg: Whether we want the data to be normalized
        5th Arg: Stride - space between consecutive vertex attributes
        6th Arg: Offset of where the position data begins in the buffer
    */

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    while(!glfwWindowShouldClose(window)) 
    {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // specify the color before clear/'painting'
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

int draw_two_triangles(GLFWwindow* window, unsigned int shaderProgram)
{ 
    float leftTriangle[] = {
        -0.5f, 1.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f,
    };

    float rightTriangle[] = {
         0.5f,  1.0f, 0.0f,
         1.0f,  0.0f, 0.0f,
         0.0f,  0.0f, 0.0f,
    };
  
    unsigned int VAO1, VAO2;
    unsigned int VBO1,  VBO2;

    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);

    glGenVertexArrays(1, &VAO1);
    glGenVertexArrays(1, &VAO2);
    
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leftTriangle), leftTriangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rightTriangle), rightTriangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    

    while(!glfwWindowShouldClose(window))
    {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);

    glDeleteVertexArrays(1, &VAO1);
    glDeleteVertexArrays(1, &VAO2);
    
    return 0;
}
