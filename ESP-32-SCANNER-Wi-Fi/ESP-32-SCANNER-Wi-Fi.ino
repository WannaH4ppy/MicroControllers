#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>


//piny ekranu
// Definiujemy piny ekranu z Twojego schematu (IdeaSpark 1.9)
#define TFT_CS 15
#define TFT_DC  2
#define TFT_RST 4
#define TFT_BLK 32 // Pin podświetlenia ekranu (Backlight)

//piny radia
#define CC1101_CSN  5 
#define CC1101_GDO0 26
#define CC1101_GDO2 25

// Piny MOSI (23) i SCLK (18) są używane domyślnie przez sprzętowe SPI ESP32, 
// więc nie musimy ich tu nawet wpisywać.

// Tworzymy "obiekt" ekranu, żeby móc do niego wysyłać komendy
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  
  // 1. WŁĄCZENIE PODŚWIETLENIA (Krytyczny krok, bez tego ekran będzie czarny!)
  pinMode(TFT_BLK, OUTPUT);
  digitalWrite(TFT_BLK, HIGH);
    // 2. INICJALIZACJA EKRANU (170x320 to standardowa rozdzielczość dla ekranów 1.9")
  tft.init(170, 320); 
  tft.setRotation(1);
  // Jeśli kolory na ekranie będą dziwne (np. czarne tło będzie białe), odznacz poniższą linijkę:
  // tft.invertDisplay(true);
  // tft.setRotation(1); // Odznacz, jeśli chcesz obrócić tekst do poziomu
  // 3. CZYSZCZENIE EKRANU
  tft.fillScreen(ST77XX_BLACK); // Zamalowujemy całe tło na czarno
  

  //Config Wi-Fi
  // Mówimy ESP32: "Bądź stacją (odbiornikiem), a nie routerem nadającym własną sieć"
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();// Rozłączamy się, gdyby pamiętał jakieś stare sieci
  delay(100);// Dajemy mu ułamek sekundy na restart anteny

  // 4. USTAWIENIA TEKSTU I WYŚWIETLANIE
             
  tft.setTextColor(ST77XX_GREEN);    // Zielony tekst (prawdziwie hackerski styl)
  tft.setTextSize(2);                // Rozmiar czcionki
  tft.setCursor(0, 0);
  tft.println("Bootowanie...");
  tft.println("Antena Wi-Fi aktywna.");

  // Wypisujemy to niżej (Y = 140), żeby nie nakładało się na test radia
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_CYAN);
  tft.setCursor(10, 140);
  tft.println("ESP32 gotowe.");

  delay(4000);
}

void loop() {
  //Czyszcenie ekranu przed skanem
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0,0);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);     
  tft.println("Skanowanie...");

  // MAGICZNA FUNKCJA: Procesor szuka sieci i zapisuje ich liczbę do zmiennej "n"
  int n = WiFi.scanNetworks();

  // KIEDY SKOŃCZY SZUKAĆ, ZNÓW CZYŚCIMY EKRAN I PISZEMY WYNIKI
  tft.fillScreen(ST77XX_BLACK); 
  tft.setCursor(0, 0);

  if (n == 0) {
    tft.setTextColor(ST77XX_RED);
    tft.println("Brak sieci w zasiegu :(");
  } else {
    tft.setTextColor(ST77XX_YELLOW);
    tft.print("Znaleziono sieci: ");
    tft.println(n);
    tft.println("--------------------");

    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(1); // Zmniejszamy czcionkę na bardzo małą (żeby upchnąć dużo nazw)

    // Wypisujemy maksymalnie 12 sieci, żeby tekst nie uciekł nam w dół poza ekran
    int max_sieci = n;
    if (max_sieci > 12) {
      max_sieci = 12; 
    }

    // Pętla przelatująca po kolei przez każdą znalezioną sieć
    for (int i = 0; i < max_sieci; ++i) {
      if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN){
        tft.setTextColor(ST77XX_GREEN); // siec bez hasla na zielono
      } else {
        tft.setTextColor(ST77XX_WHITE); // z haslem na bialo
      }
      tft.print(i + 1);                     // Numer porządkowy (1, 2, 3...)
      tft.print(": ");
      tft.print(WiFi.SSID(i));              // SSID to profesjonalna nazwa na "Nazwę Sieci"
      tft.print(" (");
      tft.print(WiFi.RSSI(i));              // RSSI to siła sygnału w decybelach (np. -60dBm)
      tft.println(" dBm)");
    }
  }

  // Czekamy 5 sekund i zaczynamy całą pętlę od nowa!
  delay(5000);
}
