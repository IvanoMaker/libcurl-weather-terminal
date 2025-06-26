# C++ Text-Based Weather App Using libcurl

This is a command-line weather application written in C++ that uses [libcurl](https://curl.se/libcurl/) to fetch weather data based on a U.S. ZIP code. It uses the [Zippopotam.us API](http://api.zippopotam.us/) to convert ZIP codes to latitude and longitude, and the [OpenWeatherMap API](https://openweathermap.org/current) to retrieve current weather conditions.

---

## 📦 Features

- Fetches city, state, and country from a U.S. ZIP code
- Gets live weather data including:
  - Temperature
  - Feels like temperature
  - Humidity
  - Weather condition (e.g., Clouds - broken clouds)
- Displays timing for each HTTP request
- Uses only standard C++ and `libcurl`
- Lightweight and easy to run

---

## 🧰 Requirements

- [MSYS2](https://www.msys2.org/) with MinGW 64-bit environment
- libcurl (included with MSYS2 via `pacman -S mingw-w64-x86_64-curl`)
- C++17-compatible compiler (e.g., `g++`)
- A **valid API key** from [OpenWeatherMap](https://home.openweathermap.org/api_keys)

---

## 🔧 Setup

1. **Clone or download the project.**

2. **Get your OpenWeatherMap API key.**

   - Create a file named `WEATHER_API_KEY.txt` in the root of the project folder.
   - Paste your API key inside that file with no extra spaces or newlines.

3. **Download the [cURL certificate bundle](https://curl.se/docs/caextract.html):**

   - Save `cacert.pem` in an `output/` folder next to the executable.

---

## 🚀 Build and Run

### 🛠️ Compile

Use the provided `compile.bat` (Windows only with MSYS2):
```bash
compile.bat
```
This will:
- Compile main.cpp
- Link against libcurl
- Output the executable to output/curl_weather_app.exe

---

⚠️ Notes & Limitations
- The JSON is parsed using a simple manual string search (extractJSONValue). For robust projects, consider using a dedicated json parser.
- Only U.S. ZIP codes are supported via the Zippopotam.us API.
- This project assumes valid and well-formed API responses.
- This project uses imperial units (&units=imperial). You can change this to metric in the weather API URL if desired.
