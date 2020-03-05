#ifndef FUNCTIONALFILTER_H
#define FUNCTIONALFILTER_H

#include <QMainWindow>
#include <QtMath>

class FunctionalFilter
{
public:
    virtual uchar function(uchar value) = 0;
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
    const int ADD_BRIGHTNESS = 10;
    uchar function(uchar value) override {
        return qBound(0, value + ADD_BRIGHTNESS, 255);
    }
};

class GammaCorrectionFilter final : public FunctionalFilter
{
public:
    const double GAMMA = 0.8;
    uchar function(uchar value) override {
        return qBound(0.0, (qPow((value/255.0), (1.0/GAMMA)) * 255.0), 255.0);
    }
};

class ContrastFilter final : public FunctionalFilter
{
public:
    const double a = 1.1;
    uchar function(uchar value) override {
        return qBound(0.0, a * (value - 128) + 128, 255.0);
    }
};

#endif // FUNCTIONALFILTER_H
