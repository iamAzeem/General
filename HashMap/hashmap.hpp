#ifndef HASHMAP_HPP_
#define HASHMAP_HPP_


namespace HashMapTest {

const unsigned int DEFAULT_SIZE = 10;

template < typename K >
class DefaultHashFunction
{
public:
    unsigned int operator()( const K& key, const unsigned int size ) const
    {
        return ( (unsigned int) key % size );
    }
};

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

    const K getKey  ( void ) const      { return _key;    }
    const V getValue( void ) const      { return _value;  }
    Entry*  getNext ( void ) const      { return _next;   }

    void print( void ) const
    {
        std::cout << "  { " << _key << ", " << _value << " }" << std::endl;
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
    TSHashMap( const unsigned int size );

    ~TSHashMap();

    bool add ( const K& key, const V& value );
    bool del ( const K& key );
    bool find( const K& key, V& value );

    const unsigned int size  ( void ) const;
    const unsigned int length( void ) const;

    bool resize( const unsigned int size );

    void print( void );

private:
    Entry<K, V>**   _hashTable;
    F               _hashFunction;
    unsigned int    _size;
    unsigned int    _length;
};

template < typename K, typename V, typename F >
TSHashMap<K, V, F>::TSHashMap( const unsigned int size ) : _hashTable( nullptr ), _size( size ), _length( 0 )
{
    /* Validate positive size; use default size otherwise */
    if ( size <= 0 )
    {
        _size = DEFAULT_SIZE;
    }

    /* Initialize hash table / buckets */
    _hashTable = new Entry<K, V>*[ _size ]();
    if ( _hashTable == nullptr )
    {
        std::cerr << "Could not allocate memory for HashMap! Exiting..." << std::endl;
        std::exit( EXIT_FAILURE );
    }
}

template < typename K, typename V, typename F >
TSHashMap<K, V, F>::~TSHashMap()
{
    std::cout << "Deleting HashMap (" << length() << ")..." << std::endl;

    /* Remove all the variable sized lists first */
    for ( unsigned int i = 0; i < _size; ++i )
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
}

template < typename K, typename V, typename F >
bool TSHashMap<K, V, F>::add ( const K& key, const V& value )
{
    Entry< K, V >* newEntry = nullptr;
    Entry< K, V >* tmpEntry = nullptr;

    /* Calculate hash value for new entry */
    const unsigned int hash = _hashFunction( key, _size );

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
            std::cerr << "Could not allocate memory for new node!" << std::endl;
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
    return true;
}

template < typename K, typename V, typename F >
bool TSHashMap<K, V, F>::del ( const K& key )
{
    Entry< K, V >* prevEntry = nullptr;
    Entry< K, V >* thisEntry = nullptr;

    /* Calculate hash to find the entry */
    const unsigned int hash = _hashFunction( key, _size );

    /* Get entry from the table if it exists */
    thisEntry = _hashTable[ hash ];

    /* Iterate through hash table to find the entry */
    while ( thisEntry && thisEntry->getKey() != key )
    {
        prevEntry = thisEntry;
        thisEntry = thisEntry->getNext();
    }

    /* If entry not found, return false */
    if ( !thisEntry ) { return false; }

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
    return true;
}

template < typename K, typename V, typename F >
bool TSHashMap<K, V, F>::find ( const K& key, V& value )
{
    /* Calculate hash value for the key */
    const unsigned int hash = _hashFunction( key, _size );

    /* Get bucket against key */
    Entry< K, V >* tmpEntry = _hashTable[ hash ];

    /* Find entry in the chain, return true if found */
    for ( ; tmpEntry ; tmpEntry = tmpEntry->getNext() )
    {
        if ( tmpEntry->getKey() == key )
        {
            value = tmpEntry->getValue();
            return true;
        }
    }

    /* If entry not found, return false */
    return false;
}

template < typename K, typename V, typename F >
const unsigned int TSHashMap<K, V, F>::size( void ) const
{
    return _size;
}

template < typename K, typename V, typename F >
const unsigned int TSHashMap<K, V, F>::length( void ) const
{
    return _length;
}

template < typename K, typename V, typename F >
bool TSHashMap<K, V, F>::resize( const unsigned int size )
{
    return true;
}

template < typename K, typename V, typename F >
void TSHashMap<K, V, F>::print( void )
{
    Entry< K, V >* thisEntry = nullptr;

    /* Print length of hash map */
    std::cout << "HashMap Length: " << length() << std::endl;

    /* Traverse hash table and print key-value pairs */
    for ( unsigned int i = 0; i < size(); ++i )
    {
        std::cout << "Bucket No: " << ( i + 1 ) << std::endl;

        /* Get first entry of bucket */
        thisEntry = _hashTable[ i ];

        /* Traverse entire bucket and print entries */
        while( thisEntry )
        {
            thisEntry->print();
            thisEntry = thisEntry->getNext();
        }
    }
}

} // HashMapTest


#endif /* HASHMAP_HPP_ */
