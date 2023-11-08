#ifndef WIDGETS_HPP
#define WIDGETS_HPP

#include <list>

#include "SFML/Graphics.hpp"

//---------------------------------------------------------------------------

class Widget
{
    public:
        virtual ~Widget() = default;

        virtual void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform = sf::Transform::Identity) = 0;

        virtual bool onMousePressed     (sf::Mouse::Button key);
        virtual bool onMouseMoved       (int x, int y, const sf::Transform &parentTransform);
        virtual bool onMouseReleased    (sf::Mouse::Button key);
        virtual bool onKeyboardPressed  (sf::Keyboard::Key key);
        virtual bool onKeyboardReleased (sf::Keyboard::Key key);
        virtual bool onTime             (float deltaSeconds);
    
        bool isFocused();
        virtual void setIsFocused(bool isFocused);

    protected:
        bool isFocused_ = true;
};

//---------------------------------------------------------------------------

#endif // WIDGETS_HPP