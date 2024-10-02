/*
  Nom du projet : Contrôle des LEDs RGB via un bouton et conversion HSV
  Auteur : Jacob Turcotte
  Date de création : 20 septembre 2024
  Description : Ce programme permet de contrôler des LEDs RGB en variant
                les couleurs à l'aide d'un bouton. La conversion des couleurs
                est réalisée en utilisant un modèle HSV vers RGB.
*/

#include <Arduino.h>
#include <math.h>

// Définition des broches pour le bouton et les LEDs
#define BUTTON_PIN 13
#define LED_PIN_R 4 
#define LED_PIN_G 5 
#define LED_PIN_B 6 

// Macro pour convertir les valeurs RGB en intensité LED
#define LED_CONVERSION(C) abs(C-255)*(1000/255)

// Déclarations des fonctions
void LED_color(float R, float G, float B);
float * HSVtoRGB(float h, float s, float v);

void setup() {
  // Initialisation du port série pour le débogage
  Serial.begin(115200);

  // Configuration des broches LED comme sorties
  pinMode(LED_PIN_R, OUTPUT_OPEN_DRAIN);
  pinMode(LED_PIN_G, OUTPUT_OPEN_DRAIN);
  pinMode(LED_PIN_B, OUTPUT_OPEN_DRAIN);

  // Configuration de la broche du bouton comme entrée avec pull-down
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);

  // Éteindre les LEDs au démarrage
  digitalWrite(LED_PIN_R, HIGH);
  digitalWrite(LED_PIN_G, HIGH);
  digitalWrite(LED_PIN_B, HIGH);
}

void loop() {
  float *RGB; // Pointeur pour stocker les valeurs RGB

  // Si le bouton est pressé
  if (digitalRead(BUTTON_PIN)) {
    // Attendre que le bouton soit relâché
    while (digitalRead(BUTTON_PIN));

    // Afficher un message dans le moniteur série
    Serial.println("TASTE THE RAINBOW!");

    // Boucle pour changer la couleur sur 360 degrés de teinte
    for (int i = 0; i < 360; i++) {
      // Conversion de la teinte HSV en RGB
      RGB = HSVtoRGB(float(i), 0.75, 0.5);
      
      // Appliquer la couleur aux LEDs
      LED_color(RGB[0], RGB[1], RGB[2]);
      
      // Attendre 10 ms avant de changer la couleur
      delay(10);
    }
  }
}

// Fonction pour convertir les valeurs HSV en RGB
float *HSVtoRGB(float h, float s, float v) {
  float RGBf[3]; // Tableau temporaire pour les valeurs RGB flottantes
  static float RGB[3]; // Tableau statique pour retourner les valeurs RGB

  // Calcul des valeurs intermédiaires pour la conversion
  float C = s * v;
  float X = C * (1.0 - abs(fmod(h / 60.0, 2.0) - 1.0));
  float m = v - C;

  // Conditions pour déterminer les valeurs RGB en fonction de la teinte (h)
  if (h < 60) {
    RGBf[0] = C;
    RGBf[1] = X;
    RGBf[2] = 0;
  } else if (h >= 60 && h < 120) {
    RGBf[0] = X;
    RGBf[1] = C;
    RGBf[2] = 0;
  } else if (h >= 120 && h < 180) {
    RGBf[0] = 0;
    RGBf[1] = C;
    RGBf[2] = X;
  } else if (h >= 180 && h < 240) {
    RGBf[0] = 0;
    RGBf[1] = X;
    RGBf[2] = C;
  } else if (h >= 240 && h < 300) {
    RGBf[0] = X;
    RGBf[1] = 0;
    RGBf[2] = C;
  } else if (h >= 300 && h < 360) {
    RGBf[0] = C;
    RGBf[1] = 0;
    RGBf[2] = X;
  }

  // Conversion des valeurs flottantes en échelle de 0 à 255
  RGB[0] = (RGBf[0] + m) * 255.0;
  RGB[1] = (RGBf[1] + m) * 255.0;
  RGB[2] = (RGBf[2] + m) * 255.0;

  // Retourner les valeurs RGB
  return RGB;
}

// Fonction pour régler la couleur des LEDs en utilisant les valeurs RGB
void LED_color(float R, float G, float B) {
  // Appliquer la conversion et régler l'intensité des LEDs
  analogWrite(LED_PIN_R, LED_CONVERSION(R));
  analogWrite(LED_PIN_G, LED_CONVERSION(G));
  analogWrite(LED_PIN_B, LED_CONVERSION(B));
}
