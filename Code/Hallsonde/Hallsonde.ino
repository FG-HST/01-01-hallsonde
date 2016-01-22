/*
 Hallsonde
 
 Misst mit verschiedenen Hallsensoren eine Magnetische Flussdichte und stellt diese auf einem Display dar.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 
 von Henning Janssen
 */

// include the library code:
#include <LiquidCrystal.h>
#include <TimerNull.h>
//#include <TimerOne.h>


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// initialize input
volatile int hallPin = A0; //Eingang A0
volatile float hallRef = 2500; //Spannung bei B = 0 mT
volatile double messwertsammlung = 0; //Speicher fuer Messwerte
volatile float messwert = 0;
volatile int messwertAnz = 0;  //Zaehler fuer Messwertanzahl
volatile float sensordivider = 25; //Sensorverhaeltnis in mT/mV


void setup()
{
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Hall-Sonde aktiv");
  
  setSensorTyp();
  
  Timer0.initialize(50);
//  Timer1.initialize(500000);
  Timer0.attachInterrupt(readHall);
}

void loop(){
//Warte und tue nichts
}

void readHall()
{
  if (5000 == messwertAnz) showRMS();
  
  messwert = (((float) analogRead(hallPin))/1023*5000 - hallRef)/sensordivider;
  messwert = messwert*messwert;
  messwertsammlung += messwert;
  messwertAnz++;
}

void showRMS()
{
  messwert = sqrt(messwertsammlung/messwertAnz);
//  messwert = messwertsammlung/messwertAnz;
  messwertsammlung = 0;
  messwertAnz = 0;
  
  setSensorTyp();
  
  lcd.setCursor(7,1);
  lcd.print("             ");
  if (messwert < 10) lcd.setCursor(9,1);
  else lcd.setCursor(8,1);
  lcd.print(messwert);
  lcd.setCursor(13,1);
  lcd.print(" mT"); 
}

void setSensorTyp()
{
  String sensorTyp;
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  int typPin4 = digitalRead(A2);
  int typPin5 = digitalRead(A3);
  if (LOW == typPin4 & LOW == typPin5){         //Sensor 1 = SS495A
    hallRef = 2480;
    sensorTyp = "SS495A ";
    sensordivider = 31.25;
  }else if (HIGH == typPin4 & LOW == typPin5){  //Sensor 2 = SS496A
    hallRef = 2470;
    sensorTyp = "SS496A ";
    sensordivider = 25;
  }else if (HIGH == typPin4 & HIGH == typPin5){  //Sensor 3 = Allegro A1302K
    hallRef = 2499;
    sensorTyp = "A1302K ";
    sensordivider = 13;
  }else sensorTyp = "Sensor?";
  
  lcd.setCursor(0,1);
  lcd.print(sensorTyp);
  
}
