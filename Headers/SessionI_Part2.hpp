// Preprocessor Directives
#pragma once


// System Headers
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <btBulletDynamicsCommon.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <shader.hpp>
#include <camera.hpp>
#include <heightmap.hpp>


#include <iostream>


#include <stb_image.h>


namespace SessionI_Part2
{
	int SessionI_Part2();
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow *window);
}




