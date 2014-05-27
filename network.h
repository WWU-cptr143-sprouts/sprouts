#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED
#include "stream.h"
#include <memory>

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class NetworkException : public IOException
{
    public:
        explicit NetworkException(string msg)
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
class NetworkConnection final : public StreamRW
{
    friend class NetworkServer;
    private:
    shared_ptr<Reader> readerInternal;
    shared_ptr<Writer> writerInternal;
    NetworkConnection(int readFd, int writeFd)

        /**
         * @brief Write what the function does here
         *
         * @param readFd
         * @param writeFd
         *
         * @return
         **/
        : readerInternal(new FileReader(fdopen(readFd, "r"))), writerInternal(new FileWriter(fdopen(writeFd, "w")))
<<<<<<< HEAD
<<<<<<< HEAD
    {
    }
=======
        {
        }
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
=======
        {
        }
>>>>>>> 3854c8d4af1c567779842cfd990fcd7aae1ece8f
    public:
    explicit NetworkConnection(wstring url, uint16_t port);

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    shared_ptr<Reader> preader() override
    {
        return readerInternal;
    }

    /**
     * @brief Write what the function does here
     *
     * @return
     **/
    shared_ptr<Writer> pwriter() override
    {
        return writerInternal;
    }
};

/**
 * @brief Write what the function does here
 *
 * @return
 **/
class NetworkServer final : public StreamServer
{
    NetworkServer(const NetworkServer &) = delete;
    const NetworkServer & operator =(const NetworkServer &) = delete;
    private:
    int fd;
    public:
    explicit NetworkServer(uint16_t port);
    ~NetworkServer();
    shared_ptr<StreamRW> accept() override;
};
#endif // NETWORK_H_INCLUDED
