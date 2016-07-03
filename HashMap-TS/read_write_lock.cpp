#include "read_write_lock.hpp"

namespace HashMapTest {

using std::unique_lock;

ReadWriteLock::ReadWriteLock() : _readers{ 0 }, _writers{ 0 }, _active{ 0 }
{
}

void ReadWriteLock::readLock ( void )
{
    unique_lock< mutex > lock( _mutex );
    ++_readers;
    while( _active < 0 || _writers > 0 )
        _rCondVar.wait( lock );
    --_readers;
    ++_active;
}

void ReadWriteLock::writeLock( void )
{
    unique_lock< mutex > lock( _mutex );
    ++_writers;
    while( _active != 0 )
        _wCondVar.wait(lock);
    --_writers;
    _active = -1;
}

void ReadWriteLock::rwUnlock ( void )
{
    unique_lock< mutex > lock( _mutex );
    if( _active > 0 )
    {
        --_active;
        if( _active == 0 )
            _wCondVar.notify_one();
    }
    else
    {
        _active = 0;
        if( _writers > 0 )
            _wCondVar.notify_one();
        else if( _readers > 0 )
            _rCondVar.notify_all();
    }
}

} // HashMapTest
