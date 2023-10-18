#include "Canvas.hpp"

//----------------------------------------------------------------------

Canvas::Canvas(sf::Vector2f size, ToolPalette& palette):
    palette_(palette)
{
    const sf::Vector2u resolution(1920, 1080);
    texture_.create(resolution.x, resolution.y);
    sf::RectangleShape background(sf::Vector2f(resolution.x, resolution.y));
    background.setFillColor(*palette.getBackgroundColor());

    texture_.draw(background);
    texture_.display();

    rect_.setTexture(&texture_.getTexture());
    rect_.setSize(size);
}

void Canvas::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    canvas.draw(rect_, parentTransform);
    palette_.getActiveTool()->preview(canvas, parentTransform.transformPoint(mousePos_));
}

bool Canvas::onMousePressed(sf::Mouse::Button key)
{
    switch (key)
    {
        case MainButton:
            isMainButtonPressed_ = true;
            palette_.getActiveTool()->onMainButton(ButtonState::Pressed, mousePos_, *this);
            break;
        case SecondaryButton:
            palette_.getActiveTool()->onSecondaryButton(ButtonState::Pressed, mousePos_, *this);
            break;
    }

    return true;
}

bool Canvas::onMouseMoved(int x, int y, const sf::Transform &parentTransform)     
{
    mousePos_ = parentTransform.getInverse().transformPoint(sf::Vector2f(x, y));
    if (isMainButtonPressed_)
        palette_.getActiveTool()->onMove(mousePos_, *this);

    return true;
}

bool Canvas::onMouseReleased(sf::Mouse::Button key)
{
    switch (key)
    {
        case MainButton:
            isMainButtonPressed_ = false;
            palette_.getActiveTool()->onMainButton(ButtonState::Released, mousePos_, *this);
            break;
        case SecondaryButton:
            palette_.getActiveTool()->onSecondaryButton(ButtonState::Released, mousePos_, *this);
            break;
    }

    return true;
}

bool Canvas::onKeyboardPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
        case Modifier1Button:
            palette_.getActiveTool()->onModifier1(ButtonState::Pressed, mousePos_, *this);
            return true;
        
        case Modifier2Button:
            palette_.getActiveTool()->onModifier2(ButtonState::Pressed, mousePos_, *this);
            return true;
        
        case Modifier3Button:
            palette_.getActiveTool()->onModifier3(ButtonState::Pressed, mousePos_, *this);
            return true;

        case ConfirmButton:
            palette_.getActiveTool()->onConfirm(mousePos_, *this);
            return true;

        case CancelButton:
            palette_.getActiveTool()->onCancel(mousePos_, *this);
            return true;

        default:
            return false;
    }
}

bool Canvas::onKeyboardReleased(sf::Keyboard::Key key)
{
    switch (key)
    {
        case Modifier1Button:
            palette_.getActiveTool()->onModifier1(ButtonState::Released, mousePos_, *this);
            return true;
        
        case Modifier2Button:
            palette_.getActiveTool()->onModifier2(ButtonState::Released, mousePos_, *this);
            return true;
        
        case Modifier3Button:
            palette_.getActiveTool()->onModifier3(ButtonState::Released, mousePos_, *this);
            return true;

        default:
            return false;
    }
}

bool Canvas::onTime(float deltaSeconds)   
{
    return false;
}

sf::RenderTexture  &Canvas::Texture() { return texture_; }
sf::RectangleShape &Canvas::Rect()    { return rect_;    }

//----------------------------------------------------------------------
