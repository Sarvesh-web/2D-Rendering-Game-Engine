#include <iostream>
#include "Game.h"
int main(int argc, char* argv[]) 
{
    Game game;
    game.Initialize();
    game.Run();
    game.Destroy();
   /* while (true) 
    {
        ProcessInput();
        Update();
        Render();
    }*/
    return 0;
}