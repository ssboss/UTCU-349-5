// UTCU CODE

// Timescale
#define k 4

// The current station:
// 0 = ALL, 1 = AZ, 2 = BAZ, 3 = EL
#define STATION 0

// 0 = regular, 1 = high rate approach
#define SEQUENCE 0

//Interface Pins
const int PIN_TXEN = 0;
const int PIN_DPSK = 1;  
const int PIN_TO = 7;   
const int PIN_FRO = 9;   
const int PIN_ANTSELRD = 4;  
const int PIN_SEQSRT = 3;

// Antenna select bits
const int PIN_ANT0 = 11;     
const int PIN_ANT1 = 12;    
const int PIN_ANT2 = 13;     

// Antenna values
const int ANT_DATA = 0;
const int ANT_SCAN = 4;
const int ANT_OFF  = 5;

// Timing
const unsigned long SEQUENCE_TIME_MS = 615;
unsigned long sequenceStart = 0;
const unsigned long PULSE_WIDTH = 20;
const unsigned long BIT_WIDTH = 64;
const unsigned long PULSE_LOW = BIT_WIDTH - PULSE_WIDTH;


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
const char haz_preamble[] = "0000000000000111010010100"; // HAZ pweamble :p

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
  pinMode(PIN_SEQSRT, OUTPUT);

  pinMode(PIN_ANT0, OUTPUT);
  pinMode(PIN_ANT1, OUTPUT);
  pinMode(PIN_ANT2, OUTPUT);

  allSignalsLow();
  setAntenna(ANT_OFF);
}

void loop() {

  if (SEQUENCE == 0){
    regSeq();
  } else {
    highAzApSeq();
  }
}


// This function makes a short 1ms pulse
// void pulseUs(int pin) {
//   digitalWrite(pin, HIGH);
//   delayMicroseconds(20 * k); // short pulse
//   digitalWrite(pin, LOW);
// }

// Regular sequence
void regSeq(){
    digitalWrite(PIN_SEQSRT, HIGH);
    delayMicroseconds(1);
    digitalWrite(PIN_SEQSRT, LOW);


    sequence1();
    sequence2();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw1_preamble);
    sendDPSKBits(bdw1_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw2_preamble);
    sendDPSKBits(bdw2_content);
    digitalWrite(PIN_TXEN, LOW);

    sequence1();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw3_preamble);
    sendDPSKBits(bdw3_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw4_preamble);
    sendDPSKBits(bdw4_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw5_preamble);
    sendDPSKBits(bdw5_content);
    digitalWrite(PIN_TXEN, LOW);

    sequence2();
    sequence1();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw6_preamble);
    sendDPSKBits(bdw6_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw1_preamble);
    sendDPSKBits(bdw1_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw2_preamble);
    sendDPSKBits(bdw2_content);
    digitalWrite(PIN_TXEN, LOW);

    sequence2();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw3_preamble);
    sendDPSKBits(bdw3_content);
    digitalWrite(PIN_TXEN, LOW);

    sequence1();
    sequence2();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw4_preamble);
    sendDPSKBits(bdw4_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw5_preamble);
    sendDPSKBits(bdw5_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw6_preamble);
    sendDPSKBits(bdw6_content);
    digitalWrite(PIN_TXEN, LOW);

    delay(1000);
}

void highAzApSeq(){
    digitalWrite(PIN_SEQSRT, HIGH);
    delayMicroseconds(1);
    digitalWrite(PIN_SEQSRT, LOW);


    sequence1High();
    sequence2High();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw1_preamble);
    sendDPSKBits(bdw1_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw2_preamble);
    sendDPSKBits(bdw2_content);
    digitalWrite(PIN_TXEN, LOW);

    sequence1High();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw3_preamble);
    sendDPSKBits(bdw3_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw4_preamble);
    sendDPSKBits(bdw4_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw5_preamble);
    sendDPSKBits(bdw5_content);
    digitalWrite(PIN_TXEN, LOW);

    sequence2High();
    sequence1High();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw6_preamble);
    sendDPSKBits(bdw6_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw1_preamble);
    sendDPSKBits(bdw1_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw2_preamble);
    sendDPSKBits(bdw2_content);
    digitalWrite(PIN_TXEN, LOW);

    sequence2High();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw3_preamble);
    sendDPSKBits(bdw3_content);
    digitalWrite(PIN_TXEN, LOW);

    sequence1High();
    sequence2High();

    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw4_preamble);
    sendDPSKBits(bdw4_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw5_preamble);
    sendDPSKBits(bdw5_content);
    digitalWrite(PIN_TXEN, LOW);
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw6_preamble);
    sendDPSKBits(bdw6_content);
    digitalWrite(PIN_TXEN, LOW);

    delay(1000);
}


void pulseUs(int pin) {
  unsigned long start = micros();

  // Serial.println("OOOOOHHHH Im pulsing :P");
  digitalWrite(pin, HIGH);
  //while (micros() - start > PULSE_WIDTH*k);
  delayMicroseconds(PULSE_WIDTH * k);
  digitalWrite(pin, LOW);
  //while(micros() - start > PULSE_LOW*k);
  delayMicroseconds(PULSE_LOW * k);
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
  char prevBit = '0';

  for (int i = 0; bits[i] != '\0'; i++) {
    if ((bits[i] == '1' && prevBit == '0') || (bits[i] == '0' && prevBit == '1')) {
      pulseUs(PIN_DPSK);
    } else {
      delayMicroseconds(BIT_WIDTH * k);
    }
    prevBit = bits[i];
  }
}


// resets all singals to low at end of cycle
void allSignalsLow() {
  digitalWrite(PIN_TXEN, LOW);
  digitalWrite(PIN_DPSK, LOW);
  digitalWrite(PIN_TO, LOW);
  digitalWrite(PIN_FRO, LOW);
  digitalWrite(PIN_ANTSELRD, LOW);
}

// -------------------------------------------
// EL, BDW1, AZ, BDW2, EL, BDW3, BAZ, BDW4, EL
void azFunc() {
  // Start of sequence
  allSignalsLow();
  digitalWrite(PIN_TXEN, HIGH);
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  

  // timing layout

  // Data section
  sendDPSKBits(az_preamble);
  digitalWrite(PIN_TXEN, LOW);
  digitalWrite(PIN_TXEN, HIGH);

  // antenna Selection phase,
  // I am setting the antenna to the correct switch position and then the time between switch
  // positions is 2 bit width so i so 64 times 2 and then times our scale factor of 4
  
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * 5 * k);

  setAntenna(1);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(2);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(3);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  // setAntenna(5);
  // pulseUs(PIN_ANTSELRD);
  // digitalWrite(PIN_ANTSELRD, LOW);

  // To scan section
  digitalWrite(PIN_ANT0, 1);
  digitalWrite(PIN_ANT1, 0);
  digitalWrite(PIN_ANT2, 0);
  pulseUs(PIN_TO);
  delayMicroseconds((6200 * k) - (BIT_WIDTH * k)); 
  digitalWrite(PIN_TXEN, LOW);


  // wait time between to and fro scans
  delayMicroseconds(600 * k);


  // Fro scan section
  digitalWrite(PIN_TXEN, HIGH);
  pulseUs(PIN_FRO);
  delayMicroseconds((6200 * k) - (BIT_WIDTH * k));

 

  // Turning antenna off section
  //setAntenna(ANT_OFF);
  //pulseUs(PIN_ANTSELRD);
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  digitalWrite(PIN_TXEN, LOW);

  delayMicroseconds(212 * k);
}


// -------------------------------------------
void elFunc(){
  // Start of sequence
  allSignalsLow();
  digitalWrite(PIN_TXEN, HIGH);
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  
  // timing layout

  // Data section
  //setAntenna(ANT_DATA);
  //pulseUs(PIN_ANTSELRD);
  //sendDPSKBits(el_preamble);
  //didData = true;

  sendDPSKBits(el_preamble);
  digitalWrite(PIN_TXEN, LOW);
  digitalWrite(PIN_TXEN, HIGH);

  // Antenna sweep
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * 5 * k);

  setAntenna(1);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(2);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(3);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  // Lets USSIM know it enable el beam
  digitalWrite(PIN_ANT0, 0);
  digitalWrite(PIN_ANT1, 1);
  digitalWrite(PIN_ANT2, 0);

  // To scan section
  pulseUs(PIN_TO);
  delayMicroseconds((1550 * k) - (BIT_WIDTH * k));
  digitalWrite(PIN_TXEN, LOW);


  // wait time between to and fro scans
  delayMicroseconds(600 * k);


  // Fro scan section
  digitalWrite(PIN_TXEN, HIGH);
  pulseUs(PIN_FRO);
  delayMicroseconds((1550 * k) - (BIT_WIDTH * k));
 

  // Turning antenna off section
  //setAntenna(ANT_OFF);
  //pulseUs(PIN_ANTSELRD);
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  digitalWrite(PIN_TXEN, LOW);
  delayMicroseconds(244 * k);
}


//-------------------------------------
void bazFunc() {
  // Start of sequence
  allSignalsLow();
  digitalWrite(PIN_TXEN, HIGH);
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  

  // timing layout

  // Data section
  //sendDPSKBits(baz_preamble);
  //didData = true;

  sendDPSKBits(baz_preamble);
  digitalWrite(PIN_TXEN, LOW);
  digitalWrite(PIN_TXEN, HIGH);

  // Antenna sweep
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * 5 * k);

  setAntenna(1);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(2);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(3);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  // Lets USSIM know to enables BAZ scanning beam
  digitalWrite(PIN_ANT0, 0);
  digitalWrite(PIN_ANT1, 0);
  digitalWrite(PIN_ANT2, 1);

  // To scan section
  pulseUs(PIN_TO);
  delayMicroseconds((4200 * k) - (BIT_WIDTH * k));
  digitalWrite(PIN_TXEN, LOW);


  // wait time between to and fro scans
  delayMicroseconds(600 * k);


  // Fro scan section
  digitalWrite(PIN_TXEN, HIGH);
  pulseUs(PIN_FRO);
  delayMicroseconds((4200 * k) - (BIT_WIDTH * k));
 

  // Turning antenna off section
  //setAntenna(ANT_OFF);
  //pulseUs(PIN_ANTSELRD);
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  digitalWrite(PIN_TXEN, LOW);
  delayMicroseconds(212 * k);
}

//-------------------------------------
void hazFunc() {
  // Start of sequence
  allSignalsLow();
  digitalWrite(PIN_TXEN, HIGH);
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  

  // timing layout

  // Data section
  //sendDPSKBits(baz_preamble);
  //didData = true;

  sendDPSKBits(haz_preamble);
  digitalWrite(PIN_TXEN, LOW);
  digitalWrite(PIN_TXEN, HIGH);

  // Antenna sweep
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * 5 * k);

  setAntenna(1);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(2);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(3);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  delayMicroseconds(64 * k);

  // Lets USSIM know to enables BAZ scanning beam
  digitalWrite(PIN_ANT0, 0);
  digitalWrite(PIN_ANT1, 0);
  digitalWrite(PIN_ANT2, 1);

  // To scan section
  pulseUs(PIN_FRO);
  delayMicroseconds((4200 * k) - (BIT_WIDTH * k));
  digitalWrite(PIN_TXEN, LOW);


  // wait time between to and fro scans
  delayMicroseconds(600 * k);


  // Fro scan section
  digitalWrite(PIN_TXEN, HIGH);
  pulseUs(PIN_TO);
  delayMicroseconds((4200 * k) - (BIT_WIDTH * k));
 

  // Turning antenna off section
  //setAntenna(ANT_OFF);
  //pulseUs(PIN_ANTSELRD);
  setAntenna(0);
  pulseUs(PIN_ANTSELRD);
  digitalWrite(PIN_TXEN, LOW);
  delayMicroseconds(212 * k);
}

// EL, BDW1, AZ, BDW2, EL, BDW3, BAZ, BDW4, EL
void sequence1() {

  // For all functions and datawords in both sequence one and two,
  // I check for the station and if the function runs in said station,
  // I run that function, if not, I delay for the time it would take to
  // run the function.

  if (STATION == 3 || STATION== 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }
  
  if (STATION == 1 || STATION == 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw1_preamble);
    sendDPSKBits(bdw1_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
    delayMicroseconds(2200 * k);
  } else {
    delay(3.1 * k);
    delayMicroseconds(2200 * k);
  }

  if (STATION == 1 || STATION == 0){
    azFunc();
  } else {
    delay(15.9 * k);
  }

  if (STATION == 1 || STATION == 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw2_preamble);
    sendDPSKBits(bdw2_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
    delayMicroseconds(2200 * k);
  } else {
    delay(3.1 * k);
    delayMicroseconds(2200 * k);
  }


  if (STATION == 3 || STATION == 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }

  if (STATION == 1 || STATION == 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw3_preamble);
    sendDPSKBits(bdw3_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
    delayMicroseconds(2200 * k);
  } else {
    delay(3.1 * k);
  }

  if (STATION == 2 || STATION == 0){
    bazFunc();
  } else {
    delay(11.9 * k);
  }

  if (STATION == 1 || STATION == 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw4_preamble);
    sendDPSKBits(bdw4_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
  } else {
    delay(3.1 * k);
  }

  if (STATION == 3 || STATION == 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }

  delay(8 * k);
}

// EL, BDW5, AZ, BDW6, EL, AUXDATA, EL
void sequence2() {
  if (STATION == 3 || STATION == 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }

  if (STATION == 1 || STATION == 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw5_preamble);
    sendDPSKBits(bdw5_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
    delayMicroseconds(2200 * k);
  } else {
    delay(3.1 * k);
    delayMicroseconds(2200 * k);
  }

  if (STATION == 1 || STATION == 0){
    azFunc();
  } else {
    delay(15.9 * k);
  }

  if (STATION == 1 || STATION == 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw6_preamble);
    sendDPSKBits(bdw6_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
    delayMicroseconds(2200 * k);
  } else {
    delay(3.1 * k);
    delayMicroseconds(2200 * k);
  }

  if (STATION == 3 || STATION == 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }


  if (STATION == 1 || STATION == 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(adw_premable);
    sendDPSKBits(adw_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(204 * k);
    delayMicroseconds(2200 * k);
  } else {
    delay(5.9 * k);
    delayMicroseconds(2200 * k);
  }

  if (STATION == 3 || STATION == 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }

  delay(18 * k);
}

void sequence1High(){
  if (STATION == 3 || STATION== 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }

  if (STATION == 1 || STATION== 0){
    hazFunc();
  } else {
    delay(11.9 * k);
  }

  if (STATION == 1 || STATION== 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw1_preamble);
    sendDPSKBits(bdw1_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
  } else {
    delay(3.1 * k);
  }

  if (STATION == 1 || STATION== 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw3_preamble);
    sendDPSKBits(bdw3_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
  } else {
    delay(3.1 * k);
  }

  if (STATION == 1 || STATION== 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw4_preamble);
    sendDPSKBits(bdw4_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
  } else {
    delay(3.1 * k);
  }

  if (STATION == 1 || STATION== 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw5_preamble);
    sendDPSKBits(bdw5_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
  } else {
    delay(3.1 * k);
  }

  if (STATION == 1 || STATION == 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw6_preamble);
    sendDPSKBits(bdw6_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
  } else {
    delay(3.1 * k);
  }

  if (STATION == 1 || STATION== 0){
    hazFunc();
  } else {
    delay(11.9 * k);
  }

  if (STATION == 3 || STATION== 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }


  if (STATION == 1 || STATION== 0){
    hazFunc();
  } else {
    delay(11.9 * k);
  }

  if (STATION == 3 || STATION== 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }

  delay(9 * k);
}

void sequence2High() {
  if (STATION == 3 || STATION== 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }

  if (STATION == 1 || STATION== 0){
    hazFunc();
  } else {
    delay(11.9 * k);
  }

  if (STATION == 1 || STATION == 0){
    digitalWrite(PIN_TXEN, HIGH);
    sendDPSKBits(bdw2_preamble);
    sendDPSKBits(bdw2_content);
    digitalWrite(PIN_TXEN, LOW);
    delayMicroseconds(220 * k);
  } else {
    delay(3.1 * k);
  }

  if (STATION == 2 || STATION == 0){
    bazFunc();
  } else {
    delay(11.9 * k);
  }

  if (STATION == 1 || STATION== 0){
    hazFunc();
  } else {
    delay(11.9 * k);
  }

  if (STATION == 3 || STATION== 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }

  if (STATION == 1 || STATION== 0){
    hazFunc();
  } else {
    delay(11.9 * k);
  }

  if (STATION == 3 || STATION== 0){
    elFunc();
  } else {
    delay(5.6 * k);
  }
}
