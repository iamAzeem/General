#ifndef HASHMAP_HPP_
#define HASHMAP_HPP_


namespace HashMapTest {

template < typename K, typename V >
class Entry;

template < typename K, typename V, typename F >
class TSHashMap
{
public:
    enum class Default : unsigned int { SIZE = 10 };

    TSHashMap( const unsigned int size );

    ~TSHashMap();

    void add ( const K& key, const V& value );
    void del ( const K& key );
    V    find( const K& key );

    const unsigned int size( void ) const;

    void resize( void );

private:
    Entry<K, V>**   _hashTable;
    F               _hashFunction;
    unsigned int    _size;
};

} // HashMapTest


#endif /* HASHMAP_HPP_ */
