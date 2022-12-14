#pragma once


#include "cgp/cgp.hpp"
#include "environment.hpp"


using cgp::mesh_drawable;


struct gui_parameters {
	bool display_frame = true;
	bool display_wireframe = false;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	camera_controller_orbit_euler camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;

	mesh_drawable global_frame;          // The standard global frame
	environment_structure environment;   // Standard environment controler
	input_devices inputs;                // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;                  // Standard GUI element storage

	// Timer used for the animation
	cgp::timer_basic timer;

	// The entire hierarchy
	cgp::hierarchy_mesh_drawable hierarchy;

	// Flappy Bird
	float gravity = -25;
	float minSpeed = -30;
	float maxSpeed = 15;

	unsigned int lastTubeId = 0;
	float lastSpawnedTube = 0;
	float bird_speed_y = 15;
	float prevDeltaTime = 0;

	// ****************************** //
	// Functions
	// ****************************** //

	void initialize();    // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();   // The display of the GUI, also called within the animation loop


	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();

};





