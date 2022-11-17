/****************************************************************************************************************************
  Credentials.h
  Library for communicating with a MySQL or MariaDB Server
  
  Based on and modified from Dr. Charles A. Bell's MySQL_Connector_Arduino Library https://github.com/ChuckBell/MySQL_Connector_Arduino
  to support nRF52, SAMD21/SAMD51, SAM DUE, STM32F/L/H/G/WB/MP1, ESP8266, ESP32, etc. boards using W5x00, ENC28J60, LAM8742A Ethernet,
  WiFiNINA, ESP-AT, built-in ESP8266/ESP32 WiFi.

  The library provides simple and easy Client interface to MySQL or MariaDB Server.
  
  Built by Khoi Hoang https://github.com/khoih-prog/MySQL_MariaDB_Generic
  Licensed under MIT license
 **********************************************************************************************************************************/

#ifndef Credentials_h
#define Credentials_h

char ssid[] = "1234";        // your network SSID (name)
char pass[] = "1234";        // your network password

#define USING_HOST_NAME     true

#if USING_HOST_NAME
  // Optional using hostname, and Ethernet built-in DNS lookup
  char server[] = "sql7.freemysqlhosting.net"; // change to your server's hostname/URL
#else
  IPAddress server(192, 168, 1, 112);
#endif

uint16_t server_port = 3306;

char default_database[] = "1234";           //"test_database";
char default_table[]    = "VIDI_X_Data";          //"test_table";

char user[]         = "1234";              // MySQL user login username
char password[]     = "1234";          // MySQL user login password

#endif    //Credentials_h
