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
        {
        }
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
