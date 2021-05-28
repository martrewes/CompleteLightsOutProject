#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <levels.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET 4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        15 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 25 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int levelSum = 1;
bool LevelComplete = false;
const byte ROWS = 5;
const byte COLS = 5;
char Keys[ROWS][COLS] = {
    {'A', 'B', 'C', 'D', 'E'},
    {'F', 'G', 'H', 'I', 'J'},
    {'K', 'L', 'M', 'N', 'O'},
    {'P', 'Q', 'R', 'S', 'T'},
    {'U', 'V', 'W', 'X', 'Y'}};
int LevelNo = 0;
char Lights[25] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
byte rowPins[COLS] = {13, 12, 14, 27, 26}; //connect to the row pinouts of the keypad
byte colPins[ROWS] = {25, 33, 32, 4, 5};   //connect to the column pinouts of the keypad, 35 and 35 wouldn't work as input pins.
//char Level1[25] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '1', '0', '1', '1', '0', '1', '0', '1'};

char * Levels[] = {Level1,Level2,Level3,Level4,Level5,Level6,Level7,Level8,Level9,Level10,Level11,Level12,Level13,Level14,Level15,Level16,Level17,Level18,Level19,Level20,Level21,Level22,Level23,Level24,Level25,Level26,Level27,Level28,Level29,Level30,Level31,Level32,Level33,Level34,Level35,Level36,Level37,Level38,Level39,Level40,Level41,Level42,Level43,Level44,Level45,Level46,Level47,Level48,Level49,Level50};

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS);

//Declare subroutines
void ChangeData(int toChange[], int count);
void ChangeLights(char customKey);
void startLevel();
void checkLevel();
void PrintLevel();
void displayLEDs();

void setup()
{
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  pixels.begin();

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  pixels.show();
}

void loop()
{

  while (LevelNo < 50)
  {
    LevelComplete = false;
    memcpy(Lights, Levels[LevelNo], sizeof(Lights));
    Serial.println("Level: " + String(LevelNo));
    PrintLevel();
    displayLEDs();
    while (LevelComplete == false)
    {
      if (levelSum > 0)
      {
        startLevel();
      }
      else
      {
        LevelComplete = true;
        LevelNo++;
        Serial.println("Level Complete!!!");
        Serial.println(" ");
        levelSum = 1;
      }
    }
  }
}

void startLevel()
{
  char customKey = customKeypad.getKey();

  if (customKey)
  {
    Serial.println(customKey);
    ChangeLights(customKey);
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.clearDisplay();
    display.println("Pressed:" + String(customKey));
    display.display();
  }
}

void ChangeLights(char customKey)
{

  if (customKey == 'A')
  {
    int toChange[] = {0, 1, 5};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'B')
  {
    int toChange[] = {0, 1, 2, 6};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'C')
  {
    int toChange[] = {1, 2, 3, 7};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'D')
  {
    int toChange[] = {2, 3, 4, 8};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'E')
  {
    int toChange[] = {3, 4, 9};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'F')
  {
    int toChange[] = {0, 5, 6, 10};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'G')
  {
    int toChange[] = {1, 5, 6, 7, 11};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'H')
  {
    int toChange[] = {2, 6, 7, 8, 12};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'I')
  {
    int toChange[] = {3, 7, 8, 9, 13};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'J')
  {
    int toChange[] = {4, 8, 9, 14};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'K')
  {
    int toChange[] = {5, 10, 11, 15};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'L')
  {
    int toChange[] = {6, 10, 11, 12, 16};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'M')
  {
    int toChange[] = {7, 11, 12, 13, 17};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'N')
  {
    int toChange[] = {8, 12, 13, 14, 18};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'O')
  {
    int toChange[] = {9, 13, 14, 19};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'P')
  {
    int toChange[] = {10, 15, 16, 20};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'Q')
  {
    int toChange[] = {11, 15, 16, 17, 21};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'R')
  {
    int toChange[] = {12, 16, 17, 18, 22};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'S')
  {
    int toChange[] = {13, 17, 18, 19, 23};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'T')
  {
    int toChange[] = {14, 18, 19, 24};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'U')
  {
    int toChange[] = {15, 20, 21};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'V')
  {
    int toChange[] = {16, 20, 21, 22};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'W')
  {
    int toChange[] = {17, 21, 22, 23};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'X')
  {
    int toChange[] = {18, 22, 23, 24};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
  if (customKey == 'Y')
  {
    int toChange[] = {19, 23, 24};
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0]));
  }
}

void ChangeData(int toChange[], int count)
{
  levelSum = 0;
  Serial.println("Going into routine");
  for (int No = 0; No < count; No++)
  {
    if (Lights[toChange[No]] == '0')
    {
      Lights[toChange[No]] = '1';
    }
    else if (Lights[toChange[No]] == '1')
    {
      Lights[toChange[No]] = '0';
    }
  }

  for (int i = 0; i < 24; i++)
  {
    if (Lights[i] == '1')
    {
      levelSum = levelSum + 1;
    }
    else
    {
      levelSum = levelSum;
    }
  }
  Serial.println("Level: " + String(LevelNo));
  PrintLevel();
  Serial.println("Tot. On: " + String(levelSum + 1));
  displayLEDs();
}

void PrintLevel()
{
  for (byte i = 0; i < 5; i++)
  {
    Serial.print(Lights[i]);
  }
  Serial.println();
  for (byte i = 5; i < 10; i++)
  {
    Serial.print(Lights[i]);
  }
  Serial.println();
  for (byte i = 10; i < 15; i++)
  {
    Serial.print(Lights[i]);
  }
  Serial.println();
  for (byte i = 15; i < 20; i++)
  {
    Serial.print(Lights[i]);
  }
  Serial.println();
  for (byte i = 20; i < 25; i++)
  {
    Serial.print(Lights[i]);
  }
  Serial.println();
}

void displayLEDs()
{
  int adj = 24;
  pixels.clear();
  for (int i = 0; i <= 24; i++)
  {

    //int adj = 24;
    if (Lights[i] == '1'){
      //Serial.println(String(i) + " is on " + Lights[i]);
      pixels.setPixelColor(adj, pixels.Color(0, 50, 0));

    }
    else if (Lights[i] == '0') {
      //Serial.println(String(i) + " is off " + Lights[i]);
      pixels.setPixelColor(adj, pixels.Color(0, 0, 0));

    }
    adj--;

  }
  pixels.show();

}
