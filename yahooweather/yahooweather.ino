//Main Code


#include <SoftwareSerial.h>


#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" 


String ADDRESS_FOR_FORECAST = "New York,NY"; //Enter zipcode for where you want to check the weather (or city,state)

int numRuns = 1;   // execution count, so that this doesn't run forever
int maxRuns = 10;  // max number of times the Yahoo WeatherByAddress Choreo should be run


void setup() {
  Serial.begin(9600); 
  Bridge.begin();

}


    

void loop()
{
  // while we haven't reached the max number of runs...
  if (numRuns <= maxRuns) {
      
    // print status
    Serial.println("Running GetWeatherByAddress - Run #" + String(numRuns++) + "...");

    // create a TembooChoreo object to send a Choreo request to Temboo
    TembooChoreo GetWeatherByAddressChoreo;
    
    // invoke the Temboo client
    GetWeatherByAddressChoreo.begin();

    // add your temboo account info
    GetWeatherByAddressChoreo.setAccountName(TEMBOO_ACCOUNT);
    GetWeatherByAddressChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    GetWeatherByAddressChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // set the name of the choreo we want to run
    GetWeatherByAddressChoreo.setChoreo("/Library/Yahoo/Weather/GetWeatherByAddress");
    
    // set choreo inputs; in this case, the address for which to retrieve weather data
    // the Temboo client provides standardized calls to 100+ cloud APIs
    GetWeatherByAddressChoreo.addInput("Address", ADDRESS_FOR_FORECAST);

    // add an output filter to extract the name of the city.
    GetWeatherByAddressChoreo.addOutputFilter("city", "/rss/channel/yweather:location/@city", "Response");
    
    // add an output filter to extract the current temperature
    GetWeatherByAddressChoreo.addOutputFilter("temperature", "/rss/channel/item/yweather:condition/@temp", "Response");

    // add an output filter to extract the date and time of the last report.
    GetWeatherByAddressChoreo.addOutputFilter("date", "/rss/channel/item/yweather:condition/@date", "Response");

    // run the choreo 
    GetWeatherByAddressChoreo.run();
        
    // parse the results and print them to the serial monitor
    while(GetWeatherByAddressChoreo.available()) {
        // read the name of the next output item
        String name = GetWeatherByAddressChoreo.readStringUntil('\x1F');
        name.trim(); // use “trim” to get rid of newlines

        // read the value of the next output item
        String data = GetWeatherByAddressChoreo.readStringUntil('\x1E');
        data.trim(); // use “trim” to get rid of newlines

        while (name == "temperature") {
        int temp = data.toInt(); // THE GOOD STUFF (variable temp = temperature!!)
        Serial.println(temp); // Print the Temperature
        
        
        
        break;
        }
            
        
    }
    
    Serial.println("");
    Serial.println("Waiting...");
    Serial.println("");
    delay(10000); // wait 10 seconds between GetWeatherByAddress calls
  }
}

