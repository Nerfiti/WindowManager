#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "../Windows/Windows.hpp"
#include "../../Tools/Tools.hpp"
#include "../../Tools/Filters.hpp"
#include "../Canvas/ScrollBar.hpp"

class ToolPalette;
class FilterPalette;
class ScrollBar;

//----------------------------------------------------------------------

class Canvas : public Window
{
    public:
        Canvas(sf::Vector2f pos, sf::Vector2f size, ToolPalette &tools, FilterPalette &filters, const char *filename = nullptr);
        Canvas(const Canvas&) = delete;
        ~Canvas();

        Canvas &operator=(const Canvas&) = delete;

        virtual void draw(sf::RenderTarget& canvas, const sf::Transform& parentTransform = sf::Transform::Identity) override;

        virtual bool onMousePressed     (sf::Mouse::Button key)                              override;
        virtual bool onMouseMoved       (int x, int y, const sf::Transform &parentTransform) override;
        virtual bool onMouseReleased    (sf::Mouse::Button key)                              override;
        virtual bool onKeyboardPressed  (sf::Keyboard::Key key)                              override;
        virtual bool onKeyboardReleased (sf::Keyboard::Key key)                              override;
        virtual bool onTime             (float deltaSeconds)                                 override;

        sf::RenderTexture  &Texture();
        sf::RectangleShape &Rect();

        void horizontalScroll(int offset);
        void verticalScroll  (int offset);
    
        void zoom(float factor);

    private:
        ToolPalette &tools_;
        FilterPalette &filters_;

        sf::RenderTexture  texture_;

        sf::Vector2f    mousePos_;
        bool isMainButtonPressed_;

        sf::Vector2i viewSize_;
        sf::Vector2i offset_;

        ScrollBar *scrollbar_;        

        static const sf::Mouse::Button MainButton      = sf::Mouse::Left;
        static const sf::Mouse::Button SecondaryButton = sf::Mouse::Right;
        static const sf::Keyboard::Key Modifier1Button = sf::Keyboard::Key::LShift;
        static const sf::Keyboard::Key Modifier2Button = sf::Keyboard::Key::LAlt;
        static const sf::Keyboard::Key Modifier3Button = sf::Keyboard::Key::LControl;
        static const sf::Keyboard::Key ConfirmButton   = sf::Keyboard::Key::Enter;
        static const sf::Keyboard::Key CancelButton    = sf::Keyboard::Key::Escape;

        ///TODO: think about config file
        static const sf::Keyboard::Key LastFilter       = sf::Keyboard::Key::F;
        static const sf::Keyboard::Key BrightnessFilter = sf::Keyboard::Key::I;
        static const sf::Keyboard::Key InverseFilter    = sf::Keyboard::Key::N;
        static const sf::Keyboard::Key GrayscaleFilter  = sf::Keyboard::Key::G;
        static const sf::Keyboard::Key BlackWhiteFilter = sf::Keyboard::Key::W; 

        static const int stdResolutionX = 256;
        static const int stdResolutionY = 256;
};

//----------------------------------------------------------------------

#endif //CANVAS_HPP