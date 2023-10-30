#include "Timer.hpp"

const float ArrowLenToWidth = 10;
const float ArrowLenToTimerRadius = 0.5f;
//-------------------------------------------------------------

Timer::Timer(sf::Vector2f position, float size):
    circle_(size),
    arrow_(sf::Vector2f(size * ArrowLenToTimerRadius / ArrowLenToWidth, size * ArrowLenToTimerRadius)),
    timerTexture_()
{
    timerTexture_.loadFromFile("./Resources/Timer.png");

    circle_.setOrigin(size, size);
    circle_.setPosition(position);

    arrow_.setOrigin(size * ArrowLenToTimerRadius / (ArrowLenToWidth * 2), size * ArrowLenToTimerRadius);
    arrow_.setPosition(position);

    arrow_.setFillColor(sf::Color::Black);
    circle_.setTexture(&timerTexture_);
}

void Timer::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    canvas.draw(circle_, parentTransform);
    canvas.draw(arrow_, parentTransform);
}

bool Timer::onTime (float deltaSeconds)
{
    const float circleDegree    = 360.0f;
    const int   numberOfSeconds = 60;
    arrow_.rotate(deltaSeconds * (circleDegree / numberOfSeconds));

    return false;
}

//-------------------------------------------------------------
