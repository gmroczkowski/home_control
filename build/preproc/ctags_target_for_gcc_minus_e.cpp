# 1 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
# 20 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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

# 42 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 43 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 44 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 45 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 46 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 47 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 48 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 49 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 50 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 64 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
OneWire onewire(31 /*Dallas temperature sensors*/); //Dallas temperature sensors
DS18B20 sensors(&onewire); //Dallas temperature sensors

# 91 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 92 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2

sunState slonce;
Blinds rolety;

// Auxiliar variables to store the current output state
boolean roletyAuto = true;

boolean roletyLight = true; //Turn on blinds down by light
int roletySetLightLevel = 600; //Set light level
int roletyCurrentLightLevel = 600; //Set light level

boolean gardenLights = false; //Garden lights variable
byte gardenLightsOffHour=2; //Hour to off gardenLights
byte gardenLightsOffMinute=10; //Minutes to off gardenLights

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
boolean podlewanieRainSensor=false; //Rain logical indicator

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
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xE8};
IPAddress ip(172, 26, 160, 16);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

boolean trigger(byte);

void setup()
{

    //-----------------From random nerd tutorials:
    // Initialize the output variables as outputs
    pinMode(23 /*droping line*/, 0x1);
    pinMode(22 /*Z1 watering circut*/, 0x1);
    pinMode(25 /*Z2 watering circut*/, 0x1);
    pinMode(27 /*Blinds up*/, 0x1);
    pinMode(26 /*Blinss down*/, 0x1);
    pinMode(24 /*Blinds stop*/, 0x1);
    pinMode(29 /*Heating upstairs*/, 0x1);
    pinMode(28 /*Heating downstairs*/, 0x1);
    pinMode(30 /*Rain sensor input*/, 0x0);
    pinMode(0 /*Light sensor for blinds analog input*/, 0x0);
    pinMode(32 /*Garden lights output*/, 0x1);
    // Set outputs to HIGH
    digitalWrite(23 /*droping line*/, 0x1);
    digitalWrite(22 /*Z1 watering circut*/, 0x1);
    digitalWrite(25 /*Z2 watering circut*/, 0x1);
    digitalWrite(27 /*Blinds up*/, 0x1);
    digitalWrite(26 /*Blinss down*/, 0x1);
    digitalWrite(24 /*Blinds stop*/, 0x1);
    digitalWrite(29 /*Heating upstairs*/, 0x1);
    digitalWrite(28 /*Heating downstairs*/, 0x1);
    digitalWrite(32 /*Garden lights output*/, 0x1);

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


                        if (header.indexOf("GET /55/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(13); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /54/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(13); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /53/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(13); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /52/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(12); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /51/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(12); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /50/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(12); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /49/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(11); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /48/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(11); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /47/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(11); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /46/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(10); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /45/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(10); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /44/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(10); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /43/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(9); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /42/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(9); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /41/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(9); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /40/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(8); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /39/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(8); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /38/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(8); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /37/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(7); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /36/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(7); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /35/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(7); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /34/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(6); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /33/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(6); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /32/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(6); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /31/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(5); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /30/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(5); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /29/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(5); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /28/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(4); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /27/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(4); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /26/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(4); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /25/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(3); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /24/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(3); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /23/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(3); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /22/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(2); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /21/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(2); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /20/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(2); //Rolety wiatrolap down
                        };

                        if (header.indexOf("GET /19/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(1); //Rolety sypialnia stop
                        };


                        if (header.indexOf("GET /18/on") >= 0) //Rolety wiatrolap up
                        {
                            rolety.blindsDown(1); //Rolety wiatrolap up
                        };


                        if (header.indexOf("GET /17/on") >= 0) //Rolety wiatrolap down
                        {
                            rolety.blindsUp(1); //Rolety wiatrolap down
                        };


                        if (header.indexOf("GET /16/on") >= 0) //Force blinds by light on
                        {
                            roletyLight = 1; //Set automatic blinds down by light
                        };

                        if (header.indexOf("GET /16/off") >= 0) //Force blinds by light off
                        {
                            roletyLight = 0; //Set blins down without light trigger
                        };

                        if (header.indexOf("GET /15/on") >= 0) //Set light level to close blinds
                        {
                            roletySetLightLevel=roletyCurrentLightLevel;
                        };


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
                            Serial.println("Rolety w gore");
                            rolety.blindsUp(0);
                            odliczanie.startTimer(1500, 5); //Only 700ms

                        };
                        /*if (header.indexOf("GET /5/off") >= 0)

                        // {

                            //Serial.println("Rolety w gore zatrzymane");

                            podniesRolety = "off";

                            digitalWrite(pin_BU, HIGH);

                        };*/
# 638 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                        if (header.indexOf("GET /4/on") >= 0) //Blinds down
                        {
                            Serial.println("Rolety w dol");
                            rolety.blindsDown(0);
                            odliczanie.startTimer(1500, 4); //Only 700ms
                        };
                        //if (header.indexOf("GET /4/off") >= 0)
                        //{
                        //    opuscRolety = "off";
                        //   digitalWrite(pin_BD, HIGH);
                        //};
                        if (header.indexOf("GET /3/on") >= 0) //Blinds stop
                        {
                            Serial.println("Rolety w stop");
                            rolety.blindsStop(0);
                            odliczanie.startTimer(1500, 3); //Only 700ms
                        };
                        /*if (header.indexOf("GET /3/off") >= 0)

                        {

                            //Serial.println("Rolety stop zatrzymane");

                            zatrzymajRolety = "off";

                            digitalWrite(pin_BS, HIGH);

                        };*/
# 661 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
# 707 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 707 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<!DOCTYPE html><html>"
# 707 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 707 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 708 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 708 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
# 708 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 708 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 709 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 709 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<link rel=\"icon\" href=\"data:,\">"
# 709 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 709 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 712 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 712 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
# 712 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 712 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 713 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 713 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;"
# 713 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 713 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 714 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 714 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}"
# 714 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 714 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 715 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 715 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ".button2 {background-color: #77878A;}</style></head>"
# 715 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 715 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        // Web Page Heading
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 717 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 717 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<body><H1>Home Control Domek Ozarow V0.07A</H1>"
# 717 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 717 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 718 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 718 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<H3>Rolety</H3>"
# 718 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 718 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 719 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 719 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<h5> Dzis jest: "
# 719 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 719 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getDate());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 721 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 721 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " "
# 721 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 721 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getWeekDay());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 723 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 723 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " godzina: "
# 723 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 723 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getTime());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 725 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 725 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " </h5>"
# 725 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 725 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 726 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 726 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " <h5> Wschod slonca: "
# 726 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 726 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)slonce.sunRiseHour());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 728 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 728 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ":"
# 728 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 728 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)slonce.sunRiseMinute());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 730 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 730 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "</h5><h5> Zachod slonca: "
# 730 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 730 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)slonce.sunSetHour());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 732 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 732 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ":"
# 732 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 732 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)slonce.sunSetMinute());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 734 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 734 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " </h5><h5> Sensor swiatla: "
# 734 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 734 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)roletyCurrentLightLevel);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 736 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 736 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " </h5>"
# 736 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 736 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 737 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 737 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " </h5><h5> Lampy w ogrodzie: "
# 737 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 737 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)gardenLights);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 739 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 739 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " </h5>"
# 739 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 739 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));


                        // Display current state, and ON/OFF buttons for GPIO 5
                        //client.println("<p>GPIO 5 - State " + output5State + "</p>");
                        // If the output5State is off, it displays the ON button

                        if (!roletyAuto)
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 748 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 748 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/2/on\"><button class=\"button\">Blinds Timer Off</button></a>"
# 748 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 748 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 752 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 752 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/2/off\"><button class=\"button button2\">Blinds Timer On</button></a>"
# 752 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 752 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        };


                        if (!roletyLight)
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 758 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 758 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/16/on\"><button class=\"button\">Light sensor off</button></a>"
# 758 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 758 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 762 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 762 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/16/off\"><button class=\"button button2\">Light sensor on</button></a>"
# 762 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 762 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        };

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 765 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 765 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/15/on\"><button class=\"button\">Set light level:"
# 765 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 765 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)roletySetLightLevel);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 767 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 767 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "</button></a></p>"
# 767 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 767 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));


                        if (!odliczanie.checkTimer(5)) //Blins up button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 772 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 772 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/5/on\"><button class=\"button\">Podnies rolety</button></a>"
# 772 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 772 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 776 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 776 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a>"
# 776 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 776 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        };

                        // Display current state, and ON/OFF buttons for GPIO 4
                        //client.println("<p>GPIO 4 - State " + output4State + "</p>");
                        // If the output4State is off, it displays the ON button
                        if (!odliczanie.checkTimer(4)) //Blins down button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 784 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 784 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/4/on\"><button class=\"button\">Opusc rolety</button></a></p>"
# 784 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 784 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 788 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 788 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a></p>"
# 788 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 788 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        };
                        ;

                        //client.println(F("</body></html>"));

                        //if (!odliczanie.checkTimer(3)) //Blins stop button
                        //{
                        //}

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 798 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 798 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<H3>Ogrzewanie</H3>"
# 798 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 798 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 799 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 799 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<h5>Temperatura zewnetrzna: <B>"
# 799 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 799 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)temperature1);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 801 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 801 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " C</B></H5>"
# 801 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 801 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
# 810 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 810 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/6/off\"><button class=\"button\">Pietro "
# 810 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 810 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperature3);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 812 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 812 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "'C dzien</button></a>"
# 812 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 812 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else //Upstairs night mode button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 816 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 816 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/6/on\"><button class=\"button button2\">Pietro "
# 816 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 816 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperature3);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 818 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 818 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "'C noc</button></a>"
# 818 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 818 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        if (!odliczanie.checkTimer(7)) //Force heat upstairs button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 823 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 823 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/7/on\"><button class=\"button\">10 min. On</button></a></p>"
# 823 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 823 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 827 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 827 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/7/on\"><button class=\"button button2\">"
# 827 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 827 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(7));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 829 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 829 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "</button></a>"
# 829 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 829 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 830 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 830 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/7/off\"><button class=\"button\">STOP!</button></a></p>"
# 830 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 830 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        };
                        //----------------Ogrzewanie Parter
                        if (ogrzewanieParterDzien) //Downstairs day mode button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 835 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 835 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/8/off\"><button class=\"button\">Parter "
# 835 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 835 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperature2);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 837 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 837 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "'C dzien</button></a>"
# 837 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 837 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else //Downstairs night mode button
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 841 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 841 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/8/on\"><button class=\"button button2\">Parter "
# 841 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 841 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperature2);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 843 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 843 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "'C noc</button></a>"
# 843 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 843 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        if (!odliczanie.checkTimer(9))
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 848 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 848 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/9/on\"><button class=\"button\">10 min. On</button></a></p>"
# 848 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 848 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 852 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 852 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/9/on\"><button class=\"button button2\">"
# 852 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 852 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(9));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 854 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 854 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "</button></a>"
# 854 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 854 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 855 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 855 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/9/off\"><button class=\"button\">STOP!</button></a></p>"
# 855 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 855 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 858 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 858 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<h5>Temperatura dzienna: "
# 858 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 858 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)temperaturaDzien);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 860 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 860 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " C</H5><H5>Temperatura nocna: "
# 860 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 860 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)temperaturaNoc);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 862 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 862 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " C</H5>"
# 862 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 862 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        //--------------Podlewanie:
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 865 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 865 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<H3>Podlewanie</H3>"
# 865 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 865 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        if (podlewanieRainSensor)
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 868 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 868 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><input type=""checkbox"" onclick=""return false"" checked> Deszcz"
# 868 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 868 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 872 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 872 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><input type=""checkbox"" onclick=""return false"" unchecked> Deszcz"
# 872 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 872 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        if (podlewanieAuto)
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 877 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 877 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/10/off\"><button class=\"button button2\">Podlewanie Auto</button></a>"
# 877 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 877 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 881 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 881 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/10/on\"><button class=\"button \">Podlewanie Reczne</button></a>"
# 881 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 881 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        if (podlewanieCykl == 0)
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 886 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 886 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/11/on\"><button class=\"button\">Start cyklu podlewania</button></a>"
# 886 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 886 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 890 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 890 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/11/off\"><button class=\"button button2\">Cykl podlewanie w trakcie, nadzisnij by zatrzymac</button></a>"
# 890 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 890 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }

                        if (!odliczanie.checkTimer(12))
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 895 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 895 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/12/on\"><button class=\"button\">Start LK</button></a>"
# 895 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 895 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 899 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 899 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/12/off\"><button class=\"button button2\">Stop LK za "
# 899 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 899 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(12));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 901 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 901 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "</button></a>"
# 901 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 901 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        };

                        if (!odliczanie.checkTimer(13))
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 906 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 906 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/13/on\"><button class=\"button\">Start Z1</button></a>"
# 906 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 906 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 910 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 910 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/13/off\"><button class=\"button button2\">Stop Z1 za "
# 910 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 910 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(13));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 912 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 912 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "</button></a>"
# 912 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 912 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        };

                        if (!odliczanie.checkTimer(14))
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 917 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 917 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/14/on\"><button class=\"button\">Start Z2</button></a></p>"
# 917 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 917 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        }
                        else
                        {
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 921 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 921 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/14/off\"><button class=\"button button2\">Stop Z2 za "
# 921 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 921 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println(odliczanie.getTime(14));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 923 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 923 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "</button></a></p>"
# 923 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 923 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                        };

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 926 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 926 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<H3>Rolety</H3>"
# 926 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 926 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 928 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 928 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/17/on\"><button class=\"button button\">Rolety w sypialni w gore</button></a>"
# 928 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 928 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 929 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 929 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/18/on\"><button class=\"button button\">W dol</button></a>"
# 929 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 929 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 930 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 930 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/19/on\"><button class=\"button button\">STOP!</button></a></p>"
# 930 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 930 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 932 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 932 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/20/on\"><button class=\"button button\">Rolety 2 w gore</button></a>"
# 932 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 932 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 933 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 933 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/21/on\"><button class=\"button button\">W dol</button></a>"
# 933 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 933 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 934 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 934 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/22/on\"><button class=\"button button\">STOP!</button></a></p>"
# 934 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 934 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 936 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 936 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/23/on\"><button class=\"button button\">Rolety 3 w gore</button></a>"
# 936 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 936 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 937 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 937 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/24/on\"><button class=\"button button\">W dol</button></a>"
# 937 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 937 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 938 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 938 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/25/on\"><button class=\"button button\">STOP!</button></a></p>"
# 938 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 938 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 940 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 940 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/26/on\"><button class=\"button button\">Rolety 4 w gore</button></a>"
# 940 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 940 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 941 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 941 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/27/on\"><button class=\"button button\">W dol</button></a>"
# 941 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 941 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 942 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 942 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/28/on\"><button class=\"button button\">STOP!</button></a></p>"
# 942 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 942 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 944 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 944 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/29/on\"><button class=\"button button\">Rolety 5 w gore</button></a>"
# 944 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 944 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 945 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 945 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/30/on\"><button class=\"button button\">W dol</button></a>"
# 945 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 945 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 946 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 946 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/31/on\"><button class=\"button button\">STOP!</button></a></p>"
# 946 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 946 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 948 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 948 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/32/on\"><button class=\"button button\">Rolety 6 w gore</button></a>"
# 948 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 948 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 949 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 949 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/33/on\"><button class=\"button button\">W dol</button></a>"
# 949 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 949 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 950 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 950 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/34/on\"><button class=\"button button\">STOP!</button></a></p>"
# 950 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 950 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 952 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 952 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/35/on\"><button class=\"button button\">Rolety 7 w gore</button></a>"
# 952 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 952 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 953 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 953 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/36/on\"><button class=\"button button\">W dol</button></a>"
# 953 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 953 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 954 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 954 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/37/on\"><button class=\"button button\">STOP!</button></a></p>"
# 954 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 954 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 956 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 956 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/38/on\"><button class=\"button button\">Rolety 8 w gore</button></a>"
# 956 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 956 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 957 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 957 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/39/on\"><button class=\"button button\">W dol</button></a>"
# 957 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 957 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 958 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 958 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/40/on\"><button class=\"button button\">STOP!</button></a></p>"
# 958 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 958 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 960 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 960 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/41/on\"><button class=\"button button\">Rolety 9 w gore</button></a>"
# 960 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 960 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 961 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 961 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/42/on\"><button class=\"button button\">W dol</button></a>"
# 961 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 961 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 962 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 962 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/43/on\"><button class=\"button button\">STOP!</button></a></p>"
# 962 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 962 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 964 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 964 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/44/on\"><button class=\"button button\">Rolety 10 w gore</button></a>"
# 964 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 964 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 965 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 965 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/45/on\"><button class=\"button button\">W dol</button></a>"
# 965 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 965 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 966 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 966 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/46/on\"><button class=\"button button\">STOP!</button></a></p>"
# 966 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 966 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 968 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 968 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/47/on\"><button class=\"button button\">Rolety 11 w gore</button></a>"
# 968 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 968 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/48/on\"><button class=\"button button\">W dol</button></a>"
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 970 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 970 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/49/on\"><button class=\"button button\">STOP!</button></a></p>"
# 970 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 970 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 972 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 972 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/50/on\"><button class=\"button button\">Rolety 12 w gore</button></a>"
# 972 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 972 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 973 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 973 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/51/on\"><button class=\"button button\">W dol</button></a>"
# 973 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 973 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 974 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 974 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/52/on\"><button class=\"button button\">STOP!</button></a></p>"
# 974 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 974 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<p><a href=\"/53/on\"><button class=\"button button\">Rolety 13 w gore</button></a>"
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 977 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 977 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/54/on\"><button class=\"button button\">W dol</button></a>"
# 977 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 977 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 978 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 978 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/55/on\"><button class=\"button button\">STOP!</button></a></p>"
# 978 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 978 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));



                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 982 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 982 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/nothing\"><button class=\"button button2\">Refresh </button></a></p>"
# 982 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 982 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        //-------------Koniec
                        client.println();
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 985 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 985 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "</body></html>"
# 985 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 985 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
        digitalWrite(25 /*Z2 watering circut*/, 0x0);
    }
    else
    {
        digitalWrite(25 /*Z2 watering circut*/, 0x1);
    };

    if (odliczanie.checkTimer(13)) //Turn on watering by Z1
    {
        digitalWrite(22 /*Z1 watering circut*/, 0x0);
    }
    else
    {
        digitalWrite(22 /*Z1 watering circut*/, 0x1);
    };

    if (odliczanie.checkTimer(12)) //Turn on watering by LK
    {
        digitalWrite(23 /*droping line*/, 0x0);
    }
    else
    {
        digitalWrite(23 /*droping line*/, 0x1);
    };

    if (odliczanie.checkTimer(9)) //Turn on force to heat downstairs
    {
        digitalWrite(28 /*Heating downstairs*/, 0x0);
    }
    else
    {
        digitalWrite(28 /*Heating downstairs*/, 0x1);
    };

    if (odliczanie.checkTimer(7)) //Turn on force to heat upstairs
    {
        digitalWrite(29 /*Heating upstairs*/, 0x0);
    }
    else
    {
        digitalWrite(29 /*Heating upstairs*/, 0x1);
    };

    if (odliczanie.checkTimer(5)) //Blins up
    {
        digitalWrite(27 /*Blinds up*/, 0x0);
    }
    else
    {
        digitalWrite(27 /*Blinds up*/, 0x1);
    };

    if (odliczanie.checkTimer(4)) //Blinds down
    {
        digitalWrite(26 /*Blinss down*/, 0x0);
    }
    else
    {
        digitalWrite(26 /*Blinss down*/, 0x1);
    };

    if (odliczanie.checkTimer(3)) //Blinds stop
    {
        digitalWrite(24 /*Blinds stop*/, 0x0);
    }
    else
    {
        digitalWrite(24 /*Blinds stop*/, 0x1);
    };

    if (gardenLights) //Check, if garden lights should be on
    {
        digitalWrite(32 /*Garden lights output*/,0x0);
    }
    else
    {
        digitalWrite(32 /*Garden lights output*/,0x1);
    }


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
        Serial.println("Rolety w gore by trigger");
        odliczanie.startTimer(1500, 5); //Only 1500ms
    };
    if ((trigger(2)) && (roletyAuto)) //Auto blins up if it is on
    {
        Serial.println("Rolety w dol by trigger");
        odliczanie.startTimer(1500, 4); //Only 1500ms
    };

    if (trigger(3) && (podlewanieAuto) && (!podlewanieRainSensor)) //Auto watering is inside the trigger, and we have no rain :)
    {
        //Serial.println("Starutjemy cykl podlewania");
        podlewanieCykl = 1;
    };

    trigger(4); //Check, if we have to turn on garden lights.

    trigger(5); //Check, if we have to turn off garden lights.

    //----------------------------------------//
    //              MAIN BLOCK                //
    //----------------------------------------//

    zegar.Flow(); //Time is flowing
    slonce.checkSun(zegar.getSecond(), zegar.getMinute(), zegar.getHour(), zegar.getMonthDay(), zegar.getMonth(), (byte)(zegar.getYear() - 2000), 0); //check, if we have a new day and need a new sunrise and sunset :)
    sensors.request(); //request all the sensors
    temperature1 = sensors.readTemperature(address1);
    temperature2 = sensors.readTemperature(address2);
    temperature3 = sensors.readTemperature(address3);

    roletyCurrentLightLevel = analogRead(0 /*Light sensor for blinds analog input*/); // Read light intensivity
    podlewanieRainSensor=digitalRead(30 /*Rain sensor input*/); // Check rain sensor
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
            odliczanie.startTimer(18000000,15); //Locking blinds light trigger to start one time.
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
        if ((zegar.getHour() == slonce.sunSetHour()) && (zegar.getMinute() == slonce.sunSetMinute()) && (zegar.getSecond() == 0) && (!lockTrigger[number])) //If we have Sun set :)
        {
            //Serial.println("Trigger: jest zachód!");
            lockTrigger[number] = true; //Locking trigger to start one time.
            return true;
        }
        else
        {
            if (zegar.getSecond() != 0)
                lockTrigger[number] = false; //Unocking trigger becouse second not zero - could be start again

        }

        if ((roletyCurrentLightLevel==roletySetLightLevel)&&(!odliczanie.checkTimer(15))&&(roletyLight)) //If it is dark
        {
            //Serial.println("Trigger: jest ciemno!");
            odliczanie.startTimer(3600000,15); //Locking trigger to start one time.
            return true;
        }
        else
        {
            return false;
        }
        return false;
        break;
    case 3:
        if ((zegar.getHour() == wateringHour) && (zegar.getMinute() == wateringMinute) && (zegar.getSecond() == 0) && (!lockTrigger[number])) //If we have watering time :)
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
    case 4:
        if ((zegar.getHour() == slonce.sunSetHour()) && (zegar.getMinute() == slonce.sunSetMinute()+5) && (zegar.getSecond() == 0) && (!gardenLights)) //If we have 5 minutes after Sun set and gardenLights are off:)
        {
            gardenLights=true;
            return true;
        }
        return false;
        break;
    case 5:
        if ((zegar.getHour() == gardenLightsOffHour) && (zegar.getMinute() == gardenLightsOffMinute) && (zegar.getSecond() == 0) && (gardenLights)) //If we have 10 minutes past 2AM :)
        {
            gardenLights=false;
            return true;
        }
        return false;
        break;
    }
}
