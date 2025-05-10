#pragma once
#include <vector>
#include "Button.h"

class Game
{
    Button StartContinue{ { 200, 200 }, {300.f, 80.f}, 0 };
    Button Exit{ {StartContinue.GetLocation().X, StartContinue.GetLocation().Y + StartContinue.GetSize().Y * 2}, 
        StartContinue.GetSize(), 1};

    enum class Inter
    {
        Menu,
        Game,
        PauseMenu,
        GameOver
    };

public:
    Game()
    {
    }
    void Moving();
    void gameLoop(float dt);
    void gameDraw();
    void GameMenu();
    void Control();
    Inter GameMode = Inter::Menu;
};