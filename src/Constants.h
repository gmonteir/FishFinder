#pragma once

#include "GLSL.h"
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 ORIGIN = glm::vec3(0);
const glm::vec3 XAXIS = glm::vec3(1.0, 0.0, 0.0);
const glm::vec3 YAXIS = glm::vec3(0.0, 1.0, 0.0);
const glm::vec3 ZAXIS = glm::vec3(0.0, 0.0, 1.0);

// Assets
const std::string RESOURCE_DIR = "../resources";

const std::string DORY_TEXTURE = "DORY_TXT";
const std::string NEMO_TEXTURE = "NEMO_TXT";
const std::string SQUIRT_TEXTURE = "SQUIRT_TXT";
const std::string JENNY_TEXTURE = "JENNY_TXT";
const std::string CHARLIE_TEXTURE = "CHARLIE_TXT";
const std::string BLOAT_TEXTURE = "BLOAT_TXT";
const std::string MARLIN_TEXTURE = "MARLIN_TXT";
const std::string GURGLE_TEXTURE = "GURGLE_TXT";
const std::string CAUSTIC_TEXTURE = "CAUSTIC_TXT";
const std::string FLOOR_TEXTURE = "FLOOR_TXT";

const std::string CUBE_SHAPE = "CUBE";
const std::string SPHERE_SHAPE = "SPHERE";
const std::string DORY_SHAPE = "DORY";
const std::string NEMO_SHAPE = "NEMO";
const std::string SQUIRT_SHAPE = "SQUIRT";
const std::string JENNY_SHAPE = "JENNY";
const std::string CHARLIE_SHAPE = "CHARLIE";
const std::string BLOAT_SHAPE = "BLOAT";
const std::string MARLIN_SHAPE = "MARLIN";
const std::string GURGLE_SHAPE = "GURGLE";
const std::string TREE_CORAL_SHAPE = "TREE_CORAL";
const std::string SOFT_CORAL_SHAPE = "SOFT_CORAL";
const std::string ELKHORN_CORAL_SHAPE = "ELKHORN_CORAL";
const std::string FLOOR_SHAPE = "FLOOR_SHAPE";
const std::string ENEMY_SHAPE = "ENEMY";

// Game Constants
constexpr float INITIAL_TIME_LIMIT = 100.0f; // seconds
constexpr int GAME_LOST = -1;
constexpr int GAME_ACTIVE = 0;
constexpr int GAME_WON = 1;
constexpr float WARNING_TIME = 15.0f;
constexpr float SCENE_CHAR_DELAY = 0.1f;
constexpr float SCENE_TEXT_DELAY = 1.0f;

// UI Constants
constexpr float WINDOW_WIDTH = 2048;
constexpr float WINDOW_HEIGHT = 1024;
constexpr float WINDOW_ASPECT = WINDOW_WIDTH / WINDOW_HEIGHT;

const glm::vec3 UI_COLOR = glm::vec3(0.99f, 0.86f, 0.01f);
const glm::vec3 UI_RED_COLOR = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 UI_GREEN_COLOR = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 UI_YELLOW_COLOR = glm::vec3(0.9f, 0.5f, 0.0f);

constexpr float UI_LINE_OFFSET = 50.0f;
constexpr float UI_BOTTOM_MARGIN = 25.0f;
constexpr float UI_LEFT_MARGIN = 25.0f;

constexpr float UI_FONT_SIZE = 0.75f;
constexpr float TITLE_FONT_SIZE = 2.5f;
constexpr float SCENE_FONT_SIZE = 2.0f;

// Player
constexpr float PLAYER_SPEED = 25;
constexpr float PLAYER_SIZE = 4;
constexpr int PLAYER_MATERIAL = 5;
constexpr float INITIAL_STAMINA = 30;
constexpr float MAX_STAMINA = 100;
constexpr float RECOVERY_SPEED = 0.5f;
constexpr float BLUR_INCREMENT = 16.0f;
constexpr float IMMUNITY_TIME = 6.0f; // seconds

// Camera
constexpr float CAMERA_SPEED = 3;
constexpr float MOUSE_SENSITIVITY = 0.03;
const glm::vec2 FIRST_PERSON_OFFSET = glm::vec2(PLAYER_SIZE * 1.5, 0);
const glm::vec2 SECOND_PERSON_OFFSET = glm::vec2(PLAYER_SIZE + 10, PLAYER_SIZE + 0.3);
const glm::vec2 THIRD_PERSON_OFFSET = glm::vec2(-SECOND_PERSON_OFFSET.x, SECOND_PERSON_OFFSET.y);
constexpr float CAMERA_FLOOR_OFFSET = 1.0f;

constexpr float MAX_VIEW = 80.0 / 180 * M_PI;

// Tile Constants
constexpr int MAP_X = 256;
constexpr int MAP_Y = 256;
constexpr int MAP_Z = 256;
constexpr int MAP_I = 20;
constexpr int MAP_J = 20;
constexpr int MAP_K = 20;
constexpr float REPEAT_FACTOR = 4.0;
const glm::vec3 FLOOR_SIZE = glm::vec3(2, 1, 2);
const glm::vec3 FLOOR_POSITION = glm::vec3((-MAP_X / 2) * FLOOR_SIZE.x, -200, (-MAP_Z / 2) * FLOOR_SIZE.z);
constexpr float WORLD_SIZE = 255;

// Follower Constants
constexpr float MAX_SPAWN_DISTANCE = 200;
constexpr float MAX_SPAWN_VELOCITY = 15;
constexpr float MIN_SPAWN_SIZE = 3;
constexpr float MAX_SPAWN_SIZE = 6;
constexpr int DEFAULT_MATERIAL = 3;
constexpr float FOLLOWER_SPEED = 40;
constexpr float FOLLOWER_OFFSET = 10.0f;
constexpr float FOLLOWER_FLOOR_OFFSET = 10.0f;
constexpr float POWERUP_OFFSET = 7.0;

// Powerup Constants
constexpr float POWERUP_SIZE = 1.7;
constexpr int POWERUP_MATERIAL = 5;
constexpr float POWERUP_DESPAWN_TIME = 50;
constexpr float STAMINA_INCREMENT = 10;

// Coral Constants
constexpr int NUM_CORAL = 100;
constexpr int TREE_CORAL_MATERIAL = 10;
constexpr int SOFT_CORAL_MATERIAL = 6;
constexpr int ELKHORN_CORAL_MATERIAL = 9;
constexpr float CORAL_FLOOR_OFFSET = 1.0;

const std::string CORAL_SHAPES[]{
	TREE_CORAL_SHAPE,
	SOFT_CORAL_SHAPE,
	ELKHORN_CORAL_SHAPE
};

const int CORAL_MATERIALS[]{
	TREE_CORAL_MATERIAL,
	SOFT_CORAL_MATERIAL,
	ELKHORN_CORAL_MATERIAL
};

// Enemy Constants
constexpr float ENEMY_SIZE = 3;
constexpr int ENEMY_MATERIAL = 7;
const glm::vec2 ENEMY_FLOOR_OFFSET_RANGE = glm::vec2(8, 15);
const glm::vec2 ENEMY_TIMER_RANGE = glm::vec2(5, 10);
constexpr int NUM_ENEMIES = 10;

// Character Constants
struct Character {
	std::string shape;
	std::string texture;
	float size;
};

constexpr int NUM_CHARACTERS = 7;
const Character CHARACTERS[]{
	{MARLIN_SHAPE, MARLIN_TEXTURE, 4},
	{NEMO_SHAPE, NEMO_TEXTURE, 2},
	{SQUIRT_SHAPE, SQUIRT_TEXTURE, 4},
	{BLOAT_SHAPE, BLOAT_TEXTURE, 5},
	{GURGLE_SHAPE, GURGLE_TEXTURE, 3},
	{JENNY_SHAPE, JENNY_TEXTURE, 4},
	{CHARLIE_SHAPE, CHARLIE_TEXTURE, 4},
};

// Caustics Constants
constexpr int NUM_CAUSTICS = 32;

// Shader Constants
constexpr int NUM_SHADERS = 12;
constexpr int SIMPLEPROG = 0;
constexpr int SKYBOXPROG = 1;
constexpr int TEXTUREPROG = 2;
constexpr int GLYPHPROG = 3;
constexpr int FLOORPROG = 4;
constexpr int REFLECTPROG = 5;
constexpr int DEPTHPROG = 6;
constexpr int FOGFBOPROG = 7;
constexpr int BLURFBOPROG = 8;
constexpr int WATERFBOPROG = 9;
constexpr int PARTICLEPROG = 10;
constexpr int LIGHTDEPTHPROG = 11;

// Light Constants
struct Light {
	glm::vec3 pos;
	float constant;
	float linear;
	float quadratic;
};

constexpr int NUM_LIGHTS = 3;
const struct Light POINT_LIGHTS[NUM_LIGHTS] = {
	{ glm::vec3(1, 90, 0), 1.0, 0.007, 0.0002 },
	{ glm::vec3(100, 70, -100), 1.0, 0.007, 0.0002 },
	{ glm::vec3(100, 50, 150), 1.0, 0.007, 0.0002 }
};

// Freetype Constants
constexpr int LEFT = 0;
constexpr int CENTER = 1;

// Particle Constants
constexpr int NUM_PARTICLES = 100;

// Light Uniform Constants
constexpr float ORTHO_SIZE = 256.0f;
constexpr float NEAR_PLANE = 0.1f;

// Scene Texts
const std::vector<std::string> SCENE1TEXT{
	"Ugh!",
	"Where am I?",
	"I need to find my friends!"
};
