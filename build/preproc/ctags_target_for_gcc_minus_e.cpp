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
# 51 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 52 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 53 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 54 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2




//#define pin_BS 24           //Blinds stop - turn off - blinds now using RF
//#define pin_BU 27           //Blinds up - turn off - blinds now using RF
//#define pin_BD 26           //Blinss down - turn off - blinds now using RF







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

unsigned long uptime; //uptime :)

int line_to_show=100; //How much line should be shown in parameters of logs

# 99 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 100 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2

sunState slonce;
Blinds rolety;

// Auxiliar variables to store the current output state
boolean roletyAuto = true;

boolean roletyLight = false; //Turn on blinds down by light
int roletySetLightLevel = 600; //Set light level
int roletyCurrentLightLevel = 600; //Set light level

boolean gardenLights = false; //Garden lights variable
boolean gardenLightsAuto = true; //Auto on for gardern lights
byte gardenLightsMinutesOffset=10; //The garden light will start after the sunset
byte gardenLightsOffHour = 2; //Hour to off gardenLights
byte gardenLightsOffMinute = 10; //Minutes to off gardenLights

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
boolean podlewanieRainSensor = false; //Rain logical indicator

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
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA};
IPAddress ip(172, 26, 160, 16);
IPAddress dns(172, 26, 160, 254);
IPAddress gateway(172, 26, 160, 254);
IPAddress subnet(255, 255, 255, 0);

unsigned int localPort = 8888; // local port to listen for UDP packets
//const char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server
const char timeServer[] = "pl.pool.ntp.org";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):

EthernetUDP Udp;
EthernetServer server(80);

boolean trigger(byte);
void sendNTPpacket(const char *address);

File logging; //write log to SD card

void setup()
{

    //-----------------From random nerd tutorials:
    // Initialize the output variables as outputs
    pinMode(23 /*droping line*/, 0x1);
    pinMode(22 /*Z1 watering circut*/, 0x1);
    pinMode(25 /*Z2 watering circut*/, 0x1);
    //pinMode(pin_BU, OUTPUT); // - turn off - blinds now using RF
    //pinMode(pin_BD, OUTPUT); // - turn off - blinds now using RF
    //pinMode(pin_BS, OUTPUT); // - turn off - blinds now using RF
    pinMode(29 /*Heating upstairs*/, 0x1);
    pinMode(28 /*Heating downstairs*/, 0x1);
    pinMode(30 /*Rain sensor input*/, 0x0);
    pinMode(0 /*Light sensor for blinds analog input*/, 0x0);
    pinMode(24 /*Garden lights output*/, 0x1);
    // Set outputs to HIGH
    digitalWrite(23 /*droping line*/, 0x1);
    digitalWrite(22 /*Z1 watering circut*/, 0x1);
    digitalWrite(25 /*Z2 watering circut*/, 0x1);
    //digitalWrite(pin_BU, HIGH); // - turn off - blinds now using RF
    //digitalWrite(pin_BD, HIGH); // - turn off - blinds now using RF
    //digitalWrite(pin_BS, HIGH); // - turn off - blinds now using RF
    digitalWrite(29 /*Heating upstairs*/, 0x1);
    digitalWrite(28 /*Heating downstairs*/, 0x1);
    digitalWrite(24 /*Garden lights output*/, 0x1);

    Serial.begin(115200); //Starting serial

    Serial.println("Starting...");
    Serial.print("Initializing SD card...");

    if (!SD.begin(4))
    {
        Serial.println("initialization failed!");
    }
    else
        Serial.println("initialization done.");

    logWrite("Starting...");

    for (int i = 0; i < 5; i++)
        lockTrigger[i] = false;

    Serial.println("Initializing Dallas temperature sensors...");
    logWrite("Initializing Dallas temperature sensors...");

    //Dallas temperature sensors:
    sensors.begin(12); //resolution 12
    sensors.request(); //request all the sensors
    // Open serial communications and wait for port to open:

    Serial.println("Initializing timelord SunRise and SunSet library...");
    logWrite("Initializing timelord SunRise and SunSet library...");

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

    Serial.println("Initializing Ethernet connection...");
    logWrite("Initializing Ethernet connection...");
    //start the Ethernet connection and the server :
    Ethernet.begin(mac, ip, dns, gateway, subnet);

    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        Serial.println("No ethernet expansion shield");
        logWrite("No ethernet expansion shield");
    }
    if (Ethernet.linkStatus() == LinkOFF)
    {
        Serial.println("Ethernet cable is not connected.");
        logWrite("Ethernet cable is not connected.");
    }

    Udp.begin(localPort); //Start UDP
    logWrite("IP:");
    logWrite((String)Ethernet.localIP());
    logWrite(" DNS server ip: ");
    logWrite((String)Ethernet.dnsServerIP());
    logWrite(" Gateway ip: ");
    logWrite((String)Ethernet.gatewayIP());
    logWrite(" Subnet mask:");
    logWrite((String)Ethernet.subnetMask());

    Serial.print("IP:");
    Serial.println(Ethernet.localIP());
    Serial.print(" DNS server ip: ");
    Serial.println(Ethernet.dnsServerIP());
    Serial.print(" gateway ip: ");
    Serial.println(Ethernet.gatewayIP());
    Serial.print(" Subnet mask:");
    Serial.println(Ethernet.subnetMask());

    logWrite("Getting the current Date and Time from NTP server...");
    Serial.println("Getting the current Date and Time from NTP server...");
    sendNTPpacket(timeServer); // send an NTP packet to a time server
    odliczanie.startTimer(60000, 0); // timer 60s for timeout of NTP receive packet
    unsigned long epoch = 0; //unic time
    while (!Udp.parsePacket())
    {
        if (!odliczanie.checkTimer(0))
        {
            Serial.println("NTP timeout. Setting default time.");
            logWrite("NTP timeout. Setting default time.");
            epoch = 1590183595; //a time to set anything
            break;
        }
    };

    Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = makeWord(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = makeWord(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    logWrite("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    if (!epoch)
        epoch = secsSince1900 - seventyYears; //if no timeout on getting NTP info
    // print Unix time:
    Serial.println(epoch);
    logWrite((String)epoch);
    time_t utcCalc = epoch;
    Serial.println("UTC Date: " + (String)year(utcCalc) + "-" + odliczanie.leadingZero(month(utcCalc)) + "-" + odliczanie.leadingZero(day(utcCalc)) + " " + odliczanie.leadingZero(hour(utcCalc)) + ":" + odliczanie.leadingZero(minute(utcCalc)));
    logWrite("UTC Date: " + (String)year(utcCalc) + "-" + odliczanie.leadingZero(month(utcCalc)) + "-" + odliczanie.leadingZero(day(utcCalc)) + " " + odliczanie.leadingZero(hour(utcCalc)) + ":" + odliczanie.leadingZero(minute(utcCalc)));

    zegar.setYear((int)year(utcCalc));
    zegar.setMonth((byte)(month(utcCalc)));
    zegar.setMonthDay((byte)day(utcCalc));
    zegar.setHour((byte)hour(utcCalc) + 2);
    zegar.setMinute((byte)minute(utcCalc));
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
    tardis.TimeZone(1 * 60); // tell TimeLord what timezone your RTC is synchronized to. You can ignore DST
    // as long as the RTC never changes back and forth between DST and non-DST
    tardis.Position(LATITUDE, LONGITUDE); // tell TimeLord where in the world we are
    tardis.DstRules(3, 4, 10, 4, 60);
    //Set up sunrise and sunset triggers
    slonce.checkSun(zegar.getSecond(), zegar.getMinute(), zegar.getHour(), zegar.getMonthDay(), zegar.getMonth(), (byte)(zegar.getYear() - 2000), 1);

    Udp.stop(); //stop UDP
    //Ethernet.begin(mac, ip);

    // start the server
    server.begin();
    Serial.print("server is at ");
    logWrite("server is at ");
    Serial.println(Ethernet.localIP());
    logWrite((String)Ethernet.localIP());
    odliczanie.startTimer(20, 1);
    slonce.checkSun(zegar.getSecond(), zegar.getMinute(), zegar.getHour(), zegar.getMonthDay(), zegar.getMonth(), (byte)(zegar.getYear() - 2000), 1);
    logging.close(); //Close logging file

    odliczanie.startTimer(1000, 0); //uptime
    odliczanie.startTimer(300000, 1); //logging time - every 5 minutes
}

void loop()
{
    int podstrona = 0; //help to show context - 0 -main web page, 1-blinds, 2-blinds settings, 3-watering, 4-heating, 5-parameters, 6-logs
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

                        if (header.indexOf("GET /71/off") >= 0) //Garder lighs off
                        {
                            gardenLights = false;
                            logWrite("Garder lighs off");
                        };

                        if (header.indexOf("GET /71/on") >= 0) //Garder lighs on
                        {
                            gardenLights = true;
                            logWrite("Garder lighs on");
                        };

                        if (header.indexOf("GET /70/off") >= 0) //Auto garden lights off
                        {
                            gardenLightsAuto = false;
                            logWrite("Auto garden lights off");
                        };

                        if (header.indexOf("GET /70/on") >= 0) //Auto garder lights on
                        {
                            gardenLightsAuto = true;
                            logWrite("Auto garder lights on");
                        };

                        if (header.indexOf("GET /69/on") >= 0) //Show garden lights
                        {
                            podstrona = 7; //Set content parameter to 6 - show garden lights settings
                            logWrite("Show garden lights");
                        };

                        if (header.indexOf("GET /68/on") >= 0) //Show log page
                        {
                            podstrona = 6; //Set content parameter to 6 - show log page
                            logWrite("Show log page");
                        };

                        if (header.indexOf("GET /67/on") >= 0) //Show heating page
                        {
                            podstrona = 4; //Set content parameter to 4 - heating
                            logWrite("Show heating page");
                        };

                        if (header.indexOf("GET /66/on") >= 0) //Show watering page
                        {
                            podstrona = 3; //Set content parameter to 3 - watering
                            logWrite("Show watering page");
                        };

                        if (header.indexOf("GET /65/on") >= 0) //Show blinds page
                        {
                            podstrona = 2; //Set content parameter to 2 - blinds settings
                            logWrite("Show blinds page");
                        };

                        if (header.indexOf("GET /64/on") >= 0) //Show blinds page
                        {
                            podstrona = 1; //Set content parameter to 1 - blinds
                            logWrite("Show blinds page");
                        };

                        if (header.indexOf("GET /63/on") >= 0) //Show main page
                        {
                            podstrona = 0; //Set content variable to 0 - main page
                            logWrite("Show main page");
                        };

                        if (header.indexOf("GET /62/on") >= 0) //Show parameters page
                        {
                            podstrona = 5; //Show parameters page
                            logWrite("Show parameters page");
                        };

                        if (header.indexOf("GET /61/on") >= 0) //Rolety lazienka gora stop
                        {
                            rolety.blindsStop(15); //Rolety lazienka gora stop
                            logWrite("Rolety lazienka gora stop");
                        };

                        if (header.indexOf("GET /60/on") >= 0) //Rolety lazienka gora down
                        {
                            rolety.blindsDown(15); //Rolety lazienka gora down
                            logWrite("Rolety lazienka gora down");
                        };

                        if (header.indexOf("GET /59/on") >= 0) //Rolety lazienka gora up
                        {
                            rolety.blindsUp(15); //Rolety lazienka gora up
                            logWrite("Rolety lazienka gora up");
                        };

                        if (header.indexOf("GET /58/on") >= 0) //Rolety lazienka gora stop
                        {
                            rolety.blindsStop(14); //Rolety lazienka gora stop
                            logWrite("Rolety lazienka gora stop");
                        };

                        if (header.indexOf("GET /57/on") >= 0) //Rolety lazienka gora down
                        {
                            rolety.blindsDown(14); //Rolety lazienka gora down
                            logWrite("Rolety lazienka gora down");
                        };

                        if (header.indexOf("GET /56/on") >= 0) //Rolety lazienka gora up
                        {
                            rolety.blindsUp(14); //Rolety lazienka gora up
                            logWrite("Rolety lazienka gora up");
                        };

                        if (header.indexOf("GET /55/on") >= 0) //Rolety lazienka gora stop
                        {
                            rolety.blindsStop(13); //Rolety lazienka gora stop
                            logWrite("Rolety lazienka gora stop");
                        };

                        if (header.indexOf("GET /54/on") >= 0) //Rolety lazienka gora down
                        {
                            rolety.blindsDown(13); //Rolety lazienka gora down
                            logWrite("Rolety lazienka gora down");
                        };

                        if (header.indexOf("GET /53/on") >= 0) //Rolety lazienka gora up
                        {
                            rolety.blindsUp(13); //Rolety lazienka gora up
                            logWrite("Rolety lazienka gora up");
                        };

                        if (header.indexOf("GET /52/on") >= 0) //Rolety kuchnia stop
                        {
                            rolety.blindsStop(12); //Rolety kuchnia stop
                            logWrite("Rolety kuchnia stop");
                        };

                        if (header.indexOf("GET /51/on") >= 0) //Rolety kuchnia down
                        {
                            rolety.blindsDown(12); //Rolety kuchnia down
                            logWrite("Rolety kuchnia down");
                        };

                        if (header.indexOf("GET /50/on") >= 0) //Rolety kuchnia up
                        {
                            rolety.blindsUp(12); //Rolety kuchnia up
                            logWrite("Rolety kuchnia up");
                        };

                        if (header.indexOf("GET /49/on") >= 0) //Rolety Mikolaj stop
                        {
                            rolety.blindsStop(11); //Rolety Mikolaj stop
                            logWrite("Rolety Mikolaj stop");
                        };

                        if (header.indexOf("GET /48/on") >= 0) //Rolety Mikolaj down
                        {
                            rolety.blindsDown(11); //Rolety Mikolaj down
                            logWrite("Rolety Mikolaj down");
                        };

                        if (header.indexOf("GET /47/on") >= 0) //Rolety Mikolaj up
                        {
                            rolety.blindsUp(11); //Rolety Mikolaj up
                            logWrite("Rolety Mikolaj up");
                        };

                        if (header.indexOf("GET /46/on") >= 0) //Rolety salon taras male stop
                        {
                            rolety.blindsStop(10); //Rolety salon taras male stop
                            logWrite("Rolety salon taras male stop");
                        };

                        if (header.indexOf("GET /45/on") >= 0) //Rolety salon taras male down
                        {
                            rolety.blindsDown(10); //Rolety salon taras male down
                            logWrite("Rolety salon taras male down");
                        };

                        if (header.indexOf("GET /44/on") >= 0) //Rolety salon taras male up
                        {
                            rolety.blindsUp(10); //Rolety salon taras male up
                            logWrite("Rolety salon taras male up");
                        };

                        if (header.indexOf("GET /43/on") >= 0) //Rolety pracownia balkon stop
                        {
                            rolety.blindsStop(9); //Rolety pracownia balkon stop
                            logWrite("Rolety pracownia balkon stop");
                        };

                        if (header.indexOf("GET /42/on") >= 0) //Rolety pracownia balkon down
                        {
                            rolety.blindsDown(9); //Rolety pracownia balkon down
                            logWrite("Rolety pracownia balkon down");
                        };

                        if (header.indexOf("GET /41/on") >= 0) //Rolety pracownia balkon up
                        {
                            rolety.blindsUp(9); //Rolety pracownia balkon up
                            logWrite("Rolety pracownia balkon up");
                        };

                        if (header.indexOf("GET /40/on") >= 0) //Rolety lazienka dol, wiatrolap stop
                        {
                            rolety.blindsStop(8); //Rolety lazienka dol, wiatrolap stop
                            logWrite("Rolety lazienka dol, wiatrolap stop");
                        };

                        if (header.indexOf("GET /39/on") >= 0) //Rolety lazienka dol, wiatrolap down
                        {
                            rolety.blindsDown(8); //Rolety lazienka dol, wiatrolap down
                            logWrite("Rolety lazienka dol, wiatrolap down");
                        };

                        if (header.indexOf("GET /38/on") >= 0) //Rolety lazienka dol, wiatrolap up
                        {
                            rolety.blindsUp(8); //Rolety lazienka dol, wiatrolap up
                            logWrite("Rolety lazienka dol, wiatrolap up");
                        };

                        if (header.indexOf("GET /37/on") >= 0) //Rolety ? stop
                        {
                            rolety.blindsStop(7); //Rolety ? stop
                            logWrite("Rolety ? stop");
                        };

                        if (header.indexOf("GET /36/on") >= 0) //Rolety ? down
                        {
                            rolety.blindsDown(7); //Rolety ? down
                            logWrite("Rolety ? down");
                        };

                        if (header.indexOf("GET /35/on") >= 0) //Rolety ? up
                        {
                            rolety.blindsUp(7); //Rolety ? up
                            logWrite("Rolety ? up");
                        };

                        if (header.indexOf("GET /34/on") >= 0) //Rolety salon taras wyjscie stop
                        {
                            rolety.blindsStop(6); //Rolety salon taras wyjscie stop
                            logWrite("Rolety salon taras wyjscie stop");
                        };

                        if (header.indexOf("GET /33/on") >= 0) //Rolety salon taras wyjscie down
                        {
                            rolety.blindsDown(6); //Rolety salon taras wyjscie down
                            logWrite("Rolety salon taras wyjscie down");
                        };

                        if (header.indexOf("GET /32/on") >= 0) //Rolety salon taras wyjscie up
                        {
                            rolety.blindsUp(6); //Rolety salon taras wyjscie up
                            logWrite("Rolety salon taras wyjscie up");
                        };

                        if (header.indexOf("GET /31/on") >= 0) //Rolety pracownia male okno stop
                        {
                            rolety.blindsStop(5); //Rolety pracownia male okno stop
                            logWrite("Rolety pracownia male okno stop");
                        };

                        if (header.indexOf("GET /30/on") >= 0) //Rolety pracownia male okno down
                        {
                            rolety.blindsDown(5); //Rolety pracownia male okno down
                            logWrite("Rolety pracownia male okno down");
                        };

                        if (header.indexOf("GET /29/on") >= 0) //Rolety pracownia male okno up
                        {
                            rolety.blindsUp(5); //Rolety pracownia male okno up
                            logWrite("Rolety pracownia male okno up");
                        };

                        if (header.indexOf("GET /28/on") >= 0) //Rolety ? stop
                        {
                            rolety.blindsStop(4); //Rolety ? stop
                            logWrite("Rolety ? stop");
                        };

                        if (header.indexOf("GET /27/on") >= 0) //Rolety ? down
                        {
                            rolety.blindsDown(4); //Rolety ? down
                            logWrite("Rolety ? down");
                        };

                        if (header.indexOf("GET /26/on") >= 0) //Rolety ? up
                        {
                            rolety.blindsUp(4); //Rolety ? up
                            logWrite("Rolety ? up");
                        };

                        if (header.indexOf("GET /25/on") >= 0) //Rolety Jagoda stop
                        {
                            rolety.blindsStop(3); //Rolety Jagoda stop
                            logWrite("Rolety Jagoda stop");
                        };

                        if (header.indexOf("GET /24/on") >= 0) //Rolety Jagoda down
                        {
                            rolety.blindsDown(3); //Rolety Jagoda down
                            logWrite("Rolety Jagoda down");
                        };

                        if (header.indexOf("GET /23/on") >= 0) //Rolety Jagoda up
                        {
                            rolety.blindsUp(3); //Rolety Jagoda up
                            logWrite("Rolety Jagoda up");
                        };

                        if (header.indexOf("GET /22/on") >= 0) //Rolety salon ogrod stop
                        {
                            rolety.blindsStop(2); //Rolety salon ogrod stop
                            logWrite("Rolety salon ogrod stop");
                        };

                        if (header.indexOf("GET /21/on") >= 0) //Rolety salon ogrod down
                        {
                            rolety.blindsDown(2); //Rolety salon ogrod down
                            logWrite("Rolety salon ogrod down");
                        };

                        if (header.indexOf("GET /20/on") >= 0) //Rolety salon ogrod up
                        {
                            rolety.blindsUp(2); //Rolety salon ogrod up
                            logWrite("Rolety salon ogrod up");
                        };

                        if (header.indexOf("GET /19/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(1); //Rolety sypialnia stop
                            logWrite("Rolety sypialnia stop");
                        };

                        if (header.indexOf("GET /18/on") >= 0) //Rolety sypialnia down
                        {
                            rolety.blindsDown(1); //Rolety sypialnia down
                            logWrite("Rolety sypialnia down");
                        };

                        if (header.indexOf("GET /17/on") >= 0) //Rolety sypialnia up
                        {
                            rolety.blindsUp(1); //Rolety sypialnia up
                            logWrite("Rolety sypialnia up");
                        };

                        if (header.indexOf("GET /16/on") >= 0) //Force blinds by light on
                        {
                            roletyLight = 1; //Set automatic blinds down by light
                            logWrite("Force auto blinds by light on");
                        };

                        if (header.indexOf("GET /16/off") >= 0) //Force blinds by light off
                        {
                            roletyLight = 0; //Set blins down without light trigger
                            logWrite("Force auto blinds by light off");
                        };

                        if (header.indexOf("GET /15/on") >= 0) //Set light level to close blinds
                        {
                            roletySetLightLevel = roletyCurrentLightLevel;
                            logWrite("Set light level to close blinds to: " + (String)roletySetLightLevel);
                        };

                        if (header.indexOf("GET /14/off") >= 0) //Force Z2 to stop waterring
                        {
                            odliczanie.resetTimer(14); //Just a reset Z2 timer
                            logWrite("Force Z2 to start waterring");
                        };

                        if (header.indexOf("GET /14/on") >= 0)
                        {
                            odliczanie.startTimer((unsigned long int)(podlewanieZ2)*1000, 14); //Start Z2 timer
                            logWrite("Force Z2 to start waterring");
                        };
                        if (header.indexOf("GET /13/off") >= 0) //Force Z1 to start waterring
                        {
                            odliczanie.resetTimer(13); //Just a reset Z1 timer
                            logWrite("Force Z1 to start waterring");
                        };

                        if (header.indexOf("GET /13/on") >= 0) //Force Z1 to start waterring
                        {
                            odliczanie.startTimer((unsigned long int)(podlewanieZ1)*1000, 13); //Start Z1 timer
                            logWrite("Force Z1 to start waterring");
                        };
                        if (header.indexOf("GET /12/off") >= 0) //Force dropping line to stop waterring
                        {
                            odliczanie.resetTimer(12); //Just a reset dropping line timer
                            logWrite("Force dropping line to stop waterring");
                        };

                        if (header.indexOf("GET /12/on") >= 0) //Forse dropping line to start
                        {
                            odliczanie.startTimer((unsigned long int)(podlewanieDL)*1000, 12); //Start dropping line timer
                            logWrite("Forse dropping line to start");
                        };

                        if (header.indexOf("GET /11/on") >= 0) //Force cycle of watering on
                        {
                            podlewanieCykl = 1; //Set cycle of watering to on
                            logWrite("Force cycle of watering on");
                        };

                        if (header.indexOf("GET /11/off") >= 0) //Force cycle of watering off
                        {
                            podlewanieCykl = 0; //Set cycle of watering to off
                            logWrite("Force cycle of watering off");
                        };

                        if (header.indexOf("GET /10/on") >= 0) //Force autoatering on
                        {
                            podlewanieAuto = true; //Set autowatering on
                            logWrite("Force autoatering on");
                        };

                        if (header.indexOf("GET /10/off") >= 0) //Force autowatering off
                        {
                            podlewanieAuto = false; //Set autowatering off
                            logWrite("Force autowatering off");
                        };

                        if (header.indexOf("GET /9/off") >= 0) //Force downstairs heating to stop
                        {
                            odliczanie.resetTimer(9); //Just a reset timer
                            logWrite("Force downstairs heating to stop");
                        };

                        if (header.indexOf("GET /9/on") >= 0) //Forse downstairs heating start for 10 minutes (or sth :)
                        {
                            odliczanie.startTimer(600000, 9); //Add to the downstairs heating timer 10000ms
                            logWrite("Forse downstairs heating start for 10 minutes (or sth :)");
                        };

                        if (header.indexOf("GET /8/off") >= 0) //Heating downstairs in "Night" mode - lower temp.
                        {
                            ogrzewanieParterDzien = false; //Heating downstairs in "Night" mode - lower temp.
                            temperaturaParter = temperaturaNoc; //Setting up night temperature upstairs
                            logWrite("Heating downstairs in ""Night"" mode - lower temp.");
                        };

                        if (header.indexOf("GET /8/on") >= 0) //Heating downstairs in "Day" mode - higher temp.
                        {
                            ogrzewanieParterDzien = true; //Heating downstairs in "Day" mode - higher temp.
                            temperaturaParter = temperaturaDzien; //Setting up day temperature downstairs
                            logWrite("Heating downstairs in ""Day"" mode - higher temp.");
                        };

                        if (header.indexOf("GET /7/off") >= 0) //Force upstairs heating to stop
                        {
                            odliczanie.resetTimer(7); //Just a reset timer
                            logWrite("Force upstairs heating to stop");
                        };

                        if (header.indexOf("GET /7/on") >= 0) //Forse upstairs heating start for 10 minutes (or sth :)
                        {
                            odliczanie.startTimer(600000, 7); //Add to the upstairs heating timer 10000ms
                            logWrite("Forse upstairs heating start for 10 minutes (or sth :)");
                        };

                        if (header.indexOf("GET /6/off") >= 0) //Heating upstairs in "Night" mode - lower temp.
                        {
                            ogrzewaniePietroDzien = false; //Heating upstairs in "Night" mode - lower temp.
                            temperaturaPietro = temperaturaNoc; //Setting up night temperature upstairs
                            logWrite("Heating upstairs in ""Night"" mode - lower temp.");
                        };

                        if (header.indexOf("GET /6/on") >= 0) //Heating upstairs in "Day" mode - higher temp.
                        {
                            ogrzewaniePietroDzien = true; //Heating upstairs in "Day" mode - higher temp.
                            temperaturaPietro = temperaturaDzien; //Setting up day temperature upstairs
                            logWrite("Heating upstairs in ""Day"" mode - higher temp.");
                        };

                        if (header.indexOf("GET /5/on") >= 0) //Blins up
                        {
                            Serial.println("Rolety w gore");
                            rolety.blindsUp(0);
                            //odliczanie.startTimer(1500, 5); //Only 700ms  - turn off - blinds now using RF
                            logWrite("All blins up");
                        };

                        if (header.indexOf("GET /4/on") >= 0) //Blinds down
                        {
                            Serial.println("Rolety w dol");
                            rolety.blindsDown(0);
                            //odliczanie.startTimer(1500, 4); //Only 700ms  - turn off - blinds now using RF
                            logWrite("All blinds down");
                        };

                        if (header.indexOf("GET /3/on") >= 0) //Blinds stop
                        {
                            Serial.println("Rolety stop");
                            rolety.blindsStop(0);
                            //odliczanie.startTimer(1500, 3); //Only 700ms  - turn off - blinds now using RF
                            logWrite("All blinds stop");
                        };

                        if (header.indexOf("GET /2/on") >= 0) //Blins auto up and down by SunRise and SunSet
                        {
                            //Serial.println("Auto");
                            roletyAuto = true;
                            logWrite("Blins auto up and down by SunRise and SunSet");
                        };
                        if (header.indexOf("GET /2/off") >= 0) //Blins auto off
                        {
                            //Serial.println("Manual");
                            roletyAuto = false;
                            //digitalWrite(pin_BS, HIGH); - turn off - blinds now using RF
                            logWrite("Blins by sunrise and sunset auto off");
                        };

                        if (header.indexOf("GET /zegar/") >= 0)
                        { //Setup clock, clock format:                             logWrite("Setting up clock manually by /zegar/RRRR-MM-DD/HH:MM");

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
                        if (header.indexOf("GET /lines/") >= 0)
                        { //Setup clock, clock format:                             line_to_show=(int)((header.substring(11, 14)));

                            Serial.println("Number of lines to show changed to :" + (String)line_to_show);
                            logWrite("Number of lines to show changed to :" + header.substring(11, 14));
                        }
                        // Display the HTML web page
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<!DOCTYPE html><html>"
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 970 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 970 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
# 970 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 970 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 971 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 971 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<link rel=\"icon\" href=\"data:,\">"
# 971 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 971 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 974 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 974 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
# 974 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 974 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 975 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 975 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;"
# 975 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 975 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}"
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 977 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 977 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ".button2 {background-color: #77878A;}table, th, td {  border: 1px solid black;margin-left:auto;margin-right:auto;}</style></head>"
# 977 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 977 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        // Web Page Heading
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 979 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 979 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<body><H1>Home Control Ozarow V0.07B</H1>"
# 979 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 979 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 980 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 980 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<h5> Dzis jest: "
# 980 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 980 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getDate());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 982 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 982 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " "
# 982 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 982 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getWeekDay());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 984 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 984 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " godzina: "
# 984 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 984 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getTime());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 986 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 986 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " Uptime: "
# 986 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 986 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(odliczanie.leadingZero(hour(uptime)));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 988 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 988 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ":"
# 988 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 988 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(odliczanie.leadingZero(minute(uptime)));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 990 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 990 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ":"
# 990 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 990 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(odliczanie.leadingZero(second(uptime)));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 992 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 992 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " </h5>"
# 992 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 992 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 993 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 993 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<h5>Temperatura zewnetrzna: <B>"
# 993 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 993 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)temperature1);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 995 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 995 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " C</B></H5>"
# 995 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 995 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        switch (podstrona)
                        {
                        case 0: //main page
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1000 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1000 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/64/on\"><button class=\"button button\">Rolety</button></a>"
# 1000 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1000 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1001 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1001 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/65/on\"><button class=\"button button\">Timery rolet</button></a>"
# 1001 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1001 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1002 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1002 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/69/on\"><button class=\"button button\">Oswietlenie ogrodu</button></a>"
# 1002 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1002 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1003 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1003 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/66/on\"><button class=\"button button\">Podlewanie</button></a>"
# 1003 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1003 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1004 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1004 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/67/on\"><button class=\"button button\">Ogrzewanie</button></a>"
# 1004 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1004 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1005 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1005 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/62/on\"><button class=\"button button\">Odczyt parametrow</button></a>"
# 1005 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1005 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1006 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1006 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/68/on\"><button class=\"button button\">Logi</button></a></p>"
# 1006 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1006 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            break;
                        case 1:
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1009 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1009 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<H3>Rolety</H3>"
# 1009 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1009 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            /* Numery rolet, ktore nie sa uzywane, zostaly wykomentowane.

                        

                            client.println(F("<p><a href=\"/26/on\"><button class=\"button button\">Rolety 4 w gore</button></a>"));

                            client.println(F("<a href=\"/27/on\"><button class=\"button button\">W dol</button></a>"));

                            client.println(F("<a href=\"/28/on\"><button class=\"button button\">STOP!</button></a></p>"));



                            client.println(F("<p><a href=\"/35/on\"><button class=\"button button\">Rolety 7 w gore</button></a>"));

                            client.println(F("<a href=\"/36/on\"><button class=\"button button\">W dol</button></a>"));

                            client.println(F("<a href=\"/37/on\"><button class=\"button button\">STOP!</button></a></p>"));



                            client.println(F("<p><a href=\"/59/on\"><button class=\"button button\">Rolety 15 w gore</button></a>"));

                            client.println(F("<a href=\"/60/on\"><button class=\"button button\">W dol</button></a>"));

                            client.println(F("<a href=\"/61/on\"><button class=\"button button\">STOP!</button></a></p>"));

                            */
# 1025 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1025 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1025 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/47/on\"><button class=\"button button\">Rolety Mikolaj (13) w gore</button></a>"
# 1025 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1025 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1026 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1026 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/48/on\"><button class=\"button button\">W dol</button></a>"
# 1026 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1026 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1027 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1027 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/49/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1027 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1027 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1029 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1029 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/23/on\"><button class=\"button button\">Rolety Jagoda (12) w gore</button></a>"
# 1029 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1029 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1030 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1030 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/24/on\"><button class=\"button button\">W dol</button></a>"
# 1030 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1030 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1031 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1031 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/25/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1031 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1031 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1033 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1033 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/53/on\"><button class=\"button button\">Rolety lazienka pietro (11) w gore</button></a>"
# 1033 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1033 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1034 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1034 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/54/on\"><button class=\"button button\">W dol</button></a>"
# 1034 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1034 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1035 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1035 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/55/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1035 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1035 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1037 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1037 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/29/on\"><button class=\"button button\">Rolety pracownia male (10) w gore</button></a>"
# 1037 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1037 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1038 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1038 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/30/on\"><button class=\"button button\">W dol</button></a>"
# 1038 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1038 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1039 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1039 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/31/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1039 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1039 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1041 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1041 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/41/on\"><button class=\"button button\">Rolety pracownia balkon (9) w gore</button></a>"
# 1041 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1041 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1042 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1042 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/42/on\"><button class=\"button button\">W dol</button></a>"
# 1042 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1042 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1043 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1043 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/43/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1043 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1043 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1045 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1045 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/17/on\"><button class=\"button button\">Rolety w sypialni (8) w gore</button></a>"
# 1045 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1045 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1046 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1046 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/18/on\"><button class=\"button button\">W dol</button></a>"
# 1046 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1046 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1047 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1047 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/19/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1047 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1047 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1049 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1049 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/56/on\"><button class=\"button button\">Rolety salon taras slepe (7) w gore</button></a>"
# 1049 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1049 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1050 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1050 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/57/on\"><button class=\"button button\">W dol</button></a>"
# 1050 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1050 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1051 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1051 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/58/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1051 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1051 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1053 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1053 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/32/on\"><button class=\"button button\">Rolety salon taras wejsciowe (6) w gore</button></a>"
# 1053 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1053 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1054 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1054 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/33/on\"><button class=\"button button\">W dol</button></a>"
# 1054 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1054 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1055 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1055 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/34/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1055 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1055 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1057 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1057 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/44/on\"><button class=\"button button\">Rolety salon od tarasu (5) w gore</button></a>"
# 1057 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1057 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1058 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1058 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/45/on\"><button class=\"button button\">W dol</button></a>"
# 1058 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1058 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1059 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1059 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/46/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1059 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1059 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1061 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1061 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/20/on\"><button class=\"button button\">Rolety salon od ogrodu (4) w gore</button></a>"
# 1061 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1061 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1062 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1062 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/21/on\"><button class=\"button button\">W dol</button></a>"
# 1062 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1062 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1063 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1063 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/22/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1063 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1063 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1065 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1065 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/50/on\"><button class=\"button button\">Rolety kuchnia (3) w gore</button></a>"
# 1065 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1065 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1066 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1066 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/51/on\"><button class=\"button button\">W dol</button></a>"
# 1066 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1066 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1067 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1067 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/52/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1067 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1067 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1069 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1069 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/38/on\"><button class=\"button button\">Rolety wiatrolap, lazienka parter (1,2) w gore</button></a>"
# 1069 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1069 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1070 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1070 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/39/on\"><button class=\"button button\">W dol</button></a>"
# 1070 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1070 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1071 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1071 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/40/on\"><button class=\"button button\">STOP!</button></a></p>"
# 1071 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1071 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            break;
                        case 2:
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1074 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1074 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<H3>Rolety</H3>"
# 1074 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1074 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1075 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1075 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " <h5> Wschod slonca: "
# 1075 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1075 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)slonce.sunRiseHour());
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1077 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1077 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          ":"
# 1077 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1077 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)slonce.sunRiseMinute());
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1079 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1079 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "</h5><h5> Zachod slonca: "
# 1079 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1079 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)slonce.sunSetHour());
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1081 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1081 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          ":"
# 1081 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1081 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)slonce.sunSetMinute());
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1083 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1083 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " </h5><h5> Sensor swiatla: "
# 1083 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1083 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)roletyCurrentLightLevel);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1085 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1085 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " </h5>"
# 1085 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1085 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1086 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1086 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " </h5><h5> Lampy w ogrodzie: "
# 1086 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1086 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)gardenLights);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1088 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1088 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " </h5>"
# 1088 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1088 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            // Display current state, and ON/OFF buttons for GPIO 5
                            //client.println("<p>GPIO 5 - State " + output5State + "</p>");
                            // If the output5State is off, it displays the ON button

                            if (!roletyAuto)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1096 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1096 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/2/on\"><button class=\"button\">Blinds Timer Off</button></a>"
# 1096 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1096 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1100 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1100 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/2/off\"><button class=\"button button2\">Blinds Timer On</button></a>"
# 1100 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1100 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            if (!roletyLight)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1105 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1105 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/16/on\"><button class=\"button\">Light sensor off</button></a>"
# 1105 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1105 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1109 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1109 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/16/off\"><button class=\"button button2\">Light sensor on</button></a>"
# 1109 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1109 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1112 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1112 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/15/on\"><button class=\"button\">Set light level:"
# 1112 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1112 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)roletySetLightLevel);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1114 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1114 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "</button></a></p>"
# 1114 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1114 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            if (!odliczanie.checkTimer(5)) //Blins up button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1118 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1118 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/5/on\"><button class=\"button\">Podnies rolety</button></a>"
# 1118 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1118 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1122 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1122 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a>"
# 1122 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1122 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            // Display current state, and ON/OFF buttons for GPIO 4
                            //client.println("<p>GPIO 4 - State " + output4State + "</p>");
                            // If the output4State is off, it displays the ON button
                            if (!odliczanie.checkTimer(4)) //Blins down button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1130 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1130 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/4/on\"><button class=\"button\">Opusc rolety</button></a></p>"
# 1130 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1130 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1134 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1134 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a></p>"
# 1134 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1134 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };
                            break;
                        case 3:
                            //--------------Podlewanie:
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1139 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1139 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<H3>Podlewanie</H3>"
# 1139 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1139 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            if (podlewanieRainSensor)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1142 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1142 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><input type=" "checkbox" " onclick=" "return false" " checked> Deszcz"
# 1142 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1142 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              ))



                                                                    );
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1150 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1150 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><input type=" "checkbox" " onclick=" "return false" " unchecked> Deszcz"
# 1150 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1150 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              ))



                                                                      );
                            }

                            if (podlewanieAuto)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1159 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1159 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/10/off\"><button class=\"button button2\">Podlewanie Auto</button></a>"
# 1159 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1159 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1163 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1163 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/10/on\"><button class=\"button \">Podlewanie Reczne</button></a>"
# 1163 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1163 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            if (podlewanieCykl == 0)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1168 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1168 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/11/on\"><button class=\"button\">Start cyklu podlewania</button></a>"
# 1168 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1168 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1172 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1172 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/11/off\"><button class=\"button button2\">Cykl podlewanie w trakcie, nadzisnij by zatrzymac</button></a>"
# 1172 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1172 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            if (!odliczanie.checkTimer(12))
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1177 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1177 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/12/on\"><button class=\"button\">Start LK</button></a>"
# 1177 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1177 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1181 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1181 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/12/off\"><button class=\"button button2\">Stop LK za "
# 1181 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1181 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(12));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1183 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1183 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a>"
# 1183 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1183 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            if (!odliczanie.checkTimer(13))
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1188 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1188 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/13/on\"><button class=\"button\">Start Z1</button></a>"
# 1188 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1188 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1192 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1192 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/13/off\"><button class=\"button button2\">Stop Z1 za "
# 1192 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1192 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(13));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1194 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1194 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a>"
# 1194 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1194 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            if (!odliczanie.checkTimer(14))
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1199 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1199 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/14/on\"><button class=\"button\">Start Z2</button></a></p>"
# 1199 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1199 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1203 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1203 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/14/off\"><button class=\"button button2\">Stop Z2 za "
# 1203 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1203 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(14));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1205 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1205 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a></p>"
# 1205 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1205 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            break;
                        case 4:
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1210 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1210 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<H3>Ogrzewanie</H3>"
# 1210 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1210 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
# 1219 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1219 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/6/off\"><button class=\"button\">Pietro "
# 1219 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1219 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((String)temperature3);
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1221 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1221 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "'C dzien</button></a>"
# 1221 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1221 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else //Upstairs night mode button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1225 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1225 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/6/on\"><button class=\"button button2\">Pietro "
# 1225 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1225 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((String)temperature3);
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1227 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1227 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "'C noc</button></a>"
# 1227 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1227 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            if (!odliczanie.checkTimer(7)) //Force heat upstairs button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1232 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1232 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/7/on\"><button class=\"button\">10 min. On</button></a></p>"
# 1232 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1232 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1236 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1236 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/7/on\"><button class=\"button button2\">"
# 1236 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1236 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(7));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1238 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1238 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a>"
# 1238 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1238 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1239 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1239 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/7/off\"><button class=\"button\">STOP!</button></a></p>"
# 1239 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1239 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };
                            //----------------Ogrzewanie Parter
                            if (ogrzewanieParterDzien) //Downstairs day mode button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1244 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1244 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/8/off\"><button class=\"button\">Parter "
# 1244 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1244 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((String)temperature2);
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1246 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1246 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "'C dzien</button></a>"
# 1246 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1246 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else //Downstairs night mode button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1250 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1250 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/8/on\"><button class=\"button button2\">Parter "
# 1250 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1250 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((String)temperature2);
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1252 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1252 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "'C noc</button></a>"
# 1252 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1252 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            if (!odliczanie.checkTimer(9))
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1257 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1257 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/9/on\"><button class=\"button\">10 min. On</button></a></p>"
# 1257 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1257 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1261 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1261 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/9/on\"><button class=\"button button2\">"
# 1261 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1261 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(9));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1263 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1263 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a>"
# 1263 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1263 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1264 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1264 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/9/off\"><button class=\"button\">STOP!</button></a></p>"
# 1264 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1264 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1267 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1267 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<h5>Temperatura dzienna: "
# 1267 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1267 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperaturaDzien);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1269 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1269 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " C</H5><H5>Temperatura nocna: "
# 1269 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1269 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperaturaNoc);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1271 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1271 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " C</H5>"
# 1271 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1271 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            break;
                        case 5: //Show parameters sub-page
                            client.println("<H5>Parametry:</H5>");
                            logging = SD.open("params.txt"); //open parametrs file
                            if (logging)
                            {
                                Serial.println("Parameters:");
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1280 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1280 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<table style=" "background-color:#FFFFFF" "><tr><th>No.</th><th>Data</th><th>Godzina</th><th>Osw.</th><th>Temp1</th><th>Temp2</th><th>Temp3</th><th>Deszcz</th><th>Osw. ogr.</th></tr>"
# 1280 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1280 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              ))

                                                                                                                                                                                               );
                                // read from the file until there's nothing else in it:
                                unsigned long end_file_position = 0; //Position in File
                                unsigned long current_file_position = 0; //Position in File
                                unsigned long beginning_line_position = 0; //Position in File

                                int line = 1; //Row counts in table
                                char ch = 0; //Char to show
                                while (logging.available())
                                    logging.read(); //Go to the end of file
                                end_file_position = logging.position(); //Remember the last position
                                current_file_position = end_file_position - 2; //Set the current position to the end of file
                                while (line <= line_to_show) //Show number of lines stored in variable
                                {
                                    client.println("<tr><td>" + (String)line + "</td><td>");
                                    //Find the beginning of the line (of beginning of file):
                                    while (((int)ch != 13))
                                    {
                                        current_file_position--; //move back to the file by two positions
                                        logging.seek(current_file_position); //Move to current position
                                        if (logging.available())
                                        {
                                            ch = logging.read(); //Read the char, if the file is still available
                                        }
                                        else
                                            break;
                                    }
                                    beginning_line_position = current_file_position; //Remeber the line beginning position
                                    current_file_position = current_file_position + 2; //Go to the next char (current is 13, missing the next loop, another is 10)
                                    logging.seek(current_file_position);
                                    if (logging.available())
                                        ch = logging.read(); //Move to next char - not chr13 :)
                                    //Read the line from beginning to the end of line or end of file:
                                    //Serial.print("Move 3 forward:" + (String)current_file_position + " Char:" + ch + " ");
                                    //Serial.println(ch, DEC);
                                    while ((int)ch != 13) //if we have the beginning of the line
                                    {
                                        if (logging.available())
                                        {
                                            ch = logging.read(); //Read the char, if the file is still available
                                        }
                                        else
                                            break;
                                        switch ((int)ch)
                                        {
                                        case 59:
                                            client.write("</td><td>");
                                            break;
                                        default:
                                            client.write(ch);
                                            break;
                                        }
                                        logging.seek(current_file_position);

                                        current_file_position++; //Go to the next char, till found 13 - loop break
                                        //Serial.print("Display loop, EOF marker:" + (String)current_file_position + " Char:" + ch + " " + char(ch) + " ");
                                        //Serial.println(ch, DEC);
                                    }
                                    //Change the line to next
                                    client.write("</td></tr>");
                                    line++;
                                    current_file_position = beginning_line_position; //Move cursor to the beginning of the line
                                    beginning_line_position = beginning_line_position - 3;
                                    if (logging.available())
                                    {
                                        ch = logging.read(); //Read the char, if the file is still available
                                    }
                                }
                                client.println("</td></tr></table>");
                                // close the file:
                                logging.close();
                            }
                            else
                            {
                                // if the file didn't open, print an error:
                                client.println("error opening params.txt");
                            }
                            break;
                        case 6:
                            //show log:
                            client.println("<H5>Logi:</H5>");
                            logging = SD.open("logs.txt"); //open parametrs file
                            if (logging)
                            {
                                Serial.println("Parameters:");
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1367 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1367 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<table style=" "background-color:#FFFFFF" "><tr><th>No.</th><th>Data</th><th>Godzina</th><th>Info</th></tr>"
# 1367 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1367 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              ))

                                                                                                                    );
                                // read from the file until there's nothing else in it:
                                unsigned long end_file_position = 0; //Position in File
                                unsigned long current_file_position = 0; //Position in File
                                unsigned long beginning_line_position = 0; //Position in File

                                int line = 1; //Row counts in table
                                char ch = 0; //Char to show
                                while (logging.available())
                                    logging.read(); //Go to the end of file
                                end_file_position = logging.position(); //Remember the last position
                                current_file_position = end_file_position - 2; //Set the current position to the end of file
                                while (line <= line_to_show) //Show number of lines stored in variable
                                {
                                    client.println("<tr><td>" + (String)line + "</td><td>");
                                    //Find the beginning of the line (of beginning of file):
                                    while (((int)ch != 13))
                                    {
                                        current_file_position--; //move back to the file by two positions
                                        logging.seek(current_file_position); //Move to current position
                                        if (logging.available())
                                        {
                                            ch = logging.read(); //Read the char, if the file is still available
                                        }
                                        else
                                            break;
                                    }
                                    beginning_line_position = current_file_position; //Remeber the line beginning position
                                    current_file_position = current_file_position + 2; //Go to the next char (current is 13, missing the next loop, another is 10)
                                    logging.seek(current_file_position);
                                    if (logging.available())
                                        ch = logging.read(); //Move to next char - not chr13 :)
                                    //Read the line from beginning to the end of line or end of file:
                                    //Serial.print("Move 3 forward:" + (String)current_file_position + " Char:" + ch + " ");
                                    //Serial.println(ch, DEC);
                                    while ((int)ch != 13) //if we have the beginning of the line
                                    {
                                        if (logging.available())
                                        {
                                            ch = logging.read(); //Read the char, if the file is still available
                                        }
                                        else
                                            break;
                                        switch ((int)ch)
                                        {
                                        case 59:
                                            client.write("</td><td>");
                                            break;
                                        default:
                                            client.write(ch);
                                            break;
                                        }
                                        logging.seek(current_file_position);

                                        current_file_position++; //Go to the next char, till found 13 - loop break
                                        //Serial.print("Display loop, EOF marker:" + (String)current_file_position + " Char:" + ch + " " + char(ch) + " ");
                                        //Serial.println(ch, DEC);
                                    }
                                    //Change the line to next
                                    client.write("</td></tr>");
                                    line++;
                                    current_file_position = beginning_line_position; //Move cursor to the beginning of the line
                                    beginning_line_position = beginning_line_position - 3;
                                    if (logging.available())
                                    {
                                        ch = logging.read(); //Read the char, if the file is still available
                                    }
                                }
                                client.println("</td></tr></table>");
                                // close the file:
                                logging.close();
                            }
                            else
                            {
                                // if the file didn't open, print an error:
                                client.println("error opening logs.txt");
                            }
//------------------------------------------------------------------------------------------
                            /*client.println("<H5>Logi:</H5>");

                            logging = SD.open("log.txt"); //open parametrs file

                            if (logging)

                            {

                                Serial.println("Logs:");

                                // read from the file until there's nothing else in it:

                                int line = 0;

                                char ch;

                                //while (logging.available())

                                while (line < 100)

                                {

                                    if (logging.available())

                                        ch = logging.read();

                                    switch ((int)ch)

                                    {

                                    case 13:

                                        client.println("<br>");

                                        line++;

                                        break;

                                    default:

                                        client.write(ch);

                                        break;

                                    }

                                }

                                // close the file:

                                logging.close();

                            }

                            else

                            {

                                // if the file didn't open, print an error:

                                client.println("error opening logs.txt");

                            }*/
# 1479 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                            break;
                        case 7: //Garden lights section
                            if (gardenLightsAuto)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1483 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1483 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/70/off\"><button class=\"button button2\">osw. ogrodu Auto</button></a>"
# 1483 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1483 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1487 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1487 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/70/on\"><button class=\"button \">osw. ogrodu Reczne</button></a>"
# 1487 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1487 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            if (gardenLights)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1491 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1491 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/71/off\"><button class=\"button button2\">On</button></a>"
# 1491 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1491 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1495 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1495 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/71/on\"><button class=\"button \">Off</button></a>"
# 1495 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1495 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            break;
                        }
                        //client.println(F("<a href=\"/63/on\"><button class=\"button button2\">Back to main page</button></a></p>"));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1501 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1501 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/nothing\"><button class=\"button button2\">Refresh </button></a></p>"
# 1501 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 1501 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        //-------------Koniec
                        client.println();
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1504 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1504 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "</body></html>"
# 1504 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 1504 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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

    if (gardenLights) //Check, if garden lights should be on
    {
        digitalWrite(24 /*Garden lights output*/, 0x0);
    }
    else
    {
        digitalWrite(24 /*Garden lights output*/, 0x1);
    }

    if (podlewanieCykl == 1) //Starting watering cycle
    {
        logWrite("Watering started in the cycle - 1 - dropping line");
        odliczanie.startTimer((unsigned long int)podlewanieDL * 1000, 12);
        podlewanieCykl = 2;
    };
    if ((!odliczanie.checkTimer(12)) && (podlewanieCykl == 2))
    {
        logWrite("Watering started in the cycle - 2 - Z1");
        odliczanie.startTimer((unsigned long int)podlewanieZ1 * 1000, 13);
        podlewanieCykl = 3;
    };
    if ((!odliczanie.checkTimer(13)) && (podlewanieCykl == 3))
    {
        logWrite("Watering started in the cycle - 3 - Z2");
        odliczanie.startTimer((unsigned long int)podlewanieZ2 * 1000, 14);
        podlewanieCykl = 0;
    }

    //----------------------------------------//
    //              Trigger BLOCK             //
    //----------------------------------------//

    if ((trigger(1)) && (roletyAuto)) //Auto blins up if it is on
    {
        Serial.println("Rolety w gore by trigger");
        logWrite("Auto all blinds up by trigger");
        rolety.blindsUp(0); //Rolety w gore
    };
    if ((trigger(2)) && (roletyAuto)) //Auto blins up if it is on
    {
        Serial.println("Rolety w dol by trigger");
        logWrite("Auto all blinds down by trigger");
        rolety.blindsDown(0); //Rolety w dol
    };

    if (trigger(3) && (podlewanieAuto) && (!podlewanieRainSensor)) //Auto watering is inside the trigger, and we have no rain :)
    {
        Serial.println("Starutjemy cykl podlewania");
        logWrite("Auto watering, and we have no rain :)");
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
    podlewanieRainSensor = digitalRead(30 /*Rain sensor input*/); // Check rain sensor
    if (!odliczanie.checkTimer(0)) //uptime is flowing
    {
        uptime++;
        odliczanie.resetTimer(0);
        odliczanie.startTimer(1000, 0);
    };
    if (!odliczanie.checkTimer(1)) //write parameters every 5 minutes
    {
        Serial.println("Writing parameters...");
        odliczanie.resetTimer(1);
        odliczanie.startTimer(300000, 1);

        logging = SD.open("params.txt", (O_READ | O_WRITE | O_CREAT | O_APPEND)); //Open log file in SD card

        if (!logging)
            Serial.println("Error opening param.txt"); //Write about opening errors, if any.
        logging.println(zegar.getDate() + ";" + zegar.getTime() + ";" + (String)roletyCurrentLightLevel + ";" + (String)temperature1 + ";" + (String)temperature2 + ";" + (String)temperature3 + ";" + (String)podlewanieRainSensor + ";" + (String)gardenLights);
        Serial.println(zegar.getDate() + ";" + zegar.getTime() + ";" + (String)roletyCurrentLightLevel + ";" + (String)temperature1 + ";" + (String)temperature2 + ";" + (String)temperature3 + ";" + (String)podlewanieRainSensor + ";" + (String)gardenLights);
        logging.close();
    };
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
            odliczanie.startTimer(18000000, 15); //Locking blinds light trigger to start one time.
            logWrite("Auto blinds down - SunRise");
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
            logWrite("Auto blinds up - SunSet");
            return true;
        }
        else
        {
            if (zegar.getSecond() != 0)
                lockTrigger[number] = false; //Unlocking trigger becouse second not zero - could be start again
        }

        if ((roletyCurrentLightLevel == roletySetLightLevel) && (!odliczanie.checkTimer(15)) && (roletyLight)) //If it is dark
        {
            odliczanie.startTimer(3600000, 15); //Locking trigger to start one time.
            logWrite("Auto blinds down - low light level");
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
            logWrite("Auto watering - it is time to do it :)");
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
        //Serial.println((slonce.sunSetMinute()+gardenLightsMinutesOffset) % 60);
        if (((zegar.getHour() == slonce.sunSetHour())||(zegar.getHour() == slonce.sunSetHour()+1)) && (zegar.getMinute() == ((slonce.sunSetMinute()+gardenLightsMinutesOffset) % 60)) && (zegar.getSecond() == 0) && (!gardenLights) && (gardenLightsAuto)) //If we have 5 minutes after Sun set and gardenLights are off:)
        {
            gardenLights = true;
            logWrite("Auto gardenLights on - 10 minutes after SunSet");
            return true;
        }
        return false;
        break;
    case 5:
        if ((zegar.getHour() == gardenLightsOffHour) && (zegar.getMinute() == gardenLightsOffMinute) && (zegar.getSecond() == 0) && (gardenLights) && (gardenLightsAuto)) //If we have 10 minutes past 2AM :)
        {
            gardenLights = false;
            logWrite("Auto gardenLights off - it is time to do it :)");
            return true;
        }
        return false;
        break;
    }
}

void sendNTPpacket(const char *address)
{
    // set all bytes in the buffer to 0
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    // Initialize values needed to form NTP request
    // (see URL above for details on the packets)
    packetBuffer[0] = 0b11100011; // LI, Version, Mode
    packetBuffer[1] = 0; // Stratum, or type of clock
    packetBuffer[2] = 6; // Polling Interval
    packetBuffer[3] = 0xEC; // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12] = 49;
    packetBuffer[13] = 0x4E;
    packetBuffer[14] = 49;
    packetBuffer[15] = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    Udp.beginPacket(address, 123); // NTP requests are to port 123
    Udp.write(packetBuffer, NTP_PACKET_SIZE);
    Udp.endPacket();
}

boolean logWrite(String info)
{
    Serial.println("Writing log info...");
    logging = SD.open("logs.txt", (O_READ | O_WRITE | O_CREAT | O_APPEND)); //Open log file in SD card
    if (!logging)
    {
        Serial.println("Error opening logs.txt"); //Write about opening errors, if any.
        return false;
    }
    logging.println(zegar.getDate() + ";" + zegar.getTime() + ";" + info);
    Serial.println(zegar.getDate() + ";" + zegar.getTime() + ";" + info);
    logging.close();
    zegar.Flow();
    return true;
}
