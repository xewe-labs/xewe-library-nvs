// XeWeNvs: persist a whole struct (including nested vectors) with FlexData
#include <XeWeNvs.h>

struct Alarm : xewe::FlexData<Alarm> {
    uint16_t    minute_of_day = 0;
    std::string label;

    static constexpr auto fields() {
        return std::make_tuple(
            fld("minute_of_day", &Alarm::minute_of_day),
            fld("label",         &Alarm::label)
        );
    }
};

struct Settings : xewe::FlexData<Settings> {
    uint8_t            brightness = 128;
    std::vector<Alarm> alarms;

    static constexpr auto fields() {
        return std::make_tuple(
            fld("brightness", &Settings::brightness),
            fld("alarms",     &Settings::alarms)
        );
    }
};

xewe::Nvs nvs;
Settings  settings;

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!nvs.read_flex("demo", "settings", settings)) {
        Serial.println("No saved settings, creating defaults");
        settings.alarms.push_back({});
        settings.alarms.back().minute_of_day = 7 * 60;
        settings.alarms.back().label         = "wake up";
        nvs.write_flex("demo", "settings", settings);
    }

    Serial.printf("loaded: %s\n", settings.as_json_str().c_str());

    // partial update from JSON: only keys present are changed
    settings.update(R"({"brightness": 200})");
    nvs.write_flex("demo", "settings", settings);
    Serial.printf("saved:  %s\n", settings.as_json_str().c_str());
}

void loop() {}
