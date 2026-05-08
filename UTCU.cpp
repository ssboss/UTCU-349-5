//Interface Pins
const int PIN_TXEN = 0;
const int PIN_DPSK = 1;  
const int PIN_TO = 5;   
const int PIN_FRO = 7;   
const int PIN_ANTSELRD = 9;   

// Antenna select bits
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

// wait time between to and fro scan (600 micro seconds)
const unsigned long WAIT_TIME_US = 600;
bool waitingForFRO = false;
unsigned long toStartMicros = 0;

//antenna const -> pins [D10][D11][D12]
const int left_ant[] = {0,0,1}; // left OCI
const int rear_ant[] = {0,1,0}; // rear OCI
const int right_ant[] = {0,1,1}; // right OCI
const int off_ant[] = {1,0,1}; // off state
const int all_ant[] = {0,0,0}; //all functions state

// preambles (we can tech combine bdw/adw preamble and contents together, but I have them separate)
const char az_preamble[] = "0000000000000111010011001"; // AZ preamble
const char el_preamble[] = "0000000000000111011100001"; // EL preamble
const char baz_preamble[] = "0000000000000111011001001"; // BAZ premable

const char bdw1_preamble[] = "0000000000000111010101000"; // bdw1 preamble
const char bdw2_preamble[] = "0000000000000111010111100"; // bdw2 preamble
const char bdw3_preamble[] = "0000000000000111011010000"; // bdw3 preamble
const char bdw4_preamble[] = "0000000000000111011000100"; // bdw4 preamble
const char bdw5_preamble[] = "0000000000000111011101100"; // bdw5 preamble
const char bdw6_preamble[] = "0000000000000111010001101"; // bdw6 preamble

const char adw_premable[] = "0000000000000111011110010"; // adw preamble

// data/aux word contents
const char bdw1_content[] = "10111100111001011111"; // bdw1 content
const char bdw2_content[] = "10000110010010100100"; // bdw2 content
const char bdw3_content[] = "11100101101011011000"; // bdw3 content
const char bdw4_content[] = "11110010111001010101"; // bdw4 content
const char bdw5_content[] = "01110011011010011010"; // bdw5 content
const char bdw6_content[] = "10000000001010000110"; // bdw6 content

const char adw_content[] = "0000111110011010000000111001111111111001000101100011001111000000"; // adw content


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


  // Start of sequence
  if (t >= SEQUENCE_TIME_MS) {
    sequenceStart = millis();
    didTO = false;
    didData = false;
    didFRO = false;
    didOff = false;
    allSignalsLow();
    digitalWrite(PIN_TXEN, HIGH);
  }

  // timing layout

  // Data section
  if (t >= 0 && !didData) {
    setAntenna(ANT_DATA);
    pulseUs(PIN_ANTSELRD);
    sendDPSKBits(az_preamble);
    didData = true;
  }

  // To scan section
  if (t >= 100 && !didTO) {
    setAntenna(ANT_SCAN);
    pulseUs(PIN_ANTSELRD);
    pulseUs(PIN_TO);
    didTO = true;
    digitalWrite(PIN_TXEN, LOW);

    toStartMicros = micros();
  }

  // Fro scan section
  if (waitingForFRO && !didFRO && micros() - toStartMicros >= WAIT_TIME_US) {
    digitalWrite(PIN_TXEN, HIGH);
    setAntenna(ANT_SCAN);
    pulseUs(PIN_ANTSELRD);
    pulseUs(PIN_FRO);
    didFRO = true;
    waitingForFRO = false;
  }

  // Turning antenna off section
  if (t >= 400 && !didOff) {
    setAntenna(ANT_OFF);
    pulseUs(PIN_ANTSELRD);
    didOff = true;
  }
}


// This function makes a short 1ms pulse
void pulseUs(int pin) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(20); // short pulse
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


// EL, BDW1, AZ, BDW2, EL, BDW3, BAZ, BDW4, EL
void sequence1() {

}

// EL, BDW5, AZ, BDW6, EL, AUXDATA, EL
void sequence2() {


}
