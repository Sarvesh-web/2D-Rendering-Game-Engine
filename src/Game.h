#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
class Game
{
private:
	//...
	bool bIsRunning = false;
	SDL_Window* window;
	SDL_Renderer* renderer;
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
};
