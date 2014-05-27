#ifndef COMPRESSED_STREAM_H_INCLUDED
#define COMPRESSED_STREAM_H_INCLUDED
#include <deque>
#include "stream.h"
#include <iostream>

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class LZ77FormatException final : public IOException
{
    public:
        LZ77FormatException()
<<<<<<< HEAD
            : IOException("LZ77 format error")
        {
        }
=======

            /**
             * @brief Write what the function does here
             *
             * @return
             **/
            : IOException("LZ77 format error")
            {
            }
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
struct LZ77CodeType final
{
    static constexpr int lengthBits = 6, offsetBits = 16 - lengthBits;
    static constexpr size_t maxLength = (1 << lengthBits) - 1, maxOffset = (1 << offsetBits) - 1;
    size_t length;
    size_t offset;
    uint8_t nextByte;
    LZ77CodeType(size_t length, size_t offset, uint8_t nextByte)

        /**
         * @brief Write what the function does here
         *
         * @param length
         * @param offset
         * @param nextByte
         *
         * @return
         **/
        : length(length), offset(offset), nextByte(nextByte)
        {
        }
    LZ77CodeType() // initialize with EOF

        /**
         * @brief Write what the function does here
         *
         * @param 0
         * @param 1
         * @param 0
         *
         * @return
         **/
        : length(0), offset(1), nextByte(0)
        {
        }
    LZ77CodeType(uint8_t nextByte)

        /**
         * @brief Write what the function does here
         *
         * @param 0
         * @param 0
         * @param nextByte
         *
         * @return
         **/
        : length(0), offset(0), nextByte(nextByte)
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
    bool hasNextByte()
    {
        return length != 0 || offset == 0;
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    bool eof()
    {
        return length == 0 && offset != 0;
    }

    /**
     * @brief Write what the function does here
     *
     * @param reader
     *
     * @return
     **/
    static LZ77CodeType read(Reader &reader)
    {
        LZ77CodeType retval;
        retval.nextByte = reader.readByte();

        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        try
        {
            uint16_t v = reader.readU16();
            retval.length = v >> offsetBits;
            retval.offset = v & maxOffset;
        }

<<<<<<< HEAD
=======
        /**
         * @brief Write what the function does here
         *
         * @param e
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        catch(EOFException &e)
        {
            throw LZ77FormatException();
        }
        return retval;
    }

    /**
     * @brief Write what the function does here
     *
     * @param writer
     **/
    void write(Writer &writer)
    {
        //cout << "Write code : 0x" << hex << (unsigned)nextByte << dec << " : length : " << length << " : offset : " << offset << endl;
        writer.writeByte(nextByte);
        uint16_t v = (offset & maxOffset) | (length << offsetBits);
        writer.writeU16(v);
    }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class ExpandReader final : public Reader
{
    private:
        shared_ptr<Reader> reader;
        static constexpr size_t bufferSize = LZ77CodeType::maxOffset + 1;
        circularDeque<uint8_t, bufferSize + 2> buffer;
        LZ77CodeType currentCode;
    public:
        ExpandReader(shared_ptr<Reader> reader)
<<<<<<< HEAD
            : reader(reader)
        {
        }

        /**
         * @brief Write what the function does here
         *
         * @param reader
         *
=======

            /**
             * @brief Write what the function does here
             *
             * @param reader
             *
             * @return
             **/
            : reader(reader)
            {
            }

        /**
         * @brief Write what the function does here
         *
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
         * @return
         **/
        ExpandReader(Reader &reader)
            : ExpandReader(shared_ptr<Reader>(&reader, [](Reader *) {}))
            {
            }
<<<<<<< HEAD
        virtual ~ExpandReader()
        {
        }
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual ~ExpandReader()
        {
        }
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        virtual uint8_t readByte() override
        {
            while(currentCode.eof())
            {
                currentCode = LZ77CodeType::read(*reader);
            }
            uint8_t retval;
            if(currentCode.length == 0)
            {
                retval = currentCode.nextByte;
                currentCode = LZ77CodeType();
            }
            else
            {
                if(currentCode.offset >= buffer.size())
                {
                    throw LZ77FormatException();
                }
                retval = buffer.cbegin()[currentCode.offset];
                if(--currentCode.length == 0)
                {
                    currentCode = LZ77CodeType(currentCode.nextByte);
                }
            }
            buffer.push_front(retval);
            if(buffer.size() > bufferSize)
            {
                buffer.pop_back();
            }
            return retval;
        }
};
<<<<<<< HEAD
=======
/**
 * @brief Write what the function does here
 *
 * @return
 **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
class CompressWriter final : public Writer
{
    private:
        static constexpr int uint8_max = (1 << 8) - 1;
        static constexpr size_t bufferSize = LZ77CodeType::maxOffset + 1;
<<<<<<< HEAD
        struct Match
        {
            size_t location, length;
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        struct Match
        {
            size_t location, length;
            /**
             * @brief Write what the function does here
             *
             * @return
             **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            Match(size_t location, size_t length)
                : location(location), length(length)
            {
            }
<<<<<<< HEAD
=======
            /**
             * @brief Write what the function does here
             *
             * @return
             **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
            Match()
                : Match(0, 0)
            {
            }
        };
        size_t location;
<<<<<<< HEAD
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        size_t getActualLocation(size_t l)
        {
            return location - l;
        }
        shared_ptr<Writer> writer;
        circularDeque<uint_fast8_t, bufferSize + 1> currentInput;
        circularDeque<uint_fast8_t, bufferSize + 2> buffer;
        list<size_t> nodes[uint8_max + 1];
<<<<<<< HEAD
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        void addByte(uint_fast8_t v)
        {
            nodes[v].push_front(++location);
            buffer.push_front(v);
            if(buffer.size() > bufferSize)
            {
                buffer.pop_back();
                for(list<size_t> & nodeList : nodes)
                {
                    for(auto i = nodeList.begin(); i != nodeList.end();)
                    {
                        if(getActualLocation(*i) >= buffer.size())
                            i = nodeList.erase(i);
                        else
                            i++;
                    }
                }
            }
        }
        /*void dumpLocation(size_t l)
          {
          cout << l << " : ";
          l = getActualLocation(l);
          if(l >= buffer.size())
          cout << "\"\"" << endl;
          else
          {
          cout << "\"";
          auto iter = buffer.cbegin() + l;
          for(int i = 0; i < 5 && i <= l; i++, iter--)
          {
          cout << (char)*iter;
          }
          cout << "\"" << endl;
          }
          }*/
<<<<<<< HEAD
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        Match getBiggestMatch()
        {
            Match retval;
            auto ii = currentInput.cbegin();
            if(ii == currentInput.cend())
                return Match();
            const list<size_t> & curNodes = nodes[*ii++];
            if(curNodes.empty())
                return Match();
            auto startII = ii;
            for(size_t startPos : curNodes)
            {
                size_t matchLength = 1;
                size_t node = startPos;
                for(ii = startII; ii != currentInput.cend(); matchLength++, ii++, node++)
                {
                    size_t pos = getActualLocation(node + 1);
                    if(pos >= buffer.size())
                        break;
                    if(buffer.cbegin()[pos] != *ii)
                        break;
                }
                if(matchLength > retval.length)
                {
                    retval.length = matchLength;
                    retval.location = getActualLocation(startPos);
                }
            }
            if(retval.length > LZ77CodeType::maxLength + 1)
                retval.length = LZ77CodeType::maxLength + 1;
            return retval;
        }
<<<<<<< HEAD
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        void writeCode()
        {
            if(currentInput.empty())
                return;
            if(currentInput.size() == 1)
            {
                addByte(currentInput.front());
                LZ77CodeType(currentInput.front()).write(*writer);
                currentInput.pop_front();
                return;
            }
            Match m = getBiggestMatch();
            if(m.length <= 1)
            {
                addByte(currentInput.front());
                LZ77CodeType(currentInput.front()).write(*writer);
                currentInput.pop_front();
                return;
            }
            m.length--;
            for(size_t i = 0; i < m.length; i++)
            {
                addByte(currentInput.front());
                currentInput.pop_front();
            }
            LZ77CodeType code(m.length, m.location, currentInput.front());
            code.write(*writer);
            addByte(currentInput.front());
            currentInput.pop_front();
        }
    public:
<<<<<<< HEAD
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        CompressWriter(shared_ptr<Writer> writer)
            : writer(writer)
        {
        }
<<<<<<< HEAD
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        CompressWriter(Writer &writer)
            : CompressWriter(shared_ptr<Writer>(&writer, [](Writer *) {}))
            {
            }
<<<<<<< HEAD
        virtual ~CompressWriter()
        {
        }
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
        virtual ~CompressWriter()
        {
        }
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        virtual void flush() override
        {
            while(!currentInput.empty())
                writeCode();
            writer->flush();
        }
<<<<<<< HEAD
=======
        /**
         * @brief Write what the function does here
         *
         * @return
         **/
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
        virtual void writeByte(uint8_t v) override
        {
            currentInput.push_back(v);
            if(currentInput.size() < bufferSize)
                return;
            writeCode();
        }
};
#endif // COMPRESSED_STREAM_H_INCLUDED
