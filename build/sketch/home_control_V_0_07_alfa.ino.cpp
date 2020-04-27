#include <Arduino.h>
#line 1 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
/*
  Web Server

  A simple web server that shows the value of the analog input pins.
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13
   Analog inputs attached to pins A0 through A5 (optional)

  created 18 Dec 2009
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe
  modified 02 Sept 2015
  by Arturo Guadalupi

*/

//--------------Grzegorz Mroczkowski ---------------------//

//Functionality:
// B. Automatic Open-Up, Close and Stop blinds using TimeLord and switch auto-on and auto-0ff.
// A. Manual Open-Up, Close and Stop blinds
//TODO:
// 11. Implement light sensor for automatic blins up and blinds down.
// 10. Implement watering automatic system with triggers
// 9. Implement watering manual system
// 8. Implement SD card logging
//Working on:
// 7. Implement heating triggers and timers
//Done:
// 6. Implement Dallas temp. sensors
// 5. Implement automatic blins based on TimeLord, Timer and blinds triggers
// 4. Implement TimeLord library with Sunrise and Sunset and automatic weekDay
// 3. Implement RTC with library and setup clock by HTML
// 2. Implement manual blinds, universal Timer
// 1. Merge code Ethernet Serverv with https://raw.githubusercontent.com/RuiSantosdotme/Random-Nerd-Tutorials/master/Projects/ESP8266_Web_Server_Arduino_IDE_updated.ino
// 0. Prepare PIN reservation for external devices

#include <SPI.h>
#include <Ethernet.h>
#include <TimeLord.h>
#include <OneWire.h>
#include <DS18B20.h>
#include <C:\Users\Grzes\Documents\Arduino\libraries\Timer.h>
#include <C:\Users\Grzes\Documents\Arduino\libraries\Timer.cpp>
#include <C:\Users\Grzes\Documents\Arduino\libraries\RTC.h>
#include <C:\Users\Grzes\Documents\Arduino\libraries\RTC.cpp>

#define pin_DL 0 //droping line
#define pin_Z1 1 //Z1 watering circut
#define pin_Z2 2 //Z2 watering circut
#define pin_BS 3 //Blinds stop
#define pin_BU 4 //Blinds up
#define pin_BD 5 //Blinss down
#define pin_TS 6 //Dallas temperature sensors
#define pin_HU 7 //Heating upstairs
#define pin_HD 8 //Heating downstairs
#define pin_RS 9 //Rain sensor

boolean lockTrigger[5]; //Locking trigger to start it only onec.

// Dallas temperature sensors address:
byte address1[8] = {0x28, 0xDA, 0x89, 0x79, 0x97, 0x11, 0x3, 0x8E};
byte address2[8] = {0x28, 0x8D, 0x47, 0x79, 0x97, 0x11, 0x3, 0xA5};
byte address3[8] = {0x28, 0x2B, 0x4E, 0x79, 0x97, 0x10, 0x3, 0x7C};

float temperature1;
float temperature2;
float temperature3;

//------------------From Random Nerd Tutorials:
String header;

//------------------For TimeLord
byte today[] = {0, 0, 12, 31, 8, 19}; // store today's date (at noon) in an array for TimeLord to use

float const LONGITUDE = 20.794147; //For TimeLord
float const LATITUDE = 52.216021;

Timer odliczanie;          //initialize software timer
RTC zegar;                 //initialize software clock
TimeLord tardis;           //initialize sunState :)
OneWire onewire(pin_TS);   //Dallas temperature sensors
DS18B20 sensors(&onewire); //Dallas temperature sensors

#include <C:\Users\Grzes\Documents\Arduino\libraries\sunState.cpp>

sunState slonce;

// Auxiliar variables to store the current output state
boolean roletyAuto = true;

// Heating system variables:
boolean ogrzewaniePietroDzien = true;

boolean ogrzewanieParterDzien = true;

float histereza = 0.2;

byte temperaturaDzien = 22;
byte temperaturaNoc = 18;

byte temperaturaPietro = temperaturaDzien;
byte temperaturaParter = temperaturaDzien;

byte ogrzewanieParterWeekendStartHour = 7;
byte ogrzewanieParterWeekendStartMinute = 0;
byte ogrzewanieParterWeekendStopHour = 22;
byte ogrzewanieParterWeekendStopMinute = 0;

byte ogrzewanieParterRanoStartHour = 4;
byte ogrzewanieParterRanoStartMinute = 30;
byte ogrzewanieParterRanoStopHour = 7;
byte ogrzewanieParterRanoStopMinute = 0;

byte ogrzewanieParterPopoludnieStartHour = 14;
byte ogrzewanieParterPopoludnieStartMinute = 0;
byte ogrzewanieParterPopoludnieStopHour = 21;
byte ogrzewanieParterPopoludnieStopMinute = 0;

byte ogrzewaniePietroWeekendStartHour = 7;
byte ogrzewaniePietroWeekendStartMinute = 0;
byte ogrzewaniePietroWeekendStopHour = 22;
byte ogrzewaniePietroWeekendStopMinute = 0;

byte ogrzewaniePietroRanoStartHour = 4;
byte ogrzewaniePietroRanoStartMinute = 30;
byte ogrzewaniePietroRanoStopHour = 7;
byte ogrzewaniePietroRanoStopMinute = 0;

byte ogrzewaniePietroPopoludnieStartHour = 14;
byte ogrzewaniePietroPopoludnieStartMinute = 0;
byte ogrzewaniePietroPopoludnieStopHour = 21;
byte ogrzewaniePietroPopoludnieStopMinute = 0;

// Watering system variables:
boolean podlewanieAuto = true;
byte podlewanieCykl = 0;
int podlewanieDL = 1200;  //Dropping line 20min (1200s)
int podlewanieZ1 = 600;   //Z1 10min (600s)
int podlewanieZ2 = 600;   //Z2 10min (600s)
byte wateringHour = 20;   //Hour of starting watering
byte wateringMinute = 15; //Minute of stariting watering

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

//----------------From Ethernet server example:
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(172, 26, 160, 17);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

boolean trigger(byte);

#line 168 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
void setup();
#line 243 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
void loop();
#line 789 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
boolean trigger(int number);
#line 168 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
void setup()
{

    //-----------------From random nerd tutorials:
    // Initialize the output variables as outputs
    pinMode(pin_DL, OUTPUT);
    pinMode(pin_Z1, OUTPUT);
    pinMode(pin_Z2, OUTPUT);
    pinMode(pin_BU, OUTPUT);
    pinMode(pin_BD, OUTPUT);
    pinMode(pin_BS, OUTPUT);
    pinMode(pin_HU, OUTPUT);
    pinMode(pin_HD, OUTPUT);
    // Set outputs to HIGH
    digitalWrite(pin_DL, HIGH);
    digitalWrite(pin_Z1, HIGH);
    digitalWrite(pin_Z2, HIGH);
    digitalWrite(pin_BU, HIGH);
    digitalWrite(pin_BD, HIGH);
    digitalWrite(pin_BS, HIGH);
    digitalWrite(pin_HU, HIGH);
    digitalWrite(pin_HD, HIGH);

    for (int i = 0; i < 5; i++)
        lockTrigger[i] = false;

    //Dallas temperature sensors:
    sensors.begin(12); //resolution 12
    sensors.request(); //request all the sensors
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    tardis.TimeZone(1 * 60); // tell TimeLord what timezone your RTC is synchronized to. You can ignore DST
    // as long as the RTC never changes back and forth between DST and non-DST
    tardis.Position(LATITUDE, LONGITUDE); // tell TimeLord where in the world we are
    if (tardis.DstRules(3, 4, 10, 4, 60)) // Daylight saving time zone
    {
        //  Serial.println("To ma sens! :)");
    }
    else
    {
        //  Serial.println("To nie ma sensu! :(");
    }

    //Serial.println("Ethernet WebServer Example");

    //start the Ethernet connection and the server :
    Ethernet.begin(mac, ip);

    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        //Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
        while (true)
        {
            delay(1); // do nothing, no point running without Ethernet hardware
        }
    }
    if (Ethernet.linkStatus() == LinkOFF)
    {
        //Serial.println("Ethernet cable is not connected.");
    }

    // start the server
    server.begin();
    //Serial.print("server is at ");
    //Serial.println(Ethernet.localIP());
    odliczanie.startTimer(20, 1);
    slonce.checkSun(zegar.getSecond(), zegar.getMinute(), zegar.getHour(), zegar.getMonthDay(), zegar.getMonth(), (byte)(zegar.getYear() - 2000), 1);
}

void loop()
{
    // listen for incoming clients
    //----------------From ethernet example:
    EthernetClient client = server.available();
    //----------------From RNT:
    if (client)
    { // If a new client connects,
        //Serial.println("New Client.");          // print a message out in the serial port
        String currentLine = ""; // make a String to hold incoming data from the client
        currentTime = millis();
        previousTime = currentTime;
        while (client.connected() && currentTime - previousTime <= timeoutTime)
        { // loop while the client's connected
            currentTime = millis();
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                //Serial.write(c);                    // print it out the serial monitor
                header += c;
                if (c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
                        client.println();

                        if (header.indexOf("GET /14/off") >= 0) //Force Z2 to stop waterring
                        {
                            odliczanie.resetTimer(14); //Just a reset Z2 timer
                        };

                        if (header.indexOf("GET /14/on") >= 0)
                        {
                            odliczanie.startTimer((unsigned long int)(podlewanieZ2)*1000, 14); //Start Z2 timer
                        };
                        if (header.indexOf("GET /13/off") >= 0) //Force Z2 to start waterring
                        {
                            odliczanie.resetTimer(13); //Just a reset Z1 timer
                        };

                        if (header.indexOf("GET /13/on") >= 0) //Force Z1 to start waterring
                        {
                            odliczanie.startTimer((unsigned long int)(podlewanieZ1)*1000, 13); //Start Z1 timer
                        };
                        if (header.indexOf("GET /12/off") >= 0) //Force dropping line to stop waterring
                        {
                            odliczanie.resetTimer(12); //Just a reset dropping line timer
                        };

                        if (header.indexOf("GET /12/on") >= 0) //Forse dropping line to start
                        {
                            odliczanie.startTimer((unsigned long int)(podlewanieDL)*1000, 12); //Start dropping line timer
                        };

                        if (header.indexOf("GET /11/on") >= 0) //Force cycle of watering on
                        {
                            podlewanieCykl = 1; //Set cycle of watering to on
                        };

                        if (header.indexOf("GET /11/off") >= 0) //Force cycle of watering off
                        {
                            podlewanieCykl = 0; //Set cycle of watering to off
                        };

                        if (header.indexOf("GET /10/on") >= 0) //Force autoatering on
                        {
                            podlewanieAuto = true; //Set autowatering on
                        };

                        if (header.indexOf("GET /10/off") >= 0) //Force autoatering off
                        {
                            podlewanieAuto = false; //Set autowatering off
                        };

                        if (header.indexOf("GET /9/off") >= 0) //Force downstairs heating to stop
                        {
                            odliczanie.resetTimer(9); //Just a reset timer
                        };

                        if (header.indexOf("GET /9/on") >= 0) //Forse downstairs heating start for 10 minutes (or sth :)
                        {
                            odliczanie.startTimer(600000, 9); //Add to the downstairs heating timer 10000ms
                        };

                        if (header.indexOf("GET /8/off") >= 0) //Heating downstairs in "Night" mode - lower temp.
                        {
                            ogrzewanieParterDzien = false;      //Heating downstairs in "Night" mode - lower temp.
                            temperaturaParter = temperaturaNoc; //Setting up night temperature upstairs
                        };

                        if (header.indexOf("GET /8/on") >= 0) //Heating downstairs in "Day" mode - higher temp.
                        {
                            ogrzewanieParterDzien = true;         //Heating downstairs in "Day" mode - higher temp.
                            temperaturaParter = temperaturaDzien; //Setting up day temperature downstairs
                        };

                        if (header.indexOf("GET /7/off") >= 0) //Force upstairs heating to stop
                        {
                            odliczanie.resetTimer(7); //Just a reset timer
                        };

                        if (header.indexOf("GET /7/on") >= 0) //Forse upstairs heating start for 10 minutes (or sth :)
                        {
                            odliczanie.startTimer(600000, 7); //Add to the upstairs heating timer 10000ms
                        };

                        if (header.indexOf("GET /6/off") >= 0) //Heating upstairs in "Night" mode - lower temp.
                        {
                            ogrzewaniePietroDzien = false;      //Heating upstairs in "Night" mode - lower temp.
                            temperaturaPietro = temperaturaNoc; //Setting up night temperature upstairs
                        };

                        if (header.indexOf("GET /6/on") >= 0) //Heating upstairs in "Day" mode - higher temp.
                        {
                            ogrzewaniePietroDzien = true;         //Heating upstairs in "Day" mode - higher temp.
                            temperaturaPietro = temperaturaDzien; //Setting up day temperature upstairs
                        };

                        if (header.indexOf("GET /5/on") >= 0) //Blins up
                        {
                            //Serial.println("Rolety w gore");
                            odliczanie.startTimer(1500, 5); //Only 700ms
                        };
                        /*if (header.indexOf("GET /5/off") >= 0)
                        // {
                            //Serial.println("Rolety w gore zatrzymane");
                            podniesRolety = "off";
                            digitalWrite(pin_BU, HIGH);
                        };*/
                        if (header.indexOf("GET /4/on") >= 0) //Blinds down
                        {
                            odliczanie.startTimer(1500, 4); //Only 700ms
                        };
                        //if (header.indexOf("GET /4/off") >= 0)
                        //{
                        //    opuscRolety = "off";
                        //   digitalWrite(pin_BD, HIGH);
                        //};
                        if (header.indexOf("GET /3/on") >= 0) //Blinds stop
                        {
                            odliczanie.startTimer(1500, 3); //Only 700ms
                        };
                        /*if (header.indexOf("GET /3/off") >= 0)
                        {
                            //Serial.println("Rolety stop zatrzymane");
                            zatrzymajRolety = "off";
                            digitalWrite(pin_BS, HIGH);
                        };*/
                        if (header.indexOf("GET /2/on") >= 0) //Blins auto up and down by SunRise and SunSet
                        {
                            //Serial.println("Auto");
                            roletyAuto = true;
                            //digitalWrite(pin_BS, LOW);
                        };
                        if (header.indexOf("GET /2/off") >= 0) //Blins auto off
                        {
                            //Serial.println("Manual");
                            roletyAuto = false;
                            //digitalWrite(pin_BS, HIGH);
                        };

                        if (header.indexOf("GET /zegar/") >= 0)
                        { //Setup clock, clock format: \  

                            zegar.setYear((int)(header.substring(11, 15).toInt()));
                            //Serial.println("Zegar: " + header.substring(11, 15).toInt());
                            zegar.setMonth((byte)(header.substring(16, 18).toInt()));
                            zegar.setMonthDay((byte)(header.substring(19, 21).toInt()));
                            zegar.setHour((byte)(header.substring(22, 24).toInt()));
                            zegar.setMinute((byte)(header.substring(25, 27).toInt()));
                            today[0] = 0;
                            today[1] = zegar.getMinute();
                            today[2] = zegar.getHour();
                            today[3] = zegar.getMonthDay();
                            today[4] = zegar.getMonth();
                            today[5] = (byte)zegar.getYear() - 2000;
                            if (tardis.DayOfWeek(today) == 1) //When TimeLord shows 1 (Sunday)...
                            {
                                zegar.setWeekDay(7); //... we have to set 7 (our Sunday :)
                            }
                            else
                            {
                                zegar.setWeekDay(tardis.DayOfWeek(today) - 1); //If not, just decrement day of the week.
                            }
                            //Serial.println("Zegar: " + zegar.getDate());
                            tardis.TimeZone(1 * 60); // tell TimeLord what timezone your RTC is synchronized to. You can ignore DST
                            // as long as the RTC never changes back and forth between DST and non-DST
                            tardis.Position(LATITUDE, LONGITUDE); // tell TimeLord where in the world we are
                            tardis.DstRules(3, 4, 10, 4, 60);
                            //Set up sunrise and sunset triggers
                            slonce.checkSun(zegar.getSecond(), zegar.getMinute(), zegar.getHour(), zegar.getMonthDay(), zegar.getMonth(), (byte)(zegar.getYear() - 2000), 1);
                        };

                        // Display the HTML web page
                        client.println(F("<!DOCTYPE html><html>"));
                        client.println(F("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"));
                        client.println(F("<link rel=\"icon\" href=\"data:,\">"));
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println(F("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"));
                        client.println(F(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;"));
                        client.println(F("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}"));
                        client.println(F(".button2 {background-color: #77878A;}</style></head>"));
                        // Web Page Heading
                        client.println(F("<body><H1>Home Control Domek Ozarow</H1>"));
                        client.println(F("<H3>Rolety</H3>"));
                        client.println(F("<h5> Dzis jest: "));
                        client.println(zegar.getDate());
                        client.println(F(" "));
                        client.println(zegar.getWeekDay());
                        client.println(F(" godzina: "));
                        client.println(zegar.getTime());
                        client.println(F(" </h5>"));
                        client.println(F(" <h5> Wschod slonca: "));
                        client.println((String)slonce.sunRiseHour());
                        client.println(F(":"));
                        client.println((String)slonce.sunRiseMinute());
                        client.println(F("</h5><h5> Zachod slonca: "));
                        client.println((String)slonce.sunSetHour());
                        client.println(F(":"));
                        client.println((String)slonce.sunSetMinute());
                        client.println(F(" </h5>"));

                        // Display current state, and ON/OFF buttons for GPIO 5
                        //client.println("<p>GPIO 5 - State " + output5State + "</p>");
                        // If the output5State is off, it displays the ON button

                        if (!roletyAuto)
                        {
                            client.println(F("<p><a href=\"/2/on\"><button class=\"button\">Rolety recznie</button></a>"));
                        }
                        else
                        {
                            client.println(F("<p><a href=\"/2/off\"><button class=\"button button2\">Rolety auto</button></a>"));
                        };

                        if (!odliczanie.checkTimer(5)) //Blins up button
                        {
                            client.println(F("<a href=\"/5/on\"><button class=\"button\">Podnies rolety</button></a>"));
                        }
                        else
                        {
                            client.println(F("<a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a>"));
                        };

                        // Display current state, and ON/OFF buttons for GPIO 4
                        //client.println("<p>GPIO 4 - State " + output4State + "</p>");
                        // If the output4State is off, it displays the ON button
                        if (!odliczanie.checkTimer(4)) //Blins down button
                        {
                            client.println(F("<a href=\"/4/on\"><button class=\"button\">Opusc rolety</button></a></p>"));
                        }
                        else
                        {
                            client.println(F("<a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a></p>"));
                        };
                        ;

                        //client.println(F("</body></html>"));

                        //if (!odliczanie.checkTimer(3)) //Blins stop button
                        //{
                        //}

                        client.println(F("<H3>Ogrzewanie</H3>"));
                        client.println(F("<h5>Temperatura zewnetrzna: <B>"));
                        client.println((String)temperature1);
                        client.println(F(" C</B></H5>"));
                        //<H5>Temperatura na parterze: "));
                        //client.println((String)temperature2);
                        //client.println(F(" C </H5><H5>Temperatura na pietrze: "));
                        //client.println((String)temperature3);
                        //client.println(F(" C </H5>"));
                        //-----------------Ogrzewanie pietro
                        if (ogrzewaniePietroDzien) //Upstairs day mode button
                        {
                            client.println(F("<p><a href=\"/6/off\"><button class=\"button\">Pietro "));
                            client.println((String)temperature3);
                            client.println(F("'C dzien</button></a>"));
                        }
                        else //Upstairs night mode button
                        {
                            client.println(F("<p><a href=\"/6/on\"><button class=\"button button2\">Pietro "));
                            client.println((String)temperature3);
                            client.println(F("'C noc</button></a>"));
                        }

                        if (!odliczanie.checkTimer(7)) //Force heat upstairs button
                        {
                            client.println(F("<a href=\"/7/on\"><button class=\"button\">10 min. On</button></a></p>"));
                        }
                        else
                        {
                            client.println(F("<a href=\"/7/on\"><button class=\"button button2\">"));
                            client.println(odliczanie.getTime(7));
                            client.println(F("</button></a>"));
                            client.println(F("<a href=\"/7/off\"><button class=\"button\">STOP!</button></a></p>"));
                        };
                        //----------------Ogrzewanie Parter
                        if (ogrzewanieParterDzien) //Downstairs day mode button
                        {
                            client.println(F("<p><a href=\"/8/off\"><button class=\"button\">Parter "));
                            client.println((String)temperature2);
                            client.println(F("'C dzien</button></a>"));
                        }
                        else //Downstairs night mode button
                        {
                            client.println(F("<p><a href=\"/8/on\"><button class=\"button button2\">Parter "));
                            client.println((String)temperature2);
                            client.println(F("'C noc</button></a>"));
                        }

                        if (!odliczanie.checkTimer(9))
                        {
                            client.println(F("<a href=\"/9/on\"><button class=\"button\">10 min. On</button></a></p>"));
                        }
                        else
                        {
                            client.println(F("<a href=\"/9/on\"><button class=\"button button2\">"));
                            client.println(odliczanie.getTime(9));
                            client.println(F("</button></a>"));
                            client.println(F("<a href=\"/9/off\"><button class=\"button\">STOP!</button></a></p>"));
                        }

                        client.println(F("<h5>Temperatura dzienna: "));
                        client.println((String)temperaturaDzien);
                        client.println(F(" C</H5><H5>Temperatura nocna: "));
                        client.println((String)temperaturaNoc);
                        client.println(F(" C</H5>"));

                        //--------------Podlewanie:
                        client.println(F("<H3>Podlewanie</H3>"));
                        client.println(F("<h5>Czas podlewania LK: "));
                        client.println((String)podlewanieDL);
                        client.println(F("</H5><h5>Czas podlewania Z1: "));
                        client.println((String)podlewanieZ1);
                        client.println(F("</H5><h5>Czas podlewania Z2: "));
                        client.println((String)podlewanieZ2);
                        client.println(F("</H5>"));
                        if (podlewanieAuto)
                        {
                            client.println(F("<p><a href=\"/10/off\"><button class=\"button button2\">Podlewanie Auto</button></a>"));
                        }
                        else
                        {
                            client.println(F("<p><a href=\"/10/on\"><button class=\"button \">Podlewanie Reczne</button></a>"));
                        }

                        if (podlewanieCykl == 0)
                        {
                            client.println(F("<a href=\"/11/on\"><button class=\"button\">Start cyklu podlewania</button></a>"));
                        }
                        else
                        {
                            client.println(F("<a href=\"/11/off\"><button class=\"button button2\">Cykl podlewanie w trakcie, nadzisnij by zatrzymac</button></a>"));
                        }

                        if (!odliczanie.checkTimer(12))
                        {
                            client.println(F("<a href=\"/12/on\"><button class=\"button\">Start LK</button></a>"));
                        }
                        else
                        {
                            client.println(F("<a href=\"/12/off\"><button class=\"button button2\">Stop LK za "));
                            client.println(odliczanie.getTime(12));
                            client.println(F("</button></a>"));
                        };

                        if (!odliczanie.checkTimer(13))
                        {
                            client.println(F("<a href=\"/13/on\"><button class=\"button\">Start Z1</button></a>"));
                        }
                        else
                        {
                            client.println(F("<a href=\"/13/off\"><button class=\"button button2\">Stop Z1 za "));
                            client.println(odliczanie.getTime(13));
                            client.println(F("</button></a>"));
                        };

                        if (!odliczanie.checkTimer(14))
                        {
                            client.println(F("<a href=\"/14/on\"><button class=\"button\">Start Z2</button></a></p>"));
                        }
                        else
                        {
                            client.println(F("<a href=\"/14/off\"><button class=\"button button2\">Stop Z2 za "));
                            client.println(odliczanie.getTime(14));
                            client.println(F("</button></a></p>"));
                        };
                        client.println(F("<a href=\"/nothing\"><button class=\"button button2\">Refresh </button></a></p>"));
                        //-------------Koniec
                        client.println();
                        client.println(F("</body></html>")); //Nie wiem czemu, ale musialem zakomentowac, bo nie wyswietlal godziny :(
                        // The HTTP response ends with another blank line
                        client.println();
                        // Break out of the while loop
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        //Serial.println("Client disconnected.");
        //Serial.println("");
    }
    //--------------------------Blok wyłączenia dla przełączników monostabilnych

    if (odliczanie.checkTimer(14)) //Turn on watering by Z2
    {
        digitalWrite(pin_Z2, LOW);
    }
    else
    {
        digitalWrite(pin_Z2, HIGH);
    };

    if (odliczanie.checkTimer(13)) //Turn on watering by Z1
    {
        digitalWrite(pin_Z1, LOW);
    }
    else
    {
        digitalWrite(pin_Z1, HIGH);
    };

    if (odliczanie.checkTimer(12)) //Turn on watering by LK
    {
        digitalWrite(pin_DL, LOW);
    }
    else
    {
        digitalWrite(pin_DL, HIGH);
    };

    if (odliczanie.checkTimer(9)) //Turn on force to heat downstairs
    {
        digitalWrite(pin_HD, LOW);
    }
    else
    {
        digitalWrite(pin_HD, HIGH);
    };

    if (odliczanie.checkTimer(7)) //Turn on force to heat upstairs
    {
        digitalWrite(pin_HU, LOW);
    }
    else
    {
        digitalWrite(pin_HU, HIGH);
    };

    if (odliczanie.checkTimer(5)) //Blins up
    {
        digitalWrite(pin_BU, LOW);
    }
    else
    {
        digitalWrite(pin_BU, HIGH);
    };

    if (odliczanie.checkTimer(4)) //Blinds down
    {
        digitalWrite(pin_BD, LOW);
    }
    else
    {
        digitalWrite(pin_BD, HIGH);
    };

    if (odliczanie.checkTimer(3)) //Blinds stop
    {
        digitalWrite(pin_BS, LOW);
    }
    else
    {
        digitalWrite(pin_BS, HIGH);
    };

    if (podlewanieCykl == 1) //Starting watering cycle
    {
        odliczanie.startTimer((unsigned long int)podlewanieDL * 1000, 12);
        podlewanieCykl = 2;
    };
    if ((!odliczanie.checkTimer(12)) && (podlewanieCykl == 2))
    {
        odliczanie.startTimer((unsigned long int)podlewanieZ1 * 1000, 13);
        podlewanieCykl = 3;
    };
    if ((!odliczanie.checkTimer(13)) && (podlewanieCykl == 3))
    {
        odliczanie.startTimer((unsigned long int)podlewanieZ2 * 1000, 14);
        podlewanieCykl = 0;
    }

    //----------------------------------------//
    //              Trigger BLOCK             //
    //----------------------------------------//

    if ((trigger(1)) && (roletyAuto)) //Auto blins up if it is on
    {
        //Serial.println("Rolety w gore by trigger");
        odliczanie.startTimer(1500, 5); //Only 100ms
    };
    if ((trigger(2)) && (roletyAuto)) //Auto blins up if it is on
    {
        odliczanie.startTimer(1500, 4); //Only 100ms
    };

    if (trigger(3) && (podlewanieAuto)) //Auto watering is inside the trigger
    {
        //Serial.println("Starutjemy cykl podlewania");
        podlewanieCykl = 1;
    };

    //----------------------------------------//
    //              MAIN BLOCK                //
    //----------------------------------------//

    zegar.Flow();                                                                                                                                     //Time is flowing
    slonce.checkSun(zegar.getSecond(), zegar.getMinute(), zegar.getHour(), zegar.getMonthDay(), zegar.getMonth(), (byte)(zegar.getYear() - 2000), 0); //check, if we have a new day and need a new sunrise and sunset :)
    sensors.request();                                                                                                                                //request all the sensors
    temperature1 = sensors.readTemperature(address1);
    temperature2 = sensors.readTemperature(address2);
    temperature3 = sensors.readTemperature(address3);
}

boolean trigger(int number)
{
    //Serial.println("Sprawdzam trigger");
    switch (number)
    {
    case 1:
        if ((zegar.getHour() == slonce.sunRiseHour()) && (zegar.getMinute() == slonce.sunRiseMinute()) && (zegar.getSecond() == 0) && (!lockTrigger[number])) //If we have Sun Rise :)
        {
            //Serial.println("Trigger: jest wschód!");
            lockTrigger[number] = true; //Locking trigger to start one time.
            return true;                //Return 1.
        }
        else
        {
            if (zegar.getSecond() != 0)
                lockTrigger[number] = false; //Unocking trigger becouse second not zero - could be start again
            return false;
        }
        break;
    case 2:
        //Serial.println("Before if hour:" + (String)zegar.getHour() + " Sun set hour:" + (String)slonce.sunSetHour() + " minute:" + (String)zegar.getMinute() + " Sun set minute" + (String)slonce.sunSetMinute() + " second: " + (String)zegar.getSecond() + " lock: " + (String)(!lockTrigger[number]));
        if ((zegar.getHour() == slonce.sunSetHour()) && (zegar.getMinute() == slonce.sunSetMinute()) && (zegar.getSecond() == 0) && (!lockTrigger[number])) //If we have Sun Rise :)
        {
            //Serial.println("Trigger: jest zachód!");
            lockTrigger[number] = true; //Locking trigger to start one time.
            return true;
        }
        else
        {
            if (zegar.getSecond() != 0)
                lockTrigger[number] = false; //Unocking trigger becouse second not zero - could be start again
            return false;
        }
        break;
    case 3:
        if ((zegar.getHour() == wateringHour) && (zegar.getMinute() == wateringMinute) && (zegar.getSecond() == 0) && (!lockTrigger[number])) //If we have Sun Rise :)
        {
            //Serial.println("Trigger: start cyklu podlewania");
            lockTrigger[number] = true;
            return true; //Return 1.
        }
        else
        {
            if (zegar.getSecond() != 0)
                lockTrigger[number] = false; //Unocking trigger becouse second not zero - could be start again
            return false;
        }
        break;
    }
}

