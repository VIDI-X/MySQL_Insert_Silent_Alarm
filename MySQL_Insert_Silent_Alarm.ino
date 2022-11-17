#include "defines.h"
#include "Credentials.h"
#include <MySQL_Generic.h>

#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include <SPI.h>

#define TFT_CS   5
#define TFT_DC  21

Adafruit_ILI9341 display = Adafruit_ILI9341(TFT_CS, TFT_DC);

int PinTrig = 33; // deklaracija pinova: Trig 13, Echo 14
int PinEcho = 32;
long t; // deklaracija varijable vrijeme
int s; // deklaracija varijable udaljenost
int n = 0;
String str;

// Sample query
//char INSERT_DATA[] = "INSERT INTO %s.%s (sensor, location, value1) VALUES ('%s',%d,%s)";
//char INSERT_DATA[] = "INSERT INTO %s.%s (sensor, location, value1, value2) VALUES ('%s',%d,%s,%s)";
char INSERT_DATA[] = "INSERT INTO %s.%s (sensor, location, value1, value2, value3) VALUES ('%s',%d,%s,%s,%s)";

char query[128];
char sens_value[10];

MySQL_Connection conn((Client *)&client);

void setup()
{
  pinMode(PinTrig, OUTPUT); // deklaracija Trig kao izlazne varijable
  pinMode(PinEcho, INPUT); // deklaracija Echo kao ulazne varijable

  display.begin();
  display.fillScreen(0);
  display.setRotation(3);
  display.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  display.setTextSize(5);
  display.println("VIDI X");
  display.setTextSize(3);
  display.println("    microcomputer");
  display.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  display.setTextSize(0);
  Serial.begin(115200);

  while (!Serial && millis() < 5000); // wait for serial port to connect

  MYSQL_DISPLAY1("\nStarting Complex_Insert_WiFi on", BOARD_NAME);
  display.print("Starting Complex_Insert_WiFi on ");
  display.println(BOARD_NAME);
  MYSQL_DISPLAY(MYSQL_MARIADB_GENERIC_VERSION);
  display.println(MYSQL_MARIADB_GENERIC_VERSION);

  // Begin WiFi section
  MYSQL_DISPLAY1("Connecting to", ssid);
  display.print("Connecting to ");
  display.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    MYSQL_DISPLAY0(".");
  }

  // print out info about the connection:
  MYSQL_DISPLAY1("Connected to network. My IP address is:", WiFi.localIP());

  MYSQL_DISPLAY3("Connecting to SQL Server @", server, ", Port =", server_port);
  MYSQL_DISPLAY5("User =", user, ", PW =", password, ", DB =", default_database);

  display.print("Connected to network. My IP address is:");
  display.print(WiFi.localIP());
  display.print(", Port =");
  display.println(server_port);
  display.print("User =");
  display.print(user);
  display.print(", PW =");
  display.print(password);
  display.print(", DB =");
  display.println(default_database);
}

void runInsert()
{
  // Initiate the query class instance
  MySQL_Query query_mem = MySQL_Query(&conn);

  if (conn.connected())
  {
    // Save
    //dtostrf(s, 1, 2, sens_value);
    str = String(s);
    str.toCharArray(sens_value, 10);
    //sprintf(query, INSERT_DATA, default_database, default_table, "Distance sensor", 1, sens_value);
    //sprintf(query, INSERT_DATA, default_database, default_table, "Distance sensor", 1, sens_value, sens_value);
    sprintf(query, INSERT_DATA, default_database, default_table, "Distance sensor", 1, "73", sens_value, sens_value);
    // Execute the query
    MYSQL_DISPLAY(query);
    display.setTextColor(ILI9341_RED, ILI9341_BLACK);
    display.println(query);
    display.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    // KH, check if valid before fetching
    if ( !query_mem.execute(query) )
    {
      MYSQL_DISPLAY("Complex Insert error");
      display.setTextColor(ILI9341_RED, ILI9341_YELLOW);
      display.println("Complex Insert error");
      display.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    }
    else
    {
      MYSQL_DISPLAY("Complex Data Inserted.");
      display.println("Complex Data Inserted.");
    }
  }
  else
  {
    MYSQL_DISPLAY("Disconnected from Server. Can't insert.");
    display.setTextColor(ILI9341_RED, ILI9341_YELLOW);
    display.println("Disconnected from Server. Can't insert.");
    display.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  }
}

void database_handler()
{
  MYSQL_DISPLAY("Connecting...");
  display.println("Connecting...");

  //if (conn.connect(server, server_port, user, password))
  if (conn.connectNonBlocking(server, server_port, user, password) != RESULT_FAIL)
  {
    delay(500);
    runInsert();
    conn.close();                     // close the connection
  }
  else
  {
    MYSQL_DISPLAY("\nConnect failed. Trying again on next iteration.");
    display.setTextColor(ILI9341_RED, ILI9341_YELLOW);
    display.println("Connect failed. Trying again on next iteration.");
    display.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  }

  MYSQL_DISPLAY("\nSleeping...");
  MYSQL_DISPLAY("================================================");
  display.println("Sleeping...");
  display.println("========================");

  delay(6000);
}


void loop() {
  // postavi Trig LOW 2 mikrosekunde – čišćenje Trig pina
  digitalWrite(PinTrig, LOW);
  delayMicroseconds(2); // generiraj ultrazvuk trajanja 10 mikrosekundi
  digitalWrite(PinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrig, LOW);
  t = pulseIn(PinEcho, HIGH); // mjeri vrijeme puta ultrazvuka
  s = t * 0.034 / 2; // pretvori vrijeme u udaljenost

  if (s < 1000) {
    display.setCursor(0, 120);
    display.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    display.print("Udaljenost = ");
    //display.setTextColor(ILI9341_RED, ILI9341_BLACK);
    display.print(s);
    
    display.println(" cm   ");
    display.print("                                                                                                                                                                                                                                                                                                                ");
    display.print("                                                                                                                                                                                                                                                                                                                ");
    Serial.print("Udaljenost = ");
    Serial.println(s);

    if (s < 100) {
      display.setCursor(0, 120);
      display.print("Udaljenost = ");
      display.setTextColor(ILI9341_RED, ILI9341_BLACK);
      display.print(s);
      display.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
      display.println(" cm   ");
      display.println("          ULJEZ");
      database_handler();

      Serial.println("ULJEZ"); // ispis na serijski monitor
    }

    delay(250); // pričekaj 250 mikrosekundi do sljedećeg mjerenja udaljenosti

  } else {
    Serial.print("Greška = ");
    Serial.print(s);
    Serial.print(" n = ");
    Serial.println(n++);
  }
}
