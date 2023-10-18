#include "ToolWidgets.hpp"

//----------------------------------------------------------------------------------------

ToolWidget::ToolWidget(ToolPalette &palette):
    palette_(palette)
    {}

void ToolWidget::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform) {}

bool ToolWidget::onMousePressed     (sf::Mouse::Button key)                              { return false; }
bool ToolWidget::onMouseMoved       (int x, int y, const sf::Transform &parentTransform) { return false; }
bool ToolWidget::onMouseReleased    (sf::Mouse::Button key)                              { return false; }
bool ToolWidget::onKeyboardReleased (sf::Keyboard::Key key)                              { return false; }
bool ToolWidget::onTime             (float deltaSeconds)                                 { return false; }

bool ToolWidget::onKeyboardPressed(sf::Keyboard::Key key)                             
{
    switch (key)
    {
        case PenKey:
            palette_.setActiveTool(ToolKeys::Pen);
            return true;

        case EraserKey:
            palette_.setActiveTool(ToolKeys::Eraser);
            return true;

        case RectKey:
            palette_.setActiveTool(ToolKeys::Rect);
            return true;

        case PolylineKey:
            palette_.setActiveTool(ToolKeys::Polyline);
            return true;

        case LineKey:
            palette_.setActiveTool(ToolKeys::Line);
            return true;

        default:
            return false;
            break;
    }
}

//----------------------------------------------------------------------------------------