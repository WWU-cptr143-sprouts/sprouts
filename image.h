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
 * @retval (variable) (description of variable)
 **/
class ImageLoadError final : public runtime_error
{
    public:
        explicit ImageLoadError(const string &arg)
            : runtime_error(arg)
        {
        }
};

/**
 * @brief Write what the function does here
 *
 * @retval (variable) (description of variable)
 **/
class Image final
{
    public:
        explicit Image(wstring resourceName);
        explicit Image(unsigned w, unsigned h);
        explicit Image(Color c);
        Image();
        Image(nullptr_t)
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
         * @retval (variable) (description of variable)
         **/
        unsigned width() const
        {
            return data->w;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        unsigned height() const
        {
            return data->h;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        operator bool() const
        {
            return data != nullptr;
        }

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
         **/
        bool operator !() const
        {
            return data == nullptr;
        }

        /**
         * @brief Write what the function does here
         *
         * @param l
         * @param r
         *
         * @retval (variable) (description of variable)
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
         * @retval (variable) (description of variable)
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
         * @retval (variable) (description of variable)
         **/
        enum RowOrder
        {
            TopToBottom,
            BottomToTop
        };

        /**
         * @brief Write what the function does here
         *
         * @retval (variable) (description of variable)
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
                : data(data), w(w), h(h), rowOrder(rowOrder), texture(0), textureValid(false)
            {
            }
            data_t(uint8_t * data, shared_ptr<data_t> rt)
                : data(data), w(rt->w), h(rt->h), rowOrder(rt->rowOrder), texture(0), textureValid(false)
            {
            }
            ~data_t();
        };
        shared_ptr<data_t> data;
        static constexpr size_t BytesPerPixel = 4;
        void setRowOrder(RowOrder newRowOrder) const;
        void swapRows(unsigned y1, unsigned y2) const;
        void copyOnWrite();
};
#endif // IMAGE_H
