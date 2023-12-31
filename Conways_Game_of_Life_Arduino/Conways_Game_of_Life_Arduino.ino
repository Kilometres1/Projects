
// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 96 

// You can use any (4 or) 5 pins 
#define SCLK_PIN 2
#define MOSI_PIN 3
#define DC_PIN   4
#define CS_PIN   5
#define RST_PIN  6

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

// Option 1: use any pins but a little slower
Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);  

// Option 2: must use the hardware SPI pins 
// (for UNO thats sclk = 13 and sid = 11) and pin 10 must be 
// an output. This is much faster - also required if you want
// to use the microSD card (see the image drawing example)
//Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

float p = 3.1415926;
int centreX = tft.width()/2;
int centreY = tft.height()/2;  

int Y =  -1;


void setup(void) {
  Serial.begin(9600);
  Serial.print("hello!");
  tft.begin();

  Serial.println(" beginning:");

  // You can optionally rotate the display by running the line below.
  // Note that a value of 0 means no rotation, 1 means 90 clockwise,
  // 2 means 180 degrees clockwise, and 3 means 270 degrees clockwise.
  //tft.setRotation(1);
  // NOTE: The test pattern at the start will NOT be rotated!  The code
  // for rendering the test pattern talks directly to the display and
  // ignores any rotation.

  uint16_t time = millis();
  tft.fillRect(0, 0, 128, 128, BLACK);
  time = millis() - time;


  tft.fillScreen(BLACK);

  loadingscreen();
  delay(500);  
  tft.fillScreen(BLACK);
 

int oldCellGrid[100] = {//initial grid setup, feel free to change
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,1,0,0,0,0,0,0,0,
0,0,0,1,0,0,0,0,0,0,
0,1,1,1,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,1,0,0,
0,0,0,0,0,0,0,1,0,0,
0,0,0,0,0,0,0,1,0,0,
0,0,0,0,0,0,0,0,0,0,
};

int newCellGrid[100]; // we dont need to define the new grid

const int CellGridSize = sizeof(oldCellGrid)/sizeof(oldCellGrid[0]);//dumb code to get the array working

for (int i = 0; i < CellGridSize;i++){//runs thru each var and prints it to the screen
  if ((i % 10) == 0){Y = Y + 1;}
    if (oldCellGrid[i] == 1){
      //Serial.println(i);
      //Serial.println(oldCellGrid[i]);
      int posY = Y * 4;
      int posX = (i - (Y * 10)) * 4;
      //Serial.println(posX);
      //Serial.println(i);
      //Serial.println(Y);
      tft.drawPixel(posX, posY, WHITE);
    
  }
}
//draws the initiaal board
//for (int i = 0; i < CellGridSize;i++){//runs thru each var and prints it to the screen
//  if ((i % 10) == 0){Y = Y + 1;}
//    if (newCellGrid[i] == 1){
//      //Serial.println(i);
//      //Serial.println(oldCellGrid[i]);
//      int posY = Y * 4;
//      int posX = 45 + (i - (Y * 10)) * 4;
//      //Serial.println(posX);
//      //Serial.println(i);
//      //Serial.println(Y);
//      tft.drawPixel(posX, posY, WHITE);   
//  }
//}


tft.fillRect(0, 0, 128, 96, BLACK);//clears new board

Serial.println("Starting game Loop!");


int gameLoop = 0;
while ( gameLoop < 3){//game loop



//wipes the new board at the start of each cycle
for (int i = 0; i < CellGridSize;i++){//runs thru each var and prints it to the screen
  newCellGrid[i] = 0;
  
}

Y = -1;
Serial.println("checking cells");
for (int i = 0; i < CellGridSize;i++){//runs thru each var and prints it to the screen//for each pixel on the old board
  if ((i % 10) == 0){Y = Y + 1;}
    int liveCheckedCells = 0;
    //check the adjacent pixels (in all 8 directions) \/ \/

    int UpCell        = oldCellGrid[i - 10];
    if (UpCell == 1){liveCheckedCells++;}
    int UpLeftCell    = oldCellGrid[i - 10 - 1];
    if (UpLeftCell == 1){liveCheckedCells++;}
    int LeftCell      = oldCellGrid[i - 1];
    if (LeftCell == 1){liveCheckedCells++;}
    int DownLeftCell  = oldCellGrid[i + 10 - 1];
    if (DownLeftCell == 1){liveCheckedCells++;}
    int DownCell      = oldCellGrid[i + 10];
    if (DownCell == 1){liveCheckedCells++;}
    int DownRightCell = oldCellGrid[i + 10 + 1];
    if (DownRightCell == 1){liveCheckedCells++;}
    int RightCell     = oldCellGrid[i + 1];
    if (RightCell == 1){liveCheckedCells++;}
    int UpRightCell   = oldCellGrid[i - 10 + 1];
    if (UpRightCell == 1){liveCheckedCells++;}
      // game rules
    Serial.println(liveCheckedCells);
    if(liveCheckedCells < 2){
    newCellGrid[i] = 0;//less than 2 adj is killed
    }
    else if(liveCheckedCells > 3){
    newCellGrid[i] = 0;  // more than 3 adj is killed
    }
    else if(liveCheckedCells == 3 && oldCellGrid[i] == 0){
    newCellGrid[i] = 1;// 3 and dead creates life
    }
    else if(liveCheckedCells == 3 || liveCheckedCells == 2){
    newCellGrid[i] = oldCellGrid[i]; //  2 or 3 adjacent maintains life
    }

    
}
Serial.println("made it past cell checker");


//draws new board
Y = -1;
for (int i = 0; i < CellGridSize;i++){
  if ((i % 10) == 0){Y = Y + 1;}
    if (newCellGrid[i] == 1){
      int newPosY = (Y * 4);
      int newPosX = (((i - (Y * 10)) * 4));
      tft.drawPixel(newPosX, newPosY, WHITE);
    }
    else{
      int newPosY = (Y * 4);
      int newPosX = (((i - (Y * 10)) * 4));
      tft.drawPixel(newPosX, newPosY, BLACK);
    }
}

Serial.println("printed new board");


for (int i = 0; i < CellGridSize;i++){//runs thru each var and sets it to the other board
  oldCellGrid[i] = newCellGrid[i];
    
  }
}



gameLoop = gameLoop + 1;
}


void loop() {
  
}








void gameoflife(){//reads cell grid and makes chnages
  
}
void loadingscreen(){

  int radius = 40;
  int xaxis = 0;
  int yaxis = 0;
  for (int count = 0; count < 360; count+=1){    
    xaxis = centreX + cos(count) * radius * 1;
    yaxis = centreY + sin(count) * radius * 1; 
    tft.drawPixel(xaxis, yaxis, WHITE);  
    //delay(100); 
  }
  tft.print("Done!");
  Serial.println("done loading");
delay(500);
}
