#include <fstream>
#include <streambuf>
#include <iostream>
#include <curl/curl.h>
#include <string>
#include <ctime>

// Callback function to write received data into a std::string.
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// function to make a HTTP request
// takes a URL and a reference to a string to store the response
// returns the time taken for the request in seconds
double makeRequest(const std::string& url, std::string& response) {
    CURL* curl = curl_easy_init(); // Initialize a cURL session
    clock_t start, end;            // time keeping variables
    double cpu_time_used;          // double to store the time taken for the request
    response.clear();              // Clear the response string before making a new request
    // if the cURL session is initialized successfully, set options and perform the request
    if (curl) {
        // curl setup stuff
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "output/cacert.pem");
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/126.0.0.0 Safari/537.36");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        start = clock();                        // start clock
        CURLcode res = curl_easy_perform(curl); // send request, program times out until the request is complete
        end = clock();                          // end clock
        cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC; // calculate the time taken for the request
        // check for errors in the request
        if (res != CURLE_OK) {
            // If the request failed, print the error message
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }
        // clean up the cURL session to protect against memory leaks
        curl_easy_cleanup(curl);
    }   // if the cURL session was not initialized successfully, print an error message 
    else {
        std::cerr << "Error: Could not initialize libcurl." << std::endl;
        return -1.0; // Indicate failure to initialize libcurl
    }
    return cpu_time_used; // Return the time taken for the request
}

// Function to extract the text from a text file
// used soley for accessing the weather API key
std::string readFileToString(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return "";
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

// basic JSON parser function
std::string extractJSONValue(const std::string& json, const std::string& key) {
    std::string pattern = "\"" + key + "\"";
    size_t keyPos = json.find(pattern);
    if (keyPos == std::string::npos) {
        return ""; // Key not found
    }

    size_t colonPos = json.find(':', keyPos);
    if (colonPos == std::string::npos) {
        return ""; // Malformed JSON
    }

    // Skip whitespace after colon
    size_t valueStart = json.find_first_not_of(" \t\n\r", colonPos + 1);
    if (valueStart == std::string::npos) return "";

    // Handle string value
    if (json[valueStart] == '"') {
        valueStart++; // skip opening quote
        size_t valueEnd = json.find('"', valueStart);
        return json.substr(valueStart, valueEnd - valueStart);
    }

    // Handle number, boolean, null
    size_t valueEnd = json.find_first_of(",}", valueStart);
    return json.substr(valueStart, valueEnd - valueStart);
}

// main function
int main(int argc, char* argv[]) {
    
    // initialize variables
    std::string url;        // url for the curl request
    std::string zipcode;    // zip code to be used in the request
    std::string response;   // string response variable
    std::string lat, lon;   // latitude and longitude variables to be used in the weather API request
    // API urls and API key file location
    std::string locationAPI = "https://api.zippopotam.us/us/";
    std::string weatherAPI = "https://api.openweathermap.org/data/2.5/weather?lat=";
    std::string weatherAPIKeyFileLocation = "WEATHER_API_KEY.txt";
    std::string weatherAPIKey = readFileToString(weatherAPIKeyFileLocation);
    
    // Determine the zip code from command-line arguments.
    if (argc > 1) { // zip code provided
        zipcode = argv[1];
        std::cout << std::endl << "    C++ Libcurl Weather App" << std::endl;
        std::cout << "[1/2] Fetching location data for " << zipcode << "..." << std::endl;
        locationAPI += zipcode;
    } else {        // no zip code provided, prompt user for input
        std::cout << std::endl << "    C++ Libcurl Weather App" << std::endl;
        std::cout << "    Please enter a zip code: ";
        std::getline(std::cin, zipcode);
        if (zipcode.empty()) {
            std::cerr << "Error: No zip code provided. Exiting." << std::endl;
            return 1; // Exit if no zip code is provided
        }
        std::cout << "[1/2] Fetching location data for " << zipcode << "..." << std::endl;
        locationAPI += zipcode;
    }

    // Make the HTTP request and measure the time taken.
    double timeTaken = makeRequest(locationAPI, response);
    
    // Check if the response is empty (response would be "{}")
    if (response.length() <= 2) {
        std::cerr << "  x | Error: No response received from the server. Check the zip code and try again" << std::endl;
        return 1; // Exit if no response is received
    } else {
        std::cout << "    | Response received from zippopotam.us" << std::endl;
        std::cout << "    | Time taken for request: " << timeTaken << " seconds" << std::endl;
        std::cout << "    | Processing location info for " << extractJSONValue(response, "place name") << ", " << extractJSONValue(response, "state abbreviation") << ", " << extractJSONValue(response, "country") << std::endl;
        
        lat = extractJSONValue(response, "latitude");  // get latitude from the response
        lon = extractJSONValue(response, "longitude"); // get longitude from the response
        
        std::cout << "[2/2] Making weather api call..." << std::endl;
        // Construct the weather API URL using the latitude and longitude
        url = weatherAPI + lat + "&lon=" + lon + "&appid=" + weatherAPIKey + "&units=imperial";
        
        // Make the HTTP request and measure the time taken.
        timeTaken = makeRequest(url, response);

        // Check if the response is empty (response would be "{}")
        if (response.length() <= 2) {
            std::cerr << "  x | Error: No response received from the weather server. Check the API key and try again" << std::endl;
            return 1; // Exit if no response is received
        } else {
            std::cout << "    | Response received from OpenWeatherMap.org" << std::endl;
            std::cout << "    | Time taken for request: " << timeTaken << " seconds" << std::endl << std::endl;
            std::cout << "=============================" << std::endl;
            std::cout << "Current Weather for " << extractJSONValue(response, "name") << std::endl;
            std::cout << "=============================" << std::endl;

            std::cout << "  Condition: " << extractJSONValue(response, "main") << " - " << extractJSONValue(response, "description") << std::endl;
            std::cout << "Temperature: " << extractJSONValue(response, "temp") << "(F)" << std::endl;
            std::cout << " Feels Like: " << extractJSONValue(response, "feels_like") << "(F)" << std::endl;
            std::cout << "   Humidity: " << extractJSONValue(response, "humidity") << "%" << std::endl << std::endl;
        }
    }
}