int luz;
int led;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(3, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  luz=analogRead(A0);
  Serial.println(luz);
  led= map(luz, 0, 600, 0, 100);
  analogWrite(3,led);
}
