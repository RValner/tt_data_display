#include <Adafruit_GFX.h> // Hardware-specific library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

char *msg[] = { "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV" };
uint8_t val = 0;
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define WHITE   0xFFFF

const unsigned int MAX_MESSAGE_LENGTH = 12;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    tft.reset();
    uint16_t identifier = tft.readID();
    Serial.print("ID = 0x");
    Serial.println(identifier, HEX);
    if (identifier == 0xEFEF) identifier = 0x9486;
    tft.begin(identifier);
    
    tft.setRotation(1);
    tft.fillScreen(0x0000);
    tft.setTextColor(WHITE, BLACK);

    tft.setTextSize(3);
    tft.setCursor(30, 50);
    tft.println("BOOST");
    tft.setCursor(180, 50);
    tft.println("OIL P");
    tft.setCursor(330, 50);
    tft.println("OIL T");

    tft.setTextSize(5);
}

void loop()
{
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    //Full message received...
    else
    {
      //Add null character to string
      message[message_pos] = '\0';

      //Print the message (or do other things)
      tft.setCursor(50, 150);
      tft.println(message);
 
      //Reset for the next message
      message_pos = 0;
    }
  }
//  //tft.fillRect(50, 150, 360, 50, BLACK);
//  tft.setCursor(50, 150);
//  tft.println(String(val));
//
//  tft.setCursor(190, 150);
//  tft.println(String(val/2));
//
//  tft.setCursor(340, 150);
//  tft.println(String(val/3));
  
  //val++;
  //delay(100);
  
//    // put your main code here, to run repeatedly:
//    uint16_t x = 50, y = 100;
//    
//    
//    tft.println(msg[aspect]);
//    tft.setCursor(x, y);
//    tft.println("[x=" + String(x) + ",y=" + String(y) + "]");
//    delay(5000);
//    
//    tft.invertDisplay(true);
//    delay(1000);
//    tft.invertDisplay(false);
//    tft.println("INVERT OFF");
    //if (++aspect >= 4) aspect = 0;
}
