#include<SoftwareSerial.h>
#include<TinyGPS.h>
TinyGPS gps;
//SoftwareSerial Serial2(2,3); //make RX arduino line is pin 2, make TX arduino line is pin 3.
SoftwareSerial GPS(10,11);
#include<LiquidCrystal.h>
LiquidCrystal lcd(4,5,6,7,8,9);

#define x A1
#define y A2
#define z A3

#define speed_INT_pin 18
#define break_INT_pin 19
#define steering_left_INT_pin 20
#define steering_right_INT_pin 21

#define rash_driving_alert 22

#define disable_alarm 23
#define alert_alarm 24

int xsample=0;
int ysample=0;
int zsample=0;

#define samples 10

#define minVal -100
#define MaxVal 100

int i=0,k=0;
int  gps_status=0;
float latitude=20.0131836;
float logitude=73.8226314;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";


volatile bool _speedINT = false;
volatile bool _breakINT = false;
volatile bool _steeringLeftINT = false;
volatile bool _steeringRightINT = false;

unsigned long speed_time = 0;
unsigned long break_time = 0;
unsigned long steering_left_time = 0;
unsigned long steering_right_time = 0;

int speed_violation_count = 0;
int break_violation_count = 0;
int steeringLeft_violation_count = 0;
int steeringRight_violation_count = 0;

void initModule(String cmd, char *res, int t)
{
  while(1)
  {
    Serial.println(cmd);
    Serial2.println(cmd);
    delay(100);
    while(Serial2.available()>0)
    {
       if(Serial2.find(res))
       {
        Serial.println(res);
        delay(t);
        return;
       }
       else
       {
        Serial.println("Error");
       }
    }
    delay(t);
  }
}
void setup() 
{
  
  pinMode(rash_driving_alert,OUTPUT);
//  digitalWrite(rash_driving_alert,LOW);

  pinMode(disable_alarm,INPUT);
  pinMode(alert_alarm,OUTPUT);
  pinMode(24,OUTPUT);
  digitalWrite(24,LOW);

  pinMode(25,OUTPUT);
  digitalWrite(25,LOW);

  pinMode(18,INPUT);
  pinMode(19,INPUT);
  pinMode(20,INPUT);
  pinMode(21,INPUT);
  pinMode(22,OUTPUT);
  //pinMode(23,OUTPUT);
  pinMode(24,OUTPUT);
  digitalWrite(22,LOW);
  digitalWrite(24,LOW);
  
  Serial2.begin(9600);
  Serial.begin(9600);
  lcd.begin(16,2);  
  lcd.print("Accident Alert  ");
  lcd.setCursor(0,1);
  lcd.print("     System     ");
  delay(8000);
  lcd.clear();
  lcd.print("Initializing");
  lcd.setCursor(0,1);
  lcd.print("Please Wait...");
  delay(1000);
  
  Serial.println("Initializing....");
  initModule("AT","OK",1000);
  initModule("ATE1","OK",1000);
  initModule("AT+CPIN?","READY",1000);  
  initModule("AT+CMGF=1","OK",1000);     
  initModule("AT+CNMI=2,2,0,0,0","OK",1000);  
  Serial.println("Initialized Successfully");
  lcd.clear();
  lcd.print("Initialized");
  lcd.setCursor(0,1);
  lcd.print("Successfully");
  delay(2000);
  lcd.clear();
  lcd.print("Callibrating ");
  lcd.setCursor(0,1);
  lcd.print("Acceleromiter");
  for(int i=0;i<samples;i++)
  {
    xsample+=analogRead(x);
    ysample+=analogRead(y);
    zsample+=analogRead(z);
  }
  xsample/=samples;
  ysample/=samples;
  zsample/=samples;

  Serial.println(xsample);
  Serial.println(ysample);
  Serial.println(zsample);
  delay(1000);
  
  lcd.clear();
  lcd.print("Waiting For GPS");
  lcd.setCursor(0,1);
  lcd.print("     Signal    ");
  //Serial.println("Wait for 10 seconds");
  delay(3000);
  GPS.begin(9600);
  gpsReceive();
  show_coordinate();
  delay(3000);
  lcd.clear();
  lcd.print("GPS is Ready");
  delay(1000);
  lcd.clear();
  lcd.print("System Ready");
  Serial.println("System Ready..");
}
void loop() 
{
    lcd.clear();
    lcd.print("System Ready");
    //Serial.println("System Ready..");
    int value1=analogRead(x);
    int value2=analogRead(y);
    int value3=analogRead(z);

    int xValue=xsample-value1;
    int yValue=ysample-value2;
    int zValue=zsample-value3;
    
    //Serial.print("x=");
    //Serial.println(xValue);
    //Serial.print("y=");
    //Serial.println(yValue);
    //Serial.print("z=");
    //Serial.println(zValue);
    if(digitalRead(18))
    {
      speed_violation_count++;
        Serial.println("Speed_violation_count++");
      while(digitalRead(18))
      {  
      }
    }
    if(digitalRead(19))
    {
      break_violation_count++;
        Serial.println("break_violation_count++;");
      while(digitalRead(19))
      {
        
      }
    }
    if(digitalRead(20))
      {
        steeringLeft_violation_count++;
          Serial.println("steeringLeft_violation_count++");
        while(digitalRead(20))
        {
          
        }
      }
    if(digitalRead(21))
      {
        steeringRight_violation_count++;
          Serial.println("steeringRight_violation_count++");
        while(digitalRead(21))
        {
          
        }
      }
      
    if(speed_violation_count > 10 || break_violation_count > 10 || steeringLeft_violation_count > 10 || steeringRight_violation_count > 10)
    {
      Serial.println("Rash Driving!");
      digitalWrite(rash_driving_alert,HIGH);
      speed_violation_count = 0;
      break_violation_count = 0;
      steeringLeft_violation_count = 0;
      steeringRight_violation_count = 0;
      delay(1000);
      Serial.println("Rash Driving!");
      lcd.clear();
      lcd.print("Rash Driving!");
      digitalWrite(24,HIGH);
      delay(2000);
      digitalWrite(24,LOW);
      digitalWrite(rash_driving_alert,LOW);
    }


    if(xValue < minVal || xValue > MaxVal  || yValue < minVal || yValue > MaxVal  || zValue < minVal || zValue > MaxVal)
    {
      gpsReceive();
      show_coordinate();
      lcd.clear();
      lcd.print("Sending SMS ");
      Serial.println("Sending SMS");
      Send();
      Serial.println("SMS Sent");
      digitalWrite(22,HIGH);
      delay(2000);
      digitalWrite(22,LOW);
      lcd.clear();
      lcd.print("System Ready");
    }
}
void gpsReceive()
{
  while (GPS.available())    // While there is data on the RX pin...
  {
    int c = GPS.read();    // load the data into a variable...
    if (gps.encode(c))     // if there is a new valid sentence...
    {
      //gps.f_get_position(&latitude, &logitude);
      Serial.print(latitude);
      Serial.print('\t');
      Serial.println(logitude);
    }
  }
}
void show_coordinate()
{
    lcd.clear();
    lcd.print("Lat:");
    lcd.print(latitude);
    lcd.setCursor(0,1);
    lcd.print("Log:");
    lcd.print(logitude);
    Serial.print("Latitude:");
    Serial.println(latitude);
    Serial.print("Longitude:");
    Serial.println(logitude);
    Serial.print("Speed(in knots)=");
    Serial.println(Speed);
    delay(2000);
    lcd.clear();
    lcd.print("Speed(Knots):");
    lcd.setCursor(0,1);
    lcd.print(Speed);
}
void Send()
{
   
   digitalWrite(alert_alarm,HIGH);
   for(int i=0;i<10000;i++)
   {
      delay(1);
      if(digitalRead(disable_alarm == true))
      {
        digitalWrite(alert_alarm,LOW);
        break;
      }
   }
   Serial2.println("AT");
   delay(500);
   serialPrint();
   Serial2.println("AT+CMGF=1");
   delay(500);
   serialPrint();
   Serial2.print("AT+CMGS=");
   Serial2.print('"');
   Serial2.print("9423412608");    //mobile no. for SMS alert
   Serial2.println('"');
   delay(500);
   serialPrint();
   Serial2.print("Latitude:");
   Serial2.println(latitude);
   delay(500);
   serialPrint();
   Serial2.print(" longitude:");
   Serial2.println(logitude);
   delay(500);
   serialPrint();
   Serial2.print(" Speed:");
   Serial2.print(Speed);
   Serial2.println("Knots");
   delay(500);
   serialPrint();
   Serial2.print("http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=");
   Serial2.print(latitude,6);
   Serial2.print("+");              //28.612953, 77.231545   //28.612953,77.2293563
   Serial2.print(logitude,6);
   Serial2.write(26);
   delay(2000);
   serialPrint();
}
void serialPrint()
{
  while(Serial2.available()>0)
  {
    Serial.print(Serial2.read());
  }
}
