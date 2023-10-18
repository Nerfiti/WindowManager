#include <math.h>

#include "Tools.hpp"

static float length(sf::Vector2f vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

static void ConnectTwoPoints(sf::RenderTarget &target, sf::Vector2f point1, sf::Vector2f point2, float thickness, sf::Color color)
{
    sf::RectangleShape connection = sf::RectangleShape(sf::Vector2f(length(point2 - point1), thickness));
    connection.setFillColor(color);
    connection.setOrigin(sf::Vector2f(0, thickness / 2));
    connection.setPosition(point1);
    
    float tangens = (point2.y - point1.y) / (point2.x - point1.x);
    float angle   = atan(tangens) * 180 / M_PI;

    if (point2.x < point1.x)
        angle += 180;

    connection.rotate(angle);

    target.draw(connection); 
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void Tool::onSecondaryButton  (ButtonState state, sf::Vector2f position, Canvas &canvas) {}
void Tool::onModifier1        (ButtonState state, sf::Vector2f position, Canvas &canvas) {}
void Tool::onModifier2        (ButtonState state, sf::Vector2f position, Canvas &canvas) {}
void Tool::onModifier3        (ButtonState state, sf::Vector2f position, Canvas &canvas) {}
void Tool::onMove             (sf::Vector2f position, Canvas &canvas) {}
void Tool::onConfirm          (sf::Vector2f position, Canvas &canvas) {}
void Tool::onCancel           (sf::Vector2f position, Canvas &canvas) {}

Widget *Tool::getWidget()     { return nullptr; }

void Tool::preview(sf::RenderTarget &window, sf::Vector2f position) {}

//---------------------------------------------------------------------
//---------------------------------------------------------------------

void ToolPen::onMainButton(ButtonState state, sf::Vector2f position, Canvas &canvas)
{    
    if (state == ButtonState::Released)
        return;

    sf::RenderTexture &texture = canvas.Texture();
    sf::Vector2u size = texture.getSize();
    if (size.x == 0)
        return;

    position.x *= size.x;
    position.y *= size.y;

    sf::Vector2f   factor = sf::Vector2f(1, (float)size.y/size.x);
    sf::RectangleShape pen = sf::RectangleShape(sf::Vector2f(thickness_, thickness_));
    pen.setOrigin(sf::Vector2f(thickness_ / 2, thickness_ / 2));
    pen.scale(factor);
    pen.setPosition(position);
    pen.setFillColor(color_);

    texture.draw(pen);

    if (needConnect_)
        ConnectTwoPoints(texture, mousePos_, position, thickness_, color_);        

    mousePos_ = position;
}

void ToolPen::onMove(sf::Vector2f position, Canvas &canvas) 
{
    needConnect_ = true;
    onMainButton(ButtonState::Pressed, position, canvas);
    needConnect_ = false;
}

void ToolPen::preview(sf::RenderTarget &window, sf::Vector2f position)
{
    const float outlineThickness = 1.0f;

    sf::RectangleShape pen(sf::Vector2f(thickness_ / 2, thickness_ / 2));
    pen.setOrigin(thickness_ / 4, thickness_ / 4);
    pen.setPosition(position);
    pen.setOutlineThickness(outlineThickness);
    pen.setOutlineColor(color_);
    pen.setFillColor(sf::Color::Transparent);

    window.draw(pen);
}

void ToolPen::setColor(sf::Color color) { color_ = color; }

//---------------------------------------------------------------------

void ToolRect::onMainButton(ButtonState state, sf::Vector2f position, Canvas &canvas)
{
    position.x *= canvas.Texture().getSize().x;
    position.y *= canvas.Texture().getSize().y;
    
    if (state == ButtonState::Pressed)
    {
        if (!mousePressed_)
            firstPoint_   = position;   

        mousePressed_ = true;
        return;
    }

    if (mousePressed_)
    {
        drawRect(canvas.Texture(), position, false);
        mousePressed_ = false;
    }
}

void ToolRect::onModifier1(ButtonState state, sf::Vector2f position, Canvas &canvas)
{
    switch (state)
    {
        case ButtonState::Pressed:
            isSquare_ = true;
            break;
        case ButtonState::Released:
            isSquare_ = false;
            break;
    }
}

void ToolRect::onModifier2(ButtonState state, sf::Vector2f position, Canvas &canvas)
{
    if (state == ButtonState::Released)
        return;

    needFill_ = !needFill_;
}

void ToolRect::onCancel(sf::Vector2f position, Canvas &canvas)
{
    mousePressed_ = false;
}

void ToolRect::preview(sf::RenderTarget &window, sf::Vector2f position)
{
    if (mousePressed_)
    {
        drawRect(window, position, true);
        return;
    }
    previewFirstPoint_ = position;
}

void ToolRect::setColor(sf::Color color) { color_ = color; }

void ToolRect::drawRect(sf::RenderTarget &target, sf::Vector2f position, bool preview)
{
    sf::Vector2f tempFirstPoint = preview ? previewFirstPoint_ : firstPoint_;

    sf::Vector2f secondPoint = isSquare_ 
                    ? (sf::Vector2f(position.x, tempFirstPoint.y + (position.x - tempFirstPoint.x)))
                    : position;

    sf::RectangleShape rect(secondPoint - tempFirstPoint);
    rect.setPosition(tempFirstPoint);
    rect.setOutlineThickness(outlineThickness_);
    rect.setOutlineColor(color_);
    rect.setFillColor(needFill_ ? color_ : sf::Color::Transparent);

    target.draw(rect);
}

//---------------------------------------------------------------------

void ToolPolyline::onMainButton(ButtonState state, sf::Vector2f position, Canvas &canvas)
{
    if (state == ButtonState::Pressed)
    {
        isNewPoint_ = true;

        sf::RenderTexture &texture = canvas.Texture();
        sf::Vector2u size = texture.getSize();
        position.x *= size.x;
        position.y *= size.y;
        
        if (pointsCount == 0)
        {
            firstPoint_ = position;
            lastPoint_ = position;

            sf::RectangleShape pixel(sf::Vector2f(thickness_, thickness_));
            pixel.setFillColor(color_);
            pixel.setPosition(position);
            texture.draw(pixel);

            pointsCount = 1;

            return;
        }

        if (simpleLine_ && pointsCount == 1)
        {
            ConnectTwoPoints(texture, lastPoint_, position, thickness_, color_);
            onCancel(position, canvas);
            return;
        }

        ConnectTwoPoints(texture, lastPoint_, position, thickness_, color_);
        lastPoint_ = position;
        ++pointsCount;
    }
}

void ToolPolyline::onCancel(sf::Vector2f position, Canvas &canvas)
{
    pointsCount = 0;
}

void ToolPolyline::onConfirm(sf::Vector2f position, Canvas &canvas)
{
    if (pointsCount > 2)
        ConnectTwoPoints(canvas.Texture(), lastPoint_, firstPoint_, thickness_, color_);
    
    pointsCount = 0;
}

void ToolPolyline::preview(sf::RenderTarget &window, sf::Vector2f position)
{
    if (pointsCount == 0)
    {
        const float outlineThickness = 1.0f;

        sf::RectangleShape pixel(sf::Vector2f(thickness_ / 2, thickness_ / 2));
        pixel.setOrigin(thickness_ / 4, thickness_ / 4);
        pixel.setPosition(position);
        pixel.setOutlineThickness(outlineThickness);
        pixel.setOutlineColor(color_);
        pixel.setFillColor(sf::Color::Transparent);

        window.draw(pixel);
    }
    else
    {
        if (isNewPoint_)
        {
            previewLastPoint_ = position;
            isNewPoint_ = false; 
        }

        ConnectTwoPoints(window, previewLastPoint_, position, thickness_, color_);
    }
}

void ToolPolyline::setColor(sf::Color color) { color_ = color; }

void ToolPolyline::setSimpleLine(bool simpleLine) { simpleLine_ = simpleLine; }

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
