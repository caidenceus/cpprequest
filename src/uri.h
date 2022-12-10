#ifndef CPPR_URI_H__
#define CPPR_URI_H__

#include <string>

// RFC 3986 Section 3
struct Uri final
{
    std::string scheme{ };
    std::string user{ };
    std::string password{ };
    std::string host{ };
    std::string port{ };
    std::string path{ };
    std::string query{ };
    std::string fragment{ };
};


/**
 * @brief Parse a URI and return a Uri object.
 *
 * @param uri The URI to parse.
 * @param port The port to connect to. This is overridden if a port is specified in the URI.
 * @return The filled URI object.
 */
Uri parse_uri(std::string uri, std::string port);


/**
 * @brief Replace reserved characters with percent encoded characters.
 * 
 * @param str The string to percent encode.
 * @return The percent encoded string.
 */
std::string percent_encode(std::string str);

#endif // CPPR_URI_H__