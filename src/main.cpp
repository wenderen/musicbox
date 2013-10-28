#include <iostream>
#include <fstream>
#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <utility>
#include <algorithm>
// #include <GLUT/glut.h> // uncomment this to compile on OS X

#include "draw.hpp"
#include "coordinate.hpp"
#include "bezier.hpp"
#include "helpers.hpp"

#define GLUT_FRAME_TIME 100

using namespace std;

int find_index_x();
int find_index_y();
int find_index_z();

float limits[80] = {0};

// each element in limits stores the extent to which the joint is allowed to rotate
// for joint k, the values limits[2*k] and limits[2*k+1] give the min and max limits respectively
// these values are hardcoded by us

/* Global vars */

float world_x_angle = 0;
float world_y_angle = 0;
float world_z_angle = 0;

float lid_angle = 0;
float box_angle = 0;
float lid_angle_increment = 3.f;

float dancer_angles[40] = {0};
float dancer_angle = 0;

float dancer_y = 0;

float door_angle = 0;

float plane_z = 0;

/* Variables determining which body part to move */
bool move_box = false; // When true, keyboard keys affect the box. When false, they affect the dancer
bool move_left = true; // When true, keys affect the left side of the dancer. Not valid for joints that have no left or right, for example, the head-neck joint
short int curr_joint = 0; // Which joint to move. This int takes values between 0 and 9, both included

/*
These are the key-to-joint mappings:
   -----------------------
    Key |     Joint
   -----------------------
    0   |     head-neck
	1   |     shoulder
	2   |     neck-torso1
	3   |     torso1-torso2
	4   |     torso2-torso3
	5   |     hip
	6   |     ankle
	7   |     wrist
	8   |     knee
	9   |     elbow
	----------------------
*/


/* Variables determining which light to turn on or off */
bool lamp_light = false; // controls the lamp on the left
bool wall_light = false; // controls the wall light on the back wall

/* Boolean controlling whether the Bezier curve is drawn or not */
bool draw_bezier = false;

/* Boolean controlling whether the animation has started or not */
bool move_camera = false;

/* Boolean controlling whether file writing is enabled or not */
bool write_enabled = false;

/* Camera coordinates */
float camera_x = 0;
float camera_y = 0;
float camera_z = 5;

int current_index = 20; // index of curve_points

vector<coordinate_t> control_points;
vector<coordinate_t> curve_points;

/* Window Parameters */
int window_id;

vector<double> GetOGLPos(int x, int y) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    posZ = (plane_z * 0.9) - 2;
 	
 	vector<double> result;
 	result.push_back(posX);
 	result.push_back(posY);
 	result.push_back(posZ);

 	return result;
}

// Find the index to change in the 'angles' vector
int find_index_x() {
	int index = 0;
	if (curr_joint <= 7) index = find_index_y() - 1;
	else {
		if (curr_joint == 8) index = (move_left ? 36 : 37);
		if (curr_joint == 9) index = (move_left ? 38 : 39);
	}
	return index;
}

int find_index_y() {
	int index = 0;
	switch (curr_joint) {
		case 0: index = 1; break;
		case 1: index = (move_left ? 4 : 7); break;
		case 2: index = 10; break;
		case 3: index = 13; break;
		case 4: index = 16; break;
		case 5: index = (move_left ? 19 : 22); break;
		case 6: index = (move_left ? 25 : 28); break;
		case 7: index = (move_left ? 31 : 34); break;
	}
	return index;
}


int find_index_z() {
	int index = find_index_y() + 1;
	return index;
}

/* Callback Declarations */
void display();
void reshape(int, int);
void keyboard(unsigned char, int, int);
void process_special_keys(int,int,int);
void mouse(int, int, int, int);
void renderGL(int, char**);
void timer(int value);

void init() {
	// initialize the control points vector
	coordinate_t box_control_point;
	box_control_point.xx = -1.87471;
	box_control_point.yy = 0.8;
	box_control_point.zz = -1;

	control_points.push_back(box_control_point);

	initGL(); // set up the camera, etc.

	init_limits(limits); // set up the limits vector
}

void timer(int value) {
	if (move_camera) {
		if (value > -1) {
			coordinate_t point = curve_points[value];
			camera_x = point.xx;
			camera_y = point.yy;
			camera_z = point.zz;
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(
				camera_x,camera_y,camera_z,
				-1.8857,-0.093121,-2,
				0,1,0
			);
		}
		else if ((value >= -20) and (value <= -1)) {
			if ((lid_angle-5) >= -90) {
				lid_angle -= 5.f;
			}
		}
		else if ((value >= -40) and (value < -20)) {
			if ((dancer_y+0.05) <= 1){
				dancer_y+=0.05;
			}
		}

		glutPostRedisplay();
		if (value >= -40) glutTimerFunc(GLUT_FRAME_TIME,timer,value-1);
		else glutTimerFunc(10000,timer, value);
		// else return;
	}
	else {
		glutTimerFunc(GLUT_FRAME_TIME,timer,value);
		// return;
	}
}

/* Callback Definitions */
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (lamp_light) glEnable(GL_LIGHT0);
	else glDisable(GL_LIGHT0);

	if (wall_light) glEnable(GL_LIGHT1);
	else glDisable(GL_LIGHT1);

	glPushMatrix();
		glRotatef(world_x_angle, 1,0,0);
		glRotatef(world_y_angle, 0,1,0);
		glRotatef(world_z_angle, 0,0,1);
		draw_all_objects(
			lid_angle,
			dancer_angles, dancer_angle, dancer_y,
			door_angle
		);
		if (not move_camera) {
			for (vector<coordinate_t>::iterator itr = control_points.begin(); itr != control_points.end(); itr++) {
				glPushMatrix();
					glTranslatef(itr->xx, itr->yy, itr->zz);
					GLfloat color[] = {0.f,0.f,1.f,1.f};
					glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color);
					glutSolidSphere(0.1,10,10);
				glPopMatrix();
			}
		}

		if (draw_bezier) {
			draw_bezier_curve(curve_points, 0.001);
		}

		if (not draw_bezier) draw_plane(plane_z);
	glPopMatrix();


	glutSwapBuffers();
}

void reshape(int w, int h) {
	if (h == 0) h = 1; // prevent a division by zero
	float ratio = 1.0f * w / h;
	
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(120, ratio, 1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camera_x,camera_y,camera_z,0,0,0,0,1,0);
}

void keyboard(unsigned char key, int x, int y) {
	if (65 <= key and key <= 90) key += 32; // so that even if caps lock is on, the program still works as expected
	int mod = glutGetModifiers();
	switch(key) {
		case 27: {
			cout << "Exiting the program...\n";
			glutDestroyWindow(window_id);
			exit(0);
		}
		break;

		// Toggle which object to move
		case 't': {
			if (move_box) cout << "Selected dancer" << endl;
			else cout << "Selected box" << endl;
			move_box = not move_box;
		}
		break;

		// Toggle which side is active
		case 'r': {
			if (move_left) cout << "Right side active" << endl;
			else cout << "Left side active" << endl;
			move_left = not move_left;
		}
		break;

		// Select a joint
		case '0':   curr_joint = 0; cout << "Current joint: head-neck\n"; break;
		case '1':	curr_joint = 1;	cout << "Current joint: shoulder\n"; break;
		case '2':	curr_joint = 2;	cout << "Current joint: neck-torso1\n"; break;
		case '3':	curr_joint = 3;	cout << "Current joint: torso1-torso2\n"; break;
		case '4':	curr_joint = 4;	cout << "Current joint: torso2-torso3\n"; break;
		case '5':	curr_joint = 5;	cout << "Current joint: hip\n"; break;
		case '6':	curr_joint = 6;	cout << "Current joint: ankle\n"; break;
		case '7':	curr_joint = 7;	cout << "Current joint: wrist\n"; break;
		case '8':	curr_joint = 8;	cout << "Current joint: knee\n"; break;
		case '9':	curr_joint = 9;	cout << "Current joint: elbow\n"; break;

		// rotate X
		case 's': {
			if (mod == GLUT_ACTIVE_SHIFT) {
				world_x_angle += 3;
			}
			else {
				if (move_box) {
					// don't change the angle if it is already shut
					if (-1*lid_angle >= lid_angle_increment) {
						lid_angle += lid_angle_increment;
					}
				}
				// all joints whether they have 1 or 3 dofs can rotate
				// about the x-axis
				else {
					int index = find_index_x();
					if (dancer_angles[index]+3 <= limits[2*index+1]) {
						dancer_angles[index] += 3;
					}
				}
			}
			glutPostRedisplay();
		}
		break;
		case 'w': {
			if (mod == GLUT_ACTIVE_SHIFT) {
				world_x_angle -= 3;
			}
			else {
				if (move_box) {
					// don't change the angle if it is already fully open
					if (-1*lid_angle <= 90-lid_angle_increment) {
						lid_angle -= lid_angle_increment;
					}
				}
				// all joints whether they have 1 or 3 dofs can rotate
				// about the x-axis
				else {
					int index = find_index_x();
					if (dancer_angles[index]-3 >= limits[2*index]) {
						dancer_angles[index] -= 3;
					}
				}
			}
			glutPostRedisplay();
		}
		break;

		// rotate Y
		case 'a': {
			if (mod == GLUT_ACTIVE_SHIFT) {
				world_y_angle -= 3;
			}
			else {
				if (not(move_box) and curr_joint <= 7) {
					int index = find_index_y();
					if (dancer_angles[index]-3 >= limits[2*index]) {
						dancer_angles[index] -= 3;
					}
				}
			}
			glutPostRedisplay();
			// nothing for 8 and 9 because the knee and elbow rotate about the z-axis only
		}
		break;
		case 'd': {
			if (mod == GLUT_ACTIVE_SHIFT) {
				world_y_angle += 3;
			}
			else {
				// check if we have to move the box
				if (not(move_box) and curr_joint <= 7) {
					int index = find_index_y();
					if (dancer_angles[index]+3 <= limits[2*index+1]) {
						dancer_angles[index] += 3;
					}
				}
			}
			glutPostRedisplay();
			// nothing for 8 and 9 because the knee and elbow rotate about the z-axis only
		}
		break;

		// rotate Z
		case 'q': {
			if (mod == GLUT_ACTIVE_SHIFT) {
				world_z_angle -= 3;
			}
			else {
				if (move_box) {}
				else if (curr_joint <= 7) {
					int index = find_index_z();
					if (dancer_angles[index]+3 <= limits[2*index+1]) {
						dancer_angles[index] += 3;
					}
				}
			}
			glutPostRedisplay();
		}
		break;
		case 'e': {
			if (mod == GLUT_ACTIVE_SHIFT) {
				world_z_angle += 3;
			}
			else {
				if (move_box) {}
				else if (curr_joint <= 7) {
					int index = find_index_z();
					if (dancer_angles[index]-3 >= limits[2*index]) {
						dancer_angles[index] -= 3;
					}
				}
			}
			glutPostRedisplay();
		}
		break;

		case ',': {
			if (not(move_box) and dancer_angle - 3 >= -90) {
				dancer_angle -= 3;
				glutPostRedisplay();
			}
		}
		break;

		case '.': {
			if (not(move_box) and dancer_angle + 3 <= 90) {
				dancer_angle += 3;
				glutPostRedisplay();
			}
		}
		break;

		// toggle lamp light
		case 'k': {
			lamp_light = not lamp_light; 
			cout << "Lamp light is now " << (lamp_light ? "ON" : "OFF") << endl;
			glutPostRedisplay();
		}
		break;

		// toggle wall light
		case 'l': {
			wall_light = not wall_light; 
			cout << "Wall light is now " << (wall_light ? "ON" : "OFF") << endl;
			glutPostRedisplay();
		}
		break;

		// open and close door
		case '+': {
			if (door_angle <= 90-3) {
				door_angle += 3;
			}
			glutPostRedisplay();
		}
		break;

		case '-': {
			if (door_angle >= 3) {
				door_angle -= 3;
			}
			glutPostRedisplay();
		}
		break;

		// Get current information
		case 'i': {
			display_info(
				curr_joint,
				move_box,
				move_left,
				lamp_light,
				wall_light
			);
		}
		break;

		// Draw the approximated Bezier curve
		case ' ': {
			if (not draw_bezier) {
				if (control_points.size() >= 3) {
					draw_bezier = true;

					coordinate_t door_control_point;
					door_control_point.xx = 2.74009;
					door_control_point.yy = -1.35724;
					door_control_point.zz = 2.5;

					control_points.push_back(door_control_point);
					curve_points = complete(control_points, 0.05);
					current_index = curve_points.size() - 1;
					move_camera = true;

					glutPostRedisplay();
				}
			}
		}
		break;

		// Enter the room
		case 'g': {
			if (not move_camera) {
				cout << "Entering room...\n";
				camera_z = 2.55;

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				gluLookAt(camera_x,camera_y,camera_z,0,0,0,0,1,0);
				glutPostRedisplay();
			}
		}
		break;
	}
}

void process_special_keys(int key, int x, int y) {
	int mod = glutGetModifiers();
	switch(key) {
		// Get help
		case GLUT_KEY_F1:
			display_keybindings_help();
			break;

		// reset everything
		case GLUT_KEY_F5: {
			cout << "Resetting everything..." << endl;
			for (int i = 0 ; i < 40 ; i++) dancer_angles[i] = 0;
			lid_angle = 0;
			lamp_light = false;
			wall_light = false;
			glutPostRedisplay();
		}
		break;

		case GLUT_KEY_DOWN: {
			plane_z -= 0.1;
			glutPostRedisplay();
		}
		break;

		case GLUT_KEY_UP: {
			plane_z += 0.1;
			glutPostRedisplay();
		}
		break;

		// Save a keyframe
		case GLUT_KEY_F3: {
			// If writing is disabled, enable it and return control
			if (not write_enabled) {
				write_enabled = true;
				cout << "You can now start saving keyframes! Press F3 to save a keyframe\n";
			}
			else {
				// If writing is enabled, allow user to either save keyframes or clear the file
				// Hold down Shift to clear the file
				if (mod == GLUT_ACTIVE_SHIFT) {
					cout << "Clearing out the file...\n";
				}
				cout << "Saved a keyframe\n";
			}
		}
		break;
	}
}

void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) {
		if (button == GLUT_DOWN) {
			vector<double> actual_coords = GetOGLPos(x,y);
			cout << actual_coords[0] << " ";
			cout << actual_coords[1] << " ";
			cout << actual_coords[2] << "\n";
			// cout << "Creating a sphere at " << x << " " << y << endl;
			coordinate_t new_control_point;
			new_control_point.xx = actual_coords[0];
			new_control_point.yy = actual_coords[1];
			new_control_point.zz = actual_coords[2];
			control_points.push_back(new_control_point);
		}
		else if (button == GLUT_RIGHT_BUTTON) {
			if (control_points.size() >= 2) {
				control_points.pop_back();
				cout << "Deleted the last control point" << endl;
			}
			else {
				cout << "Can't delete any more control points!" << endl;
			}
		}
		glutPostRedisplay();
	}
}

void renderGL(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	glutInitWindowPosition(0, 0);

	window_id = glutCreateWindow("Music Box");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(process_special_keys);
	glutMouseFunc(mouse);
	glutTimerFunc(GLUT_FRAME_TIME,timer,current_index);

	glutMainLoop();
}

int main(int argc, char** argv) {
	renderGL(argc, argv);
	return 0;
}
