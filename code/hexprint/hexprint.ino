#include <IRremote.hpp>

const int IR_RECEIVE_PIN = 2;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println(F("Point your remote at the receiver and press each button once."));
}

void loop() {
  if (IrReceiver.decode()) {

    // Basic decoded fields
    uint16_t address = IrReceiver.decodedIRData.address;
    uint16_t command = IrReceiver.decodedIRData.command;
    uint32_t raw     = IrReceiver.decodedIRData.decodedRawData;

    // One compact line per press
    Serial.print(F("RAW=0x"));
    Serial.print(raw, HEX);
    Serial.print(F("  ADDR=0x"));
    Serial.print(address, HEX);
    Serial.print(F("  CMD=0x"));
    Serial.print(command, HEX);
    Serial.print(F("  PROTO="));
    Serial.print(IrReceiver.getProtocolString());
    Serial.println();

    // Also show library helper outputs (optional, but handy)
    IrReceiver.printIRResultShort(&Serial);      // summary line
    Serial.println();
    IrReceiver.printIRResultAsCVariables(&Serial); // ready-to-copy C defines
    Serial.println();
    IrReceiver.printIRSendUsage(&Serial);        // how to send this code back out
    Serial.println();
    Serial.println(F("--------------------------------------------------"));

    IrReceiver.resume();  // ready for next button
  }
}
