#ifndef __std_string
#    define __std_string
#    include <string>
typedef std::string string;
typedef std::string string;
#endif

#ifndef __stringstream
#    define __stringstream
#    include <sstream>
#endif

template <typename... Args>
inline std::string joinStr(const Args&... args)
{
    std::ostringstream data;
    (data << ... << args);
    return data.str();
}

template <typename... Args>
inline std::string view2String(const Args&... args)
{
    std::ostringstream data;
    (data << ... << args);
    return data.str();
}
