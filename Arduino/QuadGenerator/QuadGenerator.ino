
#define PA (5)
#define PB (6)

#define TOTAL (1000)


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  
  // put your main code here, to run repeatedly:
  digitalWrite(PA, HIGH);
  delayMicroseconds(TOTAL/4);
  digitalWrite(PB, HIGH);
  delayMicroseconds(TOTAL/4);
  digitalWrite(PA, LOW);
  delayMicroseconds(TOTAL/4);
  digitalWrite(PB, LOW);
  delayMicroseconds(TOTAL/4);
}


