int touch1;
int touch2;
int touch3;
int touch4;
int touch5;
int touch6;
int touch7;
int touch8;

int oldTouchPressure1;
int oldTouchPressure2;
int oldTouchPressure3;
int oldTouchPressure4;
int oldTouchPressure5;
int oldTouchPressure6;
int oldTouchPressure7;
int oldTouchPressure8;


int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;
int state5 = 0;
int state6 = 0;
int state7 = 0;
int state8 = 0;

int pressure = 0;
int oldpressure = 0;

int slide = 0;
int oldslide = 0;

int bite = 0;
int oldbite = 0;

int thresh = 1500;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {

  // which pins to use for touch:
  touch1 = touchRead(0);
  touch2 = touchRead(1);
  touch3 = touchRead(3);
  touch4 = touchRead(4);
  touch5 = touchRead(15);
  touch6 = touchRead(16);
  touch7 = touchRead(17);
  touch8 = touchRead(23);

  printVals();


 * pressure = analogRead(A0) / 8;
  slide = analogRead(A1) / 8;
  bite = constrain(analogRead(A2)/4-1,0,127);

  //Serial.println(bite);
  
  if (pressure != oldpressure) {
    usbMIDI.sendControlChange(1, pressure, 1);
  }

  oldpressure = pressure;
  
  if (slide != oldslide) {
    usbMIDI.sendControlChange(2, slide * -1 + 127, 1);
  }

  oldslide = slide;

    if (bite != oldbite) {
    usbMIDI.sendControlChange(3, bite, 1);
  }

  oldbite = bite;



  

    if (touch1 != oldTouchPressure1) {
    usbMIDI.sendControlChange(4,constrain(((touch1-1100)/50), 0, 127),1);
    oldTouchPressure1 = touch1;
  }

      if (touch2 != oldTouchPressure2) {
    usbMIDI.sendControlChange(5,constrain(((touch2-1100)/50), 0, 127),1);
    oldTouchPressure2 = touch2;
  }

        if (touch3 != oldTouchPressure3) {
    usbMIDI.sendControlChange(6,constrain(((touch3-1100)/50), 0, 127),1);
    oldTouchPressure3 = touch3;
  }
*/


  if (touch1 > thresh && state1 == 0) {
    state1 = 1;
    usbMIDI.sendNoteOn(1, 127, 1);
  }
  if (touch1 < thresh && state1 == 1) {
    state1 = 0;
    usbMIDI.sendNoteOff(1, 0, 1);
  }

  if (touch2 > thresh && state2 == 0) {
    state2 = 1;
    usbMIDI.sendNoteOn(2, 127, 1);
  }
  if (touch2 < thresh && state2 == 1) {
    state2 = 0;
    usbMIDI.sendNoteOff(2, 0, 1);
  }

  if (touch3 > thresh && state3 == 0) {
    state3 = 1;
    usbMIDI.sendNoteOn(3, 127, 1);
  }
  if (touch3 < thresh && state3 == 1) {
    state3 = 0;
    usbMIDI.sendNoteOff(3, 0, 1);
  }

  if (touch4 > thresh && state4 == 0) {
    state4 = 1;
    usbMIDI.sendNoteOn(4, 127, 1);
  }
  if (touch4 < thresh && state4 == 1) {
    state4 = 0;
    usbMIDI.sendNoteOff(4, 0, 1);
  }

  
  if (touch5 > thresh && state5 == 0) {
    state5 = 1;
    usbMIDI.sendNoteOn(5, 127, 1);
  }
  if (touch5 < thresh && state5 == 1) {
    state5 = 0;
    usbMIDI.sendNoteOff(5, 0, 1);
  }

  
  if (touch6 > thresh && state6 == 0) {
    state6 = 1;
    usbMIDI.sendNoteOn(6, 127, 1);
  }
  if (touch6 < thresh && state6 == 1) {
    state6 = 0;
    usbMIDI.sendNoteOff(6, 0, 1);
  }

  
  if (touch7 > thresh && state7 == 0) {
    state7 = 1;
    usbMIDI.sendNoteOn(7, 127, 1);
  }
  if (touch7 < thresh && state7 == 1) {
    state7 = 0;
    usbMIDI.sendNoteOff(7, 0, 1);
  }

  if (touch8 > thresh && state8 == 0) {
    state8 = 1;
    usbMIDI.sendNoteOn(8, 127, 1);
  }
  if (touch8 < thresh && state8 == 1) {
    state8 = 0;
    usbMIDI.sendNoteOff(8, 0, 1);
  }


}

void printVals() {
  Serial.println(" ");
  Serial.print("Touch settings: ");
  Serial.print(state1);
  Serial.print(" ");
  Serial.print(state2);
  Serial.print(" ");
  Serial.print(state3);
  Serial.print(" ");
  Serial.print(state4);
    Serial.print(" ");
  Serial.print(state5);
    Serial.print(" ");
  Serial.print(state6);
    Serial.print(" ");
  Serial.print(state7);
  Serial.print(" ");
  Serial.print(state8);
  Serial.print(" pressure ");
  Serial.print(pressure / 8);

    Serial.print(" ");
  Serial.print(constrain(((touch1-1100)/50), 0, 127));
  Serial.print(" ");
  Serial.print(constrain(((touch2-1100)/50), 0, 127));
  Serial.print(" ");
  Serial.print(constrain(((touch3-1100)/50), 0, 127));
  Serial.print(" ");
  Serial.print(constrain(((touch4-1100)/50), 0, 127));
  Serial.print(" ");
  Serial.print(constrain(((touch5-1100)/50), 0, 127));
  Serial.print(" ");
  Serial.print(constrain(((touch6-1100)/50), 0, 127));
  Serial.print(" ");
  Serial.print(constrain(((touch7-1100)/50), 0, 127));
  Serial.print(" ");
  Serial.print(constrain(((touch8-1100)/50), 0, 127));
  Serial.print(" ");
  
     Serial.println(" ");
}
