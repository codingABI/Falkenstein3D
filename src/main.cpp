/*
 * Project: Falkenstein3D
 * Description: OpenGL raycaster game. Player has to find the exit of the castle. Game is short and has only one level. It is more a technical demo than a real long game.
 *
 * Copyright (c) 2022 codingABI, 2-Clause BSD License
 *
 * created by codingABI https://github.com/codingABI/Falkenstein3D 
 *
 * This project uses two different raycaster engines, which can be select by pressing Key "3":
 * - Degree based raycaster by codingABI, inspired by https://github.com/3DSage/OpenGL-Raycaster_v1 and https://github.com/3DSage/OpenGL-Raycaster_v2
 * - DDA Raycaster by Lode Vandevenne (see license details below), faster and used by default
 *
 * Keyboard control:
 * s/S - Change pixel size (impacts performance)
 * 1   - on/off for textures for floor and roof
 * 2   - on/off for floor, roof, sky and ground
 * 3   - change raycaster engine (old from codingABI <-> DDA from Lode Vandevenne) 
 * 4   - on/off for round pixels
 * 5   - on/off for automatically set pixel size dependent on framerate
 * t/T - on/off for all textures
 * f/F - on/off for fullscreen mode
 * ESC,q,Q - exit program
 * Cursor left/right - rotate player
 * Cursor up/down - move player forward/backward
 * 
 * Joystick control:
 * left/right - rotate player
 * up/down - move player forward/backward
 * 
 * Mouse control:
 * left/right mouse button - rotate player
 * middle mouse button     - move player forward
 * scroll button backward  - move player backward
 *
 * History:
 * 16.06.2022, Initial version
 * 29.06.2022, Add more comments
 * 30.06.2022, Fix offset bug in sky and ground texture, when changing pixel size
 * 01.07.2022, Add joystick and mouse support
 * 01.07.2022, First upload to github
 * 14.10.2022, Replace strncpy by snprintf
 *
 * ----------------------------------------------------------------
 * License details:
 * ----------------------------------------------------------------
 *
 * ================================================================
 * Sourcecode (except DDA raycaster functions)
 * ================================================================
 * License: 2-Clause BSD License
 * Copyright 2022 codingABI
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS 
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * ================================================================
 * DDA raycaster functions (drawRaycastDDA, drawBackground, sortSprites, drawSprites)
 * ================================================================
 * Copyright (c) 2004-2021, Lode Vandevenne
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * ROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ================================================================
 * freeGlut 3.x
 * ================================================================
 * Copyright (c) 1999-2000 Pawel W. Olszta, X-Consortium license
 * Freeglut Copyright
 * ------------------
 *    
 * Freeglut code without an explicit copyright is covered by the following 
 * copyright:
 *    
 * Copyright (c) 1999-2000 Pawel W. Olszta. All Rights Reserved.
 * Permission is hereby granted, free of charge,  to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction,  including without limitation the rights 
 * to use, copy,  modify, merge,  publish, distribute,  sublicense,  and/or sell 
 * copies or substantial portions of the Software.
 *    
 * The above  copyright notice  and this permission notice  shall be included in 
 * all copies or substantial portions of the Software.
 *    
 * THE SOFTWARE  IS PROVIDED "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS OR 
 * IMPLIED,  INCLUDING  BUT  NOT LIMITED  TO THE WARRANTIES  OF MERCHANTABILITY, 
 * FITNESS  FOR  A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN  NO EVENT  SHALL 
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER 
 * IN  AN ACTION  OF CONTRACT,  TORT OR OTHERWISE,  ARISING FROM,  OUT OF  OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *    
 * Except as contained in this notice,  the name of Pawel W. Olszta shall not be 
 * used  in advertising  or otherwise to promote the sale, use or other dealings 
 * in this Software without prior written authorization from Pawel W. Olszta.
 *
 * ================================================================
 * Textures Nr6, 7
 * ================================================================
 * Screaming Brain Studios, CC0
 * https://opengameart.org/content/tiny-texture-pack-1
 *
 * ================================================================
 * All other textures (when #define FREETEXTURES)
 * ================================================================
 * Copyright (c) 2022 codingABI, CC BY-SA 3.0
 *
 */

#define FREETEXTURES // Only CC0 or CC-BY-SA 3.0-Textures

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <GL/freeglut.h>
#include <math.h>

#ifdef FREETEXTURES 
#include "./textures_free.h" //Only CC0 or CC-BY-SA 3.0-Textures
#else
#include "./textures.h"
#endif

#define GRIDSIZE 32 // size of wall height or width
#define MAPWIDTH 16 // width of map
#define MAPHEIGHT 16 // height of map
#define STRIPEHEIGHT 32 // height of wall
#define VIEWERBOXSIZE 6 // size of viewer in 2d view
#define SKYSCALE 5 // pixel size of sky texture
#define MAXWIDTH 4096// maximal 3d view width (because of zbuffer)

#define PREVEREDVIEWANGLE 40 // viewer angle if viewport is 1:1
#define MINVIEWPORT3DWIDTH 200 // minimal width of 3d view
#define SIDEUNKNOWN 0
#define SIDELEFTRIGHT 1
#define SIDEUPDOWN 2	
#define HUGEBIGNUMBER 100000
#define STEPSIZE 0.03125f // distance when moving viewer one step forward or backward
 
// abs with support for floats
#define myAbs(x) ((x)>0?(x):-(x))

// initial viewer settings
#define DEFAULTVIEWERX 4
#define DEFAULTVIEWERY 13
#define DEFAULTVIEWERANGLE 103
// Position to finish the game
#define FINISHX 15
#define FINISHY 1

// Current viewer position, angle
float g_viewerX;
float g_viewerY;
float g_viewerAngle;

int g_fps=0; // current frames per second

// Status for pressed cursor keys
bool g_buttonUpPressed = false;
bool g_buttonDownPressed = false;
bool g_buttonLeftPressed = false;
bool g_buttonRightPressed = false;

// Status for joystick
bool g_joystickForward = false;
bool g_joystickBackward = false;
bool g_joystickLeft = false;
bool g_joystickRight = false;

// Status for mouse
bool g_mouseForward = false;
bool g_mouseBackward = false;
bool g_mouseLeft = false;
bool g_mouseRight = false;

int g_windowHeight; // Complete inner window height
int g_windowWidth; // Complete inner window width
int g_viewPort3dWidth; // width of 3d viewport (downscaled bye g_pixelSize)
int g_viewPort3dHeight; // height of 3d viewport (downscaled bye g_pixelSize)
int g_viewPort3dHalfHeight; // half of 3d-height (to improve speed)
int g_viewPort3dPhysicalWidth; // real width of 3d viewport
int g_viewPort3dPhysicalHeight; // real height of 3d viewport
int g_viewPort3dOffsetX; // begin of real 3d viewport
int g_pixelSize=2; // size of display pixel
int g_pixelOffset; // x/y-offset for pixel
int g_lineOffset; // x-offset for line 
float g_textureSkyGroundStepX; // texture pixel stepsize in sky and ground texture per display pixel step  

bool g_fullScreenMode = true; // Fullscreen mode active? (No 2D map)
bool g_showTextures = true; // Textures enabled?
bool g_showBackgroundTexture = true; // Texture for sky enabled?
bool g_showBackground = true; // Floor and roof enabled?
bool g_oldStyle = false; // use old raycaster
bool g_roundPixels = false; // round pixels?
bool g_autoPixelSize = true; // set pixel size automatically dependent on framerate
// Temporary stored previous window dimensions, when using fullscreen mode
int g_savedWindowWidth;
int g_savedWindowHeight;

// Text which to be displayed in the center of the screen if needed
#define DISPLAYTEXTMAXLENGTH 80
#define DISPLAYTEXTTIMEOUT 5
#define DISPLAYTEXTFINISHDURATIONLENGTH 5
char g_displayText[DISPLAYTEXTMAXLENGTH+1] ="";
bool g_displayTextBlinking = false;

// games states
#define STATE_START 0
#define STATE_RUNNING 1
#define STATE_FINISHED 2
#define STATE_QUIT 3
int g_state;
int g_stateStartTime;

// time of start and end of a game 
int g_gameStartTime;
int g_gameEndTime;

// cached sin and cos to improve speed
double g_cachedCos, g_cachedSin, g_cachedCos90, g_cachedSin90;

//1D Zbuffer for sprite handling
double g_zBuffer[MAXWIDTH];

// check if box in grid is filled with wall
#define ISGRIDFILLED(x,y) ((g_wallMap[(int)y][(int)x]) > 0)
// check if position is within map
#define ISGRIDINMAP(x,y) !(((int)x<0) || ((int)x > MAPWIDTH-1) || ((int)y< 0) || ((int)y > MAPHEIGHT-1))

// Map of walls
const unsigned int g_defaultWallMap[MAPHEIGHT][MAPWIDTH]= {
 {  1, 1, 8, 1, 4, 1, 8, 1, 1,13, 1,13, 1,13,24, 1 },
 {  1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,15, 0,26 },
 {  8, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1 },
 {  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0,13 },
 {  4, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 9, 0, 0, 0, 1 },
 {  1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 9, 0, 0, 0, 0,24 },
 {  8, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
 {  1, 8,10, 1, 8, 1, 8, 1, 8, 1, 8, 1, 8, 1, 8, 1 },
 {  8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,13 },
 {  1, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 9, 0,24 },
 {  8, 0, 0, 0, 0, 0, 9, 9, 9, 0, 0, 0, 0, 9, 0,13 },
 {  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 1 },
 { 11, 0, 0, 0, 0, 0,15,23,19, 0, 0, 0, 0, 0, 0,13 },
 {  1, 0, 0, 0, 0, 0,21, 0,25, 0, 9, 0, 0, 0, 0, 1 },
 {  8, 0, 0, 0, 0, 0,17, 0,22, 0, 0, 0, 0, 0, 0,13 },
 {  1,13, 1,24, 1,13, 1,16, 1, 1,13, 1,13, 1,13, 1 }
};
unsigned int g_wallMap[MAPHEIGHT][MAPWIDTH];

// Floor map
const unsigned int g_defaultFloorMap[MAPHEIGHT][MAPWIDTH]= {
 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,2 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,2,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,2,5,5,5,5,5,5,0 },
 { 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0 },
 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};	
unsigned int g_floorMap[MAPHEIGHT][MAPWIDTH];

// Roof map
const unsigned int g_defaultRoofMap[MAPHEIGHT][MAPWIDTH]= {
 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
 { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,2 },
 { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,0,0,0,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,0,0,0,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,0,0,0,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,2,7,7,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0 },
 { 7,7,7,7,7,7,7,7,2,7,7,7,7,7,7,0 },
 { 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0 },
 { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
};	

// sprite definitions
#define SPRITECOLLECTION 1
#define SPRITEOPENER 2
struct Sprite {
	double x; // x-pos of sprite
	double y; // y-pos of sprite
	int texture; // texture
	int type; // type of sprite: SPRITECOLLECTION and/or SPRITEOPENER
	bool collected; // is collected (and as a result hidden)?
	int openX; // x-pos of wall to be opened, if collected
	int openY; // y-pos of wall to be opened, if collected
};

#define MAXSPRITES 3
Sprite g_sprites[MAXSPRITES] =
{
	{10.5, 14.5, TEXTUREWALLOPENER01,SPRITECOLLECTION+SPRITEOPENER,false,2,7},
	{11.5,  5.5, TEXTUREWALLOPENER02,SPRITECOLLECTION+SPRITEOPENER,false,8,13},
	{ 7.5, 14.5, TEXTUREWALLOPENER03,SPRITECOLLECTION+SPRITEOPENER,false,15,1}
};

//arrays used to sort the sprites
int g_spriteOrder[MAXSPRITES];
double g_spriteDistance[MAXSPRITES];

// Calculate direction vector and camera plane for DDA method
void preparePositionDataForDDA() {
	float vectorLength;
	g_cachedSin = sin(M_PI*g_viewerAngle/180);
	g_cachedCos = cos(M_PI*g_viewerAngle/180);

	float halfPovAngle = (float) (g_viewPort3dWidth)/2/((g_viewPort3dWidth) / (PREVEREDVIEWANGLE*((float) g_viewPort3dWidth/g_viewPort3dHeight)));
	if (sin(M_PI*halfPovAngle/180)==0)	{
		vectorLength = HUGEBIGNUMBER;
	} else vectorLength = cos(M_PI*halfPovAngle/180)/sin(M_PI*halfPovAngle/180);

	g_cachedCos90 = cos(M_PI*(g_viewerAngle+90)/180)/vectorLength;
	g_cachedSin90 = sin(M_PI*(g_viewerAngle+90)/180)/vectorLength;
}

// Draw 2D map
void drawMap() {
	// Grid to show walls
	glBegin(GL_QUADS);

	for (int x=0;x<MAPWIDTH;x++) {
		for (int y=0;y<MAPHEIGHT;y++) {
			if (g_wallMap[y][x] > 0) glColor3f(0.5,0.5,0.5); else glColor3f(1,1,1);
			glVertex2i(x*GRIDSIZE + 1,y*GRIDSIZE+1);
			glVertex2i((x+1)*GRIDSIZE - 1,y*GRIDSIZE+1);
			glVertex2i((x+1)*GRIDSIZE - 1,(y+1)*GRIDSIZE-1);
			glVertex2i(x*GRIDSIZE + 1,(y+1)*GRIDSIZE-1);
		}
	}
	glEnd();
}

// Draw viewer on 2D map
void drawViewer() {
	// quad at position
	glColor3f(0,0,1);
	glBegin(GL_QUADS);
	glVertex2i((g_viewerX*GRIDSIZE-VIEWERBOXSIZE/2),(g_viewerY*GRIDSIZE-VIEWERBOXSIZE/2));
	glVertex2i((g_viewerX*GRIDSIZE+VIEWERBOXSIZE/2),(g_viewerY*GRIDSIZE-VIEWERBOXSIZE/2));
	glVertex2i((g_viewerX*GRIDSIZE+VIEWERBOXSIZE/2),(g_viewerY*GRIDSIZE+VIEWERBOXSIZE/2));
	glVertex2i((g_viewerX*GRIDSIZE-VIEWERBOXSIZE/2),(g_viewerY*GRIDSIZE+VIEWERBOXSIZE/2));
	glEnd();
	
	// line in view direction
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2i(g_viewerX*GRIDSIZE,g_viewerY*GRIDSIZE);
	glVertex2i((g_viewerX*GRIDSIZE+cos(M_PI*g_viewerAngle/180)*VIEWERBOXSIZE*4),(g_viewerY*GRIDSIZE+sin(M_PI*g_viewerAngle/180)*VIEWERBOXSIZE*4));
	glEnd();						
}

// Draw sky, ground, floor and roof (floor and roof based on https://lodev.org/cgtutor/raycasting.html, (c) 2004-2021, Lode Vandevenne)
void drawBackground() {
	float deltaY,darken;
	int pixel, red, green, blue, texture;
	int textureSkyGroundDeltaX = 0;
	static GLint autoSkyRotateTime = 0;
	static int autoSkyRotate = 0;
	bool isInMap = false;
	
	if (!g_showBackground) { // draw floor if background is not disabled
		
		if (!g_roundPixels) { // quad pixels
			// floor plane
			glLineWidth(1);
			glColor3f(0.4,0.4,0.4);
			glBegin(GL_QUADS);
			glVertex2i(g_viewPort3dOffsetX,g_pixelSize*g_viewPort3dHeight/2);
			glVertex2i(g_viewPort3dOffsetX+g_pixelSize*g_viewPort3dWidth,g_pixelSize*g_viewPort3dHeight/2);
			glVertex2i(g_viewPort3dOffsetX+g_pixelSize*g_viewPort3dWidth,g_pixelSize*g_viewPort3dHeight-1);
			glVertex2i(g_viewPort3dOffsetX,g_pixelSize*g_viewPort3dHeight-1);
			glEnd();
		} else { // round pixels
			glPointSize(g_pixelSize);
			glColor3f(0.4,0.4,0.4);

			glBegin(GL_POINTS);

	      	for (int viewPortX=0;viewPortX<g_viewPort3dWidth;viewPortX++) {
   		      	for (int viewPortY=0;viewPortY<g_viewPort3dHalfHeight;viewPortY++) {
					glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHalfHeight)*g_pixelSize+viewPortY*g_pixelSize+g_pixelOffset);
				}
			}
			glEnd();
		}	
	}
	
	if (g_oldStyle)	return; // old style raycaster makes sky by himself
	
	if (!g_showBackground) return;

	if (glutGet(GLUT_ELAPSED_TIME) - autoSkyRotateTime > 100) { // move sky every 100 ms one texture pixel
		autoSkyRotate++;
		autoSkyRotateTime=glutGet(GLUT_ELAPSED_TIME);
	}

	int textureSkyGroundOffsetViewer = (float) (6*SKYSCALE*TEXTURESIZE*g_viewerAngle/360); // texture offset for ground and sky, dependent on viewer rotation	
	int textureSkyGroundOffsetAutoRotate = (autoSkyRotate+ textureSkyGroundOffsetViewer/SKYSCALE)%TEXTURESIZE; // texture pixel offset for ground and sky, dependent on viewer rotation and time
	int textureSkyGroundOffsetStatic = (textureSkyGroundOffsetViewer/SKYSCALE)%TEXTURESIZE; // texture pixel offset for ground and sky, dependent on viewer rotation

	glPointSize(g_pixelSize);
	glBegin(GL_POINTS);				
	
	for (int viewPortY = 0;viewPortY < g_viewPort3dHalfHeight;viewPortY++) {
		// rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
      	float rayDirX0 = g_cachedCos - g_cachedCos90;
      	float rayDirY0 = g_cachedSin - g_cachedSin90;
      	float rayDirX1 = g_cachedCos + g_cachedCos90;
      	float rayDirY1 = g_cachedSin + g_cachedSin90;

      	// Horizontal distance from the camera to the floor for the current row.
      	// 0.5 is the z position exactly in the middle between floor and ceiling.
      	double rowDistance = rowDistance = (double) g_viewPort3dHalfHeight / (viewPortY+1);
		
		// calculate the real world step vector we have to add for each x (parallel to camera plane)
	    // adding step by step avoids multiplications with a weight in the inner loop
      	float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / g_viewPort3dWidth;
      	float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / g_viewPort3dWidth;

      	// real world coordinates of the leftmost column. This will be updated as we step to the right.
      	float floorX = g_viewerX + rowDistance * rayDirX0;
      	float floorY = g_viewerY + rowDistance * rayDirY0;
      	
		float textureSkyGroundDeltaX = 0;

      	for (int viewPortX=0;viewPortX<g_viewPort3dWidth;viewPortX++) {
			
			// the cell coord is simply got from the integer parts of floorX and floorY
        	int cellX = (int)(floorX);
        	int cellY = (int)(floorY);

        	// get the texture coordinate from the fractional part
        	int tx = (int)(TEXTURESIZE*(floorX - cellX)) & (TEXTURESIZE - 1);
        	int ty = (int)(TEXTURESIZE*(floorY - cellY)) & (TEXTURESIZE - 1);
        
        	isInMap = ISGRIDINMAP(floorX,floorY);
			darken = (float) 1+100.0f/((viewPortY+1)*g_pixelSize);
			textureSkyGroundDeltaX += g_textureSkyGroundStepX;

			// Floor
			if (isInMap) {		
				texture = g_floorMap[(int)(floorY)][(int)(floorX)];
			
				if (texture > 0 && g_showTextures && g_showBackgroundTexture) {		
					pixel = (ty*TEXTURESIZE + tx)*3;
		
			        red = g_textures[texture-1][pixel];
			        green = g_textures[texture-1][pixel+1];
			        blue = g_textures[texture-1][pixel+2];
					
					glColor3ub(red/darken,green/darken,blue/darken);
					glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHalfHeight)*g_pixelSize+viewPortY*g_pixelSize+g_pixelOffset);
				}
				if (texture > 0 && (!g_showTextures || !g_showBackgroundTexture)) {		
					glColor3ub(255/darken,0,255/darken);
					glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHalfHeight)*g_pixelSize+viewPortY*g_pixelSize+g_pixelOffset);
				}
			}
			
			// Ground
			if (!isInMap || (texture == 0 )) {
				if (g_showTextures) {
					int pixel=(((g_pixelSize*viewPortY/SKYSCALE)%TEXTURESIZE)*TEXTURESIZE+(textureSkyGroundOffsetStatic+(int) textureSkyGroundDeltaX)%TEXTURESIZE)*3;
					int red   =g_textures[TEXTUREGROUND][pixel+0];
					int green =g_textures[TEXTUREGROUND][pixel+1];
					int blue  =g_textures[TEXTUREGROUND][pixel+2];
					glColor3ub(red/darken,green/darken,blue/darken);
				} else glColor3ub(0,255/darken,255/darken);
				glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHalfHeight)*g_pixelSize+viewPortY*g_pixelSize+g_pixelOffset);
			}

			// Roof
			if (isInMap) {
				texture = g_defaultRoofMap[(int)floorY][(int)floorX];
		
				if (texture > 0 && g_showTextures && g_showBackgroundTexture) {		
					pixel = (ty*TEXTURESIZE + tx)*3;
		
			        red = g_textures[texture-1][pixel];
			        green = g_textures[texture-1][pixel+1];
			        blue = g_textures[texture-1][pixel+2];
					
					glColor3ub(red/darken,green/darken,blue/darken);
					glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHalfHeight-1)*g_pixelSize-viewPortY*g_pixelSize+g_pixelOffset);
				}
				if (texture > 0 && (!g_showTextures || !g_showBackgroundTexture)) {		
					glColor3ub(255/darken,255/darken,0);
					glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHalfHeight-1)*g_pixelSize-viewPortY*g_pixelSize+g_pixelOffset);
				}				
			}

			// Sky
			if (!isInMap || (texture == 0 )) {
				if (g_showTextures) {
					int pixel=(((g_pixelSize*viewPortY/SKYSCALE)%TEXTURESIZE)*TEXTURESIZE+(textureSkyGroundOffsetAutoRotate+(int) textureSkyGroundDeltaX)%TEXTURESIZE)*3;
					int red   =g_textures[TEXTURESKY][pixel+0];
					int green =g_textures[TEXTURESKY][pixel+1];
					int blue  =g_textures[TEXTURESKY][pixel+2];
	
					glColor3ub(red/darken,green/darken,blue/darken);
				} else glColor3ub(0,0,255/darken);
				glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHalfHeight-1)*g_pixelSize-viewPortY*g_pixelSize+g_pixelOffset);
			}

    		floorX += floorStepX;
        	floorY += floorStepY;
		}
	}
	glEnd(); 	
}

// Get RGB for texture pixel
bool getTextureColor(int texture, bool side, int pixel, float darken, int &red, int &green, int &blue) {
	red = g_textures[texture][pixel];
	green = g_textures[texture][pixel + 1];
	blue = g_textures[texture][pixel + 2];
	if ((red == 255) && (green == 0) && (blue == 255)) { // special color
		if (texture==TEXTURECANDLE) { // candle
			red = 255-((glutGet(GLUT_ELAPSED_TIME)/10)&15);
			green = 220-((glutGet(GLUT_ELAPSED_TIME)/10)&31);
			blue = 49;
			red/=darken;
			green/=darken;
			blue/=darken;
			if (side) {
				red/=2;
				green/=2;
				blue/=2;	
			}
			return true;
		}
		if (texture==TEXTURECOLORLINE) { // red color line
			red = 255-(glutGet(GLUT_ELAPSED_TIME)/10)&255;
			green = 0;
			blue = 0;
			red/=darken;
			green/=darken;
			blue/=darken;
			if (side) {
				red/=2;
				green/=2;
				blue/=2;	
			}
			return true;
		}
		return false;
	} else {
		red/=darken;
		green/=darken;
		blue/=darken;

		if (side) {
			red/=2;
			green/=2;
			blue/=2;	
		}
		return true; 
	}
}

// Sort algorithm (sort the sprites based on distance, from https://lodev.org/cgtutor/raycasting.html, (c) 2004-2021, Lode Vandevenne)
void sortSprites(int* order, double* dist, int amount)
{
	std::vector<std::pair<double, int> > sprites(amount);
	for(int i = 0; i < amount; i++) {
		sprites[i].first = dist[i];
		sprites[i].second = order[i];
	}
	std::sort(sprites.begin(), sprites.end());
	// restore in reverse order to go from farthest to nearest
	for(int i = 0; i < amount; i++) {
		dist[i] = sprites[amount - i - 1].first;
		order[i] = sprites[amount - i - 1].second;
	}
}

// Draw sprites (based on https://lodev.org/cgtutor/raycasting.html, (c) 2004-2021, Lode Vandevenne)
void drawSprites() {
	int red, green, blue, x , y;
		
	for(int i = 0; i < MAXSPRITES; i++) {
		g_spriteOrder[i] = i;
		g_spriteDistance[i] = ((g_viewerX - g_sprites[i].x) * (g_viewerX - g_sprites[i].x) + (g_viewerY - g_sprites[i].y) * (g_viewerY - g_sprites[i].y)); //sqrt not taken, unneeded
    }
    
    sortSprites(g_spriteOrder, g_spriteDistance, MAXSPRITES);
   	for(int i = 0; i < MAXSPRITES; i++) {
   		if (!g_sprites[g_spriteOrder[i]].collected && ((g_sprites[g_spriteOrder[i]].type & SPRITECOLLECTION) == SPRITECOLLECTION)) {
   			// hide collectable sprite when reached
	   		if (((int)g_sprites[g_spriteOrder[i]].x == (int) g_viewerX) && ((int)g_sprites[g_spriteOrder[i]].y==(int) g_viewerY)) {
				g_sprites[g_spriteOrder[i]].collected = true;
				if ((g_sprites[g_spriteOrder[i]].type & SPRITEOPENER) == SPRITEOPENER) { // sprite to open a wall
					x = g_sprites[g_spriteOrder[i]].openX;
					y = g_sprites[g_spriteOrder[i]].openY;
					// change floor texture near open wall
					if (y > 0) g_floorMap[y-1][x] = TEXTUREROUGHWALL+1; 
					if (y < MAPHEIGHT-1) g_floorMap[y+1][x] = TEXTUREROUGHWALL+1;
					if (x > 0) g_floorMap[y][x-1] = TEXTUREROUGHWALL+1; 
					if (x < MAPWIDTH-1) g_floorMap[y][x+1] = TEXTUREROUGHWALL+1; 
					g_wallMap[y][x] = 0; // open wall
										
					g_stateStartTime = glutGet(GLUT_ELAPSED_TIME);
					snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Wall open");
	    			g_displayTextBlinking = false;					
				}
				continue;
			}

			//translate sprite position to relative to camera
			double spriteX = g_sprites[g_spriteOrder[i]].x - g_viewerX;
			double spriteY = g_sprites[g_spriteOrder[i]].y - g_viewerY;
			
			//transform sprite with the inverse camera matrix
			// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
			// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
			// [ planeY   dirY ]                                          [ -planeY  planeX ]
					
			double invDet = 1.0 / (g_cachedCos90 * g_cachedSin - g_cachedCos * g_cachedSin90); //required for correct matrix multiplication
			
			double transformX = invDet * (g_cachedSin * spriteX - g_cachedCos * spriteY);
			double transformY = invDet * (-g_cachedSin90 * spriteX + g_cachedCos90 * spriteY); //this is actually the depth inside the screen, that what Z is in 3D
			
			int spriteScreenX = int((g_viewPort3dWidth / 2) * (1 + transformX / transformY));	
			
			//calculate height of the sprite on screen
			int spriteHeight = abs(int(g_viewPort3dHeight / (transformY))); //using 'transformY' instead of the real distance prevents fisheye
			//calculate lowest and highest pixel to fill in current stripe
			int drawStartY = -spriteHeight / 2 + g_viewPort3dHeight / 2;
			if(drawStartY < 0) drawStartY = 0;
			int drawEndY = spriteHeight / 2 + g_viewPort3dHeight / 2;
			if(drawEndY >= g_viewPort3dHeight) drawEndY = g_viewPort3dHeight - 1;
			
			//calculate width of the sprite
			int spriteWidth = spriteHeight;
			int drawStartX = -spriteWidth / 2 + spriteScreenX;
			if(drawStartX < 0) drawStartX = 0;
			int drawEndX = spriteWidth / 2 + spriteScreenX;
			if(drawEndX >= g_viewPort3dWidth) drawEndX = g_viewPort3dWidth - 1;
			
			//loop through every vertical stripe of the sprite on screen
			for(int stripe = drawStartX; stripe < drawEndX; stripe++) {
				int texX = int(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEXTURESIZE / spriteWidth) / 256;
				//the conditions in the if are:
				//1) it's in front of camera plane so you don't see things behind you
				//2) it's on the screen (left)
				//3) it's on the screen (right)
				//4) g_zBuffer, with perpendicular distance
	
				if(transformY > 0 && stripe > 0 && stripe < g_viewPort3dWidth && transformY < g_zBuffer[stripe]) {
					for(int y = drawStartY; y < drawEndY; y++) { //for every pixel of the current stripe
						int d = (y) * 256 - g_viewPort3dHeight * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
						int texY = ((d * TEXTURESIZE) / spriteHeight) / 256;
						glPointSize(g_pixelSize);
						glBegin(GL_POINTS);
						if (getTextureColor(g_sprites[g_spriteOrder[i]].texture,false, (TEXTURESIZE * texY + texX)*3, 1, red, green, blue)) {
							glColor3ub(red,green,blue); 
							glVertex2i(g_viewPort3dOffsetX + stripe*g_pixelSize+g_pixelOffset,y*g_pixelSize+g_pixelOffset);
						}
						glEnd();
					}  
				}
			}
		}
	}
}

// Raycaster via DDA (based on https://lodev.org/cgtutor/raycasting.html, (c) 2004-2021, Lode Vandevenne)
void drawRaycastDDA() {
	int red,green,blue;
	float darken;
	bool offMap;
	
	//WALL CASTING
    for(int x = 0; x < g_viewPort3dWidth; x++) {    	
		//calculate ray position and direction
		double cameraX = 2 * x / double(g_viewPort3dWidth) - 1; //x-coordinate in camera space
		double rayDirX = (g_cachedCos + g_cachedCos90 * cameraX);
		double rayDirY = (g_cachedSin + g_cachedSin90 * cameraX);
	
		//which box of the map we're in
		int mapX = int(g_viewerX);
		int mapY = int(g_viewerY);
		
		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;
		
		//length of ray from one x or y-side to next x or y-side
		double deltaDistX = (rayDirX == 0) ? 1e30 : myAbs(1 / rayDirX);
		double deltaDistY = (rayDirY == 0) ? 1e30 : myAbs(1 / rayDirY);
		double perpWallDist;
	  	
		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;
		
		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?
		
		//calculate step and initial sideDist
		if (rayDirX < 0) {
			stepX = -1;
			sideDistX = (g_viewerX - mapX) * deltaDistX;
		} else {
			stepX = 1;
			sideDistX = (mapX + 1.0 - g_viewerX) * deltaDistX;
		}
		if (rayDirY < 0) {
			stepY = -1;
			sideDistY = (g_viewerY - mapY) * deltaDistY;
		} else {
			stepY = 1;
			sideDistY = (mapY + 1.0 - g_viewerY) * deltaDistY;
		}
		
		//perform DDA
	  	offMap = false;
		while (hit == 0) {
			//jump to next map square, either in x-direction, or in y-direction
			if (sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			} else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
	    	//Check if ray has hit a wall	
	    	offMap = !ISGRIDINMAP(mapX,mapY);
	    	if (offMap || g_wallMap[mapY][mapX] > 0) hit = 1;
	  	}
	
		//Calculate distance of perpendicular ray (Euclidean distance would give fisheye effect!)
		if(side == 0) perpWallDist = (sideDistX - deltaDistX);
		else          perpWallDist = (sideDistY - deltaDistY);
		
		if (perpWallDist == 0) perpWallDist = 0.0001; // Prevent DIV0, can occur if position is very, very close to a wall
		//Calculate height of line to draw on screen
		int lineHeight = (int)(g_viewPort3dHeight / perpWallDist); // +4 in my case to fill the gaps between wall, floor and roof (or add floor and roof also for walls)
		if (lineHeight & 1) lineHeight ++; // odd height for better symetry

		darken = 1+perpWallDist/10.0f; // darken wall if far away
	
		if (!g_fullScreenMode) {
	  		// draw FOV
			glColor3f(0,1,0);
			glLineWidth(1);
			glBegin(GL_LINES);
			glVertex2i(g_viewerX*GRIDSIZE,g_viewerY*GRIDSIZE);
			glVertex2i((g_viewerX+(g_cachedCos+g_cachedCos90))*GRIDSIZE,(g_viewerY+(g_cachedSin+g_cachedSin90))*GRIDSIZE);
			glVertex2i(g_viewerX*GRIDSIZE,g_viewerY*GRIDSIZE);
			glVertex2i((g_viewerX+(g_cachedCos-g_cachedCos90))*GRIDSIZE,(g_viewerY+(g_cachedSin-g_cachedSin90))*GRIDSIZE);
			glEnd();
		}	
	
		if (offMap) continue; // no wall

		if (lineHeight<2) continue; // wall too small
		
      	//SET THE ZBUFFER FOR THE SPRITE CASTING
      	g_zBuffer[x] = perpWallDist; //perpendicular distance is used
	  		
		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + g_viewPort3dHalfHeight;
		
		if(drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + g_viewPort3dHalfHeight;
		if(drawEnd > g_viewPort3dHeight) drawEnd = g_viewPort3dHeight;
		
		if (g_showTextures) {

			//texturing calculations
			int texNum = g_wallMap[mapY][mapX] -1;  // Nr. of texture
					
			//calculate value of wallX
			double wallX; //where exactly the wall was hit
			if (side == 0) wallX = g_viewerY + perpWallDist * rayDirY;
			else           wallX = g_viewerX + perpWallDist * rayDirX;
			wallX -= floor((wallX));
			
			//x coordinate on the texture
			int texX = int(wallX * double(TEXTURESIZE));
			if(side == 0 && rayDirX > 0) texX = TEXTURESIZE - texX - 1;
			if(side == 1 && rayDirY < 0) texX = TEXTURESIZE - texX - 1;
		
			// How much to increase the texture coordinate per screen pixel
			double step = 1.0 * TEXTURESIZE / (lineHeight-1);
			
			// Starting texture coordinate
			double texPos = (double) (drawStart - g_viewPort3dHalfHeight + lineHeight / 2) * step;
			glPointSize(g_pixelSize);
			glBegin(GL_POINTS);
	
			for(int y = drawStart; y<drawEnd; y++) {
				// Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
				int texY = (int)texPos & (TEXTURESIZE - 1);
				texPos += step;
				
				int pixel = ((int)texY*TEXTURESIZE + TEXTURESIZE-texX-1)*3;
				if (getTextureColor(texNum, side == 1, pixel, darken, red, green, blue)) {
					glColor3ub(red,green,blue); 
					glVertex2i(g_viewPort3dOffsetX + x*g_pixelSize+g_pixelOffset,y*g_pixelSize+g_pixelOffset);
				}
			}
			glEnd();
		} else { // no textures enabled
			if (side != 0) glColor3f(1/darken,0,0); else glColor3f(0,1/darken,0);
			glLineWidth(g_pixelSize);
			glBegin(GL_LINES);
			glVertex2i(g_viewPort3dOffsetX + x*g_pixelSize+g_lineOffset,drawStart*g_pixelSize-1);
			glVertex2i(g_viewPort3dOffsetX + x*g_pixelSize+g_lineOffset,drawEnd*g_pixelSize-1);
			glEnd();
		}
	}	
}

// Draw raycasted scene (inspired on raycaster ideas from https://github.com/3DSage/OpenGL-Raycaster_v1 and https://github.com/3DSage/OpenGL-Raycaster_v2)
void drawRaycast() {
	float finalCrossingX,finalCrossingY;
	float angle;
	double crossingX, crossingY;
	float centerCrossingX, centerCrossingY, centerCrossingI;
	double distanceX,distanceY;
	float minDistance;
	int height;
	double cachedCos, cachedSin, cachedTan;
	double cachedFishEyeCos;
	int side;
	int lastSide = SIDEUNKNOWN;
	bool finalCrossingFound;
	double deltaX, deltaY;
	float angleStep;
	float centerAngleDiff = HUGEBIGNUMBER;
	int beginOfStripe;
	double textureX, textureY;
	int red, green, blue;
	int pixel;
	int texture;
	float darken;
	bool horizontalOffMap, verticalOffMap;
	bool isInMap = false;
	static GLint autoSkyRotateTime = 0;
	static int autoSkyRotate = 0;
	
	if (glutGet(GLUT_ELAPSED_TIME) - autoSkyRotateTime > 100) { // move sky every 100 ms one texture pixel
		autoSkyRotate++;
		autoSkyRotateTime=glutGet(GLUT_ELAPSED_TIME);
	}

	int textureSkyGroundOffsetViewer = (float) (6*SKYSCALE*TEXTURESIZE*g_viewerAngle/360); // texture offset for ground and sky, dependent on viewer rotation	
	int textureSkyGroundOffsetAutoRotate = (autoSkyRotate+ textureSkyGroundOffsetViewer/SKYSCALE)%TEXTURESIZE; // texture pixel offset for ground and sky, dependent on viewer rotation and time
	int textureSkyGroundOffsetStatic = (textureSkyGroundOffsetViewer/SKYSCALE)%TEXTURESIZE; // texture pixel offset for ground and sky, dependent on viewer rotation

	// Angle step dependent on viewport aspect ratio and stripe width
 	angleStep = g_viewPort3dWidth / (PREVEREDVIEWANGLE*((float) g_viewPort3dWidth/g_viewPort3dHeight));

	for (int viewPortX=0;viewPortX<g_viewPort3dWidth;viewPortX++) {

		// new ray angle and fix angle to element of [0;360[
		angle = (float) g_viewerAngle - ((g_viewPort3dWidth-1)/2 - viewPortX) / angleStep;
		if (angle < 0) angle += 360;
		if (angle > 360) angle -= 360;
		
		side = SIDEUNKNOWN;
		finalCrossingFound = false;
		crossingX = g_viewerX;
		crossingY = g_viewerY;
		cachedSin = sin(M_PI*angle/180);
		cachedCos = cos(M_PI*angle/180);
		if (cachedCos != 0) cachedTan = cachedSin/cachedCos; else cachedTan = HUGEBIGNUMBER; // replacement for tan(M_PI*angle/180); 
		cachedTan = tan(M_PI*angle/180);
		cachedFishEyeCos = cos(M_PI*(g_viewerAngle-angle)/180);
		distanceX = HUGEBIGNUMBER;
		distanceY = HUGEBIGNUMBER;

		do { // left or right (crossing vertical wall faces)
			if ((crossingX == g_viewerX) && (crossingY == g_viewerY)) { // first step
				if (cachedCos > 0.001){ // right
					crossingX=(int)g_viewerX+1; // grid on right
					crossingY=g_viewerY - (g_viewerX - crossingX)*cachedTan;
					// delta for the next steps
					deltaX = 1;
					deltaY = deltaX*cachedTan;
			 	} else if (cachedCos < -0.001){ // left
					crossingX=((int)g_viewerX)-0.0001; // grid on left
					crossingY=g_viewerY - (g_viewerX - crossingX)*cachedTan;
					deltaX = -1;
					deltaY = deltaX*cachedTan;
				} else {
					// too close to up or down
					crossingX=g_viewerX; 
					crossingY=g_viewerY; 
					finalCrossingFound = true;
			 	}
			 } else { // following steps
			 	verticalOffMap = !ISGRIDINMAP(crossingX,crossingY);
				if (verticalOffMap || ISGRIDFILLED(crossingX,crossingY)) { // Wall found
			  		distanceX=cachedCos*(crossingX-g_viewerX)+cachedSin*(crossingY-g_viewerY); // calculate distance between points by using transformation of Pythagorean trigonometric identity (faster then sqrt(dx^2+dy^2)).
			  		finalCrossingFound = true;
				} else {
					crossingX+=deltaX;
					crossingY+=deltaY;
				}
			 }			
		} while (!finalCrossingFound);
		
		finalCrossingX = crossingX;
		finalCrossingY = crossingY;
		crossingX = g_viewerX;
		crossingY = g_viewerY;
		finalCrossingFound = false;
		if (cachedTan == 0)  cachedTan = 0.001; // Prevent DIV0
		
		do { // up or down (crossing horizontal wall faces)
			if ((crossingX == g_viewerX) && (crossingY == g_viewerY)) { // first step
				if (cachedSin < -0.001){ // up
					crossingY=((int)g_viewerY)-0.0001; // upper grid
					crossingX=g_viewerX - (g_viewerY - crossingY)/cachedTan;
					// delta for the next steps
					deltaY = -1;
					deltaX = deltaY/cachedTan;
			 	} else if (cachedSin > 0.001){ // down
					crossingY=(int)g_viewerY+1;
					crossingX=g_viewerX - (g_viewerY - crossingY)/cachedTan;
					deltaY = 1;
					deltaX = deltaY/cachedTan;
				} else {
					// too close to left or right
					crossingX=g_viewerX; 
					crossingY=g_viewerY; 
					finalCrossingFound = true;
			 	}

			 } else { // following steps
			 	horizontalOffMap = !ISGRIDINMAP(crossingX,crossingY);	 
				if (horizontalOffMap || ISGRIDFILLED(crossingX,crossingY)) { // Wall found or outer
					distanceY=cachedCos*(crossingX-g_viewerX)+cachedSin*(crossingY-g_viewerY); // calculate distance between points by using transformation of Pythagorean trigonometric identity (faster then sqrt(dx^2+dy^2)).
			  		finalCrossingFound = true;
				} else {
					crossingX+=deltaX;
					crossingY+=deltaY;
				}
			}			
		} while (!finalCrossingFound);
	
		if( distanceY < distanceX -0.01){ 
			finalCrossingX=crossingX; 
			finalCrossingY=crossingY; 
			side = SIDEUPDOWN;
			minDistance = distanceY;
		} else if (distanceX < distanceY -0.01) { 
			minDistance = distanceX;
			side = SIDELEFTRIGHT;
		} else { 
			// can not determine if horizontal or vertical => use last used side
			minDistance = distanceX;
			side = lastSide;
		}
		
		minDistance= minDistance*cos(M_PI*(g_viewerAngle-angle)/180); //fisheye fix 
		darken = 1+minDistance/10; // darken wall if far away

		// Color for 2D lines or faces without textures
		switch (side) {
			case SIDEUPDOWN: glColor3f(1/darken,0,0); break;
			case SIDELEFTRIGHT: glColor3f(0,1/darken,0); break;
			case SIDEUNKNOWN: glColor3f(0,0,0); break; // when first ray is not clear to decide	
		}
			
		if (!g_fullScreenMode) {
	  		// line from viewer to crossing point
			glLineWidth(1);
			glBegin(GL_LINES);
			glVertex2i(g_viewerX*GRIDSIZE,g_viewerY*GRIDSIZE);
			glVertex2i(finalCrossingX*GRIDSIZE,finalCrossingY*GRIDSIZE);
			glEnd();

			// Point on crossing point
			glPointSize(1);
			glBegin(GL_POINTS);
			glVertex2i(finalCrossingX*GRIDSIZE,finalCrossingY*GRIDSIZE);
			glEnd();			
		}	
		
		if (!horizontalOffMap || !verticalOffMap) { // wall found
			// wall
		
			// current height of wall stripe (smaller if far away)			
			height = (g_viewPort3dHeight)/(minDistance); 
			if (height & 1) height++; // only odd height for symetry
			
			//SET THE ZBUFFER FOR THE SPRITE CASTING
      		g_zBuffer[viewPortX] = minDistance;

			// texture pixel height is proportional to max/real wall stripe height
			deltaY = (double) TEXTURESIZE/(height-1);
			double offsetTextureY = 0;
			if(height>g_viewPort3dHeight) { // Bigger than viewer port
				offsetTextureY=(height-g_viewPort3dHeight)/2.0; // half of "oversize"
				height=g_viewPort3dHeight; // reduce wall stripe size to viewport height
			}
		
			beginOfStripe = g_viewPort3dHalfHeight - height/2; // horizontal start of stripe
			double textureY = offsetTextureY*deltaY; // line in texture and take care of "oversize" to avoid glitches
		
			if (g_showTextures) {
				texture = (g_wallMap[(int)finalCrossingY][(int)finalCrossingX]); // Nr. of texture
				
				if (side == SIDELEFTRIGHT) { // if horizontal wall face => calc texture column from crossing y value MOD wall width and fix column direction dependent on left/right
					textureX =(int)(finalCrossingY*TEXTURESIZE)%TEXTURESIZE; // column in texture
					if(angle<90 || angle>270) textureX=TEXTURESIZE-1-textureX; // flip if needed
				}	
				if (side == SIDEUPDOWN) { // if vertical wall face => calc texture column from crossing x value MOD wall height and fix column direction dependent on up/down
					textureX=(int)(finalCrossingX*TEXTURESIZE)%TEXTURESIZE; // column in texture
					if(angle>180) textureX=TEXTURESIZE-1-textureX; // flip if needed
				}

				glPointSize(g_pixelSize);
				glBegin(GL_POINTS);
				for (int k=0;k<height;k++) {
					// get color from texture
					int pixel = ((int)(textureY)%TEXTURESIZE)*TEXTURESIZE*3 + (TEXTURESIZE-(int)(textureX)%TEXTURESIZE-1)*3;
					if (getTextureColor(texture-1, side == SIDEUPDOWN, pixel, darken, red, green, blue)) {
						glColor3ub(red,green,blue); 
						glVertex2i(g_viewPort3dOffsetX + viewPortX*g_pixelSize+g_pixelOffset,k*g_pixelSize + beginOfStripe*g_pixelSize+g_pixelOffset);
					} else { // special case, when wall point is transparent
						if (k + beginOfStripe >= g_viewPort3dHalfHeight) {
							// ground
							float backgroundDarken = 1+100/(((k+beginOfStripe)-g_viewPort3dHalfHeight) * cachedFishEyeCos * g_pixelSize);

							if (g_showBackground) {
								int pixel=(((g_pixelSize*(k + beginOfStripe)/SKYSCALE)%TEXTURESIZE)*TEXTURESIZE+(textureSkyGroundOffsetStatic+(int) (viewPortX*g_textureSkyGroundStepX))%TEXTURESIZE)*3;
								int red   =g_textures[TEXTUREGROUND][pixel+0];
								int green =g_textures[TEXTUREGROUND][pixel+1];
								int blue  =g_textures[TEXTUREGROUND][pixel+2];
			
								glColor3ub(red/backgroundDarken,green/backgroundDarken,blue/backgroundDarken);
								glVertex2i(g_viewPort3dOffsetX + viewPortX*g_pixelSize+g_pixelOffset,k*g_pixelSize + beginOfStripe*g_pixelSize+g_pixelOffset);
							} 
						} else {
							// sky
							float backgroundDarken = 1+100/((g_viewPort3dHalfHeight-(k+beginOfStripe)) * cachedFishEyeCos * g_pixelSize);
							if (g_showBackground) {
								int pixel=(((g_pixelSize*(k + beginOfStripe)/SKYSCALE)%TEXTURESIZE)*TEXTURESIZE+(textureSkyGroundOffsetAutoRotate+(int) (viewPortX*g_textureSkyGroundStepX))%TEXTURESIZE)*3;
								int red   =g_textures[TEXTURESKY][pixel+0];
								int green =g_textures[TEXTURESKY][pixel+1];
								int blue  =g_textures[TEXTURESKY][pixel+2];
			
								glColor3ub(red/backgroundDarken,green/backgroundDarken,blue/backgroundDarken);
								glVertex2i(g_viewPort3dOffsetX + viewPortX*g_pixelSize+g_pixelOffset,k*g_pixelSize + beginOfStripe*g_pixelSize+g_pixelOffset);

							} 
						}			
					}
					textureY += deltaY;	// Next texture line
				}
				glEnd();
			} else {
				glLineWidth(g_pixelSize);
				glBegin(GL_LINES);
				glVertex2i(g_viewPort3dOffsetX + viewPortX*g_pixelSize+g_lineOffset,beginOfStripe*g_pixelSize-1);
				glVertex2i(g_viewPort3dOffsetX + viewPortX*g_pixelSize+g_lineOffset,beginOfStripe*g_pixelSize+height*g_pixelSize);
				glEnd();
			}
			lastSide = side; // remember side for next stripes where side can not be determined (distanceX == distanceY)
		} else {
			// no wall, open sky
			beginOfStripe = g_viewPort3dHalfHeight;
			height = 0;
			lastSide = SIDEUNKNOWN;
		}
	
		// record strip which is nearest to viewer angle
		if (abs((g_viewerAngle-angle)*100) < centerAngleDiff) {
			centerCrossingX = finalCrossingX;
			centerCrossingY = finalCrossingY;
			centerCrossingI = viewPortX;
			centerAngleDiff = abs((g_viewerAngle-angle)*100);
		}

		// sky, ground, floor and roof
		if (g_showBackground) {
			for(int viewPortY=beginOfStripe+height;viewPortY<g_viewPort3dHeight;viewPortY++) {
				deltaY=viewPortY - g_viewPort3dHalfHeight;
				if (cachedFishEyeCos == 0) cachedFishEyeCos == 0.00001; // prevent DIV0
				// Texture X/Y = viewer + Cos/Sin(angle)*HalfScreen*TextureSize/ProjectionDepth/FishEyeCosFix
				textureX=(double) g_viewerX*TEXTURESIZE + TEXTURESIZE*cachedCos*(g_viewPort3dHalfHeight-5)/(deltaY*cachedFishEyeCos);
				textureY=g_viewerY*TEXTURESIZE + cachedSin*(g_viewPort3dHalfHeight-5)*TEXTURESIZE/deltaY/cachedFishEyeCos;
				darken = 1+100/(deltaY * cachedFishEyeCos * g_pixelSize);

				glPointSize(g_pixelSize);
				glBegin(GL_POINTS);				
	
				isInMap = ISGRIDINMAP((int)(textureX/TEXTURESIZE),(int)(textureY/TEXTURESIZE));
				 
				if (isInMap) {
					// floor
					texture = g_floorMap[((int)textureY)/TEXTURESIZE][((int)textureX)/TEXTURESIZE];

			  		if (g_showTextures && g_showBackgroundTexture) {		
						pixel = ((int)(textureY)&(TEXTURESIZE-1))*TEXTURESIZE*3 + ((int)(textureX)&(TEXTURESIZE-1))*3;
										
						if (texture > 0) {
							red = g_textures[texture-1][pixel];
							green = g_textures[texture-1][pixel + 1];
							blue = g_textures[texture-1][pixel + 2];
			
							glColor3ub(red/darken,green/darken,blue/darken);
							glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,viewPortY*g_pixelSize+g_pixelOffset);
						}
					} else {
						// floor
						if (g_floorMap[((int)textureY)/TEXTURESIZE][((int)textureX)/TEXTURESIZE] > 0 ) {
							glColor3ub(255/darken,0,255/darken);
							glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,viewPortY*g_pixelSize+g_pixelOffset);
						}
					}
				}
				// Ground
				if (!isInMap || (texture == 0 )) {
					if (g_showTextures) {
						int pixel=(((g_pixelSize*viewPortY/SKYSCALE)%TEXTURESIZE)*TEXTURESIZE+(textureSkyGroundOffsetStatic+(int) (viewPortX*g_textureSkyGroundStepX))%TEXTURESIZE)*3;
						int red   =g_textures[TEXTUREGROUND][pixel+0];
						int green =g_textures[TEXTUREGROUND][pixel+1];
						int blue  =g_textures[TEXTUREGROUND][pixel+2];
						glColor3ub(red/darken,green/darken,blue/darken);
					} else glColor3ub(0,255/darken,255/darken);
					glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,viewPortY*g_pixelSize+g_pixelOffset);
				}
				// Roof
				if (isInMap) {
					texture = g_defaultRoofMap[(int)(textureY/TEXTURESIZE)][(int)(textureX/TEXTURESIZE)];
			  		if (g_showTextures && g_showBackgroundTexture) {		
						if (texture > 0) {
							red = g_textures[texture-1][pixel];
							green = g_textures[texture-1][pixel + 1];
							blue = g_textures[texture-1][pixel + 2];

							glColor3ub(red/darken,green/darken,blue/darken);
							glVertex2i(g_viewPort3dOffsetX+viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHeight-1)*g_pixelSize-viewPortY*g_pixelSize+g_pixelOffset);
						}	
					} else {// if no textures for floor and roof
						// roof
						if (g_defaultRoofMap[((int)textureY)/TEXTURESIZE][((int)textureX)/TEXTURESIZE] > 0) {
							glColor3ub(255/darken,255/darken,0);
							glVertex2i(g_viewPort3dOffsetX + viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHeight-1)*g_pixelSize-viewPortY*g_pixelSize+g_pixelOffset);
						}	
					}
				}
				// Sky
				if (!isInMap || (texture == 0 )) {
					if (g_showTextures) {
						int pixel=(((g_pixelSize*viewPortY/SKYSCALE)%TEXTURESIZE)*TEXTURESIZE+(textureSkyGroundOffsetAutoRotate+(int) (viewPortX*g_textureSkyGroundStepX))%TEXTURESIZE)*3;
						int red   =g_textures[TEXTURESKY][pixel+0];
						int green =g_textures[TEXTURESKY][pixel+1];
						int blue  =g_textures[TEXTURESKY][pixel+2];
		
						glColor3ub(red/darken,green/darken,blue/darken);
					} else glColor3ub(0,0,255/darken);
					glVertex2i(g_viewPort3dOffsetX + viewPortX*g_pixelSize+g_pixelOffset,(g_viewPort3dHeight-1)*g_pixelSize-viewPortY*g_pixelSize+g_pixelOffset);
				}
				glEnd();
			}
		}
	}

	if (!g_fullScreenMode) { // if not in fullscreen mode
		// Center line
		glColor3f(1,1,0);
		glLineWidth(1);

		glBegin(GL_LINES);
		glVertex2i(g_viewPort3dOffsetX + centerCrossingI*g_pixelSize+g_lineOffset,0);
		glVertex2i(g_viewPort3dOffsetX + centerCrossingI*g_pixelSize+g_lineOffset,g_viewPort3dHeight*g_pixelSize-1);
		glEnd();

		// line from viewer to center point
		glBegin(GL_LINES);
		glVertex2i(g_viewerX*GRIDSIZE,g_viewerY*GRIDSIZE);
		glVertex2i(centerCrossingX*GRIDSIZE,centerCrossingY*GRIDSIZE);
		glEnd();
	
		// center point
		glPointSize(4);
		glBegin(GL_POINTS);
		glVertex2i(centerCrossingX*GRIDSIZE,centerCrossingY*GRIDSIZE);
		glEnd();	
	}
}

// draw single bitmap on screen position and scale it
void drawBitmap(int textureNbr, int posX, int posY, int scale) {
    int pixel, red, green, blue;

	glPointSize(scale);
	glBegin(GL_POINTS);

	for (int x=0;x<TEXTURESIZE;x++) {
		for (int y=0;y<TEXTURESIZE;y++) {
			pixel = (y*TEXTURESIZE+x)*3;
			red = g_textures[textureNbr][pixel];
			green = g_textures[textureNbr][pixel+1];
			blue = g_textures[textureNbr][pixel+2];
			if ((red != 255) || (green != 0) || (blue != 255)) { // draw nontransparent pixel
				glColor3ub(red,green,blue);
				glVertex2i(posX + x * scale,posY + y * scale);				
			}
		}
	}
	glEnd();	
}

// Show fps, time and collected items on screen
void drawInfos(){
	#define TEXTURESYMBOLDIVIDER 2
    char strData[DISPLAYTEXTMAXLENGTH];
    int pixel, red, green, blue, posX, posY;
	
	// Collected sprite items in a smaller size
	posX = g_viewPort3dOffsetX + g_viewPort3dWidth*g_pixelSize-TEXTURESIZE/TEXTURESYMBOLDIVIDER-1; 
	posY = g_viewPort3dHeight*g_pixelSize-TEXTURESIZE/TEXTURESYMBOLDIVIDER-1;

	glPointSize(1);
	glBegin(GL_POINTS);

	for (int i=0;i<MAXSPRITES;i++) {
		if ((g_sprites[i].type & SPRITECOLLECTION == SPRITECOLLECTION) && g_sprites[i].collected){
			for (int x=0;x<TEXTURESIZE/TEXTURESYMBOLDIVIDER;x++) {
				for (int y=0;y<TEXTURESIZE/TEXTURESYMBOLDIVIDER;y++) {
					pixel = (y*TEXTURESIZE*TEXTURESYMBOLDIVIDER+x*TEXTURESYMBOLDIVIDER)*3;
					red = g_textures[g_sprites[i].texture][pixel];
					green = g_textures[g_sprites[i].texture][pixel+1];
					blue = g_textures[g_sprites[i].texture][pixel+2];
					if ((red != 255) || (green != 0) || (blue != 255)) { // draw nontransparent pixel
						glColor3ub(red,green,blue);
						glVertex2i(posX + x,posY + y);				
					}
				}
			}
			posX -= TEXTURESIZE/TEXTURESYMBOLDIVIDER;
		}
	}

	glEnd();	
	
	// frames per second    
    if (g_fps == 0) return;
	
	if (g_state == STATE_RUNNING) {
		snprintf(strData,DISPLAYTEXTMAXLENGTH,"%d seconds", (glutGet(GLUT_ELAPSED_TIME)-g_gameStartTime)/1000);	
	} else {
		snprintf(strData,DISPLAYTEXTMAXLENGTH,"%d fps", g_fps);	
	}
    glColor3f(1,1,1);
    glRasterPos2f(g_viewPort3dOffsetX + 10, g_viewPort3dHeight*g_pixelSize-18);

   	glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)strData);
}

// draw text in the middle of the screen (draw text twice: First black and than white with a little offset)
void drawCenteredTextLine(int posY,bool smallFont=false) {
    void *font;
    
    if (smallFont) font = GLUT_BITMAP_8_BY_13; else font = GLUT_BITMAP_TIMES_ROMAN_24;
    
	// draw text in the middle of the screen (draw text twice. First black and than white with a little offset)
    glColor3f(0,0,0);
    glRasterPos2f(g_viewPort3dOffsetX + g_viewPort3dWidth*g_pixelSize/2- glutBitmapLength(font, (unsigned char*) g_displayText)/2, posY);

	glutBitmapString(font, (unsigned char*)g_displayText);
	
    glColor3f(1,1,1);
    glRasterPos2f(g_viewPort3dOffsetX + g_viewPort3dWidth*g_pixelSize/2- glutBitmapLength(font, (unsigned char*) g_displayText)/2+2, posY+2);

	glutBitmapString(font, (unsigned char*)g_displayText);
}

// Draw text messages in the middle of the screen 
void drawMessage(){
    int timeDelta;

	if (g_state== STATE_START) drawBitmap(TEXTURELOGO,g_viewPort3dOffsetX,0,2);

	if (g_displayTextBlinking && ((glutGet(GLUT_ELAPSED_TIME)/1000) & 1)) return; // blink text every 1 second

	if (g_state == STATE_QUIT) { // Quit program state
		timeDelta = (glutGet(GLUT_ELAPSED_TIME)-g_stateStartTime)/1000;
		if (timeDelta < 0) timeDelta=0;
		if  (timeDelta > DISPLAYTEXTTIMEOUT) { // Quit program
			exit(0);
		} else { // pending exit, show licenses
			#ifndef FREETEXTURES
			int posY = g_viewPort3dHeight*g_pixelSize/2-(3*(26+32)+26);
			
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Compiled binary:");
			drawCenteredTextLine(posY,true);
			posY += 26;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"(c) 2022 codingABI, CC BY-NC-SA 4.0");
			drawCenteredTextLine(posY);
			posY += 36;

			#else
			
			int posY = g_viewPort3dHeight*g_pixelSize/2-(2*(26+32)+32);
			 
			#endif
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Source code (except DDA raycaster):");
			drawCenteredTextLine(posY,true);
			posY += 26;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"(c) 2022 codingABI, 2-Clause BSD");
			drawCenteredTextLine(posY);
			posY += 36;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"DDA raycaster:");
			drawCenteredTextLine(posY,true);
			posY += 26;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"(c) 2004-2021, Lode Vandevenne, 2-Clause BSD");
			drawCenteredTextLine(posY);
			posY += 36;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Library freeGlut:");
			drawCenteredTextLine(posY,true);
			posY += 26;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"(c) 1999-2000 Pawel W. Olszta, X-Consortium license");
			drawCenteredTextLine(posY);
			posY += 36;
			
			#ifdef FREETEXTURES 

			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Textures Nr6,7 (Roof and grass):");
			drawCenteredTextLine(posY,true);
			posY += 26;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Screaming Brain Studios, CC0");
			drawCenteredTextLine(posY);
			posY += 36;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"All other textures:");
			drawCenteredTextLine(posY,true);
			posY += 26;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"codingABI, CC BY-SA 3.0");
			drawCenteredTextLine(posY);

			#else
			
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Textures Nr4,10,11,12,15,16,17,18,19,20,21,22,23,25,26 (historical textures):");
			drawCenteredTextLine(posY,true);
			posY += 26;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"(c) Bayerisches Hauptstaatsarchiv, CC BY-NC-SA 4.0");
			drawCenteredTextLine(posY);			
			posY += 36;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Textures Nr6,7 (Roof and grass):");
			drawCenteredTextLine(posY,true);
			posY += 26;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Screaming Brain Studios, CC0");
			drawCenteredTextLine(posY);
			posY += 36;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Textures Nr1,2,3,5,8,9,14,24:");
			drawCenteredTextLine(posY,true);
			posY += 26;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"codingABI, CC BY-SA 3.0");
			drawCenteredTextLine(posY);
						
			#endif
			posY += 36;
			snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"......");
			g_displayText[DISPLAYTEXTTIMEOUT-timeDelta]='\0';
			drawCenteredTextLine(posY);

			return;
		}
	}

	if ((g_state == STATE_FINISHED)) { // Finish screen

		int posY = g_viewPort3dHeight*g_pixelSize/2;

		snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Fin");
		drawCenteredTextLine(posY);


		posY += 26;

		snprintf(g_displayText,DISPLAYTEXTMAXLENGTH,"Solved in %d seconds", (g_gameEndTime - g_gameStartTime)/1000);
		drawCenteredTextLine(posY,true);

		return;
	}

    if (strncmp(g_displayText,"",DISPLAYTEXTMAXLENGTH)==0) { // no text to show
    	g_displayTextBlinking = false;
		return;
	}
	
	drawCenteredTextLine(g_viewPort3dHeight*g_pixelSize/2);
	
	if ((g_state == STATE_RUNNING) && (glutGet(GLUT_ELAPSED_TIME)-g_stateStartTime > DISPLAYTEXTTIMEOUT*1000)) { // autohide text in game state after a few seconds
		g_displayText[0]='\0';
    	g_displayTextBlinking = false;
	}	
}

// Go to running state
void changeStateToRunning() {
	if (g_state == STATE_QUIT) return; // not possible in quit program state
	g_state = STATE_RUNNING;
	g_stateStartTime = glutGet(GLUT_ELAPSED_TIME);
	g_gameStartTime = g_stateStartTime;
	snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Find the exit...");
   	g_displayTextBlinking=false;
}

// Go to finish state
void changeStateToFinished() {
	if (g_state == STATE_QUIT) return; // not possible in quit program state
	g_state = STATE_FINISHED;
	g_stateStartTime = glutGet(GLUT_ELAPSED_TIME);
	g_gameEndTime = g_stateStartTime;
	g_displayText[0]='\0';
   	g_displayTextBlinking=false;
}

// Go to start state
void changeStateToStart() {
	if (g_state == STATE_QUIT) return; // not possible in quit program state
	g_state = STATE_START;
	g_stateStartTime = glutGet(GLUT_ELAPSED_TIME);
	snprintf(g_displayText,DISPLAYTEXTMAXLENGTH+1,"Insert coins...");
   	g_displayTextBlinking=true;
   	g_gameStartTime = 0;
   	
   	// Reset sprites
   	for (int i=0;i<MAXSPRITES;i++) g_sprites[i].collected=false;

   	// Reset viewer
	g_viewerX = DEFAULTVIEWERX;
	g_viewerY = DEFAULTVIEWERY;
	g_viewerAngle = DEFAULTVIEWERANGLE;
	// Reset walls and floor
	for (int i=0;i<MAPHEIGHT;i++) {
		for (int j=0;j<MAPWIDTH;j++) {
			g_wallMap[i][j] = g_defaultWallMap[i][j];
			g_floorMap[i][j] = g_defaultFloorMap[i][j];
		}
	}
	// Reset input
	g_buttonUpPressed = false;
	g_buttonDownPressed = false;
	g_buttonLeftPressed = false;
	g_buttonRightPressed = false;
	g_joystickForward = false;
	g_joystickBackward = false;
	g_joystickLeft = false;
	g_joystickRight = false;
	g_mouseForward = false;
	g_mouseBackward = false;
	g_mouseLeft = false;
	g_mouseRight = false;
}

// Go to quit program state
void changeStateToQuit() {
	g_state = STATE_QUIT;
	g_stateStartTime = glutGet(GLUT_ELAPSED_TIME);
	g_displayText[0]='\0';
   	g_displayTextBlinking=false;
}

// Calculate viewport and offset dependent on real display- and pixelsize
void recalcDisplayProperties() {
	g_viewPort3dWidth = g_viewPort3dPhysicalWidth / g_pixelSize ;
	if (g_viewPort3dWidth > MAXWIDTH) g_viewPort3dWidth = MAXWIDTH;

	g_viewPort3dHeight = g_viewPort3dPhysicalHeight / g_pixelSize;
	if (g_viewPort3dHeight & 1) g_viewPort3dHeight--; // always odd to avoid glitches
    g_viewPort3dHalfHeight = g_viewPort3dHeight/2;

	g_pixelOffset = (g_pixelSize-1)/2;
	g_lineOffset = (g_pixelSize)/2;
	
	g_textureSkyGroundStepX = (float) g_pixelSize/SKYSCALE;
}

// Resize window
void resize(int w, int h) {
	g_windowWidth = w;
	g_windowHeight = h;

	if (g_windowHeight < MAPHEIGHT*GRIDSIZE) g_windowHeight = MAPHEIGHT*GRIDSIZE;	

	g_viewPort3dPhysicalHeight = g_windowHeight;

	if (g_fullScreenMode) {
		g_viewPort3dPhysicalWidth = g_windowWidth;
	} else {
		if (g_viewPort3dPhysicalHeight & 1) g_viewPort3dPhysicalHeight--; // odd height 
		g_viewPort3dPhysicalWidth = g_windowWidth - GRIDSIZE*MAPWIDTH;
		if (g_viewPort3dPhysicalWidth < MINVIEWPORT3DWIDTH) {
			g_viewPort3dPhysicalWidth = MINVIEWPORT3DWIDTH;
			g_windowWidth = g_viewPort3dPhysicalWidth + GRIDSIZE*MAPWIDTH;
		}
	}

	recalcDisplayProperties();
	
	preparePositionDataForDDA();
	
	glLoadIdentity();
	gluOrtho2D(-0.5,g_windowWidth-0.5,h-0.5,-0.5);  // Offset of 0.5 to show pixels on 0
	glViewport(0,0, g_windowWidth, g_windowHeight);
	if ((g_windowWidth != w) || (g_windowHeight != h)) glutReshapeWindow(g_windowWidth,g_windowHeight); // resize window if needed
}

// Common key pressed
void keyboard(unsigned char key,int x,int y)
{
	// start game on first key pressed
	if (g_state == STATE_START) changeStateToRunning();
	
	// Reset timeout in quit program state 
	if (g_state == STATE_QUIT) g_stateStartTime = glutGet(GLUT_ELAPSED_TIME);

    switch(key) {
    	// pixel size
    	case 's': // down
    		if (g_pixelSize>1) g_pixelSize--;
			recalcDisplayProperties();
    		break;
    	case 'S': // up
    		if (g_pixelSize < 16) g_pixelSize ++;
			recalcDisplayProperties();
    		break;
    	case '1': // textures for roof and floor on/off
    		g_showBackgroundTexture = !g_showBackgroundTexture;
    		break;
    	case '2': // toggle sky texture
    		g_showBackground = !g_showBackground;
    		break;
    	case '3': // DDA raycaster or my old raycaster
    		g_oldStyle = !g_oldStyle;
    		break;
    	case '4': // toggle round pixels
    		g_roundPixels = !g_roundPixels;
			recalcDisplayProperties();
    		break;
    	case '5': // toggle automatic pixel size function
    		g_autoPixelSize = !g_autoPixelSize;
    		break;
    	// toggle textures on/off
    	case 't':
    	case 'T':
    		g_showTextures = !g_showTextures;
    		break;
    	// toggle fullscreen on/off
    	case 'f':
    	case 'F': {
    		if (g_fullScreenMode) {
    			g_fullScreenMode = false;
    			glutPositionWindow(0,0);
				glutReshapeWindow(g_savedWindowWidth, g_savedWindowHeight);
				g_windowWidth = glutGet(GLUT_WINDOW_WIDTH);
				g_windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
				g_viewPort3dOffsetX = MAPWIDTH*GRIDSIZE;
			} else {
				g_fullScreenMode = true;
				g_savedWindowWidth = glutGet(GLUT_WINDOW_WIDTH);
				g_savedWindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
				g_viewPort3dOffsetX = 0;
				glutFullScreen();
			}
			break;
		}
		// exit
		case 'q':
		case 'Q':
		case 27 : {
			if (g_state!= STATE_QUIT) changeStateToQuit(); else g_stateStartTime-=(DISPLAYTEXTTIMEOUT*1000); // change to quit program state or expire timeout if already in quit program state
			break;
		}
    }
}

// Special key pressed
void specialButtonPressed(int key, int x, int y) {

	// Reset timeout in quit program state 
	if (g_state == STATE_QUIT) g_stateStartTime = glutGet(GLUT_ELAPSED_TIME);
	
	// start game on first keypress
	if (g_state == STATE_START) changeStateToRunning();

	switch (key) {
		case GLUT_KEY_UP: g_buttonUpPressed = true; break;
		case GLUT_KEY_DOWN: g_buttonDownPressed = true; break;
		case GLUT_KEY_LEFT: g_buttonLeftPressed = true; break;
		case GLUT_KEY_RIGHT: g_buttonRightPressed = true; break;
	}
}

// Special key released
void specialButtonReleased(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP: g_buttonUpPressed = false; break;
		case GLUT_KEY_DOWN: g_buttonDownPressed = false; break;
		case GLUT_KEY_LEFT: g_buttonLeftPressed = false; break;
		case GLUT_KEY_RIGHT: g_buttonRightPressed = false; break;
	}
}

// joystick
void joystick(unsigned int button, int x, int y, int z) {
	#define IGNORECENTERDELTA 15 // My theC64-joystick returns -7 when centered 
	g_joystickBackward = (y > IGNORECENTERDELTA);
	g_joystickForward = (y < -IGNORECENTERDELTA);
	g_joystickRight = (x > IGNORECENTERDELTA);
	g_joystickLeft = (x < -IGNORECENTERDELTA);

	// start game on first move
	if ((g_state == STATE_START) && (g_joystickRight || g_joystickLeft || g_joystickForward || g_joystickBackward || (button!=0))) changeStateToRunning();
}


// mouse wheel to move viewer	
void mouseWheel(int button, int dir, int x, int y) {

	g_mouseBackward = (dir < 0);
		
	// start game on first move
	if ((g_state == STATE_START) && (g_mouseBackward )) changeStateToRunning();

}


// mouse
void mouse(int button, int state, int x, int y) {
	g_mouseLeft = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN ));
	g_mouseRight = ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN ));
	g_mouseForward = ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN ));

	if ((g_state == STATE_START) && (g_mouseForward || g_mouseRight || g_mouseLeft )) changeStateToRunning();
}

// Check for currently pressed buttons, joystick and mouse
void checkInput()
{
	float newX;
	float newY;
	static GLint lastKeyCheckTime = 0;
			
	if (glutGet(GLUT_ELAPSED_TIME) - lastKeyCheckTime > 20) { // check only every 20ms to reduce speed

		if (g_buttonUpPressed || g_joystickForward || g_mouseForward) {
			newX = g_viewerX + cos(M_PI*g_viewerAngle/180) * STEPSIZE;
			newY = g_viewerY + sin(M_PI*g_viewerAngle/180) * STEPSIZE;

			if (!ISGRIDINMAP(newX,newY) || ISGRIDFILLED(newX,newY)) return;
			g_viewerX = newX;
			g_viewerY = newY;
		};
		if (g_buttonDownPressed || g_joystickBackward || g_mouseBackward) {
			newX = g_viewerX - cos(M_PI*g_viewerAngle/180) * STEPSIZE;
			newY = g_viewerY - sin(M_PI*g_viewerAngle/180) * STEPSIZE;

			if (!ISGRIDINMAP(newX,newY) || ISGRIDFILLED(newX,newY)) return;
			g_viewerX = newX;
			g_viewerY = newY;
			g_mouseBackward = false;
		};
		if (g_buttonLeftPressed || g_joystickLeft || g_mouseLeft){
			g_viewerAngle=(360+(int)g_viewerAngle-1)%360;
			preparePositionDataForDDA();
		}
		if (g_buttonRightPressed || g_joystickRight || g_mouseRight) {
			g_viewerAngle = (360+(int)g_viewerAngle+1)%360;
			preparePositionDataForDDA();
		}
		lastKeyCheckTime = glutGet(GLUT_ELAPSED_TIME);
	}
}

// Display loop
void display()
{   
	static GLint framesStartTime=0;
	static GLint lastAutorotateTime = 0;
	static int framesCounter = 0;
	#define MAXMESSAGELENGTH 80
	char strData[MAXMESSAGELENGTH];

	// Pixels round or quad
	if (g_roundPixels) glEnable( GL_POINT_SMOOTH ); else glDisable( GL_POINT_SMOOTH ); 
	
	// Autorotate in start state
	if ((g_state == STATE_START) && (glutGet(GLUT_ELAPSED_TIME) - lastAutorotateTime > 25)) { 
		g_viewerAngle += 0.1;
		if (g_viewerAngle > 360) g_viewerAngle-=360;
		preparePositionDataForDDA();
		lastAutorotateTime = glutGet(GLUT_ELAPSED_TIME);
	}

	// calculate fps
	framesCounter++;
 	if (glutGet(GLUT_ELAPSED_TIME)-framesStartTime > 1000) { // once per seconde		
 		g_fps = 1000*framesCounter/(glutGet(GLUT_ELAPSED_TIME)-framesStartTime);
 		framesStartTime = glutGet(GLUT_ELAPSED_TIME);
 		framesCounter = 0;
 		
 		snprintf(strData,MAXMESSAGELENGTH,"Falkenstein3D %dx%dx%d X %f Y %f A %.2f S %d",g_viewPort3dWidth,g_viewPort3dHeight,g_pixelSize,g_viewerX,g_viewerY,g_viewerAngle,g_state);
 		glutSetWindowTitle(strData);
				
		if (g_autoPixelSize && (g_fps!=0)) { //automatic pixel size dependent on framerate (prevents low fps)
			if ((g_fps < 30) && (g_pixelSize<16)) { 
				g_pixelSize++;
				recalcDisplayProperties();
			}
			if ((g_fps > 100) && (g_pixelSize>1)) {
				g_pixelSize--;
				recalcDisplayProperties();
			}		 	
		}
	}
	
	// Finish state timeout?
	if ((g_state == STATE_FINISHED) && (glutGet(GLUT_ELAPSED_TIME) - g_stateStartTime > DISPLAYTEXTFINISHDURATIONLENGTH*1000)) changeStateToStart();	
	// Finish reached?
	if (g_state != STATE_FINISHED && ((int) g_viewerX == FINISHX) && ((int) g_viewerY == FINISHY)) changeStateToFinished();
	
	// clear buffer and redraw
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
 	if (!g_fullScreenMode) drawMap();

	drawBackground();

 	if (!g_oldStyle) drawRaycastDDA(); else drawRaycast();
	drawSprites();
	if (!g_fullScreenMode) drawViewer();
	drawInfos();		

	drawMessage();
	
	if (g_fullScreenMode) glutSetCursor(GLUT_CURSOR_NONE); else glutSetCursor(GLUT_CURSOR_INHERIT);

 	glutSwapBuffers();  

	checkInput();
}

// Parse program arguments
int args(int argc, char **argv)
{
    GLint i;

    for (i = 1; i < argc; i++) {
    	// currently not used
	}	
    return 0;
}

// main
int main(int argc, char* argv[])
{ 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // double buffer and rgb mode
	
	// Initial window size
	g_windowHeight = glutGet(GLUT_SCREEN_HEIGHT)*0.8f;
	g_windowWidth = glutGet(GLUT_SCREEN_WIDTH)*0.9f;
	g_savedWindowHeight = g_windowHeight;
	g_savedWindowWidth = g_windowWidth;
	
	g_viewPort3dPhysicalWidth = g_windowWidth - GRIDSIZE*MAPWIDTH;
	g_viewPort3dPhysicalHeight = g_windowHeight;

	if (g_fullScreenMode) g_viewPort3dOffsetX = 0; else g_viewPort3dOffsetX = MAPWIDTH*GRIDSIZE;	
	recalcDisplayProperties();
	
	preparePositionDataForDDA();
	
	glutInitWindowSize(g_windowWidth,g_windowHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Falkenstein3D");

	if (args(argc, argv) != 0) exit(1);

	if (g_fullScreenMode) glutFullScreen();
	
	gluOrtho2D(-0.5,g_windowWidth-0.5,g_windowHeight-0.5,-0.5); // Offset of 0.5 to show pixels on 0
	
	glClearColor(0.1,0.1,0.1,0); // Default background color
	glutIdleFunc(glutPostRedisplay);
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutSpecialUpFunc(specialButtonReleased);
	glutSpecialFunc(specialButtonPressed);
	glutKeyboardFunc(keyboard);
	glutJoystickFunc(joystick, 10); 
	glutMouseFunc(mouse);
	glutMouseWheelFunc(mouseWheel);
	
	changeStateToStart();
	
	glutMainLoop();
}
