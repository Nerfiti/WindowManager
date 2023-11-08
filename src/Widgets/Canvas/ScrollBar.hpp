#ifndef SCROLL_BAR_HPP
#define SCROLL_BAR_HPP

#include "../Canvas/Canvas.hpp"

class Canvas;

class ScrollBar : public Window
{
    public:
        ScrollBar(Canvas &window, float barWidth, 
                                  sf::Color barColor      = sf::Color(255, 255, 255, 100), 
                                  sf::Color scrollerColor = sf::Color(100, 100, 100, 150));

        virtual void draw(sf::RenderTarget& canvas, 
                const sf::Transform& parentTransform = sf::Transform::Identity) override;
        
        virtual bool onMousePressed     (sf::Mouse::Button key)                              override;
        virtual bool onMouseMoved       (int x, int y, const sf::Transform &parentTransform) override;
        virtual bool onMouseReleased    (sf::Mouse::Button key)                              override;
        virtual bool onKeyboardPressed  (sf::Keyboard::Key key)                              override;
        virtual bool onKeyboardReleased (sf::Keyboard::Key key)                              override;

        bool isFocused() const;

        void setHorScrollFunc (void (*horScrollFunc )(float offset));
        void setVertScrollFunc(void (*vertScrollFunc)(float offset));
        void setZoomFunc      (void (*zoomFunc      )(float factor));

    private:
        Canvas &canvas_;

        sf::Vector2f contentSize_;
        sf::Vector2f offset_;
        sf::Vector2f viewSize_;

        sf::RectangleShape horBar_;
        sf::RectangleShape horScroller_;

        sf::RectangleShape vertBar_;
        sf::RectangleShape vertScroller_;

        sf::Color barColor_;
        sf::Color scrollerColor_;

        bool isFocused_;

        bool isHorCaptured_;
        bool isVertCaptured_;
        sf::Vector2f localMousePos_;

        static constexpr float ScrollSpeed = 0.1f;
        static constexpr float zoomFactor  = 1.1f;

        void updateScrollers();
        void horScroll(float delta);
        void vertScroll(float delta);
        void zoom(float factor);
};

#endif //SCROLL_BAR_HPP