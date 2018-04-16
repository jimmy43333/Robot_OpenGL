//
//  CallBack.hpp
//  Robot_OpenGL
//
//  Created by TG-SUNG on 2018/4/11.
//  Copyright © 2018年 宋題均. All rights reserved.
//

#ifndef CallBack_hpp
#define CallBack_hpp
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#include <iostream>
#include <stdio.h>

#define MENU_RUN 1
#define MENU_TIMER_STOP 2
#define MENU_EXIT 3
#define MENU_SPIN 4
#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))
using namespace std;
using namespace glm;

extern mat4 projection;
extern mat4 view;
extern float rotateangle[12];
extern int flag;

void My_Reshape(int width, int height);
void My_Timer(int val);
void My_Menu(int id);
void My_Mouse(int button, int state, int x, int y);
void My_Keyboard(unsigned char key, int x, int y);
void My_SpecialKeys(int key, int x, int y);

#endif /* CallBack_hpp */
