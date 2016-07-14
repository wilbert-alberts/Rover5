
volatile long leftCyclecounter = 0;
volatile long leftNrCycles = 0;
volatile long rightCyclecounter = 0;
volatile long rightNrCycles = 0;

void runConfiguration(uint16_t lp , long ln, uint16_t rp, long rn);
void getConfiguration(uint16_t* periodInUs , long* nrCycles, const char* side);
bool waitUntilReadyToRun();
void setTimer4(uint16_t periodInUs , long nrCycles);
void setTimer5(uint16_t periodInUs , long nrCycles);
void startTimers(int* lc, int* rc);

const char* leftSide = "left";
const char* rightSide = "right";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(10000);
}

void loop() {
  uint16_t leftPeriod;
  long     leftNrCycles;
  uint16_t rightPeriod;
  long     rightNrCycles;
  bool     readyToRun;

  getConfiguration(&leftPeriod, &leftNrCycles, leftSide);
  getConfiguration(&rightPeriod, &rightNrCycles, rightSide);
  readyToRun = waitUntilReadyToRun();
  if (readyToRun)
    runConfiguration(leftPeriod, leftNrCycles, rightPeriod, rightNrCycles);
}

void getConfiguration(uint16_t* periodInUs , long* nrCycles, const char* side)
{
  long p = 0;
  int c = 0;
  char r = 'n';
  String n;

  p = 0;
  c = 0;
  Serial.print("Enter length of ");
  Serial.print(side);
  Serial.println(" period:");
  while (p == 0) {
    n = Serial.readStringUntil('\n');
    p = n.toInt();
  }

  Serial.print("Enter nr of ");
  Serial.print(side);
  Serial.println(" cycles:");
  while (c == 0)
    c = Serial.parseInt();

  *periodInUs = p;
  *nrCycles = c;
}

bool waitUntilReadyToRun()
{
  char r;

  Serial.println("Ready to go (y/n)?");
  r = 0;
  do {
    r = Serial.read();
  } while ((r != 'y') and (r != 'n'));
  return r == 'y';
}
void runConfiguration(uint16_t lp , long ln, uint16_t rp, long rn)
{
  leftCyclecounter = 0;
  rightCyclecounter = 0;
  leftNrCycles = ln;
  rightNrCycles = rn;
  setTimer4(lp);
  setTimer5(rp);

  Serial.println("Starting:");
  Serial.print("   Left Period: ");
  Serial.print(lp);
  Serial.print(", Reft Period: ");
  Serial.println(rp);
  Serial.print("   Left Nr. Cycles: ");
  Serial.print(ln);
  Serial.print(", Right Nr. Cycles: ");
  Serial.println(rn);

  startTimers();
  while ((leftCyclecounter < leftNrCycles) || (rightCyclecounter < rightNrCycles)) {
    delay(100);
  }
  
}


void setTimer4(uint16_t periodInUs)
{
  // Set pins to output
  DDRH |= _BV(DDH3);
  DDRH |= _BV(DDH4);


  // Toggle output pin on match
  TCCR4A &= ~(_BV(COM4A1) | _BV(COM4B1));
  TCCR4A |= (_BV(COM4A0) | _BV(COM4B0));

  // Waveform generator: Clear on Timer Compare (CTC)
  TCCR4A &= ~(_BV(WGM41) | _BV(WGM40));
  TCCR4B &= ~(_BV(WGM42) | _BV(WGM43));
  TCCR4B |= _BV(WGM42);

  // Enable interrupt when period elapsed
  TIMSK4 |= _BV(OCIE4A);

  // Prescaler is set to 8
  // One tick 0.5 us; one period is 2*periodInUs ticks
  uint16_t period = 2 * periodInUs;

  // Set compare values to toggle pin
  OCR4A = period;
  OCR4B = period / 2;

}

void setTimer5(uint16_t periodInUs)
{
  // Set pins to output
  DDRL |= _BV(DDL3);
  DDRL |= _BV(DDL4);

  // Toggle output pin on match
  TCCR5A &= ~(_BV(COM5A1) | _BV(COM5B1));
  TCCR5A |= (_BV(COM5A0) | _BV(COM5B0));

  // Waveform generator: Clear on Timer Compare (CTC)
  TCCR5A &= ~(_BV(WGM51) | _BV(WGM50));
  TCCR5B &= ~(_BV(WGM52) | _BV(WGM53));
  TCCR5B |= _BV(WGM52);

  // Enable interrupt when period elapsed
  TIMSK5 |= _BV(OCIE5A);

  // Prescaler is set to 8
  // One tick 0.5 us; one period is 2*periodInUs ticks
  uint16_t period = 2 * periodInUs;

  // Set compare values to toggle pin
  OCR5A = period;
  OCR5B = period / 2;
}

void startTimers()
{
  // Initialize timer to zero
  TCNT4 = 0;
  TCNT5 = 0;
  // Set prescaler to 8, effectively starts timer.
  TCCR4B &= ~(_BV(CS42) | _BV(CS41) | _BV(CS40));
  TCCR5B &= ~(_BV(CS52) | _BV(CS51) | _BV(CS50));
  
  //TCCR4B |=  _BV(CS40); //Prescaler 1
  //TCCR4B |=  _BV(CS41); //Prescaler 8
  TCCR4B |=  (_BV(CS41) | _BV(CS40)); //Prescaler 64
  //TCCR4B |=  (_BV(CS42) | _BV(CS40)); // Prescaler 1024

  //TCCR5B |=  _BV(CS50); //Prescaler 1
  //TCCR5B |=  _BV(CS51); //Prescaler 8
  TCCR5B |=  (_BV(CS51) | _BV(CS50)); //Prescaler 64
  //TCCR5B |=  (_BV(CS52) | _BV(CS50)); // Prescaler 1024
}


ISR(TIMER4_COMPA_vect)
{
  leftCyclecounter++;
  if (leftCyclecounter == leftNrCycles) {
    // Stop timer 4
    TCCR4B &= ~(_BV(CS42) | _BV(CS41) | _BV(CS40));
    TIMSK4 &= ~_BV(OCIE4A);
  }
}


ISR(TIMER5_COMPA_vect)
{
  rightCyclecounter++;
  if (rightCyclecounter == rightNrCycles) {
    // Stop timer 5
    TCCR5B &= ~(_BV(CS52) | _BV(CS51) | _BV(CS50));
    TIMSK5 &= ~_BV(OCIE5A);
  }
}

