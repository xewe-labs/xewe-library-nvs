# XeWeNvs

Typed key-value storage on the ESP32 NVS partition. NVS is initialised on first use.

```cpp
#include <XeWeNvs.h>

xewe::Nvs nvs;

nvs.write<uint32_t>("app", "boots", 5);
uint32_t boots = nvs.read<uint32_t>("app", "boots", 0);     // default if missing
```

| API | |
|---|---|
| `write<T>` / `read<T>` | bool, integers, float/double, `std::string`, `String` |
| `write_blob` / `read_blob` | raw bytes |
| `write_flex` / `read_flex` | any `xewe::FlexData<T>` struct |
| `remove(ns, key)`, `reset_ns(ns)` | delete one key / a whole namespace |
| `erase_all()` | wipe the entire NVS partition |
| `set_error_handler(fn)` | where errors go (default: ESP log) |

Namespaces and keys are limited to 15 characters.

## FlexData

Derive from `xewe::FlexData<T>` and list the fields once; the struct gets a compact
binary form for flash and a JSON form for APIs, including nested structs and vectors.

```cpp
struct Settings : xewe::FlexData<Settings> {
    uint8_t     brightness = 128;
    std::string name;

    static constexpr auto fields() {
        return std::make_tuple(fld("brightness", &Settings::brightness),
                               fld("name",       &Settings::name));
    }
};

Settings s;
nvs.read_flex("app", "settings", s);
s.update(R"({"brightness": 200})");   // partial JSON merge
nvs.write_flex("app", "settings", s);
std::string json = s.as_json_str();
```

Depends on ArduinoJson 7. See `examples/ReadWrite` and `examples/FlexDataStruct`.
