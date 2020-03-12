#ifndef FUNCTIONALFILTER_H
#define FUNCTIONALFILTER_H

#include <QMainWindow>
#include <QtMath>

class FunctionalFilter
{
public:
    virtual uchar function(uchar value) = 0;
    double MAX_SLIDER = 50;
};

class InverseFilter final : public FunctionalFilter
{
public:
     uchar function(uchar value) override {
        return 255 - value;
    }
};

class BrightnessCorrectionFilter final : public FunctionalFilter
{
public:
    BrightnessCorrectionFilter(double val = 60)
    {
        ADD_BRIGHTNESS = val - MAX_SLIDER;
    }
    int ADD_BRIGHTNESS;
    uchar function(uchar value) override {
        return qBound(0, value + ADD_BRIGHTNESS, 255);
    }
};

class GammaCorrectionFilter final : public FunctionalFilter
{
public:
    GammaCorrectionFilter(double val = 60)
    {
        GAMMA = val / MAX_SLIDER;
    }
    double GAMMA;
    uchar function(uchar value) override {
        return qBound(0.0, (qPow((value/255.0), (1.0/GAMMA)) * 255.0), 255.0);
    }
};

class ContrastFilter final : public FunctionalFilter
{
public:
    ContrastFilter(double val = 60)
    {
        a = val / MAX_SLIDER;
    }
    double a;
    uchar function(uchar value) override {
        return qBound(0.0, a * (value - 128) + 128, 255.0);
    }
};

#endif // FUNCTIONALFILTER_H
