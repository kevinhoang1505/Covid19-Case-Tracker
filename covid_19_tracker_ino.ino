/* This code works with ESP8266 12E (NodeMcu v1.0) its main function is to read the data from https://www.worldometers.info/
 * Through ThingSpeak app ThingHTTP and displays on the Serial monitor
 * Refer to www.SurtrTech.com for more details and video about the project
 * This project was created during the COVID-19 Pandemic and the example here used to track the numbers in Italy  
 */

#include <ESP8266WiFi.h>        //Use ESP8266 functions                                              
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     LED_BUILTIN  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
                                                                       
const char* ssid = "FPT Bao Chung";              //Your router SSID and password                             
const char* password =  "02071976"; 
const char* host = "api.thingspeak.com";  //We read the data from this host                                   
const int httpPortRead = 80;                                           
                                                
const char* url1 = "/apps/thinghttp/send_request?api_key=ZPI800ZLLKJKSURV";     //Those URLs are mine but I kept them so you can replace the key only

int To_remove;      //There are some irrelevant data on the string and here's how I keep the index
                    //of those characters 

String Data_Raw;  //Here I keep the numbers that I got
char Cases[12],Death[12],Recover[12];
bool complete = false;
WiFiClient client;      //Create a WiFi client and http client                                                     
HTTPClient http; 

void setup() {
  
  Serial.begin(9600);            //Start the serial communication 
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();                                                             
 
  WiFi.disconnect();             //Disconnect and reconnect to the Wifi you set                                                 
  delay(1000);                                                                  
  WiFi.begin(ssid, password);                                                   

  Serial.println("Connected to the WiFi network");   //Display feedback on the serial monitor                                        
  Serial.println(WiFi.localIP()); 
  
}

//In the loop I read every URL separately and I get the data string then I remove the unecessary characters
//I keep only the values that I can display either on the serial monitor or the display
//I believe that this part is repeated 3 times and can be set on a single function that can be called
//Keep updated on www.SurtrTech.com for an optimized code  

 void loop()                                                                      
{
     //Reading 1: Reading of cases
    if( http.begin(host,httpPortRead,url1))        //Connect to the host and the url                                      
      {
        int httpCode = http.GET();                //Check feedback if there's a response                                                  
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw = http.getString();   //Here we store the raw data string

              Data_Raw = http.getString(); 
              Data_Raw.toCharArray(Cases,10);    
              Data_Raw.remove(0,10);
              Data_Raw.toCharArray(Death,8);
              Data_Raw.remove(0,7);
              Data_Raw.toCharArray(Recover,8);;                                                                                         
              Serial.print("Cases: ");  //I choosed to display it on the serial monitor to help you debug
              Serial.println(Cases);        
              Serial.print("Death: ");  //I choosed to display it on the serial monitor to help you debug
              Serial.println(Death);
              Serial.print("Recover: ");  //I choosed to display it on the serial monitor to help you debug
              Serial.println(Recover);                                        
              complete = true;                                                                          
            }
        }
        else //If we can't get data
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }



        http.end();                                                                 
      } 
    else //If we can't connect to the HTTP
      {
        Serial.printf("[HTTP} Unable to connect\n");
      }

    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    
    display.setCursor(15,0);             // Start at top-left corner
    display.println("Covid-19");
    display.setCursor(30,30);
    display.println("Cases");
    display.setCursor(10,45);
    display.println(Cases);
    display.display();
    delay(3000);
    display.clearDisplay();
    
    display.setCursor(15,0);             // Start at top-left corner
    display.println("Covid-19");
    display.setCursor(30,30);
    display.println("Death");
    display.setCursor(20,45);
    display.println(Death);
    display.display();
    delay(3000); 
    display.clearDisplay();

    display.setCursor(15,0);             // Start at top-left corner
    display.println("Covid-19");
    display.setCursor(10,30);
    display.println("Recovered");
    display.setCursor(20,45);
    display.println(Recover);
    display.display();
    delay(3000); 
    display.clearDisplay();
    
    while (WiFi.status() != WL_CONNECTED)     //In case the Wifi connexion is lost                                    
      { 
        
        WiFi.disconnect();                                                        
        delay(1000);                                                             
        
        WiFi.begin(ssid, password);                                              
        Serial.println("Reconnecting to WiFi..");       
        delay(10000);                                                             
      }

} 
  
