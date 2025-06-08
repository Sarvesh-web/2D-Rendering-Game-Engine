#include "Game.h"
#include "../ECS/ECS.h"
#include "../LogManager/LogManager.h"
#include <iostream>
Game::Game()
{
	bIsRunning = false;
	window = nullptr;
	renderer = nullptr;
	millisecsPreviousFrame = 0;

	//create a registry 
	registry = std::make_unique<Registry>();

	LOG(LM_Info, "Game constructor called!");
}

Game::~Game()
{
	LOG(LM_Info, "Game Destructor called!");
}

void Game::Initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) //returns zero if its success
	{
		LOG(LM_Error, "Error Initializing SDL.");
		return;
	}
	const char* GameName = "My Game";
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = displayMode.w;
	windowHeigth = displayMode.h;
	window = SDL_CreateWindow(GameName,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeigth,
		SDL_WINDOW_BORDERLESS
		);
	if (!window)
	{
		LOG(LM_Error, "Error in creating window.");
		return;
	}
	Uint32 rendererflag = SDL_RENDERER_ACCELERATED;
	if (bVSync)
		rendererflag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	renderer = SDL_CreateRenderer(window, -1, rendererflag);
	if (!renderer)
	{
		LOG(LM_Error, "Error in creating SDL_Renderer.");
		return;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	bIsRunning = true;
}

void Game::ProcessInput()
{
	SDL_Event sdlEvent;
	//this will get true whenever event is active 
	//its not true everytime so it won't hold the cpu
	while (SDL_PollEvent(&sdlEvent)) 
	{
		switch (sdlEvent.type) 
		{
		case SDL_QUIT:
			bIsRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				bIsRunning = false;
			}
			break;
		default:
			break;
		}
	}
}

void Game::Setup()
{

	//create some entities
	Entity tank = registry->CreateEntity();

	//Add some components to the entity
	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0,1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(50.0f, 0.0f));

	//remove some components to the entity
	tank.RemoveComponent<TransformComponent>();


}
void Game::Update()
{
	if (!bUncappedFrames)
	{
		// we are too fast so need to maintain a delay of fixed time stamp value in the update
		//while (!SDL_TICKS_PASSED(SDL_GetTicks(), millisecsPreviousFrame + MILLISECS_PER_FRAME)); //commented as it holds the cpu threads because of while loop
		int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame); //converting deltatime into seconds as timeToWait is in Seconds
		//SDL_DELAY called is based on Delegate to stop SDL operations so it solves the holding of cpu threads utiliztion
		if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
			SDL_Delay(timeToWait);
	}
	// TODO: Update game objects..
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame)/1000.0; // time to wait   in miliseconds
	// stor the previous frame time
	millisecsPreviousFrame = SDL_GetTicks();

	//TODO: 
	// MovementSystem.Update();
	// CollisionSystem.Update();
	// DamageSystem.Update();
	//playerPosition.x += playerVelocity.x* deltaTime;
	//playerPosition.y += playerVelocity.y* deltaTime;
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);
	//TODO: Render all GameObjects
	//draw a PNG
	//const char* imgPath = "./assets/images/tank-tiger-right.png";
	//SDL_Surface* surface=IMG_Load(imgPath);
	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
	//SDL_FreeSurface(surface);
	//SDL_Rect destRect = { 
	//	static_cast<int>(playerPosition.x),
	//	static_cast<int>(playerPosition.y),
	//	32,
	//	32
	//};
	//SDL_RenderCopy(renderer, texture, NULL, &destRect);
	//SDL_DestroyTexture(texture);
	//SDL_RenderFillRect(renderer, &player);
	// sdl is based on double buffer render system
	SDL_RenderPresent(renderer);
}

void Game::Run()
{
	Setup();
	//main thing is to fix time stamp for better physics simulations
	//while loop was an inconsistent solution
	while (bIsRunning)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

}