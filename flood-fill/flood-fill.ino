#include <QTRSensors.h>
#include <EEPROM.h>

#define DESTINATION 55
#define JUNCTION_DELAY 125
#define I 8
#define J 7
#define GRID_SIZE I*J

#define NUM_SENSORS  8     // number of sensors used

#define speedturn 180

#define alarm 12
#define driveMode 11
#define rightMotor1 4
#define rightMotor2 5
#define rightMotorPWM 11
#define leftMotor1 7
#define leftMotor2 6
#define leftMotorPWM 10
//#define motorPower A3

QTRSensorsRC qtrc((unsigned char[]) {
  A5, A4, A3, A2, A1, A0 , 2, 3
} , NUM_SENSORS, 2500, QTR_NO_EMITTER_PIN);


float kp = 0.25;
float kd = 0.75;
float ki = 0;
float integrator = 0;
int correction;
int setpoint = 350;
int pos = 0;
int sensorSum = 0;
float error = 0, lasterror = 0, integral = 0;
unsigned int sensors[8];
unsigned int sensors2[8];


int drivePastDelay = 100;
int leftbasespeed = 150;
int rightbasespeed = 150;
int leftmotorspeed = 0;
int rightmotorspeed = 0;

/***********************************************************************************************************/



bool justTurned = false;
bool reversed = false;
bool justStarted = true;
int junctionCount = 0;
int orientation = 0;
int address = 0;
char path[100] = "";
unsigned char path_length = 0;
/***********************************************************************************************************/

void setup()
{
  Serial.begin(9600);
  pinMode(alarm, OUTPUT);
  pinMode(driveMode, INPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  pinMode(rightMotorPWM, OUTPUT);
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(leftMotorPWM, OUTPUT);
  //pinMode(motorPower, OUTPUT);

  delay(2000);
}

void loop()
{
//MazeSolve();
follow_line();
  //readSensors(); 
 // printSensorValues();
}

void readSensors() {
  qtrc.read(sensors2);
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    if (sensors2[i] > 700) {
      sensors[i] = 1;
    } else {
      sensors[i] = 0;
    }
  }
  pos = 0;
  sensorSum = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    pos += sensors[i] * 100 * i;
    sensorSum += sensors[i];
  }
  if (sensorSum != 0) {
    pos /= sensorSum;
  }
  //Serial.println(sensorSum);
}


void printSensorValues() {
  Serial.print(sensors[0]);
  Serial.print(", ");
  Serial.print(sensors[1]);
  Serial.print(", ");
  Serial.print(sensors[2]);
  Serial.print(", ");
  Serial.print(sensors[3]);
  Serial.print(", ");
  Serial.print(sensors[4]);
  Serial.print(", ");
  Serial.print(sensors[5]);
  Serial.print(", ");
  Serial.print(sensors[6]);
  Serial.print(",");
  Serial.println(sensors[7]);
}


void turn(char dir)
{
  Serial.println(dir);
  switch (dir)
  {
    case 'L':
    
      turnLeft();
    case 'R':
      turnRight();
    case 'B':
      turnBack();
    case 'S':
      break;
  }
}


char select_turn(unsigned char found_left, unsigned char found_straight, unsigned char found_right)
{
  if (found_left)
    return 'L';
  else if (found_straight)
    return 'S';
  else if (found_right)
    return 'R';
  else
    return 'B';
} // end select_turn


void turnRight() {
  move(0, 0, 1);
  move(1, 0, 1);
  delay(50);
  move(1, rightbasespeed, 1);
  move(0, leftbasespeed, 0);
  delay(200);
  readSensors();
  while (sensors[6] != 1) {
    readSensors();
  }

  while (sensors[3] != 1) {
    readSensors();
  }
  move(0, 0, 1);
  move(1, 0, 1);
  delay(50);
}

void turnLeft() {
  move(0, 0, 1);
  move(1, 0, 1);
  delay(50);
  move(1, 100, 0);
  move(0, 100, 1);
  delay(200);
  readSensors();
  while (sensors[1] != 1) {
    readSensors();
  }
  while (sensors[4] != 1) {
    readSensors();
  }
  move(0, 0, 1);
  move(1, 0, 1);
  delay(50);
}

void turnBack() {

  move(0, 0, 1);
  move(1, 0, 1);
  delay(50);
  move(1, rightbasespeed, 1);
  move(0, leftbasespeed, 0);
  delay(200);
  readSensors();
  while (sensors[6] != 1) {
    readSensors();
    //printSensorValues();
  }
  while (sensors[3] != 1) {
    readSensors();
    //printSensorValues();
  }

  move(0, 0, 1);
  move(1, 0, 1);
  delay(50);
}


void move(int motor, int speed, int direction) {

  boolean inPin1 = HIGH;
  boolean inPin2 = LOW;

  if (direction == 1) {
    inPin1 = HIGH;
    inPin2 = LOW;
  }
  if (direction == 0) {
    inPin1 = LOW;
    inPin2 = HIGH;
  }

  if (motor == 0) {
    digitalWrite(leftMotor1, inPin1);
    digitalWrite(leftMotor2, inPin2);
    analogWrite(leftMotorPWM, speed);
  }
  if (motor == 1) {
    digitalWrite(rightMotor1, inPin1);
    digitalWrite(rightMotor2, inPin2);
    analogWrite(rightMotorPWM, speed);
  }
}


void MazeSolve()
{
  // Loop until we have solved the maze.
  while (1)
  {
    // FIRST MAIN LOOP BODY
    follow_line();

    // Drive straight a bit.  This helps us in case we entered the
    // intersection at an angle.
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    analogWrite(leftMotorPWM, 100);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    analogWrite(rightMotorPWM, 100);
    delay(25);

    // These variables record whether the robot has seen a line to the
    // left, straight ahead, and right, whil examining the current
    // intersection.
    unsigned char found_left = 0;
    unsigned char found_straight = 0;
    unsigned char found_right = 0;

    // Now read the sensors and check the intersection type.
    readSensors();
    
    // Check for left and right exits.
    if (sensors[0] > 200 && sensors[1] > 200) //  && sensors[2] > 200 && sensors[3] > 200)
      found_right = 1;
    if (sensors[7] > 200 && sensors[6] > 200) // && sensors[5] > 200 && sensors[4] > 200)
      found_left = 1;

    // Drive straight a bit more - this is enough to line up our
    // wheels with the intersection.
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(leftMotor2, LOW);
    analogWrite(leftMotorPWM, 100);
    digitalWrite(rightMotor1, HIGH);
    digitalWrite(rightMotor2, LOW);
    analogWrite(rightMotorPWM, 100);
    delay(drivePastDelay);

    readSensors();
    if (sensors[1] > 200 || sensors[2] > 200 || sensors[3] > 200 || sensors[4] > 200 || sensors[5] > 200 || sensors[6] > 200)
      found_straight = 1;

    if (sensors[0] > 600 && sensors[1] > 600 && sensors[2] > 600 && sensors[3] > 600 && sensors[4] > 600 && sensors[5] > 600 && sensors[6] > 600 && sensors[7] > 600  )
      break;
      
    Serial.print("left right straight : ");
    Serial.println(found_left);
    Serial.println(found_right);
    Serial.println(found_straight);
    unsigned char dir = select_turn(found_left, found_straight, found_right);

    // Make the turn indicated by the path.
    turn(dir);

    // Store the intersection in the path variable.
    path[path_length] = dir;
    path_length ++;

    // Simplify the learned path.
    simplify_path();
  }
  analogWrite(leftMotorPWM, 0);  // stop both motors
  analogWrite(rightMotorPWM, 0);
  // Solved the maze!
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  for (int i = 0; i++; i < path_length) {
    Serial.println(path[i]);
  }
  // Now enter an infinite loop - we can re-run the maze as many
  // times as we want to.
  while (1)
  {

    delay(10000);
    MazeSolve();

  } // end running solved

}

void simplify_path()
{
  // only simplify the path if the second-to-last turn was a 'B'
  if (path_length < 3 || path[path_length - 2] != 'B')
    return;

  int total_angle = 0;
  int i;
  for (i = 1; i <= 3; i++)
  {
    switch (path[path_length - i])
    {
      case 'R':
        total_angle += 90;
        break;
      case 'L':
        total_angle += 270;
        break;
      case 'B':
        total_angle += 180;
        break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  total_angle = total_angle % 360;

  // Replace all of those turns with a single one.
  switch (total_angle)
  {
    case 0:
      path[path_length - 3] = 'S';
      break;
    case 90:
      path[path_length - 3] = 'R';
      break;
    case 180:
      path[path_length - 3] = 'B';
      break;
    case 270:
      path[path_length - 3] = 'L';
      break;
  }

  // The path is now two steps shorter.
  path_length -= 2;
}

void follow_line(){
  while(1){
    lineFollow();
    if(detectJunction() || detectDeadEnd()){
      break;
    }
  }
}

void lineFollow() {
  readSensors();
  error = pos - setpoint;
  correction = kp * error + kd * (error - lasterror) + ki * integral;
  //Serial.println(correction);
  //delay(500);
  lasterror = error;
  integral += error;
  leftmotorspeed = leftbasespeed + correction;
  rightmotorspeed = rightbasespeed - correction;

  if (leftmotorspeed < 50) leftmotorspeed = 50;
  if (leftmotorspeed > 250) leftmotorspeed = 250;
  if (rightmotorspeed < 50) rightmotorspeed = 50;
  if (rightmotorspeed > 250) rightmotorspeed = 250;

  move(0, rightmotorspeed, 1);
  move(1, leftmotorspeed, 1);
}



bool detectJunction() {
  if (sensorSum == 8 || (sensorSum >= 5 && (sensors[0] == 0 || sensors[7] == 0))) {
    return true;
  } else {
    return false;
  }
}

bool detectDeadEnd() {
  if (sensors[0] == 0 && sensors[1] == 0 && sensors[2] == 0 && sensors[3] == 0 && sensors[4] == 0 && sensors[5] == 0 && sensors[6] == 0 && sensors[7] == 0) {
    //beepTone();
    return true;
  } else {
    return false;
  }
}
