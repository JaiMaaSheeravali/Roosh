#include <cpr/cpr.h>
#include <iostream>
#include <string>
#include <../include/json.hpp>
using namespace std;

int google_a_description(string str, string lang)
{
    try
    {
        using json = nlohmann::json;
        str = "https://api.dictionaryapi.dev/api/v2/entries/" + lang + "/" + str;
        cpr::Response r = cpr::Get(cpr::Url{str}, cpr::Timeout{5000}, cpr::Header{{"accept", "application/json"}, {"user-agent", "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/90.0.4430.93 Safari/537.36"}, {"upgrade-insecure-requests", "1"}}, cpr::VerifySsl{false});
        auto j = json::parse(r.text);
        for (auto ar : j[0].at("meanings"))
        {
            cout << ar.at("partOfSpeech") << "  " << ar.at("definitions")[0].at("definition") << endl;
        }
    }
    catch (...)
    {
        cout << "Sorry this word cannot be found at this moment\n";
    }
    return 0;
}