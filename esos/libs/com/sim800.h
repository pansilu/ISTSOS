#ifndef ISTSOSGPRS_H
#define ISTSOSGPRS_H

#define TINY_GSM_MODEM_SIM800
//#define TINY_GSM_MODEM_SIM900

#define SIM_POWER 34
#define SIM_PIN_STATUS 40

#define TIMEOUT 30000UL

// error code
#define REQUEST_SUCCESS 0
#define REQUEST_FAILURE 1
#define CONNECTION_PROBLEM 2
#define NETWORK_FAILURE 3
#define GPRS_FAILURE 4
#define SERVER_FAILURE 5

#define DEBUG_COM 0
#define DEBUG_COM_ANS 1

#define USE_SSL 1


#define OK "OK"
#define ERROR "ERROR"
#define TMP_REG_OK_HOME 1
#define TMP_REG_OK_ROAMING 5


//#include <TinyGsmClient.h>
#include "utils.h"
#include "comunication.h"

class Sim800 : public ICom
{

private:
    // generic for HardwareSerial or SoftwareSerial
    Stream* serialAT;
    //HardwareSerial* serialAT;
    // TinyGsm* modem;
    //TinyGsmClient* client;
    const char* apn;
    const char* user;
    const char* pass;
    const char* pin;
    const char* basic;
    /**
    */
    uint8_t connectGprs();
    /**
        Read response from the server
    */
    bool getResponse();
    /**
        Wake up shield if it's in sleep mode.
    */
    void wakeUpShield();
    /**
        Put the shield in sleep mode.
    */
    void sleepMode();
    

    /**
        To
    */
    void power();
    /**
        Unlock sim.

    */
    bool unlockSim();

    /**
        Send command and wait for response

        @param command command to be executed
        @param timeout limit to wait for a response
        @param expected expected response
    */
    template<typename T>
    uint8_t sendCmd(T command, uint32_t timeout=60000UL, const String expected="");

    /**
        Wait for GPRS to connect to the network

        @param timeout max timeout

        @return bool true if connected
    */
    bool waitForNetwork(unsigned long timeout=30000UL);

    /**
        Connect to the APN
    */
    bool connectToNetwork();
    

    /**
        Wait for a response

        @param timeout max timeout
        @param expected Expected string to wait

        @return true if expected string or OK
    */
    uint8_t waitResponse(uint32_t timeout=60000UL, const String expected="");

    /**
        Restart modem

        @param bool true if restarted
    */
    bool restart();

    /**
        Set bautrate

        @param timeout

        @return bool
    */
    bool autoBaud(unsigned long timeout = 10000UL);

    /**
        Send command to modem

        @param comm command
        @params command list of commands
    */
    template<typename T, typename... Args>
    void writeCmd(T comm, Args... command);

    /**
        Senf single command to the modem
        @params command command
    */
    template<typename T>
    void writeCmd(T command);

    /**
        Clear buffer
    */
    void serialFlush();

public:

    /**
        Constructor with Stream.

        @param serial Hardware or Software serial
        @param apn Access Point Name ("gprs.swisscom.ch")
        @param user APN user ("gprs")
        @param pass APN password ("gprs")
        @param basic Basic authentication string
        @param pin pin to unlock the SIM ("1234)
    */
    Sim800(Stream &serial, const char* apn, const char* user, const char* pass, const char* basic="", const char* pin="");

    /**
        Initialize SIM enviroment.
        Call it inside setup()

        @return bool TODO
    */
    uint8_t begin();



    // call this methods inside loop()
    /**
        Execute GET requests.

        @param server server to do the request
        @params resource resource URI

        @return uint8_t return code
    */
    //uint8_t executeGet(const char server[], const char uri[]);
    /**
        Execute POST requests.

        @param server server to do the request
        @param resource resource URI
        @param message post body

        @return uint8_t return code
    */
    uint8_t executePost(const char server[], const char uri[], const String& data);

    /**
        Update the internal RTC and return the current date.

        @return int* array of date [year, month, day, hour, minute, second, tz hour, tz min]
    */
    uint32_t* ntpUpdate(const char ntpServer[], int GMT);


    /**
        Disconnect from GPRS network.
    */
    void disconnect();
	/**
        Check if connected to the network
    */
    uint8_t getRegStatus();
	/**
        Get shield status

        @return bool status
    */
    bool getStatus();
	
};
#endif
