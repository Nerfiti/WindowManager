#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "../Widgets/Canvas/Canvas.hpp"

class Canvas;

//--------------------------------------------------------------

struct ControlState
{
    enum ButtonState
    {
        Pressed, Released
    } state;
};


//--------------------------------------------------------------
//--------------------------------------------------------------

class Tool
{
    public:
        virtual void onMainButton       (ControlState state, sf::Vector2f position, Canvas &canvas) = 0;
        virtual void onSecondaryButton  (ControlState state, sf::Vector2f position, Canvas &canvas);
        virtual void onModifier1        (ControlState state, Canvas &canvas);
        virtual void onModifier2        (ControlState state, Canvas &canvas);
        virtual void onModifier3        (ControlState state, Canvas &canvas);

        virtual void onMove             (sf::Vector2f position, Canvas &canvas);
        virtual void onConfirm          (Canvas &canvas);
        virtual void onCancel           ();

        virtual Widget *getWidget();
};

//--------------------------------------------------------------

class ToolPen : public Tool
{
    public:
        ToolPen();

        void onMainButton (ControlState state, sf::Vector2f position, Canvas &canvas) override;
        void onMove (sf::Vector2f position, Canvas &canvas)                           override;
        void onCancel()                                                               override;

        void setColor(sf::Color);
        void setThickness(int thickness);
    
    private:
        struct ToolSettings
        {
            sf::Color color_;
            int thickness_;
        } settings_;

        sf::RectangleShape stencil_;

        sf::Vector2f prevPoint_;
        bool       isBrushDown_ = false;

        void updateStencil(); //Uptade rectangleShape according to settings_
};

//--------------------------------------------------------------

class ToolRect : public Tool
{
    public:
        ToolRect();

        void onMainButton (ControlState state, sf::Vector2f position, Canvas &canvas) override;

        void onMove       (sf::Vector2f position, Canvas &canvas) override;
        void onModifier1  (ControlState state,    Canvas &canvas) override;
        void onModifier2  (ControlState state,    Canvas &canvas) override;
        void onConfirm    (Canvas &canvas)                        override;
        void onCancel     ()                                      override;

        Widget *getWidget() override;

        void setColor           (sf::Color color);
        void setFilling         (bool needFill  );
        void setSquaring        (bool isSquare  );
        void setOutlineThickness(int thickness  );
        void setOutlineColor    (sf::Color color);

    private:
        struct ToolSettings
        {
            sf::Color color_ = sf::Color::Black;
            bool isSquare_   = false;
            
            bool needFill_   = false;

            sf::Color outlineColor_  = sf::Color::Black;
            float outlineThickness_  = 1;
        } settings_;

        bool               mousePressed_;
        sf::Vector2f       firstPoint_;
        sf::RectangleShape stencil_;

        ///TODO: private
        class ToolRectPreview : public Widget
        {
            public:
                sf::RectangleShape previewRect;
                bool needPreview = false;

                virtual void draw(sf::RenderTarget& canvas, 
                                  const sf::Transform& parentTransform = sf::Transform::Identity) override;
        } preview_;

        void updateStencil(sf::Vector2f rightDown); //Uptade rectangleShape according to settings_
        void updatePreview(sf::Vector2f rightDown);
};

//--------------------------------------------------------------

class ToolPolyline : public Tool
{
    public:
        ToolPolyline();

        void onMainButton (ControlState state, sf::Vector2f position, Canvas &canvas) override;
        void onMove       (sf::Vector2f position, Canvas &canvas)                     override;
        void onConfirm    (Canvas &canvas)                                            override;
        void onCancel     ()                                                          override;

        Widget *getWidget() override;

        void setColor(sf::Color color);
        void setThickness(int thickness);
        void setSimpleLine(bool simpleLine);

    private:
        struct ToolSettings
        {
            sf::Color color_;
            bool simpleLine_;
            float thickness_;
        } settings_;

        sf::RectangleShape stencil_;

        sf::Vector2f firstPoint_;
        sf::Vector2f lastPoint_;

        size_t pointsCount;
        
        ///TODO: private
        class ToolLinePreview : public Widget
        {
            public:
                sf::RectangleShape line;
                bool needDrawing;

                virtual void draw(sf::RenderTarget& canvas, 
                                  const sf::Transform& parentTransform = sf::Transform::Identity) override;             
        } preview_;

        void updateStencil(sf::Vector2f position);
        void updatePreview(sf::Vector2f position);
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