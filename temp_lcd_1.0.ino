#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <TimerOne.h>

char array1[]="Temp:           "; //the string to print on the LCD row0  //16 slots, just update the chars
char array2[]="Stat:           "; //the string to print on the LCD row1  //16 slots, just update the chars
//remove me when sensor 2; once line2, sensor2 output is working
int str2count = 0;
//end remove me when sensor 2
int count = 0;//Set count=0. Here count is a count value that increases by 1 every 0.1 second, which means 1 second is counted when the value is 10
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
//TMP36 Pin Variables
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade with a
                        //500 mV offset to allow for negative temperatures
float temperatureF = 0;
bool screenUpdateNeeded = false;;

void setup()
{
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 

  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
                       
  Timer1.initialize(1000);/// set a timer
  Timer1.attachInterrupt(readsensors); // attach the service routine here
}

void loop() 
{
  if (screenUpdateNeeded)
  {
    lcd.setCursor(0,0);
    lcd.print(&array1[0]);//array1[6]);
    lcd.setCursor(0,1);
    lcd.print(&array2[0]);//array1[6]);
    screenUpdateNeeded = false;
  }
}

void readsensors()
{
  //Currently we are called every 1000th of a second (each ms)
  count++;

  //Really don't print to serial too often or update the sensors, we are not fast
  if(count == 1000) //once a second
  {
     count = 0;
     
     //Get the voltage reading from the temperature sensor
     int reading = analogRead(sensorPin);  
     //Convert that reading to voltage, for 3.3v arduino use 3.3
     float voltage = reading * 5.0;
     voltage /= 1024.0; 
     //Print out the voltage
     Serial.print(voltage); Serial.println(" volts");
     float temperatureC = (voltage - 0.5) * 100 ;//converting from 10 mv per degree wit 500 mV offset
     //Convert to Fahrenheit
     temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
     Serial.print(temperatureF); Serial.println(" degrees F");
     //Update the display strings
     updateScreenChars();
     Serial.print(array2); Serial.println(":Display Output");
     Serial.print(array2); Serial.println(":Display Output");
     screenUpdateNeeded = true;
  }
}

void updateScreenChars()
{
  //only 16 slots... nothing over array1[16] or array2[16]
  //todo:implement overflow prtotect
  //array1[]="Current temp:               "; //the string to print on the LCD
  //array2[]="Current temp:               "; //the string to print on the LCD
  char str1[5] = "00.0"; 
  char str2[5] = "00.0";

  //Convert to 4 char string for displays
  convertDecToSTR4(temperatureF, str1);
  
// start This is temp for setting str2  //sensor data for line 2
 float tmpval1 = str2count;
  //Convert to 4 char string for displays
  convertDecToSTR4(tmpval1, str2);
  str2[3] = str1[3];   //'7';//(char)str2count / 9;
  if((str2count % 2) == 0){str2[3] = 'L';}
  if(str2count == 100)
  {
    str2count = 0;
    convertDecToSTR4(tmpval1, str2);
  } //don't go over bounds ...using only 3 digit display

    str2count++;
//end    This is temp for setting str2  //sensor data for line 2
  
  //set the chars
   array1[6]=str1[0];
   array1[7]=str1[1]; 
   array1[8]=str1[2];
   array1[9]=str1[3];
      //slap a " %F"
      array1[10]=' ';
      array1[11]='*';
      array1[12]='F';
      
   array2[6]=str2[0];
   array2[7]=str2[1];
   array2[8]=str2[2];
   array2[9]=str2[3];
   //slap a " %?" 
         //slap a " %F"
      array2[10]=' ';
      array2[11]='*';
      array2[12]='?';
      
}

void convertDecToSTR4(float val , char str[])
{
  //We want these values:  "XX.X"
  //For example 45.6
  
  int newval = val*10; //lets assuem xx.x and convert to xxx, where xx is to the left and the last x is to the right of decimal

  str[0] = getNum(newval/100);
  str[1] = getNum(newval%100/10);
  str[2] = '.';
  str[3] = getNum(newval%10);
}  

char getNum(int num)
{
  char retVal = ' ';
  switch(num)
  {
    default: 
    retVal = '0'; 
    break;
    case 1: 
    retVal = '1';
    break;
    case 2: 
    retVal = '2';
    break;
    case 3: 
    retVal = '3';
    break;
    case 4: 
    retVal = '4';
    break;
    case 5: 
    retVal = '5';
    break;
    case 6: 
    retVal = '6';
    break;
    case 7: 
    retVal = '7';
    break;
    case 8: 
    retVal = '8';
    break;
    case 9: 
    retVal = '9';
    break;
  }
  return retVal;
}
