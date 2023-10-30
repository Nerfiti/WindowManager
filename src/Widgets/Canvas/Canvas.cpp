#include "Canvas.hpp"

//----------------------------------------------------------------------

Canvas::Canvas(sf::Vector2f pos, sf::Vector2f size, ToolPalette& palette, const char *filename):
    Window(pos, size, *palette.getBackgroundColor()),
    palette_            (palette),
    texture_            (),
    mousePos_           (),
    isMainButtonPressed_(false),
    viewSize_           (stdResolutionX, stdResolutionY),
    offset_             (0, 0)
{
    sf::RectangleShape background(sf::Vector2f(viewSize_.x, viewSize_.y));

    if (filename)
    {
        sf::Texture tmpTexture;
        tmpTexture.loadFromFile(filename);
        viewSize_ = sf::Vector2i(tmpTexture.getSize());

        texture_.create(viewSize_.x, viewSize_.y);
        background.setSize(sf::Vector2f(viewSize_.x, viewSize_.y));

        background.setTexture(&tmpTexture);
        texture_.draw(background);
    }   
    else
    {
        texture_.create(viewSize_.x, viewSize_.y);

        background.setFillColor(*palette.getBackgroundColor());        
        texture_.draw(background);
    }

    texture_.display();

    windowRect_.setTexture(&texture_.getTexture());
}

void Canvas::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    sf::Transform finalTransform = parentTransform * transform_;
    canvas.draw(windowRect_, finalTransform);

    ///TODO: Support scrollbar
    if (Widget *widget = palette_.getActiveTool()->getWidget())
    {
        sf::Vector2u size = texture_.getSize();
        finalTransform.scale(sf::Vector2f((float)1/size.x, (float)1/size.y));
        widget->draw(canvas, finalTransform);
    }
}

bool Canvas::onMousePressed(sf::Mouse::Button key)
{
    ControlState state {
        .state = ControlState::Pressed
    };

    switch (key)
    {
        case MainButton:
            isMainButtonPressed_ = true;
            palette_.getActiveTool()->onMainButton(state, mousePos_, *this);
            break;
        case SecondaryButton:
            palette_.getActiveTool()->onSecondaryButton(state, mousePos_, *this);
            break;
    }

    return true;
}

bool Canvas::onMouseMoved(int x, int y, const sf::Transform &parentTransform)     
{
    mousePos_ = (parentTransform * transform_).getInverse().transformPoint(sf::Vector2f(x, y));
    palette_.getActiveTool()->onMove(mousePos_, *this);
    return true;
}

bool Canvas::onMouseReleased(sf::Mouse::Button key)
{
    ControlState state {
        .state = ControlState::Released
    };

    switch (key)
    {
        case MainButton:
            isMainButtonPressed_ = false;
            palette_.getActiveTool()->onMainButton(state, mousePos_, *this);
            break;
        case SecondaryButton:
            palette_.getActiveTool()->onSecondaryButton(state, mousePos_, *this);
            break;
    }

    return true;
}

bool Canvas::onKeyboardPressed(sf::Keyboard::Key key)
{
    ControlState state {
        .state = ControlState::Pressed
    };

    switch (key)
    {
        case Modifier1Button:
            palette_.getActiveTool()->onModifier1(state, *this);
            return true;
        
        case Modifier2Button:
            palette_.getActiveTool()->onModifier2(state, *this);
            return true;
        
        case Modifier3Button:
            palette_.getActiveTool()->onModifier3(state, *this);
            return true;

        case ConfirmButton:
            palette_.getActiveTool()->onConfirm(*this);
            return true;

        case CancelButton:
            palette_.getActiveTool()->onCancel();
            return true;

        default:
            return false;
    }
}

bool Canvas::onKeyboardReleased(sf::Keyboard::Key key)
{
    ControlState state {
        .state = ControlState::Released
    };

    switch (key)
    {
        case Modifier1Button:
            palette_.getActiveTool()->onModifier1(state, *this);
            return true;
        
        case Modifier2Button:
            palette_.getActiveTool()->onModifier2(state, *this);
            return true;
        
        case Modifier3Button:
            palette_.getActiveTool()->onModifier3(state, *this);
            return true;

        default:
            return false;
    }
}

bool Canvas::onTime(float deltaSeconds) { return false; }

sf::RenderTexture  &Canvas::Texture() { return texture_; }
sf::RectangleShape &Canvas::Rect()    { return windowRect_; }

void Canvas::horizontalScroll(int offset)
{
    offset_.x += offset;
    windowRect_.setTextureRect(sf::IntRect(offset_, viewSize_));
}

void Canvas::verticalScroll(int offset)
{
    offset_.y += offset;
    windowRect_.setTextureRect(sf::IntRect(offset_, viewSize_));
}

void Canvas::zoom(sf::Vector2f factor)
{
    viewSize_.x *= factor.x;
    viewSize_.y *= factor.y;
    windowRect_.setTextureRect(sf::IntRect(offset_, viewSize_));
}

void Canvas::zoom(float factor)
{
    zoom(sf::Vector2f(factor, factor));
}

//----------------------------------------------------------------------
