//
// Created by stepan on 09.01.17.
//

#include <curl/curl.h>

static size_t _requestCallback(void *ptr, size_t size, size_t nmemb, struct string *s) {
    size_t new_len = s->len + size*nmemb,
    old_len = s->len;
    _new_string(s, new_len+1);
    memcpy(s->ptr+old_len, ptr, size*nmemb);
    return  size*nmemb;
}

char *getRequestUrl(char *params) {
    struct string *url, *p;
    url = newString(auth_api_url);
    p = concat(newString("?access_key="), newString(auth_api_secret));
    p = concat(p, newString("&"));
    p = concat(p, newString(params));
    url = concat(url, p);
    char *r = getChar(url);
    free(&url->ptr);
    free(&p->ptr);
    return r;
}

struct string *makeRequest(char *url) {
    struct string *response = newString("");
    CURL *curl= curl_easy_init();

    if (curl) {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _requestCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    return response;
}
