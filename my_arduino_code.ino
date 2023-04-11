#include<SoftwareSerial.h>
SoftwareSerial SCAN(11,1);
SoftwareSerial SMS(12,13);
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

char CARD_1[]="$0000959393";
char CARD_2[]="$0000963967";
String message="";

#include <Key.h>
#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
char customKey=0;
byte rowPins[ROWS] = {3,4,5,6};
byte colPins[COLS] = {7,8,9,10};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

#define Password_Length 5
#define solar_read A3
#define mains_read A2
byte data_count = 0, master_count = 0;
long randomNumber;
String mystr;
char buff[10],buf[40];
int limt_flag1=0;
int limt_flag2=0;
int limt_flag3=0;
int charging1_flag =0;
int charging_flag=0;
char Data[Password_Length];
int card1_balance = 500;
int card2_balance = 500;
int flag1 = 0;
int card1_flag = 0;
int card2_flag = 0;
int otp_flag = 0;

int input_read1 = 0;
int input_read2 = 0;
//int input_read = 0;
int relay_pin2 = A1;
int relay_pin1 = A0;
int relay_pin3 = 2;

void setup() 
{
  Serial.begin(9600);
  SCAN.begin(9600);
  SMS.begin(9600);
  
  pinMode(mains_read,INPUT);
  pinMode(solar_read,INPUT);
  
  pinMode(relay_pin1,OUTPUT);
  pinMode(relay_pin2,OUTPUT);
  pinMode(relay_pin3,OUTPUT);
   
  Wire.begin();
  lcd.init();                     
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("EV CHARGING");
  lcd.setCursor(4,1);
  lcd.print("STATION");
  intGsm("6300134294","EV - CHARGING STATION");
  delay(2000);
}

void loop() 
{
  while(1)
{
  int i = 1;
  int recharge_amount = 0;
  char buff2[15];
  input_read1 = analogRead(solar_read);
  input_read2 = analogRead(mains_read);
//  Serial.println(input_read);
  SCAN.begin(9600);
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print("SCAN YOUR CARD");
  delay(1000);
  SCAN.begin(9600);
  
//  Serial.println("ENTERED");
  while(SCAN.available()>0)
    {
      message=SCAN.readString();
      Serial.println("ENTERED");
      int str_len = message.length() + 1;
      char textmessage[11];
      message.toCharArray(textmessage,str_len);
      Serial.println(textmessage);
      textmessage[11]='\0';
      if(!strcmp(textmessage,CARD_1))
      {
        while(i)
        {
           sprintf(buff2,"amt in card:%d",card1_balance); 
           lcd.clear();    
           lcd.setCursor(0,0);
           lcd.print("NAME: Shaik Moin");
           lcd.setCursor(0,1);
           lcd.print(buff2);
           
           delay(3000);
           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("PRESS 1 FOR CONFM");
           lcd.setCursor(0,1);
           lcd.print("PRESS D FOR rchrge"); 
           limt_flag1 =1;          
           customKey=0;
           while(customKey==0)
           {
              customKey = customKeypad.getKey();
           }
           if(customKey == '1')
           {
              customKey=0;
              IntToASCI();
              intGsm("6300134294",buff);
              delay(2000);
              OTP_checking();
              Serial.println("out from charging");
              i = 0; 
           }
          if(customKey == 'D')
           {
              customKey=0;
              recharge_amount = recharge();
              card1_balance += recharge_amount;                           
           }
        }
      }
      if(!strcmp(textmessage,CARD_2))
      {
        while(i)
        {
           sprintf(buff2,"amt in card:%d",card2_balance); 
           lcd.clear();    
           lcd.setCursor(0,0);
           lcd.print("NAME: Ahmed");
           lcd.setCursor(0,1);
           lcd.print(buff2);
           delay(3000);
           customKey=0;
           lcd.clear();
           lcd.setCursor(0,0);
           lcd.print("PRESS 1 FOR CONFM");;
           lcd.setCursor(0,1);
           lcd.print("PRESS D FOR rchrge"); 
           limt_flag2 =1;          
           while(customKey==0)
           {
             customKey = customKeypad.getKey();
           }
        if(customKey == '1')
        {
           customKey=0;
           IntToASCI();
           intGsm("9550964921",buff);
           delay(2000);      
           OTP_checking();
           Serial.println("out from charging");
           customKey=0;
           i = 0;
        }
          if(customKey == 'D')
           {
              customKey=0;
              recharge_amount = recharge();
              card2_balance += recharge_amount;             
           }        
       }
       Serial.println("out");
      }
    }
  }
}
void PASSWORD()
{
  customKey=0;
  while(customKey==0)
     {
      customKey = customKeypad.getKey();
     }
      Serial.println(customKey);
}
void clearData()
{
  while(data_count !=0)
  {
    Data[data_count--] = 0; 
  }
  return;
}

void IntToASCI()
{ 
  clearData();
  int i=0;
  randomNumber = random(1000,9999);
  mystr= String(randomNumber);
  String one = mystr;  
  String message = one;
  String str = "otp is ";
  String all = str + message;

  // Convert String to char array
  int str_len = message.length() + 1;
  char textmessage[str_len];
  message.toCharArray(textmessage,str_len);
  strcpy(buff,textmessage);
  Serial.println(buff);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("sending otp");
  lcd.setCursor(0,1);
  lcd.print("........ ");

}
void OTP_checking()
{
  clearData();
  int k=1;
  int p=0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ENTER OTP");
  lcd.setCursor(0,1);
  lcd.print(".......");
  while(k<=4)
  {  
    customKey = customKeypad.getKey();
    if (customKey)
    {
      Data[data_count] = customKey;
      lcd.setCursor(p,2);
      lcd.print(Data[data_count]);
      p++;
      data_count++; 
    }

    if(data_count == Password_Length-1)
    {
      if(!strcmp(Data, buff))
      {
        lcd.clear();
        lcd.print("Correct");
        delay(2000);
        charging();
        clearData();
        break;
      }
      else
      {
        if(k==3)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("ATTEMPTED MORE TIMES");
//          intGsm("7338546982","ATTEMPTED MORE TIMES");
          customKey=0;
          clearData();
          loop();
          delay(2000);
          exit; 
        }
        else
        {
          clearData();
          k++;
          p=0;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("INCORRECT PASSWORD");
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("RE-ENTER PASSWORD");
          delay(1000);
        }
      }
   }
 }
}
void buff_clear()
{
  for(int i=0;i<4;i++)
  buff[i]=0;
}

void intGsm( char *num1, char * str1 )
{
  char buff[10],i=0;
  SMS.write('A');
  delay(100);
  SMS.write('T');
  delay(100);
  SMS.write('E');
  delay(100);
  SMS.write('0');
  delay(100);
  SMS.write('\r');
  SMS.write("AT+CMGF=1\r"); //Initialize GSM For mobile
  delay(500);
  SMS.write("AT+CMGS=\"");
  delay(500);
  SMS.write(num1);
  delay(500);
  SMS.write("\"\r");
  SMS.write(str1);
  delay(500);
  SMS.write(26);
  delay(500);
  Serial.println("meassage sent");
}

void charging()
{
  Serial.print("Entered");
  input_read1 = analogRead(solar_read);
  input_read2 = analogRead(mains_read);
  Serial.println(input_read1);
  
    if(input_read1 > 500)
    {
      flag1 = 1;
      charging_flag =1;
      billing();
    }
    else
    {
      flag1 = 1;
      charging_flag = 2;
      billing();     
    }
}
void charging_1()
{
    input_read1 = analogRead(solar_read);
  input_read2 = analogRead(mains_read);
    if(input_read1 > 500)
    {
      flag1 = 1;
      charging1_flag = 1;
      limitted(); 
    }
    else
    {
      flag1 = 1;
      charging1_flag = 2;
      limitted();      
    }
}
void limitted()
{
  if(flag1 == 1)
  {
   char ch;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("2.limitted");
      lcd.setCursor(0,1);
   lcd.print("selected");
   delay(2000);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("1.30s");
   lcd.setCursor(8,0);
   lcd.print("2.1 Min");
   lcd.setCursor(0,1);
   lcd.print("3.5 Min");
   lcd.setCursor(8,1);
   lcd.print("4.30 Min");
   delay(2000);
   while(1)
   {
   ch = customKeypad.getKey();
   switch(ch)
   {
    case '1':limit_billing(30);
             break;
    case '2':limit_billing(60);
             break;
    case '3':limit_billing(300);
             break;
    case '4':limit_billing(1800);
             break;                                       
   }
   if(ch=='1'||ch=='2'||ch=='3'||ch=='4')
   {
    delay(2000);
    break;
   }
   }
}
}
void billing()
{
  Serial.println("entered billing");
  char ch;
  int amount = 0;
   if(flag1 == 1)
   {
      int count = 0;
      int i = 1;
      char buff1[15];
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("1.unlimitted");
      lcd.setCursor(0,1);
      lcd.print("2.limitted");
      delay(2000);
      while(1)
      {
      ch = customKeypad.getKey();
      if(ch == '1')
      {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("1).unlimitted");
         lcd.setCursor(0,1);
   lcd.print("selected");
      delay(2000);
      lcd.clear();
      if(charging_flag == 1)
      {
      digitalWrite(relay_pin2,HIGH);
      digitalWrite(relay_pin3,HIGH);
      charging_flag = 0;
      }
      if(charging_flag == 2)
      {
      digitalWrite(relay_pin1,HIGH);
      digitalWrite(relay_pin3,HIGH);
      charging_flag = 0; 
      }
      lcd.print("billing.....");
      lcd.setCursor(0,1);
      lcd.print("TIME:");
      while(i)
      {
         count++;
         lcd.setCursor(6,1);
         lcd.print(count);   
         lcd.print("s");     
         delay(1000);
         if(SCAN.available()>0)
          {
             message=SCAN.readString();
             int str_len = message.length() + 1;
             char textmessage[11];
             message.toCharArray(textmessage,str_len);
             Serial.println(textmessage);
             textmessage[11]='\0';
             if(!strcmp(textmessage,CARD_1))
              {
                 amount = count *1;
                 sprintf(buff1,"total amt =%d",amount);
                 lcd.clear();
                 lcd.setCursor(0,0);
                 lcd.print(buff1);
                 delay(2000);
                 lcd.clear();
                 card1_balance -= amount;
                 i = 0;
                 flag1 = 0;
                 digitalWrite(relay_pin1,LOW);
                 digitalWrite(relay_pin3,LOW);
                 digitalWrite(relay_pin2,LOW);                 
                }
              if(!strcmp(textmessage,CARD_2) )
               {
                 amount = count *1;
                 sprintf(buff1,"total amt =%d",amount);
                 lcd.clear();
                 lcd.setCursor(0,0);
                 lcd.print(buff1);
                 delay(2000);
                 lcd.clear();
                 card2_balance -=amount;
                 i = 0;
                 flag1 = 0;
                 digitalWrite(relay_pin1,LOW);
                 digitalWrite(relay_pin3,LOW);
                 digitalWrite(relay_pin2,LOW);
            }
//              if(!strcmp(textmessage,CARD_3) )
//               {
//                 amount = count *1;
//                 sprintf(buff1,"total amt =%d",amount);
//                 lcd.clear();
//                 lcd.setCursor(0,0);
//                 lcd.print(buff1);
//                 delay(2000);
//                 lcd.clear();
//                 card3_balance -=amount;
//                 i = 0;
//                 flag1 = 0;
//                 digitalWrite(relay_pin1,HIGH);
//                 digitalWrite(relay_pin3,LOW);
//                 digitalWrite(relay_pin2,HIGH);
//            }            
                         
   }
}
break;
   }
else if(ch == '2')
{
 charging_1(); 
 break;   
}
      }
}
}

//void billing()
//{
//  int amount = 0;
//   if(flag1 == 1)
//   {
//      int count = 0;
//      int i = 1;
//      char buff1[15];
//      lcd.clear();
//      lcd.setCursor(0,0);
//      lcd.print("billing.....");
//      lcd.setCursor(0,1);
//      lcd.print("TIME:");
//      while(i)
//      {
//         count++;
//         lcd.setCursor(6,1);
//         lcd.print(count);        
//         delay(1000);
//         if(Serial.available()>0)
//          {
//             message=Serial.readString();
//             int str_len = message.length() + 1;
//             char textmessage[11];
//             message.toCharArray(textmessage,str_len);
//             Serial.println(textmessage);
//             textmessage[11]='\0';
//             if(!strcmp(textmessage,CARD_1))
//              {
//                 amount = count *1;
//                 Serial.print(amount);
//                 sprintf(buff1,"total amt =%d",amount);
//                 lcd.clear();
//                 lcd.setCursor(0,0);
//                 lcd.print(buff1);
//                 delay(2000);
//                 lcd.clear();
//                 card1_balance -= amount;
//                 lcd.clear();
//                 lcd.setCursor(0,1);
//                 lcd.print("Bal:");
//                 lcd.setCursor(5,1);
//                 lcd.print(card1_balance);                 
//                 delay(2000);
//                 lcd.clear();                 
//                 i = 0;
//                 flag1 = 0;
//                 digitalWrite(relay_pin1,LOW);
//                 digitalWrite(relay_pin3,LOW);
//                 digitalWrite(relay_pin2,LOW); 
//                 intGsm("7338546982", buff1);
//                 delay(2000);                                 
//                }
//              if(!strcmp(textmessage,CARD_2) )
//               {
//                 amount = count *1;
//                 sprintf(buff1,"total amt =%d",amount);
//                 lcd.clear();
//                 lcd.setCursor(0,0);
//                 lcd.print(buff1);
//                 delay(2000);
//                 lcd.clear();
//
//                 card2_balance -=amount;
//                 lcd.clear();
//                 lcd.setCursor(0,1);
//                 lcd.print("Bal:");
//                 lcd.setCursor(5,1);
//                 lcd.print(card2_balance -= amount);                 
//                 delay(2000);
//                 lcd.clear();                 
//                 i = 0;
//                 flag1 = 0;
//                 digitalWrite(relay_pin1,LOW);
//                 digitalWrite(relay_pin3,LOW);
//                 digitalWrite(relay_pin2,LOW);
//                 intGsm("7338546982", buff1);
//                 delay(2000);                 
//            }
////              else 
////               {
////                 lcd.clear();
////                 lcd.setCursor(0,0);
////                 lcd.print("invalid card");
////                 count++;                
////               }
//              
//   }
//}
//   }
//   Serial.println("out from billling");
//}

int recharge()
{
   char Data1[10];
   int data_count1 = 0;
   clearData();
   int k=1,num;
   int p=0;
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("enter recharge amount:");
  while(k)
  {
    customKey = customKeypad.getKey();
    if (customKey && customKey != 'D')
    {
      Data1[data_count1] = customKey;
      lcd.setCursor(p,1);
      lcd.print(Data1[data_count1]);
      p++;
      data_count1++; 
    }
    if (customKey == 'D') 
    {
      Serial.println(Data1);
      num = atoi(Data1);
      k = 0;      
    }
  }
  return num;
}
void limit_billing(int t)
{
      int amount = 0;
      int count = 0;
      char buff1[15];
      if(charging1_flag == 1)
      {
      digitalWrite(relay_pin2,HIGH);
      digitalWrite(relay_pin3,HIGH);
      charging1_flag = 0 ;
      }
      if(charging1_flag == 2)
      {
      digitalWrite(relay_pin1,HIGH);
      digitalWrite(relay_pin3,HIGH);
      charging1_flag = 0; 
      }
      lcd.clear();  
      lcd.print("billing.....");
      lcd.setCursor(0,1);
      lcd.print("TIME:");
      while(count < t)
      {
         count++;
         lcd.setCursor(6,1);
         lcd.print(count); 
         lcd.print("s");       
         delay(1000);
      }
      amount = count *1;
      sprintf(buff1,"total amt =%d",amount);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(buff1);
      delay(2000);
      lcd.clear();
      if(limt_flag1 == 1)
      {
      card1_balance -= amount;
      Serial.print(card1_balance);
      limt_flag1 =0;
      }
      else if(limt_flag2 == 1)
      {
      card2_balance -= amount;
      limt_flag2 =0;
      }
//      else if(limt_flag3 == 1)
//      {
//      card3_balance -= amount;
//      limt_flag3 =0;
//      }            
      flag1 = 0;
      digitalWrite(relay_pin1,LOW);
      digitalWrite(relay_pin3,LOW);
      digitalWrite(relay_pin2,LOW);                 
      }
