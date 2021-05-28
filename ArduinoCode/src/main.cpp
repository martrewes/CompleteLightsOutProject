#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <levels.h>

// Definitions and initialisation for the screen
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 
#define OLED_RESET 4        
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definitions and initialisation for the LED "strip"
#define PIN       15 
#define NUMPIXELS 25 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Declaring variables required for the game
int levelSum = 1;             //used to tell if the level is complete
bool LevelComplete = false;   //used to pull out of the level loop
int LevelNo = 0;              //used to pull level data into currentLevel[]
int lvlMoves = 0;

// Declarations and initialisation of the keypad (switch matrix)
const byte ROWS = 5;
const byte COLS = 5;
char Keys[ROWS][COLS] = {
    {'A', 'B', 'C', 'D', 'E'},
    {'F', 'G', 'H', 'I', 'J'},
    {'K', 'L', 'M', 'N', 'O'},
    {'P', 'Q', 'R', 'S', 'T'},
    {'U', 'V', 'W', 'X', 'Y'}};
byte rowPins[COLS] = {13, 12, 14, 27, 26}; //connect to the row pinouts of the keypad
byte colPins[ROWS] = {25, 33, 32, 4, 5};   //connect to the column pinouts of the keypad, 35 and 35 wouldn't work as input pins.
Keypad customKeypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS);

// Define the variable that will contain the current level information on change
char currentLevel[25] = {'0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'};
// Levels[] contains a list of all of the level information, included in levels.h
char * Levels[] = {Level1,Level2,Level3,Level4,Level5,Level6,Level7,Level8,Level9,Level10,
                   Level11,Level12,Level13,Level14,Level15,Level16,Level17,Level18,Level19,
                   Level20,Level21,Level22,Level23,Level24,Level25,Level26,Level27,Level28,
                   Level29,Level30,Level31,Level32,Level33,Level34,Level35,Level36,Level37,
                   Level38,Level39,Level40,Level41,Level42,Level43,Level44,Level45,Level46,
                   Level47,Level48,Level49,Level50};



//Declare subroutines
void ChangeData(int toChange[], int count);
void ChangeLights(char customKey);
void awaitKeyPress();
void checkLevel();
void PrintLevel();
void displayLEDs();
void updateDisplay();

void setup()
{
  Serial.begin(9600); //Keeping on hand just in case, for now

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  //Starts LEDs
  pixels.begin();
  //Clear and display nothing on OLED (stops splash screen)
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  //settle
  delay(500);
}

void loop()
{
// Nested loops make up the level logic. So far only one colour, but I am hoping to add another. As well as more levels too.
// I will need to somehow make a menu system in order to do this, but as this is just software I can worry about that when
// I have completed the hardware build. So far I have 50 Levels to start with. All from the LightsOut 2000 Game.
  while (LevelNo < 50)
  {
  //Initialise the Level -----
    //Level complete needs to be set as false, this will allow me to pull out of the current level loop
    LevelComplete = false;
    // Copy level data into currentLevel array
    memcpy(currentLevel, Levels[LevelNo], sizeof(currentLevel));
    // Show the level on the LEDs
    displayLEDs();
    //Update the display to reflect level change
    updateDisplay();

    //This is the first varification to tell if the level has been complete
    while (LevelComplete == false)
    {
      //Second varification, so I can come out of this if statement, flip the levelComplete bool, then initialise the next level.
      if (levelSum > 0)
      {
        awaitKeyPress();
      }
      else
      {
        //Flip the bool
        LevelComplete = true;
        //Add 1 to LevelNo, and reset number of moves and level sum.
        LevelNo++;
        lvlMoves = 0;
        levelSum = 1;
      }
    }
  }
}

void awaitKeyPress()
{
  //Waits for a key to be pressed in the switch matrix
  char customKey = customKeypad.getKey();
  //If there is something..
  if (customKey)
  {
    lvlMoves++;                 // Add 1 to moves counter
    ChangeLights(customKey);    // Modify current level array using the pressed key
    displayLEDs();              // Reflect the change on the LEDs
    updateDisplay();            // Update the OLED to show move counter increased
  }
}

void ChangeLights(char customKey)
{
  // The keys are laid out in a 5x5 matrix, and I wanted to ensure I had a few spare characters in case I wanted to add 
  // more control buttons in the future
  // I did try to clean this up a bit, by moving out the change data routine from each if statement, but unfortunately
  // you can not change an array size once it has been declared by the seems of things.

  if (customKey == 'A')
  {
    // Creates an array to pass to the change data routine
    int toChange[] = {0, 1, 5};                                   // These are the locations that will be manipulated int he matrix
    ChangeData(toChange, sizeof(toChange) / sizeof(toChange[0])); // Run the change data routine, which passes in the array and size
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
  // Temporarily set the level sum to 0, so it can be recounted
  levelSum = 0;
  // Start a for loop to only change relevent data in the array
  for (int No = 0; No < count; No++)
  {
    // If the character in the array at the position supplied is equal to 0, change it to 1, else if it's 1, change it to zero.
    // I deliberatly used an else if, so I could possibly add more characters (more colours) in the future 
    if (currentLevel[toChange[No]] == '0')
    {
      currentLevel[toChange[No]] = '1';
    }
    else if (currentLevel[toChange[No]] == '1')
    {
      currentLevel[toChange[No]] = '0';
    }
  }
  // Start the counting routine to see if any lights are still on
  for (int i = 0; i < 24; i++)
  {
    if (currentLevel[i] == '1')   // This may be changed to "!= '0' " to account for different characters in the future
    {
      levelSum = levelSum + 1;
    }
    else
    {
      levelSum = levelSum; //Don't really think I need this statement
    }
  }
}

void PrintLevel()
{
  // This routine just printed out the level map to the serial port. Was useful before the PCBs showed up.
  // I will keep this routine in for the time being in the case that I need it (eg. Developing without the hardware)
  for (byte i = 0; i < 5; i++)
  {
    Serial.print(currentLevel[i]);
  }
  Serial.println();
  for (byte i = 5; i < 10; i++)
  {
    Serial.print(currentLevel[i]);
  }
  Serial.println();
  for (byte i = 10; i < 15; i++)
  {
    Serial.print(currentLevel[i]);
  }
  Serial.println();
  for (byte i = 15; i < 20; i++)
  {
    Serial.print(currentLevel[i]);
  }
  Serial.println();
  for (byte i = 20; i < 25; i++)
  {
    Serial.print(currentLevel[i]);
  }
  Serial.println();
}

void displayLEDs()
{
  int adj = 24;     // Okay so this was my fix to the mistake I made to the PCB. Ideally I wouldn't need this, but I wasn't just going to order another lot of PCBs
                    // as I was able to change it in code. This will be commented out once I have fixed the PCB design.
  pixels.clear();   // Clear the data for the LEDs so it can be rebuilt.
  for (int i = 0; i <= 24; i++)
  {
    if (currentLevel[i] == '1'){
      // If the current (adjusted) location contains a 1, light up that LED Green
      pixels.setPixelColor(adj, pixels.Color(0, 50, 0));
    }
    else if (currentLevel[i] == '0') {
      // If it's a 0, do not light it.
      pixels.setPixelColor(adj, pixels.Color(0, 0, 0));
    }
    adj--;          // Remove 1 from the adjustment so it can continue working backwards
  }
  pixels.show();    // Show the changes to the LEDs
}

void updateDisplay() {
  // A useall routine to change the data on the OLED display
  display.clearDisplay();                         // Clear the buffer.
  display.drawBitmap(0,0,bgImage,128,32,WHITE);   // Draw the background.
  display.setCursor(28, 10);                      // Set the cursor and display the Level No
  display.print(String(LevelNo + 1));             // as it starts at zero, add 1.
  display.setCursor(98, 10);                      // Move the cursor to the second location
  display.print(String(lvlMoves));                // to display the amount of moves.
  display.display();                              // Write to the display.
}

// Current Stats.
//      RAM:   [          ]   5.0% (used 16296 bytes from 327680 bytes)
//      Flash: [==        ]  19.3% (used 252998 bytes from 1310720 bytes)