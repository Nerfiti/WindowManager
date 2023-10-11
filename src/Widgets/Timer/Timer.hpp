#ifndef TIMER_HPP
#define TIMER_HPP

#include "../Widget.hpp"

//-------------------------------------------------------------

class Timer : public Widget 
{
    public:
        Timer(sf::Vector2f position, float size);

        virtual void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform = sf::Transform::Identity) override;

        virtual bool onMousePressed     (sf::Mouse::Button key) override;
        virtual bool onMouseMoved       (float x, float y)      override;
        virtual bool onMouseReleased    (sf::Mouse::Button key) override;
        virtual bool onKeyboardPressed  (sf::Keyboard::Key key) override;
        virtual bool onKeyboardReleased (sf::Keyboard::Key key) override;
        virtual bool onTime             (float deltaSeconds)    override;

    private:
        sf::CircleShape    circle_;
        sf::RectangleShape arrow_;
};

//-------------------------------------------------------------

#endif //TIMER_HPP