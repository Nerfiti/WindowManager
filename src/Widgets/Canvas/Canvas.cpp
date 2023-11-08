#include "Canvas.hpp"

//----------------------------------------------------------------------

Canvas::Canvas(sf::Vector2f pos, sf::Vector2f size, ToolPalette &tools, FilterPalette &filters, const char *filename):
    Window(pos, size, *tools.getBackgroundColor()),
    tools_              (tools),
    filters_            (filters),
    texture_            (),
    mousePos_           (),
    isMainButtonPressed_(false),
    viewSize_           (stdResolutionX, stdResolutionY),
    offset_             (0, 0),
    scrollbar_          (new ScrollBar(*this, 0.03f))
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

        background.setFillColor(*tools.getBackgroundColor());        
        texture_.draw(background);
    }

    texture_.display();

    windowRect_.setTexture(&texture_.getTexture());
}

Canvas::~Canvas()
{
    delete scrollbar_;
}

void Canvas::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    sf::Transform finalTransform = parentTransform * transform_;
    canvas.draw(windowRect_, finalTransform);
    scrollbar_->draw(canvas, finalTransform);

    if (!isFocused_)
        return;

    if (Widget *widget = tools_.getActiveTool()->getWidget())
    {
        sf::Vector2u size = texture_.getSize();
        finalTransform.scale(sf::Vector2f((float)1/viewSize_.x, 
                                          (float)1/viewSize_.y));
        finalTransform.translate(offset_.x/size.x, offset_.y/size.y);
        widget->draw(canvas, finalTransform);
    }
}

bool Canvas::onMousePressed(sf::Mouse::Button key)
{
    if (!windowRect_.getLocalBounds().contains(mousePos_))
        return false;

    if (scrollbar_->onMousePressed(key))
        return true;

    ControlState state {
        .state = ControlState::Pressed
    };

    isFocused_ = true;

    switch (key)
    {
        case MainButton:
            isMainButtonPressed_ = true;
            tools_.getActiveTool()->onMainButton(state, mousePos_, *this);
            break;
        case SecondaryButton:
            tools_.getActiveTool()->onSecondaryButton(state, mousePos_, *this);
            break;
    }

    return true;
}

bool Canvas::onMouseMoved(int x, int y, const sf::Transform &parentTransform)     
{
    mousePos_   = (parentTransform * transform_).getInverse().transformPoint(sf::Vector2f(x, y));
 
    if (!isFocused_)
        return false;

    if (!windowRect_.getLocalBounds().contains(mousePos_))
    {
        isFocused_ = false;
        tools_.getActiveTool()->onCancel();
        return false;
    }

    if (scrollbar_->onMouseMoved(x, y, parentTransform))
        return true;

    sf::Vector2u contentSize = texture_.getSize();

    mousePos_.x = (mousePos_.x * viewSize_.x + offset_.x) / contentSize.x;
    mousePos_.y = (mousePos_.y * viewSize_.y + offset_.y) / contentSize.y;

    tools_.getActiveTool()->onMove(mousePos_, *this);
    return true;
}

bool Canvas::onMouseReleased(sf::Mouse::Button key)
{
    ControlState state {
        .state = ControlState::Released
    };

    if (scrollbar_->isFocused() && scrollbar_->onMouseReleased(key))
        return true;

    switch (key)
    {
        case MainButton:
            isMainButtonPressed_ = false;
            tools_.getActiveTool()->onMainButton(state, mousePos_, *this);
            break;
        case SecondaryButton:
            tools_.getActiveTool()->onSecondaryButton(state, mousePos_, *this);
            break;
    }

    return true;
}

bool Canvas::onKeyboardPressed(sf::Keyboard::Key key)
{
    ControlState state {
        .state = ControlState::Pressed
    };

    if (scrollbar_->onKeyboardPressed(key))
        return true;

    FilterMask mask(texture_.getSize().x, texture_.getSize().y);

    switch (key)
    {
        case Modifier1Button:
            tools_.getActiveTool()->onModifier1(state, *this);
            return true;
        
        case Modifier2Button:
            tools_.getActiveTool()->onModifier2(state, *this);
            return true;
        
        case Modifier3Button:
            tools_.getActiveTool()->onModifier3(state, *this);
            return true;

        case ConfirmButton:
            tools_.getActiveTool()->onConfirm(*this);
            return true;

        case CancelButton:
            tools_.getActiveTool()->onCancel();
            return true;

        ///TODO: Filter widget with hotkeys
        case BrightnessFilter:
            filters_.setLastFilter(0);
            filters_.getFilter(0)->applyFilter(*this, mask);
            return true;
        
        case InverseFilter:
            filters_.setLastFilter(1);
            filters_.getFilter(1)->applyFilter(*this, mask);
            return true;

        case GrayscaleFilter:
            filters_.setLastFilter(2);
            filters_.getFilter(2)->applyFilter(*this, mask);
            return true;

        case BlackWhiteFilter:
            filters_.setLastFilter(3);
            filters_.getFilter(3)->applyFilter(*this, mask);
            return true;

        case LastFilter:
            filters_.getLastFilter()->applyFilter(*this, mask);
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

    scrollbar_->onKeyboardReleased(key);

    switch (key)
    {
        case Modifier1Button:
            tools_.getActiveTool()->onModifier1(state, *this);
            return true;
        
        case Modifier2Button:
            tools_.getActiveTool()->onModifier2(state, *this);
            return true;
        
        case Modifier3Button:
            tools_.getActiveTool()->onModifier3(state, *this);
            return true;

        default:
            return false;
    }
}

bool Canvas::onTime(float deltaSeconds) 
{
    scrollbar_->onTime(deltaSeconds);
    return false;
}

sf::RenderTexture  &Canvas::Texture() { return texture_; }
sf::RectangleShape &Canvas::Rect()    { return windowRect_; }

void Canvas::horizontalScroll(int offset)
{
    offset_.x = offset;
    windowRect_.setTextureRect(sf::IntRect(offset_, viewSize_));
}

void Canvas::verticalScroll(int offset)
{
    offset_.y = offset;
    windowRect_.setTextureRect(sf::IntRect(offset_, viewSize_));
}

void Canvas::zoom(float factor)
{
    sf::Vector2u size = texture_.getSize();
    viewSize_.x /= factor;
    viewSize_.y /= factor;

    if (offset_.x + viewSize_.x > size.x)
        offset_.x = size.x - viewSize_.x;

    if (offset_.y + viewSize_.y > size.y)
        offset_.y = size.y - viewSize_.y;
        
    windowRect_.setTextureRect(sf::IntRect(offset_, viewSize_));
}

//----------------------------------------------------------------------
