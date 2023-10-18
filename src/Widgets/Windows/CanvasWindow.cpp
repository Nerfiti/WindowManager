#include "CanvasWindow.hpp"

CanvasWindow::CanvasWindow(sf::Vector2f pos, sf::Vector2f size, float frameHeight, ToolPalette &palette, sf::Color color):
    FrameWindow(pos, size, frameHeight, color),
    canvas_(sf::Vector2f(1, 1), palette),
    toolWidget_(palette)
    {}

CanvasWindow::CanvasWindow(float posX, float posY, float width, float height, float frameHeight, ToolPalette &palette, sf::Color color):
    CanvasWindow(sf::Vector2f(posX, posY), sf::Vector2f(width, height), frameHeight, palette, color)
    {}

void CanvasWindow::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    sf::Transform finalTransform = parentTransform * transform_;

    FrameWindow::draw(canvas, parentTransform);
    canvas_.draw(canvas, finalTransform);
}

bool CanvasWindow::onMousePressed(sf::Mouse::Button key)
{
    if (FrameWindow::onMousePressed(key))
    {
        canvas_.onMousePressed(key);
        return true;
    }

    return false;
}

bool CanvasWindow::onMouseMoved(int x, int y, const sf::Transform &parentTransform)
{
    if (FrameWindow::onMouseMoved(x, y, parentTransform))
    {
        canvas_.onMouseMoved(x, y, parentTransform * transform_);
        return true;
    }

    return false;
}

bool CanvasWindow::onMouseReleased(sf::Mouse::Button key)    
{
    if (FrameWindow::onMouseReleased(key))
    {
        canvas_.onMouseReleased(key);
        return true;
    }

    return false;
}

bool CanvasWindow::onKeyboardPressed(sf::Keyboard::Key key)
{
    if (!FrameWindow::onKeyboardPressed(key) && !toolWidget_.onKeyboardPressed(key))
    {
        canvas_.onKeyboardPressed(key);
        return true;
    }

    return false;
}

bool CanvasWindow::onKeyboardReleased(sf::Keyboard::Key key)
{
    if (!FrameWindow::onKeyboardReleased(key) && !toolWidget_.onKeyboardReleased(key))
    {
        canvas_.onKeyboardReleased(key);
        return true;
    }

    return false;
}

bool CanvasWindow::onTime(float deltaSeconds)   
{
    FrameWindow::onTime(deltaSeconds);
    canvas_.onTime(deltaSeconds);
    
    return false;
}

//------------------------------------------------------------------