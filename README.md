# Earthquake Detector

This project is an earthquake detector using an Arduino Uno, an ADXL345 accelerometer, an LCD display, LEDs, and a buzzer. The device detects seismic activity and provides visual and audible alerts based on the intensity of the detected motion.

## Components

- Arduino Uno
- ADXL345 Accelerometer (I2C address 0x53)
- LCD Display with I2C interface (address 0x27)
- Buzzer (connected to pin 9)
- Green LED (connected to pin 4)
- Yellow LED (connected to pin 5)
- Red LED (connected to pin 6)
- Necessary wiring and power supply

## Libraries

- `Wire.h` (for I2C communication)
- `LiquidCrystal_I2C.h` (for LCD control)

## Circuit Diagram

1. **ADXL345 Accelerometer**:
   - VCC to 3.3V
   - GND to GND
   - SDA to A4
   - SCL to A5

2. **LCD Display**:
   - VCC to 5V
   - GND to GND
   - SDA to A4
   - SCL to A5

3. **LEDs**:
   - Green LED: Anode to pin 4, Cathode to GND (through a resistor)
   - Yellow LED: Anode to pin 5, Cathode to GND (through a resistor)
   - Red LED: Anode to pin 6, Cathode to GND (through a resistor)

4. **Buzzer**:
   - Positive to pin 9
   - Negative to GND

## Code Explanation

The code initializes the accelerometer, LCD, LEDs, and buzzer. It reads the accelerometer values and compares them to previous values to detect motion. Based on the intensity of the detected motion, it updates the LCD display, LEDs, and buzzer.

### Setup

```arduino
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
```

### Loop

```arduino
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
```

### Read Accelerometer

```arduino
void readAccelerometer() {
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345_ADDR, 6, true);

  x = Wire.read() | (Wire.read() << 8);
  y = Wire.read() | (Wire.read() << 8);
  z = Wire.read() | (Wire.read() << 8);
}
```

## Uploading the Code

1. Install the Arduino extension for Visual Studio Code.
2. Open the `earthquake_detector.ino` file in VS Code.
3. Select the Arduino Uno board and the correct COM port.
4. Click the upload button or use the command palette to upload the code to the Arduino.

## Usage

Once the code is uploaded, the device will start monitoring for seismic activity. The LCD will display the status, and the LEDs and buzzer will provide visual and audible alerts based on the detected motion.

- **Normal Condition**: Green LED on, no buzzer sound.
- **Mild Shake**: Yellow LED on, short buzzer sound.
- **Strong Shake (Earthquake Alert)**: Red LED on, continuous buzzer sound.

## License

This project is licensed under the MIT License.
