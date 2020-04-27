# 1 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
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
# 20 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
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

# 42 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2
# 43 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2
# 44 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2
# 45 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2
# 46 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2
# 47 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2
# 48 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2
# 49 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2
# 50 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2
# 62 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
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

Timer odliczanie; //initialize software timer
RTC zegar; //initialize software clock
TimeLord tardis; //initialize sunState :)
OneWire onewire(6 /*Dallas temperature sensors*/); //Dallas temperature sensors
DS18B20 sensors(&onewire); //Dallas temperature sensors

# 89 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 2

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
int podlewanieDL = 1200; //Dropping line 20min (1200s)
int podlewanieZ1 = 600; //Z1 10min (600s)
int podlewanieZ2 = 600; //Z2 10min (600s)
byte wateringHour = 20; //Hour of starting watering
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

void setup()
{

    //-----------------From random nerd tutorials:
    // Initialize the output variables as outputs
    pinMode(0 /*droping line*/, 0x1);
    pinMode(1 /*Z1 watering circut*/, 0x1);
    pinMode(2 /*Z2 watering circut*/, 0x1);
    pinMode(4 /*Blinds up*/, 0x1);
    pinMode(5 /*Blinss down*/, 0x1);
    pinMode(3 /*Blinds stop*/, 0x1);
    pinMode(7 /*Heating upstairs*/, 0x1);
    pinMode(8 /*Heating downstairs*/, 0x1);
    // Set outputs to HIGH
    digitalWrite(0 /*droping line*/, 0x1);
    digitalWrite(1 /*Z1 watering circut*/, 0x1);
    digitalWrite(2 /*Z2 watering circut*/, 0x1);
    digitalWrite(4 /*Blinds up*/, 0x1);
    digitalWrite(5 /*Blinss down*/, 0x1);
    digitalWrite(3 /*Blinds stop*/, 0x1);
    digitalWrite(7 /*Heating upstairs*/, 0x1);
    digitalWrite(8 /*Heating downstairs*/, 0x1);

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
            { // if there's bytes to read from the client,
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
                            ogrzewanieParterDzien = false; //Heating downstairs in "Night" mode - lower temp.
                            temperaturaParter = temperaturaNoc; //Setting up night temperature upstairs
                        };

                        if (header.indexOf("GET /8/on") >= 0) //Heating downstairs in "Day" mode - higher temp.
                        {
                            ogrzewanieParterDzien = true; //Heating downstairs in "Day" mode - higher temp.
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
                            ogrzewaniePietroDzien = false; //Heating upstairs in "Night" mode - lower temp.
                            temperaturaPietro = temperaturaNoc; //Setting up night temperature upstairs
                        };

                        if (header.indexOf("GET /6/on") >= 0) //Heating upstairs in "Day" mode - higher temp.
                        {
                            ogrzewaniePietroDzien = true; //Heating upstairs in "Day" mode - higher temp.
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
# 380 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
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
# 399 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
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
                        { //Setup clock, clock format: 

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
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 445 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 445 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<!DOCTYPE html><html>"
# 445 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 445 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 446 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 446 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
# 446 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 446 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 447 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 447 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<link rel=\"icon\" href=\"data:,\">"
# 447 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 447 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 450 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 450 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
# 450 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 450 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 451 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 451 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;"
# 451 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 451 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 452 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 452 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}"
# 452 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 452 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 453 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 453 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      ".button2 {background-color: #77878A;}</style></head>"
# 453 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 453 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        // Web Page Heading
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 455 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 455 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<body><H1>Home Control Domek Ozarow</H1>"
# 455 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 455 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 456 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 456 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<H3>Rolety</H3>"
# 456 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 456 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 457 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 457 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<h5> Dzis jest: "
# 457 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 457 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getDate());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 459 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 459 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      " "
# 459 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 459 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getWeekDay());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 461 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 461 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      " godzina: "
# 461 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 461 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getTime());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 463 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 463 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      " </h5>"
# 463 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 463 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 464 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 464 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      " <h5> Wschod slonca: "
# 464 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 464 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)slonce.sunRiseHour());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 466 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 466 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      ":"
# 466 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 466 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)slonce.sunRiseMinute());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 468 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 468 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "</h5><h5> Zachod slonca: "
# 468 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 468 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)slonce.sunSetHour());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 470 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 470 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      ":"
# 470 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 470 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)slonce.sunSetMinute());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 472 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 472 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      " </h5>"
# 472 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 472 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));

                        // Display current state, and ON/OFF buttons for GPIO 5
                        //client.println("<p>GPIO 5 - State " + output5State + "</p>");
                        // If the output5State is off, it displays the ON button

                        if (!roletyAuto)
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 480 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 480 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/2/on\"><button class=\"button\">Rolety recznie</button></a>"
# 480 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 480 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 484 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 484 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/2/off\"><button class=\"button button2\">Rolety auto</button></a>"
# 484 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 484 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        };

                        if (!odliczanie.checkTimer(5)) //Blins up button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 489 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 489 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/5/on\"><button class=\"button\">Podnies rolety</button></a>"
# 489 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 489 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 493 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 493 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a>"
# 493 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 493 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        };

                        // Display current state, and ON/OFF buttons for GPIO 4
                        //client.println("<p>GPIO 4 - State " + output4State + "</p>");
                        // If the output4State is off, it displays the ON button
                        if (!odliczanie.checkTimer(4)) //Blins down button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 501 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 501 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/4/on\"><button class=\"button\">Opusc rolety</button></a></p>"
# 501 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 501 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 505 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 505 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a></p>"
# 505 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 505 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        };
                        ;

                        //client.println(F("</body></html>"));

                        //if (!odliczanie.checkTimer(3)) //Blins stop button
                        //{
                        //}

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 515 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 515 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<H3>Ogrzewanie</H3>"
# 515 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 515 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 516 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 516 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<h5>Temperatura zewnetrzna: <B>"
# 516 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 516 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)temperature1);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 518 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 518 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      " C</B></H5>"
# 518 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 518 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        //<H5>Temperatura na parterze: "));
                        //client.println((String)temperature2);
                        //client.println(F(" C </H5><H5>Temperatura na pietrze: "));
                        //client.println((String)temperature3);
                        //client.println(F(" C </H5>"));
                        //-----------------Ogrzewanie pietro
                        if (ogrzewaniePietroDzien) //Upstairs day mode button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 527 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 527 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/6/off\"><button class=\"button\">Pietro "
# 527 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 527 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperature3);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 529 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 529 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "'C dzien</button></a>"
# 529 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 529 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else //Upstairs night mode button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 533 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 533 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/6/on\"><button class=\"button button2\">Pietro "
# 533 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 533 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperature3);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 535 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 535 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "'C noc</button></a>"
# 535 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 535 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        if (!odliczanie.checkTimer(7)) //Force heat upstairs button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 540 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 540 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/7/on\"><button class=\"button\">10 min. On</button></a></p>"
# 540 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 540 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 544 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 544 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/7/on\"><button class=\"button button2\">"
# 544 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 544 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(7));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 546 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 546 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "</button></a>"
# 546 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 546 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 547 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 547 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/7/off\"><button class=\"button\">STOP!</button></a></p>"
# 547 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 547 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        };
                        //----------------Ogrzewanie Parter
                        if (ogrzewanieParterDzien) //Downstairs day mode button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 552 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 552 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/8/off\"><button class=\"button\">Parter "
# 552 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 552 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperature2);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 554 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 554 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "'C dzien</button></a>"
# 554 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 554 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else //Downstairs night mode button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 558 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 558 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/8/on\"><button class=\"button button2\">Parter "
# 558 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 558 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperature2);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 560 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 560 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "'C noc</button></a>"
# 560 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 560 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        if (!odliczanie.checkTimer(9))
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 565 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 565 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/9/on\"><button class=\"button\">10 min. On</button></a></p>"
# 565 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 565 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 569 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 569 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/9/on\"><button class=\"button button2\">"
# 569 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 569 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(9));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 571 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 571 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "</button></a>"
# 571 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 571 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 572 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 572 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/9/off\"><button class=\"button\">STOP!</button></a></p>"
# 572 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 572 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 575 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 575 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<h5>Temperatura dzienna: "
# 575 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 575 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)temperaturaDzien);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 577 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 577 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      " C</H5><H5>Temperatura nocna: "
# 577 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 577 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)temperaturaNoc);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 579 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 579 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      " C</H5>"
# 579 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 579 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));

                        //--------------Podlewanie:
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 582 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 582 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<H3>Podlewanie</H3>"
# 582 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 582 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 583 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 583 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<h5>Czas podlewania LK: "
# 583 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 583 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)podlewanieDL);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 585 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 585 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "</H5><h5>Czas podlewania Z1: "
# 585 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 585 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)podlewanieZ1);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 587 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 587 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "</H5><h5>Czas podlewania Z2: "
# 587 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 587 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)podlewanieZ2);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 589 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 589 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "</H5>"
# 589 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 589 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        if (podlewanieAuto)
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 592 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 592 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/10/off\"><button class=\"button button2\">Podlewanie Auto</button></a>"
# 592 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 592 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 596 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 596 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/10/on\"><button class=\"button \">Podlewanie Reczne</button></a>"
# 596 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 596 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        if (podlewanieCykl == 0)
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 601 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 601 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/11/on\"><button class=\"button\">Start cyklu podlewania</button></a>"
# 601 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 601 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 605 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 605 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/11/off\"><button class=\"button button2\">Cykl podlewanie w trakcie, nadzisnij by zatrzymac</button></a>"
# 605 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 605 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        if (!odliczanie.checkTimer(12))
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 610 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 610 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/12/on\"><button class=\"button\">Start LK</button></a>"
# 610 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 610 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 614 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 614 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/12/off\"><button class=\"button button2\">Stop LK za "
# 614 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 614 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(12));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 616 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 616 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "</button></a>"
# 616 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 616 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        };

                        if (!odliczanie.checkTimer(13))
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 621 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 621 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/13/on\"><button class=\"button\">Start Z1</button></a>"
# 621 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 621 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 625 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 625 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/13/off\"><button class=\"button button2\">Stop Z1 za "
# 625 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 625 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(13));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 627 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 627 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "</button></a>"
# 627 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 627 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        };

                        if (!odliczanie.checkTimer(14))
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 632 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 632 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/14/on\"><button class=\"button\">Start Z2</button></a></p>"
# 632 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 632 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 636 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 636 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/14/off\"><button class=\"button button2\">Stop Z2 za "
# 636 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 636 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(14));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 638 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 638 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          "</button></a></p>"
# 638 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 638 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                          )));
                        };
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 640 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 640 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/nothing\"><button class=\"button button2\">Refresh </button></a></p>"
# 640 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 640 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      )));
                        //-------------Koniec
                        client.println();
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 643 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 643 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      "</body></html>"
# 643 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 643 "c:\\Users\\Grzes\\Documents\\Arduino\\home_control_V_0_07_alfa\\home_control_V_0_07_alfa.ino"
                                      ))); //Nie wiem czemu, ale musialem zakomentowac, bo nie wyswietlal godziny :(
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
                { // if you got anything else but a carriage return character,
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
        digitalWrite(2 /*Z2 watering circut*/, 0x0);
    }
    else
    {
        digitalWrite(2 /*Z2 watering circut*/, 0x1);
    };

    if (odliczanie.checkTimer(13)) //Turn on watering by Z1
    {
        digitalWrite(1 /*Z1 watering circut*/, 0x0);
    }
    else
    {
        digitalWrite(1 /*Z1 watering circut*/, 0x1);
    };

    if (odliczanie.checkTimer(12)) //Turn on watering by LK
    {
        digitalWrite(0 /*droping line*/, 0x0);
    }
    else
    {
        digitalWrite(0 /*droping line*/, 0x1);
    };

    if (odliczanie.checkTimer(9)) //Turn on force to heat downstairs
    {
        digitalWrite(8 /*Heating downstairs*/, 0x0);
    }
    else
    {
        digitalWrite(8 /*Heating downstairs*/, 0x1);
    };

    if (odliczanie.checkTimer(7)) //Turn on force to heat upstairs
    {
        digitalWrite(7 /*Heating upstairs*/, 0x0);
    }
    else
    {
        digitalWrite(7 /*Heating upstairs*/, 0x1);
    };

    if (odliczanie.checkTimer(5)) //Blins up
    {
        digitalWrite(4 /*Blinds up*/, 0x0);
    }
    else
    {
        digitalWrite(4 /*Blinds up*/, 0x1);
    };

    if (odliczanie.checkTimer(4)) //Blinds down
    {
        digitalWrite(5 /*Blinss down*/, 0x0);
    }
    else
    {
        digitalWrite(5 /*Blinss down*/, 0x1);
    };

    if (odliczanie.checkTimer(3)) //Blinds stop
    {
        digitalWrite(3 /*Blinds stop*/, 0x0);
    }
    else
    {
        digitalWrite(3 /*Blinds stop*/, 0x1);
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

    zegar.Flow(); //Time is flowing
    slonce.checkSun(zegar.getSecond(), zegar.getMinute(), zegar.getHour(), zegar.getMonthDay(), zegar.getMonth(), (byte)(zegar.getYear() - 2000), 0); //check, if we have a new day and need a new sunrise and sunset :)
    sensors.request(); //request all the sensors
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
            return true; //Return 1.
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