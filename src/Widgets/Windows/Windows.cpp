#include "Windows.hpp"

//---------------------------------------------------------------------------

static constexpr float Epsilon = 1e-8f;

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
    closeButtonTexture_ (),
    isOpen_             (true)
{
    if (size.y > Epsilon)
    {
        frame_.setSize(sf::Vector2f(1, frameHeight / size.y));
        closeButton_.setRadius(frameHeight / 3 / size.y);

        float radius = closeButton_.getRadius();
        closeButton_.setOrigin(radius, radius);

        float offset = frameHeight / 2 / size.y;
        closeButton_.setPosition(sf::Vector2f(1 - offset, offset));

        frame_.setFillColor(sf::Color(64, 64, 64));
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
        sf::Vector2f localMousePos = mousePos_;

        float radius = closeButton_.getRadius();
        if (lengthInSquare(closeButton_.getPosition() - localMousePos) < radius * radius)
        {
            closeButtonCaptured_ = true;
        }
        else if (frame_.getGlobalBounds().contains(localMousePos))
        {
            frameCaptured_ = true;
        }

        return true;
    }

    return false;
}

bool FrameWindow::onMouseMoved(int x, int y, const sf::Transform &parentTransform)
{
    sf::Vector2f newMousePos = (parentTransform * transform_).getInverse().transformPoint(sf::Vector2f(x, y));

    sf::Vector2f deltaMouse = newMousePos - mousePos_;

    if (frameCaptured_)
    {
        move(deltaMouse);
        return true;
    }

    mousePos_ = newMousePos;
    
    if (windowRect_.getLocalBounds().contains(newMousePos))
    {
        mouseInWindow_ = true;

        float radius = closeButton_.getRadius();
        if (! (lengthInSquare(closeButton_.getPosition() - newMousePos) < radius * radius))
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
        sf::Vector2f localMousePos = mousePos_;

        if (lengthInSquare(closeButton_.getPosition() - localMousePos) < closeButton_.getRadius() && closeButtonCaptured_)
        {
            windowRect_.setFillColor(color_);
            closeButtonCaptured_ = false;

            close();
        }
        else if (frame_.getGlobalBounds().contains(localMousePos))
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
    FrameWindow(pos, size, frameHeight, color),
    windows_()
    {}

ContainerWindow::ContainerWindow(float posX, float posY, float width, float height, float frameHeight, sf::Color color):
    ContainerWindow(sf::Vector2f(posX, posY), sf::Vector2f(width, height), frameHeight, color)
    {}

ContainerWindow::~ContainerWindow()
{
    for (auto it : windows_)
        delete it;
}

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
    if (!checked)
        return false;
    
    checked = false;

    std::_List_iterator<FrameWindow *> target = windows_.end();
    for (auto it = windows_.begin(); it != windows_.end(); ++it)
    {
        if (!checked && (*it)->onMousePressed(key))
        {
            target = it;
            checked = true;
            (*it)->setIsFocused(true);
            continue;
        }

        (*it)->setIsFocused(false);
    }

    if (target != windows_.end())
    {
        FrameWindow *activeWindow = *target;
        windows_.erase(target);
        windows_.insert(windows_.begin(), activeWindow);
    }

    return true;
}

bool ContainerWindow::onMouseMoved(int x, int y, const sf::Transform &parentTransform)
{
    if (!FrameWindow::onMouseMoved(x, y, parentTransform))
        return false;

    sf::Transform finalTransform = parentTransform * transform_;
    
    for (auto &it : windows_)
    {
        if (it->onMouseMoved(x, y, finalTransform))
            return true;
    }

    return true;
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
    bool checked = FrameWindow::onKeyboardPressed(key);

    for (auto &it : windows_)
    {
        if (!it->onKeyboardPressed(key))
            continue;
    
        return true;
    }

    return checked;
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
    windows_.push_front(window);
}

void ContainerWindow::close()
{
    FrameWindow::close();
    for (auto it : windows_)
        it->close();
}

//---------------------------------------------------------------------------
