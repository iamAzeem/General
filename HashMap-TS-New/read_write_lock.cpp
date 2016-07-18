#include "read_write_lock.hpp"


namespace HashMapTest {

using std::unique_lock;

ReadWriteLock::ReadWriteLock() : _nReaders{ 0 }, _nWriters{ 0 }, _active{ 0 }
{
}

void ReadWriteLock::readLock ( void )
{
    unique_lock< mutex > lock( _mutex );        // Acquire lock for condition variable
    ++_nReaders;                                // Increment number of readers
    while ( _active < 0 || _nWriters > 0 )      // LOOP: To avoid spurious wake-ups
    {
        _rCondVar.wait( lock );                 // Wait for the notification to read
    }
    --_nReaders;                                // Decrement number of readers
    ++_active;                                  // Increment active status; block writers
}

void ReadWriteLock::writeLock( void )
{
    unique_lock< mutex > lock( _mutex );        // Acquire lock for condition variable
    ++_nWriters;                                // Increment number of writers
    while ( _active != 0 )                      // LOOP: To avoid spurious wake-ups,
    {                                           //       while there's reader(s) reading
        _wCondVar.wait( lock );                 // Wait for the notification to write
    }
    --_nWriters;                                // Decrement number of writers
    _active = -1;                               // Reset active status; block readers
}

void ReadWriteLock::rwUnlock ( void )
{
    unique_lock< mutex > lock( _mutex );        // Acquire lock for condition variable
    if ( _active > 0 )                          // IF: Readers are active?
    {
        --_active;                              // Decrement a reader active status
        if ( _active == 0 )                     // IF: There's no reader?
            _wCondVar.notify_one();             //     Notify a writer to write
    }
    else                                        // ELSE: readers are not active
    {
        _active = 0;                            // Reset active status
        if ( _nWriters > 0 )                    // IF: There are writers?
            _wCondVar.notify_one();             //     Notify a writer to write
        else if ( _nReaders > 0 )               // IF: There are readers?
            _rCondVar.notify_all();             //     Notify all readers to read
    }
}

} // HashMapTest
