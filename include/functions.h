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

void draw_rectangle_container_texture(GLFWwindow* window);
void draw_rectangle_container_multiple_texture(GLFWwindow* window);
void draw_rectangle_container_multiple_texture_inverse(GLFWwindow* window);
void draw_rectangle_container_multiple_texture_different_wrapping(GLFWwindow* window);
void draw_rectangle_container_multiple_texture_783(GLFWwindow* window);
void draw_rectangle_container_multiple_texture_784(GLFWwindow* window);
void draw_rectangle_container_multiple_texture_translate(GLFWwindow* window);

void going_3d_plane(GLFWwindow* window);
void smile_cube_3d(GLFWwindow* window);
void smile_ten_cubes_3d(GLFWwindow* window);

void rotating_cube(GLFWwindow* window);
void rotating_multiple_cubes(GLFWwindow* window);
void rotating_multiple_cubes_with_camera(GLFWwindow* window);
void rotating_multiple_cubes_with_mouse_camera(GLFWwindow* window);
void rotating_multiple_cubes_with_mouse_camera_class(GLFWwindow* window);

#endif
