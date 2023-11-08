#ifndef CANVAS_WINDOW_HPP
#define CANVAS_WINDOW_HPP

#include "Windows.hpp"
#include "../Canvas/Canvas.hpp"
#include "../Canvas/ToolWidgets.hpp"
#include "../../Tools/Filters.hpp"

class CanvasWindow : public FrameWindow
{
    public:
        CanvasWindow(sf::Vector2f pos, sf::Vector2f size, float frameHeight, ToolPalette &tools, FilterPalette &filters, sf::Color color = sf::Color::White);
        CanvasWindow(float posX, float posY, float width, float height, float frameHeight, ToolPalette &tools, FilterPalette &filters, sf::Color color = sf::Color::White);

        void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform = sf::Transform::Identity) override;

        bool onMousePressed (sf::Mouse::Button key)                             override;
        bool onMouseMoved   (int x, int y, const sf::Transform &patentTrasform) override;
        bool onMouseReleased(sf::Mouse::Button key)                             override;
        bool onKeyboardPressed  (sf::Keyboard::Key key)                         override;
        bool onKeyboardReleased (sf::Keyboard::Key key)                         override;
        bool onTime             (float deltaSeconds)                            override;

        void horScroll (float offset);
        void vertScroll(float offset);
        void zoom      (float factor);

    private:
        Canvas canvas_;
        ToolWidget toolWidget_;
};

#endif //CANVAS_WINDOW_HPP