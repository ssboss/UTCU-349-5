//Interface Pins
const int PIN_TXEN     = 0;
const int PIN_DPSK     = 1;  
const int PIN_TO       = 5;   
const int PIN_FRO      = 7;   
const int PIN_ANTSELRD = 9;   

// Antena select bits
const int PIN_ANT0 = 10;     
const int PIN_ANT1 = 11;    
const int PIN_ANT2 = 12;     

// Antenna values
const int ANT_DATA = 0;
const int ANT_SCAN = 4;
const int ANT_OFF  = 5;

// Timing
const unsigned long SEQUENCE_TIME_MS = 615;
unsigned long sequenceStart = 0;

// flags to make sure each event is only happening once
bool didTO = false;
bool didData = false;
bool didFRO = false;
bool didOff = false;


const char testData[] = "0000000000000111010011001"; // AZ preamble

void setup() {
  pinMode(PIN_TXEN, OUTPUT);
  pinMode(PIN_DPSK, OUTPUT);
  pinMode(PIN_TO, OUTPUT);
  pinMode(PIN_FRO, OUTPUT);
  pinMode(PIN_ANTSELRD, OUTPUT);

  pinMode(PIN_ANT0, OUTPUT);
  pinMode(PIN_ANT1, OUTPUT);
  pinMode(PIN_ANT2, OUTPUT);

  allSignalsLow();
  setAntenna(ANT_OFF);

  sequenceStart = millis();
}

void loop() {
  unsigned long t = millis() - sequenceStart;

  if (t >= SEQUENCE_TIME_MS) {
    sequenceStart = millis();
    didTO = false;
    didData = false;
    didFRO = false;
    didOff = false;
    allSignalsLow();
  }

  // timing layout

  // To scan section
  if (t >= 0 && !didTO) {
    setAntenna(ANT_SCAN);
    pulseMs(PIN_ANTSELRD);
    pulseMs(PIN_TO);
    didTO = true;
  }

  // Data section
  if (t >= 100 && !didData) {
    setAntenna(ANT_DATA);
    pulseMs(PIN_ANTSELRD);
    pulseMs(PIN_TXEN);
    sendDPSKBits(testData);
    didData = true;
  }

  // Fro scan section
  if (t >= 250 && !didFRO) {
    setAntenna(ANT_SCAN);
    pulseMs(PIN_ANTSELRD);
    pulseMs(PIN_FRO);
    didFRO = true;
  }

  // Turning antenna off section
  if (t >= 400 && !didOff) {
    setAntenna(ANT_OFF);
    pulseMs(PIN_ANTSELRD);
    didOff = true;
  }
}


// This function makes a short 1ms pulse
void pulseMs(int pin) {
  digitalWrite(pin, HIGH);
  delay(1); // short pulse
  digitalWrite(pin, LOW);
}

// This function writes a 3-bit antenna number
void setAntenna(int value) {
  digitalWrite(PIN_ANT0, value & 0x01);
  digitalWrite(PIN_ANT1, (value >> 1) & 0x01);
  digitalWrite(PIN_ANT2, (value >> 2) & 0x01);
}

// This function will send the binary data as differential DPSK values
// Pretty much reading if the binary data switches from 0 to 1 or 1 to 0
void sendDPSKBits(const char *bits) {
  char previous = '0';

  for (int i = 0; bits[i] != '\0'; i++) {
    if (bits[i] == '1') {
      previous = (previous == '0') ? '1' : '0';
    }

    digitalWrite(PIN_DPSK, previous == '1' ? HIGH : LOW);

    delayMicroseconds(100); // bit time placeholder
  }

  digitalWrite(PIN_DPSK, LOW);
}


// resets all singals to low at end of cycle
void allSignalsLow() {
  digitalWrite(PIN_TXEN, LOW);
  digitalWrite(PIN_DPSK, LOW);
  digitalWrite(PIN_TO, LOW);
  digitalWrite(PIN_FRO, LOW);
  digitalWrite(PIN_ANTSELRD, LOW);
}
