#include <iostream>
#include <map>

#define IS_EVEN(N) ( (N & 1) == 0 )

typedef unsigned long long ulonglong;

namespace RecursionTest
{
    const ulonglong func ( const ulonglong number )
    {
        static std::map<ulonglong, ulonglong> lookupTable;

        /* Validate positive number */
        if ( number < 0 ) return -1;

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
}

int main (void)
{
    const ulonglong number = 123456789012345678ULL;
    std::cout << RecursionTest::func( number ) << std::endl;
    return 0;
}
