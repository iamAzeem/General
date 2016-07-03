#include <iostream>
#include <cstdlib>
#include "hashmap.hpp"


namespace HashMapTest {

template < typename K, typename V >
class Entry
{
public:
    Entry( const K& key, const V& value ) : _key( key ), _value( value ), _next( nullptr )
    {
    }

    void setKey  ( const K& key )       { _key   = key;   }
    void setValue( const V& value )     { _value = value; }
    void setNext ( Entry*   next )      { _next  = next;  }

    const K getKey  ( void ) const      { return _key;   }
    const V getValue( void ) const      { return _value; }
    Entry*  getNext ( void ) const      { return _next;  }

private:
    K       _key;
    V       _value;
    Entry*  _next;
};

template < typename K, typename V, typename F >
TSHashMap<K, V, F>::TSHashMap( const unsigned int size ) : _hashTable( nullptr ), _size( size )
{
    /* Validate positive size; use default size otherwise */
    if ( size <= 0 )
    {
        _size = Default::SIZE;
    }

    /* Initialize hash table / buckets */
    _hashTable = new Entry<K, V>*[ _size ];
    if ( _hashTable == nullptr )
    {
        std::cerr << "Could not allocate memory for HashMap! Exiting..." << std::endl;
        std::exit( EXIT_FAILURE );
    }
}

template < typename K, typename V, typename F >
TSHashMap<K, V, F>::~TSHashMap()
{
    /* Remove all the variable sized lists first */
    for ( unsigned int i = 0; i < _size; ++i )
    {
        /* Get entry for current list */
        Entry< K, V >* currentEntry = _hashTable[ i ];

        /* Remove the current entry list */
        while ( currentEntry )
        {
            Entry< K, V >* tempEntry = currentEntry;
            currentEntry = currentEntry->getNext();
            delete tempEntry;
        }

        /* Reset table entry */
        _hashTable[ i ] = nullptr;
    }

    /* Delete hash table */
    delete [] _hashTable;
    _hashTable = nullptr;
}

template < typename K, typename V, typename F >
void TSHashMap<K, V, F>::add ( const K& key, const V& value )
{

}

template < typename K, typename V, typename F >
void TSHashMap<K, V, F>::del ( const K& key )
{

}

template < typename K, typename V, typename F >
V TSHashMap<K, V, F>::find ( const K& key )
{

}

template < typename K, typename V, typename F >
const unsigned int TSHashMap<K, V, F>::size( void ) const
{
    return _size;
}

template < typename K, typename V, typename F >
void TSHashMap<K, V, F>::resize( void )
{

}

} // HashMapTest
