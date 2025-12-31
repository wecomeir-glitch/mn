/*
 * IR Debug Test Sketch
 * Purpose: Diagnose IR reception issues with detailed serial debugging output
 * Date: 2025-12-31
 * 
 * This sketch monitors IR receiver input and logs all signals for debugging
 */

// IR Receiver pin (adjust based on your setup)
#define IR_RECEIVER_PIN 2

// Serial debugging configuration
#define BAUD_RATE 9600
#define DEBUG_ENABLED true

// Signal detection thresholds
#define MIN_PULSE_WIDTH 10      // Minimum pulse width in microseconds
#define MAX_PULSE_WIDTH 150000  // Maximum pulse width in microseconds

// Global variables
volatile unsigned long lastSignalTime = 0;
volatile unsigned long signalStartTime = 0;
volatile boolean signalReceived = false;
unsigned int signalCount = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(BAUD_RATE);
  
  // Configure IR receiver pin
  pinMode(IR_RECEIVER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(IR_RECEIVER_PIN), irInterrupt, CHANGE);
  
  // Print startup information
  Serial.println("===========================================");
  Serial.println("      IR RECEIVER DEBUG TEST STARTED");
  Serial.println("===========================================");
  Serial.print("Baud Rate: ");
  Serial.println(BAUD_RATE);
  Serial.print("IR Receiver Pin: ");
  Serial.println(IR_RECEIVER_PIN);
  Serial.print("Timestamp (UTC): 2025-12-31 20:17:02");
  Serial.println();
  Serial.println("Waiting for IR signals...");
  Serial.println("-----------");
  Serial.println();
}

void loop() {
  // Check if a signal was received
  if (signalReceived) {
    signalCount++;
    
    Serial.print("[SIGNAL #");
    Serial.print(signalCount);
    Serial.print("] ");
    Serial.print("Time: ");
    printCurrentTime();
    Serial.print(" | Pin State: ");
    Serial.println(digitalRead(IR_RECEIVER_PIN) ? "HIGH" : "LOW");
    
    Serial.print("  ├─ Start Time: ");
    Serial.print(signalStartTime);
    Serial.println(" µs");
    
    Serial.print("  ├─ Duration: ");
    unsigned long duration = lastSignalTime - signalStartTime;
    Serial.print(duration);
    Serial.println(" µs");
    
    Serial.print("  ├─ Frequency Est: ");
    if (duration > 0) {
      unsigned long freq = 1000000 / (duration * 2);
      Serial.print(freq);
      Serial.println(" Hz");
    } else {
      Serial.println("N/A");
    }
    
    Serial.print("  └─ Status: ");
    if (duration >= MIN_PULSE_WIDTH && duration <= MAX_PULSE_WIDTH) {
      Serial.println("✓ VALID");
    } else {
      Serial.println("✗ OUT OF RANGE");
    }
    
    Serial.println();
    
    // Reset the signal flag
    signalReceived = false;
  }
  
  // Optional: Print periodic status message
  static unsigned long lastStatusPrint = 0;
  if (millis() - lastStatusPrint > 5000) {
    Serial.print("[STATUS] Signals received: ");
    Serial.print(signalCount);
    Serial.print(" | Uptime: ");
    Serial.print(millis() / 1000);
    Serial.println(" seconds");
    lastStatusPrint = millis();
  }
  
  delay(10);
}

/*
 * Interrupt handler for IR receiver signal changes
 * Triggered on both rising and falling edges
 */
void irInterrupt() {
  unsigned long currentTime = micros();
  
  if (digitalRead(IR_RECEIVER_PIN) == LOW) {
    // Falling edge - signal started
    signalStartTime = currentTime;
  } else {
    // Rising edge - signal ended
    lastSignalTime = currentTime;
    signalReceived = true;
  }
}

/*
 * Helper function to print current time in human-readable format
 * Note: This uses millis() - for real timestamps, implement RTC
 */
void printCurrentTime() {
  unsigned long uptime = millis();
  unsigned long seconds = (uptime / 1000) % 60;
  unsigned long minutes = (uptime / 60000) % 60;
  unsigned long hours = (uptime / 3600000) % 24;
  
  if (hours < 10) Serial.print("0");
  Serial.print(hours);
  Serial.print(":");
  if (minutes < 10) Serial.print("0");
  Serial.print(minutes);
  Serial.print(":");
  if (seconds < 10) Serial.print("0");
  Serial.print(seconds);
  Serial.print(" (+");
  Serial.print(uptime / 1000);
  Serial.print("s)");
}

/*
 * Optional: Diagnostic function to check IR receiver health
 * Call this manually via serial command if needed
 */
void checkIRReceiverHealth() {
  Serial.println("\n=== IR RECEIVER HEALTH CHECK ===");
  
  Serial.print("Pin ");
  Serial.print(IR_RECEIVER_PIN);
  Serial.print(" State: ");
  Serial.println(digitalRead(IR_RECEIVER_PIN) ? "HIGH" : "LOW");
  
  Serial.print("Total Signals Received: ");
  Serial.println(signalCount);
  
  Serial.println("Receiver appears to be: ");
  if (digitalRead(IR_RECEIVER_PIN) == HIGH) {
    Serial.println("  ✓ READY (idle state is HIGH)");
  } else {
    Serial.println("  ⚠ ACTIVE (receiving signal)");
  }
  
  Serial.println("================================\n");
}

// Future extension: Add serial command handler for diagnostics
// void handleSerialCommand() {
//   if (Serial.available()) {
//     char cmd = Serial.read();
//     if (cmd == 'h') {
//       checkIRReceiverHealth();
//     } else if (cmd == 'r') {
//       signalCount = 0;
//       Serial.println("Signal counter reset.");
//     }
//   }
// }
