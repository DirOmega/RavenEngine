#pragma once

#ifdef WIN32
#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#else
#include <unistd.h>
#define Sleep(t) sleep(t)
#endif

#include "gl3w.h"

#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1

#include "glfw3.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
