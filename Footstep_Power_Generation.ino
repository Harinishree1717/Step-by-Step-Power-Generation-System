#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensoroutput = A3;
int ledoutput = 13;
int THRESHOLD = 20;
int sensorValue = 0;

unsigned long lastChangeTime = 0;
bool showingStatus = false;
unsigned long displayDuration = 2000;  // Default duration for LOW

enum PressureState { NONE, PRESSURE_HIGH, PRESSURE_LOW };
PressureState currentState = NONE;
PressureState previousState = NONE;

void setup() {
  pinMode(ledoutput, OUTPUT);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  // Welcome and team info
  lcd.setCursor(0, 0);
  lcd.print("TEAM 18");
  lcd.setCursor(0, 1);
  lcd.print("Power Gen System");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("KaviyaShree K");
  lcd.setCursor(0, 1);
  lcd.print("HariniShree S");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Haritha P");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Guided by:");
  lcd.setCursor(0, 1);
  lcd.print("Mr.Senthilnathan");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Monitoring...");
}

void loop() {
  sensorValue = analogRead(sensoroutput);
  Serial.println(sensorValue);

  // Determine the current pressure state
  if (sensorValue >= THRESHOLD) {
    currentState = PRESSURE_HIGH;
  } else {
    currentState = PRESSURE_LOW;
  }

  // Check for state change
  if (currentState != previousState) {
    lcd.clear();
    if (currentState == PRESSURE_HIGH) {
      digitalWrite(ledoutput, HIGH);
      lcd.setCursor(1, 0);
      lcd.print("HIGH");
      lcd.setCursor(0, 1);
      lcd.print("ABOVE THRESHOLD");
      displayDuration = 5000;  // 5 seconds for HIGH
    } else {
      digitalWrite(ledoutput, LOW);
      lcd.setCursor(1, 0);
      lcd.print("LOW");
      lcd.setCursor(0, 1);
      lcd.print("BELOW THRESHOLD");
      displayDuration = 2000;  // 2 seconds for LOW
    }

    lastChangeTime = millis();
    showingStatus = true;
    previousState = currentState;
  }

  // After the specified duration, return to monitoring state
  if (showingStatus && millis() - lastChangeTime >= displayDuration) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Monitoring...");
    digitalWrite(ledoutput, LOW); // Ensure LED is off during monitoring
    showingStatus = false;
  }

  delay(100);
}
