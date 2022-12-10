#include "uri.h"       // Uri, percent_encode

#include "error.h"     // ParseError
#include "utility.hpp" // is_alpha, is_digit

#include <string>
#include <unordered_map>

#define NPOS std::string::npos

static bool valid_uri_scheme_character(char const c)
{
    if (is_alpha(c) || is_digit(c) || c == '+' || c == '-' || c == '.')
        return true;
    return false;
}


static bool valid_url_query_char(char const c)
{
    return
        (is_alpha(c) || is_digit(c)) ||
        (c == '+' || c == '.' || c == '-' || c == '_') ||
        (c == '~' || c == '=');
}


// URI = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
Uri parse_uri(std::string uri, std::string port)
{
    Uri rtn;
    rtn.port = port;

    std::string::iterator head = uri.begin();
    std::string::iterator tail = uri.end();

    if (head == tail || !is_alpha(*head))
    {
        throw cpprerr::ParseError{
            "Invalid URI. Please refer to RFC 3986 Section 3: "
            "https://www.rfc-editor.org/rfc/rfc3986#section-3.1"
        };
    }

    for (; head != tail && valid_uri_scheme_character(*head); ++head)
        rtn.scheme.push_back(*head);

    std::string authority = std::string(head, tail);
    if (head == tail || authority.substr(0, 3) != "://")
    {
        throw cpprerr::ParseError{
            "Invalid URI. Please refer to RFC 3986 Section 3: "
            "https://www.rfc-editor.org/rfc/rfc3986#section-3.1"
        };
    }
    authority = authority.substr(3);

    // Fragment appears at the end of the string
    int index = authority.find('#');
    if (index != NPOS) {
        rtn.fragment = authority.substr(index + 1);
        authority.resize(index);
    }

    // Query appears directpy before fragment
    index = authority.find('?');
    if (index != NPOS) {
        rtn.query = authority.substr(index + 1);
        authority.resize(index);
    }
  
    // Percent encode the query
    rtn.query = percent_encode(rtn.query);

    // At this point, URI = [authority] path. Path always begins with '/'
    index = authority.find('/');
    if (index != NPOS) {
        rtn.path = authority.substr(index);
        authority.resize(index);
    } else {
        rtn.path = "/"; // Default request path
    }

    // [username [:password ] @] host
    std::string user_pass;
    index = authority.find('@');
    if (index != NPOS) {
        user_pass = authority.substr(0, index);

        index = user_pass.find(':');
        if (index != NPOS) {
            rtn.user = user_pass.substr(0, index);
            rtn.password = user_pass.substr(index + 1);
        } else {
            rtn.user = user_pass;
        }
        rtn.host = authority.substr(index + 1);
    } else {
        rtn.host = authority;
    }
  
    // Optional port host [:port]
    index = rtn.host.find(':');
    if (index != NPOS) {
        rtn.port = rtn.host.substr(index + 1);
        rtn.host.resize(index);
    }

    return rtn;
}


std::string percent_encode(std::string const str)
{
    // RFC 3986 Section 2.2. Reserved characters
    // ? # [ ] @ $ & ' ( ) * + , ; =
    std::unordered_map<char, std::string> pe;
    pe[' '] = "%20";
    pe['!'] = "%21";
    pe['"'] = "%22";
    pe['#'] = "%23";
    pe['$'] = "%24";
    pe['%'] = "%25";
    pe['&'] = "%26";
    pe['\''] = "%27";
    pe['('] = "%28";
    pe[')'] = "%29";
    pe['*'] = "%2A";
    pe['+'] = "%2B";
    pe[','] = "%2C";
    pe['-'] = "%2D";
    pe['.'] = "%2E";
    pe['/'] = "%2F";
    pe['0'] = "%30";
    pe['1'] = "%31";
    pe['2'] = "%32";
    pe['3'] = "%33";
    pe['4'] = "%34";
    pe['5'] = "%35";
    pe['6'] = "%36";
    pe['7'] = "%37";
    pe['8'] = "%38";
    pe['9'] = "%39";
    pe[':'] = "%3A";
    pe[';'] = "%3B";
    pe['<'] = "%3C";
    pe['='] = "%3D";
    pe['>'] = "%3E";
    pe['?'] = "%3F";
    pe['@'] = "%40";
    pe['['] = "%5B";
    pe['\\'] = "%5C";
    pe[']'] = "%5D";
    pe['^'] = "%5E";
    pe['_'] = "%5F";
    pe['`'] = "%60";
    pe['{'] = "%7B";
    pe['|'] = "%7C";
    pe['}'] = "%7D";
    pe['~'] = "%7E";

    std::string percent_encoded;

    for (auto it = str.begin(); it != str.end(); ++it)
    {
        if (!valid_url_query_char(*it))
            percent_encoded += pe[*it];
        else
            percent_encoded.push_back(*it);
    }

    return percent_encoded;
}