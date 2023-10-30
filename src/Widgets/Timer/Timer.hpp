#ifndef TIMER_HPP
#define TIMER_HPP

#include "../Widget.hpp"

//-------------------------------------------------------------

class Timer : public Widget 
{
    public:
        Timer(sf::Vector2f position, float size);

        virtual void draw(sf::RenderTarget& canvas, 
                            const sf::Transform& parentTransform = sf::Transform::Identity) override;

        virtual bool onTime(float deltaSeconds) override;

    private:
        sf::CircleShape    circle_;
        sf::RectangleShape  arrow_;
        sf::Texture  timerTexture_;
};

//-------------------------------------------------------------

#endif //TIMER_HPP