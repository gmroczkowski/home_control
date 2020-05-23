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
# 68 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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

# 97 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2
# 98 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 2

sunState slonce;
Blinds rolety;

// Auxiliar variables to store the current output state
boolean roletyAuto = true;

boolean roletyLight = true; //Turn on blinds down by light
int roletySetLightLevel = 600; //Set light level
int roletyCurrentLightLevel = 600; //Set light level

boolean gardenLights = false; //Garden lights variable
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
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
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

File logging;
File blinds;
File watering;
File heating;
File parameters;

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

    Serial.begin(115200); //Starting serial

    Serial.print("Initializing SD card...");

    if (!SD.begin(4))
    {
        Serial.println("initialization failed!");
    }
    else
        Serial.println("initialization done.");

    logging = SD.open("log.txt", (O_READ | O_WRITE | O_CREAT | O_APPEND)); //Open log file in SD card

    if (!logging)
        Serial.println("Error opening log.txt"); //Write about opening errors, if any.

    logging.println("Starting...");

    for (int i = 0; i < 5; i++)
        lockTrigger[i] = false;

    logging.println("Initializing Dallas temperatu sensors...");

    //Dallas temperature sensors:
    sensors.begin(12); //resolution 12
    sensors.request(); //request all the sensors
    // Open serial communications and wait for port to open:

    logging.println("Initializing timelord SunRise and SunSet library...");

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

    logging.println("Initializing Ethernet connection...");
    //start the Ethernet connection and the server :
    Ethernet.begin(mac, ip, dns, gateway, subnet);

    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware)
    {
        logging.println("No ethernet expansion shield");
    }
    if (Ethernet.linkStatus() == LinkOFF)
    {
        Serial.println("Ethernet cable is not connected.");
        logging.println("Ethernet cable is not connected.");
    }

    Udp.begin(localPort); //Start UDP
    logging.print("IP:");
    logging.print(Ethernet.localIP());
    logging.print(" DNS server ip: ");
    logging.print(Ethernet.dnsServerIP());
    logging.print(" Gateway ip: ");
    logging.print(Ethernet.gatewayIP());
    logging.print(" Subnet mask:");
    logging.println(Ethernet.subnetMask());

    Serial.print("IP:");
    Serial.print(Ethernet.localIP());
    Serial.print(" DNS server ip: ");
    Serial.print(Ethernet.dnsServerIP());
    Serial.print(" gateway ip: ");
    Serial.print(Ethernet.gatewayIP());
    Serial.print(" Subnet mask:");
    Serial.println(Ethernet.subnetMask());

    logging.println("Getting the current Date and Time from NTP server...");
    Serial.println("Getting the current Date and Time from NTP server...");
    sendNTPpacket(timeServer); // send an NTP packet to a time server
    odliczanie.startTimer(60000, 0); // timer 60s for timeout of NTP receive packet
    unsigned long epoch = 0; //unic time
    while (!Udp.parsePacket())
    {
        if (!odliczanie.checkTimer(0))
        {
            Serial.println("NTP timeout. Setting any time.");
            logging.println("NTP timeout. Setting any time.");
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
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    if (!epoch)
        epoch = secsSince1900 - seventyYears; //if no timeout on getting NTP info
    // print Unix time:
    Serial.println(epoch);
    time_t utcCalc = epoch;
    Serial.println("Date:" + (String)year(utcCalc) + "-" + (String)month(utcCalc) + "-" + (String)day(utcCalc) + " " + (String)hour(utcCalc) + ":" + (String)minute(utcCalc));
    logging.println("Date:" + (String)year(utcCalc) + "-" + (String)month(utcCalc) + "-" + (String)day(utcCalc) + " " + (String)hour(utcCalc) + ":" + (String)minute(utcCalc));

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
    Serial.println(Ethernet.localIP());
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

                        if (header.indexOf("GET /67/on") >= 0) //Show heating page
                        {
                            podstrona = 4; //Set content parameter to 4 - heating
                        };

                        if (header.indexOf("GET /66/on") >= 0) //Show watering page
                        {
                            podstrona = 3; //Set content parameter to 3 - watering
                        };

                        if (header.indexOf("GET /65/on") >= 0) //Show blinds page
                        {
                            podstrona = 2; //Set content parameter to 2 - blinds settings
                        };

                        if (header.indexOf("GET /64/on") >= 0) //Show blinds page
                        {
                            podstrona = 1; //Set content parameter to 1 - blinds
                        };

                        if (header.indexOf("GET /63/on") >= 0) //Show main page
                        {
                            podstrona = 0; //Set content variable to 0 - main page
                        };

                        if (header.indexOf("GET /62/on") >= 0) //Rolety lazienka gora stop
                        {
                            podstrona = 5; //Rolety lazienka gora stop
                        };

                        if (header.indexOf("GET /61/on") >= 0) //Rolety lazienka gora stop
                        {
                            rolety.blindsStop(15); //Rolety lazienka gora stop
                        };

                        if (header.indexOf("GET /60/on") >= 0) //Rolety lazienka gora up
                        {
                            rolety.blindsDown(15); //Rolety lazienka gora up
                        };

                        if (header.indexOf("GET /59/on") >= 0) //Rolety lazienka gora down
                        {
                            rolety.blindsUp(15); //Rolety lazienka gora down
                        };

                        if (header.indexOf("GET /58/on") >= 0) //Rolety lazienka gora stop
                        {
                            rolety.blindsStop(14); //Rolety lazienka gora stop
                        };

                        if (header.indexOf("GET /57/on") >= 0) //Rolety lazienka gora up
                        {
                            rolety.blindsDown(14); //Rolety lazienka gora up
                        };

                        if (header.indexOf("GET /56/on") >= 0) //Rolety lazienka gora down
                        {
                            rolety.blindsUp(14); //Rolety lazienka gora down
                        };

                        if (header.indexOf("GET /55/on") >= 0) //Rolety lazienka gora stop
                        {
                            rolety.blindsStop(13); //Rolety lazienka gora stop
                        };

                        if (header.indexOf("GET /54/on") >= 0) //Rolety lazienka gora up
                        {
                            rolety.blindsDown(13); //Rolety lazienka gora up
                        };

                        if (header.indexOf("GET /53/on") >= 0) //Rolety lazienka gora down
                        {
                            rolety.blindsUp(13); //Rolety lazienka gora down
                        };

                        if (header.indexOf("GET /52/on") >= 0) //Rolety kuchnia stop
                        {
                            rolety.blindsStop(12); //Rolety kuchnia stop
                        };

                        if (header.indexOf("GET /51/on") >= 0) //Rolety kuchnia up
                        {
                            rolety.blindsDown(12); //Rolety kuchnia up
                        };

                        if (header.indexOf("GET /50/on") >= 0) //Rolety kuchnia down
                        {
                            rolety.blindsUp(12); //Rolety kuchnia down
                        };

                        if (header.indexOf("GET /49/on") >= 0) //Rolety Mikolaj stop
                        {
                            rolety.blindsStop(11); //Rolety Mikolaj stop
                        };

                        if (header.indexOf("GET /48/on") >= 0) //Rolety Mikolaj up
                        {
                            rolety.blindsDown(11); //Rolety Mikolaj up
                        };

                        if (header.indexOf("GET /47/on") >= 0) //Rolety Mikolaj down
                        {
                            rolety.blindsUp(11); //Rolety Mikolaj down
                        };

                        if (header.indexOf("GET /46/on") >= 0) //Rolety salon taras male stop
                        {
                            rolety.blindsStop(10); //Rolety salon taras male stop
                        };

                        if (header.indexOf("GET /45/on") >= 0) //Rolety salon taras male up
                        {
                            rolety.blindsDown(10); //Rolety salon taras male up
                        };

                        if (header.indexOf("GET /44/on") >= 0) //Rolety salon taras male down
                        {
                            rolety.blindsUp(10); //Rolety salon taras male down
                        };

                        if (header.indexOf("GET /43/on") >= 0) //Rolety pracownia balkon stop
                        {
                            rolety.blindsStop(9); //Rolety pracownia balkon stop
                        };

                        if (header.indexOf("GET /42/on") >= 0) //Rolety pracownia balkon up
                        {
                            rolety.blindsDown(9); //Rolety pracownia balkon up
                        };

                        if (header.indexOf("GET /41/on") >= 0) //Rolety pracownia balkon down
                        {
                            rolety.blindsUp(9); //Rolety pracownia balkon down
                        };

                        if (header.indexOf("GET /40/on") >= 0) //Rolety lazienka dol, wiatrolap stop
                        {
                            rolety.blindsStop(8); //Rolety lazienka dol, wiatrolap stop
                        };

                        if (header.indexOf("GET /39/on") >= 0) //Rolety lazienka dol, wiatrolap up
                        {
                            rolety.blindsDown(8); //Rolety lazienka dol, wiatrolap up
                        };

                        if (header.indexOf("GET /38/on") >= 0) //Rolety lazienka dol, wiatrolap down
                        {
                            rolety.blindsUp(8); //Rolety lazienka dol, wiatrolap down
                        };

                        if (header.indexOf("GET /37/on") >= 0) //Rolety ? stop
                        {
                            rolety.blindsStop(7); //Rolety ? stop
                        };

                        if (header.indexOf("GET /36/on") >= 0) //Rolety ? up
                        {
                            rolety.blindsDown(7); //Rolety ? up
                        };

                        if (header.indexOf("GET /35/on") >= 0) //Rolety ? down
                        {
                            rolety.blindsUp(7); //Rolety ? down
                        };

                        if (header.indexOf("GET /34/on") >= 0) //Rolety salon taras wyjscie stop
                        {
                            rolety.blindsStop(6); //Rolety salon taras wyjscie stop
                        };

                        if (header.indexOf("GET /33/on") >= 0) //Rolety salon taras wyjscie up
                        {
                            rolety.blindsDown(6); //Rolety salon taras wyjscie up
                        };

                        if (header.indexOf("GET /32/on") >= 0) //Rolety salon taras wyjscie down
                        {
                            rolety.blindsUp(6); //Rolety salon taras wyjscie down
                        };

                        if (header.indexOf("GET /31/on") >= 0) //Rolety pracownia male okno stop
                        {
                            rolety.blindsStop(5); //Rolety pracownia male okno stop
                        };

                        if (header.indexOf("GET /30/on") >= 0) //Rolety pracownia male okno up
                        {
                            rolety.blindsDown(5); //Rolety pracownia male okno up
                        };

                        if (header.indexOf("GET /29/on") >= 0) //Rolety pracownia male okno down
                        {
                            rolety.blindsUp(5); //Rolety pracownia male okno down
                        };

                        if (header.indexOf("GET /28/on") >= 0) //Rolety ? stop
                        {
                            rolety.blindsStop(4); //Rolety ? stop
                        };

                        if (header.indexOf("GET /27/on") >= 0) //Rolety ? up
                        {
                            rolety.blindsDown(4); //Rolety ? up
                        };

                        if (header.indexOf("GET /26/on") >= 0) //Rolety ? down
                        {
                            rolety.blindsUp(4); //Rolety ? down
                        };

                        if (header.indexOf("GET /25/on") >= 0) //Rolety Jagoda stop
                        {
                            rolety.blindsStop(3); //Rolety Jagoda stop
                        };

                        if (header.indexOf("GET /24/on") >= 0) //Rolety Jagoda up
                        {
                            rolety.blindsDown(3); //Rolety Jagoda up
                        };

                        if (header.indexOf("GET /23/on") >= 0) //Rolety Jagoda down
                        {
                            rolety.blindsUp(3); //Rolety Jagoda down
                        };

                        if (header.indexOf("GET /22/on") >= 0) //Rolety salon ogrod stop
                        {
                            rolety.blindsStop(2); //Rolety salon ogrod stop
                        };

                        if (header.indexOf("GET /21/on") >= 0) //Rolety salon ogrod up
                        {
                            rolety.blindsDown(2); //Rolety salon ogrod up
                        };

                        if (header.indexOf("GET /20/on") >= 0) //Rolety salon ogrod down
                        {
                            rolety.blindsUp(2); //Rolety salon ogrod down
                        };

                        if (header.indexOf("GET /19/on") >= 0) //Rolety sypialnia stop
                        {
                            rolety.blindsStop(1); //Rolety sypialnia stop
                        };

                        if (header.indexOf("GET /18/on") >= 0) //Rolety sypialnia up
                        {
                            rolety.blindsDown(1); //Rolety sypialnia up
                        };

                        if (header.indexOf("GET /17/on") >= 0) //Rolety sypialnia down
                        {
                            rolety.blindsUp(1); //Rolety sypialnia down
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
                            roletySetLightLevel = roletyCurrentLightLevel;
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
# 793 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
# 816 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
# 861 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 861 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<!DOCTYPE html><html>"
# 861 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 861 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 862 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 862 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
# 862 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 862 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 863 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 863 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<link rel=\"icon\" href=\"data:,\">"
# 863 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 863 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        // CSS to style the on/off buttons
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 866 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 866 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}"
# 866 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 866 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 867 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 867 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;"
# 867 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 867 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 868 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 868 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}"
# 868 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 868 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 869 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 869 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ".button2 {background-color: #77878A;}</style></head>"
# 869 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 869 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        // Web Page Heading
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 871 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 871 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<body><H1>Home Control Domek Ozarow V0.07A</H1>"
# 871 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 871 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 872 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 872 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<h5> Dzis jest: "
# 872 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 872 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getDate());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 874 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 874 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " "
# 874 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 874 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getWeekDay());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 876 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 876 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " godzina: "
# 876 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 876 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(zegar.getTime());
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 878 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 878 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " Uptime: "
# 878 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 878 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(odliczanie.leadingZero(hour(uptime)));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 880 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 880 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ":"
# 880 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 880 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(odliczanie.leadingZero(minute(uptime)));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 882 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 882 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      ":"
# 882 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 882 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println(odliczanie.leadingZero(second(uptime)));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 884 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 884 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " </h5>"
# 884 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 884 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 885 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 885 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<h5>Temperatura zewnetrzna: <B>"
# 885 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 885 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        client.println((String)temperature1);
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 887 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 887 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      " C</B></H5>"
# 887 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 887 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));

                        switch (podstrona)
                        {
                        case 0: //main page
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 892 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 892 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/64/on\"><button class=\"button button\">Rolety</button></a>"
# 892 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 892 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 893 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 893 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/65/on\"><button class=\"button button\">Timery rolet</button></a>"
# 893 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 893 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 894 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 894 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/66/on\"><button class=\"button button\">Podlewanie</button></a>"
# 894 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 894 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 895 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 895 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/67/on\"><button class=\"button button\">Ogrzewanie</button></a>"
# 895 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 895 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 896 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 896 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/62/on\"><button class=\"button button\">Odczyt parametrow</button></a></p>"
# 896 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 896 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            break;
                        case 1:
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 899 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 899 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<H3>Rolety</H3>"
# 899 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 899 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
# 915 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 915 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 915 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/47/on\"><button class=\"button button\">Rolety Mikolaj (13) w gore</button></a>"
# 915 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 915 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 916 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 916 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/48/on\"><button class=\"button button\">W dol</button></a>"
# 916 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 916 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 917 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 917 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/49/on\"><button class=\"button button\">STOP!</button></a></p>"
# 917 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 917 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 919 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 919 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/23/on\"><button class=\"button button\">Rolety Jagoda (12) w gore</button></a>"
# 919 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 919 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 920 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 920 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/24/on\"><button class=\"button button\">W dol</button></a>"
# 920 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 920 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 921 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 921 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/25/on\"><button class=\"button button\">STOP!</button></a></p>"
# 921 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 921 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 923 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 923 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/53/on\"><button class=\"button button\">Rolety lazienka pietro (11) w gore</button></a>"
# 923 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 923 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 924 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 924 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/54/on\"><button class=\"button button\">W dol</button></a>"
# 924 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 924 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 925 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 925 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/55/on\"><button class=\"button button\">STOP!</button></a></p>"
# 925 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 925 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 927 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 927 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/29/on\"><button class=\"button button\">Rolety pracownia male (10) w gore</button></a>"
# 927 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 927 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 928 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 928 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/30/on\"><button class=\"button button\">W dol</button></a>"
# 928 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 928 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 929 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 929 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/31/on\"><button class=\"button button\">STOP!</button></a></p>"
# 929 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 929 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 931 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 931 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/41/on\"><button class=\"button button\">Rolety pracownia balkon (9) w gore</button></a>"
# 931 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 931 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 932 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 932 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/42/on\"><button class=\"button button\">W dol</button></a>"
# 932 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 932 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 933 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 933 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/43/on\"><button class=\"button button\">STOP!</button></a></p>"
# 933 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 933 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 935 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 935 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/17/on\"><button class=\"button button\">Rolety w sypialni (8) w gore</button></a>"
# 935 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 935 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 936 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 936 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/18/on\"><button class=\"button button\">W dol</button></a>"
# 936 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 936 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 937 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 937 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/19/on\"><button class=\"button button\">STOP!</button></a></p>"
# 937 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 937 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 939 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 939 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/56/on\"><button class=\"button button\">Rolety salon taras slepe (7) w gore</button></a>"
# 939 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 939 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 940 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 940 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/57/on\"><button class=\"button button\">W dol</button></a>"
# 940 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 940 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 941 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 941 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/58/on\"><button class=\"button button\">STOP!</button></a></p>"
# 941 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 941 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 943 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 943 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/32/on\"><button class=\"button button\">Rolety salon taras wejsciowe (6) w gore</button></a>"
# 943 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 943 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 944 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 944 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/33/on\"><button class=\"button button\">W dol</button></a>"
# 944 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 944 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 945 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 945 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/34/on\"><button class=\"button button\">STOP!</button></a></p>"
# 945 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 945 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 947 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 947 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/44/on\"><button class=\"button button\">Rolety salon od tarasu (5) w gore</button></a>"
# 947 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 947 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 948 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 948 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/45/on\"><button class=\"button button\">W dol</button></a>"
# 948 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 948 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 949 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 949 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/46/on\"><button class=\"button button\">STOP!</button></a></p>"
# 949 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 949 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 951 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 951 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/20/on\"><button class=\"button button\">Rolety salon od ogrodu (4) w gore</button></a>"
# 951 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 951 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 952 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 952 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/21/on\"><button class=\"button button\">W dol</button></a>"
# 952 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 952 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 953 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 953 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/22/on\"><button class=\"button button\">STOP!</button></a></p>"
# 953 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 953 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 955 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 955 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/50/on\"><button class=\"button button\">Rolety kuchnia (3) w gore</button></a>"
# 955 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 955 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 956 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 956 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/51/on\"><button class=\"button button\">W dol</button></a>"
# 956 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 956 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 957 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 957 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/52/on\"><button class=\"button button\">STOP!</button></a></p>"
# 957 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 957 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 959 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 959 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<p><a href=\"/38/on\"><button class=\"button button\">Rolety wiatrolap, lazienka parter (1,2) w gore</button></a>"
# 959 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 959 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 960 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 960 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/39/on\"><button class=\"button button\">W dol</button></a>"
# 960 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 960 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 961 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 961 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/40/on\"><button class=\"button button\">STOP!</button></a></p>"
# 961 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 961 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            break;
                        case 2:
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 964 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 964 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<H3>Rolety</H3>"
# 964 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 964 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 965 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 965 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " <h5> Wschod slonca: "
# 965 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 965 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)slonce.sunRiseHour());
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 967 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 967 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          ":"
# 967 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 967 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)slonce.sunRiseMinute());
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "</h5><h5> Zachod slonca: "
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 969 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)slonce.sunSetHour());
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 971 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 971 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          ":"
# 971 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 971 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)slonce.sunSetMinute());
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 973 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 973 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " </h5><h5> Sensor swiatla: "
# 973 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 973 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)roletyCurrentLightLevel);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 975 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 975 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " </h5>"
# 975 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 975 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " </h5><h5> Lampy w ogrodzie: "
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 976 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)gardenLights);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 978 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 978 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " </h5>"
# 978 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 978 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            // Display current state, and ON/OFF buttons for GPIO 5
                            //client.println("<p>GPIO 5 - State " + output5State + "</p>");
                            // If the output5State is off, it displays the ON button

                            if (!roletyAuto)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 986 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 986 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/2/on\"><button class=\"button\">Blinds Timer Off</button></a>"
# 986 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 986 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 990 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 990 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/2/off\"><button class=\"button button2\">Blinds Timer On</button></a>"
# 990 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 990 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            if (!roletyLight)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 995 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 995 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/16/on\"><button class=\"button\">Light sensor off</button></a>"
# 995 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 995 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 999 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 999 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/16/off\"><button class=\"button button2\">Light sensor on</button></a>"
# 999 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 999 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1002 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1002 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<a href=\"/15/on\"><button class=\"button\">Set light level:"
# 1002 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1002 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)roletySetLightLevel);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1004 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1004 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "</button></a></p>"
# 1004 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1004 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            if (!odliczanie.checkTimer(5)) //Blins up button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1008 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1008 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/5/on\"><button class=\"button\">Podnies rolety</button></a>"
# 1008 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1008 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1012 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1012 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a>"
# 1012 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1012 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            // Display current state, and ON/OFF buttons for GPIO 4
                            //client.println("<p>GPIO 4 - State " + output4State + "</p>");
                            // If the output4State is off, it displays the ON button
                            if (!odliczanie.checkTimer(4)) //Blins down button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1020 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1020 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/4/on\"><button class=\"button\">Opusc rolety</button></a></p>"
# 1020 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1020 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1024 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1024 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/3/on\"><button class=\"button\">Rolety stop</button></a></p>"
# 1024 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1024 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };
                            break;
                        case 3:
                            //--------------Podlewanie:
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1029 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1029 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<H3>Podlewanie</H3>"
# 1029 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1029 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            if (podlewanieRainSensor)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1032 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1032 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><input type=" "checkbox" " onclick=" "return false" " checked> Deszcz"
# 1032 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1032 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              ))



                                                                    );
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1040 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1040 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><input type=" "checkbox" " onclick=" "return false" " unchecked> Deszcz"
# 1040 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1040 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              ))



                                                                      );
                            }

                            if (podlewanieAuto)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1049 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1049 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/10/off\"><button class=\"button button2\">Podlewanie Auto</button></a>"
# 1049 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1049 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1053 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1053 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/10/on\"><button class=\"button \">Podlewanie Reczne</button></a>"
# 1053 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1053 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            if (podlewanieCykl == 0)
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1058 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1058 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/11/on\"><button class=\"button\">Start cyklu podlewania</button></a>"
# 1058 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1058 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1062 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1062 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/11/off\"><button class=\"button button2\">Cykl podlewanie w trakcie, nadzisnij by zatrzymac</button></a>"
# 1062 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1062 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            if (!odliczanie.checkTimer(12))
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1067 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1067 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/12/on\"><button class=\"button\">Start LK</button></a>"
# 1067 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1067 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1071 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1071 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/12/off\"><button class=\"button button2\">Stop LK za "
# 1071 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1071 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(12));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1073 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1073 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a>"
# 1073 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1073 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            if (!odliczanie.checkTimer(13))
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1078 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1078 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/13/on\"><button class=\"button\">Start Z1</button></a>"
# 1078 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1078 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1082 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1082 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/13/off\"><button class=\"button button2\">Stop Z1 za "
# 1082 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1082 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(13));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1084 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1084 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a>"
# 1084 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1084 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            if (!odliczanie.checkTimer(14))
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1089 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1089 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/14/on\"><button class=\"button\">Start Z2</button></a></p>"
# 1089 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1089 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1093 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1093 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/14/off\"><button class=\"button button2\">Stop Z2 za "
# 1093 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1093 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(14));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1095 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1095 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a></p>"
# 1095 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1095 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };

                            break;
                        case 4:
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1100 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1100 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<H3>Ogrzewanie</H3>"
# 1100 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1100 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
# 1109 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1109 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/6/off\"><button class=\"button\">Pietro "
# 1109 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1109 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((String)temperature3);
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1111 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1111 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "'C dzien</button></a>"
# 1111 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1111 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else //Upstairs night mode button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1115 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1115 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/6/on\"><button class=\"button button2\">Pietro "
# 1115 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1115 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((String)temperature3);
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1117 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1117 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "'C noc</button></a>"
# 1117 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1117 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            if (!odliczanie.checkTimer(7)) //Force heat upstairs button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1122 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1122 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/7/on\"><button class=\"button\">10 min. On</button></a></p>"
# 1122 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1122 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1126 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1126 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/7/on\"><button class=\"button button2\">"
# 1126 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1126 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(7));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1128 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1128 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a>"
# 1128 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1128 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1129 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1129 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/7/off\"><button class=\"button\">STOP!</button></a></p>"
# 1129 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1129 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            };
                            //----------------Ogrzewanie Parter
                            if (ogrzewanieParterDzien) //Downstairs day mode button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1134 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1134 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/8/off\"><button class=\"button\">Parter "
# 1134 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1134 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((String)temperature2);
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1136 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1136 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "'C dzien</button></a>"
# 1136 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1136 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else //Downstairs night mode button
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1140 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1140 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<p><a href=\"/8/on\"><button class=\"button button2\">Parter "
# 1140 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1140 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((String)temperature2);
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1142 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1142 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "'C noc</button></a>"
# 1142 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1142 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            if (!odliczanie.checkTimer(9))
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1147 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1147 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/9/on\"><button class=\"button\">10 min. On</button></a></p>"
# 1147 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1147 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }
                            else
                            {
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1151 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1151 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/9/on\"><button class=\"button button2\">"
# 1151 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1151 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println(odliczanie.getTime(9));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1153 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1153 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "</button></a>"
# 1153 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1153 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                                client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1154 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1154 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              "<a href=\"/9/off\"><button class=\"button\">STOP!</button></a></p>"
# 1154 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                              ); &__c[0];}))
# 1154 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                              )));
                            }

                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1157 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1157 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          "<h5>Temperatura dzienna: "
# 1157 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1157 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperaturaDzien);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1159 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1159 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " C</H5><H5>Temperatura nocna: "
# 1159 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1159 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));
                            client.println((String)temperaturaNoc);
                            client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1161 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1161 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          " C</H5>"
# 1161 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                          ); &__c[0];}))
# 1161 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                          )));

                            break;
                        case 5: //Show parameters sub-page
                            client.println("<H5>Logi:</H5>");
                            logging = SD.open("log.txt"); //open parametrs file
                            if (logging)
                            {
                                Serial.println("Logs:");

                                // read from the file until there's nothing else in it:
                                while (logging.available())
                                {
                                    //client.println("<br>");
                                    client.write(logging.read());
                                    //client.println("</p>");
                                }
                                // close the file:
                                logging.close();
                            }
                            else
                            {
                                // if the file didn't open, print an error:
                                client.println("error opening params.txt");
                            }
                            client.println("<H5>Parametry:</H5>");
                            logging = SD.open("params.txt"); //open parametrs file
                            if (logging)
                            {
                                Serial.println("Parameters:");
                                // read from the file until there's nothing else in it:
                                while (logging.available())
                                {
                                    //client.println("<br>");
                                    client.write(logging.read());
                                    //client.println("<p>");
                                }
                                // close the file:
                                logging.close();
                            }
                            else
                            {
                                // if the file didn't open, print an error:
                                client.println("error opening params.txt");
                            }
                            client.println("<H5>");
                            break;
                        }
                        //client.println(F("<a href=\"/63/on\"><button class=\"button button2\">Back to main page</button></a></p>"));
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1210 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1210 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "<a href=\"/nothing\"><button class=\"button button2\">Refresh </button></a></p>"
# 1210 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 1210 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      )));
                        //-------------Koniec
                        client.println();
                        client.println((reinterpret_cast<const __FlashStringHelper *>(
# 1213 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 1213 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
                                      "</body></html>"
# 1213 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino" 3
                                      ); &__c[0];}))
# 1213 "c:\\Users\\gmroczkowski\\Documents\\Arduino\\home_control\\home_control_V_0_07_alfa.ino"
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
        digitalWrite(32 /*Garden lights output*/, 0x0);
    }
    else
    {
        digitalWrite(32 /*Garden lights output*/, 0x1);
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
    podlewanieRainSensor = digitalRead(30 /*Rain sensor input*/); // Check rain sensor
    if (!odliczanie.checkTimer(0)) //uptime is flowing
    {
        uptime++;
        odliczanie.resetTimer(0);
        odliczanie.startTimer(1000, 0);
    };
    if (!odliczanie.checkTimer(1)) //write parameters every 5 minutes
    {
        Serial.println("Writeing parameters...");
        odliczanie.resetTimer(1);
        odliczanie.startTimer(30000, 1);

        logging = SD.open("params.txt", (O_READ | O_WRITE | O_CREAT | O_APPEND)); //Open log file in SD card

        if (!logging)
            Serial.println("Error opening parameters.txt"); //Write about opening errors, if any.
        logging.println(zegar.getDate() + ";" + zegar.getTime() + ";" + (String)roletyCurrentLightLevel + ";" + (String)temperature1 + ";" + (String)temperature2 + ";" + (String)temperature3);
        Serial.println(zegar.getDate() + ";" + zegar.getTime() + ";" + (String)roletyCurrentLightLevel + ";" + (String)temperature1 + ";" + (String)temperature2 + ";" + (String)temperature3);
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

        if ((roletyCurrentLightLevel == roletySetLightLevel) && (!odliczanie.checkTimer(15)) && (roletyLight)) //If it is dark
        {
            //Serial.println("Trigger: jest ciemno!");
            odliczanie.startTimer(3600000, 15); //Locking trigger to start one time.
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
        if ((zegar.getHour() == slonce.sunSetHour()) && (zegar.getMinute() == slonce.sunSetMinute() + 5) && (zegar.getSecond() == 0) && (!gardenLights)) //If we have 5 minutes after Sun set and gardenLights are off:)
        {
            gardenLights = true;
            return true;
        }
        return false;
        break;
    case 5:
        if ((zegar.getHour() == gardenLightsOffHour) && (zegar.getMinute() == gardenLightsOffMinute) && (zegar.getSecond() == 0) && (gardenLights)) //If we have 10 minutes past 2AM :)
        {
            gardenLights = false;
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
