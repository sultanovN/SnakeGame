#include "GameClass.h"
#include "Engine.h"
#include "RandomNumber.h"
#include "GTimer.h"


enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT
};

//x,y for each cel of Snake

struct MatPos
{
    int X;
    int Y;
};
std::chrono::milliseconds stepTime;
Direction playerDirection = UP;
std::vector<MatPos> SnakeCels;
const int rows = 20;
const int cols = 20;
bool mat[cols][rows];
int cellSize = 20;
int borderSize = 3;

MatPos Food;

//int countNeighbours(int x, int y)
//{
//    int count = 0;
//    for (int i = x - 1; i < x + 2; i++)
//        for (int j = y - 1; j < y + 2; j++)
//        {
//            if ((i != x || j != y) && (i >= 0 && i < cols) && (j >= 0 && j < rows))
//            {
//                count += mat[i][j];
//            }
//        }
//    return count;
//}

void FoodSpawn()
{
    int X = RandomNumberInRange(0, cols - 1);
    int Y = RandomNumberInRange(0, rows - 1);
    for (int i = 0; i < SnakeCels.size(); i++)
    {
        if (X == SnakeCels.at(i).X && Y == SnakeCels.at(i).Y)
        {
            X = RandomNumberInRange(0, cols - 1);
            Y = RandomNumberInRange(0, rows - 1);
            i = 0;
        }
    }
    Food.X = X;
    Food.Y = Y;
}

bool FoodGetEaten()
{
    if (SnakeCels.at(0).X == Food.X && SnakeCels.at(0).Y == Food.Y)
    {
        SnakeCels.emplace_back(MatPos{ SnakeCels.back().X + SnakeCels.back().X - SnakeCels.at(SnakeCels.size() - 2).X,
            SnakeCels.back().Y});
        if (stepTime >= std::chrono::milliseconds(100))
        {
            stepTime = stepTime - std::chrono::milliseconds(10);
        }
        FoodSpawn();
        return true;
    }
    return false;
}

std::chrono::steady_clock::time_point timer;
void StartGrid()
{
    SnakeCels.clear();
    playerDirection = RIGHT;
    stepTime = std::chrono::milliseconds(350);
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            mat[i][j] = 0;
        }
    }
    for (int8_t i = 8; i > 5; i--)
    {
        SnakeCels.emplace_back(MatPos{i, 4 });
        mat[i][4] = 1;
    }
    FoodSpawn();
}

void Game::GameMenu()
{
    if ((GameMode == Inter::Menu || GameMode == Inter::GameOver) && StartContinue.isClicking({ get_cursor_x() * 1.f, get_cursor_y() * 1.f },
        is_mouse_button_pressed(0) || is_key_pressed(13)))
    {
        GameMode = Inter::Game;
        StartGrid();
    }
    else if (GameMode == Inter::PauseMenu && StartContinue.isClicking({ get_cursor_x() * 1.f, get_cursor_y() * 1.f },
        is_mouse_button_pressed(0) || is_key_pressed(13)))
    {
        GameMode = Inter::Game;
    }
    else if ((GameMode == Inter::Menu || GameMode == Inter::PauseMenu || GameMode == Inter::GameOver) && Exit.isClicking({ get_cursor_x() * 1.f, get_cursor_y() * 1.f },
        is_mouse_button_pressed(0) || is_key_pressed(13)))
    {
        schedule_quit_game();
    }
}

// temporary solution, 
void Game::Control()
{
    if (is_key_pressed(VK_ESCAPE))
    {
        GameMode = Inter::PauseMenu;
    }
    if (is_key_pressed(VK_UP) && !is_key_pressed(VK_LEFT) && !is_key_pressed(VK_RIGHT) && playerDirection != DOWN && playerDirection != UP)
    {
        playerDirection = UP;
        Moving();
        StartTimer(timer);
    }
    else if (is_key_pressed(VK_DOWN) && !is_key_pressed(VK_LEFT) && !is_key_pressed(VK_RIGHT) && playerDirection != UP && playerDirection != DOWN)
    {
        playerDirection = DOWN;
        Moving();
        StartTimer(timer);
    }
    else if (is_key_pressed(VK_RIGHT) && !is_key_pressed(VK_UP) && !is_key_pressed(VK_DOWN) && playerDirection != LEFT && playerDirection != RIGHT)
    {
        playerDirection = RIGHT;
        Moving();
        StartTimer(timer);
    }
    else if (is_key_pressed(VK_LEFT) && !is_key_pressed(VK_UP) && !is_key_pressed(VK_DOWN) && playerDirection != RIGHT && playerDirection != LEFT)
    {
        playerDirection = LEFT;
        Moving();
        StartTimer(timer);
    }
    else
    {
        if (DidTimerEnd(timer, stepTime))
        {
            Moving();
            StartTimer(timer);
        }
    }
    FoodGetEaten();

}

void Game::Moving()
{
    MatPos temp = SnakeCels.at(0);

    for (int cel = 1; cel < SnakeCels.size(); cel++)
    {

        if (cel == SnakeCels.size() - 1)
        {
            mat[SnakeCels.at(cel).X][SnakeCels.at(cel).Y] = 0;
        }

        // Snake collision with itself
        if (SnakeCels.at(0).X == SnakeCels.at(cel).X &&
            SnakeCels.at(0).Y == SnakeCels.at(cel).Y)
        {
            GameMode = Inter::GameOver;
            break;
        }

        //tail following
        //Every move last cel moves to the cel ahead of it
        std::swap(temp, SnakeCels.at(cel));
        StartTimer(timer);
    }
    switch (playerDirection)
    {
    case UP:
    {
        SnakeCels.at(0).Y--;
        mat[SnakeCels.at(0).X][SnakeCels.at(0).Y] = 1;
        break;
    }
    case DOWN:
    {
        SnakeCels.at(0).Y++;
        mat[SnakeCels.at(0).X][SnakeCels.at(0).Y] = 1;
        break;
    }
    case RIGHT:
    {
        SnakeCels.at(0).X++;
        mat[SnakeCels.at(0).X][SnakeCels.at(0).Y] = 1;
        break;
    }
    case LEFT:
    {
        SnakeCels.at(0).X--;
        mat[SnakeCels.at(0).X][SnakeCels.at(0).Y] = 1;
        break;
    }
    default:
    {
        break;
    }
    }
    if (SnakeCels.at(0).X < 0 || SnakeCels.at(0).X >= cols || SnakeCels.at(0).Y < 0
        || SnakeCels.at(0).Y >= rows)
    {
        GameMode = Inter::GameOver;
    }
    
}

void drawGrid()
{
    for (int x = 0; x < cols; x++)
    {
        for (int y = 0; y < rows; y++)
        {
            if (mat[x][y] == 1)
            {
                DrawRectangle((x * cellSize) + ((x + 1) * borderSize),
                    (y * cellSize) + ((y + 1) * borderSize), cellSize, cellSize, Colors::Green);
            }
            if (x == SnakeCels.at(0).X && y == SnakeCels.at(0).Y && mat[x][y] == 1)
            {
                DrawRectangle((x * cellSize) + ((x + 1) * borderSize),
                    (y * cellSize) + ((y + 1) * borderSize), cellSize, cellSize, 0x00BB00);
            }
            else if (mat[x][y] == 0)
            {
                DrawRectangle((x * cellSize) + ((x + 1) * borderSize),
                    (y * cellSize) + ((y + 1) * borderSize), cellSize, cellSize, 0x000000);
            }
            if (x == Food.X && y == Food.Y)
            {
                DrawRectangle((x * cellSize) + ((x + 1) * borderSize),
                    (y * cellSize) + ((y + 1) * borderSize), cellSize, cellSize, Colors::Red);
            }
        }
    }
}


// Input does not register sometimes
void Game::gameLoop(float dt)
{
    switch (GameMode)
    {
    case Inter::Menu:
    {
        GameMenu();
        break;
    }
    case Inter::Game:
    {
        Control();

        

        break;
    }
    case Inter::PauseMenu:
    {
        GameMenu();
        break;
    }
    case Inter::GameOver:
    {
        GameMenu();
        break;
    }
    }
    //if (!GameOver)
    //{
    //    /*if (!Started)
    //    {
    //        Started = true;
    //        StartGrid();
    //    }*/
    //    
    //}
}


void Game::gameDraw()
{
    // clear backbuffer
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

    //Background color
    DrawRectangle(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFFFFFF);

    switch (GameMode)
    {
    case Inter::Game:
    {
        drawGrid();
        break;
    }
    case Inter::Menu:
    {
        StartContinue.Draw();
        Exit.Draw();
        break;
    }
    case Inter::PauseMenu:
    {
        StartContinue.Draw();
        Exit.Draw();
        break;
    }
    case Inter::GameOver:
    {
        drawGrid();
        StartContinue.Draw();
        Exit.Draw();
        break;
    }
    }
}

