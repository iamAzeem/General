#ifndef READ_WRITE_LOCK_HPP_
#define READ_WRITE_LOCK_HPP_

#include <mutex>
#include <condition_variable>


namespace HashMapTest {

using std::mutex;
using std::condition_variable;

class ReadWriteLock
{
public:
    ReadWriteLock();

    void readLock ( void );
    void writeLock( void );
    void rwUnlock ( void );

private:
    mutex                   _mutex;
    condition_variable      _rCondVar;
    condition_variable      _wCondVar;
    size_t                  _readers;
    size_t                  _writers;
    int                     _active;
};

} // HashMapTest


#endif /* READ_WRITE_LOCK_HPP_ */
