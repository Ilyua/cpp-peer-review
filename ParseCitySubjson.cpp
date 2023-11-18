struct CountryInfo {
    string name;
    string iso_code;
    string phone_code;
    string time_zone;
    vector<Language> languages;
};

void ParseCitySubjson(vector<City>& cities, const Json& json, const CountryInfo& country_info) {
    const string& country_name = country_info.name;
    const string& country_iso_code = country_info.iso_code;
    const string& country_phone_code = country_info.phone_code;
    const string& country_time_zone = country_info.time_zone;
    const vector<Language>& languages = country_info.languages;

    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          country_phone_code + city_obj["phone_code"s].AsString(), country_name, country_iso_code,
                          country_time_zone, languages});
    }
}

void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        CountryInfo country_info {
                country_obj["name"s].AsString(),
                country_obj["iso_code"s].AsString(),
                country_obj["phone_code"s].AsString(),
                country_obj["time_zone"s].AsString(),
                {}
        };

        Country& country = countries.emplace_back(country_info.name, country_info.iso_code, country_info.phone_code, country_info.time_zone);

        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }

        ParseCitySubjson(cities, country_obj["cities"s], country_info);
    }
}
