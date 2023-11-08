#include "ScrollBar.hpp"

ScrollBar::ScrollBar(Canvas &window, float barWidth, sf::Color barColor, sf::Color scrollerColor):
    Window(0, 0, 1, 1, sf::Color::Transparent),
    canvas_          (window),
    contentSize_     (1, 1),
    offset_          (0, 0),
    viewSize_        (1, 1),
    horBar_          (sf::Vector2f(1, barWidth)),
    horScroller_     (sf::Vector2f(1, barWidth)),
    vertBar_         (sf::Vector2f(barWidth, 1 - barWidth)),
    vertScroller_    (sf::Vector2f(barWidth, 1 - barWidth)),
    barColor_        (barColor),
    scrollerColor_   (scrollerColor),
    isFocused_       (true),
    isHorCaptured_   (false),
    isVertCaptured_  (false),
    localMousePos_   ()
{
    horBar_.setPosition(sf::Vector2f(0, 1 - barWidth));
    horBar_.setFillColor(barColor);
    horScroller_.setFillColor(scrollerColor);

    vertBar_.setPosition(sf::Vector2f(1 - barWidth, 0));
    vertBar_.setFillColor(barColor);
    vertScroller_.setFillColor(scrollerColor);

    updateScrollers();
}

void ScrollBar::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    if (!isFocused_)
        return;

    canvas.draw(horBar_,       parentTransform);
    canvas.draw(horScroller_,  parentTransform);
    canvas.draw(vertBar_,      parentTransform);
    canvas.draw(vertScroller_, parentTransform);
}

bool ScrollBar::onMousePressed(sf::Mouse::Button key)                             
{
    if (isFocused_ && horScroller_.getGlobalBounds().contains(localMousePos_))
    {
        isHorCaptured_ = true;
        return true;
    }

    if (isFocused_ && vertScroller_.getGlobalBounds().contains(localMousePos_))
    {
        isVertCaptured_ = true;
        return true;
    }

    return false;
}

bool ScrollBar::onMouseMoved(int x, int y, const sf::Transform &parentTransform)
{
    sf::Vector2f newPos = (parentTransform * transform_).getInverse().transformPoint(sf::Vector2f(x, y));
    sf::Vector2f delta = newPos - localMousePos_;

    if (isVertCaptured_)
    {   
        vertScroll(delta.y);
    }
    else if (isHorCaptured_)
    {
        horScroll(delta.x);
    }
    else
    {
        if (vertBar_.getGlobalBounds().contains(newPos) || horBar_.getGlobalBounds().contains(newPos))
            isFocused_ = true;
        else
            isFocused_ = false;        
    }

    localMousePos_ = newPos;
    return false;
}

bool ScrollBar::onMouseReleased(sf::Mouse::Button key)                             
{
    if (isHorCaptured_ || isVertCaptured_)
    {
        isHorCaptured_  = false;
        isVertCaptured_ = false;

        return true;
    }

    return false;
}

bool ScrollBar::onKeyboardPressed(sf::Keyboard::Key key)                             
{
    switch (key)
    {
        case sf::Keyboard::Equal:
            zoom(zoomFactor);
            return true;
    
        case sf::Keyboard::Key::Dash:
            zoom(1 / zoomFactor);
            return true;
        
        case sf::Keyboard::Up:
            vertScroll(-ScrollSpeed);
            return true;
            
        case sf::Keyboard::Down:
            vertScroll(ScrollSpeed);
            return true;
        
        case sf::Keyboard::Left:
            horScroll(-ScrollSpeed);
            return true;
        
        case sf::Keyboard::Right:
            horScroll(ScrollSpeed);
            return true;
        
        default:
            return false;
    }
}

bool ScrollBar::onKeyboardReleased(sf::Keyboard::Key key)                             
{
    return false;
}

bool ScrollBar::isFocused() const { return isFocused_; }

//private:

void ScrollBar::updateScrollers()
{   
    const float barWidth = horBar_.getSize().y;

    horScroller_.setPosition (sf::Vector2f(offset_.x, 1 - barWidth));
    horScroller_.setSize     (sf::Vector2f(viewSize_.x / contentSize_.x, barWidth));
    

    vertScroller_.setPosition(sf::Vector2f(1 - barWidth, offset_.y));
    vertScroller_.setSize    (sf::Vector2f(barWidth, viewSize_.y / contentSize_.y));
}

void ScrollBar::horScroll(float delta)
{
    float scrollPos  = horScroller_.getPosition().x;
    float scrollSize = horScroller_.getSize().x;

    if (scrollPos + delta < 0)
        delta = -scrollPos;
    if (scrollPos + delta + scrollSize > contentSize_.x)
        delta = contentSize_.x - scrollPos - scrollSize;

    offset_.x += delta;
    canvas_.horizontalScroll(offset_.x * canvas_.Texture().getSize().x);
    updateScrollers();
}

void ScrollBar::vertScroll(float delta)
{
    float scrollPos  = vertScroller_.getPosition().y;
    float scrollSize = vertScroller_.getSize().y;
    
    if (scrollPos + delta < 0)
        delta = -scrollPos;
    if (scrollPos + delta + scrollSize > contentSize_.y)
        delta = contentSize_.y - scrollPos - scrollSize;

    offset_.y += delta;
    canvas_.verticalScroll(offset_.y * canvas_.Texture().getSize().y);
    updateScrollers();
}

void ScrollBar::zoom(float factor)
{
    sf::Vector2f tmpViewSize = viewSize_ / factor;
    if (tmpViewSize.x > 1 || tmpViewSize.y > 1)
        return;
    
    viewSize_ = tmpViewSize;

    if (offset_.x + viewSize_.x > contentSize_.x)
        offset_.x = contentSize_.x - viewSize_.x;

    if (offset_.y + viewSize_.y > contentSize_.y)
        offset_.y = contentSize_.y - viewSize_.y;
    
    canvas_.zoom(factor);
    updateScrollers();
}

//-----------------------------------------------------------------------