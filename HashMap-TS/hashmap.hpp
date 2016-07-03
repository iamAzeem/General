#ifndef HASHMAP_HPP_
#define HASHMAP_HPP_

#include "logger.hpp"
#include "read_write_lock.hpp"

namespace HashMapTest {

const unsigned int DEFAULT_SIZE = 10;

typedef unsigned int HashType;

template < typename K >
class DefaultHashFunction
{
public:
    HashType operator()( const K& key, const size_t size ) const
    {
        return ( (HashType) key % size );
    }
};

template < typename K, typename V >
class Entry
{
public:
    Entry( const K& key, const V& value ) : _key{ key }, _value{ value }, _next{ nullptr }
    {
    }

    void setKey  ( const K& key )       { _key   = key;   }
    void setValue( const V& value )     { _value = value; }
    void setNext ( Entry*   next )      { _next  = next;  }

    const K getKey  ( void ) const      { return _key;    }
    const V getValue( void ) const      { return _value;  }
    Entry*  getNext ( void ) const      { return _next;   }

    void print( void ) const
    {
        LOCK_STREAM();
        LOG_INF() << "  { " << _key << ", " << _value << " }" << endl;
        UNLOCK_STREAM();
    }

private:
    K       _key;
    V       _value;
    Entry*  _next;
};

template < typename K, typename V >
class Entry;

template < typename K, typename V, typename F = DefaultHashFunction< K > >
class TSHashMap
{
public:
    TSHashMap( const size_t size );

    ~TSHashMap();

    bool add ( const K& key, const V& value );
    bool del ( const K& key );
    bool find( const K& key, V& value );

    const size_t size  ( void ) const;
    const size_t length( void ) const;

    bool resize( const size_t size );

    void print( void );

private:
    Entry<K, V>**   _hashTable;
    F               _hashFunction;
    size_t          _size;
    size_t          _length;
    ReadWriteLock   _mutex;
};

template < typename K, typename V, typename F >
TSHashMap<K, V, F>::TSHashMap( const size_t size ) : _hashTable{ nullptr }, _size{ size }, _length{ 0 }
{
    /* Validate positive size; use default size otherwise */
    if ( size <= 0 )
    {
        _size = DEFAULT_SIZE;
    }

    /* Initialize hash table / buckets */
    _hashTable = new Entry<K, V>*[ _size ]{};
    if ( _hashTable == nullptr )
    {
        LOCK_STREAM();
        LOG_ERR() << "Could not allocate memory for HashMap! Exiting..." << endl;
        UNLOCK_STREAM();

        std::exit( EXIT_FAILURE );
    }

    LOCK_STREAM();
    LOG_INF() << "HashMap created! Size: " << size << endl;
    UNLOCK_STREAM();
}

template < typename K, typename V, typename F >
TSHashMap<K, V, F>::~TSHashMap()
{
    _mutex.writeLock();

    LOCK_STREAM();
    LOG_INF() << "Deleting HashMap (" << length() << ")..." << endl;
    UNLOCK_STREAM();

    /* Remove all the variable sized lists first */
    for ( size_t i = 0; i < _size; ++i )
    {
        /* Get entry for current list */
        Entry< K, V >* thisEntry = _hashTable[ i ];

        /* Remove the current entry list */
        while ( thisEntry )
        {
            Entry< K, V >* tempEntry = thisEntry;
            thisEntry = thisEntry->getNext();
            delete tempEntry;

            _length--;
        }

        /* Reset table entry */
        _hashTable[ i ] = nullptr;
    }

    /* Delete and reset hash table */
    delete [] _hashTable;
    _hashTable = nullptr;

    LOCK_STREAM();
    LOG_INF() << "HashMap deleted successfully!" << endl;
    UNLOCK_STREAM();

    _mutex.rwUnlock();
}

template < typename K, typename V, typename F >
bool TSHashMap<K, V, F>::add ( const K& key, const V& value )
{
    Entry< K, V >* newEntry = nullptr;
    Entry< K, V >* tmpEntry = nullptr;

    _mutex.writeLock();

    /* Calculate hash value for new entry */
    const HashType hash = _hashFunction( key, _size );

    /* Get entry location from hash table using hash */
    newEntry = _hashTable[ hash ];

    /* Check if entry already exists */
    while ( newEntry && newEntry->getKey() != key )
    {
        tmpEntry = newEntry;
        newEntry = newEntry->getNext();
    }

    /* Add new entry */
    if ( !newEntry )
    {
        /* Create new entry if it doesn't exist */
        newEntry = new Entry< K, V >( key, value );
        if ( !newEntry )
        {
            LOCK_STREAM();
            LOG_ERR() << "Could not allocate memory for new node!" << endl;
            UNLOCK_STREAM();

            _mutex.rwUnlock();
            return false;
        }

        if ( !tmpEntry )
        {
            /* Add first entry */
            _hashTable[ hash ] = newEntry;
        }
        else
        {
            /* Add another entry in the chain */
            tmpEntry->setNext( newEntry );
        }
    }
    else
    {
        /* Update value existing entry */
        newEntry->setValue( value );
    }

    /* Increment length of hash map */
    _length++;

    _mutex.rwUnlock();

    return true;
}

template < typename K, typename V, typename F >
bool TSHashMap<K, V, F>::del ( const K& key )
{
    Entry< K, V >* prevEntry = nullptr;
    Entry< K, V >* thisEntry = nullptr;

    _mutex.writeLock();

    /* Calculate hash to find the entry */
    const HashType hash = _hashFunction( key, _size );

    /* Get entry from the table if it exists */
    thisEntry = _hashTable[ hash ];

    /* Iterate through hash table to find the entry */
    while ( thisEntry && thisEntry->getKey() != key )
    {
        prevEntry = thisEntry;
        thisEntry = thisEntry->getNext();
    }

    /* If entry not found, return false */
    if ( !thisEntry )
    {
        _mutex.rwUnlock();
        return false;
    }

    /* If found, remove entry from hash table */
    if ( !prevEntry )
    {
        /* If it's first entry, adjust bucket */
        _hashTable[ hash ] = thisEntry->getNext();
    }
    else
    {
        /* If it's in the chain, adjust chain */
        prevEntry->setNext( thisEntry->getNext() );
    }

    /* Delete entry */
    delete thisEntry;

    /* Decrement length of hash map */
    _length--;

    _mutex.rwUnlock();

    return true;
}

template < typename K, typename V, typename F >
bool TSHashMap<K, V, F>::find ( const K& key, V& value )
{
    _mutex.readLock();

    /* Calculate hash value for the key */
    const HashType hash = _hashFunction( key, _size );

    /* Get bucket against key */
    Entry< K, V >* tmpEntry = _hashTable[ hash ];

    bool isFound = false;

    /* Find entry in the chain, return true if found */
    while ( tmpEntry && !isFound )
    {
        if ( tmpEntry->getKey() == key )
        {
            value = tmpEntry->getValue();
            isFound = true;
        }

        tmpEntry = tmpEntry->getNext();
    }

    _mutex.rwUnlock();

    /* If entry not found, return false */
    return isFound;
}

template < typename K, typename V, typename F >
const size_t TSHashMap<K, V, F>::size( void ) const
{
    return _size;
}

template < typename K, typename V, typename F >
const size_t TSHashMap<K, V, F>::length( void ) const
{
    return _length;
}

template < typename K, typename V, typename F >
bool TSHashMap<K, V, F>::resize( const size_t size )
{
    return true;
}

template < typename K, typename V, typename F >
void TSHashMap<K, V, F>::print( void )
{
    Entry< K, V >* thisEntry = nullptr;

    _mutex.readLock();

    /* Print length of hash map */
    LOCK_STREAM();
    LOG_INF() << "HashMap Length: " << length() << endl;
    UNLOCK_STREAM();

    /* Traverse hash table and print key-value pairs */
    for ( size_t i = 0; i < size(); ++i )
    {
        LOCK_STREAM();
        LOG_INF() << "Bucket No: " << ( i + 1 ) << endl;
        UNLOCK_STREAM();

        /* Get first entry of bucket */
        thisEntry = _hashTable[ i ];

        /* Traverse entire bucket and print entries */
        while( thisEntry )
        {
            thisEntry->print();
            thisEntry = thisEntry->getNext();
        }
    }

    _mutex.rwUnlock();
}

} // HashMapTest


#endif /* HASHMAP_HPP_ */
