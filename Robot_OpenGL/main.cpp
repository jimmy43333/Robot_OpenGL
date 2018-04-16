//
//  main.cpp
//  Robot_OpenGL
//
//  Created by TG-SUNG on 2018/4/11.
//  Copyright © 2018年 宋題均. All rights reserved.
//

#include <OpenGL/gl3.h>
#include <GLUT/glut.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include "CallBack.hpp"

using namespace std;
using namespace glm;

mat4 view;
mat4 projection;
mat4 Identy_Init(1.0);
GLint um4p;
GLint um4m;
GLint um4v;
GLint unotex;

GLuint program;
GLuint vao;
GLuint dbuffer;
GLuint ibuffer;
GLuint texture[2];

//Set up the robot
mat4 hmodel;
mat4 bmodel[5];
mat4 lmodel[4];
mat4 amodel[4];
mat4 models; //scale and output the model matrix to uniform 
mat4 RL;
vec3 move;
float rotateangle[12];
int flag = 0;

char** loadShaderSource(const char* file)
{
    FILE* fp = fopen(file, "rb");
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *src = new char[sz + 1];
    fread(src, sizeof(char), sz, fp);
    src[sz] = '\0';
    char **srcp = new char*[1];
    srcp[0] = src;
    return srcp;
}

void freeShaderSource(char** srcp)
{
    delete srcp[0];
    delete srcp;
}

static const GLfloat vertex_positions[] =
{
    0.5,0.5,0.5,
    0.5,0.5,-0.5,
    0.5,-0.5,-0.5,
    0.5,-0.5,0.5,
    -0.5,0.5,0.5,
    -0.5,0.5,-0.5,
    -0.5,-0.5,-0.5,
    -0.5,-0.5,0.5,
};

static const GLushort indices[] =
{
    0,1,5,0,5,4,
    0,1,2,0,2,3,
    4,5,6,4,6,7,
    7,6,2,7,2,3,
    4,7,0,0,7,3,
    1,5,6,1,6,2
};

#define B 0x33, 0x55, 0x00, 0x00
#define W 0x66, 0xBB, 0xBB, 0xFF
#define R 0x66, 0x11, 0x22, 0x33
#define G 0x11, 0x00, 0xAA, 0x33
static const GLubyte tex_data[] =
{
    B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B,
    B, W, B, W, B, W, B, W,
    W, B, W, B, W, B, W, B,
};

static const GLubyte tex2_data[] =
{
    R, R, R, R, R, R, R, R,
    R, G, G, G, G, G, G, R,
    R, G, R, R, R, R, G, R,
    R, G, R, G, G, R, G, R,
    R, G, R, G, G, R, G, R,
    R, G, R, R, R, R, G, R,
    R, G, G, G, G, G, G, R,
    R, R, R, R, R, R, R, R,
};
#undef B
#undef W
#undef R
#undef G

void Run(){
    float currentTime = glutGet(GLUT_ELAPSED_TIME) * 0.1f;
    rotateangle[1] = 10*cos(deg2rad(currentTime));
    rotateangle[3] = 10*cos(deg2rad(currentTime));
    rotateangle[4] = 60*sin(deg2rad(currentTime));
    rotateangle[5] = 60*-sin(deg2rad(currentTime));
    rotateangle[6] = 40*-abs(cos(deg2rad(currentTime)-deg2rad(180)));
    rotateangle[7] = 40*-abs(cos(deg2rad(currentTime)-deg2rad(180)));
    rotateangle[8] = 60*-sin(deg2rad(currentTime));
    rotateangle[9] = 60*sin(deg2rad(currentTime));
    rotateangle[10] = 40*abs(cos(deg2rad(currentTime)-deg2rad(180)));
    rotateangle[11] = 40*abs(cos(deg2rad(currentTime)-deg2rad(180)));
}

void Spin(){
    float currentTime = glutGet(GLUT_ELAPSED_TIME) * 0.1f;
    rotateangle[1] = deg2rad(currentTime*20);
    rotateangle[3] = -1 * deg2rad(currentTime*20);
    rotateangle[8] = deg2rad(currentTime*20);
    rotateangle[9] = -1* deg2rad(currentTime*20);
}


void My_Init(){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //Build the program
    program = glCreateProgram();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    char** vertexShaderSource = loadShaderSource("vertex_shader.glsl");
    char** fragmentShaderSource = loadShaderSource("fragment_shader.glsl");
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
    freeShaderSource(vertexShaderSource);
    freeShaderSource(fragmentShaderSource);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //Bind and input vertex to location 0 in vertex_shader
    glGenBuffers(1,&dbuffer);
    glBindBuffer(GL_ARRAY_BUFFER,dbuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_positions),vertex_positions,GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    //Bind the input texture to location 1 in vertex_shader
    glGenBuffers(1,&dbuffer);
    glBindBuffer(GL_ARRAY_BUFFER,dbuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertex_positions),vertex_positions,GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    //Bind the index buffer to draw the vertex
    glGenBuffers(1,&ibuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    um4m = glGetUniformLocation(program, "m_matrix");
    um4m = glGetUniformLocation(program, "v_matrix");
    um4p = glGetUniformLocation(program, "proj_matrix");
    unotex = glGetUniformLocation(program, "notex");
    glUseProgram(program);
    
    //Input the texture data to buffer
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, 8, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glGenTextures(1, &texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 8, 8, GL_RGBA, GL_UNSIGNED_BYTE, tex2_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    for(int i =0;i<12;i++){
        rotateangle[i]=0;
    }
    rotateangle[0]=45;
}

void My_Display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
    static const GLfloat one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, green);
    glClearBufferfv(GL_DEPTH, 0, &one);
    
    glUseProgram(program);
    glUniformMatrix4fv(um4v, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(um4p, 1, GL_FALSE, &projection[0][0]);
    
    RL = translate(Identy_Init,vec3(0.0f,3.0f,0.0f));
    RL = rotate(RL,deg2rad(rotateangle[0]),vec3(0.0f, 1.0f, 0.0f));
    //RL = translate(Identy_Init,vec3(0.0f,0.0f,0.0f));
    //RL = scale(RL,vec3(3.0f, 3.0f, 3.0f));
    if(flag==1){
        Run();
        flag = 1;
    }
    if(flag ==2){
        Spin();
        flag = 2;
    }
    //Draw the body
    for(int i =0;i< 5;i++){
        switch(i){
            case 0:
                bmodel[i] = translate(RL,vec3(0,1.0f,0));
                models = scale(bmodel[i],vec3(2.0f,1.0f,2.0f));
                glBindTexture(GL_TEXTURE_2D, texture[1]);
                break;
            case 1:
                bmodel[i] = rotate(RL,deg2rad(rotateangle[1]),vec3(0.0f, 1.0f, 0.0f));
                bmodel[i] = rotate(bmodel[i],deg2rad(rotateangle[2]),vec3(1.0f, 0.0f, 0.0f));
                bmodel[i] = translate(bmodel[i],vec3(0,0,0));
                models = scale(bmodel[i],vec3(5.0f,1.0f,1.0f));
                glUniform1i(unotex, 1);
                break;
            case 2:
                bmodel[i] = rotate(RL,deg2rad(rotateangle[1]),vec3(0.0f, 1.0f, 0.0f));
                bmodel[i] = translate(bmodel[i],vec3(0,0,0));
                models = scale(bmodel[i],vec3(1.0f,1.0f,5.0f));
                glUniform1i(unotex, 1);
                break;
            case 3:
                bmodel[i] = translate(RL,vec3(0,-1.5f,0));
                models = scale(bmodel[i],vec3(1.0,2.0f,2.0));
                glBindTexture(GL_TEXTURE_2D, texture[0]);
                break;
            case 4:
                bmodel[i] = rotate(RL,deg2rad(rotateangle[3]),vec3(0.0f, 1.0f, 0.0f));
                bmodel[i] = translate(bmodel[i],vec3(0,-3.0f,0));
                models = scale(bmodel[i],vec3(2.0f,1.0f,2.0f));
                glBindTexture(GL_TEXTURE_2D, texture[1]);
                break;
        }
        glUniformMatrix4fv(um4m, 1, GL_FALSE, value_ptr(models));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
        glUniform1i(unotex, 0);
    }
    //Draw the head
    hmodel = translate(bmodel[0],vec3(0,1.0f,0));
    glUniformMatrix4fv(um4m, 1, GL_FALSE, &hmodel[0][0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    //Draw the lag
    for(int i =0;i<4;i++){
        switch(i){
            case 0:
                lmodel[i] = translate(bmodel[4],vec3(-1.5f,0.0f,0));
                lmodel[i] = rotate(lmodel[i],deg2rad(rotateangle[4]),vec3(-1.0f,0.0f,0.0f));
                lmodel[i] = translate(lmodel[i],vec3(0.0f,-1.0f,0.0f));
                models = scale(lmodel[i],vec3(1.0f,3.0f,1.0f));
                glUniform1i(unotex, 1);
                break;
            case 1:
                lmodel[i] = translate(bmodel[4],vec3(1.5f,0.0f,0));
                lmodel[i] = rotate(lmodel[i],deg2rad(rotateangle[5]),vec3(-1.0f,0.0f,0.0f));
                lmodel[i] = translate(lmodel[i],vec3(0.0f,-1.0f,0.0f));
                models = scale(lmodel[i],vec3(1.0f,3.0f,1.0f));
                glUniform1i(unotex, 1);
                break;
            case 2:
                lmodel[i] = translate(lmodel[0],vec3(0.0f,-2.0f,0.0f));
                lmodel[i] = rotate(lmodel[i],deg2rad(rotateangle[6]),vec3(-1.0f,0.0f,0.0f));
                lmodel[i] = translate(lmodel[i],vec3(0.0f,-1.0f,0.0f));
                models = scale(lmodel[i],vec3(1.0f,3.0f,1.0f));
                glBindTexture(GL_TEXTURE_2D, texture[1]);
                break;
            case 3:
                lmodel[i] = translate(lmodel[1],vec3(0.0f,-2.0f,0.0f));
                lmodel[i] = rotate(lmodel[i],deg2rad(rotateangle[7]),vec3(-1.0f,0.0f,0.0f));
                lmodel[i] = translate(lmodel[i],vec3(0.0f,-1.0f,0.0f));
                models = scale(lmodel[i],vec3(1.0f,3.0f,1.0f));
                glBindTexture(GL_TEXTURE_2D, texture[1]);
                break;
        }
        glUniformMatrix4fv(um4m, 1, GL_FALSE, value_ptr(models));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
        glUniform1i(unotex, 0);
    }
    //Draw the arm
    for(int i =0;i<4;i++){
        switch(i){
            case 0:
                amodel[i] = translate(bmodel[1],vec3(-3.0f,0.0f,0));
                amodel[i] = rotate(amodel[i],deg2rad(rotateangle[8]),vec3(-1.0f,0.0f,0.0f));
                models = scale(amodel[i],vec3(1.0f,3.0f,2.0f));
                glBindTexture(GL_TEXTURE_2D, texture[0]);
                break;
            case 1:
                amodel[i] = translate(bmodel[1],vec3(3.0f,0.0f,0));
                amodel[i] = rotate(amodel[i],deg2rad(rotateangle[9]),vec3(-1.0f,0.0f,0.0f));
                models = scale(amodel[i],vec3(1.0f,3.0f,2.0f));
                glBindTexture(GL_TEXTURE_2D, texture[0]);
                break;
            case 2:
                amodel[i] = translate(amodel[0],vec3(0.0f,-2.0f,0.0f));
                amodel[i] = rotate(amodel[i],deg2rad(rotateangle[10]),vec3(-1.0f,0.0f,0.0f));
                amodel[i] = translate(amodel[i],vec3(0.0f,-1.0f,0.0f));
                models = scale(amodel[i],vec3(1.0f,3.0f,1.0f));
                glBindTexture(GL_TEXTURE_2D, texture[1]);
                break;
            case 3:
                amodel[i] = translate(amodel[1],vec3(0.0f,-2.0f,0.0f));
                amodel[i] = rotate(amodel[i],deg2rad(rotateangle[11]),vec3(-1.0f,0.0f,0.0f));
                amodel[i] = translate(amodel[i],vec3(0.0f,-1.0f,0.0f));
                models = scale(amodel[i],vec3(1.0f,3.0f,1.0f));
                glBindTexture(GL_TEXTURE_2D, texture[1]);
                break;
        }
        glUniformMatrix4fv(um4m, 1, GL_FALSE, value_ptr(models));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    }
    glutSwapBuffers();
}

int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1000, 1000);
    glutCreateWindow("Robot");
    My_Init();
    int menu_main = glutCreateMenu(My_Menu);
    glutSetMenu(menu_main);
    glutAddMenuEntry("Run", MENU_RUN);
    glutAddMenuEntry("Spin", MENU_SPIN);
    glutAddMenuEntry("Stop", MENU_TIMER_STOP);
    glutAddMenuEntry("Exit", MENU_EXIT);
    glutSetMenu(menu_main);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutDisplayFunc(My_Display);
    glutReshapeFunc(My_Reshape);
    glutMouseFunc(My_Mouse);
    glutKeyboardFunc(My_Keyboard);
    glutSpecialFunc(My_SpecialKeys);
    glutTimerFunc(16,My_Timer,0);
    glutMainLoop();
    return 0;
}
