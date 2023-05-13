/***************************************************************************
 Capteur de CO2
 version 0.01
 Pas encore compensé en température (utilisation du capteur température humidité ens 210)
 ***************************************************************************/

#include <Wire.h>
int jaune = 13;
int rouge = 12;
int vert = 14;

int seuil_bon_air = 600;
int seuil_air_moyen = 900;

#include "ScioSense_ENS160.h"  // ENS160 library
ScioSense_ENS160      ens160(ENS160_I2CADDR_1);

void setup() {

  Serial.begin(115200);

  while (!Serial) {}

  pinMode(jaune, OUTPUT);
  pinMode(vert, OUTPUT);
  pinMode(rouge, OUTPUT);
  for (int i = 0;i<10; i++) {
    digitalWrite(jaune, HIGH);digitalWrite(vert, HIGH); digitalWrite(rouge, HIGH);delay(100);
    digitalWrite(jaune, LOW);digitalWrite(vert, LOW);digitalWrite(rouge, LOW);delay(100);
  }
  digitalWrite(jaune, LOW);
  digitalWrite(vert, LOW);
  digitalWrite(rouge, LOW);

  Serial.println("ENS160 Capteur de qualité de l'air (ici C02");
  delay(1000);

  Serial.print("ENS160...");
  ens160.begin();
  Serial.println(ens160.available() ? "done." : "failed!");
  if (ens160.available()) {
    Serial.print("\tRev: "); Serial.print(ens160.getMajorRev());
    Serial.print("."); Serial.print(ens160.getMinorRev());
    Serial.print("."); Serial.println(ens160.getBuild());  
    Serial.print("\tStandard mode ");
    Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
  }
}

void loop() {
  int CCO2 = 0;
  if (ens160.available()) {
    ens160.measure(true);
    ens160.measureRaw(true);
    CCO2 = ens160.geteCO2();
    Serial.print("CO2 = "); Serial.print(CCO2); Serial.println (" ppm");
    if (CCO2 < 200 ) {
      digitalWrite(jaune, HIGH);
      digitalWrite(vert, HIGH);
      digitalWrite(rouge, HIGH);
      delay(300);
      digitalWrite(jaune, LOW);
      digitalWrite(vert, LOW);
      digitalWrite(rouge, LOW);
    }   
    else if (CCO2 < seuil_bon_air) {
      digitalWrite(jaune, LOW);
      digitalWrite(vert, HIGH);
      digitalWrite(rouge, LOW);
    }
    else if (CCO2 < seuil_air_moyen) {
      digitalWrite(jaune, HIGH);
      digitalWrite(vert, LOW);
      digitalWrite(rouge, LOW);
    }
    else  {
      digitalWrite(jaune, LOW);
      digitalWrite(vert, LOW);
      digitalWrite(rouge, HIGH);
    }
    
  }
  else {
    digitalWrite(jaune, HIGH);
    digitalWrite(vert, HIGH);
    digitalWrite(rouge, HIGH);
    delay(300);
    digitalWrite(jaune, LOW);
    digitalWrite(vert, LOW);
    digitalWrite(rouge, LOW);
    Serial.println(" Le capteur déconne pendant la première heure d'utilisation\ndamn!!!!!!!");    
  }
  delay (2000);
}


