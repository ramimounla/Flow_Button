#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

  
/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

const int buttonPin3 = D3;     // the number of the pushbutton pin

int buttonState = 0;
int loopCounter = 0;

/**** TODO FILL THE FOLLOWING  ****/
String WirelessSSID = "?"; 
String WirelessKey = "?";
String FlowUrl = "?";
/**** TODO END  ****/

void setup() {
  Serial.begin(9600);                 //Serial connection
  Serial.println("\nStart setup" );
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  doubleBlink();
  pinMode(buttonPin3, INPUT);

  tryToConnect();
  Serial.println("Done setup" );
  quadBlink();  
}

// the loop function runs over and over again forever
void loop() {
  buttonState = digitalRead(buttonPin3);

  if (buttonState == LOW) {
    Serial.println("Going LOW and calling Flow" );
    digitalWrite(LED_BUILTIN, LOW); //Turn onboard LED on while connecting
    callFlowButton();
    doubleBlink();
  }
  else {
    digitalWrite(LED_BUILTIN, HIGH);
  }

  loopCounter++;
  if (loopCounter == 40) {
    singleBlink();
    loopCounter = 0;
    Serial.println("Waiting for input..." );
  }
  delay(100);
}

void callFlowButton() {
  if (WiFi.status() != WL_CONNECTED) { //Check WiFi connection status
    Serial.println("Error in WiFi connection");
    tryToConnect();
  }

  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  
  HTTPClient http;    //Declare object of class HTTPClient
  http.begin(*client, FlowUrl);     //Specify request destination
              
  Serial.println("Sending request");
  int httpCode = http.GET();

  Serial.print("Resutl is: ");
  Serial.println(httpCode);

  http.end();  //Close connection
}


void tryToConnect() {
  WiFi.begin(WirelessSSID, WirelessKey);   //WiFi connection

  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void singleBlink(){
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(100);                      // Wait for two seconds (to demonstrate the active low LED)
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(100);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
}
void quadBlink(){
  doubleBlink();
  doubleBlink();
}

void doubleBlink() {
  singleBlink();
  singleBlink();
}