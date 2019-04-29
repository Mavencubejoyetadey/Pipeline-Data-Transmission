/*
 * GSM DATA SENT TO SERVER to ANALOG VALUE 0-999
 * SERVER
 * SMS
 * SMS & SERVER
 * date 12th April 2019
 */



#include <SoftwareSerial.h>
  SoftwareSerial myGsm(7,8);


String dataMessage;
int readingCount = 0;
unsigned long lastSMSSentTime = 0;


void setup() {
   myGsm.begin(19200);  
   Serial.begin(19200);  
   delay(500);
}



void analogPin(){ 
    int a = map(analogRead(A0), 0, 1023, 0, 999);
    int b = map(analogRead(A1), 0, 1023, 0, 999);
    int c = map(analogRead(A2), 0, 1023, 0, 999);
    int d = map(analogRead(A3), 0, 1023, 0, 999);
    int e = map(analogRead(A4), 0, 1023, 0, 999);
    int f = map(analogRead(A5), 0, 1023, 0, 999);
    dataMessage += (String(a)+"," + String(b)+"," +String(c)+"," + String(d)+","+String(e)+"," + String(f)+",");   // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
    readingCount++;

    while(readingCount>=5) {              //Sent Message There is no SMS space left from 120 characters
      //Serial.println(dataMessage);
      readingCount = 0;
      return;
    } 
 }


void printSerialData() {
   while(myGsm.available()!=0)
   Serial.write(myGsm.read());
}


void loop() {
  analogPin();
    
  if(readingCount == 0){
    //Serial.println(dataMessage);
       sendServer(); // Sent data to webserver      
       if ((unsigned long)(millis() - lastSMSSentTime) > 2000){
            lastSMSSentTime = millis();
            //sendSMS();  // Sent data to SMS
            Serial.println("SMS SEND");
          }
       dataMessage = "";  // clear 
    }
  delay(100);     
 }


void sendSMS(){    
     myGsm.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
     delay(1000);                   // Delay of 1 second
     myGsm.println("AT+CMGS=\"+919641892607\"\r"); // Replace x with mobile number
     delay(1000);
     myGsm.println(dataMessage);// The SMS text you want to send
     delay(1000);
     myGsm.println((char)26);// ASCII code of CTRL+Z for saying the end of sms to  the module 
}



void sendServer(){

  myGsm.println("AT+CIPSHUT");
  delay(500);
  printSerialData();
 
  myGsm.println("AT+CIPMUX=0");
  delay(1000);
  printSerialData();
  
  myGsm.println("AT+CGATT=1");
  delay(1000);
  printSerialData();
  
  myGsm.println("AT+CSTT=\"www\",\"\",\"\"");//setting the APN,2nd parameter empty works for all networks 
  delay(3000);
  printSerialData();
  
  myGsm.println();
  myGsm.println("AT+CIICR");
  delay(2000);
  printSerialData();
  
  myGsm.println("AT+CIFSR"); //init the HTTP request
  delay(2000); 
  printSerialData();

 
  myGsm.println("AT+CIPSTART=\"TCP\",\"gaziproperties.in\",\"80\"");
  delay(5000);
  printSerialData();

  delay(5000);
  
  myGsm.println("AT+CIPSEND");
  delay(100);
  printSerialData();
  
  myGsm.print("GET /db.php?data=");
  myGsm.print(dataMessage);   // Sent DATA TO WEBSERVER
  myGsm.println(" HTTP/1.1");
  myGsm.println("Host: gaziproperties.in");
  myGsm.println();
  myGsm.println((char)26); //myGsm.write(0x1A);    
    
  delay(3000);
  myGsm.println("AT+CIPCLOSE");
  printSerialData();

  myGsm.println("AT+CIPSHUT");
  delay(1000);
  printSerialData();

 }
