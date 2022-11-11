// OLED_pin_scl_sck
// OLED_pin_sda_mosi
// OLED_pin_cs_ss
// OLED_pin_res_rst
// OLED_pin_dc_rs


#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128

#define OLED_pin_scl_sck 11
#define OLED_pin_sda_mosi 12
#define OLED_pin_dc_rs 14
#define OLED_pin_res_rst 13
#define OLED_pin_cs_ss 15

#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF
#define ORANGE          0xFB00


#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

const int BarDistanceDown = 17;
const int BarThickness = 5;
const int BarRoundedThickness = 2;

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_pin_cs_ss, OLED_pin_dc_rs, OLED_pin_sda_mosi, OLED_pin_scl_sck, OLED_pin_res_rst);

void CreateItem(char *str, int PosY){
  tft.setCursor(0,PosY);
  tft.print(str);
}

void setup(void){
  Serial.begin(9600);
  tft.begin();
  tft.fillRect(0, 0, 128, 128, BLACK);

  tft.setTextSize(2);
  tft.setTextColor(GREEN);

  tft.setCursor(0,0);

 
  CreateItem("CPU",0);
  CreateItem("RAM",32);
  CreateItem("GPU",64);
  CreateItem("VRAM",96);


}

void UpdateItem(float ItemPercent, int PosY){
  unsigned long Color = 0xFFFF;

  if(ItemPercent > 90){Color = RED;}else{Color = BLUE;};
  tft.setTextColor(Color);
  tft.setTextSize(1);
  tft.setCursor(90,PosY + 7);

  tft.fillRect(90, PosY + 7, 38, 8, BLACK);
  tft.print(String(ItemPercent, 1) + "%");

  tft.fillRoundRect(0, PosY+BarDistanceDown, 128, BarThickness, BarRoundedThickness, WHITE);
  tft.fillRoundRect(0, PosY+BarDistanceDown, int(1.28*ItemPercent), BarThickness, BarRoundedThickness , Color);
}

void loop() {
  while (Serial.available() == 0) {}
  String input = Serial.readString();

  int indexSize =  input.length()-1;
  float DataOutput[10];
  int DataUpto = 0;
  char buffer[10];
  int bufferUpto = 0;

  for (int i = 0; i <= indexSize; i++) {
    if(input.charAt(i)==','){
      DataOutput[DataUpto] = String(buffer).toFloat();
      memset(buffer, ' ', sizeof(buffer));
      bufferUpto = 0;      
      DataUpto++;
      
    }else{
      buffer[bufferUpto] = input.charAt(i);
      bufferUpto++;

    }
  }

  float GpuUsage = DataOutput[0];
  float GpuVramUsed = DataOutput[1];
  float GpuVram = DataOutput[2];
  float CpuUsage = DataOutput[3];
  float UsedRam = DataOutput[4];
  float TotalRam = DataOutput[5];
  float RamPercentage = (UsedRam / TotalRam) * 100;
  float VramPercentage = (GpuVramUsed / GpuVram) * 100;
  
  UpdateItem(CpuUsage,0);
  UpdateItem(RamPercentage,32);
  UpdateItem(GpuUsage,64);
  UpdateItem(VramPercentage,96);

}
