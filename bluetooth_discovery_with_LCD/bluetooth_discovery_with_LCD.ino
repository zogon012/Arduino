#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SimpleTimer.h>

String preString = "Made By 012";     // this string contains the response from the BT module
String inString = "Youngil Kim";
LiquidCrystal_I2C lcd(0x27, 16, 2);
SimpleTimer timer;
#define INTERVAL  30000
#define CR_LF "\r\n"

void spend_time(int time) {
  delay(time);
}

void lcd_display(){
    lcd.backlight();    
    lcd.clear();     
    lcd.setCursor(0,0);           // 0번째 줄 0번째 셀부터 입력하게 합니다.
    lcd.print(preString);       // 문구를 출력합니다.
    lcd.setCursor(0,1);           // 1번째 줄 0번째 셀부터 입력하게 합니다.
    lcd.print(inString);     // 문구를 출력합니다.
    preString =   inString;
    spend_time(1000);
    lcd.noBacklight();
}

void send_string_to_bt_module(String command) {
    Serial1.print(command + CR_LF);
}

void initializing(){
  send_string_to_bt_module("AT+NAME=HelloLand");
  spend_time(100);  
  send_string_to_bt_module("at+init");  
  spend_time(100);    
  send_string_to_bt_module("at+iac=9e8b33"); 
  spend_time(100);     
  send_string_to_bt_module("at+class=0");  
  spend_time(100);    
  send_string_to_bt_module("at+inqm=1,1,48"); //Inquire access mode , the maximum of Bluetooth devices response, The maximum of limited inquiring time 
  spend_time(1000);
  lcd_display(); 
}

void setup_pin_setting() {
  pinMode(8, OUTPUT);    //HC-05
  digitalWrite(8,HIGH);
  Serial.begin(115200);     // Serial to PC
  Serial1.begin(38400);     // Serial to HC-05 BlueTooth module in "AT" mode always @38k4
  lcd.init();
  lcd.backlight();
  timer.setInterval(INTERVAL, inquiry);      
}

void inquiry(){
  send_string_to_bt_module("at+inq");
}

void read_remote_name(String ba_addr) {
  String cmd = "at+rname?";
  cmd = cmd + ba_addr;
  send_string_to_bt_module(cmd + CR_LF);
}

void parsing_string_with_exec() {
    if(inString.startsWith("+INQ") == 1) {
      inString = inString.substring(5,19);
      inString.replace(':', ',');
      Serial.println(inString);
      read_remote_name(inString);
    }
    if(inString.startsWith("+RNAME") == 1) {
      inString = inString.substring(7);
      Serial.println(inString);      
      lcd_display();
    }
}

void setup(){
  setup_pin_setting();
  spend_time(1000);   
  initializing();          
}

void loop(){
  timer.run();  
  if (Serial1.available()) {
    inString = Serial1.readStringUntil('\n');
    parsing_string_with_exec();
  }
  if (Serial.available())
    Serial1.write(Serial.read());
}
