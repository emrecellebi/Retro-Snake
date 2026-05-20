#ifndef CONGIG_H
#define CONGIG_H

#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "raylib/rcamera.h"
#include "raylib/rlgl.h"

#define CURRENT_FPS			60

/** Screens **/
#define SCREEN_WIDTH 		1280
#define SCREEN_HEIGHT 		800
#define CENTER_WIDTH 		(GetScreenWidth() / 2)
#define CENTER_HEIGHT 		(GetScreenHeight() / 2)

/** Colors **/
#define RGB(r, g, b)		CLITERAL(Color){ r, g, b, 255 }
#define RGBA(r, g, b, a)	CLITERAL(Color){ r, g, b, a }

#endif