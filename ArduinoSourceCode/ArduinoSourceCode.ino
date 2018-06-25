//*****************************************
//https://github.com/DerLarsus/rollingBox
//*****************************************
#include <Servo.h>
Servo servo1;
Servo servo2;

//Ultrasonic Sensor 1 (HC-SR04)
#define triggerFront 3 //HC-SR04 Trig
#define echoFront 2    //HC-SR04 Echo

//Ultrasonic Sensor 2 (HC-SR04)
#define triggerBack 5 //HC-SR04 Trig
#define echoBack 4    //HC-SR04 Echo

char command; 

void setup() 
{       
  //begin serial
  Serial.begin(9600); 

  //Ultrasonic Sensor 1 setup
  pinMode(triggerFront, OUTPUT);
  pinMode(echoFront, INPUT);
  digitalWrite(triggerFront, HIGH); 

  //Ultrasonic Sensor 2 setup
  pinMode(triggerBack, OUTPUT);
  pinMode(echoBack, INPUT);
  digitalWrite(triggerBack, HIGH); 
}

void loop(){
  //observed obstacles
  int averageDistanceFront= getAverageDistanceFront();
  int averageDistanceBack= getAverageDistanceBack();
  if(averageDistanceFront<=6){
        stopVehicle();
        delay(1000);
        servo1.attach(9);
        servo2.attach(10);
        servo1.write(180); 
        servo2.write(0); 
        delay(2000);
        stopVehicle();
        
      } else if(averageDistanceBack<6){
        stopVehicle();
        delay(1000);
        servo1.attach(9);
        servo2.attach(10);    
        servo1.write(0);
        servo2.write(180);
        delay(2000);
        stopVehicle();
        } else {
          //handle input for navigation
          if(Serial.available() > 0){     
            command = Serial.read(); 
            stopVehicle(); //initialize with motors stoped
              switch(command){
              case '1':  
                forward();      
                break;
              case '2':  
                back();     
                break;
              case '3':  
                left();     
                break;
              case '4':
                right();   
                break;
              case '5':
                stopVehicle();
                break;
            }
        } 
    }
}

void forward(){
  servo1.attach(9);
  servo2.attach(10); 
   
  servo1.write(0);
  servo2.write(180);
}

void back(){
  servo1.attach(9);
  servo2.attach(10);  
  
  servo1.write(180);
  servo2.write(0);
}

void left(){
  servo1.attach(9);
  servo2.attach(10);
  
  servo1.write(180); 
  servo2.write(180); 
}

void right(){
  servo1.attach(9);
  servo2.attach(10);  
  
  servo1.write(0); 
  servo2.write(0); 
}

void stopVehicle(){
  servo1.detach();
  servo2.detach();
}


int getDistanceFront(){ 
  long distance=0;
  long timeValue=0;

  digitalWrite(triggerFront, LOW); 
  delayMicroseconds(3);
  noInterrupts();
  digitalWrite(triggerFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerFront, LOW); 
  timeValue = pulseIn(echoFront, HIGH);
  interrupts(); 
 
  timeValue = (timeValue/2);
  distance = timeValue / 29.1; // time in cm
  
  return(distance); 
}

int getAverageDistanceFront(){ 
  int temp=0;
  int average;
  int distance;

  delay(10);
  temp=getDistanceFront();
  delay(10);
  for (int i=0; i<10; i++){
    distance=getDistanceFront();
    average=(0.8*temp) + (0.2*distance);
    temp=average;
    delay(10);
  }
  return (average);
}

int getDistanceBack(){ 
  long distance=0;
  long timeValue=0;

  digitalWrite(triggerBack, LOW); 
  delayMicroseconds(3);
  noInterrupts();
  digitalWrite(triggerBack, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerBack, LOW); 
  timeValue = pulseIn(echoBack, HIGH);
  interrupts(); 
 
  timeValue = (timeValue/2);
  distance = timeValue / 29.1; //time in cm
  
  return(distance); 
}

int getAverageDistanceBack(){ 
  int temp=0;
  int average;
  int distance;

 delay(10);
 temp=getDistanceBack();
 delay(10);
 for (int i=0; i<10; i++){
   distance=getDistanceBack();
   average=(0.8*temp) + (0.2*distance);
   temp=average;
   delay(10);
  }
  return (average);
}
