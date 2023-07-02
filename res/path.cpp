
#ifndef __std_string
#define __std_string
#include <string>
typedef std::string string;
typedef std::string_view string_view;
#endif

inline string_view getFileExt(string_view path){
    return path.substr(path.find_last_of('.') + 1);
}

inline string_view getFileName(string_view path){
    return path.substr(path.find_last_of('\\') + 1);
}

inline string_view getFileNameWithoutExt(string_view path){
    return path.substr(getFileName(path).find_last_of('.') + 1);
}
