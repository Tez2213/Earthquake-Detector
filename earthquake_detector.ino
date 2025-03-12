#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ADXL345_ADDR 0x53   // I2C address of ADXL345
#define BUZZER_PIN 9
#define GREEN_LED 4
#define YELLOW_LED 5
#define RED_LED 6

LiquidCrystal_I2C lcd(0x27, 16, 2);

int x, y, z;
int x_prev = 1, y_prev = 2, z_prev = 238; // Default stable values

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Earthquake Det.");

  // Set LEDs and Buzzer as outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Configure ADXL345
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x2D);
  Wire.write(0x08);  // Measurement mode
  Wire.endTransmission();
}

void loop() {
  readAccelerometer(); // Read sensor values

  int deltaX = abs(x - x_prev);
  int deltaY = abs(y - y_prev);
  int deltaZ = abs(z - z_prev);
  int maxChange = max(deltaX, max(deltaY, deltaZ));

  Serial.print("X: "); Serial.print(x);
  Serial.print(" | Y: "); Serial.print(y);
  Serial.print(" | Z: "); Serial.println(z);

  if (maxChange <= 10) {
    // Normal Condition
    lcd.setCursor(0, 1);
    lcd.print("Status: Normal  ");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER_PIN);
  } else if (maxChange > 10 && maxChange <= 25) {
    // Mild Shake
    lcd.setCursor(0, 1);
    lcd.print("Status: Warning ");
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    delay(1000); 
    noTone(BUZZER_PIN);
  } else {
    // Strong Shake - Earthquake Alert
    lcd.setCursor(0, 1);
    lcd.print("EARTHQUAKE ALERT");
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    tone(BUZZER_PIN, 1000);
    delay(1000);  // Buzzer ON for 1 second
    noTone(BUZZER_PIN);
  }

  x_prev = x;
  y_prev = y;
  z_prev = z;

  delay(50);  // Short delay before next reading
}

void readAccelerometer() {
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345_ADDR, 6, true);

  x = Wire.read() | (Wire.read() << 8);
  y = Wire.read() | (Wire.read() << 8);
  z = Wire.read() | (Wire.read() << 8);
}