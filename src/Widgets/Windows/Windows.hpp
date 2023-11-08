#ifndef WINDOWS_HPP
#define WINDOWS_HPP

#include "../Widget.hpp"
#include "../Timer/Timer.hpp"

//---------------------------------------------------------------------------

class Window : public Widget
{
    public:
        Window(sf::Vector2f pos, sf::Vector2f size, sf::Color color = sf::Color::Black);
        Window(float posX, float posY, float width, float height, sf::Color color = sf::Color::Black);

        virtual ~Window() = default;

        sf::Transform &Transform();

    protected:
        sf::RectangleShape windowRect_;
        sf::Transform      transform_;
        sf::Color          color_;
};

//---------------------------------------------------------------------------

class FrameWindow : public Window
{
    public:
        FrameWindow(sf::Vector2f pos, sf::Vector2f size, float frameHeight, sf::Color color = sf::Color::Black);
        FrameWindow(float posX, float posY, float width, float height, float frameHeight, sf::Color color = sf::Color::Black);
    
        virtual ~FrameWindow() = default;

        void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform = sf::Transform::Identity) override;

        bool onMousePressed     (sf::Mouse::Button key)                              override;
        bool onMouseMoved       (int x, int y, const sf::Transform &parentTransform) override;
        bool onMouseReleased    (sf::Mouse::Button key)                              override;
        bool onKeyboardPressed  (sf::Keyboard::Key key)                              override;
        bool onKeyboardReleased (sf::Keyboard::Key key)                              override;
        bool onTime             (float deltaSeconds)                                 override;

        bool isOpen();

        virtual void move(sf::Vector2f move);
        virtual void close();

        void setCloseButtonTexture(const char *filename);
    
    protected:
        sf::RectangleShape frame_;
        sf::CircleShape    closeButton_;
        sf::Texture        closeButtonTexture_;
        
        sf::Vector2f mousePos_;

        bool mousePressed_;
        bool mouseInWindow_;
        bool frameCaptured_;
        bool closeButtonCaptured_;
        bool isOpen_;
};

//---------------------------------------------------------------------------

class ContainerWindow : public FrameWindow
{
    public:
        ContainerWindow(sf::Vector2f pos, sf::Vector2f size, float frameHeight, sf::Color color = sf::Color::Black);
        ContainerWindow(float posX, float posY, float width, float height, float frameHeight, sf::Color color = sf::Color::Black);
        
        virtual ~ContainerWindow() = default;

        void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform = sf::Transform::Identity) override;

        bool onMousePressed     (sf::Mouse::Button key)                              override;
        bool onMouseMoved       (int x, int y, const sf::Transform &parentTransform) override;
        bool onMouseReleased    (sf::Mouse::Button key)                              override;
        bool onKeyboardPressed  (sf::Keyboard::Key key)                              override;
        bool onKeyboardReleased (sf::Keyboard::Key key)                              override;
        bool onTime             (float deltaSeconds)                                 override;  

        void close() override;

        void addWindow(FrameWindow *window);
    
    protected:
        std::list<FrameWindow *>windows_;
};

//---------------------------------------------------------------------------

class MainWindow : public ContainerWindow
{
    public:
        MainWindow(sf::RenderWindow &LinuxWinow, sf::Vector2f pos, sf::Vector2f size, float frameHeight, sf::Color color = sf::Color::Black);
        MainWindow(sf::RenderWindow &LinuxWinow, float posX, float posY, float width, float height, float frameHeight, sf::Color color = sf::Color::Black);

        bool onTime(float deltaSeconds) override;

        void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform) override;
        
        void move(sf::Vector2f move) override;
        void close() override;

    private:
        sf::RenderWindow &LinuxWindow_;
        Timer             timer_;
};

//---------------------------------------------------------------------------

#endif //WINDOWS_HPP