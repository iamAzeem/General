#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>

bool isPalindrome( const std::string& s );

int main( void )
{
    std::vector< std::string > strings
    {
        "radar",
        "hello",
        "world",
        "aabaa"
    };

    std::for_each( strings.begin(), strings.end(), []( const auto& s )
    {
        std::cout << std::quoted( s ) << " : isPalindrome? "
                  << std::boolalpha   << isPalindrome( s ) << '\n';
    });

    return EXIT_SUCCESS;
}

bool isPalindrome( const std::string& s )
{
    return std::equal( s.begin(),
                       std::next( s.begin(), s.length() / 2 ),
                       s.rbegin(),
                       std::next( s.rbegin(), s.length() / 2 ) );
}
