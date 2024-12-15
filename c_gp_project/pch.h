#ifndef PCH_H
#define PCH_H

#define _CRT_SECURE_NO_WARNINGS
#define SCREEN_WIDTH 1920 
#define SCREEN_HEIGHT 1080

#define _USE_MATH_DEFINES

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <map>
#include <windows.h>

// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <GL/glm/glm.hpp>
#include <GL/glm/gtc/matrix_transform.hpp>
#include <GL/glm/gtc/type_ptr.hpp>
#include "tiny_obj_loader.h"
#include "stb_image.h"

#include "Shader.h"
#include "Camera.h"
#include "Gun.h"
#include "Crosshair.h"
#include "background.h"
#include "Target.h"

#endif // PCH_H