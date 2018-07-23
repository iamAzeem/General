#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

bool isPalindrome( const std::string& s );

int main( void )
{
    const std::vector< std::string > testStrings
    {
        "radar",
        "hello",
        "world",
        "aabaa"
    };

    std::for_each( testStrings.begin(), testStrings.end(), []( const auto& s )
    {
        std::cout << std::quoted( s ) << " : isPalindrome? "
                  << std::boolalpha   << isPalindrome( s ) << '\n';
    });

    return EXIT_SUCCESS;
}

bool isPalindrome( const std::string& s )
{
    const auto mid = s.length() / 2;
    return std::equal( s.begin(),
                       std::next( s.begin(), mid ),
                       s.rbegin(),
                       std::next( s.rbegin(), mid ) );
}
