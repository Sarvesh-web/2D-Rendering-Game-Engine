#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>

const int Desired_FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / Desired_FPS;
class Game
{
private:
	//...
	bool bIsRunning = false;
	Uint32 millisecsPreviousFrame =0;
	SDL_Window* window;
	SDL_Renderer* renderer;

	std::unique_ptr<Registry> registry;
public:
	Game();
	~Game();
	void Initialize();
	void Setup();
	void Run();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();
	
	int windowWidth = 800;
	int windowHeigth = 600;
	bool bVSync = false;
	bool bUncappedFrames = true;
};
