#include "Adafruit_DHT/Adafruit_DHT.h"                                          //DHT libaries found in the online particle ide
#include "HttpClient/HttpClient.h"                                              //Http libaries found in the online particle ide

HttpClient http;

#define VARIABLE_ID  " "                                 //Variable ID for Temperature inside, must be set inbetween " "
#define VARIABLE_ID2 " "                                 //Variable ID for Humidity inside, must be set inbetween " "                                         
#define VARIABLE_ID4 " "                                 //Variable ID for Temperature outside, must be set inbetween " "           
#define VARIABLE_ID5 " "                                 //Variable ID for Humidity outside, must be set inbetween " "

#define TOKEN " "                                        //Acess Token, must be set inbetween " "

#define DHTPINinside 2                                   //Photon Pin used for DHT sensor inside
#define DHTPINoutside 3                                  //Photon Pin used for DHT sensor outside

#define DHTinsideTYPE DHT22                              //Define DHT type (either DHT22 or DHT11)
#define DHToutsideTYPE DHT22  

unsigned long lastTime = 0UL;                            //Define lastTime to 0


DHT dhtinside(DHTPINinside, DHTinsideTYPE);
DHT dhtoutside(DHTPINoutside, DHToutsideTYPE);


http_header_t headers[] = {                             //https://github.com/nmattisson/HttpClient                   
      { "Content-Type", "application/json" },
      { "X-Auth-Token" , TOKEN },
    { NULL, NULL }                                      //NOTE: Always terminate headers will NULL
};

http_request_t request;
http_response_t response;


void setup() {
    
    request.hostname = "things.ubidots.com";            //From Ubidots http://ubidots.com/docs/api/
    request.port = 80;
    
    dhtinside.begin();                                  //Begin reading form the DHT sensor
    dhtoutside.begin();
    
    RGB.control(true);                                  //Set the Photon rgb led to a lower brightness 
    RGB.brightness(3);
    RGB.control(false);
}

void loop() {                                           //Start the Loop to post the Temperature to the Server 
    
    unsigned long now = millis();
    
     
    if (now-lastTime>300000UL) {                        //Check if time has progressed for 5 min if so post Temperature to the Server
        lastTime = now;                                 //300000 milliseconds = 5 minutes, 600000 msec = 10 min
        
        float h_outside = dhtoutside.getHumidity();
        float t_outside = dhtoutside.getTempCelcius();
        float f_outside = dhtoutside.getTempFarenheit();
        
        request.path = "/api/v1.6/variables/"VARIABLE_ID4"/values";
        request.body = "{\"value\":" + String(f_outside) + "}";
        http.post(request, response, headers);
        
        request.path = "/api/v1.6/variables/"VARIABLE_ID5"/values";
        request.body = "{\"value\":" + String(h_outside) + "}";
        http.post(request, response, headers);
        
        float h_inside = dhtinside.getHumidity();
        float t_inside = dhtinside.getTempCelcius();
        float f_inside = dhtinside.getTempFarenheit();
        
        request.path = "/api/v1.6/variables/"VARIABLE_ID"/values";
        request.body = "{\"value\":" + String(f_inside) + "}";
        http.post(request, response, headers);
        
        request.path = "/api/v1.6/variables/"VARIABLE_ID2"/values";
        request.body = "{\"value\":" + String(h_inside) + "}";
        http.post(request, response, headers);
    }
}