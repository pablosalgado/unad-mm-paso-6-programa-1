#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);

//
// Definición de puertos de entrada para la ZONA 1
//
int pIFZ1 = 13; // PIR
int pTSZ1 = 12; // Touch Sensor
int pPSZ1 = 11; // Pressure sensor

//
// Definición de puertos de entrada para la ZONA 1
//
int pIFZ2 = 10; // PIR
int pTSZ2 = 9;  // Touch Sensor
int pPSZ2 = 8;  // Pressure sensor

// Definición de puertos para control de los LED
int pLV = 7; // Verde
int pLA = 6; // Amarillo
int pLR = 5; // Rojo

// Definición de puertos para el control de actuadores
int pMZ1 = 4; // Motor de la puerta de la ZONA 1
int pMZ2 = 3; // Motor de la puerta de la ZONA 2
int pMPP = 2; // Motor de la puerta principal del museo
int pEL  = 1; // Iluminación de emergencia
int pBUZ = 0; // Sirena de la alarma

boolean alarmaZ2;
boolean alarmaZ1;

void setup() {
  alarmaZ1 = false;
  alarmaZ2 = false;

  // Configura puertos de la zona 1
  pinMode(pIFZ1, INPUT);
  pinMode(pTSZ1, INPUT);
  pinMode(pPSZ1, INPUT);

  // Puertos de control de los LED  
  pinMode(pLV, OUTPUT);
  pinMode(pLA, OUTPUT);
  pinMode(pLR, OUTPUT);

  // Puertos de control de los actuadores
  pinMode(pMZ1, OUTPUT);
  pinMode(pMZ2, OUTPUT);
  pinMode(pMPP, OUTPUT);
  pinMode(pEL, OUTPUT);
  pinMode(pBUZ, OUTPUT);
  
  // Asegurar el estado inicial de los actuadores
  digitalWrite(pMZ1, LOW);
  digitalWrite(pMZ2, LOW);
  digitalWrite(pMPP, LOW);
  digitalWrite(pEL, LOW);
  digitalWrite(pBUZ, LOW);

  lcd.begin();
  lcd.print("PASM Security");
  
  delay(300);
}

void loop() {
  // --------------------------------------------------------------------------
  // ZONA 1
  // --------------------------------------------------------------------------
  
  // Lectura de sensores de la ZONA 1
  boolean vIRZ1 = digitalRead(pIFZ1);
  boolean vTSZ1 = digitalRead(pTSZ1);
  boolean vPSZ1 = digitalRead(pPSZ1);

  // Determinar el estado de cada uno de los LEDs de la ZONA 1
  boolean lVZ1 = !vIRZ1 && !vTSZ1 && !vPSZ1;
  boolean lAZ1 = vTSZ1 && !vPSZ1 || vIRZ1 && !vPSZ1;
  boolean lRZ1 = vPSZ1;

  if(!alarmaZ1) {
    alarmaZ1 = lRZ1;    
  }

  // Visualizar estado de la ZONA 1. En caso de alarma se sigue desplegando el
  // mensaje en el display, lo cual habilita el monitoreo de las restantes 
  // zonas para intentar evitar una distracción en una zona mientras el objetivo
  // es otra zona del museo.
  if (!alarmaZ1) {
    visualizarEstado("ZONA 1", lVZ1, lAZ1, lRZ1);
  } else {
    visualizarEstado("ZONA 1", false, false, true);
    activarActuadores();
  }

  // --------------------------------------------------------------------------
  // ZONA 2
  // --------------------------------------------------------------------------

  // Lectura de sensores de la ZONA 2
  boolean vIRZ2 = digitalRead(pIFZ2);
  boolean vTSZ2 = digitalRead(pTSZ2);
  boolean vPSZ2 = digitalRead(pPSZ2);

  // Determinar el estado de cada uno de los LEDs de la ZONA 2
  boolean lVZ2 = !vIRZ2 && !vTSZ2 && !vPSZ2;
  boolean lAZ2 = vTSZ2 && !vPSZ2 || vIRZ2 && !vPSZ2;
  boolean lRZ2 = vPSZ2;
  
  if(!alarmaZ2) {
    alarmaZ2 = lRZ2;    
  }

  // Visualizar estado de la ZONA 2. En caso de alarma se sigue desplegando el
  // mensaje en el display, lo cual habilita el monitoreo de las restantes 
  // zonas para intentar evitar una distracción en una zona mientras el objetivo
  // es otra zona del museo.
  if (!alarmaZ2) {
    visualizarEstado("ZONA 2", lVZ2, lAZ2, lRZ2);
  } else {
    visualizarEstado("ZONA 2", false, false, true);
    activarActuadores();
  }
}

/**
 * Visualiza el estado de la zona en el display y enciende los LED de acuerdo
 * al mismo.
 */
void visualizarEstado(const char msg[], boolean lv, boolean la, boolean lr) {
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(msg);

  lcd.setCursor(0,1);
  if(lv) {
    lcd.print("Normal");     
  } else if (la) {
    lcd.print("Alerta");     
  } else if (lr) {
    lcd.print("Alarma");     
  }
  
  digitalWrite(pLV, lv);
  digitalWrite(pLA, la);
  digitalWrite(pLR, lr);

  delay(100);
}

/**
 * Rutina de activación de los actuadores en caso de alarma
 */
void activarActuadores() {
  // Activar los motores de las puertas de todas las zonas
  digitalWrite(pMZ1, HIGH);
  digitalWrite(pMZ2, HIGH);
  digitalWrite(pMPP, HIGH);
  digitalWrite(pEL, HIGH);
  digitalWrite(pBUZ, HIGH);
}

