#include <EEPROM.h>
#include <Servo.h>


Servo leftservo;
Servo rightservo;
int ms;
int ls;
int rs;
int lms;
int rms;
char path[600];
int n;
int i=0;
int type;
int gg_path;
boolean endProgramm=false;
boolean endFound=false;
int path_length=0;
void setup() {
  Serial.begin(9600);
  maprun();
  tone(12,660,2000);
  leftservo.detach();
  rightservo.detach();
  Serial.println();
     for(int j=0;j<path_length+1;j++){
              Serial.print(path[j]);
             Serial.print(path[j]+" ");
                             }
     Serial.println();
  delay(5000);
  finalrun();
  beep();
  delay(300);
  finalbeep();
  leftservo.detach();
  rightservo.detach();
  
  
}

void loop() {
  
  
}

void left(){

  if(((lms>500)&&(ls>500)&&(ms>500)&&(rms<500))||((rms>500)&&(rs>500)&&(ms>500)&&(lms<500))||((rms>500)&&(rs>500)&&(ms>500)&&(ls>500)&&(lms>500))){
    type = nodeType();
    switch(type){
      case 0:break;
      case 1:
      //End or start
        leftservo.detach();
        rightservo.detach();
        endFound = true;
//        finalbeep();
        break;
      case 2:
      //T junction
        beep();
        path[path_length]='L';
        path_length++;
        simplify_path();
        left90();
        break;
      case 3:
      //Dead left
        beep();
        path[path_length]='L';
        path_length++;
        simplify_path();
        left90();
        break;
      case 4:
      //Dead right
        beep();
        path[path_length]='R';
        path_length++;
        simplify_path();
        right90();
        break;
      case 5:
      //Dead end
        break;
      case 6:
      //Cross
        beep();
        path[path_length]='L';
        path_length++;
        simplify_path();
        left90();
        break;
      case 7:
      //Straight with left
        beep();
        path[path_length]='L';
        path_length++;
        simplify_path();
        left90();
        break;
      case 8:
      //Straight with right
        beep();
        path[path_length]='S';
        path_length++;
        simplify_path();
        break;  
    }    
    }
  
if (ls<500 && lms<500 && ms<500 && rms<500 && rs<500 ){
  path[path_length]='B';
  path_length++;
  simplify_path();
  finalbeep();
  turn180();
  }
if(ls>500){
//  if (ls>500 && lms>500 && ms>500 && rms<500  && rs<500 ){
//    path[i]='L';
//    i++;
//    beep();
//    left90();
//    t++;
//  }
//  else{
//    leftservo.attach(8);
//    rightservo.attach(7);
//    leftservo.write(70);
//    rightservo.write(70);
//  }
//}
//else if(lms>500){
//  if (ls>500 && lms>500 && ms>500 && rms<500  && rs<500 ){
//  path[i]='L';
//  i++;
//  beep();
//  left90();
//  t++;
//  }
//  else{
    leftservo.attach(8);
    rightservo.attach(7);
    leftservo.write(70);
    rightservo.write(0);
  }
//}
else if(ms>500){
      leftservo.attach(8);
      rightservo.attach(7);
      leftservo.write(180);//180
      rightservo.write(0);//0

}
else if(rs>500){
//  if (rs>500 && rms>500 && ms>500 && ls<500 && lms<500 ){
//    path[i]='R';
//    i++;
//    beep();
//    right90();
//    t++;
//  }
//  else{
//    leftservo.attach(8);
//    rightservo.attach(7);
//    leftservo.write(75);
//    rightservo.write(73);
//  }
//}
//else if(rms>500){
//    if (rs>500 && rms>500 && ms>500 && ls<500 && lms<500 ){
//      path[i]='R';
//      i++;
//      beep();
//      right90();
//      t++;
//    }
//  else{
    leftservo.attach(8);
    rightservo.attach(7);
    leftservo.write(180);
    rightservo.write(73);
     }
//}
else{
  leftservo.write(100);
  rightservo.write(100);
}
//

  }
  
void left90(){
    leftservo.attach(8);
    rightservo.attach(7);
    leftservo.write(74);
    rightservo.write(70);
    delay(150);
    leftservo.write(73);
    rightservo.write(70);
    delay(150);
    leftservo.write(65);
    rightservo.write(70);
    delay(350);
    leftservo.detach();
    rightservo.detach();
    while(true){
          ms = analogRead(A2);
          rms = analogRead(A0);
          rs = analogRead(A1);
          ls = analogRead(A6);
          lms = analogRead(A7);
          leftservo.attach(8);
          rightservo.attach(7);
          leftservo.write(65);
          rightservo.write(70);
          if(rs<500 && rms<500 && ms>500 && ls<500 && lms<500){break;}
                    }
  }
void right90(){
    leftservo.attach(8);
    rightservo.attach(7);
    leftservo.write(74);
    rightservo.write(70);
    delay(150);
    leftservo.write(73);
    rightservo.write(70);
    delay(150);
   leftservo.write(75);
   rightservo.write(78);
   delay(350);
   leftservo.detach();
   rightservo.detach();

  while(true){
        ms = analogRead(A2);
        rms = analogRead(A0);
        rs = analogRead(A1);
        ls = analogRead(A6);
        lms = analogRead(A7);
        leftservo.attach(8);
        rightservo.attach(7);
        leftservo.write(75);
        rightservo.write(78);
        if(rs<500 && rms<500 && ms>500 && ls<500 && lms<500){break;}
              }
  }

void turn180(){
  while(true){
      ms = analogRead(A2);
      rms = analogRead(A0);
      rs = analogRead(A1);
      ls = analogRead(A6);
      lms = analogRead(A7);
      leftservo.attach(8);
      rightservo.attach(7);
      leftservo.write(75);
      rightservo.write(78);
      if(rs<500 && rms<500 && ms>500 && ls<500 && lms<500){break;}
                }
  }

void beep(){

      tone(12,660,100);
            }
void finalbeep(){

      tone(12,660,300);
      }
void maprun(){
  while(!endFound){
      ms = analogRead(A2);
      rms = analogRead(A0);
      rs = analogRead(A1);
      ls = analogRead(A6);
      lms = analogRead(A7);
      left();
//      Serial.println();
//      for(int j=0;j<i+1;j++){
//              Serial.print(path[j]);
//              Serial.print(path[j]+" ");
//                              }
//      Serial.println();
//  
  }
  }


int nodeType(){
  /*    ms = analogRead(A2);
        rms = analogRead(A0);
        rs = analogRead(A1);
        ls = analogRead(A6);
        lms = analogRead(A7); */
  cMove();
  if(analogRead(A2)<500 && analogRead(A0)<500 && analogRead(A1)<500 && analogRead(A6)<500 && analogRead(A7)<500){
    //if all current values are white
    if(ms>500 && rms>500 && rs>500 && ls>500 && lms>500){
      //if all previous sensor values are all black
      return 2;
    }else if(ms>500 && ls>500 && lms>500 && rms<500){
      //if all previous left sensor values are black
      return 3;
    }else if(ms>500 && rms>500 && rs>500 && lms<500){
      //if all previous right sensor values are black
      return 4;
    }else if(ms<500 && rms<500 && lms<500){
      //if previous middle sensor value is white
      return 5;
    }else{
      //undetermined
      return 0;
    }
  }else if(analogRead(A2)>500 && analogRead(A0)<500 && analogRead(A7)<500){
    //if current middle value is black
    if(ms>500 && rms>500 && rs>500 && ls>500 && lms>500){
      //if all previous sensor values are black
      return 6;
    }else if(ms>500 && ls>500 && lms>500 && rms<500){
      //if all previous left sensor values are black
      return 7;
    }else if(ms>500 && rms>500 && rs>500 && lms<500){
      //if all previous right sensor values are black
      return 8;
    }else{
      //undetermined
      return 0;
    }
  }else if(analogRead(A2)>500 && analogRead(A0)>500 && analogRead(A1)>500 && analogRead(A6)>500 && analogRead(A7)>500){
    //if all current values are black
    return 1;
  }else{
    //undetermined
    return 0;
  }
}
void cMove(){
  beep();
  beep();
    leftservo.attach(8);
    rightservo.attach(7);
    leftservo.write(74);
    rightservo.write(70);
    delay(175);
    leftservo.write(73);
    rightservo.write(70);
    delay(175);
  
//  leftservo.detach();
//  rightservo.detach();
}

void simplify_path()
{
  if(path_length < 3 || path[path_length-2] != 'B')
    return;

  int total_angle = 0;
  int i;
  for(i=1;i<=3;i++)
  {
    switch(path[path_length-i])
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
  total_angle = total_angle % 360;

  switch(total_angle)
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
  path_length -= 2;
  
}

void ggRun(){

  if(((lms>500)&&(ls>500)&&(ms>500)&&(rms<500))||((rms>500)&&(rs>500)&&(ms>500)&&(lms<500))||((rms>500)&&(rs>500)&&(ms>500)&&(ls>500)&&(lms>500))){
        
        beep();
        Serial.println(path[i]);
        if(path[i]=='L'){
          beep();
          cMove();
          left90();
          i++;
          ggMove();
          
          
        }else if(path[i]=='R'){
          cMove();
          right90();
          i++;
          ggMove();
        }else if(path[i]=='S'){
          cMove();
          //cMove();
          i++;
          ggMove();
          
        }
        else if(path[i]=='B'){
          endProgramm=true;
          
          
        }       
        }
  ggMove();
  
}

void ggMove(){
      ms = analogRead(A2);
      rms = analogRead(A0);
      rs = analogRead(A1);
      ls = analogRead(A6);
      lms = analogRead(A7);
    if(ms>500&&rs>500){
      leftservo.attach(8);
      rightservo.attach(7);
      leftservo.write(180);
      rightservo.write(75);
    }else if(ms>500&&ls>500){
      leftservo.attach(8);
      rightservo.attach(7);
      leftservo.write(70);
      rightservo.write(0);
    }else if(ms>500){
      leftservo.attach(8);
      rightservo.attach(7);
      leftservo.write(180);  //75
      rightservo.write(0); //68
    }else{
      leftservo.write(180);
      rightservo.write(180);
    }
}

void straight(){
      leftservo.attach(8);
      rightservo.attach(7);
      leftservo.write(180);
      rightservo.write(0);
      delay(400);

  }
void finalrun(){
      ms = analogRead(A2);
      rms = analogRead(A0);
      rs = analogRead(A1);
      ls = analogRead(A6);
      lms = analogRead(A7);
  while(!endProgramm){
    ggRun();
    }
  }
