#ifndef WIDGETS_HPP
#define WIDGETS_HPP

#include <list>

#include "SFML/Graphics.hpp"

//---------------------------------------------------------------------------

class Widget
{
    public:
        virtual void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform = sf::Transform::Identity) = 0;

        virtual bool onMousePressed     (sf::Mouse::Button key) = 0;
        virtual bool onMouseMoved       (float x, float y)      = 0;
        virtual bool onMouseReleased    (sf::Mouse::Button key) = 0;
        virtual bool onKeyboardPressed  (sf::Keyboard::Key key) = 0;
        virtual bool onKeyboardReleased (sf::Keyboard::Key key) = 0;
        virtual bool onTime             (float deltaSeconds)    = 0;
};

//---------------------------------------------------------------------------

#endif // WIDGETS_HPP