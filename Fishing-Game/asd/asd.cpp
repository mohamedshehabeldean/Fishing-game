#define STB_IMAGE_IMPLEMENTATION
#include<Windows.h> // get screen size
#include<iostream>
#include<cmath>
#include<GL/stb_image.h>
#include<gl/glut.h>
#include<iostream>
#include<math.h>
#include<string>
#include <ctime>

#define PI 3.1415926

GLfloat eyez = 16, eyey = 1.5, eyex = 0.5;
GLfloat upz = 0, upx = 0.5;
GLfloat cam = -10;

///////////////// Global variables ///////////////
unsigned int texture;
unsigned char* data = NULL;
int wid, hei, nrChannels;
bool fullScreen = true;
bool smooth, firstTimeq = true
, firstTimep = true, stop;
bool keystates[256];//عشان تدوس ع اكتر من زرار فنفس الوقت 
float width, height, sWidth, sHeight, sy, cx = 1, cy = 0;//cx,cy to change color of صنارة
float x1, x2;
float a1 = 0;//angle of rotate
int as = 1, qw = 1;
float x_circle, y_circle, i_circle;

// Time and score
int score = 0;
int seconds = 60;
int t_begin = time(NULL) + 60;
bool game_end = false;
bool win = false, lose = true;

// Light vars
GLfloat amb[] = { 1.0f,1.0f,0.17f,1.0f };
GLfloat diff[] = { 1.0f,0.78f,0.52f,1.0f };
GLfloat spec[] = { 0.84f,0.0f,0.01f,1.0f };
GLfloat shine = 38.0f;

GLfloat lightamb[] = { 0.5, 0.5, 0.5, 1 };
GLfloat lightdiff[] = { 1, 1, 1, 1 };
GLfloat lightspec[] = { 1, 1, 1, 1 };
GLfloat lightPos[] = { 16, 9.8, -9, 1.0 };

// positions of the center of fishing rod
GLfloat xrod = 0.3, yrod = 6.7;

//shark center
GLfloat xsh = 8, ysh = -7;
bool sharkCol = false;


/// Gold fish
bool fish_moving_right1 = true;
bool fish_moving_right2 = true;
bool fish_moving_right3 = true;
float gold_fish_speed = 3;

float gold_fish_x1 = 5;
float gold_fish_y1 = 0.0f;
bool gold_fish_coll1 = false;

float gold_fish_x2 = -5;
float gold_fish_y2 = -2;
bool gold_fish_coll2 = false;

float gold_fish_x3 = -22;
float gold_fish_y3 = -4;
bool gold_fish_coll3 = false;

//red fish1
float red_fish_x1 = 5;
float red_fish_y1 = -3;
float red_fish_speed1 = 0.5;
bool red_fish_coll1 = false;

//red fish2
float red_fish_x2 = 0;
float red_fish_y2 = -6;
float red_fish_speed2 = 0.3;
bool red_fish_coll2 = false;

//red fish3
float red_fish_x3 = -10;
float red_fish_y3 = 0;
float red_fish_speed3 = 0.7;
bool red_fish_coll3 = false;


// green fish
float green_fish_y = -3;
float green_fish_x = -3;
float green_fish_speed = 0.6;
bool green_fish_coll = false;

///////////////// End Global variables ///////////////

// opengl functions 
void draw();
void reshape(int, int);
void backGround();
void load(int imgnum);
void check(unsigned char* data);
void timer(int);
void keyboarddown(unsigned char, int, int);//unsigned character>>ascll code,int>>x for position of mouse and int for position of y 
void keyboardup(unsigned char, int, int);
void keyspecialdown(int, int, int);
void keyspecialup(int, int, int);

// Functions used in drawing objects
////////////////////////////////////
void sky();
void bird();
void sun();
void sea();
void ship();
void sea_star();
void plant();
///////////////////////////////////
void upperShark(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
void lowerShark(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
void shark(float x, float y, float z);
void sharkCollison();
////////////////////////////////////
void drawGoldFish(float, float, float);
void goldFishColl1();
void goldFishColl2();
void goldFishColl3();
////////////////////////////////////
void draw_red_fish(float, float, float);
void redFishColl1();
void redFishColl2();
void redFishColl3();
void draw_green_fish(float, float, float);
void greenFishColl();
////////////////////////////////////
void displayString(float x, float y, float z, float r, float g, float b, std::string w);

int main(int argc, char** argv)
{
	// glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	// end of glut init
	// create window
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
	//glutInitWindowSize(width, height);
	//glutInitWindowPosition(10,10);
	glutCreateWindow("SEA");
	glutFullScreen();
	// end create window
	// call back functions
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutTimerFunc(1000, timer, 0);
	glutKeyboardFunc(keyboarddown);
	glutKeyboardUpFunc(keyboardup);
	glutSpecialFunc(keyspecialdown);
	glutSpecialUpFunc(keyspecialup);
	/////////ADD sound to game/////////////////

	PlaySound(TEXT("sound.wav"), NULL, SND_ASYNC);
	////////********//////////////

	backGround();
	// end call back functions
	// main loop
	glutMainLoop();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0f, eyey, eyez, 0.0f, eyey, 0.0f, 0.0f, 1.0f, 0.0f);

	////////*******************////////
	sky();
	bird();
	sun();
	sea();
	sea_star();
	plant();
	////////*******************////////

	ship();

	if (!game_end)
	{
		if (!sharkCol)
			shark(xsh, ysh, cam);
		if (!gold_fish_coll1)
			drawGoldFish(gold_fish_x1, gold_fish_y1, cam);
		if (!gold_fish_coll2)
			drawGoldFish(gold_fish_x2, gold_fish_y2, cam);
		if (!gold_fish_coll3)
			drawGoldFish(gold_fish_x3, gold_fish_y3, cam);
		if (!red_fish_coll1)
			draw_red_fish(red_fish_x1, red_fish_y1, cam + 1);
		if (!red_fish_coll2)
			draw_red_fish(red_fish_x2, red_fish_y2, cam + 1);
		if (!red_fish_coll3)
			draw_red_fish(red_fish_x3, red_fish_y3, cam + 1);
		if (!green_fish_coll)
			draw_green_fish(green_fish_x, green_fish_y, cam + 1);

		//displayScore
		displayString(-17.5, 10.5, cam, 1, 1, 1, "Score : " + std::to_string(score));
		//displayTime
		displayString(-17.5, 9.5, cam, 1, 1, 1, "Timer : " + std::to_string(seconds));
	}
	else
	{
		if (win)
		{
			//display Winning
			displayString(-2, 0, cam, 1, 1, 1, "Winner Congrats " + std::to_string(score));
		}
		else if (lose)
		{
			//display Loser
			displayString(-2, 0, cam, 1, 1, 1, "Loser try again " + std::to_string(score));
		}
	}


	glutSwapBuffers();
}

void reshape(int w, int h)
{

	if (h == 0)
	{
		h = 1;
	}
	GLfloat ratio = (GLfloat)w / (GLfloat)h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void backGround()
{
	// color [0,1]
	glClearColor(0, 0, 0, 0);


	/*------Begin Ligth-------*/
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightspec);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdiff);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);

	glEnable(GL_COLOR_MATERIAL);
	/*------End Ligth-------*/

	glEnable(GL_TEXTURE_2D);//texture 
	glEnable(GL_DEPTH_TEST);//z
}

void timer(int v)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 120, timer, 0);

	if (seconds == 0)
	{
		game_end = true;

		if (score >= 8)
			win = true;
		else
			lose = true;


	}

	if (!game_end)
	{
		int t_now = time(NULL);
		seconds = t_begin - t_now;

		if (!sharkCol)
		{
			sharkCollison();
			if (xsh > -26)
			{
				xsh = xsh - 1;
			}
			else
			{
				xsh = 24;
				ysh = ((float)rand() / (float)RAND_MAX) * 3.0f - 7.0f; // get random range for Y value from 0 to -7
			}
		}

		if (!gold_fish_coll1) {
			goldFishColl1();
			if (fish_moving_right1) {
				gold_fish_x1 += gold_fish_speed;
				if (gold_fish_x1 > 23) {
					gold_fish_x1 = -20.0f;
					fish_moving_right1 = false;
				}
			}
			else {
				gold_fish_x1 -= gold_fish_speed;
				if (gold_fish_x1 < -23) {
					gold_fish_x1 = -20.0f;
					gold_fish_y1 = ((float)rand() / (float)RAND_MAX) * 3.0f - 7.0f; // get random range for Y value from 0 to -6
					fish_moving_right1 = true;
				}
			}
		}
		if (!gold_fish_coll2) {
			goldFishColl2();
			if (fish_moving_right2) {
				gold_fish_x2 += gold_fish_speed;
				if (gold_fish_x2 > 23) {
					gold_fish_x2 = -21.0f;
					fish_moving_right2 = false;
				}
			}
			else {
				gold_fish_x2 -= gold_fish_speed;
				if (gold_fish_x2 < -23) {
					gold_fish_x2 = -21.0f;
					gold_fish_y2 = ((float)rand() / (float)RAND_MAX) * 3.0f - 7.0f; // get random range for Y value from 0 to -6
					fish_moving_right2 = true;
				}
			}
		}
		if (!gold_fish_coll3) {
			goldFishColl3();
			if (fish_moving_right3) {
				gold_fish_x3 += gold_fish_speed;
				if (gold_fish_x3 > 23) {
					gold_fish_x3 = -22.0f;
					fish_moving_right3 = false;
				}
			}
			else {
				gold_fish_x3 -= gold_fish_speed;
				if (gold_fish_x3 < -23) {
					gold_fish_x3 = -22.0f;
					gold_fish_y3 = ((float)rand() / (float)RAND_MAX) * 3.0f - 7.0f; // get random range for Y value from 0 to -6
					fish_moving_right3 = true;
				}
			}
		}

		// hadi
		if (!red_fish_coll1)
		{
			redFishColl1();
			red_fish_x1 += red_fish_speed1;

			// reverse direction if fish goes off screen
			if (red_fish_x1 > 18 + 4.8 || red_fish_x1 < -18 - 4.8)
			{
				red_fish_x1 = -18 - 4.8;
				red_fish_y1 = ((float)rand() / (float)RAND_MAX) * 3.0f - 7.0f; // get random range for Y value from 0 to -7
			}
		}

		if (!red_fish_coll2)
		{
			redFishColl2();
			red_fish_x2 += red_fish_speed2;

			// reverse direction if fish goes off screen
			if (red_fish_x2 > 18 + 4.8 || red_fish_x2 < -18 - 4.8)
			{
				red_fish_x2 = -18 - 4.8;
				red_fish_y2 = ((float)rand() / (float)RAND_MAX) * 3.0f - 7.0f; // get random range for Y value from 0 to -7
			}
		}

		if (!red_fish_coll3)
		{
			redFishColl3();
			red_fish_x3 += red_fish_speed3;

			// reverse direction if fish goes off screen
			if (red_fish_x3 > 18 + 4.8 || red_fish_x3 < -18 - 4.8)
			{
				red_fish_x3 = -18 - 4.8;
				red_fish_y3 = ((float)rand() / (float)RAND_MAX) * 3.0f - 7.0f; // get random range for Y value from 0 to -7
			}
		}

		if (!green_fish_coll) {
			greenFishColl();
			green_fish_x += green_fish_speed;

			// reverse direction if fish goes off screen
			if (green_fish_x > 18 + 4.8 || green_fish_x < -18 - 4.8) {
				green_fish_x = -18 - 4.8;
				green_fish_y = ((float)rand() / (float)RAND_MAX) * 3.0f - 7.0f; // get random range for Y value from 0 to -7
			}
		}
	}
	else
	{

	}
}

void keyboarddown(unsigned char key, int x, int y)
{
	keystates[key] = true;//طول منت حاطت ايدك عالزرار هو شغال
	if (key == 'c' || key == 'C') //عشان تغير الصنارة
	{
		cx = !cx;
		cy = !cy;
	}
	if (key == 27)
	{
		exit(0);
	}
	if (key == 'p')
	{
		std::cout << "score = " << score << std::endl;
		std::cout << "S = " << seconds << std::endl;
	}
}

void keyboardup(unsigned char key, int x, int y)
{
	keystates[key] = false;//لما تشيل ايدك من عالزرار ميشتغلشي  
}

void keyspecialdown(int key, int x, int y)
{
	keystates[key] = true;
	//لما يدوس ع الاف اتنين مع الاتجاه بيزود السرعة
	if (keystates[GLUT_KEY_F2] && keystates[GLUT_KEY_LEFT])
	{
		if (xrod > -18)
		{
			x1 -= 0.8;
			xrod -= 0.8;
		}
	}
	else if (keystates[GLUT_KEY_F2] && keystates[GLUT_KEY_RIGHT])
	{
		if (xrod < 18)
		{
			x1 += 0.8;
			xrod += 0.8;
		}
	}
	else if (keystates[GLUT_KEY_F2] && keystates[GLUT_KEY_UP])
	{
		if (yrod < 6.7)
		{
			x2 += 0.8;
			yrod += 0.8;
		}
	}
	else if (keystates[GLUT_KEY_F2] && keystates[GLUT_KEY_DOWN])
	{
		if (yrod >= -8.5)
		{
			x2 -= 0.8;
			yrod -= 0.8;
		}
	}
	else if (keystates[GLUT_KEY_RIGHT])
	{
		if (xrod < 18)
		{
			x1 += 0.5;
			xrod += 0.5;
		}
	}
	//لما يدوس ع الاف اتنين مع الاتجاه بيزود السرعة
	else if (keystates[GLUT_KEY_LEFT])
	{
		if (xrod > -18)
		{
			x1 -= 0.5;
			xrod -= 0.5;
		}
	}
	else if (keystates[GLUT_KEY_DOWN])
	{
		if (yrod >= -8.5)
		{
			x2 -= 0.4;
			yrod -= 0.4;
			a1 += 0.5;
		}
	}
	else if (keystates[GLUT_KEY_UP])
	{
		if (yrod < 6.7)
		{
			x2 += 0.4;
			yrod += 0.4;
			a1 -= 0.5;
		}
	}
	else if (key == GLUT_KEY_F1)
	{
		fullScreen = !fullScreen;//toggle
		if (fullScreen)
		{
			glutFullScreen();
		}
		else
		{
			glutReshapeWindow(width / 2, height / 2);
			glutPositionWindow(width / 2 - width / 4, height / 2 - height / 4);//position to window
		}
	}
}

void keyspecialup(int key, int x, int y)
{
	keystates[key] = false;
}

void load(int imgnum)
{
	if (imgnum == 1)
	{

		//data = stbi_load("C:\\Users\\Zo3'la\\Desktop\\SEA\\asd\\image4.jpg", &wid, &hei, &nrChannels, 0);
		data = stbi_load("image4.jpg", &wid, &hei, &nrChannels, 0);
		check(data);
	}
	/*else if (imgnum == 2)
	{

		data = stbi_load("sky.jpg", &wid, &hei, &nrChannels, 0);
		check(data);
	}*/

}

void check(unsigned char* data)
{
	if (data)
	{
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);//deconstructor to remove pointer
}

//function of the sea
void sea()
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTranslatef(0, 0.0, cam);
	load(1);
	glBegin(GL_QUADS);
	//glColor3f(0.0, 0.0, 1.0);
	glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-20, 2, 0);
	glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-20, -10, 0);
	glTexCoord2d(1.0f, 1.0f);
	glVertex3f(20, -10, 0);
	glTexCoord2d(0.0f, 1.0f);
	glVertex3f(20, 2, 0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	glPopMatrix();
}
///////////**********************//////////
//function of the sky
void sky()
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texture);

	glTranslatef(0, 0.0, cam - 1);
	//load(2);
	glBegin(GL_QUADS);
	glColor3f(0.529, 0.807, 0.921);
	//glTexCoord2d(0.0f, 0.0f);
	glVertex3f(-20, 18, 0);
	//glTexCoord2d(1.0f, 0.0f);
	glVertex3f(-20, 2, 0);
	//glTexCoord2d(1.0f, 1.0f);
	glVertex3f(20, 2, 0);
	//glTexCoord2d(0.0f,1.0f);
	glVertex3f(20, 18, 0);
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	glPopMatrix();

}
//////////*********************//////////////
//function of the ship
void ship()
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(x1 - 12, 1.0, cam + 1);

	glBegin(GL_QUADS);
	glColor3f(1, 0.647, 0);
	glVertex3f(-4, 2.5, 0.0);
	glVertex3f(-2.5, 0, 0.0);
	glVertex3f(6, 0, 0.0);
	glVertex3f(7.5, 2.5, 0.0);
	glEnd();

	//الشراع
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);
	glVertex3f(-2, 3, 0.0);
	glVertex3f(-4, 6, 0.0);
	glVertex3f(-2, 8, 0.0);


	glEnd();
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(-2.05, 4, 0.0);
	glVertex3f(-2.05, 0, 0.0);
	glVertex3f(-2, 6, 0.0);
	glVertex3f(0, 0, 0.0);
	glVertex3f(-2, 8, 0.0);
	glVertex3f(2, 0, 0.0);


	glEnd();

	//glPopAttrib();

////////////////////////////
// fisherman/////
///////////////////////////

	//glPushAttrib(GL_CURRENT_BIT); //

	glTranslatef(4, 6, 0);

	//glutSolidSphere(0.4, 20, 20);

	//head of man
	glBegin(GL_LINE_LOOP);
	//color of line
	glColor3f(1, 1, 1);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	for (i_circle = 0; i_circle <= 2 * 3.14; i_circle += 0.1)
	{
		x_circle = 0.5 * cos(i_circle);
		y_circle = 0.5 * sin(i_circle);
		glVertex2f(x_circle, y_circle);
	}

	glEnd();
	//eye
	glEnable(GL_POINT_SMOOTH);//عشان لما تكبر النقطة تبقي دايرة مش مربع
	glPointSize(8);
	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);
	glVertex3f(0.20, 0.10, 0.0);

	glEnd();

	//kneck and mouse
	glBegin(GL_LINES);
	glVertex3f(0.15, -0.20, 0.0);
	glVertex3f(0.45, -0.20, 0.0);

	glVertex3f(0.15, -0.50, 0.0);
	glVertex3f(0.15, -1.1, 0.0);

	glVertex3f(0, -0.50, 0.0);
	glVertex3f(0, -1.1, 0.0);

	glEnd();
	//body
	glBegin(GL_LINE_STRIP);
	glVertex3f(-0.20, -1.1, 0.0);
	glVertex3f(-0.20, -2, 0.0);
	glVertex3f(0.40, -2, 0.0);
	glVertex3f(0.40, -1.1, 0.0);
	glVertex3f(-0.20, -1.1, 0.0);



	glEnd();
	//legs
	glBegin(GL_LINES);
	glVertex3f(-0.20, -2, 0.0);
	glVertex3f(-0.10, -3, 0.0);
	glVertex3f(0.40, -2, 0.0);
	glVertex3f(0.30, -3, 0.0);

	glEnd();
	//shoes
	glColor3f(1, 1, 1);
	glBegin(GL_QUAD_STRIP);
	glVertex3f(-0.10, -3, 0.0);
	glVertex3f(-0.10, -4, 0.0);
	glVertex3f(0.30, -4, 0.0);
	glVertex3f(0.30, -3, 0.0);

	glEnd();
	//arm1
	glBegin(GL_LINE_STRIP);
	glVertex3f(-0.15, -1.5, 0.0);
	glVertex3f(0.70, -1.6, 0.0);
	glVertex3f(2, -1.5, 0.0);

	glEnd();
	//arm2
	glBegin(GL_LINES);
	glVertex3f(0.40, -1.4, 0.0);
	glVertex3f(2, -1.2, 0.0);

	glEnd();
	//صنارة
	glPointSize(13);
	glBegin(GL_POINTS);
	glColor3f(0, 1, 0);
	glVertex3f(2, -1.3, 0.0);//بداية الصنارة
	glEnd();
	glColor3f(0, cx, cy);
	glBegin(GL_LINES);
	glVertex3f(2, -1.3, 0.0);
	glVertex3f(8, 1, 0.0);

	glEnd();

	glTranslatef(8, 1, 0.0);

	//glutSolidSphere(0.4, 20, 20);

	//glPushMatrix();
	//glRotatef(a1, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	//color of line
	glColor3f(1, 1, 1);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	for (i_circle = 0; i_circle <= 2 * 3.14; i_circle += 0.1)
	{
		x_circle = 0.2 * cos(i_circle);
		y_circle = 0.2 * sin(i_circle);
		glVertex2f(x_circle, y_circle);
	}

	glEnd();
	//glPopMatrix();

	//glPushMatrix();

	//tail
	glBegin(GL_LINES);
	glColor3f(cx, cy, 0);
	glVertex3f(0.20, 0, 0.0);
	glVertex3f(0.20, x2 - 1, 0.0);

	glEnd();

	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 1);
	glVertex3f(0.20, x2 - 1, 0.0);
	glVertex3f(-0.3, x2 - 1.5, 0.0);
	glVertex3f(0.8, x2 - 1.5, 0.0);


	glEnd();
	//glPopMatrix();


	glPopAttrib();
	glPopMatrix();

}

// Begin functions of the shark
void shark(float x, float y, float z)
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glTranslatef(x, y, z + 1);

	// shark eye
	glEnable(GL_POINT_SMOOTH);
	glPointSize(3);
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	glVertex3f(-3.5, 0.5, 0);
	glEnd();
	glPointSize(6);
	glColor3f(1, 1, 1);
	glBegin(GL_POINTS);
	glVertex3f(-3.5, 0.5, 0);
	glEnd();

	//shark mouth
	glColor3f(0.6, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-4.5, 0, 0);
	glVertex3f(-4.2, -0.2, 0);
	glVertex3f(-3.2, -0.2, 0);
	glVertex3f(-3.5, 0, 0);
	glEnd();

	// black line
	glColor3f(0, 0, 0);
	glLineWidth(0.1);
	glBegin(GL_LINE_LOOP);
	glVertex3f(5, 0, 0);
	glVertex3f(-5, 0, 0);
	glEnd();

	// zo3nfa body
	glColor3f(0.5, 0.4, 0.5);
	glBegin(GL_TRIANGLES);
	glVertex3f(1.5, 0, 0);
	glVertex3f(0, -0.7, 0);
	glVertex3f(0.2, 0, 0);
	glEnd();


	// zo3nfa upper
	glColor3f(0.5, 0.4, 0.5);
	glBegin(GL_TRIANGLES);
	//1st
	glVertex3f(-3, 0.5, 0);
	glVertex3f(-0.5, 0.5, 0);
	glVertex3f(-0.5, 1.8, 0);
	glEnd();

	// zo3nfa upper
	//2nd
	glColor3f(0.5, 0.4, 0.5);
	glBegin(GL_TRIANGLES);
	glVertex3f(0, 0.5, 0);
	glVertex3f(1.5, 0.5, 0);
	glVertex3f(1.5, 1.5, 0);
	glEnd();

	// The body of the shark
	glColor3f(0.5, 0.4, 0.5);
	upperShark(0, 0, 0, 5);
	glColor3f(0.5, 0.5, 0.5);
	lowerShark(0, 0, 0, 5);

	//sharktails
	glColor3f(0.5, 0.5, 0.5);
	//lower tail
	glBegin(GL_TRIANGLES);
	glVertex3f(5, 0, 0);
	glVertex3f(8, -0.75, 0);
	glVertex3f(6.5, 0, 0);

	//upper tail
	glVertex3f(5, 0, 0);
	glVertex3f(8, 0.75, 0);
	glVertex3f(6.5, 0, 0);
	glEnd();


	glPopAttrib();
	glPopMatrix();
}

void upperShark(GLfloat x, GLfloat y, GLfloat z, GLfloat radius)
{
	int i;
	int lineAmount = 360;
	glLineWidth(5);
	glBegin(GL_TRIANGLE_FAN);
	for (i = 0; i <= lineAmount; i++)
	{

		glVertex3f(
			x + (radius * cos(i * PI / lineAmount)),
			(y + (radius * sin(i * PI / lineAmount)) / 5.0f),
			z
		);

	}
	glEnd();
}

void lowerShark(GLfloat x, GLfloat y, GLfloat z, GLfloat radius)
{
	int i;
	int lineAmount = 720;
	GLfloat TwicePi = 2.0f * PI;
	glLineWidth(5);
	glBegin(GL_TRIANGLE_FAN);
	for (i = 360; i <= lineAmount; i++)
	{

		glVertex3f(
			x + (radius * cos(i * TwicePi / lineAmount)),
			(y + (radius * sin(i * TwicePi / lineAmount)) / 8.0f),
			z
		);

	}
	glEnd();
}

void sharkCollison()
{
	GLfloat x0rect = xsh - 5,
		x1rect = xsh + 5,
		y0rect = ysh + 1,
		y1rect = ysh - 0.5;

	if (
		xrod >= x0rect &&
		xrod <= x1rect &&
		yrod <= y0rect &&
		yrod >= y1rect
		)
	{
		sharkCol = true;
		score = score - 2;
	}

}
// End functions of the shark


/********************************************************************************************\
								Golden Fish (+2 bouns)
\********************************************************************************************/

// Function to draw fish
// Width of fish 2.4, height 1
void drawGoldFish(float x, float y, float z)
{

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glTranslatef(x, y, z + 1);


	// Mouth
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0, -0.1, 0.0);
	glVertex3f(0.5, -0.2, 0.0);
	glVertex3f(1.0, -0.1, 0.0);
	glEnd();

	// Eye
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(8.0f);
	glBegin(GL_POINTS);
	glVertex3f(0.5f, 0.2f, 0.0f);
	glEnd();

	// Body
	glColor3f(1.0f, 0.75f, 0.0f);
	glScalef(1, 0.5, 0.0f);
	glutSolidSphere(1, 50, 50);

	// Tail
	glColor3f(1.0f, 0.75f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-1.4, -0.9, 0.0);
	glVertex3f(-1.4, 0.9, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);
	glEnd();

	glPopAttrib();
	glPopMatrix();

}

void goldFishColl1()
{
	GLfloat x0rec = gold_fish_x1 - 1.4, x1rec = gold_fish_x1 + 1, y0rec = gold_fish_y1 + 0.5, y1rec = gold_fish_y1 - 0.5;

	if (
		xrod >= x0rec &&
		xrod <= x1rec &&
		yrod <= y0rec &&
		yrod >= y1rec
		)
	{
		gold_fish_coll1 = true;
		score = score + 2;
	}
}

void goldFishColl2()
{
	GLfloat x0rec = gold_fish_x2 - 1.4, x1rec = gold_fish_x2 + 1, y0rec = gold_fish_y2 + 0.5, y1rec = gold_fish_y2 - 0.5;

	if (
		xrod >= x0rec &&
		xrod <= x1rec &&
		yrod <= y0rec &&
		yrod >= y1rec
		)
	{
		gold_fish_coll2 = true;
		score = score + 2;
	}
}

void goldFishColl3()
{
	GLfloat x0rec = gold_fish_x3 - 1.4, x1rec = gold_fish_x3 + 1, y0rec = gold_fish_y3 + 0.5, y1rec = gold_fish_y3 - 0.5;

	if (
		xrod >= x0rec &&
		xrod <= x1rec &&
		yrod <= y0rec &&
		yrod >= y1rec
		)
	{
		gold_fish_coll3 = true;
		score = score + 2;
	}
}

/////////////////////////****************/////////////////////

//function of bird
void bird()
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(2, 6, cam);
	glPointSize(7);
	glBegin(GL_POINTS);
	//color of line
	glColor3f(1, 1, 1);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glVertex3f(3, 4, 0);

	glEnd();
	glLineWidth(5);
	glBegin(GL_LINES);
	//color of line
	glColor3f(0, 0, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glVertex3f(3, 4, 0);
	glVertex3f(4, 4.6, 0);
	glVertex3f(3, 4, 0);
	glVertex3f(2, 4.6, 0);



	glEnd();

	//////////2
	glPointSize(7);
	glBegin(GL_POINTS);
	//color of line
	glColor3f(1, 1, 1);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glVertex3f(-1, 5, 0);

	glEnd();
	glLineWidth(5);
	glBegin(GL_LINES);
	//color of line
	glColor3f(0, 0, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glVertex3f(-1, 5, 0);
	glVertex3f(0, 5.6, 0);
	glVertex3f(-1, 5, 0);
	glVertex3f(-2, 5.6, 0);



	glEnd();
	/////////3
	glPointSize(7);
	glBegin(GL_POINTS);
	//color of line
	glColor3f(1, 1, 1);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glVertex3f(-5, 5, 0);

	glEnd();
	glLineWidth(5);
	glBegin(GL_LINES);
	//color of line
	glColor3f(0, 0, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glVertex3f(-5, 5, 0);
	glVertex3f(-4, 5.6, 0);
	glVertex3f(-5, 5, 0);
	glVertex3f(-6, 5.6, 0);



	glEnd();
	////4
	glPointSize(7);
	glBegin(GL_POINTS);
	//color of line
	glColor3f(1, 1, 1);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glVertex3f(-15, 5, 0);

	glEnd();
	glLineWidth(5);
	glBegin(GL_LINES);
	//color of line
	glColor3f(0, 0, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glVertex3f(-15, 5, 0);
	glVertex3f(-14, 5.6, 0);
	glVertex3f(-15, 5, 0);
	glVertex3f(-16, 5.6, 0);



	glEnd();
	glPopAttrib();
	glPopMatrix();

}

///////////////////////***************/////////////////////////

///////////////////////****************//////////////////////

//function of sun
void sun()
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(16, 9.8, cam + 1);
	glBegin(GL_TRIANGLE_FAN);
	//color of line
	glColor3f(1, 1, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	for (i_circle = 0; i_circle <= 2 * 3.14; i_circle += 0.1)
	{
		x_circle = 2 * cos(i_circle);
		y_circle = 2 * sin(i_circle);
		glVertex2f(x_circle, y_circle);
	}

	glEnd();
	glPopMatrix();
	glPopAttrib();
}

//////////////////////****************//////////////////////

// Red fish
void draw_red_fish(float x, float y, float z)
{

	// Body
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glTranslatef(x, y, z);

	// line from (-4,0) to ( -2,0)
	glColor3f(0.0f, 1.0f, 1.0f); // sets color to cyan
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex3f(-1.0f, 0.1f, 0.0f);
	glVertex3f(-2.53f, 0.1f, 0.0f);

	glVertex3f(-2.0f, 0.4f, 0.0f);
	glVertex3f(-2.53f, 0.4f, 0.0f);

	glVertex3f(-2.0f, -0.2f, 0.0f);
	glVertex3f(-2.53f, -0.2f, 0.0f);

	glEnd();

	// Tail
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLES);
	glVertex3f(-1.2f, 0.0f, 0.0f);
	glVertex3f(-2.5f, 0.9f, 0.0f);
	glVertex3f(-2.5f, -0.9f, 0.0f);
	glEnd();

	// eye
	glColor3f(0.0f, 0.0f, 0.0f); // sets color to blue
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex3f(0.9, 0.5, 0.0);
	glEnd();

	// line from (4,0) to (2,0)  
	glColor3f(0.0f, 0.0f, 0.0f); // sets color to black 
	glLineWidth(1.2);
	glBegin(GL_LINES);
	glVertex3f(0.9f, 0.0f, 0.0f);
	glVertex3f(2.4f, 0.0f, 0.0f);
	glEnd();

	// line vertical (very small) front 
	glColor3f(0.0f, 1.0f, 1.0f); // sets color to black
	glBegin(GL_LINES);
	glVertex3f(0.7f, 0.38f, 0.0f);
	glVertex3f(0.7, -0.18f, 0.0f);

	// line vertical (small) front 
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, -0.3f, 0.0f);

	// line vertical (med)  front
	glVertex3f(0.3f, 0.7f, 0.0f);
	glVertex3f(0.3f, -0.5f, 0.0f);

	// line vertical (large) 
	glVertex3f(0.01f, 1.2f, 0.0f);
	glVertex3f(0.01f, -1.2f, 0.0f);

	// line vertical (med)  back
	glVertex3f(-0.3f, 0.7f, 0.0f);
	glVertex3f(-0.3f, -0.5f, 0.0f);

	// line vertical (small) back 
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, -0.3f, 0.0f);

	// line vertical (very small) back 
	glVertex3f(-0.7f, 0.38f, 0.0f);
	glVertex3f(-0.7f, -0.18f, 0.0f);
	glEnd();


	glColor3f(0.0f, 1.0f, 1.0f); // sets color to cyan
	glBegin(GL_TRIANGLES);
	// TOP fin
	glVertex3f(0.0f, 1.2f, 0.0f);
	glVertex3f(0.0f, 1.65f, 0.0f);
	glVertex3f(0.4f, 2.0f, 0.0f);

	glVertex3f(0.0f, 1.2f, 0.0f);
	glVertex3f(0.0f, 1.65f, 0.0f);
	glVertex3f(-0.4f, 2.0f, 0.0f);

	// Buttom fins
	glVertex3f(0.0f, -1.2f, 0.0f);
	glVertex3f(0.0f, -1.65f, 0.0f);
	glVertex3f(0.4f, -2.0f, 0.0f);

	glVertex3f(0.0f, -1.2f, 0.0f);
	glVertex3f(0.0f, -1.65f, 0.0f);
	glVertex3f(-0.4f, -2.0f, 0.0f);
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(2.4f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, 0.0f);
	glVertex3f(-2.4f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.2f, 0.0f);
	glEnd();
	glPopAttrib();
	glPopMatrix();
}

void redFishColl1()
{
	GLfloat x0rec = red_fish_x1 - 2.4, x1rec = red_fish_x1 + 2.4, y0rec = red_fish_y1 + 1.2, y1rec = red_fish_y1 - 1.2;

	if (
		xrod >= x0rec &&
		xrod <= x1rec &&
		yrod <= y0rec &&
		yrod >= y1rec
		)
	{
		red_fish_coll1 = true;
		score = score + 1;
	}
}

void redFishColl2()
{
	GLfloat x0rec = red_fish_x2 - 2.4, x1rec = red_fish_x2 + 2.4, y0rec = red_fish_y2 + 1.2, y1rec = red_fish_y2 - 1.2;

	if (
		xrod >= x0rec &&
		xrod <= x1rec &&
		yrod <= y0rec &&
		yrod >= y1rec
		)
	{
		red_fish_coll2 = true;
		score = score + 1;
	}
}

void redFishColl3()
{
	GLfloat x0rec = red_fish_x3 - 2.4, x1rec = red_fish_x3 + 2.4, y0rec = red_fish_y3 + 1.2, y1rec = red_fish_y3 - 1.2;

	if (
		xrod >= x0rec &&
		xrod <= x1rec &&
		yrod <= y0rec &&
		yrod >= y1rec
		)
	{
		red_fish_coll3 = true;
		score = score + 1;
	}
}

// Green fish
void draw_green_fish(float x, float y, float z)
{

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT);
	glTranslatef(x, y, z);

	// line from (-4,0) to ( -2,0)
	glColor3f(1.0f, 0.0f, 0.0f); // sets color to cyan
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex3f(-2.0f, 0.1f, 0.0f);
	glVertex3f(-2.53f, 0.1f, 0.0f);

	glVertex3f(-2.0f, 0.4f, 0.0f);
	glVertex3f(-2.53f, 0.4f, 0.0f);

	glVertex3f(-2.0f, -0.2f, 0.0f);
	glVertex3f(-2.53f, -0.2f, 0.0f);

	glEnd();

	// Tail
	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLES);
	glVertex3f(-1.2f, 0.0f, 0.0f);
	glVertex3f(-2.5f, 0.9f, 0.0f);
	glVertex3f(-2.5f, -0.9f, 0.0f);
	glEnd();

	// eye
	glColor3f(0.0f, 0.0f, 0.0f); // sets color to blue
	glEnable(GL_POINT_SMOOTH);
	glPointSize(10);
	glBegin(GL_POINTS);
	glVertex3f(0.9, 0.5, 0.0);
	glEnd();

	// line from (4,0) to (2,0)  
	glColor3f(0.0f, 0.0f, 0.0f); // sets color to black 
	glLineWidth(1.2);
	glBegin(GL_LINES);
	glVertex3f(0.9f, 0.0f, 0.0f);
	glVertex3f(2.4f, 0.0f, 0.0f);
	glEnd();


	glColor3f(1.0f, 0.0f, 0.0f); // sets color to black
	glEnable(GL_POINT_SMOOTH);
	glPointSize(20);
	glBegin(GL_POINTS);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(-0.2, 0.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f); // sets color to cyan
	glBegin(GL_TRIANGLES);
	// TOP fin
	glVertex3f(0.0f, 1.2f, 0.0f);
	glVertex3f(0.0f, 1.65f, 0.0f);
	glVertex3f(0.4f, 2.0f, 0.0f);

	glVertex3f(0.0f, 1.2f, 0.0f);
	glVertex3f(0.0f, 1.65f, 0.0f);
	glVertex3f(-0.4f, 2.0f, 0.0f);

	// Buttom fins
	glVertex3f(0.0f, -1.2f, 0.0f);
	glVertex3f(0.0f, -1.65f, 0.0f);
	glVertex3f(0.4f, -2.0f, 0.0f);

	glVertex3f(0.0f, -1.2f, 0.0f);
	glVertex3f(0.0f, -1.65f, 0.0f);
	glVertex3f(-0.4f, -2.0f, 0.0f);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(2.4f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.2f, 0.0f);
	glVertex3f(-2.4f, 0.0f, 0.0f);
	glVertex3f(0.0f, -1.2f, 0.0f);
	glEnd();
	glPopAttrib();
	glPopMatrix();
}

void greenFishColl()
{
	GLfloat x0rec = green_fish_x - 2.4, x1rec = green_fish_x + 2.4, y0rec = green_fish_y + 1.2, y1rec = green_fish_y - 1.2;

	if (
		xrod >= x0rec &&
		xrod <= x1rec &&
		yrod <= y0rec &&
		yrod >= y1rec
		)
	{
		green_fish_coll = true;
		score = score - 1;
	}
}

// Displaying String
void displayString(float x, float y, float z, float r, float g, float b, std::string w)
{
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(r, g, b);
	glRasterPos3f(x, y, z + 1);
	int len, i;
	len = (int)w.size();
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, w[i]);
	}
	glPopAttrib();
}

///////*****BOTTOM OF THE SEA********////////////////
void sea_star()
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(12, -7, cam + 0.5);
	glColor3f(255, 140, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(0.9, 0, 0);
	glVertex3f(0.8, 1, 0);
	glVertex3f(-0.5, 0, 0);

	glVertex3f(-0.8, 1, 0);
	glVertex3f(-1, 0, 0);

	glVertex3f(-1.8, -0.5, 0);
	glVertex3f(-0.8, -1, 0);

	glVertex3f(-0.5, -1.9, 0);
	glVertex3f(-0.2, -1, 0);
	glVertex3f(0.8, -1.9, 0);
	glVertex3f(0.5, -1, 0);
	glVertex3f(1.7, -0.5, 0);


	glEnd();

	glPopMatrix();
	glPopAttrib();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(12, -7, cam + 0.6);

	glColor3f(0, 0, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);

	glVertex3f(0.5, 0, 0);



	glEnd();
	glBegin(GL_POINTS);

	glVertex3f(-0.5, 0, 0);



	glEnd();
	glBegin(GL_LINES);

	glVertex3f(-0.4, -0.9, 0);
	glVertex3f(0.4, -0.9, 0);



	glEnd();
	glPopMatrix();
	glPopAttrib();

	////star 2///////////////////
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(-10, -7, cam + 0.5);
	glColor3f(255, 140, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(0.9, 0, 0);
	glVertex3f(0.8, 1, 0);
	glVertex3f(-0.5, 0, 0);

	glVertex3f(-0.8, 1, 0);
	glVertex3f(-1, 0, 0);

	glVertex3f(-1.8, -0.5, 0);
	glVertex3f(-0.8, -1, 0);

	glVertex3f(-0.5, -1.9, 0);
	glVertex3f(-0.2, -1, 0);
	glVertex3f(0.8, -1.9, 0);
	glVertex3f(0.5, -1, 0);
	glVertex3f(1.7, -0.5, 0);


	glEnd();

	glPopMatrix();
	glPopAttrib();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(-10, -7, cam + 0.6);

	glColor3f(0, 0, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);

	glVertex3f(0.5, 0, 0);



	glEnd();
	glBegin(GL_POINTS);

	glVertex3f(-0.5, 0, 0);



	glEnd();
	glBegin(GL_LINES);

	glVertex3f(-0.4, -0.9, 0);
	glVertex3f(0.4, -0.9, 0);



	glEnd();
	glPopMatrix();
	glPopAttrib();
}

///////////plant///////////////
void plant()
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(-15, -9, cam + 0.5);
	glColor3f(0, 1, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glLineWidth(8);//لو عاوز تعمل سمك للخط
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 2, 0);
	glVertex3f(0.5, 2.5, 0);
	glVertex3f(0.5, 3, 0);
	glVertex3f(0.3, 3.5, 0);
	glVertex3f(0, 4, 0);
	glVertex3f(-0.5, 4.5, 0);
	glVertex3f(-0.7, 4.7, 0);



	glEnd();
	glPopMatrix();
	glPopAttrib();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(-12, -9, cam + 0.5);
	glColor3f(0, 1, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glLineWidth(8);//لو عاوز تعمل سمك للخط
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 2, 0);
	glVertex3f(0.5, 2.5, 0);
	glVertex3f(0.5, 3, 0);
	glVertex3f(0.3, 3.5, 0);
	glVertex3f(0, 4, 0);
	glVertex3f(-0.5, 4.5, 0);
	glVertex3f(-0.7, 4.7, 0);



	glEnd();
	glPopMatrix();
	glPopAttrib();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(-13, -9, cam + 0.5);
	glColor3f(0, 1, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glLineWidth(8);//لو عاوز تعمل سمك للخط
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 2, 0);
	glVertex3f(0.5, 2.5, 0);
	glVertex3f(0.5, 3, 0);
	glVertex3f(0.3, 3.5, 0);
	glVertex3f(0, 4, 0);
	glVertex3f(-0.5, 4.5, 0);
	glVertex3f(-0.7, 4.7, 0);



	glEnd();
	glPopMatrix();
	glPopAttrib();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(-14, -9, cam + 0.5);
	glColor3f(0, 1, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glLineWidth(8);//لو عاوز تعمل سمك للخط
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 2, 0);
	glVertex3f(0.5, 2.5, 0);
	glVertex3f(0.5, 3, 0);
	glVertex3f(0.3, 3.5, 0);
	glVertex3f(0, 4, 0);
	glVertex3f(-0.5, 4.5, 0);
	glVertex3f(-0.7, 4.7, 0);



	glEnd();
	glPopMatrix();
	glPopAttrib();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(-13.5, -9, cam + 0.5);
	glColor3f(0, 1, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glLineWidth(8);//لو عاوز تعمل سمك للخط
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 2, 0);
	glVertex3f(0.5, 2.5, 0);
	glVertex3f(0.5, 3, 0);
	glVertex3f(0.3, 3.5, 0);
	glVertex3f(0, 4, 0);
	glVertex3f(-0.5, 4.5, 0);
	glVertex3f(-0.7, 4.7, 0);



	glEnd();
	glPopMatrix();
	glPopAttrib();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(-14.5, -9, cam + 0.5);
	glColor3f(0, 1, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glLineWidth(8);//لو عاوز تعمل سمك للخط
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 2, 0);
	glVertex3f(0.5, 2.5, 0);
	glVertex3f(0.5, 3, 0);
	glVertex3f(0.3, 3.5, 0);
	glVertex3f(0, 4, 0);
	glVertex3f(-0.5, 4.5, 0);
	glVertex3f(-0.7, 4.7, 0);



	glEnd();
	glPopMatrix();
	glPopAttrib();

	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); //
	glTranslatef(-12.5, -9, cam + 0.5);
	glColor3f(0, 1, 0);//عشان تعمل تدرج فالالوان للشكل  تحط لكل نقطتين لون 
	glLineWidth(8);//لو عاوز تعمل سمك للخط
	glBegin(GL_LINE_STRIP);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 2, 0);
	glVertex3f(0.5, 2.5, 0);
	glVertex3f(0.5, 3, 0);
	glVertex3f(0.3, 3.5, 0);
	glVertex3f(0, 4, 0);
	glVertex3f(-0.5, 4.5, 0);
	glVertex3f(-0.7, 4.7, 0);



	glEnd();
	glPopMatrix();
	glPopAttrib();
}