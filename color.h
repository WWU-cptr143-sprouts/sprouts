#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include <cstdint>
#include <ostream>
#include "util.h"

using namespace std;

struct Color final /// RGBA color with rgba in the range 0.0 to 1.0
{
    float r, g, b, a; /// a is opacity -- 0 is transparent and 1 is opaque
    Color(float v, float a = 1)
    {
        r = g = b = v;
        this->a = a;
    }
    Color()
    {
        r = g = b = a = 0;
    }
    Color(float r, float g, float b, float a = 1)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    uint8_t ri() const /// get red value as a byte in the range 0 to 0xFF
    {
        return ifloor(limit(r * 256.0f, 0.0f, 255.0f));
    }
    uint8_t gi() const /// get green value as a byte in the range 0 to 0xFF
    {
        return ifloor(limit(g * 256.0f, 0.0f, 255.0f));
    }
    uint8_t bi() const /// get blue value as a byte in the range 0 to 0xFF
    {
        return ifloor(limit(b * 256.0f, 0.0f, 255.0f));
    }
    uint8_t ai() const /// get alpha value as a byte in the range 0 to 0xFF
    {
        return ifloor(limit(a * 256.0f, 0.0f, 255.0f));
    }
    void ri(uint8_t v) /// set red value as a byte in the range 0 to 0xFF
    {
        r = (unsigned)v * (1.0f / 255.0f);
    }
    void gi(uint8_t v) /// set green value as a byte in the range 0 to 0xFF
    {
        g = (unsigned)v * (1.0f / 255.0f);
    }
    void bi(uint8_t v) /// set blue value as a byte in the range 0 to 0xFF
    {
        b = (unsigned)v * (1.0f / 255.0f);
    }
    void ai(uint8_t v) /// set alpha value as a byte in the range 0 to 0xFF
    {
        a = (unsigned)v * (1.0f / 255.0f);
    }
    friend Color scale(Color l, Color r) /// multiply componentwise
    {
        return Color(l.r * r.r, l.g * r.g, l.b * r.b, l.a * r.a);
    }
    friend Color scale(float l, Color r) /// scales the color but not the alpha
    {
        return Color(l * r.r, l * r.g, l * r.b, r.a);
    }
    friend Color scale(Color l, float r) /// scales the color but not the alpha
    {
        return Color(l.r * r, l.g * r, l.b * r, l.a);
    }
    friend ostream & operator <<(ostream & os, const Color & c) /// writes a color to a ostream
    {
        return os << "RGBA(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    }
};

template <>
inline const Color interpolate<Color>(const float t, const Color a, const Color b)
{
    return Color(interpolate(t, a.r, b.r), interpolate(t, a.g, b.g), interpolate(t, a.b, b.b), interpolate(t, a.a, b.a));
}

#endif // COLOR_H_INCLUDED
