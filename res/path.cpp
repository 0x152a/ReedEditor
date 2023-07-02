
#ifndef __std_string
#    define __std_string
#    include <string>
typedef std::string      string;
typedef std::string string;
#endif

inline string getFileExt(string path)
{
    return path.substr(path.find_last_of('.') + 1);
}

inline string getFileName(string path)
{
    return path.substr(path.find_last_of('\\') + 1);
}

inline string getFileNameWithoutExt(string path)
{
    return path.substr(getFileName(path).find_last_of('.') + 1);
}
