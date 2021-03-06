#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

#include "helpers.hpp"
#include "room.hpp"

using namespace std;

// Display current information about the program (current joint, which lights are on/off etc.)
void
display_info(
	short int curr_joint,
	bool move_left,
	bool lamp_light,
	bool wall_light,
	bool record_mode
) {
	cout << endl << "                -------INFORMATION-----------\n\n";

	// Show current mode
	cout << "Current mode: " << (record_mode ? "RECORD" : "PLAYBACK") << endl;
	// Show which body joint is active
	switch(curr_joint) {
		case 0: cout << "Current joint: head-neck\n"; break;
		case 1:	cout << "Current joint: shoulder\n"; break;
		case 2:	cout << "Current joint: neck-torso1\n"; break;
		case 3:	cout << "Current joint: torso1-torso2\n"; break;
		case 4:	cout << "Current joint: torso2-torso3\n"; break;
		case 5:	cout << "Current joint: hip\n"; break;
		case 6:	cout << "Current joint: ankle\n"; break;
		case 7:	cout << "Current joint: wrist\n"; break;
		case 8:	cout << "Current joint: knee\n"; break;
		case 9:	cout << "Current joint: elbow\n"; break;
	}
	if (curr_joint > 4) {
		// Show which side is active (only if the joint has three degrees of freedom)
		cout << "Active side: ";
		if (move_left) cout << " left" << endl;
		else cout << " right" << endl;
	}
	// Show state of the two lights
	cout << "Lamp light is " << (lamp_light ? "ON" : "OFF") << endl;
	cout << "Wall light is " << (wall_light ? "ON" : "OFF") << endl;

	cout << "\n                ----------------------------\n\n";
}

// Display help on what key to press to perform a certain task
void
display_keybindings_help() {
	cout << endl << "       ------------Keybindings help------------" << endl << endl;
	cout << "Exit the program: `Esc`";
	cout << "Get help on keybindings: `F1`";
	cout << "Toggle which object to move: `t` (when you start the program, the dancer is selected)";
	cout << "Toggle which side to move: `r` (when you start the program, the left side is selected)";
	cout << "Select joint: `0` to `9`";
	cout << "Rotate about x-axis: `w` and `s`";
	cout << "Rotate about x-axis: `a` and `d`";
	cout << "Rotate about x-axis: `q` and `e`";
	cout << "Rotate the entire dancer about the y-axis: `,` and `.`";
	cout << "Display information: `i`";
	cout << "Switch on/switch off the lamp: `k`";
	cout << "Switch on/switch off the wall light: `l`";
	cout << "Reset everything: `F5`";
	cout << "Enter the room: `Enter`";
	cout << "Move the control point selection plane along the z-axis: `↑` and `↓`";
	cout << "Select a point to use as a control point: mouse click";
	cout << "Start the animation: `Space`";
	cout << "Delete the last created control points: right mouse click";
	cout << "Clear the keyframes file while the program is running: `F4`";
	cout << "Save the cuurent keyframe to the keyframes file: `F3`";
	cout << "Start playback: `p`";
	cout << "\n       ----------------------------------------" << endl << endl;
}

// Configure OpenGL
void
initGL() {
	glClearColor(0, 0, 0, 1.f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST); // enable Z-buffer algorithm
	glEnable(GL_ALPHA_TEST); // enable alpha value testing
	glAlphaFunc(GL_GREATER,0);

	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);

	// Set up light 0
	GLfloat lamp_light_ambient[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lamp_light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lamp_light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lamp_light_position[] = {-3,0,0,1};

	glLightfv(GL_LIGHT0, GL_AMBIENT, lamp_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lamp_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lamp_light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lamp_light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.03);

	// Set up light 1
	GLfloat wall_light_ambient[] = {1.0, 1.0, 0.0, 1.0};
	GLfloat wall_light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat wall_light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat wall_light_position[] = {4.5,2.3,-5,1};

	glLightfv(GL_LIGHT1, GL_AMBIENT, wall_light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, wall_light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, wall_light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, wall_light_position);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 2);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.1);

	glLightfv(GL_LIGHT1, GL_POSITION, wall_light_position);
}

// Initialize the limits array
void
init_limits(float* limits) {
	// head-neck
	limits[0]=-15;limits[1]=15;
	limits[2]=-60;limits[3]=60;
	limits[4]=-20;limits[5]=20;
	// left shoulder
	limits[6]=-90;limits[7]=40;
	limits[8]=-20;limits[9]=20;
	limits[10]=-50;limits[11]=0;
	// right shoulder
	limits[12]=-90;limits[13]=40;
	limits[14]=-20;limits[15]=20;
	limits[16]=0;limits[17]=50;
	// neck-torso1
	limits[18]=-30;limits[19]=30;
	limits[20]=-60;limits[21]=75;
	limits[22]=-15;limits[23]=15;
	// torso1-torso2
	limits[24]=-20;limits[25]=90;
	limits[26]=-60;limits[27]=60;
	limits[28]=-10;limits[29]=10;
	// torso2-torso3
	limits[30]=-20;limits[31]=40;
	limits[32]=-60;limits[33]=60;
	limits[34]=-10;limits[35]=10;
	// left hip
	limits[36]=-30;limits[37]=30;
	limits[38]=-50;limits[39]=10;
	limits[40]=-10;limits[41]=3;
	// right hip
	limits[42]=-30;limits[43]=30;
	limits[44]=-10;limits[45]=50;
	limits[46]=-3;limits[47]=10;
	// left ankle
	limits[48]=0;limits[49]=30;
	limits[50]=-10;limits[51]=10;
	limits[52]=-10;limits[53]=10;
	// right ankle
	limits[54]= 0;limits[55]=30;
	limits[56]=-10;limits[57]=10;
	limits[58]=-10;limits[59]=10;
	// left wrist
	limits[60]=-60;limits[61]=60;
	limits[62]=-20;limits[63]=20;
	limits[64]=-20;limits[65]=20;
	// right wrist
	limits[66]=-60;limits[67]=60;
	limits[68]=-20;limits[69]=20;
	limits[70]=-20;limits[71]=20;
	// left knee
	limits[72]=0;limits[73]=110;
	// right knee
	limits[74]=0;limits[75]=110;
	// left elbow
	limits[76]=-140;limits[77]=0;
	// right elbow
	limits[78]=-140;limits[79]=0;
}

// Find the index to change in the 'angles' vector
int
find_index_x(
	int& curr_joint,
	bool move_left
) {
	int index = 0;
	if (curr_joint <= 7) index = find_index_y(curr_joint, move_left) - 1;
	else {
		if (curr_joint == 8) index = (move_left ? 36 : 37);
		if (curr_joint == 9) index = (move_left ? 38 : 39);
	}
	return index;
}

int
find_index_y(
	int& curr_joint,
	bool move_left
) {
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


int
find_index_z(
	int& curr_joint,
	bool move_left
) {
	int index = find_index_y(curr_joint, move_left) + 1;
	return index;
}

int define_marker_sphere() {
	int marker = glGenLists(1);
	glNewList(marker, GL_COMPILE);
		GLfloat color[] = {0.f,0.f,1.f,1.f};
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color);
		glutSolidSphere(0.1,10,10);
	glEndList();
	return marker;
}

void draw_marker_sphere() {
	int marker = define_marker_sphere();
	glCallList(marker);
}

void init_lists(
	int* room_display_lists
) {
	room_display_lists[0] = define_floor();
	room_display_lists[1] = define_ceiling();
	room_display_lists[2] = define_side_wall(-6,true);
	room_display_lists[3] = define_side_wall(9,false);
	room_display_lists[4] = define_front_wall();
	room_display_lists[5] = define_back_wall();
	room_display_lists[6] = define_base_and_walls();
	room_display_lists[7] = define_head();
	room_display_lists[8] = define_neck();
	room_display_lists[9] = define_torso1();
	room_display_lists[10] = define_torso2();
	room_display_lists[11] = define_torso3();
	room_display_lists[12] = define_hip();
	room_display_lists[13] = define_hip();
	room_display_lists[14] = define_thigh();	
	room_display_lists[15] = define_knee();
	room_display_lists[16] = define_ankle();
	room_display_lists[17] = define_leg();
	room_display_lists[18] = define_foot();
	room_display_lists[19] = define_shoulder();
	room_display_lists[20] = define_upper_arm();
	room_display_lists[21] = define_elbow();
	room_display_lists[22] = define_lower_arm();
	room_display_lists[23] = define_wrist();
	room_display_lists[24] = define_hand();
	room_display_lists[25] = define_hat();
	room_display_lists[26] = define_stick();
	room_display_lists[27] = define_table_top();
	room_display_lists[28] = define_table_leg();
	room_display_lists[29] = define_one_legged_table_top();
	room_display_lists[30] = define_one_legged_table_leg();
	room_display_lists[31] = define_one_legged_table_base();
	room_display_lists[32] = define_chair_seat();
	room_display_lists[33] = define_chair_leg();
	room_display_lists[34] = define_chair_back();
	room_display_lists[35] = define_stool_seat();
	room_display_lists[36] = define_stool_leg();
	room_display_lists[37] = define_lamp_stand();
	room_display_lists[38] = define_lamp_head();
	room_display_lists[39] = define_lamp_base();
	room_display_lists[40] = define_wall_light_base();
	room_display_lists[41] = define_wall_light_neck();
	room_display_lists[42] = define_wall_light_head();
}