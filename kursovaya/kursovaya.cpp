#include <Windows.h>
#include <iostream>
#include <locale>
#include <glfw3.h>
#include <glut.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include <vector>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
GLFWwindow* win = nullptr;
GLUquadricObj* quad = gluNewQuadric();
int width = 1280, height = 720; //Размеры окна
bool keys[1024] = { false }, jump = false; //Нажатия кнопок
double walkPosX = 0, walkPosY = 0, heightJump = 0, speed = 0.1; //Движение камеры
double lastPosX = width / 2, lastPosY = height / 2, offsetPosX = 0, offsetPosY = 0, sensitivity = 0.1; //Движение мыши
unsigned int  tFloor, tWall, tRoof, tRoll, tDoor, tMetal, tMetal_2, tMetalRed, tWood, tWoodDark; //Текстуры




void mouse_callback(GLFWwindow* win, double xpos, double ypos){
	if (offsetPosY*sensitivity >= 90 || (offsetPosY*sensitivity <= -90)) {
		glfwSetCursorPos(win, xpos, offsetPosY + lastPosY);
	}

	offsetPosX = xpos - lastPosX;
	offsetPosY = ypos - lastPosY;
	
	if (offsetPosY*sensitivity >= 90)
		offsetPosY = 90 / sensitivity;
	else
		if (offsetPosY*sensitivity <= -90)
			offsetPosY = -90 / sensitivity;
}


void key_callback(GLFWwindow* win, int key, int scancode, int action, int mode) {

	if (action == GLFW_PRESS)
		keys[key] = true;
	else
		if (action == GLFW_RELEASE)
			keys[key] = false;
}




void SetColor(float red, float green, float blue, float alpha) {
	glColor4f(red, green, blue, alpha);
}



float coord_texture[] = { 0,1, 1,1, 1,0, 0,0, 
				0,1, 0,0, 1,0, 1,1, 
				0,0, 0,1, 1,1, 1,0,  
				0,0, 0,1, 1,1, 1,0, 
				0,1, 0,0, 1,0, 1,1,
				0,1, 1,1, 1,0, 0,0, };
float vertices[] = { 1,1,2, -1,1,2, -1,-1,2, 1,-1,2,
				 1,1,2, -1,1,2, -1,1,4, 1, 1,4,
				-1,-1,2, 1,-1,2, 1,-1,4, -1,-1,4,
				-1,-1,2, -1,1,2, -1,1,4, -1, -1,4,
				1,1,2, 1,-1,2, 1,-1,4, 1,1,4,
				1,1,4, -1,1,4, -1,-1,4, 1,-1,4, };

void renderCube() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, coord_texture);
	glDrawArrays(GL_QUADS, 0, 24);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


float ambient[] = { 0.5, 0.5, 0.4, 1 };
GLfloat light_position[] = { 0, 5, 5, 0 };
void drawLight() {
	glEnable(GL_LIGHT5);
	glLightfv(GL_LIGHT5, GL_POSITION, light_position);
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambient);
	SetColor(1, 1, 1, 1);
}





void renderHouse() {
	/*Пол*/
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tFloor);
	glScalef(5, 0.3, 5);
	glTranslatef(0, -1, -2.7);
	renderCube();
	glPopMatrix();

	/*Стены*/
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, tWall); 
	glTranslatef(-3, 4, -4.4);
	glScalef(2, 4, 0.3);
	renderCube();
	glTranslatef(3, 0, 0);
	renderCube();
	glBindTexture(GL_TEXTURE_2D, tRoll);
	glTranslatef(-1.5, 0, 0.3);
	glScalef(0.525, 0.5, 0.9);
	renderCube();
	glBindTexture(GL_TEXTURE_2D, tWall);
	glTranslatef(0, 1.5, 0);
	glScalef(1, 0.5, 1);
	renderCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 4, 5.5);
	glScalef(5, 4, 0.3);
	renderCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5, 4, -13.5);
	glScalef(0.3, 4, 5);
	renderCube();
	glTranslatef(34, 0, 0);
	renderCube();
	glPopMatrix();

	/*Крыша*/
	glBindTexture(GL_TEXTURE_2D, tRoof);
	glPushMatrix();
	glTranslatef(0, 8, 1.5);
	glRotatef(90, -1, 0, 0);
	glRotatef(45, 0, 0, 1);
	gluCylinder(quad, 7, 0, 2, 4, 4);
	glPopMatrix();
}

void renderBarrel() {
	static float rotate = 0, tr = 0;
	static bool f = false;

	if (!f) {
		rotate += 5;
		tr -= 0.05;
		if (tr <= -6.5)
			f = true;
	}
	else {
		rotate -= 5;
		tr += 0.05;
		if (tr >= 2.65)
			f = false;
	}

	glBindTexture(GL_TEXTURE_2D, tMetalRed);
	glPushMatrix();
	glTranslatef(2, 0.2, 0);
	glTranslatef(tr, 0, 0);
	glRotatef(90, 1, 0, 0);
	glRotatef(rotate, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0, 2, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(quad, 0.2, 0.2, 1, 20, 20);
	gluSphere(quad, 0.2, 20, 20);
	glTranslatef(0, 0, 1);
	glScalef(1, 1, 0.2);
	gluSphere(quad, 0.2, 20, 20);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, tMetal_2);
	glPushMatrix();
	glTranslatef(0, 2.4, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(quad, 0.05, 0.05, 0.5, 20, 20);
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(0, 2.4, 0);
	gluSphere(quad, 0.05, 20, 20);
	glPopMatrix();
	glPopMatrix();
}


void renderBox() {
	static float rotate = 0;
	static bool f = false;


	if (!f) {
		rotate += 1;
		if (rotate >= 90)
			f = true;
	}
	else {
		rotate -= 1;
		if (rotate <= 0)
			f = false;
	}



	glBindTexture(GL_TEXTURE_2D, tMetal);
	glPushMatrix();
	glTranslatef(0, 1, 3);
	glPushMatrix();
	glTranslatef(0, -0.9, 0.74);
	glScalef(0.7, 0.1, 0.42);
	renderCube();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, tWood);
	glPushMatrix();
	glTranslatef(0, 0, 2);
	glScalef(1, 1, 0.6);
	glRotatef(90, 1, 0, 0);
	glRotatef(45, 0, 0, 1);
	gluCylinder(quad, 1, 1, 1, 4, 4);
	glPopMatrix();


	//Крышка
	glBindTexture(GL_TEXTURE_2D, tWoodDark);
	glPushMatrix();
	glTranslatef(0, 0, 2.5);
	glRotatef(rotate, 1, 0, 0);
	glTranslatef(0, -0.85, -2.5);
	glPushMatrix();
	glTranslatef(0, 1, 0.74);
	glScalef(0.7, 0.05, 0.42);
	renderCube();
	glPopMatrix();


	glBindTexture(GL_TEXTURE_2D, 0);
	SetColor(0.5, 0.5, 0.5, 1);
	glMaterialf(GL_FRONT, GL_EMISSION, 0);
	glPushMatrix();
	glTranslatef(0, 1, 2);
	glScalef(1, 1, 0.6);
	glRotatef(90, 1, 0, 0);
	glRotatef(45, 0, 0, 1);
	gluCylinder(quad, 1, 1, 0.2, 4, 4);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
}

void renderDoor() {

	glBindTexture(GL_TEXTURE_2D, tDoor);
	glPushMatrix();
	glTranslatef(-6.4, 5.5, 0.8);
	glScalef(1, 0.9, 1);
	glRotatef(180, 0, 1, 0);
	glPushMatrix();
	glTranslatef(-5.5, -3, 0);
	glScalef(0.1, 3, 1);
	renderCube();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, tMetal);
	glPushMatrix();
	glTranslatef(-5.4, -3, 2.3);
	glScalef(0.1, 1, 1);
	gluSphere(quad, 0.11, 20, 20);

	glTranslatef(-2, 0, 0);
	gluSphere(quad, 0.11, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.4, -3, 2.3);
	glRotatef(90, 0, 1, 0);
	gluCylinder(quad, 0.04, 0.04, 0.1, 20, 20);

	glTranslatef(0, 0, -0.3);
	gluCylinder(quad, 0.04, 0.04, 0.1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.3, -3, 2);
	glScalef(0.01, 0.05, 0.13);
	renderCube();

	glTranslatef(-40, 0, 0);
	renderCube();
	glPopMatrix();
	glPopMatrix();
}


void scene3d() {
	drawLight();
	renderHouse();
	renderBarrel();
	renderBox();
	renderDoor();
}

void camera() {

	//Закрытие программы на клавишу ESC
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(win, GL_TRUE);



	//Прыжок
	if (keys[GLFW_KEY_SPACE] && !jump && heightJump == 0) {
		jump = true;
		keys[GLFW_KEY_SPACE] = false;
	}

	if (jump) {
		heightJump += 0.15f;
		if (heightJump > 3)
			jump = false;	
	}
	else {
		if(heightJump > 0)
			heightJump -= 0.15f;
	}


	//Движение 
	if (keys[GLFW_KEY_W]) {
		walkPosX += sin(offsetPosX * sensitivity / 180 * M_PI) * speed;
		walkPosY += cos(offsetPosX * sensitivity / 180 * M_PI) * speed;
	}

	if (keys[GLFW_KEY_S]) {
		walkPosX -= sin(offsetPosX * sensitivity / 180 * M_PI) * speed;
		walkPosY -= cos(offsetPosX * sensitivity / 180 * M_PI) * speed;
	}

	if (keys[GLFW_KEY_A]) {
		walkPosX += sin((offsetPosX * sensitivity / 180 * M_PI) - M_PI * 0.5) * speed;
		walkPosY += cos((offsetPosX * sensitivity / 180 * M_PI) - M_PI * 0.5) * speed;
	}

	if (keys[GLFW_KEY_D]) {
		walkPosX -= sin((offsetPosX * sensitivity / 180 * M_PI) - M_PI * 0.5) * speed;
		walkPosY -= cos((offsetPosX * sensitivity / 180 * M_PI) - M_PI * 0.5) * speed;
	}

	
	//Поворот
	glRotatef(offsetPosY* sensitivity, 1, 0, 0);
	glRotatef(offsetPosX * sensitivity, 0, -1, 0);

	//Передвижение
	glTranslatef(walkPosX, -3 - heightJump, walkPosY);
}


void load_texture(const char* namefile, unsigned int texture) {
	int widht, height, format;
	unsigned char* d;
	d = stbi_load(namefile, &widht, &height, &format, 0);
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format == 4 ? GL_RGBA : GL_RGB, widht, height, 0, format == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, d);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_TEXTURE_2D);
	stbi_image_free(d);
}

void init_texture() {
	glGenTextures(1, &tFloor);
	load_texture("floor.jpg", tFloor);

	glGenTextures(1, &tWall);
	load_texture("wall.jpg", tWall);

	glGenTextures(1, &tRoof);
	load_texture("roof.jpg", tRoof);

	glGenTextures(1, &tRoll);
	load_texture("roll.jpg", tRoll);

	glGenTextures(1, &tDoor);
	load_texture("door.jpg", tDoor);

	glGenTextures(1, &tMetal);
	load_texture("metal.jpg", tMetal);

	glGenTextures(1, &tMetal_2);
	load_texture("metal_2.jpg", tMetal_2);

	glGenTextures(1, &tMetalRed);
	load_texture("red_metal.jpg", tMetalRed);

	glGenTextures(1, &tWood);
	load_texture("wood.jpg", tWood);

	glGenTextures(1, &tWoodDark);
	load_texture("wood_dark.jpg", tWoodDark);
}

int main(void) {
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");
	if (!glfwInit()) {
		cout << "Ошибка инициализации GLFW" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


	win = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
	if (win == NULL) {
		cout << "Ошибка создания окна" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(win); 
	glfwSetKeyCallback(win, key_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(win, width / 2, height / 2);


	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);


	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(1, -1, -1, 1, 1, 90);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	init_texture();
	gluQuadricOrientation(quad, GLU_OUTSIDE);
	gluQuadricNormals(quad, GL_SMOOTH);
	gluQuadricDrawStyle(quad, GLU_FILL);
	gluQuadricTexture(quad, GL_TRUE);


	while (!glfwWindowShouldClose(win))
	{
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		camera();
		scene3d();
		glPopMatrix();
		glfwSwapBuffers(win);
		glfwPollEvents();
		Sleep(5);
	}
}