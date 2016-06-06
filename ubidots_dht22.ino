#include "Adafruit_DHT/Adafruit_DHT.h"                                          //DHT libaries
#include "HttpClient/HttpClient.h"                                              //Http libaries


HttpClient http;
#define VARIABLE_ID  "ID"                                 //Variable ID (F) for Temperature inside DHT22
#define VARIABLE_ID2 "ID"                                 //Variable ID (%) for Humidity inside DHT22
#define VARIABLE_ID3 "ID"                                 //Variable ID(F) for Temperature outside LM335
#define VARIABLE_ID4 "ID"                                 //Variable ID(F) for Temperature outside DHT22
#define VARIABLE_ID5 "ID"                                 //Variable ID (%) for Humidity outside DHT22

#define TOKEN "Token"                                     //Acess Token

#define DHTPINinside 2                                    // what digital pin DHT22 inside is connected to
#define DHTPINoutside 3                                   // what digital pin DHT22 outside is connected to

#define DHTinsideTYPE DHT22		                            // DHT 22 
#define DHToutsideTYPE DHT22	                            // DHT 22 

unsigned long lastTime = 0UL;
float temp_in_kelvin=0, temp_in_fahrenheit=0;


DHT dhtinside(DHTPINinside, DHTinsideTYPE);
DHT dhtoutside(DHTPINoutside, DHToutsideTYPE);


http_header_t headers[] = {
      { "Content-Type", "application/json" },
      { "X-Auth-Token" , TOKEN },
    { NULL, NULL }                                        // NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;


void setup() {
    
    request.hostname = "things.ubidots.com";
    request.port = 80;
    
    dhtinside.begin();
    dhtoutside.begin();
    
    RGB.control(true);                                    // Set Particle Core Status LED to low brightness
    RGB.brightness(3);
    RGB.control(false);
}

void loop() {
    
    
    unsigned long now = millis();                         //Every 10 min publish uptime
    
    if (now-lastTime>300000UL) {
        lastTime = now;
        
        float h_inside = dhtinside.getHumidity();
   	    float t_inside = dhtinside.getTempCelcius();
   	    float f_inside = dhtinside.getTempFarenheit();
   	    
   	    float h_outside = dhtoutside.getHumidity();
   	    float t_outside = dhtoutside.getTempCelcius();
   	    float f_outside = dhtoutside.getTempFarenheit();
   	    
   	    temp_in_kelvin = analogRead(0) * 0.08056640625;
   	    temp_in_fahrenheit = ((temp_in_kelvin ) * 9 / 5) - 460.67;          // Calculate LM335 temperature 
   	    
        request.path = "/api/v1.6/variables/"VARIABLE_ID"/values";
        request.body = "{\"value\":" + String(f_inside) + "}";
        http.post(request, response, headers);
        
        request.path = "/api/v1.6/variables/"VARIABLE_ID2"/values";
        request.body = "{\"value\":" + String(h_inside) + "}";
        http.post(request, response, headers);
        
        request.path = "/api/v1.6/variables/"VARIABLE_ID4"/values";
        request.body = "{\"value\":" + String(f_outside) + "}";
        http.post(request, response, headers);
        
        request.path = "/api/v1.6/variables/"VARIABLE_ID5"/values";
        request.body = "{\"value\":" + String(h_outside) + "}";
        http.post(request, response, headers);
        
        request.path = "/api/v1.6/variables/"VARIABLE_ID3"/values";
        request.body = "{\"value\":" + String(temp_in_fahrenheit) + "}";
        http.post(request, response, headers);

    }
}
