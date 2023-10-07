#pragma once

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <GL/glew.h>

class Shader{
public:
	unsigned int Program;
	Shader(const char* vertexPath, const char* fragmentPath);
	void Use();
};

