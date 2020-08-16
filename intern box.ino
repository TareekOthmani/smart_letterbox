void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
    if (Serial.available() > 0) {
     char car1 = Serial.read();
     switch (car1) {
      case 1:
      digitalWrite(13, HIGH);
      delay(300);
      digitalWrite(13, LOW);
      delay(300);
      digitalWrite(13, HIGH);
      delay(300);
      digitalWrite(13, LOW);
      delay(300);
      digitalWrite(13, HIGH);
      delay(300);
      digitalWrite(13, LOW);
      delay(300);
     break;
     default:
     digitalWrite(13,LOW);
     break;

}
    }
}
