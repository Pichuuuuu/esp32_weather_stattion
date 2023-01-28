#include <math.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define WINDDIR 33
#define WINDSPEED 18
#define T 5
String WINDGURU_API = "http://www.windguru.cz/upload/api.php";
String WINDGURU_ID = "YOUR_ID";
String WINDGURU_SALT = "YOUR_SALT";
String WINDGURU_HASH = "YOUR_HASH";
String WINDY_API = "http://stations.windy.com/pws/update/";
String WINDY_KEY = "YOUR_KEY";
Adafruit_BME280 bme;

volatile unsigned long Rotations;
volatile unsigned long ContactBounceTime;
const char* ssid = "YOUR_SSID_WIFI";
const char* password = "YOUR_PASSWORD_WIFI";
float WindSpeed;

int get_wind_dir() {
  return(map(analogRead(WINDDIR), 0, 4095, 0, 360) % 360);  
}

void isr_rotation () {
  if ((millis() - ContactBounceTime) > 15 ) {
    Rotations++;
    ContactBounceTime = millis();
  }
}

float get_wind_speed() {
  float WindSpeed = 0;

  Rotations = 0;
  attachInterrupt(digitalPinToInterrupt(WINDSPEED), isr_rotation, FALLING);
  delay(T * 1000);
  detachInterrupt(digitalPinToInterrupt(WINDSPEED));
  WindSpeed = Rotations * (2.25 / T);
  Serial.println(WindSpeed);
  return(WindSpeed);
}

void up_windy_api(float wind_avg, float wind_max, int wind_dir)
{
  HTTPClient http;
  http.begin(WINDY_API + WINDY_KEY + "?windspeedmph=" + String(wind_avg) + "&winddir=" + String(wind_dir) + "&windgustmph=" + String(wind_max));
  int rep = http.GET();
  //Serial.println(WINDGURU_API + "?uid=" + WINDGURU_ID + "&salt=" + WINDGURU_SALT + "&hash=" + WINDGURU_HASH + "&wind_avg=" + String(wind_avg) + "&wind_dir=" + String(wind_dir));
  Serial.println("http code : " + String(rep));
}

void up_windguru_api(float wind_avg, float wind_min, float wind_max, int wind_dir, float rh, float temperature, float pressure)
{
  HTTPClient http;
  http.begin(WINDGURU_API + "?uid=" + WINDGURU_ID + "&salt=" + WINDGURU_SALT + "&hash=" + WINDGURU_HASH + "&wind_avg=" + String(wind_avg / 1.151) + "&wind_direction=" + String(wind_dir) + "&wind_max=" + String(wind_max / 1.151) + "&wind_min=" + String(wind_min / 1.151) + "&rh=" + String(rh) + "&temperature=" + String(temperature) + "&mslp=" + pressure);
  int rep = http.GET();
  //Serial.println(WINDGURU_API + "?uid=" + WINDGURU_ID + "&salt=" + WINDGURU_SALT + "&hash=" + WINDGURU_HASH + "&wind_avg=" + String(wind_avg) + "&wind_dir=" + String(wind_dir));
  Serial.println("http code : " + String(rep));
}

void setup() {
  Serial.begin(9600);
  pinMode(27, OUTPUT);     
  digitalWrite(27, HIGH);
  pinMode(26, OUTPUT);     
  digitalWrite(26, LOW);
  pinMode(WINDSPEED, INPUT_PULLUP);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  bme.begin(0x76);
}


void loop() {
  float wind_speed = 0;
  float wind_avg = -1;
  float wind_max = 0;
  float wind_min = 1000;
  int wind_dir_avg = -1;
  int wind_dir_avg_tmp = 0;
  float temperature;
  float humidity;
  float pressure;

  for (int i = 0; i < 12 ; i++){
    wind_speed = get_wind_speed();
    if(wind_max < wind_speed)
      wind_max = wind_speed;
    if(wind_min > wind_speed)
      wind_min = wind_speed;
    if(wind_avg >= 0)
      wind_avg = (wind_avg + wind_speed) / 2.0;
    else
      wind_avg = wind_speed;
  }
  wind_dir_avg = get_wind_dir();
  delay(1000);
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  Serial.println("wind AVG : " + String(wind_avg));
  Serial.println("wind dir : " + String(wind_dir_avg));
  if(WiFi.status() == WL_CONNECTED)
  {
    up_windguru_api(wind_avg, wind_min, wind_max, wind_dir_avg, humidity, temperature, pressure);
    delay(1000);
    up_windy_api(wind_avg, wind_max, wind_dir_avg);
  }
}
