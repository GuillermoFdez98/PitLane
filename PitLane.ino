#include <Servo.h>


//====================================================
//
//  Declaración de pines y variables  
// 
//====================================================


// Serial port baudrate
#define SERIAL_BAUDRATE 115200

// Pit lane 1
#define SERVO_3_PIN 7
#define SERVO_5_PIN 9
#define PIT1_BUTTON_PIN 4
#define PIT1_LEFT_SENSOR_PIN A3
#define PIT1_RIGHT_SENSOR_PIN A4
Servo servo_pit1_left, servo_pit1_right;
bool pit1_button = false;
bool pit1_left_sensor = false;
bool pit1_right_sensor = false;
bool pit1_status = false;

// Pit lane 2
#define SERVO_4_PIN 8
#define SERVO_6_PIN 10
#define PIT2_BUTTON_PIN 3
#define PIT2_LEFT_SENSOR_PIN 11
#define PIT2_RIGHT_SENSOR_PIN 12
Servo servo_pit2_left, servo_pit2_right;
bool pit2_button = false;
bool pit2_left_sensor = false;
bool pit2_right_sensor = false;
bool pit2_status = false;

// Track change
#define SERVO_1_PIN 5
#define SERVO_2_PIN 6
#define TRACK_CHANGE_PIN 2
#define TRACK_CHANGE_LIGHT A2
Servo track_change_1, track_change_2;
bool track_change_button;

// Pit light pins
#define PIT_LIGHT_1 A0
#define PIT_LIGHT_2 A1

// Delays
#define ATTACH_DETACH_DELAY 100
#define CLOSE_PIT_DELAY 800


//====================================================
//
//  System initialization
// 
//====================================================


void setup() {
  
  // Inicia el puerto serie para depurar
  Serial.begin(SERIAL_BAUDRATE);

  // Serial monitor OK
  Serial.println("Serial monitor initialized OK");
  
  // Inicializa las luces y las apaga
  pinMode(PIT_LIGHT_1, OUTPUT);
  digitalWrite(PIT_LIGHT_1, LOW);
  pinMode(PIT_LIGHT_2, OUTPUT);
  digitalWrite(PIT_LIGHT_2, LOW);
  pinMode(TRACK_CHANGE_LIGHT, OUTPUT);
  digitalWrite(TRACK_CHANGE_LIGHT, LOW);

  // Inicia los detectores
  pinMode(PIT1_LEFT_SENSOR_PIN, INPUT_PULLUP);
  pinMode(PIT1_RIGHT_SENSOR_PIN, INPUT_PULLUP);
  pinMode(PIT2_LEFT_SENSOR_PIN, INPUT_PULLUP);
  pinMode(PIT2_RIGHT_SENSOR_PIN, INPUT_PULLUP);

  // Modo de los pines de los botones
  pinMode(PIT1_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PIT2_BUTTON_PIN, INPUT_PULLUP);
  pinMode(TRACK_CHANGE_PIN, INPUT);

  // Hace una secuencia de luces de test
  Init_Lights_Sequence();

  // Configura el cambio de los carriles al valor inicial del botón
  if(digitalRead(TRACK_CHANGE_PIN) == HIGH){
    track_change_button = true;
    Two_Tracks();
  }else if(digitalRead(TRACK_CHANGE_PIN) == LOW){
    track_change_button = false;
    One_Track();
  }

  // Default state
  Pit1_Close();
  Pit2_Close();

  // Inicializacion acabada
  Serial.println("System initialization OK");
}


//====================================================
//
//  Pit status management  
// 
//====================================================


void loop() {

  // Reads pit call button status
  if(digitalRead(PIT1_BUTTON_PIN) == LOW && pit1_button == false){
    pit1_button = true;
    Serial.println("PIT 1: button pressed");
    delay(100);
  }
  if(digitalRead(PIT2_BUTTON_PIN) == LOW && pit2_button == false){
    pit2_button = true;
    Serial.println("PIT 2: button pressed");
    delay(100);
  }

  // Read PIT 1 left sensor
  if(digitalRead(PIT1_LEFT_SENSOR_PIN) == LOW && pit1_status == true && pit1_left_sensor == false){
    if(digitalRead(PIT1_LEFT_SENSOR_PIN) == LOW){
      if(digitalRead(PIT1_LEFT_SENSOR_PIN) == LOW){
        if(digitalRead(PIT1_LEFT_SENSOR_PIN) == LOW){
          pit1_left_sensor = true;
          Serial.println("PIT 1: left sensor");
        }
      }
    }
  }
  // Read PIT 1 right sensor
  if(digitalRead(PIT1_RIGHT_SENSOR_PIN) == LOW && pit1_status == true && pit1_right_sensor == false){
    if(digitalRead(PIT1_RIGHT_SENSOR_PIN) == LOW){
      if(digitalRead(PIT1_RIGHT_SENSOR_PIN) == LOW){
        if(digitalRead(PIT1_RIGHT_SENSOR_PIN) == LOW){
          pit1_right_sensor = true;
          Serial.println("PIT 1: right sensor");
        }
      }
    }
  }
  // Read PIT 2 left sensor
  if(digitalRead(PIT2_LEFT_SENSOR_PIN) == LOW && pit2_status == true && pit2_left_sensor == false){
    if(digitalRead(PIT2_LEFT_SENSOR_PIN) == LOW){
      if(digitalRead(PIT2_LEFT_SENSOR_PIN) == LOW){
        if(digitalRead(PIT2_LEFT_SENSOR_PIN) == LOW){
          pit2_left_sensor = true;
          Serial.println("PIT 2: left sensor");
        }
      }
    }
  }
  // Read PIT 2 right sensor
  if(digitalRead(PIT2_RIGHT_SENSOR_PIN) == LOW && pit2_status == true && pit2_right_sensor == false){
    if(digitalRead(PIT2_RIGHT_SENSOR_PIN) == LOW){
      if(digitalRead(PIT2_RIGHT_SENSOR_PIN) == LOW){
        if(digitalRead(PIT2_RIGHT_SENSOR_PIN) == LOW){
          pit2_right_sensor = true;
          Serial.println("PIT 2: right sensor");
        }
      }
    }
  }
  
  // Reads track change button status
  if(digitalRead(TRACK_CHANGE_PIN) == HIGH && track_change_button == false){
    track_change_button = true;
    Serial.println("Two tracks activated");
    Two_Tracks();
  }else if(digitalRead(TRACK_CHANGE_PIN) == LOW && track_change_button == true){
    track_change_button = false;
    Serial.println("One tracks activated");
    One_Track();
  }

  // Controls pit 1 open
  if(pit1_status == false && pit1_button == true){
    pit1_status = true;
    Pit1_Open();
  // Controls pit 1 close
  }else if(pit1_status == true && pit1_left_sensor == true && pit1_right_sensor == true){
    pit1_button = false;
    pit1_status = false;
    Pit1_Close();
    pit1_left_sensor = false;
    pit1_right_sensor = false;
  }

  // Controls pit 2 open
  if(pit2_status == false && pit2_button == true){
    pit2_status = true;
    Pit2_Open();
  // Controls pit 2 close
  }else if(pit2_status == true && pit2_left_sensor == true && pit2_right_sensor == true){
    pit2_button = false;
    pit2_status = false;
    Pit2_Close();
    pit2_left_sensor = false;
    pit2_right_sensor = false;
  }

  // Turn off pit ligths
  if(!pit1_status && !pit2_status){
    Pit_Lights_Off();
  }
}


//====================================================
//
//  Auxiliar functions
// 
//====================================================


// Abre los servos del carril 1
void Pit1_Open(){
  servo_pit1_left.attach(SERVO_3_PIN);
  servo_pit1_right.attach(SERVO_5_PIN);
  delay(ATTACH_DETACH_DELAY);
  servo_pit1_left.write(110);
  servo_pit1_right.write(140);
  Pit_Lights_On();
  delay(ATTACH_DETACH_DELAY);
  servo_pit1_left.detach();
  servo_pit1_right.detach();
}

// Cierra los servos del carril 1
void Pit1_Close(){
  servo_pit1_left.attach(SERVO_3_PIN);
  servo_pit1_right.attach(SERVO_5_PIN);
  delay(CLOSE_PIT_DELAY);
  servo_pit1_left.write(60);
  servo_pit1_right.write(105);
  delay(ATTACH_DETACH_DELAY);
  servo_pit1_left.detach();
  servo_pit1_right.detach();
}

// Abre los servos del carril 2
void Pit2_Open(){
  servo_pit2_left.attach(SERVO_4_PIN);
  servo_pit2_right.attach(SERVO_6_PIN);
  delay(ATTACH_DETACH_DELAY);
  servo_pit2_left.write(50);
  servo_pit2_right.write(50);
  Pit_Lights_On();
  delay(ATTACH_DETACH_DELAY);
  servo_pit2_left.detach();
  servo_pit2_right.detach();
}

// Cierra los servos del carril 2
void Pit2_Close(){
  servo_pit2_left.attach(SERVO_4_PIN);
  servo_pit2_right.attach(SERVO_6_PIN);
  delay(CLOSE_PIT_DELAY);
  servo_pit2_left.write(80);
  servo_pit2_right.write(20);
  delay(ATTACH_DETACH_DELAY);
  servo_pit2_left.detach();
  servo_pit2_right.detach();
}

// Configura el cambio para dos carriles
void Two_Tracks(){
  track_change_1.attach(SERVO_1_PIN);
  track_change_2.attach(SERVO_2_PIN);
  delay(ATTACH_DETACH_DELAY);
  track_change_1.write(90);
  track_change_2.write(90);
  Track_Change_Light_Off();
  delay(ATTACH_DETACH_DELAY);
  track_change_1.detach();
  track_change_2.detach();
  delay(ATTACH_DETACH_DELAY);
}

// Configura el cambio para un carril
void One_Track(){
  track_change_1.attach(SERVO_1_PIN);
  track_change_2.attach(SERVO_2_PIN);
  delay(ATTACH_DETACH_DELAY);
  track_change_1.write(125);
  track_change_2.write(40);
  Track_Change_Light_On();
  delay(ATTACH_DETACH_DELAY);
  track_change_1.detach();
  track_change_2.detach();
  delay(ATTACH_DETACH_DELAY);
}

void Pit_Lights_On(){
  digitalWrite(PIT_LIGHT_1, HIGH);
  digitalWrite(PIT_LIGHT_2, HIGH);
}

void Pit_Lights_Off(){
  digitalWrite(PIT_LIGHT_1, LOW);
  digitalWrite(PIT_LIGHT_2, LOW);
}

void Track_Change_Light_On(){
  digitalWrite(TRACK_CHANGE_LIGHT, HIGH);
}

void Track_Change_Light_Off(){
  digitalWrite(TRACK_CHANGE_LIGHT, LOW);
}

void Init_Lights_Sequence(){
  for(int i = 0; i < 3; i++){
    Pit_Lights_On();
    Track_Change_Light_On();
    delay(200);
    Pit_Lights_Off();
    Track_Change_Light_Off();
    delay(500);
  }
}
