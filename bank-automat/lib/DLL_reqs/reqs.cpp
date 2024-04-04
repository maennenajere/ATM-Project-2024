#include<iostream>
#include<format>
#include<curl/curl.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *data) {
    size_t total_size = size * nmemb;
    data->append((char*)contents, total_size);
    return total_size;
}

std::string makeReq(const char* url, std::string cookie, const std::string &data, bool isPost) {
    CURL *curl;
    CURLcode res;
    std::string response = "empty";

    struct curl_slist *list = NULL;
    list = curl_slist_append(list, std::format("Cookie: {}", cookie).c_str());

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

/*
int main() {
    const char* url = "http://example.com";
    std::string cookie = "ilikecats";
    std::string res = makeReq(url, cookie, "{\"test\":\"yes\"}", true); // this is a post req
    std::string res = makeReq(url, cookie, "", false); // this is a get req
    std::cout << "result " << res << "\n";
    return 0;
}
*/
