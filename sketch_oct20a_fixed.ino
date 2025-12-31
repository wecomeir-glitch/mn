#include <IRremote.hpp>

// Undefine REPEAT macro from IRremote to avoid conflict with DFPlayerMini_Fast
#ifdef REPEAT
#undef REPEAT
#endif

#include <DFPlayerMini_Fast.h>
#include <SoftwareSerial.h>

// Pin definitions
const int IR_RECEIVER_PIN = 11;
const int DF_PLAYER_RX_PIN = 9;
const int DF_PLAYER_TX_PIN = 10;

// Software serial for DFPlayer
SoftwareSerial dfSerial(DF_PLAYER_RX_PIN, DF_PLAYER_TX_PIN);

// IR Receiver setup
IRrecv irReceiver(IR_RECEIVER_PIN);
decode_results irResults;

// DFPlayer setup
DFPlayerMini_Fast dfPlayer;

void setup() {
  Serial.begin(9600);
  
  // Initialize IR receiver
  irReceiver.enableIRIn();
  Serial.println("IR Receiver initialized");
  
  // Initialize DFPlayer
  dfSerial.begin(9600);
  dfPlayer.begin(dfSerial);
  
  Serial.println("DFPlayer initialized");
  Serial.println("System ready");
}

void loop() {
  // Check for IR signals
  if (irReceiver.decode(&irResults)) {
    handleIRCommand(irResults.command);
    irReceiver.resume();
  }
  
  // Check DFPlayer status
  if (dfPlayer.available()) {
    uint8_t type = dfPlayer.readType();
    uint16_t value = dfPlayer.read();
    handleDFPlayerEvent(type, value);
  }
}

void handleIRCommand(uint32_t command) {
  Serial.print("IR Command received: 0x");
  Serial.println(command, HEX);
  
  // Map IR commands to DFPlayer functions
  switch(command) {
    case 0x1:  // Play (example command code)
      dfPlayer.play(1);
      Serial.println("Playing track 1");
      break;
      
    case 0x2:  // Next track
      dfPlayer.next();
      Serial.println("Next track");
      break;
      
    case 0x3:  // Previous track
      dfPlayer.previous();
      Serial.println("Previous track");
      break;
      
    case 0x4:  // Volume up
      dfPlayer.volumeUp();
      Serial.println("Volume up");
      break;
      
    case 0x5:  // Volume down
      dfPlayer.volumeDown();
      Serial.println("Volume down");
      break;
      
    case 0x6:  // Pause/Resume
      dfPlayer.pause();
      Serial.println("Pause/Resume");
      break;
      
    default:
      Serial.println("Unknown IR command");
      break;
  }
}

void handleDFPlayerEvent(uint8_t type, uint16_t value) {
  // Handle DFPlayer events
  switch(type) {
    case TimeOut:
      Serial.println("DFPlayer timeout");
      break;
      
    case WrongStack:
      Serial.println("DFPlayer stack error");
      break;
      
    case DFPlayerCardInserted:
      Serial.println("SD card inserted");
      break;
      
    case DFPlayerCardRemoved:
      Serial.println("SD card removed");
      break;
      
    case DFPlayerTrackFinished:
      Serial.print("Track finished: ");
      Serial.println(value);
      break;
      
    default:
      break;
  }
}
