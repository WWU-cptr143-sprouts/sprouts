#ifndef STREAM_H
#define STREAM_H
#include <cstdint>
#include <stdexcept>
#include <cwchar>
#include <string>
#include <cstdio>
#include <cstring>
#include <memory>
#include <list>
#include <sstream>
#include "util.h"
#ifdef DEBUG_STREAM

/**
 * @brief Write what the function does here
 *
 * @return
 **/
#include <iostream>
#define DUMP_V(fn, v) do {cerr << #fn << ": read " << v << endl;} while(false)
#else
#define DUMP_V(fn, v)
#endif
using namespace std;

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class IOException : public runtime_error
{
    public:
        explicit IOException(const string & msg)
<<<<<<< HEAD
<<<<<<< HEAD
            : runtime_error(msg)
        {
        }
        explicit IOException(exception * e, bool deleteIt = true)
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

            /**
             * @brief Write what the function does here
             *
             * @param msg
             *
             * @return
             **/
            : runtime_error(msg)
            {
            }
        explicit IOException(exception * e, bool deleteIt = true)

            /**
             * @brief Write what the function does here
             *
             * @param e
             * @param nullptr
             *
             * @return
             **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            : runtime_error((dynamic_cast<IOException *>(e) == nullptr) ? string("IO Error : ") + e->what() : string(e->what()))
            {
                if(deleteIt)
                    delete e;
            }
        explicit IOException(exception & e, bool deleteIt = false)
<<<<<<< HEAD
<<<<<<< HEAD
            : IOException(&e, deleteIt)
        {
        }
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

            /**
             * @brief Write what the function does here
             *
             * @param e
             * @param deleteIt
             *
             * @return
             **/
            : IOException(&e, deleteIt)
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
class EOFException final : public IOException
{
    public:
        explicit EOFException()
<<<<<<< HEAD
<<<<<<< HEAD
            : IOException("IO Error : reached end of file")
        {
        }
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            : IOException("IO Error : reached end of file")
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
class NoStreamsLeftException final : public IOException
{
    public:
        explicit NoStreamsLeftException()
<<<<<<< HEAD
<<<<<<< HEAD
            : IOException("IO Error : no streams left")
        {
        }
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            : IOException("IO Error : no streams left")
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
class UTFDataFormatException final : public IOException
{
    public:
        explicit UTFDataFormatException()
<<<<<<< HEAD
<<<<<<< HEAD
            : IOException("IO Error : invalid UTF data")
        {
        }
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            : IOException("IO Error : invalid UTF data")
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
class InvalidDataValueException final : public IOException
{
    public:
        explicit InvalidDataValueException(string msg)
<<<<<<< HEAD
<<<<<<< HEAD
            : IOException(msg)
        {
        }
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

            /**
             * @brief Write what the function does here
             *
             * @param msg
             *
             * @return
             **/
            : IOException(msg)
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
class Reader
{
    private:
        template <typename T>

            /**
             * @brief Write what the function does here
             *
             * @param v
             * @param min
             * @param max
             *
             * @return
             **/
            static T limitAfterRead(T v, T min, T max)
            {

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                /**
                 * @brief Write what the function does here
                 *
                 * @param max
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                if(v < min || v > max)
                {
                    ostringstream os;
                    os << "read value out of range : " << v;
                    throw InvalidDataValueException(os.str());
                }
                return v;
            }
    public:

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        Reader()
        {
        }
        Reader(const Reader &) = delete;
        const Reader & operator =(const Reader &) = delete;

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual ~Reader()
        {
        }
        virtual uint8_t readByte() = 0;

        /**
         * @brief Write what the function does here
         *
         * @param array
         * @param count
         **/
        void readBytes(uint8_t * array, size_t count)
        {

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            /**
             * @brief Write what the function does here
             *
             * @return
             **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            for(size_t i = 0; i < count; i++)
            {
                array[i] = readByte();
            }
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        uint8_t readU8()
        {
            uint8_t retval = readByte();
            DUMP_V(readU8, (unsigned)retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        int8_t readS8()
        {
            int8_t retval = readByte();
            DUMP_V(readS8, (int)retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        uint16_t readU16()
        {
            uint16_t v = readU8();
            uint16_t retval = (v << 8) | readU8();
            DUMP_V(readU16, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        int16_t readS16()
        {
            int16_t retval = readU16();
            DUMP_V(readS16, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        uint32_t readU32()
        {
            uint32_t v = readU16();
            uint32_t retval = (v << 16) | readU16();
            DUMP_V(readU32, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        int32_t readS32()
        {
            int32_t retval = readU32();
            DUMP_V(readS32, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        uint64_t readU64()
        {
            uint64_t v = readU32();
            uint64_t retval = (v << 32) | readU32();
            DUMP_V(readU64, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        int64_t readS64()
        {
            int64_t retval = readU64();
            DUMP_V(readS64, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        float readF32()
        {
            static_assert(sizeof(float) == sizeof(uint32_t), "float is not 32 bits");

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            union
            {
                uint32_t ival;
                float fval;
            };
            ival = readU32();
            float retval = fval;
            DUMP_V(readF32, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        double readF64()
        {
            static_assert(sizeof(double) == sizeof(uint64_t), "double is not 64 bits");

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            union
<<<<<<< HEAD
<<<<<<< HEAD
            {
                uint64_t ival;
                double fval;
            };
            ival = readU64();
            double retval = fval;
            DUMP_V(readF64, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        bool readBool()
        {
            return readU8() != 0;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        wstring readString()
        {
            wstring retval = L"";

            for(;;)
            {
                uint32_t b1 = readU8();

=======
            {
                uint64_t ival;
                double fval;
            };
            ival = readU64();
            double retval = fval;
            DUMP_V(readF64, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        bool readBool()
        {
            return readU8() != 0;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        wstring readString()
        {
            wstring retval = L"";

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            for(;;)
            {
=======
            {
                uint64_t ival;
                double fval;
            };
            ival = readU64();
            double retval = fval;
            DUMP_V(readF64, retval);
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        bool readBool()
        {
            return readU8() != 0;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        wstring readString()
        {
            wstring retval = L"";

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            for(;;)
            {
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                uint32_t b1 = readU8();

                /**
                 * @brief Write what the function does here
                 *
                 * @param 0
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                if(b1 == 0)
                {
                    DUMP_V(readString, "\"" + wstringToString(retval) + "\"");
                    return retval;
                }

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                /**
                 * @brief Write what the function does here
                 *
                 * @param 0x80
                 * @param 0
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                else if((b1 & 0x80) == 0)
                {
                    retval += (wchar_t)b1;
                }

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                /**
                 * @brief Write what the function does here
                 *
                 * @param 0xE0
                 * @param 0xC0
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                else if((b1 & 0xE0) == 0xC0)
                {
                    uint32_t b2 = readU8();
                    if((b2 & 0xC0) != 0x80)
                        throw UTFDataFormatException();
                    uint32_t v = b2 & 0x3F;
                    v |= (b1 & 0x1F) << 6;
                    retval += (wchar_t)v;
                }

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                /**
                 * @brief Write what the function does here
                 *
                 * @param 0xF0
                 * @param 0xE0
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                else if((b1 & 0xF0) == 0xE0)
                {
                    uint32_t b2 = readU8();
                    if((b2 & 0xC0) != 0x80)
                        throw UTFDataFormatException();
                    uint32_t b3 = readU8();
                    if((b3 & 0xC0) != 0x80)
                        throw UTFDataFormatException();
                    uint32_t v = b3 & 0x3F;
                    v |= (b2 & 0x3F) << 6;
                    v |= (b1 & 0xF) << 12;
                    retval += (wchar_t)v;
                }

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                /**
                 * @brief Write what the function does here
                 *
                 * @param 0xF8
                 * @param 0xF0
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                else if((b1 & 0xF8) == 0xF0)
                {
                    uint32_t b2 = readU8();
                    if((b2 & 0xC0) != 0x80)
                        throw UTFDataFormatException();
                    uint32_t b3 = readU8();
                    if((b3 & 0xC0) != 0x80)
                        throw UTFDataFormatException();
                    uint32_t b4 = readU8();
                    if((b4 & 0xC0) != 0x80)
                        throw UTFDataFormatException();
                    uint32_t v = b4 & 0x3F;
                    v |= (b3 & 0x3F) << 6;
                    v |= (b2 & 0x3F) << 12;
                    v |= (b1 & 0x7) << 18;
                    if(v >= 0x10FFFF)
                        throw UTFDataFormatException();
                    retval += (wchar_t)v;
                }
                else
                    throw UTFDataFormatException();
            }
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        uint8_t readLimitedU8(uint8_t min, uint8_t max)
        {
            return limitAfterRead(readU8(), min, max);
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        int8_t readLimitedS8(int8_t min, int8_t max)
        {
            return limitAfterRead(readS8(), min, max);
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        uint16_t readLimitedU16(uint16_t min, uint16_t max)
        {
            return limitAfterRead(readU16(), min, max);
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        int16_t readLimitedS16(int16_t min, int16_t max)
        {
            return limitAfterRead(readS16(), min, max);
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        uint32_t readLimitedU32(uint32_t min, uint32_t max)
        {
            return limitAfterRead(readU32(), min, max);
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        int32_t readLimitedS32(int32_t min, int32_t max)
        {
            return limitAfterRead(readS32(), min, max);
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        uint64_t readLimitedU64(uint64_t min, uint64_t max)
        {
            return limitAfterRead(readU64(), min, max);
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        int64_t readLimitedS64(int64_t min, int64_t max)
        {
            return limitAfterRead(readS64(), min, max);
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        float readFiniteF32()
        {
            float retval = readF32();

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            /**
             * @brief Write what the function does here
             *
             * @param retval
             *
             * @return
             **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            if(!isfinite(retval))
            {
                throw InvalidDataValueException("read value is not finite");
            }
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        double readFiniteF64()
        {
            double retval = readF64();

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            /**
             * @brief Write what the function does here
             *
             * @param retval
             *
             * @return
             **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            if(!isfinite(retval))
            {
                throw InvalidDataValueException("read value is not finite");
            }
            return retval;
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        float readLimitedF32(float min, float max)
        {
            return limitAfterRead(readFiniteF32(), min, max);
        }

        /**
         * @brief Write what the function does here
         *
         * @param min
         * @param max
         *
         * @return
         **/
        double readLimitedF64(double min, double max)
        {
            return limitAfterRead(readFiniteF64(), min, max);
        }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class Writer
{
    public:

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        Writer()
        {
        }
        Writer(const Writer &) = delete;
        const Writer & operator =(const Writer &) = delete;

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual ~Writer()
        {
        }
        virtual void writeByte(uint8_t) = 0;

        /**
         * @brief Write what the function does here
         *
         virtual void flush()
         {
         }

        /**
         * @brief Write what the function does here
         *
         * @param array
         * @param count
         **/
        void writeBytes(const uint8_t * array, size_t count)
        {
            for(size_t i = 0; i < count; i++)
                writeByte(array[i]);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeU8(uint8_t v)
        {
            writeByte(v);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeS8(int8_t v)
        {
            writeByte(v);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeU16(uint16_t v)
        {
            writeU8((uint8_t)(v >> 8));
            writeU8((uint8_t)(v & 0xFF));
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeS16(int16_t v)
        {
            writeU16(v);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeU32(uint32_t v)
        {
            writeU16((uint16_t)(v >> 16));
            writeU16((uint16_t)(v & 0xFFFF));
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeS32(int32_t v)
        {
            writeU32(v);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeU64(uint64_t v)
        {
            writeU32((uint64_t)(v >> 32));
            writeU32((uint64_t)(v & 0xFFFFFFFFU));
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeS64(int64_t v)
        {
            writeU64(v);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeF32(float v)
        {
            static_assert(sizeof(float) == sizeof(uint32_t), "float is not 32 bits");

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            union
            {
                uint32_t ival;
                float fval;
            };
            fval = v;
            writeU32(ival);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeF64(double v)
        {
            static_assert(sizeof(double) == sizeof(uint64_t), "double is not 64 bits");

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            union
            {
                uint64_t ival;
                double fval;
            };
            fval = v;
            writeU64(ival);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeBool(bool v)
        {
            writeU8(v ? 1 : 0);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        void writeString(wstring v)
        {

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            /**
             * @brief Write what the function does here
             *
             * @return
             **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            for(size_t i = 0; i < v.length(); i++)
            {
                uint32_t ch = v[i];

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                /**
                 * @brief Write what the function does here
                 *
                 * @param 0x80
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                if(ch != 0 && ch < 0x80)
                {
                    writeU8(ch);
                }

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                /**
                 * @brief Write what the function does here
                 *
                 * @param 0x800
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                else if(ch < 0x800)
                {
                    writeU8(0xC0 | ((ch >> 6) & 0x1F));
                    writeU8(0x80 | ((ch) & 0x3F));
                }

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                /**
                 * @brief Write what the function does here
                 *
                 * @param 0x1000
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                else if(ch < 0x1000)
                {
                    writeU8(0xE0 | ((ch >> 12) & 0xF));
                    writeU8(0x80 | ((ch >> 6) & 0x3F));
                    writeU8(0x80 | ((ch) & 0x3F));
                }

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                /**
                 * @brief Write what the function does here
                 *
                 * @return
                 **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
                else
                {
                    writeU8(0xF0 | ((ch >> 18) & 0x7));
                    writeU8(0x80 | ((ch >> 12) & 0x3F));
                    writeU8(0x80 | ((ch >> 6) & 0x3F));
                    writeU8(0x80 | ((ch) & 0x3F));
                }
            }
            writeU8(0);
        }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class FileReader final : public Reader
{
    private:
        FILE * f;
    public:

        /**
         * @brief Write what the function does here
         *
         * @param fileName
         *
         * @return
         **/
        FileReader(wstring fileName)
        {
            string str = wstringToString(fileName);
            f = fopen(str.c_str(), "rb");
            if(f == nullptr)
                throw IOException(string("IO Error : ") + strerror(errno));
        }
        explicit FileReader(FILE * f)
<<<<<<< HEAD
<<<<<<< HEAD
            : f(f)
        {
            assert(f != nullptr);
        }
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

            /**
             * @brief Write what the function does here
             *
             * @param f
             *
             * @return
             **/
            : f(f)
            {
                assert(f != nullptr);
            }
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual ~FileReader()
        {
            fclose(f);
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual uint8_t readByte() override
        {
            int ch = fgetc(f);

<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            /**
             * @brief Write what the function does here
             *
             * @param EOF
             *
             * @return
             **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            if(ch == EOF)
            {
                if(ferror(f))
                    throw IOException("IO Error : can't read from file");
                throw EOFException();
            }
            return ch;
        }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class FileWriter final : public Writer
{
    private:
        FILE * f;
    public:

        /**
         * @brief Write what the function does here
         *
         * @param fileName
         *
         * @return
         **/
        FileWriter(wstring fileName)
        {
            string str = wstringToString(fileName);
            f = fopen(str.c_str(), "wb");
            if(f == nullptr)
                throw IOException(string("IO Error : ") + strerror(errno));
        }
        explicit FileWriter(FILE * f)
<<<<<<< HEAD
<<<<<<< HEAD
            : f(f)
        {
            assert(f != nullptr);
        }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual ~FileWriter()
        {
            fclose(f);
        }

=======

            /**
             * @brief Write what the function does here
             *
             * @param f
             *
             * @return
             **/
            : f(f)
            {
                assert(f != nullptr);
            }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual ~FileWriter()
        {
            fclose(f);
        }

>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        virtual void writeByte(uint8_t v) override
        {
            if(fputc(v, f) == EOF)
                throw IOException("IO Error : can't write to file");
        }

        /**
         * @brief Write what the function does here
         *
         virtual void flush() override
         {
         if(EOF == fflush(f))
         throw IOException("IO Error : can't write to file");
         }
         };

=======

            /**
             * @brief Write what the function does here
             *
             * @param f
             *
             * @return
             **/
            : f(f)
            {
                assert(f != nullptr);
            }

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual ~FileWriter()
        {
            fclose(f);
        }

        /**
         * @brief Write what the function does here
         *
         * @param v
         **/
        virtual void writeByte(uint8_t v) override
        {
            if(fputc(v, f) == EOF)
                throw IOException("IO Error : can't write to file");
        }

        /**
         * @brief Write what the function does here
         *
         virtual void flush() override
         {
         if(EOF == fflush(f))
         throw IOException("IO Error : can't write to file");
         }
         };

>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
/**
 * @brief Write what the function does here
 *
 * @return
 **/
class MemoryReader final : public Reader
{
    private:
        const shared_ptr<const uint8_t> mem;
        size_t offset;
        const size_t length;
    public:
        explicit MemoryReader(shared_ptr<const uint8_t> mem, size_t length)
<<<<<<< HEAD
<<<<<<< HEAD
            : mem(mem), offset(0), length(length)
        {
        }
        template <size_t length>

            /**
             * @brief Write what the function does here
             *
             * @param length
             *
             * @return
             **/
            explicit MemoryReader(const uint8_t a[length])
=======

            /**
             * @brief Write what the function does here
             *
             * @param mem
             * @param 0
             * @param length
             *
             * @return
             **/
            : mem(mem), offset(0), length(length)
            {
            }
        template <size_t length>

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            explicit MemoryReader(const uint8_t a[length])




=======

            /**
             * @brief Write what the function does here
             *
             * @param mem
             * @param 0
             * @param length
             *
             * @return
             **/
            : mem(mem), offset(0), length(length)
            {
            }
        template <size_t length>

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            explicit MemoryReader(const uint8_t a[length])




>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f



            /**
             * @brief Write what the function does here
             *
             * @param 0
             *
             * @return
             **/
            /**
             * @brief Write what the function does here
             *
             * @param 0
             *
             * @return
             **/
            /**
             * @brief Write what the function does here
             *
             * @param 0
             *
             * @return
             **/
            /**
             * @brief Write what the function does here
             *
             * @param 0
             *
             * @return
             **/
            /**
             * @brief Write what the function does here
             *
             * @param 0
             *
             * @return
             **/
            /**
             * @brief Write what the function does here
             *
             * @param 0
             *
             * @return
             **/
            /**
             * @brief Write what the function does here
             *
             * @param 0
             *
             * @return
             **/
<<<<<<< HEAD
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            : MemoryReader(shared_ptr<const uint8_t>(&a[0], [](const uint8_t *){}))
            {
            }
        virtual uint8_t readByte() override
        {
            if(offset >= length)
                throw EOFException();
            return mem.get()[offset++];
        }
};
class StreamPipe final
{
    StreamPipe(const StreamPipe &) = delete;
    const StreamPipe & operator =(const StreamPipe &) = delete;
    private:
    shared_ptr<Reader> readerInternal;
    shared_ptr<Writer> writerInternal;
    public:
    StreamPipe();
    Reader & reader()
    {
        return *readerInternal;
    }
    Writer & writer()
    {
        return *writerInternal;
    }
    shared_ptr<Reader> preader()
    {
        return readerInternal;
    }
    shared_ptr<Writer> pwriter()
    {
        return writerInternal;
    }
};
class DumpingReader final : public Reader
{
    private:
        Reader &reader;
    public:
        DumpingReader(Reader& reader)
            : reader(reader)
        {
        }
        virtual uint8_t readByte() override;
};
struct StreamRW
{
    StreamRW()
    {
    }
    StreamRW(const StreamRW &) = delete;
    const StreamRW & operator =(const StreamRW &) = delete;
    virtual ~StreamRW()
    {
    }
    Reader & reader()
    {
        return *preader();
    }
    Writer & writer()
    {
        return *pwriter();
    }
    virtual shared_ptr<Reader> preader() = 0;
    virtual shared_ptr<Writer> pwriter() = 0;
};
class StreamRWWrapper final : public StreamRW
{
    private:
        shared_ptr<Reader> preaderInternal;
        shared_ptr<Writer> pwriterInternal;
    public:
        StreamRWWrapper(shared_ptr<Reader> preaderInternal, shared_ptr<Writer> pwriterInternal)
            : preaderInternal(preaderInternal), pwriterInternal(pwriterInternal)
        {
        }
        virtual shared_ptr<Reader> preader() override
        {
            return preaderInternal;
        }
        virtual shared_ptr<Writer> pwriter() override
        {
            return pwriterInternal;
        }
};
class StreamBidirectionalPipe final
{
    private:
        StreamPipe pipe1, pipe2;
        shared_ptr<StreamRW> port1Internal, port2Internal;
    public:
        StreamBidirectionalPipe()
        {
            port1Internal = shared_ptr<StreamRW>(new StreamRWWrapper(pipe1.preader(), pipe2.pwriter()));
            port2Internal = shared_ptr<StreamRW>(new StreamRWWrapper(pipe2.preader(), pipe1.pwriter()));
        }
        shared_ptr<StreamRW> pport1()
        {
            return port1Internal;
        }
        shared_ptr<StreamRW> pport2()
        {
            return port2Internal;
        }
        StreamRW & port1()
        {
            return *port1Internal;
        }
        StreamRW & port2()
        {
            return *port2Internal;
        }
};
struct StreamServer
{
    StreamServer()
    {
    }
    StreamServer(const StreamServer &) = delete;
    const StreamServer & operator =(const StreamServer &) = delete;
    virtual ~StreamServer()
    {
    }
    virtual shared_ptr<StreamRW> accept() = 0;
};
class StreamServerWrapper final : public StreamServer
{
    private:
        list<shared_ptr<StreamRW>> streams;
        shared_ptr<StreamServer> nextServer;
    public:
        StreamServerWrapper(list<shared_ptr<StreamRW>> streams, shared_ptr<StreamServer> nextServer = nullptr)
            : streams(streams), nextServer(nextServer)
        {
        }
        virtual shared_ptr<StreamRW> accept() override
        {
            if(streams.empty())
            {
                if(nextServer == nullptr)
                    throw NoStreamsLeftException();
                return nextServer->accept();
            }
            shared_ptr<StreamRW> retval = streams.front();
            streams.pop_front();
            return retval;
        }
};
#endif // STREAM_H
