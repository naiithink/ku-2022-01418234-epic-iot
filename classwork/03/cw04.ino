
#include<TM1637Display.h>

#define CLK A13
#define DIO A12

TM1637Display display = TM1637Display(CLK, DIO);

// Create an array that turns all segments ON
const uint8_t allON[] = {0xff, 0xff, 0xff, 0xff};

// Create an array that turns all segments OFF
const uint8_t allOFF[] = {0x00, 0x00, 0x00, 0x00};

// Create an array that sets individual segments per digit to display the word "dOnE"
const uint8_t done[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

// Create degree celsius symbol
const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Degree symbol
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};



int displayNum = 0;
int hour = 0;
int mins = 0;

String dir="";
unsigned long last_run = 0;


void shaft_moved(){
  if(millis() - last_run> 5){
    if (digitalRead(17) == 1){
        if(hour != 0){
          mins+=15;
        }else{
          mins +=5;
        }
      if (mins == 60){
          mins = 0;
          hour+=1;
      }

      displayNum = (hour * 100) + mins ;
       
      display.showNumberDecEx(displayNum, 0b11100000, false ,4,0);


    }
    
    last_run = millis();

  }

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  attachInterrupt(18,shaft_moved,CHANGE);
  pinMode(17,INPUT);
  
  

}

void loop() {
  display.setBrightness(2);
      // Set all segments ON
    // display.setSegments(allON);
 display.showNumberDecEx(displayNum, 0b11100000, true,4,0);




}