#include "scene.hpp"


using namespace cgp;
const char* tubeNames[3] = {"Tube A1", "Tube B1", "Tube C1"};

void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 0.0f, -20.0f, 0.0f }, {0,0,0}, {0,0,1});
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Create the hierarchy
	// ************************************ //

	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	mesh_drawable cube_base;
	mesh_drawable cylinder_base;
	mesh_drawable sphere_junction;
	mesh_drawable cylinder1;
	mesh_drawable cube1;
	mesh_drawable cylinder1_son;
	mesh_drawable yellow_cylinder;
	mesh_drawable bird_body;
	mesh_drawable bird_head;
	mesh_drawable bird_nose;
	mesh_drawable wing_up;
	mesh_drawable wing_low;
	mesh_drawable eye;
	mesh_drawable tube;
	mesh_drawable tube_top;
	mesh_drawable ground;

	// Create the geometry of the meshes
	//   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)
	/*cube_base.initialize_data_on_gpu(mesh_primitive_cube()); cube_base.model.scaling = 0.15f;
	cylinder_base.initialize_data_on_gpu(mesh_primitive_cylinder(0.05f, { 0,0,0 }, { 0,0,0.5f }));
	sphere_junction.initialize_data_on_gpu(mesh_primitive_sphere(0.1f));
	cylinder1.initialize_data_on_gpu(mesh_primitive_cylinder(0.05f, { 0,0,0 }, { 1.0f,0,0 }));
	cube1.initialize_data_on_gpu(mesh_primitive_cube()); cube1.model.scaling = 0.15f;
	cylinder1_son.initialize_data_on_gpu(mesh_primitive_cylinder(0.03f, { 0,0,-0.25f }, { 0.0f,0,0.25f }));
	yellow_cylinder.initialize_data_on_gpu(mesh_primitive_cylinder(0.02f, { -0.15f,0,0 }, { 0.15f,0,0 }));*/
	bird_body.initialize_data_on_gpu(mesh_primitive_ellipsoid({ 0.5f, 0.3f, 0.3f }));
	bird_head.initialize_data_on_gpu(mesh_primitive_sphere(0.2f));
	bird_nose.initialize_data_on_gpu(mesh_primitive_cone(0.12f, 0.2f));
	wing_up.initialize_data_on_gpu(mesh_primitive_quadrangle({ -0.3f, 0, 0 }, { 0.3f, 0, 0 }, { 0.3f, 0.4f, 0 }, { -0.3f, 0.4f, 0}));
	wing_low.initialize_data_on_gpu(mesh_primitive_quadrangle({ -0.3f, 0, 0 }, { 0.3f, 0, 0 }, { 0.15f, 0.25f, 0 }, { -0.15f, 0.25f, 0 }));
	eye.initialize_data_on_gpu(mesh_primitive_disc(0.05f));
	tube.initialize_data_on_gpu(mesh_primitive_cylinder(1, {0, 0, 0}, {0, 0, 12.0f}, 10, 20, true));
	tube_top.initialize_data_on_gpu(mesh_primitive_cylinder(1.2f, { 0, 0, 0 }, { 0, 0, 0.6f }, 10, 20, true));
	ground.initialize_data_on_gpu(mesh_primitive_quadrangle({ -50.0f, 0, 0 }, { 50.0f, 0, 0 }, { 50.0f, 20.0f, 0 }, { -50.0f, 20.0f, 0 }));

	// Set the color of some elements
	vec3 color1 = { 0.8f, 0.5f, 0.7f };
	vec3 yellow = { 1.0f, 1.0f, 0.0f };
	vec3 white = { 1.0, 1.0f, 1.0f };
	vec3 orange = { 1.0f, 0.3f, 0.0f };
	vec3 black = { 0.0f, 0.0f, 0.0f };
	vec3 green = { 0.5f, 1.0f, 0.0f };
	vec3 brown = { 0.87f, 0.72f, 0.53f };

	cylinder1.material.color = color1;
	cube1.material.color = color1;
	cylinder1.material.color = color1;
	cylinder1_son.material.color = color1;
	yellow_cylinder.material.color = yellow;
	bird_body.material.color = yellow;
	bird_head.material.color = yellow;
	bird_nose.material.color = orange;
	wing_up.material.color = white;
	wing_low.material.color = white;
	eye.material.color = black;
	tube.material.color = green;
	tube_top.material.color = green;

	ground.material.phong.specular = 0;
	ground.material.phong.diffuse = 1;
	ground.material.color = brown;

	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.
	/*hierarchy.add(cube_base, "Cube base");
	hierarchy.add(cylinder_base, "Cylinder base", "Cube base");
	hierarchy.add(sphere_junction, "Sphere junction", "Cylinder base", { 0,0,0.5f }); // the translation is used to place the sphere at the extremity of the cylinder
	hierarchy.add(cylinder1, "Cylinder 1", "Sphere junction");
	hierarchy.add(cube1, "Cube 1", "Cylinder 1", { 1.0f,0,0 }); // the translation is used to place the cube at the extremity of the cylinder
	hierarchy.add(cylinder1_son, "Cylinder 1 son", "Cube 1");
	hierarchy.add(yellow_cylinder, "Yellow cylinder 1", "Cylinder 1 son", {0, 0, 0.25f});
	hierarchy.add(yellow_cylinder, "Yellow cylinder 2", "Cylinder 1 son", { 0, 0, -0.25f });*/

	// Bird
	hierarchy.add(bird_body, "Bird body");
	hierarchy.add(bird_head, "Bird head", "Bird body", { 0.45f, 0, 0.3f });
	hierarchy.add(bird_nose, "Bird nose", "Bird head", { 0.12f, 0, 0 }, rotation_transform::from_axis_angle({0,1,0}, 1.571f));
	hierarchy.add(wing_up, "Bird up left wing", "Bird body");
	hierarchy.add(wing_up, "Bird up right wing", "Bird body");
	hierarchy.add(wing_low, "Bird low left wing", "Bird up left wing", {0, 0.4f, 0});
	hierarchy.add(wing_low, "Bird low right wing", "Bird up right wing", { 0, 0.4f, 0 });
	hierarchy["Bird body"].transform_local.translation.z = 3.0f;
	hierarchy["Bird body"].transform_local.scaling = 2.0f;

	// Tubes
	hierarchy.add(tube, "Tube A1");
	hierarchy.add(tube_top, "Tube A1 hat", "Tube A1", { 0, 0, 12.0f });
	hierarchy.add(tube, "Tube A2", "Tube A1", { 0, 0, 32.5f /* hole between tubes */ });
	hierarchy.add(tube_top, "Tube A2 hat", "Tube A2", { 0, 0, 12.0f });
	hierarchy["Tube A2"].transform_local.rotation = rotation_transform::from_axis_angle({ 0, 1, 0 }, Pi);
	hierarchy["Tube A1"].transform_local.translation.z = -17.0f; // hole height
	hierarchy["Tube A1"].transform_local.translation.x = 100.0f; // first tubes spawn on the left out of frame

	hierarchy.add(tube, "Tube B1");
	hierarchy.add(tube_top, "Tube B1 hat", "Tube B1", { 0, 0, 12.0f });
	hierarchy.add(tube, "Tube B2", "Tube B1", { 0, 0, 32.5f });
	hierarchy.add(tube_top, "Tube B2 hat", "Tube B2", { 0, 0, 12.0f });
	hierarchy["Tube B2"].transform_local.rotation = rotation_transform::from_axis_angle({ 0, 1, 0 }, Pi);
	hierarchy["Tube B1"].transform_local.translation.x = 100.0f;

	hierarchy.add(tube, "Tube C1");
	hierarchy.add(tube_top, "Tube C1 hat", "Tube C1", { 0, 0, 12.0f });
	hierarchy.add(tube, "Tube C2", "Tube C1", { 0, 0, 32.5f });
	hierarchy.add(tube_top, "Tube C2 hat", "Tube C2", { 0, 0, 12.0f });
	hierarchy["Tube C2"].transform_local.rotation = rotation_transform::from_axis_angle({ 0, 1, 0 }, Pi);
	hierarchy["Tube C1"].transform_local.translation.x = 100.0f;

	// Ground
	hierarchy.add(ground, "Ground");
	hierarchy["Ground"].transform_local.translation.z = -11.0f;

	// Skyblue color
	environment.background_color = { 0.53, 0.81, 0.92 };
}


float mapRange(float val, float low, float up, float newLow, float newUp)
{
	return (val - low) / (up - low) * (newUp - newLow) + newLow;
}

void scene_structure::display_frame()
{
	float deltaTime = timer.t - prevDeltaTime;
	prevDeltaTime = timer.t;

	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();

	// Update the current time
	timer.update();

	// Update bird physics
	if (bird_speed_y > minSpeed) {
		bird_speed_y += gravity * deltaTime;
	}

	// Tube spawning
	lastSpawnedTube += deltaTime;
	if (lastSpawnedTube > 2.0f)
	{
		const char* tubeName = tubeNames[lastTubeId];
		std::cout << "Spawning tube " << tubeName << std::endl;

		float randY = rand_interval(-20.0f, -9.0f);
		hierarchy[tubeName].transform_local.translation.z = randY;
		hierarchy[tubeName].transform_local.translation.x = 10.0f;

		lastSpawnedTube = 0;
		lastTubeId = (++lastTubeId) % 3;
	}

	float normMaxSpeed = 1;
	float normMinSpeed = -1;
	float norm = mapRange(bird_speed_y, minSpeed, maxSpeed, normMinSpeed, normMaxSpeed);

	// Apply transformation to some elements of the hierarchy
	/*hierarchy["Cylinder 1"].transform_local.rotation = rotation_transform::from_axis_angle({0,0,1}, timer.t);
	hierarchy["Cube base"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, cos(timer.t));
	hierarchy["Cube 1"].transform_local.rotation = rotation_transform::from_axis_angle({1,0,0}, -3 * timer.t);
	hierarchy["Cylinder 1 son"].transform_local.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, 8 * timer.t);*/

	// Bird physics
	hierarchy["Bird body"].transform_local.translation += {0, 0, bird_speed_y * deltaTime};
	hierarchy["Bird body"].transform_local.rotation = rotation_transform::from_axis_angle({ 0, 1, 0 }, -norm);

	// Bird animations
	hierarchy["Bird up left wing"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -cos(7 * timer.t) / 2);
	hierarchy["Bird low left wing"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -cos(7 * timer.t));
	hierarchy["Bird up right wing"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, cos(7 * timer.t) / 2) * rotation_transform::from_axis_angle({ 1,0,0 }, Pi);
	hierarchy["Bird low right wing"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, cos(7 * timer.t));
	hierarchy["Bird head"].transform_local.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, cos(2 * timer.t) / 4);

	// Tubes
	float tubeSpeed = 0.035f;
	hierarchy["Tube A1"].transform_local.translation.x -= tubeSpeed;
	hierarchy["Tube B1"].transform_local.translation.x -= tubeSpeed;
	hierarchy["Tube C1"].transform_local.translation.x -= tubeSpeed;

	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchy.update_local_to_global_coordinates();

	// Draw the hierarchy as a single mesh
	draw(hierarchy, environment);
	if (gui.display_wireframe)
		draw_wireframe(hierarchy, environment);
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}

