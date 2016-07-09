
#define PA (18)
#define PB (19)

#define LED (13)

int d= 0;

#define TOTAL (5000UL)

void waitPeriod(long  usecs);

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(PA, OUTPUT);
  pinMode(PB, OUTPUT);
  pinMode(LED, OUTPUT);

  Serial.println("Setup finished.");
}

void loop() {
  d = 1 - d;
  Serial.println(d);
  digitalWrite(LED, d);
  
  // put your main code here, to run repeatedly:
  digitalWrite(PA, HIGH);
  waitPeriod(TOTAL/4);
  digitalWrite(PB, HIGH);
  waitPeriod(TOTAL/4);
  digitalWrite(PA, LOW);
  waitPeriod(TOTAL/4);
  digitalWrite(PB, LOW);
  waitPeriod(TOTAL/4);
}

void waitPeriod(long usecs)
{
  if (usecs > 1000) {
    delay(usecs/1000);
  }
  else {
    delayMicroseconds(usecs);
  }
}

