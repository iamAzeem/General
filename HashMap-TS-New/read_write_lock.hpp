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
    mutex                   _mutex;         // common mutex for unique_lock
    condition_variable      _rCondVar;      // condition variable for readers
    condition_variable      _wCondVar;      // condition variable for writers
    size_t                  _nReaders;      // count for readers
    size_t                  _nWriters;      // count for writers
    int                     _active;        // flag to avoid spurious wake-ups
};

} // HashMapTest


#endif /* READ_WRITE_LOCK_HPP_ */
