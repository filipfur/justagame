#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>

class IO
{
public:
    IO()
    {

    }

    virtual ~IO() noexcept
    {
        _connection = nullptr;
    }

    void write(char* data, int len)
    {
        memcpy(_connection->_buffer, data, len);
        _connection->_bytesAvailable = len;
        _connection->_cv.notify_one();
    }

    int read(char* data)
    {
        std::unique_lock<std::mutex> lk(_mutex);
        _cv.wait(lk, [this](){return _bytesAvailable > 0 || !isConnected();});
        if(!isConnected())
        {
            return -1;
        }
        assert(_bytesAvailable > 0);
        memmove(data, _buffer, _bytesAvailable);
        int numRead = _bytesAvailable;
        _bytesAvailable = 0;
        return numRead;
    }

    void connect(IO* connection)
    {
        _connection = connection;
    }

    void disconnect()
    {
        _connection = nullptr;
        _cv.notify_one();
    }

    bool isConnected() const
    {
        return _connection != nullptr;
    }

private:
    IO* _connection{nullptr};
    char _buffer[2048];
    int _bytesAvailable{0};
    std::mutex _mutex;
    std::condition_variable _cv;
};