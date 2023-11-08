#include <iostream>
#include <math.h>

#include "Tools.hpp"

static float length(sf::Vector2f vec) { return sqrt(vec.x * vec.x + vec.y * vec.y); }

static void getLineBetweenTwoPoints(sf::Vector2f point1, sf::Vector2f point2, sf::RectangleShape &outRect)
{
    sf::Vector2f size = outRect.getSize();
    outRect.setSize(sf::Vector2f(length(point2 - point1), size.y));
    outRect.setOrigin(sf::Vector2f(0, size.y / 2));
    outRect.setPosition(point1);
    
    float tangens = (point2.y - point1.y) / (point2.x - point1.x);
    float angle   = atan(tangens) * 180 / M_PI;

    if (point2.x < point1.x)
        angle += 180;

    outRect.setRotation(angle);
}

static void ConnectTwoPoints(sf::RenderTarget &target, sf::Vector2f point1, sf::Vector2f point2, float thickness, sf::Color color)
{
    sf::RectangleShape connection(sf::Vector2f(0, thickness));
    connection.setFillColor(color);

    getLineBetweenTwoPoints(point1, point2, connection);

    target.draw(connection); 
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Tool::onSecondaryButton  (ControlState state, sf::Vector2f position, Canvas &canvas) {}
void Tool::onModifier1        (ControlState state,    Canvas &canvas)                     {}
void Tool::onModifier2        (ControlState state,    Canvas &canvas)                     {}
void Tool::onModifier3        (ControlState state,    Canvas &canvas)                     {}
void Tool::onMove             (sf::Vector2f position, Canvas &canvas)                     {}
void Tool::onConfirm          (Canvas &canvas)                                            {}
void Tool::onCancel           ()                                                          {}

Widget *Tool::getWidget()     { return nullptr; }

//---------------------------------------------------------------------
//---------------------------------------------------------------------

ToolPen::ToolPen():
    settings_   (ToolSettings{.color_ = sf::Color::Black, .thickness_ = 6}),
    stencil_    (),
    prevPoint_  (),
    isBrushDown_(false)
    { updateStencil(); }

void ToolPen::onMainButton(ControlState state, sf::Vector2f position, Canvas &canvas)
{   
    if (state.state == ControlState::ButtonState::Released)
    {
        onCancel();
        return;
    }

    sf::RenderTexture &texture = canvas.Texture();
    sf::Vector2u size = texture.getSize();

    position.x = (int)(position.x * size.x);
    position.y = (int)(position.y * size.y);

    stencil_.setPosition(position);
    texture.draw(stencil_);

    if (isBrushDown_) 
        ConnectTwoPoints(texture, prevPoint_, position, settings_.thickness_, settings_.color_);        

    prevPoint_ = position;

    isBrushDown_ = true;
}

void ToolPen::onMove(sf::Vector2f position, Canvas &canvas) 
{
    if (isBrushDown_)
        onMainButton(ControlState{.state = ControlState::ButtonState::Pressed}, position, canvas);
}

void ToolPen::onCancel() { isBrushDown_ = false; }

void ToolPen::setColor(sf::Color color)   
{ 
    settings_.color_ = color; 
    stencil_.setFillColor(color);
}

void ToolPen::setThickness(int thickness) 
{ 
    settings_.thickness_ = thickness; 
    stencil_.setRadius((float)thickness / 2);
}

void ToolPen::updateStencil()
{
    float radius = (float)settings_.thickness_ / 2;
    stencil_.setRadius    (radius);
    stencil_.setFillColor(settings_.color_);
    stencil_.setOrigin   (sf::Vector2f(radius, radius));
}

//---------------------------------------------------------------------

ToolRect::ToolRect():
    settings_(ToolSettings{.color_ = sf::Color::Black, 
                           .isSquare_ = false, 
                           .needFill_ = false, 
                           .outlineColor_ = sf::Color::Black,
                           .outlineThickness_ = 6}),
    mousePressed_(false),
    firstPoint_  (),
    preview_     (),
    stencil_     ()
    {}

void ToolRect::onMainButton(ControlState state, sf::Vector2f position, Canvas &canvas)
{
    sf::RenderTexture &texture = canvas.Texture();
    sf::Vector2u size = texture.getSize();

    position.x = (int)(position.x * size.x);
    position.y = (int)(position.y * size.y);
    
    if (state.state == ControlState::ButtonState::Pressed)
    {
        if (!mousePressed_)
        {
            firstPoint_   = position;   
            stencil_.setPosition(firstPoint_);
            preview_.previewRect.setPosition(position);
        }
        mousePressed_ = true;
        return;
    }

    //if released
    if (mousePressed_)
    {
        updateStencil(position);
        onConfirm(canvas);
        mousePressed_ = false;
        preview_.needPreview = false;
    }
}

void ToolRect::onMove(sf::Vector2f position, Canvas &canvas)
{
    if (!mousePressed_)
        return;
    
    preview_.needPreview = true;

    sf::RenderTexture &texture = canvas.Texture();
    sf::Vector2u size = texture.getSize();

    position.x = (int)(position.x * size.x);
    position.y = (int)(position.y * size.y);

    updatePreview(position);
}

void ToolRect::onModifier1(ControlState state, Canvas &canvas)
{
    switch (state.state)
    {
        case ControlState::ButtonState::Pressed:
            setSquaring(true);
            break;
        case ControlState::ButtonState::Released:
            setSquaring(false);
            break;
    }
}

void ToolRect::onModifier2(ControlState state, Canvas &canvas)
{
    if (state.state == ControlState::ButtonState::Released)
        return;

    setFilling(!settings_.needFill_);
}

void ToolRect::onConfirm(Canvas &canvas)
{
    canvas.Texture().draw(stencil_);
}

void ToolRect::onCancel()
{
    mousePressed_ = false;
}

void ToolRect::setColor(sf::Color color)  
{ 
    settings_.color_ = color;
    preview_.previewRect.setFillColor(color); 
}

void ToolRect::setFilling(bool needFill)  
{ 
    settings_.needFill_ = needFill;
    preview_.previewRect.setFillColor( needFill ? settings_.color_ : sf::Color::Transparent );
}

void ToolRect::setSquaring(bool isSquare) 
{ 
    settings_.isSquare_ = isSquare; 
}

void ToolRect::setOutlineThickness(int thickness)
{   
    settings_.outlineThickness_ = thickness;
    preview_.previewRect.setOutlineThickness(thickness);
}

void ToolRect::setOutlineColor(sf::Color color)

{
    settings_.outlineColor_ = color;
    preview_.previewRect.setOutlineColor(color);
}

Widget *ToolRect::getWidget() { return &preview_; }

void ToolRect::ToolRectPreview::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    if (needPreview)
        canvas.draw(previewRect, parentTransform);
}

void ToolRect::updateStencil(sf::Vector2f rightDown)
{
    stencil_.setFillColor( (settings_.needFill_) ? settings_.color_ : sf::Color::Transparent );
    stencil_.setOutlineColor(settings_.outlineColor_);
    stencil_.setOutlineThickness(settings_.outlineThickness_);
    
    float sizeX = rightDown.x - firstPoint_.x;
    int sizeYSign = (rightDown.y > firstPoint_.y) ? 1 : -1;
    float sizeY = (settings_.isSquare_) ? (sizeX * sizeYSign) : (rightDown.y - firstPoint_.y);

    stencil_.setSize(sf::Vector2f(sizeX, sizeY));
}

void ToolRect::updatePreview(sf::Vector2f rightDown)
{
    preview_.previewRect.setFillColor( (settings_.needFill_) ? settings_.color_ : sf::Color::Transparent );
    preview_.previewRect.setOutlineColor(settings_.outlineColor_);
    preview_.previewRect.setOutlineThickness(settings_.outlineThickness_);

    float sizeX = rightDown.x - firstPoint_.x;
    int sizeYSign = (rightDown.y > firstPoint_.y) ? 1 : -1;
    float sizeY = (settings_.isSquare_) ? (sizeX * sizeYSign) : (rightDown.y - firstPoint_.y);

    preview_.previewRect.setSize(sf::Vector2f(sizeX, sizeY));
}

//---------------------------------------------------------------------

ToolPolyline::ToolPolyline():
    settings_(ToolSettings{.color_ = sf::Color::Black, 
                           .simpleLine_ = false, 
                           .thickness_ = 6}),
    stencil_   (),
    firstPoint_(),
    lastPoint_ (),
    pointsCount(0),
    preview_()
{
    preview_.needDrawing = false;
}

void ToolPolyline::onMainButton(ControlState state, sf::Vector2f position, Canvas &canvas)
{
    if (state.state != ControlState::ButtonState::Pressed)
        return;
    
    sf::RenderTexture &texture = canvas.Texture();
    sf::Vector2u size = texture.getSize();
    
    position.x = (int)(position.x * size.x);
    position.y = (int)(position.y * size.y);
    
    if (pointsCount == 0)
    {
        firstPoint_ = position;
        lastPoint_ = position;
        pointsCount = 1;
        return;
    }

    updateStencil(position);
    texture.draw(stencil_);
    preview_.needDrawing = false;

    if (settings_.simpleLine_ && pointsCount == 1)
    {
        onCancel();
        return;
    }

    lastPoint_ = position;
    ++pointsCount;
}

void ToolPolyline::onMove(sf::Vector2f position, Canvas &canvas)
{ 
    if (pointsCount <= 0)
        return;

    preview_.needDrawing = true;
    sf::Vector2u size = canvas.Texture().getSize();

    position.x = (int)(position.x * size.x);
    position.y = (int)(position.y * size.y);
    updatePreview(position);
}

void ToolPolyline::onCancel()
{
    pointsCount = 0;
    preview_.needDrawing = false;
}

void ToolPolyline::onConfirm(Canvas &canvas)
{
    if (pointsCount > 2)
        ConnectTwoPoints(canvas.Texture(), lastPoint_, firstPoint_, settings_.thickness_, settings_.color_);
    
    pointsCount = 0;
    preview_.needDrawing = false;
}

Widget *ToolPolyline::getWidget() { return &preview_; }

void ToolPolyline::setColor(sf::Color color) { settings_.color_ = color; }

void ToolPolyline::setThickness(int thickness) { settings_.thickness_ = thickness; }

void ToolPolyline::setSimpleLine(bool simpleLine) { settings_.simpleLine_ = simpleLine; }

void ToolPolyline::ToolLinePreview::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    if (needDrawing)
        canvas.draw(line, parentTransform);
}

///TODO: delete copy&paste
void ToolPolyline::updateStencil(sf::Vector2f position)
{
    stencil_.setFillColor(settings_.color_);
    stencil_.setOrigin(sf::Vector2f(0, settings_.thickness_ / 2));

    sf::Vector2f size = stencil_.getSize();
    stencil_.setSize(sf::Vector2f(size.x, settings_.thickness_));

    getLineBetweenTwoPoints(lastPoint_, position, stencil_);
}

void ToolPolyline::updatePreview(sf::Vector2f position)
{
    preview_.line.setFillColor(settings_.color_);
    preview_.line.setOrigin(sf::Vector2f(0, settings_.thickness_ / 2));
    
    sf::Vector2f size = preview_.line.getSize();
    preview_.line.setSize(sf::Vector2f(size.x, settings_.thickness_));

    getLineBetweenTwoPoints(lastPoint_, position, preview_.line);
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

ToolPalette::ToolPalette(sf::Color backgroundColor, sf::Color foregroundColor):
    backgroundColor_(backgroundColor),
    foregroundColor_(foregroundColor)
{
    setActiveTool(ToolKeys::Pen);
}

Tool *ToolPalette::setActiveTool(ToolKeys key)
{
    switch (key)
    {
        case ToolKeys::Pen:
            activeTool_ = &pen_;
            pen_.setColor(foregroundColor_);
            break;
        
        case ToolKeys::Eraser:
            activeTool_ = &pen_;
            pen_.setColor(backgroundColor_);
            break;
        
        case ToolKeys::Rect:
            activeTool_ = &rect_;
            rect_.setColor(foregroundColor_);
            break;
        
        case ToolKeys::Polyline:
            activeTool_ = &polyLine_;
            polyLine_.setColor(foregroundColor_);
            polyLine_.setSimpleLine(false);
            break;
        
        case ToolKeys::Line:
            activeTool_ = &polyLine_;
            polyLine_.setColor(foregroundColor_);
            polyLine_.setSimpleLine(true);
            break;
    }

    return activeTool_;
}

Tool *ToolPalette::getActiveTool() { return activeTool_; }

sf::Color *ToolPalette::getForegroundColor() { return &foregroundColor_; }

sf::Color *ToolPalette::getBackgroundColor() { return &backgroundColor_; }

//---------------------------------------------------------------------
