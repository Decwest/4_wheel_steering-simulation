///////////////////////////////////////////////////////////////////
//                                                               //
//         Sample Program of 2-D Graphics  sd_2d_v1.cpp          //
//                 Original 2013/Oct/16 Nishi                    //
//                 Modified 2013/Oct/20 Nakazawa                 //
//                 Modified 2015/Dec/10 Takahashi                //
///////////////////////////////////////////////////////////////////

//joycon対応してるけど重くなるから使わないなら切ったほうがいい。j押したらjoycon対応する

#include <math.h>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <string.h>
#include <sstream>
//#include <GL/glut.h>
#include <fstream>
#include <iomanip>
#include "sdglib.h"

using namespace std;

char str[] = " - 2D Graphics (4_wheel_steering simulation) - ";

double vx = 0, vy = 0, w = 0, delta = 15 * M_PI / 180, a = 30, d = 150, theta_1 = 0, theta_2 = 0, theta_3 = 0, theta_4 = 0, theta = 0, timer = 0;
bool simuflag = false, joyconflag = false;

//初期ステアリング位置
static double x_1 = d, y_1 = 0, x_2 = 0, y_2 = d, x_3 = -d, y_3 = 0, x_4 = 0, y_4 = -d;

double vx_1 = 0, vy_1 = 0, vx_2 = 0, vy_2 = 0, vx_3 = 0, vy_3 = 0, vx_4 = 0, vy_4 = 0;

void draw(double x, double y, double theta) {
	using namespace SDGLibF;

	double x1 = x + a * cos(theta - delta);
	double y1 = y + a * sin(theta - delta);
	double x2 = x + a * cos(theta + delta);
	double y2 = y + a * sin(theta + delta);
	double x3 = x + a * cos(M_PI + theta - delta);
	double y3 = y + a * sin(M_PI + theta - delta);
	double x4 = x + a * cos(M_PI + theta + delta);
	double y4 = y + a * sin(M_PI + theta + delta);

	SetColor(0.0, 0.0, 0.0);
	DrawLine(1, x1, y1, x2, y2);
	DrawLine(1, x2, y2, x3, y3);
	DrawLine(1, x3, y3, x4, y4);
	DrawLine(1, x4, y4, x1, y1);
	DrawPoint(2, x, y);

	//cout << y1 << " " << y2 << endl;

}

void displayfunc() {
	using namespace SDGLibF;

	Before();	// Set drawing plane

	stringstream ss;
	ss << "time= " << timer;
	SetColor(0.0, 0.0, 0.0);
	DrawString(500, 480, ss.str());
	stringstream ss2;
	ss2 << fixed << "vx= " << setprecision(4) << vx;
	DrawString(500, 450, ss2.str());
	stringstream ss3;
	ss3 << fixed << "vy= " << setprecision(4) << vy;
	DrawString(500, 420, ss3.str());
	stringstream ss4;
	ss4 << fixed << "w= " << setprecision(4) << w;
	DrawString(500, 390, ss4.str());
	stringstream ss5;
	SetColor(0.0, 0.0, 1.0);
	if (!simuflag) DrawString(-700, 450, "Press s to start simulation...");
	SetColor(1.0, 0.0, 0.0);
	if (joyconflag) DrawString(-700, 400, "Game Controller Available");

	draw(x_1, y_1, theta_1);
	draw(x_2, y_2, theta_2);
	draw(x_3, y_3, theta_3);
	draw(x_4, y_4, theta_4);

	DrawLine(1, x_1, y_1, x_2, y_2);
	SetColor(0.0, 0.0, 0.0);
	DrawLine(1, x_2, y_2, x_3, y_3);
	DrawLine(1, x_3, y_3, x_4, y_4);
	DrawLine(1, x_4, y_4, x_1, y_1);
	//DrawPoint(2, 0, 0);

	After();	// Set drawn plane
}

void simulation() {
	using namespace SDGLibF;

	if(joyconflag) glutForceJoystickFunc();

	static double dt = 0.002;

	theta += w * dt;
	if (theta == 0 && vx==0 && vy==0);//0除算を防ぐ
	else {
		theta_1 = atan((vy + d * w * cos(theta)) / (vx - d * w * sin(theta)));
		theta_2 = atan((vy - d * w * sin(theta)) / (vx - d * w * cos(theta)));
		theta_3 = atan((vy - d * w * cos(theta)) / (vx + d * w * sin(theta)));
		theta_4 = atan((vy + d * w * sin(theta)) / (vx + d * w * cos(theta)));
	}

	vx_1 = vx - d * w * sin(theta);
	vy_1 = vy + d * w * cos(theta);
	vx_2 = vx - d * w * cos(theta);
	vy_2 = vy - d * w * sin(theta);
	vx_3 = vx + d * w * sin(theta);
	vy_3 = vy - d * w * cos(theta);
	vx_4 = vx + d * w * cos(theta);
	vy_4 = vy + d * w * sin(theta);

	x_4 += vx_4 * dt;
	y_4 += vy_4 * dt;
	x_1 += vx_1 * dt;
	y_1 += vy_1 * dt;
	x_2 += vx_2 * dt;
	y_2 += vy_2 * dt;
	x_3 += vx_3 * dt;
	y_3 += vy_3 * dt;

	timer += dt;

	ReDraw();
}


void joystick(unsigned int ButtonMask, int x, int y, int z)
{
	vx = x * 0.1;
	vy = y * -0.1;

	if (ButtonMask & 1) w += M_PI / 180 * 0.01;
	if (ButtonMask & 2) w -= M_PI / 180 * 0.01;
	/*
	if (300 < x) printf("右");
	if (x < -300) printf("左");
	if (300 < y) printf("上");
	if (y < -300) printf("下");

	if (ButtonMask & 1) printf("△");
	if (ButtonMask & 2) printf("○");
	if (ButtonMask & 4) printf("×");
	if (ButtonMask & 8) printf("□");
	if (ButtonMask & 16) printf("L2");
	if (ButtonMask & 32) printf("R2");
	if (ButtonMask & 64) printf("L1");
	if (ButtonMask & 128) printf("R1");
	if (ButtonMask & 256) printf("START");
	if (ButtonMask & 512) printf("SELECT");

	*/

	glutPostRedisplay();/* 画面再描画 */
}

void keyboardfunc(unsigned char k, int _x, int _y) {
	using namespace SDGLibF;
	switch (k) {
	case 27:	/* Escape */
		exit(0);
		break;
	case 's':
		IdleFunc(simulation);
		simuflag = true;
		break;
	case 'S':
		IdleFunc(NULL);
		simuflag = false;
		break;
	case 'z':
		w -= M_PI / 180;
		break;
	case 'x':
		w += M_PI / 180;
		break;
	case 'j':
		glutJoystickFunc(joystick, 10);
		joyconflag = true;
		break;
	defalt:
		return;
	}

	ReDraw();
}

void specialkeydown(int key, int x, int y)
{
	using namespace SDGLibF;
	if (key == GLUT_KEY_UP)//矢印「上」
	{
		vy += 1;
	}

	if (key == GLUT_KEY_DOWN)//矢印「下」
	{
		vy -= 1;
	}

	if (key == GLUT_KEY_LEFT) {
		vx -= 1;
	}

	if (key == GLUT_KEY_RIGHT)
	{
		vx += 1;
	}
	//cout << vx << " " << vy << endl;
	ReDraw();

}


int main(void)
{
	SDGLib mygraphic(1500, 1000, "- 2D Graphics - (4_wheel_steering simulation)", -750, 750, -500, 500);
	mygraphic.SetCursor(GLUT_CURSOR_WAIT);
	mygraphic.Display(displayfunc);
	mygraphic.Keyboard(keyboardfunc);
	glutSpecialFunc(specialkeydown);
	mygraphic.Start();
	return 0;
}
