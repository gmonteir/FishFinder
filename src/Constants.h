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
const std::string DORY_TEXTURE = "DORY";
const std::string CUBE_SHAPE = "CUBE";
const std::string DORY_SHAPE = "DORY";
const std::string NEMO_SHAPE = "NEMO";
const std::string TREE_CORAL_SHAPE = "TREE_CORAL";
const std::string SOFT_CORAL_SHAPE = "SOFT_CORAL";
const std::string ELKHORN_CORAL_SHAPE = "ELKHORN_CORAL";
const std::string RESOURCE_DIR = "../resources";

// Tags
const std::string NEMO_TAG = "NEMO";
const std::string POWERUP_TAG = "POWERUP";
const std::string CORAL_TAG = "CORAL";
// Player
constexpr float PLAYER_SPEED = 20;
constexpr float PLAYER_SIZE = 3;
constexpr int PLAYER_MATERIAL = 5;
constexpr float INITIAL_STAMINA = 3;
constexpr float MAX_STAMINA = 10;

constexpr float CAMERA_SPEED = 5;
const glm::vec2 FIRST_PERSON_OFFSET = glm::vec2(PLAYER_SIZE * 1.5, 0);
const glm::vec2 SECOND_PERSON_OFFSET = glm::vec2(PLAYER_SIZE + 10, PLAYER_SIZE + 0.3);
const glm::vec2 THIRD_PERSON_OFFSET = glm::vec2(-SECOND_PERSON_OFFSET.x, SECOND_PERSON_OFFSET.y);

const glm::vec3 FLOOR_POSITION = glm::vec3(0, -2, 0);
const glm::vec3 FLOOR_SIZE = glm::vec3(60, .05, 60);

constexpr float MAX_VIEW = 80.0 / 180 * M_PI;

// Entity Constants
constexpr int MAX_SPAWN_ENTITIES = 30;
constexpr float MAX_SPAWN_DISTANCE = 50.0;
constexpr float MAX_SPAWN_VELOCITY = 15;
constexpr float MIN_SPAWN_SIZE = 1;
constexpr float MAX_SPAWN_SIZE = 3;
constexpr float WORLD_SIZE = 55.0;
constexpr int DEFAULT_MATERIAL = 3;
constexpr int STOPPED_MATERIAL = 0;

// Powerup Constants
constexpr float POWERUP_SIZE = 0.7;
constexpr int POWERUP_MATERIAL = 5;
constexpr float POWERUP_DESPAWN_TIME = 10.0;

// Tile Constants
constexpr int MAP_X = 32;
constexpr int MAP_Z = 32;

// Coral Constants
constexpr int NUM_CORAL = 20;
constexpr int TREE_CORAL_INDEX = 0;
constexpr int SOFT_CORAL_INDEX = 1;
constexpr int ELKHORN_CORAL_INDEX = 2;
constexpr int TREE_CORAL_MATERIAL = 8;
constexpr int SOFT_CORAL_MATERIAL = 6;
constexpr int ELKHORN_CORAL_MATERIAL = 7;