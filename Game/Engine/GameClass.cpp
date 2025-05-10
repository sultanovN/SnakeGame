#include "GameClass.h"
#include "Engine.h"
#include "RandomNumber.h"
#include "GTimer.h"
//void Game::GameMenu()
//{
//    if (GameMode == Inter::Menu && StartContinue.isClicking({ get_cursor_x() * 1.f, get_cursor_y() * 1.f },
//        is_mouse_button_pressed(0) || is_key_pressed(13)))
//    {
//        ReSpawn();
//        GameMode = Inter::Game;
//    }
//    else if (GameMode == Inter::PauseMenu && StartContinue.isClicking({ get_cursor_x() * 1.f, get_cursor_y() * 1.f },
//        is_mouse_button_pressed(0) || is_key_pressed(13)))
//    {
//        GameMode = Inter::Game;
//    }
//    else if ((GameMode == Inter::Menu || GameMode == Inter::PauseMenu) && Exit.isClicking({ get_cursor_x() * 1.f, get_cursor_y() * 1.f },
//        is_mouse_button_pressed(0) || is_key_pressed(13)))
//    {
//        schedule_quit_game();
//    }
//}

uint8_t playerLength = 4;
uint8_t playerLocationX = 4;
uint8_t playerLocationY = 4;
std::chrono::milliseconds stepTime = std::chrono::milliseconds(450);

bool Started = false;
bool GameOver = false;

enum Direction
{
    UP,
    DOWN,
    RIGHT,
    LEFT
};

Direction playerDirection = UP;

//x,y for each cel of Snake
//Every move last cel moves to the cel ahead of him

struct Vec
{
    int X;
    int Y;

    void operator=(Vec vec) 
    { 
        this->X = vec.X; 
        this->Y = vec.Y; 
    }
};

std::vector<Vec> SnakeCels;
const int rows = 20;
const int cols = 20;
bool mat[cols][rows];
int cellSize = 20;
int borderSize = 3;

Vec Food;

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
    int X = RandomNumberInRange(0, cols);
    int Y = RandomNumberInRange(0, rows);
    for (auto& cel : SnakeCels)
    {
        if (X == cel.X && Y == cel.Y)
        {
            FoodSpawn();
        }

    }
    Food.X = X;
    Food.Y = Y;
}

bool FoodGetEaten()
{
    if (SnakeCels.at(0).X == Food.X && SnakeCels.at(0).Y == Food.Y)
    {
        SnakeCels.emplace_back(Vec{ SnakeCels.back().X + SnakeCels.back().X - SnakeCels.at(SnakeCels.size() - 2).X, 
            SnakeCels.back().Y});
        if (stepTime >= std::chrono::milliseconds(50))
        {
            stepTime -= std::chrono::milliseconds(50);
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

    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            mat[i][j] = 0;
        }
    }
    for (int8_t i = 8; i > 4; i--)
    {
        SnakeCels.emplace_back(Vec{i, 4 });
        mat[i][4] = 1;
    }
    FoodSpawn();
}

void Control()
{
    
    if (is_key_pressed(VK_UP) && playerDirection != DOWN)
    {
        playerDirection = UP;
    }
    else if (is_key_pressed(VK_DOWN) && playerDirection != UP)
    {
        playerDirection = DOWN;
    }
    else if (is_key_pressed(VK_RIGHT) && playerDirection != LEFT)
    {
        playerDirection = RIGHT;
    }
    else if (is_key_pressed(VK_LEFT) && playerDirection != RIGHT)
    {
        playerDirection = LEFT;
    }
}

void Game::Moving()
{
    //control each up --j, right i++ etc
    // cel behind follows
    //i == X 
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
        GameOver = true;
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
                    (y * cellSize) + ((y + 1) * borderSize), cellSize, cellSize, 0x00FF00);
            }
            else if (mat[x][y] == 0)
            {
                DrawRectangle((x * cellSize) + ((x + 1) * borderSize),
                    (y * cellSize) + ((y + 1) * borderSize), cellSize, cellSize, 0x000000);
            }
            if (x == Food.X && y == Food.Y)
            {
                DrawRectangle((x * cellSize) + ((x + 1) * borderSize),
                    (y * cellSize) + ((y + 1) * borderSize), cellSize, cellSize, 0xFF0000);
            }
        }
    }
}


// Input does not register all time
void Game::gameLoop(float dt)
{
    if (!GameOver)
    {
        if (!Started)
        {
            Started = true;
            StartGrid();
        }
        Control();

        

        Vec temp = SnakeCels.at(0);
        if (DidTimerEnd(timer, stepTime))
        {
            //moving first cel
            Moving();

            //Food
            FoodGetEaten();

            for (int cel = 1; cel < SnakeCels.size(); cel++)
            {
                //tail following
                if (SnakeCels.at(0).X == SnakeCels.at(cel).X && 
                    SnakeCels.at(0).Y == SnakeCels.at(cel).Y)
                {
                    GameOver = true;
                    break;
                }
                if (cel == SnakeCels.size() - 1)
                {
                    mat[SnakeCels.at(cel).X][SnakeCels.at(cel).Y] = 0;
                }
                std::swap(temp, SnakeCels.at(cel));
                StartTimer(timer);
                /*if (x == SnakeCels.at(cel).X && y == SnakeCels.at(cel).Y)
                {
                    mat[x][y] = 1;
                }
                else
                {
                    mat[x][y] = 0;
                }*/
            }
        }
        
    }
    

    

    /*switch (GameMode)
    {
    case Inter::Menu:
    {
        GameMenu();
        break;
    }
    case Inter::Game:
    {
    }
    case Inter::PauseMenu:
    {
        GameMenu();
        break;
    }
    case Inter::GameOver:
    {
        break;
    }
    default:
        break;
    }*/

    
}


void Game::gameDraw()
{
    // clear backbuffer
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

    //Background color
    DrawRectangle(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFFFFFF);
    drawGrid();
    /*switch (GameMode)
    {
    case Inter::Game:
    {
       
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
    }*/
}