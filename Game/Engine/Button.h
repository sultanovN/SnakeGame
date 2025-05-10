#pragma once
#include "../Entity/Entity.h"

//create button order function
class Button : public Entity
{
    uint8_t buttonOrderNum;
public:
    Button(Vector2D Location, Vector2D Size, uint8_t buttonOrderNum, uint32_t Color = Colors::Red)
        : Entity(Location, Size, 0.f, Color), buttonOrderNum(buttonOrderNum)
    {
    }

    bool isHovering(Vector2D mouseLoc)
    {
        if (RectRectCollision(Location, Size, mouseLoc, { 0.f, 0.f }))
        {
            return true;
        }
        return false;
    }

    //bSelect == (is_mouse_button_pressed(0) || is_key_pressed(VK_RETURN), mouse location: get_cursor_x(), get_cursor_y()
    bool isClicking(Vector2D mouseLoc, bool bSelect)
    {
        if (isHovering(mouseLoc) && bSelect)
        {
            return true;
        }
        return false;
    }
};