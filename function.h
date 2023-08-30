#include <iostream>
#include <curl/curl.h>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <random>
#include <algorithm>

size_t discardDataCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    return size * nmemb;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

std::string exec(const char* url) {
    std::string result;

    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "CURL initialization failed." << std::endl;
    }

    return result;
}

void hit_sender(const std::string& tg_id, const std::string& text, const std::string& bot_token) {
    CURL* curl = curl_easy_init();
    char* encodedText = curl_easy_escape(curl, text.c_str(), text.length());
    std::string url = "https://api.telegram.org/bot" + bot_token + "/sendMessage?chat_id=" + tg_id + "&text=" + encodedText;
    curl_free(encodedText);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discardDataCallback); // Set the discard function
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
}

std::vector<std::string> multiexplode(const std::vector<char>& separators, const std::string& str) {
    std::string modifiedStr = str;
    for (char sep : separators) {
        modifiedStr = std::regex_replace(modifiedStr, std::regex("\\" + std::string(1, sep)), std::string(1, separators[0]));
    }

    std::vector<std::string> result;
    std::string delimiter = std::string(1, separators[0]);
    size_t pos = 0;
    while ((pos = modifiedStr.find(delimiter)) != std::string::npos) {
        std::string token = modifiedStr.substr(0, pos);
        result.push_back(token);
        modifiedStr.erase(0, pos + delimiter.length());
    }
    result.push_back(modifiedStr);
    return result;
}

std::string Getstr(const std::string& input, const std::string& start, const std::string& end) {
    std::size_t startPos = input.find(start);
    if (startPos == std::string::npos)
        return "";

    startPos += start.length();
    std::size_t endPos = input.find(end, startPos);
    if (endPos == std::string::npos)
        return "";

    return input.substr(startPos, endPos - startPos);
}

std::string findValue(const std::string& jsonString, const std::string& key) {
    std::size_t keyPosition = jsonString.find("\"" + key + "\":");
    if (keyPosition == std::string::npos) {
        return "";
    }

    std::size_t valuePosition = keyPosition + key.length() + 2; 
    std::size_t valueLength = jsonString.find_first_of(",}", valuePosition) - valuePosition;

    std::string value = jsonString.substr(valuePosition, valueLength);

    if (!value.empty() && value[0] == ':') {
        value.erase(0, 1);
    }
    value.erase(std::remove(value.begin(), value.end(), '\"'), value.end());

    return value;
}

std::vector<std::string> splitString(const std::string& input, const char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(input);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string getRandomElement(const std::vector<std::string>& elements) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, elements.size() - 1);
    
    return elements[dist(gen)];
}

size_t writeData(void* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* response = static_cast<std::string*>(userdata);
    size_t totalSize = size * nmemb;

    response->append(static_cast<const char*>(ptr), totalSize);

    return totalSize;
}

bool check_proxy(const std::string& proxy) {

    bool invalid = true;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org");
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        bool invalid = false;
    } else {
        bool invalid = true;
        }
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return invalid;
}

