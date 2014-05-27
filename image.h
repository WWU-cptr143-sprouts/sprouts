#ifndef IMAGE_H
#define IMAGE_H
#include <cstdint>
#include <cwchar>
#include <string>
#include <stdexcept>
#include <mutex>
#include <memory>
#include "color.h"
#include "stream.h"
using namespace std;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class ImageLoadError final : public runtime_error
{
    public:
        explicit ImageLoadError(const string &arg)
<<<<<<< HEAD
<<<<<<< HEAD
            : runtime_error(arg)
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
            : runtime_error(arg)
            {
            }
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class Image final
{
    public:
        explicit Image(wstring resourceName);
        explicit Image(unsigned w, unsigned h);
        explicit Image(Color c);
        Image();
        Image(nullptr_t)
<<<<<<< HEAD
<<<<<<< HEAD
            : Image()
=======

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            : Image()
            {
            }
        void setPixel(int x, int y, Color c);
        Color getPixel(int x, int y) const;
        void bind() const;
        static void unbind();

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        unsigned width() const
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        {
            return data->w;
        }
<<<<<<< HEAD
        void setPixel(int x, int y, Color c);
        Color getPixel(int x, int y) const;
        void bind() const;
        static void unbind();

=======

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            : Image()
            {
            }
        void setPixel(int x, int y, Color c);
        Color getPixel(int x, int y) const;
        void bind() const;
        static void unbind();

>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======

>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
<<<<<<< HEAD
        unsigned width() const
        {
            return data->w;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        unsigned height() const
        {
            return data->h;
<<<<<<< HEAD
=======
=======
        unsigned height() const
        {
            return data->h;
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        operator bool() const
        {
            return data != nullptr;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        bool operator !() const
        {
            return data == nullptr;
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        }

        /**
         * @brief Write what the function does here
         *
<<<<<<< HEAD
<<<<<<< HEAD
         * @return
         **/
        operator bool() const
        {
            return data != nullptr;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        bool operator !() const
        {
            return data == nullptr;
        }

        /**
         * @brief Write what the function does here
         *
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
         * @param l
         * @param r
         *
         * @return
         **/
        friend bool operator ==(Image l, Image r)
        {
            return l.data == r.data;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @return
         **/
        friend bool operator !=(Image l, Image r)
        {
            return l.data != r.data;
        }
        /*void write(Writer &writer, Client &client) const;
          static Image read(Reader &reader, Client &client);*/
    private:

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        enum RowOrder
        {
            TopToBottom,
            BottomToTop
        };

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        struct data_t
        {
            uint8_t * const data;
            const unsigned w, h;
            RowOrder rowOrder;
            uint32_t texture;
            size_t graphicsNumber = -1;
            bool textureValid;
            mutex lock;
            data_t(uint8_t * data, unsigned w, unsigned h, RowOrder rowOrder)
<<<<<<< HEAD
<<<<<<< HEAD
                : data(data), w(w), h(h), rowOrder(rowOrder), texture(0), textureValid(false)
            {
            }
            data_t(uint8_t * data, shared_ptr<data_t> rt)
                : data(data), w(rt->w), h(rt->h), rowOrder(rt->rowOrder), texture(0), textureValid(false)
            {
            }
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

                /**
                 * @brief Write what the function does here
                 *
                 * @param data
                 * @param w
                 * @param h
                 * @param rowOrder
                 * @param 0
                 * @param false
                 *
                 * @return
                 **/
                : data(data), w(w), h(h), rowOrder(rowOrder), texture(0), textureValid(false)
                {
                }
            data_t(uint8_t * data, shared_ptr<data_t> rt)

                /**
                 * @brief Write what the function does here
                 *
                 * @param data
                 * @param w
                 * @param h
                 * @param rowOrder
                 * @param 0
                 * @param false
                 *
                 * @return
                 **/
                : data(data), w(rt->w), h(rt->h), rowOrder(rt->rowOrder), texture(0), textureValid(false)
                {
                }
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            ~data_t();
        };
        shared_ptr<data_t> data;
        static constexpr size_t BytesPerPixel = 4;
        void setRowOrder(RowOrder newRowOrder) const;
        void swapRows(unsigned y1, unsigned y2) const;
        void copyOnWrite();
};
#endif // IMAGE_H
