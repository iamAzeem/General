#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "logger.hpp"
#include "hashmap.hpp"


namespace HashMapTest {

using std::time;
using std::rand;
using std::srand;
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
    GLOBAL_HASHMAP_SIZE     = 20,
    ENTRIES_AT_STARTUP      = 100,
    NUM_OF_WRITER_THREADS   = 1,
    NUM_OF_READER_THREADS   = 10,
    NUM_OF_WRITER_INTERVALS = 100,
    NUM_OF_READER_INTERVALS = 200,
    WRITER_STARTUP_DELAY_MS = 100,
    READER_STARTUP_DELAY_MS = 200,
    WRITE_INTERVAL_IN_MS    = 50,
    READ_INTERVAL_IN_MS     = 100
};

/* typedef for TestType */
typedef unsigned int TestType;

/* Global hash map for tester */
TSHashMap< TestType, TestType > globalHashMap{ GLOBAL_HASHMAP_SIZE };

bool setupTestEnvironment( void )
{
    LOG_INF() << "Setting up test environment..." << endl;

    /* Initialize randomizer */
    srand( time( 0 ) );

    /* Populate global hash map */
    for ( size_t i = 0; i < ENTRIES_AT_STARTUP; ++i )
    {
        const TestType key = i + 1;
        const TestType val = rand() % ENTRIES_AT_STARTUP;

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
    /* Introduce startup delay */
    sleep_for( milliseconds( WRITER_STARTUP_DELAY_MS ) );

    LOCK_STREAM();
    LOG_INF() << "Writer thread started! TID: " << get_id() << endl;
    UNLOCK_STREAM();

    /* Start of main test loop */
    for ( size_t i = 0; i < NUM_OF_WRITER_INTERVALS; ++i )
    {
        /* Introduce interval delay */
        sleep_for( milliseconds( WRITE_INTERVAL_IN_MS ) );

        /* Get a random entry number */
        const TestType key = rand() % ENTRIES_AT_STARTUP;
        const TestType val = rand() % ENTRIES_AT_STARTUP;

        /* On EVEN interval, add / update a random entry */
        if ( (i & 1) == 0 )
        {
            if ( globalHashMap.add( key, val ) )
            {
                LOCK_STREAM();
                LOG_INF() << "Writer thread (" << get_id() << ") added KV:{" << key << ", " << val << "}"<< endl;
                UNLOCK_STREAM();
            }
        }
        else /* On ODD interval, delete a random entry */
        {
            if ( globalHashMap.del( key ) )
            {
                LOCK_STREAM();
                LOG_INF() << "Writer thread (" << get_id() << ") deleted entry K:{" << key << "}"<< endl;
                UNLOCK_STREAM();
            }
        }
    }
}

void readerCallback( void )
{
    /* Introduce startup delay */
    sleep_for( milliseconds( READER_STARTUP_DELAY_MS ) );

    LOCK_STREAM();
    LOG_INF() << "Reader thread started! TID: " << get_id() << endl;
    UNLOCK_STREAM();

    /* Start of main test loop */
    for ( size_t i = 0; i < NUM_OF_READER_INTERVALS; ++i )
    {
        /* Introduce interval delay */
        sleep_for( milliseconds( READ_INTERVAL_IN_MS ) );

        /* Get a random entry key */
        TestType key = rand() % ENTRIES_AT_STARTUP;
        TestType val = 0;

        /* Find the random entry and print */
        if ( globalHashMap.find( key, val ) )
        {
            LOCK_STREAM();
            LOG_INF() << "Reader thread (" << get_id() << ") found KV:{" << key << ", " << val << "}"<< endl;
            UNLOCK_STREAM();
        }
    }
}

void hashmapTester( void )
{
    if ( !setupTestEnvironment() ) return;

    LOCK_STREAM();
    LOG_INF() << "HashMap Test started!" << endl;
    UNLOCK_STREAM();

    /* Set up writer and reader threads */
    thread writerThreads[ NUM_OF_WRITER_THREADS ] = {};
    thread readerThreads[ NUM_OF_READER_THREADS ] = {};

    /* Create writer and reader threads */
    for ( auto& wt : writerThreads ) wt = thread( writerCallback );
    for ( auto& rt : readerThreads ) rt = thread( readerCallback );

    /* Join writer and reader threads */
    for ( auto& wt : writerThreads ) wt.join();
    for ( auto& rt : readerThreads ) rt.join();
}

} // HashMap Test


int main( void )
{
    HashMapTest::hashmapTester();
    return EXIT_SUCCESS;
}
