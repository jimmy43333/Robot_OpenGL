//
//  CallBack.cpp
//  Robot_OpenGL
//
//  Created by TG-SUNG on 2018/4/11.
//  Copyright © 2018年 宋題均. All rights reserved.
//

#include "CallBack.hpp"
void My_Reshape(int width, int height){
    glViewport(0, 0, width, height);
    float viewportAspect = (float)width / (float)height;
    projection = perspective(deg2rad(100.0f), viewportAspect, 0.1f, -50.0f);
    view = lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f,0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}


void My_Timer(int val)
{
    glutPostRedisplay();
    glutTimerFunc(16, My_Timer, val);
}

void My_Menu(int id)
{
    if(id == MENU_RUN){
        flag = 1;
        glutTimerFunc(16, My_Timer, 0);
    }
    if(id == MENU_SPIN){
        flag = 2;
        glutTimerFunc(16, My_Timer, 0);
    }
    if(id == MENU_TIMER_STOP){
        flag = false;
        for(int i =0;i<12;i++){
            rotateangle[i]=0;
        }
        rotateangle[0]=45;
    }
    if(MENU_EXIT){
        exit;
    }
}

void My_Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            //view = lookAt(vec3(x/30, y/30, 5.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
        }
        else if (state == GLUT_UP)
        {
            printf("Mouse %d is released at (%d, %d)\n", button, x, y);
        }
    }
}

void My_Keyboard(unsigned char key, int x, int y)
{
    if (key == 'a')
    {
        if(-90 < rotateangle[1] && rotateangle[1] <=100){
            rotateangle[1] = rotateangle[1] -10;
        }
    }
    if (key == 'd')
    {
        if(-100 <= rotateangle[1] && rotateangle[1] <90){
            rotateangle[1] = rotateangle[1] +10;
        }
    }
    if (key=='w'){
        if(rotateangle[8] < 180){
            rotateangle[8] = rotateangle[8] +6;
            if(rotateangle[8] < -45){
                rotateangle[10] = rotateangle[10] +3;
            }else if(rotateangle[8]<0){
                if(rotateangle[10] > 0){
                    rotateangle[10] = rotateangle[10] -3;
                }
            }else if(rotateangle[8] <45){
                rotateangle[10] = rotateangle[10] +3;
            }else if(rotateangle[8] <=90){
                if(rotateangle[10] > 0){
                    rotateangle[10] = rotateangle[10] -3;
                }
            }else if(rotateangle[8] <=135){
                rotateangle[10] = rotateangle[10] +3;
            }else{
                if(rotateangle[10] > 0){
                    rotateangle[10] = rotateangle[10] -3;
                }
            }
        }
    }
    if (key=='s'){
        if(-90 < rotateangle[8]){
            rotateangle[8] = rotateangle[8] -6;
        if(rotateangle[8] < -45){
                if(rotateangle[10] > 0){
                    rotateangle[10] = rotateangle[10] - 3;
                }
            }else if(rotateangle[8]<0){
                rotateangle[10] = rotateangle[10] + 3;
            }else if(rotateangle[8] <45){
                if(rotateangle[10] > 0){
                    rotateangle[10] = rotateangle[10] - 3;
                }
            }else if(rotateangle[8] <=90){
                rotateangle[10] = rotateangle[10] + 3;
            }else if(rotateangle[8] <=135){
                if(rotateangle[10] > 0){
                    rotateangle[10] = rotateangle[10] - 3;
                }
            }else{
                if(rotateangle[10] < 90){
                    rotateangle[10] = rotateangle[10] + 3;
                }
            }
        }
    }
    if (key=='W'){
        if(rotateangle[9] < 180){
            rotateangle[9] = rotateangle[9] +6;
            if(rotateangle[9] < -45){
                rotateangle[11] = rotateangle[11] +3;
            }else if(rotateangle[9]<0){
                if(rotateangle[11] > 0){
                    rotateangle[11] = rotateangle[11] -3;
                }
            }else if(rotateangle[9] <45){
                rotateangle[11] = rotateangle[11] +3;
            }else if(rotateangle[9] <=90){
                if(rotateangle[11] > 0){
                    rotateangle[11] = rotateangle[11] -3;
                }
            }else if(rotateangle[9] <=135){
                rotateangle[11] = rotateangle[11] +3;
            }else{
                if(rotateangle[11] > 0){
                    rotateangle[11] = rotateangle[11] -3;
                }
            }
        }
    }
    if (key=='S'){
        if(-90 < rotateangle[9]){
            rotateangle[9] = rotateangle[9] -6;
            if(rotateangle[9] < -45){
                if(rotateangle[11] > 0){
                    rotateangle[11] = rotateangle[11] - 3;
                }
            }else if(rotateangle[9]<0){
                rotateangle[11] = rotateangle[11] + 3;
            }else if(rotateangle[9] <45){
                if(rotateangle[11] > 0){
                    rotateangle[11] = rotateangle[11] - 3;
                }
            }else if(rotateangle[9] <=90){
                rotateangle[11] = rotateangle[11] + 3;
            }else if(rotateangle[9] <=135){
                if(rotateangle[11] > 0){
                    rotateangle[11] = rotateangle[11] - 3;
                }
            }else{
                if(rotateangle[11] < 90){
                    rotateangle[11] = rotateangle[11] + 3;
                }
            }
        }
    }
}

void My_SpecialKeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            view = rotate(view,deg2rad(10),vec3(1.0,0.0,0.0));
            break;
        case GLUT_KEY_DOWN:
            view = rotate(view,-deg2rad(10),vec3(1.0,0.0,0.0));
            break;
        case GLUT_KEY_LEFT:
            view = rotate(view,deg2rad(10),vec3(0.0,1.0,0.0));
            break;
        case GLUT_KEY_RIGHT:
            view = rotate(view,-deg2rad(10),vec3(0.0,1.0,0.0));
            break;
    }
}

