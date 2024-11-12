#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <GLFW/glfw3.h>

void process_input(GLFWwindow* window);

int draw_rectangle(GLFWwindow* window, unsigned int shaderProgram);
int draw_triangle(GLFWwindow* window, unsigned int shaderProgram);
int draw_two_triangles(GLFWwindow* window, unsigned int shaderProgram);

#endif
