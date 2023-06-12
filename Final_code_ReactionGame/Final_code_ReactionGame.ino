#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// GPIO touch pins: 0, 2, 4, 12, 13, 14, 15, 27, 32, 33
// 0 en 2 werken niet -> gereserveerd op deze ESP

// Leddelen
const int numberOne[] = {0, 1, 2, 3};
const int numberTwo[] = {4, 5, 6, 7};
const int numberThree[] = {8, 9, 10, 11};
const int numberFour[] = {12, 13, 14, 15};
const int numberFive[] = {16, 17, 18, 19};
const int numberSix[] = {20, 21, 22, 23};
const int numberSeven[] = {24, 25, 26, 27};
const int numberEight[] = {28, 29, 30, 31};
const int numberNine[] = {32, 33, 34, 35};
const int numberTen[] = {36, 37, 38, 39};
const int numberEleven[] = {40, 41, 42, 43};
const int numberTwelve[] = {44, 45, 46, 47};
const int numberThirtheen[] = {48, 49, 50, 51};
const int numberFourteen[] = {52, 53, 54, 55};
const int numberFifteen[] = {56, 57, 58, 59};
const int numberSixteen[] = {60, 61, 62, 63};
const int numberSeventeen[] = {64, 65, 66, 67};
const int numberEighteen[] = {68, 69, 70, 71};
const int numberNineteen[] = {72, 73, 74, 75};

// Constants
#define ATTEMPTS 1
#define THRESHOLD 17
#define WAIT_TIME random(1000, 4000); // Milliseconds for next round
#define LED_PIN1 16
#define LED_PIN2 17
#define LED_AMOUNT1 77 // 76 leds
#define LED_AMOUNT2 77 // 76 leds

const int TOUCH_PINS1[] = {4, 12, 13, 14};
const int TOUCH_PINS2[] = {15, 27, 32, 33};

Adafruit_NeoPixel strip1(LED_AMOUNT1, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_AMOUNT2, LED_PIN2, NEO_GRB + NEO_KHZ800);

// For testing only
// const int TOUCH_PINS1[] = {4};
// const int TOUCH_PINS2[] = {27};

// Variables
int player1 = 1, player2 = 2;
int green = 1, red = 2, yellow = 3;
int startButton = 4;

// Function prototypes
int startGame(int player);
void printTime(int timePerRound, int totalTime);
void blinkLeds(int color, int side);
void fadeIn(uint32_t color, int startLed, int endLed, int side);
void fadeOut(uint32_t color, int startLed, int endLed, int side);
void setBrightness(uint32_t color, int brightness, int startLed, int endLed, int side);
int randomButton(int player);
void activateButton(int player, int buttonPlayer);
int waitForTouch(int button);
int determineWinner(int scorePlayer1, int scorePlayer2);
void showWinner(int winner);
void turnOnLeds(int button, int color);
void turnOffLeds();
void wait(int waitMillis);

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  delay(1000); // Delay to launch the serial monitor
  randomSeed(analogRead(0));
  strip1.begin();
  strip2.begin();
  strip1.clear(); // Set all pixel colors to 'off'
  strip2.clear(); // Set all pixel colors to 'off'
  Serial.println("Setup complete");
}

void loop()
{
  int startValue;
  Serial.println("Game starts with holding startbutton");
  do
  {
    blinkLeds(green, player1);
    // blinkLeds(yellow, 0);
    // blinkLeds(red, 0);
    // blinkLeds(yellow, 0);
    // blinkLeds(green, 0);
    startValue = touchRead(startButton);
    Serial.print("Value of touch from startbutton: ");
    Serial.println(startValue);
  } while (startValue > THRESHOLD);

  //waitForTouch(startButton);
  blinkLeds(yellow, player1);
  int timePlayer1 = startGame(player1);
  blinkLeds(yellow, player2);
  int timePlayer2 = startGame(player2);
  int winner = determineWinner(timePlayer1, timePlayer2);
  showWinner(winner);
}

int startGame(int player)
{
  int reactionTime = 0;
  int buttonPlayer = 0;
  Serial.print("Starting now!");
  delay(3000);

  if (player == 1)
  {
    Serial.println("Player 1 turn");
    //delay(1000);
  }
  else if (player == 2)
  {
    Serial.println("Player 2 turn");
   // delay(1000);
  }
  else
  {
    Serial.println("Error, no valid player selected");
  }

  for (int a = 0; a < ATTEMPTS; a++)
  {
    Serial.print("Touch button: ");
    buttonPlayer = randomButton(player);
    Serial.println(buttonPlayer);
    activateButton(player, buttonPlayer);
    int timePerRound = waitForTouch(buttonPlayer);
    turnOffLeds();
    reactionTime += timePerRound;
    printTime(timePerRound, reactionTime);
    int waitTime = random(1000, 5500);
    delay(waitTime);
  }

  return reactionTime;
}

int randomButton(int player)
{
  int randomIndex;
  int buttonChosen = 0;
  if (player == 1)
  {
    randomIndex = random(0, sizeof(TOUCH_PINS1) / sizeof(int));
    buttonChosen = TOUCH_PINS1[randomIndex];
  }
  else if (player == 2)
  {
    randomIndex = random(0, sizeof(TOUCH_PINS2) / sizeof(int));
    buttonChosen = TOUCH_PINS2[randomIndex];
  }
  else
  {
    Serial.println("ERROR, no chosen player");
  }
  //Serial.println("Random button chosen");
  return buttonChosen;
}

void activateButton(int player, int buttonPlayer)
{
  // int button;
  int buttonNumber;
  int buttonToLedMapping[] = {-1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 1, 2, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, -1, -1, -1, -1, 6, 7};
  //                           0   1   2   3  4   5   6   7   8   9   10 11 12  13 14 15  16  17  18  19  20  21  22  23  24  25 26 27   28  29  30  31 32 33

  buttonNumber = buttonToLedMapping[buttonPlayer];

  if (buttonNumber == -1)
  {
    Serial.println("Error, activatebutton2");
  }
  else
  {
    //int randomColor = random(1, 4);
    turnOnLeds(buttonNumber, yellow);
  }
  Serial.println("Button activated");
}

int waitForTouch(int button)
{
  int capValue;
  unsigned long currentMillis, nextMillis;

  currentMillis = millis();
  capValue = touchRead(button);
  do
  {
    capValue = touchRead(button);
  } while (capValue > THRESHOLD);

  nextMillis = millis();
  Serial.println("Touched, time captured");
  return nextMillis - currentMillis;
}

void printTime(int timePerRound, int totalTime)
{
  unsigned long secondsRound = timePerRound / 1000;      // Convert milliseconds to seconds
  unsigned long millisecondsRound = timePerRound % 1000; // Extract milliseconds
  unsigned long secondsTotal = totalTime / 1000;         // Convert milliseconds to seconds
  unsigned long millisecondsTotal = totalTime % 1000;    // Extract milliseconds
  Serial.print("Time for round: ");
  Serial.print(secondsRound);
  Serial.print(",");
  Serial.print(millisecondsRound);
  Serial.print("s");
  Serial.print("      ");
  Serial.print("Total time:  ");
  Serial.print(secondsTotal);
  Serial.print(",");
  Serial.print(millisecondsTotal);
  Serial.println("s");
}

int determineWinner(int timePlayer1, int timePlayer2)
{
  Serial.println("Winner determined");
  if (timePlayer1 < timePlayer2)
  {
    return 1;
  }
  if (timePlayer1 > timePlayer2)
  {
    return 2;
  }
  return 0;
}
//
void showWinner(int winner)
{
  if (winner == 1)
  {
    Serial.println("Player 1 wins!");
    // Leds bij player 1 groen knipperen
    blinkLeds(green, 1);
    // Leds bij player 2 rood
    for (int i = 4; i < 7; i++)
    {
      turnOnLeds(i, red);
    }
  }
  else if (winner == 2)
  {
    Serial.println("Player 2 wins!");
    // Leds bij player 2 groen knipperen
    blinkLeds(green, 2);
    // Leds bij player 1 rood
    for (int i = 0; i < 3; i++)
    {
      turnOnLeds(i, red);
    }
  }
  else
  {
    Serial.println("It's a tie");
    // Leds bij beide spelers groen knipperen
    blinkLeds(green, 0);
  }
  Serial.println("Game over.");
}

void blinkLeds(int color, int side)
{
  Serial.println("Blinking leds");
  // if (side == 3){
  //     fadeIn(strip1.Color(0, 127, 0), numberOne[0], numberNineteen[3], side);  // Fade in with green color
  //     delay(1000);                                                             // Wait for a second
  //     fadeOut(strip1.Color(0, 127, 0), numberOne[0], numberNineteen[3], side); // Fade out with green color
  //     delay(1000);                                                             // Wait for a second
  //     break;
  // } STARTKNOP????
  if (side == 0)
  {
    switch (color)
    {
    case 1:
      // green
      fadeIn(strip1.Color(0, 127, 0), numberOne[0], numberNineteen[3], side);  // Fade in with green color
      fadeIn(strip2.Color(0, 127, 0), numberOne[0], numberNineteen[3], side);  // Fade in with green color
      delay(1000);                                                             // Wait for a second
      fadeOut(strip1.Color(0, 127, 0), numberOne[0], numberNineteen[3], side); // Fade out with green color
      fadeOut(strip2.Color(0, 127, 0), numberOne[0], numberNineteen[3], side); // Fade out with green color
      delay(1000);                                                             // Wait for a second
      break;
    case 2:
      // red
      fadeIn(strip1.Color(127, 0, 0), numberOne[0], numberNineteen[3], side);  // Fade in with red color
      fadeIn(strip2.Color(127, 0, 0), numberOne[0], numberNineteen[3], side);  // Fade in with red color
      delay(1000);                                                             // Wait for a second
      fadeOut(strip1.Color(127, 0, 0), numberOne[0], numberNineteen[3], side); // Fade out with red color
      fadeOut(strip2.Color(127, 0, 0), numberOne[0], numberNineteen[3], side); // Fade out with red color
      delay(1000);                                                             // Wait for a second
      break;
    case 3:
      // yellow
      fadeIn(strip1.Color(100, 100, 100), numberOne[0], numberNineteen[3], side);  // Fade out with yellow color
      fadeIn(strip2.Color(100, 100, 100), numberOne[0], numberNineteen[3], side);  // Fade out with yellow color
      delay(1000);                                                               // Wait for a second
      fadeOut(strip1.Color(100, 100, 100), numberOne[0], numberNineteen[3], side); // Fade out with yellow color
      fadeOut(strip2.Color(100, 100, 100), numberOne[0], numberNineteen[3], side); // Fade out with yellow color
      delay(1000);                                                               // Wait for a second
      break;
    default:
      break;
    }
  }
  else if (side == 1)
  {
    switch (color)
    {
    case 1:
      // green
      fadeIn(strip1.Color(0, 127, 0), numberOne[0], numberNineteen[3], side);  // Fade in with green color
      delay(1000);                                                             // Wait for a second
      fadeOut(strip1.Color(0, 127, 0), numberOne[0], numberNineteen[3], side); // Fade out with green color
      delay(1000);                                                             // Wait for a second
      break;
    case 2:
      // red
      fadeIn(strip1.Color(127, 0, 0), numberOne[0], numberNineteen[3], side);  // Fade in with red color
      delay(1000);                                                             // Wait for a second
      fadeOut(strip1.Color(127, 0, 0), numberOne[0], numberNineteen[3], side); // Fade out with red color
      delay(1000);                                                             // Wait for a second
      break;
    case 3:
      // yellow
      fadeIn(strip1.Color(100, 100, 100), numberOne[0], numberNineteen[3], side);  // Fade out with yellow color
      delay(1000);                                                               // Wait for a second
      fadeOut(strip1.Color(100, 100, 100), numberOne[0], numberNineteen[3], side); // Fade out with yellow color
      delay(1000);                                                               // Wait for a second
      break;
    default:
      break;
    }
  }
  else if (side == 2)
  {
    switch (color)
    {
    case 1:
      // green
      fadeIn(strip2.Color(0, 127, 0), numberOne[0], numberNineteen[3], side);  // Fade in with green color
      delay(1000);                                                             // Wait for a second
      fadeOut(strip2.Color(0, 127, 0), numberOne[0], numberNineteen[3], side); // Fade out with green color
      delay(1000);                                                             // Wait for a second
      break;
    case 2:
      // red
      fadeIn(strip2.Color(127, 0, 0), numberOne[0], numberNineteen[3], side);  // Fade in with red color
      delay(1000);                                                             // Wait for a second
      fadeOut(strip2.Color(127, 0, 0), numberOne[0], numberNineteen[3], side); // Fade out with red color
      delay(1000);                                                             // Wait for a second
      break;
    case 3:
      // yellow
      fadeIn(strip2.Color(100, 100, 100), numberOne[0], numberNineteen[3], side);  // Fade out with yellow color
      delay(1000);                                                               // Wait for a second
      fadeOut(strip2.Color(100, 100, 100), numberOne[0], numberNineteen[3], side); // Fade out with yellow color
      delay(1000);                                                               // Wait for a second
      break;
    default:
      break;
    }
  }
}

void fadeIn(uint32_t color, int startLed, int endLed, int side)
{
  for (int brightness = 0; brightness <= 127; brightness++)
  {
    setBrightness(color, brightness, startLed, endLed, side);
    delay(10); // Delay for smooth fading effect
  }
}

void fadeOut(uint32_t color, int startLed, int endLed, int side)
{
  for (int brightness = 127; brightness >= 0; brightness--)
  {
    setBrightness(color, brightness, startLed, endLed, side);
    delay(10); // Delay for smooth fading effect
  }
}

void setBrightness(uint32_t color, int brightness, int startLed, int endLed, int side)
{
  if (side == 0){
    for (int i = startLed; i <= endLed; i++)
    {
      strip1.setBrightness(brightness);
      strip1.setPixelColor(i, color); // Set the specified color
      strip2.setBrightness(brightness);
      strip2.setPixelColor(i, color); // Set the specified color
    }
    strip1.show(); // Update the LED strip with the new brightness and colors
    strip2.show(); // Update the LED strip with the new brightness and colors
  }
  else if (side == 1)
  {
    for (int i = startLed; i <= endLed; i++)
    {
      strip1.setBrightness(brightness);
      strip1.setPixelColor(i, color); // Set the specified color
    }
    strip1.show(); // Update the LED strip with the new brightness and colors
  }
  else if (side == 2)
  {
    for (int i = startLed; i <= endLed; i++)
    {
      strip2.setBrightness(brightness);
      strip2.setPixelColor(i, color); // Set the specified color
    }
    strip2.show(); // Update the LED strip with the new brightness and colors
  }
  else
  {
    Serial.println("Error setBrightness");
  }
}

void wait(int waitMillis)
{
  unsigned long startTime = millis();
  while (millis() - startTime < waitMillis)
  {
    // Wait
  }
}

void turnOnLeds(int button, int color)
{
  // Code to turn on LEDs for given button
  Serial.println("Leds turned on");
  uint32_t ledColor1 = strip1.Color(0, 127, 0); // Default color: Green
  uint32_t ledColor2 = strip2.Color(0, 127, 0); // Default color: Green

  if (color == 2) {
    ledColor1 = strip1.Color(127, 0, 0); // Color: Red
    ledColor2 = strip2.Color(127, 0, 0); // Color: Red
  } else if (color == 3) {
    ledColor1 = strip1.Color(100, 100, 100); // Color: Yellow
    ledColor2 = strip2.Color(100, 100, 100); // Color: Yellow
  }

    switch (button)
    {
    case 0:
      for (int i = numberOne[0]; i < numberFive[3]; i++)
      {
        strip1.setPixelColor(i, ledColor1); 
      }
      for (int i = 0; i < sizeof(numberEleven) / sizeof(numberEleven[0]); i++)
      {
        strip1.setPixelColor(numberEleven[i], ledColor1); 
      }
      strip1.show();
      break;
    case 1:
      for (int i = numberFive[0]; i < numberTen[3]; i++)
      {
        strip1.setPixelColor(i, ledColor1);
      }
      strip1.show();
      break;
    case 2:
      for (int i = numberEleven[0]; i < numberFifteen[3]; i++)
      {
        strip1.setPixelColor(i, ledColor1); 
      }
      for (int i = 0; i < sizeof(numberSix) / sizeof(numberSix[0]); i++)
      {
        strip1.setPixelColor(numberSix[i], ledColor1);
      }
      strip1.show();
      break;
    case 3:
      for (int i = numberFifteen[0]; i < numberNineteen[3]; i++)
      {
        strip1.setPixelColor(i, ledColor1); 
      }
      for (int i = 0; i < sizeof(numberSeven) / sizeof(numberSeven[0]); i++)
      {
        strip1.setPixelColor(numberSix[i], ledColor1); 
      }
      strip1.show();
      break;
    case 4:
      for (int i = numberOne[0]; i < numberFive[3]; i++)
      {
        strip2.setPixelColor(i, ledColor2);
      }
      for (int i = 0; i < sizeof(numberEleven) / sizeof(numberEleven[0]); i++)
      {
        strip2.setPixelColor(numberEleven[i], ledColor2); 
      }
      strip2.show();
      break;
    case 5:
      for (int i = numberFive[0]; i < numberTen[3]; i++)
      {
        strip2.setPixelColor(i, ledColor2); 
      }
      strip2.show();
      break;
    case 6:
      for (int i = numberEleven[0]; i < numberFifteen[3]; i++)
      {
        strip2.setPixelColor(i, ledColor2);
      }
      for (int i = 0; i < sizeof(numberSix) / sizeof(numberSix[0]); i++)
      {
        strip2.setPixelColor(numberSix[i], ledColor2); 
      }
      strip2.show();
      break;
    case 7:
      for (int i = numberFifteen[0]; i < numberNineteen[3]; i++)
      {
        strip2.setPixelColor(i, ledColor2);
      }
      for (int i = 0; i < sizeof(numberSeven) / sizeof(numberSeven[0]); i++)
      {
        strip2.setPixelColor(numberSix[i], ledColor2);
      }
      strip2.show();
      break;
    default:
      Serial.println("Error turning on Leds");
      break;
    }
  }
  

void turnOffLeds()
{
  // Code om alle LEDs uit te zetten
  //Serial.println("Leds off");
  Serial.println("Button off");
  for (int i = numberOne[0]; i < numberNineteen[3]; i++)
  {
    strip1.setPixelColor(i, strip1.Color(0, 0, 0)); // RGB
    strip2.setPixelColor(i, strip2.Color(0, 0, 0)); // RGB
    strip1.show();                                  // Send the updated pixel colors to the hardware.
    strip2.show();                                  // Send the updated pixel colors to the hardware.
  }
}