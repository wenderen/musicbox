#include <GL/glut.h>
#include "box.hpp"

// Definitions for the components of the box
int define_base_and_walls() {
	int base_and_walls = glGenLists(1);
	glNewList(base_and_walls, GL_COMPILE);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // comment this out to enable normal fill drawing of polygons
		glTranslatef(-1.5,-3.5,-2);
		glScalef(3,3,3);
		glColor3ub(140,60,200);
		// Front wall
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(1,0,0);
			glVertex3f(1,0.5,0);
			glVertex3f(0,0.5,0);
		glEnd();
		glColor3ub(100,120,100);
		// Right wall
		glBegin(GL_QUADS);
			glVertex3f(1,0,0);
			glVertex3f(1,0,-1);
			glVertex3f(1,0.5,-1);
			glVertex3f(1,0.5,0);
		glEnd();
		// Left wall
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0,0.5,0);
			glVertex3f(0,0.5,-1);
			glVertex3f(0,0,-1);
		glEnd();
		glColor3ub(100,170,50);
		// Back wall
		glBegin(GL_QUADS);
			glVertex3f(0,0,-1);
			glVertex3f(1,0,-1);
			glVertex3f(1,0.5,-1);
			glVertex3f(0,0.5,-1);
		glEnd();
		glColor3ub(200,170,50);

		// Base
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(1,0,0);
			glVertex3f(1,0,-1);
			glVertex3f(0,0,-1);
		glEnd();
	glEndList();
	return base_and_walls;
}

int define_lid() {
	int lid = glGenLists(1);
	glNewList(lid, GL_COMPILE);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // comment this out to enable normal fill drawing of polygons
		glTranslatef(-1.5,-2,-2); // at y = -2 the lid exactly covers the box
		glScalef(3,3,3);
		glColor3ub(220,70,100);
		// glColor3i(1,1,1);
		// Front wall
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(1,0,0);
			glVertex3f(1,0.05,0);
			glVertex3f(0,0.05,0);
		glEnd();
		glColor3ub(100,20,100);
		// Right wall
		glBegin(GL_QUADS);
			glVertex3f(1,0,0);
			glVertex3f(1,0,-1);
			glVertex3f(1,0.05,-1);
			glVertex3f(1,0.05,0);
		glEnd();
		// Left wall
		glBegin(GL_QUADS);
			glVertex3f(0,0,0);
			glVertex3f(0,0.05,0);
			glVertex3f(0,0.05,-1);
			glVertex3f(0,0,-1);
		glEnd();
		// Back wall
		glBegin(GL_QUADS);
			glVertex3f(0,0,-1);
			glVertex3f(1,0,-1);
			glVertex3f(1,0.05,-1);
			glVertex3f(0,0.05,-1);
		glEnd();
		glColor3ub(130,140,50);
		// Top
		glBegin(GL_QUADS);
			glVertex3f(0,0.05,0);
			glVertex3f(1,0.05,0);
			glVertex3f(1,0.05,-1);
			glVertex3f(0,0.05,-1);
		glEnd();
	glEndList();
	return lid;
}

// Definitions for the components of the dancer
// Dancer's head
int define_head() {
	int head = glGenLists(1);
	glNewList(head, GL_COMPILE);
		glColor3ub(165, 42, 42);
		// Front face
		glBegin(GL_POLYGON);
			glVertex3f(-0.25,0,0);
			glVertex3f(0.25,0,0);
			glVertex3f(0.35,0.25,0);
			glVertex3f(0.3,0.5,0);
			glVertex3f(-0.3,0.5,0);
			glVertex3f(-0.35,0.25,0);
		glEnd();
		// Back face
		glBegin(GL_POLYGON);
			glVertex3f(-0.25,0,-0.5);
			glVertex3f(0.25,0,-0.5);
			glVertex3f(0.35,0.25,-0.5);
			glVertex3f(0.3,0.5,-0.5);
			glVertex3f(-0.3,0.5,-0.5);
			glVertex3f(-0.35,0.25,-0.5);
		glEnd();
		// Scalp
		glBegin(GL_QUADS);
			glVertex3f(-0.3,0.5,0);
			glVertex3f(0.3,0.5,0);
			glVertex3f(0.3,0.5,-0.5);
			glVertex3f(-0.3,0.5,-0.5);
		glEnd();
		// Top left side
		glColor3ub(165, 112, 42);
		glBegin(GL_QUADS);
			glVertex3f(-0.3,0.5,0);
			glVertex3f(-0.35,0.25,0);
			glVertex3f(-0.35,0.25,-0.5);
			glVertex3f(-0.3,0.5,-0.5);
		glEnd();
		// Bottom left side
		glColor3ub(100, 112, 200);
		glBegin(GL_QUADS);
			glVertex3f(-0.35,0.25,0);
			glVertex3f(-0.25,0,0);
			glVertex3f(-0.25,0,-0.5);
			glVertex3f(-0.35,0.25,-0.5);
		glEnd();
		// Top right side
		glColor3ub(165, 112, 42);
		glBegin(GL_QUADS);
			glVertex3f(0.3,0.5,0);
			glVertex3f(0.35,0.25,0);
			glVertex3f(0.35,0.25,-0.5);
			glVertex3f(0.3,0.5,-0.5);
		glEnd();
		// Bottom right side
		glColor3ub(100, 112, 200);
		glBegin(GL_QUADS);
			glVertex3f(0.35,0.25,0);
			glVertex3f(0.25,0,0);
			glVertex3f(0.25,0,-0.5);
			glVertex3f(0.35,0.25,-0.5);
		glEnd();
		// Chin
		glBegin(GL_QUADS);
			glVertex3f(-0.25,0,0);
			glVertex3f(0.25,0,0);
			glVertex3f(0.25,0,-0.5);
			glVertex3f(-0.25,0,-0.5);
		glEnd();
	glEndList();
	return head;
}

// Dancer's neck
int define_neck() {
	int neck = glGenLists(1);
	glNewList(neck, GL_COMPILE);
		glColor3ub(150, 60, 20);
		glScalef(0.25,0.375,0.25);
		glutSolidCube(1);
		glScalef(4,2.67,4); // anti-scale to prevent the head from scaling
	glEndList();
	return neck;
}

// Dancer's top torso (torso1)
int define_torso1() {
	int torso1 = glGenLists(1);
	glNewList(torso1, GL_COMPILE);
		glColor3ub(19, 215, 132);
		glutSolidCube(1);
	glEndList();
	return torso1;
}

// Dancer's middle torso (torso2)
int define_torso2() {
	int torso2 = glGenLists(1);
	glNewList(torso2, GL_COMPILE);
		glColor3ub(83, 164, 39);
		glScalef(0.8,0.25,0.5);	
		glutSolidCube(1);
		glScalef(1.25,4,2);
	glEndList();
	return torso2;
}

// Either of the hips of the dancer
int define_hip() {
	int hip = glGenLists(1);
	glNewList(hip, GL_COMPILE);
		glColor3ub(150, 124, 148);
		glScalef(0.25, 0.25, 0.25);
		glutSolidCube(1);
		glScalef(4,4,4);
	glEndList();
	return hip;
}

// Either of the thighs of the dancer
int define_thigh() {
	int thigh = glGenLists(1);
	glNewList(thigh, GL_COMPILE);
		glColor3ub(124, 124, 255);
		glScalef(0.25,0.8,0.25);
		glutSolidCube(1);
		glScalef(4,1.25,4);
	glEndList();
	return thigh;
}

// Either of the knees of the dancer
int define_knee() {
	int knee = glGenLists(1);
	glNewList(knee, GL_COMPILE);
		glColor3ub(200, 200, 50);
		glScalef(0.2, 0.2, 0.2);
		glutSolidCube(1);
		glScalef(5,5,5);
	glEndList();
	return knee;
}

// Either of the legs of the dancer
int define_leg() {
	int leg = glGenLists(1);
	glNewList(leg, GL_COMPILE);
		glColor3ub(50, 148, 148);

		glScalef(0.25,0.6,0.25);
		glutSolidCube(1);
		glScalef(4,3.33,4);
	glEndList();
	return leg;
}

// Either of the ankles of the dancer
int define_ankle() {
	int ankle = glGenLists(1);
	glNewList(ankle, GL_COMPILE);
		glColor3ub(50,200,50);
		glScalef(0.2,0.05,0.2);
		glutSolidCube(1);
		glScalef(5,20,5);
	glEndList();
	return ankle;
}

// Either of the feet of the dancer
int define_foot() {
	int feet = glGenLists(1);
	glNewList(feet, GL_COMPILE);
		glColor3ub(100, 50, 50);
		// Top of the foot
		glBegin(GL_QUADS);
			glVertex3f(-0.125,0,0); // A
			glVertex3f(0.125,0,0);  // B
			glVertex3f(0.125,0,-0.25);  // C
			glVertex3f(-0.125,0,-0.25); // D
		glEnd();
		// Sole of the foot
		glBegin(GL_QUADS);
			glVertex3f(-0.125,-0.1,0.15); // E
			glVertex3f(0.125,-0.1,0.15); // F
			glVertex3f(0.125,-0.1,-0.25); // G
			glVertex3f(-0.125,-0.1,-0.25); // H
		glEnd();
		// Left face of the foot
		glBegin(GL_QUADS);
			glVertex3f(-0.125,0,0); // A
			glVertex3f(-0.125,0,-0.25); // D
			glVertex3f(-0.125,-0.1,-0.25); // H
			glVertex3f(-0.125,-0.1,0.15); // E
		glEnd();
		// Right face of the foot
		glBegin(GL_QUADS);
			glVertex3f(0.125,0,0); // B
			glVertex3f(0.125,0,-0.25); // C
			glVertex3f(0.125,-0.1,-0.25); // G
			glVertex3f(0.125,-0.1,0.15); // F
		glEnd();
	glEndList();
	return feet;
}

// Drawing function for the box
void draw_box(double lid_degrees, double box_degrees) {
	int base_and_walls = define_base_and_walls();
	int lid = define_lid();
	glPushMatrix();
		// glScalef(1,1,2);
		glTranslatef(0, 0, -2);
		glRotatef(box_degrees, 0, 1, 0);
		glTranslatef(0, 0, 2);
		// draw the lid
		glPushMatrix();
			glTranslatef(0, -2, -5);
			glRotatef(lid_degrees, 1, 0, 0);
			glTranslatef(0, 2, 5);
			glCallList(lid);
		glPopMatrix();
		// draw the base and walls
		glCallList(base_and_walls);
	glPopMatrix();
}

// Drawing function for the dancer
void draw_dancer(double angle) {
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // comment this out to enable normal fill drawing of polygons

	// define the various components of the dancer
	int head = define_head();
	int neck = define_neck();

	int torso1 = define_torso1();
	int torso2 = define_torso2();
	int torso3 = define_torso1();

	int left_hip = define_hip();
	int right_hip = define_hip();

	int left_thigh = define_thigh();
	int right_thigh = define_thigh();

	int left_knee = define_knee();
	int right_knee = define_knee();

	int left_ankle = define_ankle();
	int right_ankle = define_ankle();

	int left_leg = define_leg();
	int right_leg = define_leg();

	int left_foot = define_foot();
	int right_foot = define_foot();

	// torso3 and onwards
	glPushMatrix();
		// glScalef(1.4,1.4,1.4); // scale the dancer
		// glRotatef(angle, 0, 1, 0);
		glScalef(1,0.5,0.5);
		glCallList(torso3); // draw torso3
		glScalef(1,2,2);
		// torso2 and onwards
		glPushMatrix();
			glTranslatef(0,0.375,0);
			glCallList(torso2); // draw torso2
			//torso1 and onwards
			glPushMatrix();
				glTranslatef(0,0.625,0); // translation of torso1 wrt torso2
				// glTranslatef(0,-0.6875,0.25);
				glScalef(1,1,0.5);
				glCallList(torso1); //draw torso1
				glScalef(1,1,2); // anti-scale to prevent the other dependent parts from scaling
				// glTranslatef(0,0.6875,-0.25); // anti-translate
				// left arm
				glPushMatrix();
					// draw left arm
				glPopMatrix();
				// right arm
				glPushMatrix();
					// draw right arm
				glPopMatrix();
				// neck
				glPushMatrix();
					glRotatef(angle, 0, 1, 0);
					glTranslatef(0,0.6875,0); // translation of neck wrt torso1
					glCallList(neck); // draw the neck
					// head
					glPushMatrix();
						glTranslatef(0,0.1875,0.25); // translation of head wrt neck
						glScalef(1,1.5,1);
						glCallList(head); // draw the head
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
		// left hip
		glPushMatrix();
			glTranslatef(-0.25,-0.375,0);
			glCallList(left_hip);
			// left thigh
			glPushMatrix();
				glTranslatef(0,-0.525,0);
				glCallList(left_thigh);
				// left knee
				glPushMatrix();
					glTranslatef(0,-0.5,0);
					glCallList(left_knee);
					// left leg
					glPushMatrix();
						glTranslatef(0,-0.4,0);
						glCallList(left_leg);
						// left ankle
						glPushMatrix();
							glTranslatef(0,-0.175,0); // not sure how we got -0.175...
							// also, it seems to be independent of the leg length!
							glCallList(left_ankle);
							// left foot
							glPushMatrix();
								glTranslatef(0,-0.025,0.125);
								glCallList(left_foot);
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
		// right hip
		glPushMatrix();
			glTranslatef(0.25,-0.375,0);
			glCallList(right_hip);
			// right thigh
			glPushMatrix();
				glTranslatef(0,-0.525,0);
				glCallList(right_thigh);
				// right knee
				glPushMatrix();
					glTranslatef(0,-0.5,0);
					glCallList(right_knee);
					// right leg
					glPushMatrix();
						glTranslatef(0,-0.4,0);
						glCallList(right_leg);
						// right ankle
						glPushMatrix();
							glTranslatef(0,-0.175,0);
							glCallList(right_ankle);
							// right foot
							glPushMatrix();
								glTranslatef(0,-0.025,0.125);
								glCallList(right_foot);
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}