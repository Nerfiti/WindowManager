#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "CanvasWindow.hpp"
#include "../Timer/Timer.hpp"

//---------------------------------------------------------------------------

class MainWindow : public ContainerWindow
{
    public:
        MainWindow(sf::RenderWindow &LinuxWinow, sf::Vector2f pos, sf::Vector2f size, float frameHeight, sf::Color color = sf::Color::Black);
        MainWindow(sf::RenderWindow &LinuxWinow, float posX, float posY, float width, float height, float frameHeight, sf::Color color = sf::Color::Black);

        ~MainWindow();

        bool onKeyboardPressed  (sf::Keyboard::Key key) override;
        bool onKeyboardReleased (sf::Keyboard::Key key) override;
        bool onTime(float deltaSeconds)                 override;

        void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform) override;
        
        void move(sf::Vector2f move) override;
        void close() override;

    private:
        sf::RenderWindow &LinuxWindow_;
        Timer             timer_;

        ToolPalette   *tools_;
        FilterPalette *filters_;

        ContainerWindow *canvasArea_;

        bool isLControlPressed_;

        static constexpr size_t MaxPathLen = 100;
        char path_[MaxPathLen + 1] = "";

        void loadImage();
};

//---------------------------------------------------------------------------

#endif //MAIN_WINDOW_HPP