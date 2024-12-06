#ifndef PCH_H
#define PCH_H

#define SCREEN_WIDTH 1920 
#define SCREEN_HEIGHT 1080

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <map>

// OpenGL
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <gl/glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Crosshair.h"
#include "stb_image.h"
#include "Target.h"

#endif // PCH_H
