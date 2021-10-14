#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <vector>

const int WIDTH = 1280;
const int HEIGHT = 720;
const double PI = 3.1415926535897932385;
const double EVALUE = 2.7182818284590452353;

void SHOWGREEN(std::string str) 
{
	std::cout << "\033[0;32m" << str << "\033[0m" << std::endl;
}

void SHOWRED(std::string str) 
{
	std::cout << "\033[0;31m" << str << "\033[0m" << std::endl;
}

void SHOWYELLOW(std::string str)
{
	std::cout << "\033[0;33m" << str << "\033[0m" << std::endl;
}

void SHOWBLUE(std::string str)
{
	std::cout << "\033[0;34m" << str << "\033[0m" << std::endl;
}