#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SimpleTimer.h>

String preString;     // this string contains the response from the BT module
String inString;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SimpleTimer timer;

void lcd_display()
{
    lcd.backlight();    
    lcd.clear();     
    lcd.setCursor(0,0);           // 0번째 줄 0번째 셀부터 입력하게 합니다.
    lcd.print(preString);       // 문구를 출력합니다.
    lcd.setCursor(0,1);           // 1번째 줄 0번째 셀부터 입력하게 합니다.
    lcd.print(inString);     // 문구를 출력합니다.
    preString =   inString;
    delay(100);
    lcd.noBacklight();
}


void setup(){

  pinMode(8, OUTPUT);    //HC-05
  digitalWrite(8,HIGH);
  Serial.begin(115200);     // Serial to PC
  Serial1.begin(38400);     // Serial to HC-05 BlueTooth module in "AT" mode always @38k4
  lcd.init();
  lcd.backlight();
  timer.setInterval(60000, inquiry);        
  delay(1000);
  
  
  Serial1.print("AT+NAME=HelloLand\r\n");
  delay(100);  
  Serial1.print("at+init\r\n");  
  delay(100);    
  Serial1.print("at+iac=9e8b33\r\n"); 
  delay(100);     
  Serial1.print("at+class=0\r\n ");  
  delay(100);    
  Serial1.print("at+inqm=1,9,48\r\n");
  delay(1000);  
          
}

void inquiry(){
    Serial1.print("at+inq\r\n");
}

void loop(){
  timer.run();  
  if (Serial1.available()) {

    inString = Serial1.readStringUntil('\n');    
    int index1 = inString.indexOf('/');
    inString =  inString.substring(0, index1);
 
    Serial.println(inString);
    lcd_display();
  }
  if (Serial.available())
    Serial1.write(Serial.read());

}
