#include <LiquidCrystal.h>

#define Voltage_Divder_Volt A0
#define ammeterVolt A1 
#define readed  A2
#define capacitor  A3
#define probe A4           
#define buzzer_sound A5    


#define RS 2 
#define EN 3
#define D4 4 
#define D5 5
#define D6 6
#define D7 7 

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


//Resistors
#define Shunt_Resistance 10 
#define Series_Resistance  1 
#define Ammeter_Resistance 0.01

int switch5 = 9;
int Switch1 = 10;
int Switch2 = 11;
int Switch3 = 12;
int Switch4 = 13;
int VCC = 8 ; 

int Active_Device = 0 ; 


int Pressed_Button() {
  for (int i = 9 ; i < 14 ; i++) {
    if (digitalRead(i) == LOW) {
      
      return i; 
    }
  }
  return -1;
}

void setup() {
  Serial.begin(9600);
  pinMode(Voltage_Divder_Volt, INPUT);
  pinMode(ammeterVolt, INPUT);
  pinMode(readed, INPUT);
  pinMode(Switch1, INPUT_PULLUP);
  pinMode(Switch2, INPUT_PULLUP);
  pinMode(Switch3, INPUT_PULLUP);
  pinMode(Switch4, INPUT_PULLUP);
  pinMode(switch5, INPUT_PULLUP);
  pinMode(probe, INPUT_PULLUP);
  pinMode(buzzer_sound, OUTPUT);
  pinMode(VCC , OUTPUT) ;
  digitalWrite(VCC , LOW );
  lcd.begin(16, 2);

  
}

void Print_Reading(String Device , float Reading , String Unit , int Fraction_Num = 2 ) {
  lcd.clear();  // Clear previous output
  lcd.setCursor(0, 0);
  lcd.print(Device);

  lcd.setCursor(0, 1);
  lcd.print(Reading, Fraction_Num);  // Print float with 2 decimal places
  lcd.print(" ");
  lcd.print(Unit);
}

void WelcomeMessage(){
  lcd.clear();  // Clear previous output
  lcd.setCursor(0, 0);
  lcd.print("Measure Project");
  lcd.setCursor(0, 1);
  lcd.print("AVO METER");

}


void voltmeter() {
  float voltOut = (analogRead(Voltage_Divder_Volt) * 5.0)/1023;
  float voltIn = voltOut * (Shunt_Resistance + Series_Resistance) / Series_Resistance; 
  Serial.print("The voltage = ");
  Serial.print(voltIn);
  Serial.println(" V");
  Print_Reading("DC Voltameter" , voltIn ,"V");
}

void ammeter() {
  int val = analogRead(ammeterVolt);
  float voltOut = (val * 5.0)/1023;
  float current = (voltOut / Ammeter_Resistance);
  Serial.print("The Current = ");
  Serial.print(current);
  Serial.println(" mA");
  Print_Reading("DC Ammeter" , current ,"mA");

}

void ohmmeter() {
  digitalWrite( VCC , HIGH);
  delay(100) ;
  int val = analogRead(readed);
  float volt = (val * 5.0) / 1023.0;
  float volt_across_unknown = 5.0 - volt;
  float current = volt / 979.0;
  float res = volt_across_unknown / current;
  Serial.print("resistance = ");
  Serial.print(res);
  Serial.println(" ohm");
  digitalWrite( VCC , LOW);
  if (res > 1000) {
     Print_Reading("Ohm-meter" , res/1000 ,"Kohm");
  }
  else {
     Print_Reading("Ohm-meter" , res ,"Ohm");
  }

}

void capacitance_meter() {
  float cap = 0;
  long start_time = 0;
  long time_interval = 0;
  
  digitalWrite(VCC, LOW);
  pinMode(capacitor, OUTPUT);
  digitalWrite(capacitor, LOW);
  delay(500);

  pinMode(capacitor, INPUT);
  start_time = micros();
  digitalWrite(VCC, HIGH);
  while (analogRead(capacitor) < 646) {}

  time_interval = micros() - start_time;
  cap = (float)time_interval / 10000.0;

  Serial.print("capacitance = ");
  Serial.print(cap);
  Serial.println(" uF");
  digitalWrite(VCC, LOW);
  Print_Reading("Capacitance-meter" , cap ," uF");
  delay(100);

}


void buzzer()
{
  int state = 0;
  state = digitalRead(probe);
  
  if (state == LOW) {
    tone(buzzer_sound, 985);
    Print_Reading("Continuity Test" , 1 ," ");
  
  }
  else {noTone(buzzer_sound);Print_Reading("Continuity Test" , 0 ," ");}
  delay(100);
}



void loop() {
  
int newPressed = Pressed_Button();
if (newPressed != -1 && newPressed != Active_Device) {
  Active_Device = newPressed;
}

  

  switch (Active_Device) {
    case 9:
      Serial.println("Now buzzer");
      buzzer();
      break;
    
    case 10:
      noTone(buzzer_sound);
      Serial.println("Now Voltmeter");
      voltmeter();
      break;

    case 11:
      noTone(buzzer_sound);
      Serial.println("Now Ammeter");
      ammeter();
      break;

    case 12:
      noTone(buzzer_sound);
      Serial.println("Now Ohmmeter");
      ohmmeter();
      break;

    case 13:
      noTone(buzzer_sound);
      Serial.println("Now Capacitance");
      capacitance_meter();
      break;

    default:
      WelcomeMessage(); 
      noTone(buzzer_sound);
      break;
  }

  delay(100) ; 
}

