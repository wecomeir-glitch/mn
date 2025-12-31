/*
 * Smart Lamp with PWM Control
 * Updated: 2025-12-31
 * 
 * This sketch demonstrates proper PWM control for LED brightness
 * using analogWrite() on pins 4, 5, and 10 for smooth dimming control
 */

// PWM Control Pins
const int PIN_RED = 4;      // Red LED on PWM pin 4
const int PIN_GREEN = 5;    // Green LED on PWM pin 5
const int PIN_BLUE = 10;    // Blue LED on PWM pin 10

// Brightness variables (0-255)
int redBrightness = 255;
int greenBrightness = 255;
int blueBrightness = 255;

// Control variables
int brightness = 128;       // Overall brightness (0-255)
int colorMode = 0;          // Different color modes
unsigned long lastUpdate = 0;
const int UPDATE_INTERVAL = 50; // Update every 50ms

void setup() {
  // Initialize PWM pins as outputs
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("Smart Lamp PWM Control Initialized");
  Serial.println("Pins 4, 5, 10 configured for PWM output");
}

void loop() {
  // Check if it's time to update
  if (millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();
    
    // Update color mode periodically
    updateColorMode();
    
    // Apply brightness values using PWM
    applyPWM();
  }
}

/**
 * Apply PWM control to RGB LEDs
 * analogWrite() sets PWM duty cycle (0=off, 255=full brightness)
 */
void applyPWM() {
  analogWrite(PIN_RED, redBrightness);
  analogWrite(PIN_GREEN, greenBrightness);
  analogWrite(PIN_BLUE, blueBrightness);
}

/**
 * Cycle through different color modes
 */
void updateColorMode() {
  static unsigned long modeChangeTime = 0;
  
  // Change mode every 5 seconds
  if (millis() - modeChangeTime > 5000) {
    modeChangeTime = millis();
    colorMode = (colorMode + 1) % 5;
  }
  
  // Apply different color patterns
  switch(colorMode) {
    case 0: // White - All on
      redBrightness = brightness;
      greenBrightness = brightness;
      blueBrightness = brightness;
      Serial.println("Mode: White");
      break;
      
    case 1: // Red
      redBrightness = brightness;
      greenBrightness = 0;
      blueBrightness = 0;
      Serial.println("Mode: Red");
      break;
      
    case 2: // Green
      redBrightness = 0;
      greenBrightness = brightness;
      blueBrightness = 0;
      Serial.println("Mode: Green");
      break;
      
    case 3: // Blue
      redBrightness = 0;
      greenBrightness = 0;
      blueBrightness = brightness;
      Serial.println("Mode: Blue");
      break;
      
    case 4: // Pulse effect
      redBrightness = (brightness * (128 + 127 * sin(millis() / 500.0))) / 255;
      greenBrightness = (brightness * (128 + 127 * sin(millis() / 500.0 + 2.09))) / 255;
      blueBrightness = (brightness * (128 + 127 * sin(millis() / 500.0 + 4.19))) / 255;
      Serial.println("Mode: Pulse");
      break;
  }
}

/**
 * Optional: Function to set brightness directly (0-255)
 * Can be called from serial input or other sources
 */
void setBrightness(int level) {
  brightness = constrain(level, 0, 255);
  Serial.print("Brightness set to: ");
  Serial.println(brightness);
}

/**
 * Optional: Set individual color brightness
 */
void setRGBBrightness(int r, int g, int b) {
  redBrightness = constrain(r, 0, 255);
  greenBrightness = constrain(g, 0, 255);
  blueBrightness = constrain(b, 0, 255);
  
  Serial.print("RGB set to: R=");
  Serial.print(redBrightness);
  Serial.print(" G=");
  Serial.print(greenBrightness);
  Serial.print(" B=");
  Serial.println(blueBrightness);
}

// Optional: Handle serial input for manual control
void serialEvent() {
  while (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    if (input.startsWith("B")) { // Brightness command: B128
      int level = input.substring(1).toInt();
      setBrightness(level);
    }
    else if (input.startsWith("RGB")) { // RGB command: RGB255,128,64
      int r = 0, g = 0, b = 0;
      sscanf(input.c_str(), "RGB%d,%d,%d", &r, &g, &b);
      setRGBBrightness(r, g, b);
    }
    else if (input == "HELP") {
      Serial.println("\n=== Smart Lamp Commands ===");
      Serial.println("B<0-255> - Set overall brightness");
      Serial.println("RGB<r>,<g>,<b> - Set individual RGB values");
      Serial.println("Example: B200 or RGB255,0,128");
      Serial.println("============================\n");
    }
  }
}
