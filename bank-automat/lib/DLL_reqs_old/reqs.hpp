#include <string>
#ifndef REQS_HPP
#define REQS_HPP

#ifdef __cplusplus
extern "C++" {
#endif

#ifdef _WIN32
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __attribute__((visibility("default")))
#endif

DLL_EXPORT std::string makeReq(const char* url, std::string cookie, const std::string &data, bool isPost);

#ifdef __cplusplus
}
#endif

#endif /* REQS_HPP */
