#include "./Widgets/Windows/Windows.hpp"

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

    sf::Transform defaultTransform = sf::Transform::Identity;
    defaultTransform.scale((sf::Vector2f)LinuxWindow.getSize());

    //Main window
    MainWindow mainWindow(LinuxWindow, 0, 0, 1, 1, 0.05, sf::Color::Green);
    
    //Adding windows
    const int numberOfTestWindows = 6;
    FrameWindow *testWindows[numberOfTestWindows] =                                 {
            new ContainerWindow(  0, 0.2, 0.5, 0.5, 0.05, sf::Color::Yellow),
            new     FrameWindow(0.6, 0.2, 0.4, 0.4, 0.05, sf::Color::White ),
            new     FrameWindow(0.4, 0.6, 0.4, 0.4, 0.05, sf::Color::Black ),
            
            new     FrameWindow(  0, 0.2, 0.5, 0.5, 0.05, sf::Color::Blue  ),
            new     FrameWindow(0.6, 0.2, 0.4, 0.4, 0.05, sf::Color::White ),
            new     FrameWindow(0.1, 0.6, 0.4, 0.4, 0.05, sf::Color::Black )
                                                                                };
    for (int i = 0; i < numberOfTestWindows / 2; ++i)
    {
        mainWindow.addWindow(testWindows[i]);
        ((ContainerWindow *)testWindows[0])->addWindow(testWindows[i + numberOfTestWindows / 2]);
    }

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

    delete (ContainerWindow *)testWindows[0];
    for (int i = 1; i < numberOfTestWindows; ++i)
        delete testWindows[i];

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
                sf::Vector2f mousePos = defaultTransform.getInverse().transformPoint(event.mouseMove.x, event.mouseMove.y);
                mainWindow.onMouseMoved(mousePos.x, mousePos.y);
                break;
            }
            case sf::Event::KeyPressed:
            {
                mainWindow.onKeyboardPressed(event.key.code);

                if (event.key.code == sf::Keyboard::Escape)
                    LinuxWindow.close();

                break;
            }
        }
    }
}

//------------------------------------------------------------------------------------------
