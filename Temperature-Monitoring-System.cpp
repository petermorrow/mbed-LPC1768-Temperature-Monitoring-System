//Assessed Lab 2 - Temperature Monitoring System with alarm
//Peter Morrow - 40204698
//Make sure appropriate libaries are imported to this project 
#include "mbed.h"
#include "LM75B.h"
#include "C12832.h" //Different Library to C12832_lcd.h

C12832 lcd(p5, p7, p6, p8, p11);

LM75B sensor(p28,p27);// Temp Sensor connection on dev board
Serial pc(USBTX,USBRX);

AnalogIn Pot1(p19); //Pot 1
AnalogIn Pot2(p20); //Pot 1

DigitalOut RED_led(p23);//LED for alarm 
DigitalOut BLUE_led(p25);//LED for alarm 
DigitalOut GREEN_led(p24);//LED to signify remote mode

InterruptIn JoyStick_Centre(p14); // Allocate Joystick Press to this interrupt 
InterruptIn JoyStick_Up(p15); // Allocate Up Joystick to this interrupt
InterruptIn JoyStick_Down(p12); // Allocate Down Joystick to this interrupt


int STATE = 0; //Manual STATE : = 1 Remote STATE 
int tmp_char; // Declare character variable as integer

float Upper_Alarm;
float Lower_Alarm;

float Sensor_Value = sensor; //Read temp sensor value into global float holder  

void Change_Upper_Alarm(){
    
    Upper_Alarm = Pot1.read()*50;
    
        if (Upper_Alarm <= 10){
            Upper_Alarm = 10;
            }
            
        if (Upper_Alarm >= 40){
            Upper_Alarm = 40;
            }  
    }
    
    
void Change_Lower_Alarm(){
    
    Lower_Alarm = Pot2.read()*50;
    
        if (Lower_Alarm <= 0){
            Lower_Alarm = 0;
            }
        if (Lower_Alarm >= 25){ 
            Lower_Alarm = 25;
            }
    }
    
void Test_Temp(){
    if(Sensor_Value >= Upper_Alarm){//When Temp is above the Upper alarm limit - Red LED turns on 
        RED_led=0; //RED LED ON
    }
    else if(sensor <= Lower_Alarm){ //When Temp is below the lower alarm limit - Blue LED turns on 
        BLUE_led=0;
    }
    
    else{
        RED_led=1; //RED LED OFF
        BLUE_led=1;//BLUE LED OFF
        }
 }
 
 void JoyStick_Centre_ISR(){
    
    if(pc.readable()==1)//Check if serial port has a char waiting
    pc.printf("Remote Connection Established...");
    wait(0.1);
    }
    
    
    
void JoyStick_Up_ISR(){
    
    STATE = 1;
    wait(0.1);//debounce timer
    }
    
    
    
void JoyStick_Down_ISR(){
    
    STATE = 0;
    wait(0.1);//debounce timer
    }
    

int main ()
{
    
    RED_led = 1;//LED initially off
    BLUE_led = 1;//LED initially off
    GREEN_led = 1;//LED initially off
    JoyStick_Centre.rise(&JoyStick_Centre_ISR);
    JoyStick_Up.rise(&JoyStick_Up_ISR);   
    JoyStick_Down.rise(&JoyStick_Down_ISR);

 if (sensor.open()) {
        while(1){
            
            if (STATE == 1){
                GREEN_led = 0; // Green LED ON 
                
               // if(pc.readable()==1)//Check if serial port has a char waiting
                
                tmp_char = pc.getc(); // Read char from serial port
                
                if (tmp_char == 113){ //ASCII for letter q
                    Upper_Alarm++;
                    wait(0.1);//debounce timer
                    }
                else if (tmp_char == 97){  //ASCII for letter a
                    Upper_Alarm--;
                    wait(0.1);//debounce timer
                    }
                else if (tmp_char == 119){ //ASCII for letter w
                    Lower_Alarm++;
                    wait(0.1);//debounce timer
                    }
                else if (tmp_char == 115){  //ASCII for letter s
                    Lower_Alarm--;
                    wait(0.1);//debounce timer
                    }
                else {
                    wait(0.1);
                    }
                    
                Test_Temp();
                if(Sensor_Value >= Upper_Alarm){//When Temp is above the Upper alarm limit - Red LED turns on 
                GREEN_led = 1; // Green LED OFF 
                }
                
                else if(sensor <= Lower_Alarm){ //When Temp is below the lower alarm limit - Blue LED turns on 
                GREEN_led = 1; // Green LED OFF
                }
                else {
                GREEN_led = 0; // Green LED ON 
                    }
                
                lcd.locate(2,1);
                lcd.printf("Current Temp:     %.1f\n", Sensor_Value);
                wait(0.1);
                
                if (Upper_Alarm <= Lower_Alarm){
                Upper_Alarm = Lower_Alarm;
                }
                if (Upper_Alarm <= 10){
                Upper_Alarm = 10;
                }
                if (Upper_Alarm >= 40){
                Upper_Alarm = 40;
                }
                lcd.locate(2,10);
                lcd.printf("High Limit (10-40): %.1f\n", Upper_Alarm);
                wait(0.1);
                
                if (Lower_Alarm >= Upper_Alarm){
                Lower_Alarm = Upper_Alarm;
                }
                if (Lower_Alarm <= 0){
                Lower_Alarm = 0;
                }
                if (Lower_Alarm >= 25){ 
                Lower_Alarm = 25;
                }
                lcd.locate(2,20);
                lcd.printf("Low Limit (0-25):   %.1f\n", Lower_Alarm);
                
                
                }
            
            if (STATE == 0){
                
            GREEN_led = 1; //Green LED OFF
            
            Test_Temp();
            lcd.locate(2,1);
            lcd.printf("Current Temp:     %.1f\n", Sensor_Value);
            wait(0.1);
            
            Change_Upper_Alarm();
            if (Upper_Alarm <= Lower_Alarm){
            Upper_Alarm = Lower_Alarm;
            }
            lcd.locate(2,10);
            lcd.printf("High Limit (10-40): %.1f\n", Upper_Alarm);
            wait(0.1);
            
            Change_Lower_Alarm();
            if (Lower_Alarm >= Upper_Alarm){
            Lower_Alarm = Upper_Alarm;
            }
            lcd.locate(2,20);
            lcd.printf("Low Limit (0-25):   %.1f\n", Lower_Alarm);
            
            }
        }
    }
         
}

