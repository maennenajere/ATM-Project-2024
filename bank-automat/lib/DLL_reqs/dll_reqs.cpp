#include "dll_reqs.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *data) {
    size_t total_size = size * nmemb;
    data->append((char*)contents, total_size);
    return total_size;
}

std::string makeReq(const char* url, std::string cookie, const std::string &data, bool isPost) {
    CURL *curl;
    CURLcode res;
    std::string response;

    struct curl_slist *list = NULL;

    std::ostringstream cookieStream;
    cookieStream << "Cookie: " << cookie;
    list = curl_slist_append(list, cookieStream.str().c_str());

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        if (isPost) {
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            list = curl_slist_append(list, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_slist_free_all(list);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    return response;
}
