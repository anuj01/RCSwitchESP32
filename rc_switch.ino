/*
  A simple RCSwitch/Ethernet/Webserver demo
  
  https://github.com/sui77/rc-switch/
*/

#include <RCSwitch.h>

// Load Wi-Fi library
#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

// Set web server port number to 80
WiFiServer server(80);                        // Server Port 80

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String outputAstate = "off";
String outputBstate = "off";
String outputCstate = "off";
String outputDstate = "off";

// Assign output variables to GPIO pins
//const int outputA = 26;
//const int outputB = 27;
//const int outputC = 28;
//const int outputD = 29;


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

// RCSwitch configuration
RCSwitch mySwitch = RCSwitch();
int RCTransmissionPin = 26;


/**
 * Setup
 */
void setup() {

  Serial.begin(115200);

  // Initialize the output variables as outputs
  //pinMode(outputA, OUTPUT);
  //pinMode(outputB, OUTPUT);
  // Set outputs to LOW
  //digitalWrite(outputA, LOW);
  //digitalWrite(outputB, LOW);
  
   // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  
  mySwitch.enableTransmit( RCTransmissionPin );
}

/**
 * Loop
 */
void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /A/on") >= 0) {
              Serial.println("Switch A on");
              outputAstate = "on";
              //digitalWrite(outputA, HIGH);
              mySwitch.send(96002 , 24);
              delay(500);  
            } else if (header.indexOf("GET /A/off") >= 0) {
              Serial.println("Switch A off");
              outputAstate = "off";
              //digitalWrite(outputA, LOW);
              mySwitch.send(96002 , 24);
              delay(500); 
            } else if (header.indexOf("GET /B/on") >= 0) {
              Serial.println("Switch B on");
              outputBstate = "on";
              //digitalWrite(outputB, HIGH);
              mySwitch.send(96008 , 24);
              delay(500); 
            } else if (header.indexOf("GET /B/off") >= 0) {
              Serial.println("Switch B off");
              outputBstate = "off";
              //digitalWrite(outputB, LOW);
              mySwitch.send(96008 , 24);
              delay(500); 
            }else if (header.indexOf("GET /C/on") >= 0) {
              Serial.println("Switch C on");
              outputCstate = "on";
              //digitalWrite(outputC, HIGH);
              mySwitch.send(96001 , 24);
              delay(500); 
            } else if (header.indexOf("GET /C/off") >= 0) {
              Serial.println("Switch C off");
              outputCstate = "off";
              //digitalWrite(outputC, LOW);
              mySwitch.send(96001 , 24);
              delay(500); 
            }else if (header.indexOf("GET /D/on") >= 0) {
              Serial.println("Switch D on");
              outputDstate = "on";
              //digitalWrite(outputD, HIGH);
              mySwitch.send(96004 , 24);
              delay(500); 
            } else if (header.indexOf("GET /D/off") >= 0) {
              Serial.println("Switch D off");
              outputDstate = "off";
              //digitalWrite(outputD, LOW);
              mySwitch.send(96004 , 24);
              delay(500); 
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>RC Switch Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for Switch A  
            client.println("<p>Switch A - State " + outputAstate + "</p>");
            // If the outputAstate is off, it displays the ON button       
            if (outputAstate=="off") {
              client.println("<p><a href=\"/A/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/A/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for Switch B  
            client.println("<p>Switch B - State " + outputBstate + "</p>");
            // If the outputBstate is off, it displays the ON button       
            if (outputBstate=="off") {
              client.println("<p><a href=\"/B/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/B/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for Switch B  
            client.println("<p>Switch C - State " + outputCstate + "</p>");
            // If the outputCstate is off, it displays the ON button       
            if (outputCstate=="off") {
              client.println("<p><a href=\"/C/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/C/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for Switch B  
            client.println("<p>Switch D - State " + outputDstate + "</p>");
            // If the outputDstate is off, it displays the ON button       
            if (outputDstate=="off") {
              client.println("<p><a href=\"/D/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/D/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
