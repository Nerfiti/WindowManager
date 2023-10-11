#include "Windows.hpp"

//---------------------------------------------------------------------------

static float lengthInSquare(sf::Vector2f vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

//---------------------------------------------------------------------------

Window::Window(sf::Vector2f pos, sf::Vector2f size, sf::Color color):
    windowRect_   (sf::Vector2f(1, 1)),
    transform_    (),
    color_        (color)
{
    transform_.translate(pos).scale(size);
    windowRect_.setFillColor(color_);
}

Window::Window(float posX, float posY, float width, float height, sf::Color color):
    Window(sf::Vector2f(posX, posY), sf::Vector2f(width, height), color)
    {}

//---------------------------------------------------------------------------

FrameWindow::FrameWindow(sf::Vector2f pos, sf::Vector2f size, float frameHeight, sf::Color color):
    Window              (pos, size, color),
    frame_              (sf::Vector2f(1, 0)),
    closeButton_        (0),
    mousePos_           (sf::Vector2f(-1, -1)),
    mousePressed_       (false),
    mouseInWindow_      (false),
    frameCaptured_      (false),
    closeButtonCaptured_(false),
    isOpen_             (true),
    closeButtonTexture_ ()
{
    if (size.y)
    {
        frame_.setSize(sf::Vector2f(1, frameHeight / size.y));
        closeButton_.setRadius(frameHeight / 3 / size.y);

        float radius = closeButton_.getRadius();
        closeButton_.setOrigin(radius, radius);

        float offset = frameHeight / 2 / size.y;
        closeButton_.setPosition(sf::Vector2f(1 - offset, offset));

        //closeButton_.setFillColor(sf::Color::Cyan);
        frame_.setFillColor(sf::Color::Magenta);
        windowRect_.setFillColor(color_);

        setCloseButtonTexture("./Resources/CloseButton.png");
    }
}

FrameWindow::FrameWindow(float posX, float posY, float width, float height, float frameHeight, sf::Color color):
    FrameWindow(sf::Vector2f(posX, posY), sf::Vector2f(width, height), frameHeight, color)
    {}

void FrameWindow::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    if (!isOpen_)
        return;

    sf::Transform finalTransform = parentTransform * transform_;
    
    canvas.draw(windowRect_, finalTransform);
    canvas.draw(frame_, finalTransform);
    canvas.draw(closeButton_, finalTransform);
}

bool FrameWindow::onMousePressed(sf::Mouse::Button key)
{
    if (mouseInWindow_ && key == sf::Mouse::Button::Left)
    {
        mousePressed_ = true;

        float radius = closeButton_.getRadius();
        if (lengthInSquare(closeButton_.getPosition() - mousePos_) < radius * radius)
        {
            closeButtonCaptured_ = true;
            //closeButton_.setFillColor(sf::Color::Red);
        }
        else if (frame_.getGlobalBounds().contains(mousePos_))
        {
            frameCaptured_ = true;
        }

        return true;
    }

    return false;
}

bool FrameWindow::onMouseMoved(float x, float y)
{
    sf::Vector2f newMousePos = transform_.getInverse().transformPoint(sf::Vector2f(x, y));
    sf::Vector2f deltaMouse  = newMousePos - mousePos_; 

    if (frameCaptured_)
    {
        move(deltaMouse);   
        return true;
    }

    mousePos_ = newMousePos;

    if (windowRect_.getLocalBounds().contains(mousePos_))
    {
        mouseInWindow_ = true;

        float radius = closeButton_.getRadius();
        if (! (lengthInSquare(closeButton_.getPosition() - mousePos_) < radius * radius))
        {
            closeButtonCaptured_ = false;
        }

        return true;
    }
    else
    {
        frameCaptured_ = false;
        mouseInWindow_ = false;
        mousePressed_  = false;
    }

    return false;
}

bool FrameWindow::onMouseReleased(sf::Mouse::Button key)
{
    if (mouseInWindow_ && key == sf::Mouse::Button::Left)
    {
        mousePressed_ = false;

        if (lengthInSquare(closeButton_.getPosition() - mousePos_) < closeButton_.getRadius() && closeButtonCaptured_)
        {
            windowRect_.setFillColor(color_);
            closeButtonCaptured_ = false;

            close();
        }
        else if (frame_.getGlobalBounds().contains(mousePos_))
        {
            frameCaptured_ = false;
        }

        return true;
    }

    return false;
}

bool FrameWindow::onKeyboardPressed(sf::Keyboard::Key key)
{
    return false;
}

bool FrameWindow::onKeyboardReleased(sf::Keyboard::Key key)
{
    return false;
}

bool FrameWindow::onTime(float deltaSeconds)
{
    return false;
}  

void FrameWindow::close()
{
    isOpen_ = false;
}

void FrameWindow::move(sf::Vector2f move)
{
    transform_.translate(move);
}

void FrameWindow::setCloseButtonTexture(const char *filename)
{
    closeButtonTexture_.loadFromFile(filename);
    closeButton_.setTexture(&closeButtonTexture_);
}

bool FrameWindow::isOpen()
{
    return isOpen_;
}

//---------------------------------------------------------------------------

ContainerWindow::ContainerWindow(sf::Vector2f pos, sf::Vector2f size, float frameHeight, sf::Color color):
    FrameWindow(pos, size, frameHeight, color)
    {}

ContainerWindow::ContainerWindow(float posX, float posY, float width, float height, float frameHeight, sf::Color color):
    FrameWindow(posX, posY, width, height, frameHeight, color)
    {}

void ContainerWindow::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    FrameWindow::draw(canvas, parentTransform);

    sf::Transform finalTransform = parentTransform * transform_;
    for (auto it = windows_.rbegin(); it != windows_.rend(); ++it)
    {
        (*it)->draw(canvas, finalTransform);
    }
}

bool ContainerWindow::onMousePressed(sf::Mouse::Button key)
{
    bool checked = FrameWindow::onMousePressed(key);

    for (auto it = windows_.begin(); it != windows_.end(); ++it)
    {
        if ((*it)->onMousePressed(key))
        {
            FrameWindow *activeWindow = *it;
            windows_.erase(it);
            windows_.insert(windows_.begin(), activeWindow);
            return true;
        }
    }

    return checked;
}

bool ContainerWindow::onMouseMoved(float x, float y)
{
    FrameWindow::onMouseMoved(x, y);
    sf::Vector2f newCoords = transform_.getInverse().transformPoint(sf::Vector2f(x, y));
    for (auto &it : windows_)
    {
        if (it->onMouseMoved(newCoords.x, newCoords.y))
            return true;
    }

    return false;
}

bool ContainerWindow::onMouseReleased(sf::Mouse::Button key)
{
    FrameWindow::onMouseReleased(key);

    for (auto it = windows_.begin(); it != windows_.end(); ++it)
    {
        if ((*it)->onMouseReleased(key))
        {
            if (!(*it)->isOpen())
                windows_.erase(it);

            return true;
        }
    }

    return false;
}

bool ContainerWindow::onKeyboardPressed(sf::Keyboard::Key key)   
{
    FrameWindow::onKeyboardPressed(key);

    for (auto &it : windows_)
    {
        if (it->onKeyboardPressed(key))
            return true;
    }

    return false;
}

bool ContainerWindow::onKeyboardReleased(sf::Keyboard::Key key)   
{
    FrameWindow::onKeyboardReleased(key);

    for (auto &it : windows_)
    {
        if (it->onKeyboardReleased(key))
            return true;
    }

    return false;
}

bool ContainerWindow::onTime(float deltaSeconds)
{
    FrameWindow::onTime(deltaSeconds);

    for (auto &it : windows_)
    {
        if (it->onTime(deltaSeconds))
            return true;
    }

    return false;
}

void ContainerWindow::addWindow(FrameWindow *window)
{
    windows_.push_back(window);
}

void ContainerWindow::close()
{
    FrameWindow::close();
    for (auto it : windows_)
        it->close();
}

//---------------------------------------------------------------------------

MainWindow::MainWindow(sf::RenderWindow &LinuxWindow, sf::Vector2f pos, sf::Vector2f size, float frameHeight, sf::Color color):
    ContainerWindow(pos, size, frameHeight, color),
    LinuxWindow_(LinuxWindow),
    timer_(sf::Vector2f(1 - 0.1, 1 - 0.1), 0.1)
    {}

MainWindow::MainWindow(sf::RenderWindow &LinuxWindow, float posX, float posY, float width, float height, float frameHeight, sf::Color color):
    MainWindow(LinuxWindow, sf::Vector2f(posX, posY), sf::Vector2f(width, height), frameHeight, color)
    {}

bool MainWindow::onTime(float deltaSeconds)
{
    ContainerWindow::onTime(deltaSeconds);
    timer_.onTime(deltaSeconds);

    return false;
}

void MainWindow::draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform)
{
    ContainerWindow::draw(canvas, parentTransform);
    timer_.draw(canvas, parentTransform);
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

//---------------------------------------------------------------------------
