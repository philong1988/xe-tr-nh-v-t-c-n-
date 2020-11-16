#include <Servo.h>
#define trig 3
#define echo 4
Servo srf05;
  // tạo đối tượng servo để điều khiển một servo
  
#define inL1 6
#define inL2 7
#define inR1 8
#define inR2 9 
void setup()
{
pinMode(inL1, OUTPUT);
pinMode(inL2, OUTPUT);
pinMode(inR1, OUTPUT);
pinMode(inR2, OUTPUT);
pinMode(trig,OUTPUT);
pinMode(echo,INPUT);
srf05.attach(5); 
 // gắn servo trên chân 9 vào đối tượng servo
} 

void loop() 
{ 
objectAvoider (inL1, inL2, inR1, inR2,50, 1000);
}
 int objectDistance_cm (byte angle)
{  
srf05.write(angle);
  delay(500);
  unsigned long duration;
  //biến đo thời gian  
  
int distance;
//biến lưu khoảng cách   
/* phát xung từ chân trig */  

digitalWrite(trig,0);  
delayMicroseconds(2);  
digitalWrite(trig,1);   
delayMicroseconds(5);  
digitalWrite(trig,0);    

/* tính toán thời gian */  
duration = pulseIn(echo,HIGH )
distance = int(duration/2/29.412);   

/* in kết quả ra Serial monitor */
//Serial.print(distance);  
//Serial.println("cm");  
//delay(200);  
return distance;
}

void robotMover (byte inR1, byte inR2, byte inL1, byte inL2, byte action)
{  
/*  
inR1 inR2 là 2 chân tín hiệu động cơ bên phải  
inL1 inL2 là 2 chân tín hiệu động cơ bên trái  
action= 0 đứng yên  action =1 đi thẳng  
action =2 lùi lại  action =3 quay trái  
action =4 quay phải  action =5 rẽ trái  
action =6 rẽ phải  
action =7 rẽ lùi trái  
action =8 rẽ lùi phải   
*/ 
 switch (action)  
{
    
case 0:// không di chuyển      
motorControlNoSpeed(inR1, inR2, 0);      
motorControlNoSpeed(inL1, inL2, 0);      
break;    
case 1://đi thẳng      
motorControlNoSpeed(inR1, inR2, 1);      
motorControlNoSpeed(inL1, inL2, 1);      
break;    
case 2:// lùi lại      
motorControlNoSpeed(inR1, inR2, 2);      
motorControlNoSpeed(inL1, inL2, 2);      
break;    case 3:// quay trái      
motorControlNoSpeed(inR1, inR2, 1);      
motorControlNoSpeed(inL1, inL2, 2);      
break;    
case 4:// quay phải      
motorControlNoSpeed(inR1, inR2, 2);      
motorControlNoSpeed(inL1, inL2, 1);      
break;    
case 5:// rẽ trái      
motorControlNoSpeed(inR1, inR2, 1);      
motorControlNoSpeed(inL1, inL2, 0);      
break;    
case 6:// rẽ phải      
motorControlNoSpeed(inR1, inR2, 0);      
motorControlNoSpeed(inL1, inL2, 1);      
break;    
case 7:// rẽ lùi trái      
motorControlNoSpeed(inR1, inR2, 2);      
motorControlNoSpeed(inL1, inL2, 0);      
break;    
case 8:// rẽ lùi phải      
motorControlNoSpeed(inR1, inR2, 0);      
motorControlNoSpeed(inL1, inL2, 2);      
break;    
default:      
action = 0;        
}}  
void motorControlNoSpeed (byte in1,byte in2, byte direct)
{
// in1 and in2 are 2 signal pins to control the motor// 
en is the enable pin// 
the defauspeed is the highest// 
direct includes://    
0:Stop//    
1:Move on 1 direct//    
2:Move on another directswitch (direct)   
{    
case 0:// Dừng không quay      
digitalWrite(in1,LOW);      
digitalWrite(in2,LOW);      
break;    
case 1:// Quay chiều thứ 1      
digitalWrite(in1,HIGH);      
digitalWrite(in2,LOW);     
 break;        
case 2:// Quay chiều thứ 2     
digitalWrite(in1,LOW); 
digitalWrite(in2,HIGH);      
break;    
//default:   
}} 
oid objectAvoider (byte inR1, byte inR2, byte inL1, byte inL2, byte allow_distance, int turn_back_time)
{    
robotMover(inR1,inR2,inL1,inL2,1);  
Serial.println("Tiến");  
//delay(10);  
int front_distance=objectDistance_cm (90);  
int left_distance;  
int right_distance;  
int max_distance;  
if (front_distance > allow_distance)    
{      
robotMover(inR1,inR2,inL1,inL2,1);      
Serial.println("Tiến");      
delay(10);    
}    
if (front_distance <= allow_distance)    
{          
robotMover(inR1,inR2,inL1,inL2,2);      
Serial.println("Lùi");      
delay(300);      
robotMover(inR1,inR2,inL1,inL2,0);      
left_distance = objectDistance_cm (180); 
//đo khoảng cách bên trái      
Serial.println("left: ");          
Serial.println(left_distance);      
//delay (3000);      
right_distance = objectDistance_cm (0); 
//đo khoảng cách bên phải      
Serial.println("right: ");      
Serial.println(right_distance);      
Serial.println("front: ");      
Serial.println(front_distance);      
//delay (3000);      
max_distance = max(left_distance,right_distance);
// so sánh giá trị lớn nhất với khoảng cách bên phải (gán bằng cái lớn nhất)      
if  (max_distance==left_distance)      
{        
robotMover(inR1,inR2,inL1,inL2,3);       
Serial.println("Rẽ trái");     
   delay (turn_back_time/2);
// Nếu bên trái là khoảng cách lớn nhất thì rẽ trái     
 }      
else     
 {   
 if  (max_distance==right_distance)        
{         
 robotMover(inR1,inR2,inL1,inL2,4);
// Nếu bên phải có khoảng cách lớn nhất thì rẽ phải        
  Serial.println("Rẽ phải");        
  delay (turn_back_time/2);       
 } 
     }  
    }}
