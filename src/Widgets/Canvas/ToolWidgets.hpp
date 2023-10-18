#ifndef TOOL_WIDGETS_HPP
#define TOOL_WIDGETS_HPP

#include "../Widget.hpp"
#include "../../Tools/Tools.hpp"

class ToolWidget : public Widget
{
    public:
        ToolWidget(ToolPalette &palette);

        virtual void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform = sf::Transform::Identity) override;

        virtual bool onMousePressed     (sf::Mouse::Button key)                              override;
        virtual bool onMouseMoved       (int x, int y, const sf::Transform &parentTransform) override;
        virtual bool onMouseReleased    (sf::Mouse::Button key)                              override;
        virtual bool onKeyboardPressed  (sf::Keyboard::Key key)                              override;
        virtual bool onKeyboardReleased (sf::Keyboard::Key key)                              override;
        virtual bool onTime             (float deltaSeconds)                                 override;

    private:
        ToolPalette &palette_;

        static const sf::Keyboard::Key PenKey      = sf::Keyboard::Key::P;
        static const sf::Keyboard::Key EraserKey   = sf::Keyboard::Key::E;
        static const sf::Keyboard::Key RectKey     = sf::Keyboard::Key::R;
        static const sf::Keyboard::Key PolylineKey = sf::Keyboard::Key::B;
        static const sf::Keyboard::Key LineKey     = sf::Keyboard::Key::L;

};

#endif //TOOL_WIDGETS_HPP