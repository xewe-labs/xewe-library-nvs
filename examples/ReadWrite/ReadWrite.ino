// XeWeNvs: typed values that survive a reboot
#include <XeWeNvs.h>

xewe::Nvs nvs;

void setup() {
    Serial.begin(115200);
    delay(1000);

    // namespaces and keys are limited to 15 characters by ESP-IDF
    uint32_t boots = nvs.read<uint32_t>("demo", "boot_count", 0) + 1;
    nvs.write<uint32_t>("demo", "boot_count", boots);

    if (nvs.read<std::string>("demo", "owner").empty()) {
        nvs.write<std::string>("demo", "owner", "first boot owner");
    }

    Serial.printf("boot #%u, owner: %s\n", boots, nvs.read<std::string>("demo", "owner").c_str());
    Serial.println("Press reset to count another boot.");
}

void loop() {}
