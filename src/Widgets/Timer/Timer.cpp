#include "Timer.hpp"

const float ArrowLenToWidth = 10;

//-------------------------------------------------------------

Timer::Timer(sf::Vector2f position, float size):
    circle_(size),
    arrow_(sf::Vector2f(size / ArrowLenToWidth, size))
{
    circle_.setOrigin(size, size);
    circle_.setPosition(position);

    arrow_.setOrigin(size / (ArrowLenToWidth * 2), size);
    arrow_.setPosition(position);

    arrow_.setFillColor(sf::Color::Black);
    circle_.setFillColor(sf::Color(255, 128, 30));
}

void Timer::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    canvas.draw(circle_, parentTransform);
    canvas.draw(arrow_, parentTransform);
}

bool Timer::onMousePressed     (sf::Mouse::Button key)                              {return false;}
bool Timer::onMouseMoved       (int x, int y, const sf::Transform &parentTransform) {return false;}
bool Timer::onMouseReleased    (sf::Mouse::Button key)                              {return false;}
bool Timer::onKeyboardPressed  (sf::Keyboard::Key key)                              {return false;}
bool Timer::onKeyboardReleased (sf::Keyboard::Key key)                              {return false;}

bool Timer::onTime (float deltaSeconds)
{
    const float circleDegree    = 360.0f;
    const int   numberOfSeconds = 60;
    arrow_.rotate(deltaSeconds * (circleDegree / numberOfSeconds));

    return false;
}

//-------------------------------------------------------------
