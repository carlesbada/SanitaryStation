#include <Adafruit_MLX90614.h>
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>


#define echoPin 5 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define GreenLed 8
#define YellowLed 10
#define RedLed 9
#define Relay 7

LiquidCrystal_I2C lcd(0x27,20,4); // set lcd A4 SDA - A5 SLC
Adafruit_MLX90614 mlx = Adafruit_MLX90614(); // set temperature sensor SLC SDA

// defines variables
int State; // variable defining the state of the action
int Temp; // variable for temperature measurement
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
float temperaturaObjeto; // variable for temperature

void setup() {
  
  pinMode(trigPin, OUTPUT);// Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  pinMode(GreenLed, OUTPUT);
  pinMode(YellowLed, OUTPUT);
  pinMode(RedLed, OUTPUT);
  pinMode(Relay, OUTPUT);
  Serial.begin(9600);      // Serial Communication is starting with 9600 of baudrate speed
  mlx.begin();             // Temperature sensor
  lcd.init();              // initialize the lcd 
  lcd.init();
  lcd.clear();
  lcd.backlight();
  State = 0; // rest state as initial
  Temp = 0;  // temp  control as 0 initial
}

void loop() {
  Serial.print(distance);
  if (State == 0) { // if there is no hand detected:
    lcd.setCursor(1,0);
    lcd.print("Station ready");
    lcd.setCursor(1,1);
    lcd.print("Waiting user");
    // Clears the trigPin condition
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

    if (distance < 7 && distance > 1){
      State = 1;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Hand Detected");
    }

  }
  if (State == 1) { // if hand has been detected:
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Hand detected");
    lcd.setCursor(1,1);
    lcd.print("Measure temp...");
    while (Temp == 0) {
      temperaturaObjeto= mlx.readObjectTempC()+7;
      if (temperaturaObjeto>34 && temperaturaObjeto<45){
        Temp = 1;
      }
      else{
        Temp = 0;
      }
    }
    if (temperaturaObjeto<37.0){
      digitalWrite(GreenLed,HIGH);
      Serial.print("greeen");
    } else if (temperaturaObjeto<38){
      digitalWrite(YellowLed,HIGH);
    } else {
      digitalWrite(RedLed,HIGH);
    }
    
    // Print temperature in the LCD
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("BODY TEMPERATURE");
    lcd.setCursor(1,1);
    lcd.print(temperaturaObjeto);
    lcd.setCursor(4,1);
    lcd.print((char)223);
    lcd.setCursor(5,1);
    lcd.print("C");
    delay(1000);

    // Give the advise of the gel supply
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("SUPPLYING GEL");
    lcd.setCursor(1,1);
    lcd.print("IN");
    lcd.setCursor(4,1);
    lcd.print("3");
    lcd.setCursor(6,1);
    lcd.print("SECONDS");
    delay(1000);
    lcd.setCursor(4,1);
    lcd.print("2");
    delay(1000);
    lcd.setCursor(4,1);
    lcd.print("1");
    delay(1500);

    digitalWrite(Relay, HIGH); // Close relay to activate the pump
    delay(500); // During 1 sec give gel
    digitalWrite(Relay, LOW); // Open relay to close the pump
    
    State = 0; // turn state to rest
    Temp = 0;  // turn temp to original
    lcd.clear(); // clean the LCD
    digitalWrite(GreenLed,LOW);
    digitalWrite(YellowLed,LOW);
    digitalWrite(RedLed,LOW);
  }
}


    

   
    


 
