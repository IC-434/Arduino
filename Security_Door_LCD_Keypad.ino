#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27
#define LCD_COLUMNS 16
#define LCD_LINES   4
#define LEDR 10
#define LEDG 11

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_LINES);

const uint8_t ROWS = 4;
const uint8_t COLS = 4;

char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

uint8_t colPins[COLS] = { 5, 4, 3, 2 }; // Pins connected to C1, C2, C3, C4
uint8_t rowPins[ROWS] = { 9, 8, 7, 6 }; // Pins connected to R1, R2, R3, R4
char key;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const char* password = "1234"; // Passwort festlegen
char enteredCode[5]; // Passwort abspeichern
int codeIndex = 0; // Index for tracking entered code

void setup() {
  // Serial.begin(9600);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Deine Eingabe:"); 
}

void loop() {

  key = keypad.getKey();

  if(key)
  {
    if (key >= '0' && key <= '9' && codeIndex < 4) 
    {
      // Wenn die Taste eine Zahl ist und noch nicht 4 Ziffern eingegeben wurden
      enteredCode[codeIndex] = key;  // Ziffer speichern
      codeIndex++;                 // Zähler erhöhen
      lcd.setCursor(codeIndex, 1); // Cursor auf die nächste Position in der zweiten Zeile setzen
      lcd.print('*');               // Sternchen anzeigen
    }

    if (key == '#') 
    { 
      // Wenn die Raute-Taste gedrückt wird
      enteredCode[codeIndex] = '\0';  // Nullterminator hinzufügen, um das Array als String abzuschließen
      lcd.clear();                   // Display leeren
      lcd.setCursor(0, 0);           // Cursor zurücksetzen
      lcd.print("Deine Eingabe:");      // Ausgabe-Text anzeigen
      lcd.setCursor(0, 1);           // Auf zweite Zeile wechseln
      lcd.print(enteredCode);           // Eingegebene Ziffern anzeigen
      codeIndex = 0;                // Index zurücksetzen für neue Eingabe   
    }

    if (key == 'C') 
    { 
      //Wenn die C-Taste gedrückt wird, Eingabe löschen
      codeIndex = 0;                // Index zurücksetzen
      lcd.clear();                   // Display leeren
      lcd.setCursor(0, 0); // Cursor zurücksetzen
      digitalWrite(LEDG, LOW); // LEDG aus
      digitalWrite(LEDR, LOW); // LEDR aus
      lcd.print("Deine Eingabe:");      
    }

    if (codeIndex == 4)
    {
      if (strcmp(enteredCode, password) == 0)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zugang gewaehrt");
        lcd.setCursor(0, 1); 
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDR, LOW);

        memset(enteredCode, 0, sizeof(enteredCode));
        codeIndex = 0;
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Zugang"); 
        lcd.setCursor(0, 1);
        lcd.print("verweigert");
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, LOW);
      }
    }
  }
}
