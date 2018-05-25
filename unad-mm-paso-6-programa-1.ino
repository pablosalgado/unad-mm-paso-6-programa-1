#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

//
// Definición de puertos de entrada para la ZONA 1
//
int pIFZ1 = 13; // PIR
int pTSZ1 = 12; // Touch Sensor
int pPSZ1 = 11; // Pressure sensor

// Definición de puerto para control de los LED
int pLV = 7;
int pLA = 6;
int pLR = 5;

boolean alarma;

void setup() {
  alarma = false;
  
  // Configura puertos de la zona 1
  pinMode(pIFZ1, INPUT);
  pinMode(pTSZ1, INPUT);
  pinMode(pPSZ1, INPUT);

  // Puertos de control de los LED  
  pinMode(pLV, OUTPUT);
  pinMode(pLA, OUTPUT);
  pinMode(pLR, OUTPUT);

  lcd.begin();
  lcd.print("PASM Security");
  delay(1000);
}

void loop() {
  if (alarma) {
    return;
  }
  
  lcd.clear();

  // Lectura de sensores de la ZONA 1
  boolean vIRZ1 = digitalRead(pIFZ1);
  boolean vTSZ1 = digitalRead(pTSZ1);
  boolean vPSZ1 = digitalRead(pPSZ1);

  // Determinar el estado de cada uno de los LEDs de la ZONA 1
  boolean lVZ1 = !vIRZ1 && !vTSZ1 && !vPSZ1;
  boolean lAZ1 = vTSZ1 && !vPSZ1 || vIRZ1 && !vPSZ1;
  boolean lRZ1 = vPSZ1;

  // Apagar todos los LED para visualizar el nuevo estado
  digitalWrite(pLV, LOW);
  digitalWrite(pLA, LOW);
  digitalWrite(pLR, LOW);

  lcd.setCursor(0,0);
  lcd.print("Zona 1");
  
  lcd.setCursor(0,1);
  if(lVZ1) {
    digitalWrite(pLV, HIGH);
    lcd.print("Normal");     
  } else if (lAZ1) {
    digitalWrite(pLA, HIGH);
    lcd.print("Alerta");     
  } else if (lRZ1) {
    digitalWrite(pLR, HIGH);
    lcd.print("Alarma");     
  }

  // Enceder los LEDs 
  
  alarma = lRZ1;

  delay(100);
}
