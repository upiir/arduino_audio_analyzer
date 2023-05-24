// simple project using Arduino UNO and 128x64 OLED Display to display audio analyzer
// in the real project, Audio Analyzer Module from DFRobot is used
// for this WOKWI version, the bars are moving randomly

// created by upir, 2023
// youtube channel: https://www.youtube.com/upir_upir

// YOUTUBE VIDEO: https://youtu.be/dCofwhHcW7Y

// Links from the video:
// DFRobot Audio Module: https://s.click.aliexpress.com/e/_DeqqzSV
// DFRobot Audio Module Documentation: https://wiki.dfrobot.com/Audio_Analyzer_v2_SKU_DFR0126
// DFRobot Analog Sound Sensor: https://s.click.aliexpress.com/e/_DmUYNQ9
// WOKWI starting project (OLED menu): https://youtu.be/K5e0lFRvZ2E
// 128x64 SSD1306 OLED Display: https://s.click.aliexpress.com/e/_DCYdWXb 
// 128x64 SSD1306 OLED Display (smaller): https://s.click.aliexpress.com/e/_DCKdvnh
// Arduino UNO: https://s.click.aliexpress.com/e/_AXDw1h
// UNO Shield with male pins: https://s.click.aliexpress.com/e/_DcvssKD
// Photopea (online Photoshop-like tool): https://www.photopea.com/
// Arduino breadboard prototyping shield: https://s.click.aliexpress.com/e/_ApbCwx
// u8g2 documentation: https://github.com/olikraus/u8g2/wiki/u8g2reference


// Related videos with Arduino and OLED display:
// Arduino Parking Sensor - https://youtu.be/sEWw087KOj0
// Turbo pressure gauge with Arduino and OLED display - https://youtu.be/JXmw1xOlBdk
// Arduino Car Cluster with OLED Display - https://youtu.be/El5SJelwV_0
// Knob over OLED Display - https://youtu.be/SmbcNx7tbX8
// Arduino + OLED = 3D ? - https://youtu.be/kBAcaA7NAlA
// Arduino OLED Gauge - https://youtu.be/xI6dXTA02UQ
// Smaller & Faster Arduino - https://youtu.be/4GfPQoIRqW8



#include "U8g2lib.h" // include u8g2 library for drawing on the OLED display
#include <AudioAnalyzer.h> // include audio analyzer library from DF Robot - https://github.com/CainZ/DFR0126

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); // ["F" = full framebuffer, size = 1024 bytes, hardware I2C connection]
Analyzer Audio = Analyzer(4,5,A0);//Strobe pin ->4  RST pin ->5 Analog Pin ->A0

byte audio_bar_height[7]; // sizes for the individual bars
byte audio_bar_peak[7]; // positions for the individual peaks (lines over the bars)
int FreqVal[7]; // measured values from the analog analyzer sensor

const unsigned char upir_logo [] PROGMEM = {    // upir logo
  0xEA, 0x3A, 0xAA, 0x28, 0x6A, 0x1A, 0x26, 0x2A, };

void setup() {
  u8g2.begin(); // start the u8g2 library/display
  u8g2.setColorIndex(1);  // set the color to white  
  Audio.Init(); // Init audio analyzer module 
}


void loop() {

  // read values from the audio analyzer
  Audio.ReadFreq(FreqVal);//Return 7 values of 7 bands pass filiter
                          //Frequency(Hz):63  160  400  1K  2.5K  6.25K  16K
                          //FreqVal[]:      0    1    2    3    4    5    6  
  

  u8g2.clearBuffer();  // clear buffer for storing display content in RAM

  for (int i=0; i<7; i++) { // loop for every fraquency (63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25kHz and 16kHz)

    //int random_value = random(1024); // calculate random value between 0-1024
    //audio_bar_height[i] = audio_bar_height[i] + ((map(random_value, 0, 1024, 0, 53) - audio_bar_height[i]) / 4.0); // update the bar with a new value

    audio_bar_height[i] = map( max((FreqVal[i]-100),0) , 0, 1024, 0, 53); // set the bar value based on the measurement from the audio analyzer sensor

    // calculate the peak position
    if (audio_bar_peak[i] < audio_bar_height[i]) { // if the peak is below the current bar size
      audio_bar_peak[i] = audio_bar_height[i]; // move peak to the new max. position (i.e. size of the bar)
    } else if (audio_bar_peak[i] > audio_bar_height[i]) { // if the bar is lower than the peak
      audio_bar_peak[i]--; // slowly move the peak down, one pixel every frame
    }

    u8g2.drawBox(2 + i*19, 53-audio_bar_height[i], 10, audio_bar_height[i]); // draw bar

    u8g2.drawBox(2 + i*19, 53-audio_bar_peak[i], 10, 1); // draw peak
  }

  // draw the individual labels
  u8g2.setFont(u8g2_font_nerhoe_tr);
  u8g2.drawStr(  2, 64, "63"); 
  u8g2.drawStr( 19, 64, "160"); 
  u8g2.drawStr( 37, 64, "400"); 
  u8g2.drawStr( 60, 64, "1K"); 
  u8g2.drawStr( 75, 64, "2.5K"); 
  u8g2.drawStr( 95, 64, "6.3K"); 
  u8g2.drawStr(115, 64, "16K");  

  u8g2.drawXBMP(0, 0, 16, 4, upir_logo);  // draw upir logo             

  u8g2.sendBuffer(); // send buffer from RAM to display controller
}
