#include <ESP8266WiFi.h>                                                 
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 


#define OLED_RESET     LED_BUILTIN  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
                                                                       
const char* ssid = "SSID";                                     
const char* password =  "********"; 
const char* host = "api.thingspeak.com";                           
const int httpPortRead = 80;                                           
                                                
const char* url1 = "url thingspeak";     

int To_remove;      

String Data_Raw;  
char Cases[12],Death[12],Recover[12];
bool complete = false;
WiFiClient client;                                                        
HTTPClient http; 

void setup() {
  
  Serial.begin(9600);            
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();                                                             
 
  WiFi.disconnect();                                 
  delay(1000);                                                                  
  WiFi.begin(ssid, password);                                                   

  Serial.println("Connected to the WiFi network");                                    
  Serial.println(WiFi.localIP()); 
  
}
 void loop()                                                                      
{
     //Reading 1: Reading of cases
    if( http.begin(host,httpPortRead,url1))                                 
      {
        int httpCode = http.GET();                                                                 
        if (httpCode > 0)                                                              
        {
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
            {                
              
              Data_Raw = http.getString();   

              Data_Raw = http.getString(); 
              Data_Raw.toCharArray(Cases,10);    
              Data_Raw.remove(0,10);
              Data_Raw.toCharArray(Death,8);
              Data_Raw.remove(0,7);
              Data_Raw.toCharArray(Recover,8);;                                                                                         
              Serial.print("Cases: ");  
              Serial.println(Cases);        
              Serial.print("Death: ");  
              Serial.println(Death);
              Serial.print("Recover: ");  
              Serial.println(Recover);                                        
              complete = true;                                                                          
            }
        }
        else 
        {
          Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }



        http.end();                                                                 
      } 
    else 
      {
        Serial.printf("[HTTP} Unable to connect\n");
      }

    display.setTextSize(2);             
    display.setTextColor(SSD1306_WHITE);        
    
    display.setCursor(15,0);             
    display.println("Covid-19");
    display.setCursor(30,30);
    display.println("Cases");
    display.setCursor(10,45);
    display.println(Cases);
    display.display();
    delay(3000);
    display.clearDisplay();
    
    display.setCursor(15,0);             
    display.println("Covid-19");
    display.setCursor(30,30);
    display.println("Death");
    display.setCursor(20,45);
    display.println(Death);
    display.display();
    delay(3000); 
    display.clearDisplay();

    display.setCursor(15,0);             
    display.println("Covid-19");
    display.setCursor(10,30);
    display.println("Recovered");
    display.setCursor(20,45);
    display.println(Recover);
    display.display();
    delay(3000); 
    display.clearDisplay();
    
    while (WiFi.status() != WL_CONNECTED)                                     
      { 
        
        WiFi.disconnect();                                                        
        delay(1000);                                                             
        
        WiFi.begin(ssid, password);                                              
        Serial.println("Reconnecting to WiFi..");       
        delay(10000);                                                             
      }

} 
  
