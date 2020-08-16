#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include<SPI.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(1, 0);

#define SS_PIN 31
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
LiquidCrystal lcd(12, 11, 10, 9, 8, 7); //(rs, enable, d4, d5, d6, d7)
Servo myservo;

int cap1 = 39;
int cap2 = 36;
int led1 = 37;
int led2 = 35;
int inputPin = 3;
int buz = A2;
int pos = 0;
int val = 0;
int pirState = LOW;
String content = "";


int test = 0;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(100);
  SPI.begin();
  mfrc522.PCD_Init();   // Initiate MFRC522
  lcd.begin(20, 4);
  myservo.attach(4);
  // Print a message to the LCD.
  lcd.clear();
  lcd.print("bonjour");
  pinMode(cap1, INPUT);
  pinMode(cap2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(led2, OUTPUT);
  pinMode(buz, OUTPUT);
  myservo.write(90);
}



void loop() {


  if (!(digitalRead(cap1)) or (!(digitalRead(cap2)))) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("il y'a un");
    lcd.setCursor(1, 5);
    lcd.print("un courriel");


    SendMessage();

    val = digitalRead(inputPin);  // read input value
    if (val == HIGH) {            // check if the input is HIGH
      if (pirState == LOW) {
        // we have just turned on
        Serial.println("Motion detected!");
        tone(buz, 1000); // Send 1KHz sound signal...
        delay(500);
        noTone(buz);
        delay(200);
        tone(buz,1000);
        delay(500);
        noTone(buz);
        // We only want to print on the output change, not state
        pirState = HIGH;
      }
    }
    else if (val == LOW) {
      noTone(buz);     // Stop sound...
      if (pirState == HIGH) {
        // we have just turned of
        Serial.println("Motion ended!");
        noTone(buz);     // Stop sound...
        // We only want to print on the output change, not state
        pirState = LOW;
      }
    }

    Serial1.write('4');



    if ( ! mfrc522.PICC_IsNewCardPresent())
    {
      return;
    }
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
    {
      return;
    }
    //Show UID on serial monitor
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("UID tag :");
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    lcd.setCursor(0, 2);
    lcd.print(content.substring(1));

    if (content.substring(1) == "A3 C1 1E 20") //change here the UID of the card/cards that you want to give access
    {
      digitalWrite(led2, HIGH);
      lcd.setCursor(0, 3);
      lcd.print("Authorized");
      delay(500);
      content = "";

      for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      delay(5000);
      for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
      }
      digitalWrite(led2, LOW);
    }
    else   {
      
      content = "";
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("CARD NOT");
      lcd.setCursor(2, 2);
      lcd.print("Authorised");
      lcd.setCursor(3, 3);
      lcd.print("try again");
      digitalWrite(led1, HIGH);
      delay(5000);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("bonjour");
      digitalWrite(led1, LOW);
    }
  }

  else {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("bonjour");
    Serial1.write('0');
  }

}



void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"0021653655723\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("I am SMS from GSM Module");// The SMS text you want to send
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
