/*
 * ATmega16 ADVANCED VISUALIZER
 * ----------------------------
 * Tombol 1 (Pin 33): Ganti MODE Tampilan (Bar -> Dot -> Center)
 * Tombol 2 (Pin 34): Ganti SENSITIVITAS (High -> Med -> Low)
 */

const int audioPin = A0; 
const int btnMode = 31;   // Pin Fisik 33 (PA7)
const int btnSens = 30;   // Pin Fisik 34 (PA6)

int ledPins[] = {16, 17, 18, 19, 20, 21, 22, 23}; 

// VARIABEL STATUS
int visualMode = 0;       // 0=Bar, 1=Dot, 2=Center
int sensitivity = 20;     // Mulai dari sensitif

void setup() {
  for (int i = 0; i < 8; i++) pinMode(ledPins[i], OUTPUT);
  
  pinMode(btnMode, INPUT_PULLUP);
  pinMode(btnSens, INPUT_PULLUP);
  
  analogReference(INTERNAL2V56); // Tegangan ref 2.56V (Super Peka)
  
  // Efek pembuka: Kasih tau kalau siap
  openingEffect();
}

void loop() {
  // --- CEK TOMBOL MODE (Pin 33) ---
  if (digitalRead(btnMode) == LOW) {
    visualMode++; 
    if (visualMode > 2) visualMode = 0; // Balik ke 0 kalau udah mentok
    
    // Kedip sebentar tanda ganti mode
    allOff(); delay(100); 
    while(digitalRead(btnMode) == LOW); // Tunggu tombol dilepas (biar gak loncat)
  }

  // --- CEK TOMBOL SENSITIVITAS (Pin 34) ---
  if (digitalRead(btnSens) == LOW) {
    sensitivity = sensitivity + 15;
    if (sensitivity > 50) sensitivity = 5; // Balik ke paling peka (5)
    
    // Kedip tanda ganti sens
    digitalWrite(ledPins[0], HIGH); delay(100); digitalWrite(ledPins[0], LOW);
    while(digitalRead(btnSens) == LOW); 
  }

  // --- BACA AUDIO ---
  int signal = analogRead(audioPin);
  
  // Noise Gate (Buang sinyal sampah di bawah 3)
  int cleanSignal = signal; 
  if (cleanSignal < 3) cleanSignal = 0;

  int level = map(cleanSignal, 3, sensitivity, 0, 8);
  if (level > 8) level = 8;
  if (level < 0) level = 0;

  // --- TAMPILKAN SESUAI MODE ---
  allOff(); // Matikan dulu semua sebelum digambar ulang

  switch (visualMode) {
    case 0: // MODE BAR (Biasa)
      for (int i = 0; i < level; i++) digitalWrite(ledPins[i], HIGH);
      break;

    case 1: // MODE DOT (Titik Loncat)
      if (level > 0) digitalWrite(ledPins[level - 1], HIGH);
      // Nyalakan lampu paling bawah dikit biar gak gelap total
      if (level == 0) digitalWrite(ledPins[0], LOW); 
      break;

    case 2: // MODE CENTER (Mekar dari Tengah)
      // Kita bagi level jadi separuh buat atas, separuh buat bawah
      int halfLevel = level / 2; 
      // Titik tengah ada di index 3 dan 4
      for (int i = 0; i < halfLevel; i++) {
        digitalWrite(ledPins[3 - i], HIGH); // Ke Bawah
        digitalWrite(ledPins[4 + i], HIGH); // Ke Atas
      }
      break;
  }

  delay(2); // Delay pendek
}

void allOff() {
  for (int i = 0; i < 8; i++) digitalWrite(ledPins[i], LOW);
}

void openingEffect() {
  for(int i=0; i<8; i++) { digitalWrite(ledPins[i], HIGH); delay(30); }
  for(int i=0; i<8; i++) { digitalWrite(ledPins[i], LOW); delay(30); }
  delay(200);
}
