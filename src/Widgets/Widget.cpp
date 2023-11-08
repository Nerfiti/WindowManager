#include "Widget.hpp"

bool Widget::onMousePressed     (sf::Mouse::Button key)                              { return false; }
bool Widget::onMouseMoved       (int x, int y, const sf::Transform &parentTransform) { return false; }
bool Widget::onMouseReleased    (sf::Mouse::Button key)                              { return false; }
bool Widget::onKeyboardPressed  (sf::Keyboard::Key key)                              { return false; }
bool Widget::onKeyboardReleased (sf::Keyboard::Key key)                              { return false; }
bool Widget::onTime             (float deltaSeconds)                                 { return false; }
bool Widget::isFocused          ()                                                   { return isFocused_; }
void Widget::setIsFocused       (bool isFocused)                                     { isFocused_ = isFocused; }