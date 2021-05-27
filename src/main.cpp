#include <Arduino.h>
#include <SoftwareSerial.h>

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;

SoftwareSerial mySerial(10, 11, false); // RX, TX

void blinky() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
  delay(750);                       // wait for half a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off
  delay(750);                       // wait for half a second
}

void blink( int nbblink  ,  int delai ) {
  Serial.print("blink : ");
  Serial.print(nbblink);
  Serial.print(" -> ");
  Serial.println(delai);
  delay(delai);
  for (int i = 0; i <= nbblink ; i++) {
    blinky();
  }
  delay(delai);
}

void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\r';
  char rc;

  while (mySerial.available() > 0 && newData == false) {
    rc = mySerial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedChars);
    blink(5, 1000);
    newData = false;
  }
}

void setup() {
  Serial.begin(9600);
  //  while (!Serial) {
  //    ; // wait for serial port to connect. Needed for native USB port only
  //  }
  mySerial.begin(9600);
  while (!mySerial) {
    ; // wait for serial port to connect. Needed for native USB port only
  };
  blink(2, 3000);
  Serial.println("<Arduino is ready>");
  blink(5, 3000);
  mySerial.print("* 0 IR 001\r");  //start projector
  mySerial.print("* 0 IR 050\r");   //switch to hdmi

}

void loop() {
  blink(2, 3000);
  //  Serial.println("<--->");
  recvWithEndMarker();
  showNewData();
}

//
//      Serial.write("OKOKOKOKOK\r");
//      Serial.write("* 0 IR 001\r");
//      Serial.write("* 0 IR 002\r");
