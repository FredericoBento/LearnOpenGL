#include <iostream>
#include <math.h>

#include "Shader.h"
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

int draw_two_triangles_different_color(GLFWwindow* window)
{ 
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
            "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* orangleFragShaderSource= "#version 330 core\n"
        "out vec4 FragColor;"
        "void main()\n"
        "{\n"
            "FragColor= vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    const char* yellowFragShaderSource = "#version 330 core\n"
        "out vec4 FragColor;"
        "void main()\n"
        "{\n"
            "FragColor= vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char log[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(vertexShader, sizeof(log), NULL, log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                log << std::endl;
    }

    unsigned int orangeFragShader;  
    orangeFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(orangeFragShader, 1, &orangleFragShaderSource, NULL);
    glCompileShader(orangeFragShader);
    glGetShaderiv(orangeFragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(orangeFragShader, sizeof(log), NULL,  log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << 
                log << std::endl;
    }

    unsigned int yellowFragShader;  
    yellowFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(yellowFragShader, 1, &yellowFragShaderSource, NULL);
    glCompileShader(yellowFragShader);
    glGetShaderiv(yellowFragShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(yellowFragShader, sizeof(log), NULL,  log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << 
                log << std::endl;
    }

    unsigned int shaderProgram[2];
    shaderProgram[0] = glCreateProgram();
    shaderProgram[1] = glCreateProgram();

    glAttachShader(shaderProgram[0], vertexShader);
    glAttachShader(shaderProgram[1], vertexShader);

    glAttachShader(shaderProgram[0], orangeFragShader);
    glAttachShader(shaderProgram[1], yellowFragShader);

   glLinkProgram(shaderProgram[0]);
   glLinkProgram(shaderProgram[1]);

   for (int i = 0; i < 2; i++) 
   {
       glGetProgramiv(shaderProgram[i], GL_LINK_STATUS, &success);
       if (!success)
       {
           glGetProgramInfoLog(shaderProgram[i], sizeof(log), NULL, log);
           std::cout << "ERROR:SHADER::PROGRAM:::LINKING_FAILED\n" <<
                   log << std::endl;
       }
   }

   glDeleteShader(vertexShader);
   glDeleteShader(orangeFragShader);
   glDeleteShader(yellowFragShader);
        
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

   

    while(!glfwWindowShouldClose(window))
    {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram[0]);

        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram[1]);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(orangeFragShader);
    glDeleteShader(yellowFragShader);
    glDeleteProgram(shaderProgram[0]);
    glDeleteProgram(shaderProgram[1]);

    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);

    glDeleteVertexArrays(1, &VAO1);
    glDeleteVertexArrays(1, &VAO2);
    
    return 0;
}

 void draw_multicolor_triangle(GLFWwindow* window) 
 {
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;"
        "uniform vec4 ourColor;"
        "void main()\n"
        "{\n"
        "FragColor = ourColor;\n"
        "}\0";

    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
    }

    unsigned int shaderProgram; //Will link the shaders, using the output of each as the input of the other
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
                infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

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

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glad_glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

 }
 void draw_multicolor_triangle_without_uniform(GLFWwindow* window) 
 {
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 ourColor;"
        "void main()\n"
        "{\n"
            "gl_Position = vec4(aPos, 1.0);\n"
            "ourColor = aColor;"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;"
        "in vec3 ourColor;"
        "void main()\n"
        "{\n"
            "FragColor = vec4(ourColor, 1.0);\n"
        "}\0";

    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) 
    {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
    }

    unsigned int shaderProgram; //Will link the shaders, using the output of each as the input of the other
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
                infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
       0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 *sizeof(float)));
    glEnableVertexAttribArray(1);

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

 }

void draw_triangle_upside_down(GLFWwindow* window)
{
    // 6.8 - 1

    Shader shader("shaders/681/shader.vs", "shaders/681/shader.fs");

    float vertices[] = {
       0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 *sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)) 
    {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // specify the color before clear/'painting'
        glClear(GL_COLOR_BUFFER_BIT);
    
        shader.use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void draw_triangle_right_side(GLFWwindow* window)
{
    // 6.8 - 2

    Shader shader("shaders/682/shader.vs", "shaders/682/shader.fs");
    float offset = 0.5f;

    float vertices[] = {
       0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 *sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)) 
    {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // specify the color before clear/'painting'
        glClear(GL_COLOR_BUFFER_BIT);
    
        shader.use();
        shader.setFloat("offsetX", offset);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void draw_triangle_right_side_other_color(GLFWwindow* window)
{
    // 6.8 - 3

    Shader shader("shaders/683/shader.vs", "shaders/683/shader.fs");
    float offset = 0.5f;

    float vertices[] = {
       0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 *sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)) 
    {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // specify the color before clear/'painting'
        glClear(GL_COLOR_BUFFER_BIT);
    
        shader.use();
        shader.setFloat("offsetX", offset);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }


    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

