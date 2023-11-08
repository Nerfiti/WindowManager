#include "cstring"

#include "Filters.hpp"

static sf::Color &operator*=(sf::Color &color, float factor)
{
    constexpr int maxColorVal = 255;
    color.r = std::min((int)(color.r * factor), maxColorVal);
    color.g = std::min((int)(color.g * factor), maxColorVal);
    color.b = std::min((int)(color.b * factor), maxColorVal);

    return color;
}

static sf::Color operator*(const sf::Color &color, float factor)
{
    sf::Color newColor = color;
    return newColor *= factor;
}

static sf::Color &operator+=(sf::Color &color, int rhs)
{
    constexpr int maxColorVal = 255;
    color.r = std::min((color.r + rhs), maxColorVal);
    color.g = std::min((color.g + rhs), maxColorVal);
    color.b = std::min((color.b + rhs), maxColorVal);

    return color;
}

static sf::Color operator+(const sf::Color &color, int rhs)
{
    sf::Color newColor = color;
    return newColor += rhs;
}

static sf::Uint8 average(const sf::Color &color)
{
    return (color.r + color.g + color.b) / 3;
}

//--------------------------------------------------------

FilterMask::FilterMask(size_t width, size_t height):
    isInvert_(false),
    mask_    (new bool[width * height]),
    width_   (width),
    height_  (height)
{
    fill(true);
}

FilterMask::~FilterMask() { delete [] mask_; }

size_t FilterMask::getWidth() const { return width_; }

size_t FilterMask::getHeight() const { return height_; }

bool FilterMask::getPixel(size_t x, size_t y) const
{
    return mask_[width_ * y + x] != isInvert_;
}

void FilterMask::setPixel(size_t x, size_t y, bool value)
{
    mask_[width_ * y + x] = (value != isInvert_);
}

void FilterMask::fill(bool value)
{
    isInvert_ = false;
    size_t size = width_ * height_;
    for (size_t i = 0; i < size; ++i)
        mask_[i] = value;
}

void FilterMask::invert()
{
    isInvert_ = !isInvert_;
}

//--------------------------------------------------------

FilterBrightness::FilterBrightness(int light):
    light_ (light)
    {}

void FilterBrightness::applyFilter(Canvas& canvas, const FilterMask& mask)
{
    sf::RenderTexture &texture = canvas.Texture();
    sf::Vector2u size = texture.getSize();

    sf::Image image = texture.getTexture().copyToImage();
    for (size_t x = 0; x < size.x; ++x)
    {
        for (size_t y = 0; y < size.y; ++y)
        {
            sf::Color color = image.getPixel(x, y) + (mask.getPixel(x, y) ? light_ : 0);

            image.setPixel(x, y, color);
        }
    }

    sf::Texture newTexture;
    newTexture.loadFromImage(image);
    sf::Sprite newImage(newTexture);
    texture.draw(newImage);
}

//--------------------------------------------------------

void FilterInverse::applyFilter(Canvas& canvas, const FilterMask& mask)
{
    sf::RenderTexture &texture = canvas.Texture();
    sf::Vector2u size = texture.getSize();

    sf::Image image = texture.getTexture().copyToImage();
    for (size_t x = 0; x < size.x; ++x)
    {
        for (size_t y = 0; y < size.y; ++y)
        {
            if (!mask.getPixel(x, y))
                continue;

            sf::Color color(255, 255, 255);
            color.r -= image.getPixel(x, y).r;
            color.g -= image.getPixel(x, y).g;
            color.b -= image.getPixel(x, y).b;
            
            image.setPixel(x, y, color);
        }
    }

    sf::Texture newTexture;
    newTexture.loadFromImage(image);
    sf::Sprite newImage(newTexture);
    texture.draw(newImage);
}

//--------------------------------------------------------

void FilterGrayscale::applyFilter(Canvas& canvas, const FilterMask& mask)
{
    sf::RenderTexture &texture = canvas.Texture();
    sf::Vector2u size = texture.getSize();

    sf::Image image = texture.getTexture().copyToImage();
    for (size_t x = 0; x < size.x; ++x)
    {
        for (size_t y = 0; y < size.y; ++y)
        {
            if (!mask.getPixel(x, y))
                continue;

            sf::Uint8 avg = average(image.getPixel(x, y));
            sf::Color color(avg, avg, avg);
            
            image.setPixel(x, y, color);
        }
    }

    sf::Texture newTexture;
    newTexture.loadFromImage(image);
    sf::Sprite newImage(newTexture);
    texture.draw(newImage);
}

//--------------------------------------------------------

void FilterBlackAndWhite::applyFilter(Canvas& canvas, const FilterMask& mask)
{
    sf::RenderTexture &texture = canvas.Texture();
    sf::Vector2u size = texture.getSize();

    sf::Image image = texture.getTexture().copyToImage();
    for (size_t x = 0; x < size.x; ++x)
    {
        for (size_t y = 0; y < size.y; ++y)
        {
            if (!mask.getPixel(x, y))
                continue;

            sf::Uint8 blackOrWhite = (average(image.getPixel(x, y)) > 128) ? 255 : 0;
            sf::Color color(blackOrWhite, blackOrWhite, blackOrWhite);
            
            image.setPixel(x, y, color);
        }
    }

    sf::Texture newTexture;
    newTexture.loadFromImage(image);
    sf::Sprite newImage(newTexture);
    texture.draw(newImage);
}

//--------------------------------------------------------

FilterPalette::FilterPalette():
    lastId_ (0),
    filters_(),
    numberOfFilters_(3)
{
    filters_.push_back(new FilterBrightness(20));
    filters_.push_back(new FilterInverse);
    filters_.push_back(new FilterGrayscale);
    filters_.push_back(new FilterBlackAndWhite);
}

Filter *FilterPalette::getLastFilter()
{
    return getFilter(lastId_);
}

void FilterPalette::setLastFilter(size_t filterId)
{
    lastId_ = filterId;
}

Filter *FilterPalette::getFilter(size_t filterId)
{
    return filters_[filterId];
}

size_t FilterPalette::getFilterCount() const
{
    return numberOfFilters_;
}

//--------------------------------------------------------
