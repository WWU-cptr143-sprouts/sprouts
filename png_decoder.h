#ifndef PNG_DECODER_H_INCLUDED
#define PNG_DECODER_H_INCLUDED
#include <string>
#include <cstdint>
#include <stdexcept>
#include "stream.h"
using namespace std;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class PngLoadError final : public IOException
{
    public:
        explicit PngLoadError(const string & arg)
<<<<<<< HEAD
<<<<<<< HEAD
            : IOException(arg)
        {
        }
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

            /**
             * @brief Write what the function does here
             *
             * @param arg
             *
             * @return
             **/
            : IOException(arg)
            {
            }
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
};
/** read and decode png files<br/>
  bytes in RGBA format
  */

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class PngDecoder final
{
    private:
        unsigned w, h;
        uint8_t * data;
        PngDecoder(const PngDecoder &) = delete;
        const PngDecoder &operator =(const PngDecoder &) = delete;
    public:
        explicit PngDecoder(Reader & reader);
<<<<<<< HEAD
<<<<<<< HEAD

        /**
         * @brief Write what the function does here
         *
         * @param rt
         *
         * @return
         **/
        PngDecoder(PngDecoder && rt)
        {
            w = rt.w;
            h = rt.h;
            data = rt.data;
            rt.data = nullptr;
        }
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

        /**
         * @brief Write what the function does here
         *
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
         * @param rt
         *
         * @return
         **/
        PngDecoder(PngDecoder && rt)
        {
            w = rt.w;
            h = rt.h;
            data = rt.data;
            rt.data = nullptr;
        }

        /**
         * @brief Write what the function does here
         *
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
         * @return
         **/
        ~PngDecoder()
        {
            delete []data;
        }

        /**
         * @brief Write what the function does here
         *
         * @param x
         * @param y
         * @param byteNum
         *
         * @return
         **/
        uint8_t operator()(int x, int y, int byteNum) const
        {
            if(x < 0 || (unsigned)x >= w || y < 0 || (unsigned)y >= h || byteNum < 0 || byteNum >= 4)
                throw range_error("index out of range in PngDecoder::operator()(int x, int y, int byteNum) const");
            size_t index = y;
            index *= w;
            index += x;
            index *= 4;
            index += byteNum;
            return data[index];
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        int width() const
        {
            return w;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        int height() const
        {
            return h;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        uint8_t * removeData()
        {
            uint8_t * retval = data;
            data = nullptr;
            return retval;
        }
};
#endif // PNG_DECODER_H_INCLUDED
