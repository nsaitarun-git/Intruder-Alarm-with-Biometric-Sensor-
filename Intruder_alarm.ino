#include <LiquidCrystal.h>
#include <Adafruit_Fingerprint.h>
int rs=6;
int en=7;
int d4=8;
int d5=9;
int d6=10;
int d7=11;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
int motionPin=13;
int ledPin=4; 
int ledPin2=12;
int buzzPin=5;
int motionState;
int ldrPin=A0;
int ldrVal;


SoftwareSerial mySerial(2,3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID=0;
int fID=0;


void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
  pinMode(motionPin,INPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(ledPin2,OUTPUT);
  pinMode(buzzPin,OUTPUT);
  pinMode(ldrPin,INPUT);
  

  lcd.setCursor(3,0);
  lcd.print("Saitarun's");
  lcd.setCursor(1,1);
  lcd.print("Intruder Alarm");
  tone(buzzPin,3000);
  delay(100);
  noTone(buzzPin);
  delay(100);
  tone(buzzPin,3000);
  delay(100);
  noTone(buzzPin);
  delay(3000);
  lcd.clear();
  lcd.print("Please Wait...");
  delay(3000);
  lcd.clear();
  startFingerprintSensor();
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Fetching LDR");
  lcd.setCursor(0,1);
  lcd.print("data...");
  delay(3000);
  lcd.clear();
  


}

void loop() {
  
  motionState=digitalRead(motionPin);
  ldrVal=analogRead(ldrPin);
  getFingerprintID();
  
if(ldrVal>=800){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Night Time");
  lcd.setCursor(0,1);
  lcd.print("Alarm Active");
  digitalWrite(ledPin,HIGH);
   delay(50);
}
 if(ldrVal<=800){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Day Time");
  lcd.setCursor(0,1);
  lcd.print("Alarm Disarmed");
  digitalWrite(ledPin,LOW);
  delay(50);
  
  
  
  }

 if(motionState==0 && ldrVal>=800){
   while(fingerprintID!=1){
    fingerprintID=getFingerprintID();
    lcd.setCursor(0,0);
    lcd.print("Intruder Alert");
    lcd.setCursor(0,1);
    lcd.print("Scan Finger");
    digitalWrite(ledPin,HIGH);
    digitalWrite(ledPin2,LOW);
    delay(100);
    digitalWrite(ledPin,LOW);
    digitalWrite(ledPin2,HIGH);
    tone(buzzPin,2000, 500);
    delay(250);
    noTone(buzzPin);
    lcd.clear();
    if(fingerprintID==1){
      fID=1;
      break;
    }else if(fingerprintID==2){
     fID=2;
     break;
   }else if(fingerprintID==3){
     fID=3;
     break;
   }
   }
   fingerprintID=0;
   lcd.clear();
   digitalWrite(ledPin,LOW);
   lcd.setCursor(3,0);
   lcd.print("Biometrics");
   lcd.setCursor(3,1);
   lcd.print("Confirmed!");
   tone(buzzPin,600,500);
   delay(200);
   noTone(buzzPin);
   delay(200);
   tone(buzzPin,600,500);
   delay(200);
   noTone(buzzPin);
   delay(2000);
   lcd.clear();
   
  IdentifyPerson();

 for(int i=10;i>0;i--){
  lcd.setCursor(0,0);
  lcd.print("Alarm Reset in");
  lcd.setCursor(7,1);
  lcd.print(i);
  tone(buzzPin,450,500);
  delay(100);
  noTone(buzzPin);
  delay(850);
  lcd.clear();
 }
 tone(buzzPin,850,500);
 delay(250);

  }else{
   
    digitalWrite(ledPin2,LOW);
    digitalWrite(buzzPin,LOW);
    
  }

 
  
}
void IdentifyPerson(){
  if(fID==1){
   lcd.setCursor(0,0);
   lcd.print("Name:Saitarun");
   lcd.setCursor(0,1);
   lcd.print("ID:#");
   lcd.setCursor(4,1);
   lcd.print(finger.fingerID);
   lcd.setCursor(6,1);
   lcd.print("Surety:");
   lcd.setCursor(13,1);
   lcd.print(finger.confidence);
   delay(4000);
   lcd.clear();
   }else if(fID==2){
    lcd.setCursor(0,0);
   lcd.print("Name:Charan");
   lcd.setCursor(0,1);
   lcd.print("ID:#");
   lcd.setCursor(4,1);
   lcd.print(finger.fingerID);
   lcd.setCursor(6,1);
   lcd.print("Surety:");
   lcd.setCursor(13,1);
   lcd.print(finger.confidence);
   delay(4000);
   lcd.clear();
   }else if(fID==3){
    lcd.setCursor(0,0);
   lcd.print("Name:Vishali");
   lcd.setCursor(0,1);
   lcd.print("ID:#");
   lcd.setCursor(4,1);
   lcd.print(finger.fingerID);
   lcd.setCursor(6,1);
   lcd.print("Surety:");
   lcd.setCursor(13,1);
   lcd.print(finger.confidence);
   delay(4000);
   lcd.clear();
   }

   fID=0;
}

void startFingerprintSensor()
{
  
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    lcd.print("Found FGPT");
    lcd.setCursor(0,1);
    lcd.print("sensor!");
    tone(buzzPin,600,500);
    delay(100);
    noTone(buzzPin);
    delay(100);
    tone(buzzPin,600,500);
  } else {
    lcd.print("Didn't find FPGT");
    lcd.setCursor(0,1);
    lcd.print("sensor :(");
    tone(buzzPin,200,500);
    delay(100);
    noTone(buzzPin);
    delay(300);
    tone(buzzPin,100,500);
  }
  Serial.println("Waiting for valid finger...");
}

int getFingerprintID() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
