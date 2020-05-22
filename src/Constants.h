#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "GLSL.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

const glm::vec3 ORIGIN = glm::vec3(0);
const glm::vec3 XAXIS = glm::vec3(1.0, 0.0, 0.0);
const glm::vec3 YAXIS = glm::vec3(0.0, 1.0, 0.0);
const glm::vec3 ZAXIS = glm::vec3(0.0, 0.0, 1.0);

// Assets
const std::string DORY_TEXTURE = "DORY_TXT";
const std::string NEMO_TEXTURE = "NEMO_TXT";
const std::string SQUIRT_TEXTURE = "SQUIRT_TXT";
const std::string CAUSTIC_TEXTURE = "CAUSTIC_TXT";
const std::string FLOOR_TEXTURE = "FLOOR_TXT";

const std::string CUBE_SHAPE = "CUBE";
const std::string DORY_SHAPE = "DORY";
const std::string NEMO_SHAPE = "NEMO";
const std::string SQUIRT_SHAPE = "SQUIRT";
const std::string TREE_CORAL_SHAPE = "TREE_CORAL";
const std::string SOFT_CORAL_SHAPE = "SOFT_CORAL";
const std::string ELKHORN_CORAL_SHAPE = "ELKHORN_CORAL";
const std::string FLOOR_SHAPE = "FLOOR_SHAPE";
const std::string ENEMY_SHAPE = "CUBE";
const std::string RESOURCE_DIR = "../resources";

// Game Constants
constexpr int NUM_CHARACTERS = 7;
constexpr float INITIAL_TIME_LIMIT = 10.0f; // seconds

// UI Constants
constexpr float WINDOW_WIDTH = 2048;
constexpr float WINDOW_HEIGHT = 1024;
constexpr float WINDOW_ASPECT = WINDOW_WIDTH / WINDOW_HEIGHT;

const glm::vec3 UI_COLOR = glm::vec3(0.2f, 1.0f, 0.2f);
constexpr float UI_LINE_OFFSET = 50.0f;
constexpr float UI_BOTTOM_MARGIN = 25.0f;
constexpr float UI_LEFT_MARGIN = 25.0f;
constexpr float UI_FONT_SIZE = 0.75f;

constexpr float TITLE_FONT_SIZE = 2.5f;

// Player
constexpr float PLAYER_SPEED = 25;
constexpr float PLAYER_SIZE = 3;
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

// Entity Constants
constexpr int MAX_SPAWN_ENTITIES = 50;
constexpr float MAX_SPAWN_DISTANCE = 200;
constexpr float MAX_SPAWN_VELOCITY = 15;
constexpr float MIN_SPAWN_SIZE = 3;
constexpr float MAX_SPAWN_SIZE = 6;
constexpr int DEFAULT_MATERIAL = 3;
constexpr int STOPPED_MATERIAL = 0;
constexpr float FLOOR_OFFSET = 1.0;
constexpr float FOLLOWER_OFFSET = 10.0;
constexpr float POWERUP_OFFSET = 7.0;

// Powerup Constants
constexpr float POWERUP_SIZE = 0.7;
constexpr int POWERUP_MATERIAL = 5;
constexpr float POWERUP_DESPAWN_TIME = 50;
constexpr float STAMINA_INCREMENT = 20;

// Coral Constants
constexpr int NUM_CORAL = 50;
constexpr int TREE_CORAL_INDEX = 0;
constexpr int SOFT_CORAL_INDEX = 1;
constexpr int ELKHORN_CORAL_INDEX = 2;
constexpr int TREE_CORAL_MATERIAL = 10;
constexpr int SOFT_CORAL_MATERIAL = 6;
constexpr int ELKHORN_CORAL_MATERIAL = 9;

// Enemy Constants
constexpr int ENEMY_SIZE = 3;
constexpr int ENEMY_MATERIAL = 3;
constexpr int NUM_ENEMIES = 5;

// Nemo Constants
constexpr float NEMO_SPEED = 20;
constexpr float NEMO_SIZE = 2;
constexpr int NEMO_MATERIAL = 5;
constexpr float FOLLOWING_OFFSET = 10;

// Squirt Constants
constexpr float SQUIRT_SIZE = 3;

// Caustics Constants
constexpr int NUM_CAUSTICS = 32;

// Shader Constants
constexpr int NUM_SHADERS = 7;
constexpr int SIMPLEPROG = 0;
constexpr int SKYBOXPROG = 1;
constexpr int TEXTUREPROG = 2;
constexpr int GLYPHPROG = 3;
constexpr int FLOORPROG = 4;
constexpr int FBOPROG = 5;
constexpr int BLURPROG = 6;