#include <iostream>
#include <cstdlib>
#include <map>

/* Macro for even number test */
#define IS_EVEN(N) ( (N & 1) == 0 )

/* typedef for recursive return type */
typedef unsigned long long ulonglong;

/* Recursion Test */
namespace RecursionTest {

ulonglong func ( const ulonglong number )
{
    /* Lookup table for recursive function */
    static std::map<ulonglong, ulonglong> lookupTable;

    /* Handle first two base condition: f(0) = 1, f(1) = 1  */
    if ( number <= 1 ) return 1;

    /* Look up in table for existing entry */
    auto lookupIterator = lookupTable.find ( number );
    if ( lookupIterator != lookupTable.end() )
    {
        return lookupIterator->second;
    }

    /* Calculate: f(2n) = f(n) and f(2n + 1) = f(n) + f(n - 1) */
    auto value = IS_EVEN( number ) ? func ( number / 2 )
                                   : func ( number / 2 ) + func ( number / 2 - 1 );

    /* Update lookup table for current number */
    lookupTable[ number ] = value;
    return value;
}

} // RecursionTest

int main( void )
{
    const ulonglong number = 123456789012345678ULL;
    std::cout << "Answer: " << RecursionTest::func( number ) << std::endl;
    return EXIT_SUCCESS;
}
