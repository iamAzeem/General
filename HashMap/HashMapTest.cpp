#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <mutex>
#include "logger.hpp"
#include "hashmap.hpp"

namespace HashMapTest {

using std::cout;
using std::cerr;
using std::endl;
using std::thread;
using std::this_thread::get_id;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using HashMapTest::TSHashMap;

/* Function Prototypes */
bool setupTestEnvironment( void );

void writerCallback( void );
void readerCallback( void );
void hashmapTester ( void );

/* Test Default Configurations */
enum TestDefaults
{
    ENTRIES_AT_STARTUP      = 100,
    NUM_OF_WRITER_THREADS   = 1,
    NUM_OF_READER_THREADS   = 10,
    WRITER_STARTUP_DELAY_MS = 100,
    READER_STARTUP_DELAY_MS = 200,
    WRITE_INTERVAL_IN_MS    = 50,
    READ_INTERVAL_IN_MS     = 100
};

/* Global hash map for tester */
TSHashMap< unsigned int, unsigned int > globalHashMap( 20 );

bool setupTestEnvironment( void )
{
    LOG_INF() << "Setting up test environment..." << endl;

    std::srand( std::time( NULL ) );

    /* Populate global hash map */
    for ( unsigned int i = 0; i < ENTRIES_AT_STARTUP; ++i )
    {
        const unsigned int key = i + 1;
        const unsigned int val = rand() % 100;

        if ( !globalHashMap.add( key, val ) )
        {
            LOG_ERR() << "Could not add entry in HashMap!" << endl;
            return false;
        }
    }

    //globalHashMap.print();

    LOG_INF() << "Test environment setup completed!" << endl;
    return true;
}

void writerCallback( void )
{
    sleep_for( milliseconds( WRITER_STARTUP_DELAY_MS ) );

    LOCK_STREAM();
    LOG_INF() << "Writer thread started! TID: " << get_id() << endl;
    UNLOCK_STREAM();

    const unsigned int intervals = 100;
    for ( unsigned int i = 0; i < intervals; ++i )
    {
        sleep_for( milliseconds( rand() % WRITE_INTERVAL_IN_MS ) );
    }
}

void readerCallback( void )
{
    sleep_for( milliseconds( READER_STARTUP_DELAY_MS ) );

    LOCK_STREAM();
    LOG_INF() << "Reader thread started! TID: " << get_id() << endl;
    UNLOCK_STREAM();
}

void hashmapTester( void )
{
    if ( !setupTestEnvironment() ) return;

    /* Set up writer and reader threads */
    thread writerThreads[ NUM_OF_WRITER_THREADS ] = {};
    thread readerThreads[ NUM_OF_READER_THREADS ] = {};

    /* Create writer and reader threads */
    for ( unsigned int i = 0; i < NUM_OF_WRITER_THREADS; ++i )
    {
        writerThreads[ i ] = thread( writerCallback );
    }

    for ( unsigned int i = 0; i < NUM_OF_READER_THREADS; ++i )
    {
        readerThreads[ i ] = thread( readerCallback );
    }

    /* Join writer and reader threads */
    for ( unsigned int i = 0; i < NUM_OF_WRITER_THREADS; ++i )
    {
        writerThreads[ i ].join();
    }

    for ( unsigned int i = 0; i < NUM_OF_READER_THREADS; ++i )
    {
        readerThreads[ i ].join();
    }

    LOCK_STREAM();
    LOG_INF() << "HashMap Test started!" << endl;
    UNLOCK_STREAM();
}

} // HashMap Test


int main( void )
{
    HashMapTest::hashmapTester();
    return 0;
}
