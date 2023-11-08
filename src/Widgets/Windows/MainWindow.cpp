#include "cstring"

#include "MainWindow.hpp"

//---------------------------------------------------------------------------

MainWindow::MainWindow(sf::RenderWindow &LinuxWindow, sf::Vector2f pos, sf::Vector2f size, float frameHeight, sf::Color color):
    ContainerWindow   (pos, size, frameHeight, color),
    LinuxWindow_      (LinuxWindow),
    timer_            (sf::Vector2f(0.05f, 1 - 0.05f), 0.05f),
    tools_            (new ToolPalette(sf::Color::White)),
    filters_          (new FilterPalette),
    canvasArea_       (nullptr),
    isLControlPressed_(false)
{
    ///TODO: Config file

    //Tmp windows without interactions
    addWindow(new FrameWindow(0, 0.05f, 1, 0.02f, 0, sf::Color(32, 32, 32)));
    addWindow(new FrameWindow(0, 0.07f, 0.1f, 1, 0, sf::Color(64, 64, 64)));

    canvasArea_ = new ContainerWindow(0.1f, 0.07f, 0.9f, 0.93f, 0.0f, sf::Color(150, 150, 150));
    addWindow(canvasArea_);
}

MainWindow::MainWindow(sf::RenderWindow &LinuxWindow, float posX, float posY, float width, float height, float frameHeight, sf::Color color):
    MainWindow(LinuxWindow, sf::Vector2f(posX, posY), sf::Vector2f(width, height), frameHeight, color)
    {}

MainWindow::~MainWindow()
{
    delete tools_;
    delete filters_;
}

bool MainWindow::onKeyboardPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::LControl:
        {
            isLControlPressed_ = true;
            return true;
        }

        case sf::Keyboard::O:
        {
            if (isLControlPressed_)
            {    
                loadImage();
                return true;
            }
        }
    }

    return ContainerWindow::onKeyboardPressed(key);
}

bool MainWindow::onKeyboardReleased(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::LControl:
        {
            isLControlPressed_ = false;
            return true;
        }

        default:
            return ContainerWindow::onKeyboardReleased(key);
    }
}

bool MainWindow::onTime(float deltaSeconds)
{
    ContainerWindow::onTime(deltaSeconds);
    timer_.onTime(deltaSeconds);

    return false;
}

void MainWindow::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    ContainerWindow::draw(canvas, parentTransform);
 
    sf::Transform finalTransform = parentTransform * transform_;
    timer_.draw(canvas, finalTransform);
}

void MainWindow::move(sf::Vector2f move)
{
    sf::Vector2u winSize = LinuxWindow_.getSize();
    move.x *= winSize.x;
    move.y *= winSize.y;

    LinuxWindow_.setPosition(LinuxWindow_.getPosition() + sf::Vector2i(move.x, move.y));
}

void MainWindow::close()
{
    LinuxWindow_.close();
}

void MainWindow::loadImage()
{
    FILE *stream = popen("zenity --file-selection", "r");
    if (!fgets(path_, MaxPathLen, stream))
    {
        printf("Error opening stream in \"popen()\"\n");
        return;
    }

    size_t endId = strcspn(path_, "\n\r");
    if (endId == MaxPathLen)
    {
        printf("Filename is too long.\n");
        return;
    }
    path_[endId] = '\0';            

    canvasArea_->addWindow(new CanvasWindow(0.2f, 0.2f, 0.7f, 0.7f, 0.05f, *tools_, *filters_, path_));
}
