#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "../Widget.hpp"
#include "../../Tools/Tools.hpp"

class ToolPalette;

//----------------------------------------------------------------------

class Canvas : public Widget
{
    public:
        Canvas(sf::Vector2f size, ToolPalette& palette);

        virtual void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform = sf::Transform::Identity) override;

        virtual bool onMousePressed     (sf::Mouse::Button key)                              override;
        virtual bool onMouseMoved       (int x, int y, const sf::Transform &parentTransform) override;
        virtual bool onMouseReleased    (sf::Mouse::Button key)                              override;
        virtual bool onKeyboardPressed  (sf::Keyboard::Key key)                              override;
        virtual bool onKeyboardReleased (sf::Keyboard::Key key)                              override;
        virtual bool onTime             (float deltaSeconds)                                 override;

        sf::RenderTexture  &Texture();
        sf::RectangleShape &Rect();

    private:
        sf::RenderTexture  texture_;
        sf::RectangleShape rect_;

        ToolPalette& palette_;

        sf::Vector2f    mousePos_;
        bool isMainButtonPressed_;

        static const sf::Mouse::Button MainButton      = sf::Mouse::Left;
        static const sf::Mouse::Button SecondaryButton = sf::Mouse::Right;
        static const sf::Keyboard::Key Modifier1Button = sf::Keyboard::Key::LShift;
        static const sf::Keyboard::Key Modifier2Button = sf::Keyboard::Key::LAlt;
        static const sf::Keyboard::Key Modifier3Button = sf::Keyboard::Key::LControl;
        static const sf::Keyboard::Key ConfirmButton   = sf::Keyboard::Key::Enter;
        static const sf::Keyboard::Key CancelButton    = sf::Keyboard::Key::Escape;
};

//----------------------------------------------------------------------

#endif //CANVAS_HPP