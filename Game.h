#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Text_Renderer.h"
#include "Texture.h"
#include "Scene.h"

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
    unsigned int            Level;
    unsigned int            Points;
    float lastX, lastY;
    bool firstMouse = true;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void Update(float dt);
    void Render(float dt);
    void KeyboardInput(float dt);
    void MouseInput(float xpos, float ypose);
    // Game State
    void ResetGame();
    bool IsCompleted();

private:
    void checkInfractions();
};

#endif