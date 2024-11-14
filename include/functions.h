#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <GLFW/glfw3.h>

void process_input(GLFWwindow* window);

int draw_rectangle(GLFWwindow* window, unsigned int shaderProgram);
int draw_triangle(GLFWwindow* window, unsigned int shaderProgram);
int draw_two_triangles(GLFWwindow* window, unsigned int shaderProgram);
int draw_two_triangles_different_color(GLFWwindow* window);
void draw_multicolor_triangle(GLFWwindow* window);
void draw_multicolor_triangle_without_uniform(GLFWwindow* window); 
void draw_triangle_upside_down(GLFWwindow* window);
void draw_triangle_right_side(GLFWwindow* window);
void draw_triangle_right_side_other_color(GLFWwindow* window);

#endif
