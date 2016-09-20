#define clock 3
#define data 4
#define left 6
#define right 7
#define alarm 11
#define redlight 13
#define bluelight 12

// Variable for temperature
float tempC;
float tempR;
int tempPin = 0;
int minTemp;//minimum temperatur
int maxTemp;//maximum temperatur
boolean celcius = true;

// Variable for light sensor
const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to
int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

//varible for 7 segment
byte start = B10000000;
byte zero  = B01111110;
byte one   = B00000110;
byte two   = B11011010;
byte three = B11010110;
byte four  = B10100110;
byte five  = B11110100;
byte six   = B11111100;
byte seven = B01000110;
byte eight = B11111110;
byte nine  = B11110110;

//other variable
boolean stopProtector = false;
String inString = "";

void setup()
{
  pinMode(clock, OUTPUT); // make the clock pin an output
  pinMode(data , OUTPUT); // make the data pin an output3
  pinMode(left, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(redlight, OUTPUT);
  pinMode(bluelight, OUTPUT);
  pinMode(alarm, OUTPUT);
  digitalWrite(left, LOW);
  digitalWrite(right, LOW);

  // Button
  pinMode(2, INPUT);

  Serial.begin(9600);
  attachInterrupt(0, changeState, FALLING);
  shiftOut(data, clock, LSBFIRST, start);
  setting();

}

void setting()
{
  Serial.println("================================");
  Serial.println("   Plant Protector Started");
  Serial.println("================================");
  Serial.println("Pilih suhu celcius minimum:");
  bool finish = true;
  while (finish) {
    while (Serial.available() > 0) {
      int inChar = Serial.read();
      if (isDigit(inChar)) {
        // convert the incoming byte to a char
        // and add it to the string:
        inString += (char)inChar;
      }
      // if you get a newline, print the string,
      // then the string's value:
      if (inChar == '\n') {
        finish = false;
        Serial.print("Value:");
        minTemp = inString.toInt();
        Serial.println(minTemp);
        // clear the string for new input:
        inString = "";
      }
    }
  }
  Serial.println("Pilih suhu celcius maksimum:");
  finish = true;
  while (finish) {
    while (Serial.available() > 0) {
      int inChar = Serial.read();
      if (isDigit(inChar)) {
        // convert the incoming byte to a char
        // and add it to the string:
        inString += (char)inChar;
      }
      // if you get a newline, print the string,
      // then the string's value:
      if (inChar == '\n') {
        finish = false;
        Serial.print("Value:");
        maxTemp = inString.toInt();
        Serial.println(maxTemp);
        // clear the string for new input:
        inString = "";
      }
    }
  }

  Serial.println("Type \"stop\" to stop the Plant Protector");
}

void loop()
{
  while (Serial.available()>0) { //Wait for user input
    String input=Serial.readString();
    if(input== "stop\n"){
        stopProtector=true;
        Serial.println("================================");
        Serial.println("   Plant Protector Stopped");
        Serial.println("================================");
        digitalWrite(left, LOW);
        digitalWrite(right, LOW);
        shiftOut(data, clock, LSBFIRST, start);
        delay(500);
        digitalWrite(left, HIGH);
        digitalWrite(right, HIGH);
        digitalWrite(redlight, LOW);
        digitalWrite(bluelight, LOW);
        digitalWrite(alarm, LOW);
        
    } else if(input == "start\n") {
        stopProtector=false;
        digitalWrite(left, LOW);
        digitalWrite(right, LOW);
        shiftOut(data, clock, LSBFIRST, start);
        setting();
    }
  }

  if (!stopProtector) {
    int first_digit = 0;
    int second_digit = 0;

    tempC = analogRead(tempPin);
    if (celcius) {
      tempC = tempC * 0.48828125;
      first_digit = tempC / 10;
      second_digit = (int) tempC % 10;
    } else {
      tempR = tempC * 0.48828125 * 0.8;
      first_digit = tempR / 10;
      second_digit = (int) tempR % 10;
    }

    byte first_byte;
    byte second_byte;

    if (first_digit == 0) {
      first_byte = zero;
    } else if (first_digit == 1) {
      first_byte = one;
    } else if (first_digit == 2) {
      first_byte = two;
    } else if (first_digit == 3) {
      first_byte = three;
    } else if (first_digit == 4) {
      first_byte = four;
    } else if (first_digit == 5) {
      first_byte = five;
    } else if (first_digit == 6) {
      first_byte = six;
    } else if (first_digit == 7) {
      first_byte = seven;
    } else if (first_digit == 8) {
      first_byte = eight;
    } else if (first_digit == 9) {
      first_byte = nine;
    }

    if (second_digit == 0) {
      second_byte = zero;
    } else if (second_digit == 1) {
      second_byte = one;
    } else if (second_digit == 2) {
      second_byte = two;
    } else if (second_digit == 3) {
      second_byte = three;
    } else if (second_digit == 4) {
      second_byte = four;
    } else if (second_digit == 5) {
      second_byte = five;
    } else if (second_digit == 6) {
      second_byte = six;
    } else if (second_digit == 7) {
      second_byte = seven;
    } else if (second_digit == 8) {
      second_byte = eight;
    } else if (second_digit == 9) {
      second_byte = nine;
    }

    if (celcius) {
      if (tempC < minTemp || tempC > maxTemp)
      {
        digitalWrite(alarm, HIGH);
      }else{
        digitalWrite(alarm, LOW);
      }
    } else {
      if (tempR < (minTemp * 0.8) || tempR > (maxTemp * 0.8))
      {
        digitalWrite(alarm, HIGH);
      }else{
       
        digitalWrite(alarm, LOW);
      }
    }

    // read the analog in value:
    sensorValue = analogRead(analogInPin);
    // map it to the range of the analog out:
    outputValue = map(sensorValue, 0, 1023, 0, 255);

    if (outputValue < 30) {
      digitalWrite(bluelight, HIGH);
    } else {
      digitalWrite(bluelight, LOW);
    }

    if (!celcius) {
      digitalWrite(redlight, HIGH);
    } else {
      digitalWrite(redlight, LOW);
    }

    shiftOut(data, clock, LSBFIRST, first_byte);
    delay(10);
    digitalWrite(left, LOW);
    digitalWrite(right, HIGH);
    shiftOut(data, clock, LSBFIRST, second_byte);
    delay(10);
    digitalWrite(left, HIGH);
    digitalWrite(right, LOW);
  } 
}

void changeState() {
  celcius = !celcius;
  if(celcius) {
     Serial.println("Temperature switched to Celcius"); 
  } else {
     Serial.println("Temperature switched to Reamur");
  }
}
