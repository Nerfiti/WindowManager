#ifndef FILTERS_HPP
#define FILTERS_HPP

#include "../Widgets/Canvas/Canvas.hpp"

class Canvas;

//--------------------------------------------------------

class FilterMask
{
    public:
        FilterMask(size_t width, size_t height);
        FilterMask(const FilterMask&) = delete;
        ~FilterMask();
        
        FilterMask &operator=(const FilterMask&) = delete;

        size_t getWidth() const;
        size_t getHeight() const;

        bool getPixel(size_t x, size_t y) const;
        void setPixel(size_t x, size_t y, bool value);

        void fill(bool value);

        void invert();
    
    private:
        bool isInvert_;
        
        bool  *mask_;
        size_t width_;
        size_t height_;
};

//--------------------------------------------------------

class Filter
{
    public:
        virtual ~Filter() = default;

        virtual void applyFilter(Canvas& canvas, const FilterMask& mask) = 0;
};

class FilterBrightness : public Filter
{
    public:
        FilterBrightness(int light);

        void applyFilter(Canvas& canvas, const FilterMask& mask) final override;

    private:
        int light_;
};

class FilterInverse : public Filter
{
    public:
        void applyFilter(Canvas& canvas, const FilterMask& mask) override;
};

class FilterGrayscale : public Filter
{
    public:
        void applyFilter(Canvas& canvas, const FilterMask& mask) override;
};

class FilterBlackAndWhite : public Filter
{
    public:
        void applyFilter(Canvas& canvas, const FilterMask& mask) override;
};

//--------------------------------------------------------

class FilterPalette
{
    public:
        FilterPalette();

        Filter *getLastFilter();
        void setLastFilter(size_t filterId);

        Filter *getFilter(size_t filterId);
        size_t getFilterCount() const;

    private:
        size_t lastId_;
        std::vector<Filter *> filters_;        

        size_t numberOfFilters_;        
};

//--------------------------------------------------------

#endif //FILTERS_HPP