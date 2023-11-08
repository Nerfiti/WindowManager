#include "CanvasWindow.hpp"

CanvasWindow::CanvasWindow(sf::Vector2f pos, sf::Vector2f size, float frameHeight, ToolPalette &tools, FilterPalette &filters, const char *filename, sf::Color color):
    FrameWindow(pos, size, frameHeight, color),
    canvas_(sf::Vector2f(0, frameHeight / size.y), sf::Vector2f(1, 1 - frameHeight / size.y), tools, filters, filename),
    toolWidget_(tools)
    {}

CanvasWindow::CanvasWindow(float posX, float posY, float width, float height, float frameHeight, ToolPalette &palette, FilterPalette &filters, const char *filename, sf::Color color):
    CanvasWindow(sf::Vector2f(posX, posY), sf::Vector2f(width, height), frameHeight, palette, filters, filename, color)
    {}

void CanvasWindow::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    sf::Transform finalTransform = parentTransform * transform_;

    FrameWindow::draw(canvas, parentTransform);
    canvas_.draw(canvas, finalTransform);
}

bool CanvasWindow::onMousePressed(sf::Mouse::Button key)
{
    if (!FrameWindow::onMousePressed(key))
        return false;

    if (frameCaptured_ || closeButtonCaptured_ || canvas_.onMousePressed(key))
    {
        canvas_.setIsFocused(true);
        return true;
    }

    return false;
}

bool CanvasWindow::onMouseMoved(int x, int y, const sf::Transform &parentTransform)
{
    if (!FrameWindow::onMouseMoved(x, y, parentTransform))
        return false;

    sf::Transform finalTransform = parentTransform * transform_;
    
    canvas_.onMouseMoved(x, y, finalTransform);

    return true;
}

bool CanvasWindow::onMouseReleased(sf::Mouse::Button key)    
{
    FrameWindow::onMouseReleased(key);

    canvas_.onMouseReleased(key);

    return true;
}

bool CanvasWindow::onKeyboardPressed(sf::Keyboard::Key key)
{
    if (FrameWindow::onKeyboardPressed(key) ||
         toolWidget_.onKeyboardPressed(key) ||
             canvas_.onKeyboardPressed(key)) 
        return true;

    return false;
}

bool CanvasWindow::onKeyboardReleased(sf::Keyboard::Key key)
{
    if (FrameWindow::onKeyboardReleased(key) ||
         toolWidget_.onKeyboardReleased(key) ||
             canvas_.onKeyboardReleased(key)) 
        return true;

    return false;
}

bool CanvasWindow::onTime(float deltaSeconds)   
{
    FrameWindow::onTime(deltaSeconds);
    canvas_.onTime(deltaSeconds);
    
    return false;
}

void CanvasWindow::setIsFocused(bool isFocused)
{
    isFocused_ = isFocused;
    canvas_.setIsFocused(isFocused);
}

//------------------------------------------------------------------