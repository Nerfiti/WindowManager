#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "../Widgets/Canvas/Canvas.hpp"

class Canvas;

//--------------------------------------------------------------

enum class ButtonState
{
    Pressed,
    Released
};

//--------------------------------------------------------------
//--------------------------------------------------------------

class Tool
{
    public:
        virtual void onMainButton       (ButtonState state, sf::Vector2f position, Canvas &canvas) = 0;
        virtual void onSecondaryButton  (ButtonState state, sf::Vector2f position, Canvas &canvas);
        virtual void onModifier1        (ButtonState state, sf::Vector2f position, Canvas &canvas);
        virtual void onModifier2        (ButtonState state, sf::Vector2f position, Canvas &canvas);
        virtual void onModifier3        (ButtonState state, sf::Vector2f position, Canvas &canvas);

        virtual void onMove             (sf::Vector2f position, Canvas &canvas);
        virtual void onConfirm          (sf::Vector2f position, Canvas &canvas);
        virtual void onCancel           (sf::Vector2f position, Canvas &canvas);

        virtual Widget *getWidget();

        virtual void preview(sf::RenderTarget &window, sf::Vector2f position);
};

//--------------------------------------------------------------

class ToolPen : public Tool
{
    public:
        void onMainButton (ButtonState state, sf::Vector2f position, Canvas &canvas) override;
        void onMove (sf::Vector2f position, Canvas &canvas) override;

        void preview(sf::RenderTarget &window, sf::Vector2f position) override;

        void setColor(sf::Color);
    
    private:
        int   thickness_ = 6;
        sf::Color color_ = sf::Color::Black;

        sf::Vector2f mousePos_;
        bool      needConnect_ = false;
};

//--------------------------------------------------------------

class ToolRect : public Tool
{
    public:
        void onMainButton (ButtonState state, sf::Vector2f position, Canvas &canvas) override;
        void onModifier1  (ButtonState state, sf::Vector2f position, Canvas &canvas) override;
        void onModifier2  (ButtonState state, sf::Vector2f position, Canvas &canvas) override;
        void onCancel     (sf::Vector2f position, Canvas &canvas) override;
        
        void preview(sf::RenderTarget &window, sf::Vector2f position) override;
    
        void setColor(sf::Color color);

    private:
        float outlineThickness_  = 3;
        sf::Color color_         = sf::Color::Black;

        sf::Vector2f firstPoint_;
        sf::Vector2f previewFirstPoint_;

        bool mousePressed_ = false;
        bool isSquare_     = false;
        bool needFill_     = false;

        void drawRect(sf::RenderTarget &target, sf::Vector2f position, bool preview);
};

//--------------------------------------------------------------

class ToolPolyline : public Tool
{
    public:
        void onMainButton (ButtonState state, sf::Vector2f position, Canvas &canvas) override;
        void onCancel     (sf::Vector2f position, Canvas &canvas) override;
        void onConfirm    (sf::Vector2f position, Canvas &canvas) override;
    
        void preview(sf::RenderTarget &window, sf::Vector2f position) override;

        void setColor(sf::Color color);
        void setSimpleLine(bool simpleLine);

    private:
        sf::Vector2f firstPoint_;
        sf::Vector2f lastPoint_;

        sf::Vector2f previewLastPoint_;

        float thickness_ = 6;
        sf::Color color_;

        bool simpleLine_   = false;
        size_t pointsCount = 0;

        bool isNewPoint_ = false;
};

//--------------------------------------------------------------
//--------------------------------------------------------------

enum class ToolKeys
{
    Pen,
    Eraser,
    Rect,
    Polyline,
    Line
};

class ToolPalette
{
    public:
        ToolPalette(sf::Color backgroundColor, sf::Color foregroundColor = sf::Color::Black);

        Tool      *setActiveTool(ToolKeys key);
        Tool      *getActiveTool();
        sf::Color *getForegroundColor();
        sf::Color *getBackgroundColor();

    private:
        ToolPen      pen_;
        ToolRect     rect_;
        ToolPolyline polyLine_;

        Tool *activeTool_ = &rect_;

        sf::Color foregroundColor_ = sf::Color::Black;
        sf::Color backgroundColor_ = sf::Color::White;
};

//--------------------------------------------------------------
//--------------------------------------------------------------


#endif //TOOLS_HPP