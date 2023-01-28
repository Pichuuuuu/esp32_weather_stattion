# DIY Weather Station

This project is a simple weather station that uses an ESP32 microcontroller to collect data from a wind speed sensor and a BME280 sensor for measuring temperature, humidity, and pressure. The data is then sent to two different weather APIs (Windguru and Windy) for storage and display on their respective websites.

## Hardware

-   ESP32 microcontroller
-   BME280 sensor for temperature, humidity, and pressure
-   DAVIS ventage 2 anemometer for wind speed and wind direction

## Software

-   Arduino IDE for programming and uploading code to the ESP32
-   Adafruit BME280 library for reading data from the BME280 sensor
-   WiFi library for connecting the ESP32 to a WiFi network
-   HTTPClient library for sending data to the weather APIs

## Setup

1.  Connect the ESP32 to your computer using a micro-USB cable.
2.  Open the Arduino IDE and install the necessary libraries by going to Sketch > Include Library > Manage Libraries. Search for and install the Adafruit BME280, WiFi, and HTTPClient libraries.
3.  Open the weather_station.ino file in the Arduino IDE.
4.  Update the following variables with your own information:
    -   `ssid` and `password` with the name and password of your WiFi network
    -   `WINDGURU_ID`, `WINDGURU_SALT`, and `WINDGURU_HASH` with your Windguru API credentials
    -   `WINDY_KEY` with your Windy API key
5.  Upload the code to the ESP32 by clicking the upload button in the Arduino IDE.

## Usage

Once the code is uploaded and the ESP32 is connected to a WiFi network, the weather station will automatically begin sending data to the Windguru and Windy APIs. The data can then be viewed on the respective websites.

## Note

This is a sample code, you should check for the correct pinout and libraries for your specific board. Also you should check for the correct API endpoints and credentials for the API you want to use.
