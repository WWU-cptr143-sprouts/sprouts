#ifndef TEXTURE_DESCRIPTOR_H_INCLUDED
#define TEXTURE_DESCRIPTOR_H_INCLUDED
#include "image.h"
#include "util.h"

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct TextureDescriptor
{
    Image image;
    float minU, maxU, minV, maxV;
    TextureDescriptor(Image image = Image())

        /**
         * @brief Write what the function does here
         *
         * @param image
         * @param 0
         * @param 1
         * @param 0
         * @param 1
         *
         * @return
         **/
        : image(image), minU(0), maxU(1), minV(0), maxV(1)
        {
        }
    TextureDescriptor(Image image, float minU, float maxU, float minV, float maxV)

        /**
         * @brief Write what the function does here
         *
         * @param image
         * @param minU
         * @param maxU
         * @param minV
         * @param maxV
         *
         * @return
         **/
        : image(image), minU(minU), maxU(maxU), minV(minV), maxV(maxV)
<<<<<<< HEAD
    {
    }
=======
        {
        }
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    operator bool() const
    {
        return (bool)image;
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    bool operator !() const
    {
        return !image;
    }

    /**
     * @brief Write what the function does here
     *
     * @param minU
     * @param maxU
     * @param minV
     * @param maxV
     *
     * @return
     **/
    TextureDescriptor subTexture(const float minU, const float maxU, const float minV, const float maxV) const
    {
        return TextureDescriptor(image,
                interpolate(minU, this->minU, this->maxU),
                interpolate(maxU, this->minU, this->maxU),
                interpolate(minV, this->minV, this->maxV),
                interpolate(maxV, this->minV, this->maxV));
    }
};
#endif // TEXTURE_DESCRIPTOR_H_INCLUDED
