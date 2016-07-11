
volatile long nrCycles = 0;
volatile long maxNrCycles = 0;
volatile bool finished = false;

void runConfiguration(uint16_t periodInUs , long nrCycles);
void getConfiguration(uint16_t* periodInUs , long* nrCycles);
void setPeriodAndCompare(uint16_t periodInUs , long nrCycles);
void startTimer();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
}

void loop() {
  uint16_t period;
  long     nrCycles;

  getConfiguration(&period, &nrCycles);
  runConfiguration(period, nrCycles);
}

void getConfiguration(uint16_t* periodInUs , long* nrCycles)
{
  long p = 0;
  int c = 0;
  char r = 'n';
  String n;

  Serial.setTimeout(10000);
  while (r != 'y') {
    p = 0;
    c = 0;
    Serial.println("Enter length of period:");
    while (p == 0) {
      n = Serial.readStringUntil('\n');
      p = n.toInt();
    }

    Serial.println("Enter nr cycles:");
    while (c == 0)
      c = Serial.parseInt();

    Serial.println("Ready to go (y/n)?");
    r = 0;
    do {
      r = Serial.read();
    } while ((r != 'y') and (r != 'n'));
  }
  Serial.println("Starting:");
  Serial.print("   Period: ");
  Serial.println(p);
  Serial.print("   Nr. Cycles: ");
  Serial.println(c);


  *periodInUs = p;
  *nrCycles = c;
}
void runConfiguration(uint16_t p , long n)
{
  setPeriodAndCompare(p, n);
  startTimer();
  while (!finished) {
    delay(1);
    //Serial.println(nrCycles);
  }
}


void setPeriodAndCompare(uint16_t periodInUs , long nrCycles)
{
  // Prescaler is set to 8
  Serial.println(periodInUs);
  Serial.println(nrCycles);

  // One tick 0.5 us; one period is 2*periodInUs ticks
  uint16_t period = 2 * periodInUs;

  // Set compare values to toggle pin
  OCR4A = period;
  OCR4B = period / 2;

  // Toggle output pin on match
  TCCR4A &= ~(_BV(COM4A1) | _BV(COM4B1));
  TCCR4A |= (_BV(COM4A0) | _BV(COM4B0));

  // Waveform generator: Clear on Timer Compare (CTC)
  TCCR4A &= ~(_BV(WGM41) | _BV(WGM40));
  TCCR4B &= ~(_BV(WGM42) | _BV(WGM43));
  TCCR4B |= _BV(WGM42);

  // Enable interrupt when period elapsed
  TIMSK4 |= _BV(OCIE4A);

  // Set boundary to stop
  maxNrCycles = nrCycles;
}

void startTimer()
{
  // Initialize timer to zero
  TCNT4 = 0;
  nrCycles = 0;
  finished = false;

  // Set prescaler to 8, effectively starts timer.
  TCCR4B &= ~(_BV(CS42) | _BV(CS41) | _BV(CS40));
  //TCCR4B |=  _BV(CS40); //Prescaler 1
  //TCCR4B |=  _BV(CS41); //Prescaler 8
  TCCR4B |=  (_BV(CS42) | _BV(CS40)); // Prescaler 1024
  

}


ISR(TIMER4_COMPA_vect)
{
  nrCycles++;
  if (nrCycles == maxNrCycles) {
    // Stop timer 4
    TCCR4B &= ~(_BV(CS42) | _BV(CS41) | _BV(CS40));
    TIMSK4 &= ~_BV(OCIE4A);
    finished = true;
  }
}

