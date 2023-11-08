#include "./Widgets/Windows/MainWindow.hpp"

//------------------------------------------------------------------------------------------

const int WIDTH  = 1000;
const int HEIGHT = 1000;

//------------------------------------------------------------------------------------------

void pollEvents(sf::RenderWindow& LinuxWindow, ContainerWindow& mainWindow, sf::Transform &defaultTransform);

//------------------------------------------------------------------------------------------

int main()
{
    //System init
    sf::RenderWindow LinuxWindow(sf::VideoMode(WIDTH, HEIGHT), "", sf::Style::None);
    LinuxWindow.setPosition(sf::Vector2i(100, 100));
    LinuxWindow.setFramerateLimit(144);

    sf::Transform defaultTransform = sf::Transform::Identity;
    defaultTransform.scale((sf::Vector2f)LinuxWindow.getSize());

    //Main window
    MainWindow mainWindow(LinuxWindow, 0, 0, 1, 1, 0.05f, sf::Color(128, 128, 128));

    //Main loop
    sf::Clock timer;
    while (LinuxWindow.isOpen())
    {
        pollEvents(LinuxWindow, mainWindow, defaultTransform);
        mainWindow.onTime(timer.restart().asSeconds());

        mainWindow.draw(LinuxWindow, defaultTransform);
        
        LinuxWindow.display();
        LinuxWindow.clear();        
    }

    return 0;
}

//------------------------------------------------------------------------------------------

void pollEvents(sf::RenderWindow& LinuxWindow, ContainerWindow& mainWindow, sf::Transform &defaultTransform)
{
    sf::Event event;
    while (LinuxWindow.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
            {
                LinuxWindow.close();
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                mainWindow.onMousePressed(event.mouseButton.button);
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                mainWindow.onMouseReleased(event.mouseButton.button);
                break;
            }
            case sf::Event::MouseMoved:
            {
                sf::Vector2f mousePos = sf::Transform::Identity.getInverse().transformPoint(
                                                (float)event.mouseMove.x, (float)event.mouseMove.y);
                mainWindow.onMouseMoved((int)mousePos.x, (int)mousePos.y, defaultTransform);
                break;
            }
            case sf::Event::KeyPressed:
            {
                mainWindow.onKeyboardPressed(event.key.code);
                break;
            }
            case sf::Event::KeyReleased:
            {
                mainWindow.onKeyboardReleased(event.key.code);
                break;
            }
            default:
                break;
        }
    }
}

//------------------------------------------------------------------------------------------
