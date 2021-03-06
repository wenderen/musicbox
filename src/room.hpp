#ifndef ROOM_H
#define ROOM_H

// Room walls
int define_floor();
int define_ceiling();
int define_side_wall(int x, bool left);
int define_back_wall();
int define_front_wall();
int define_door();

// Table
int define_table_top();
int define_table_leg();

// One_legged table
int define_one_legged_table_top();
int define_one_legged_table_leg();
int define_one_legged_table_base();

// Chair
int define_chair_seat();
int define_chair_leg();
int define_chair_back();

// Lamp
int define_lamp_stand();
int define_lamp_head();
int define_lamp_base();
int define_lamp_light();

// Stool
int define_stool_seat();
int define_stool_leg();

// Wall light

int define_wall_light_base();
int define_wall_light_neck();
int define_wall_light_head();

#endif