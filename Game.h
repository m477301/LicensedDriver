#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include "Game_Level.h"
#include <iostream>

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

enum Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Initial size of the player paddle
//const glm::vec3 ROAD_SIZE(100.0f, 100.0f,0.0f);
//const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);

// Initial velocity of the player paddle
const float ROAD_VELOCITY(0.0f);

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    bool                    KeysProcessed[1024];
    unsigned int            Width, Height, Depth;
    float lastX, lastY;
    bool firstMouse = true;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void Update(float dt);
    void Render();
    void KeyboardInput(float dt);
    void MouseInput(float xpos, float ypose);
    void ScrollInput(float yoffset);

};

#endif