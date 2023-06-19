#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// GPIO touch pins: 0, 2, 4, 12, 13, 14, 15, 27, 32, 33
// 0 and 2 don't work -> reservated on this ESP

// Ledparts
const int numberOne[] = { 0, 1, 2, 3 };
const int numberTwo[] = { 4, 5, 6, 7 };
const int numberThree[] = { 8, 9, 10, 11 };
const int numberFour[] = { 12, 13, 14, 15 };
const int numberFive[] = { 16, 17, 18, 19 };
const int numberSix[] = { 20, 21, 22, 23 };
const int numberSeven[] = { 24, 25, 26, 27 };
const int numberEight[] = { 28, 29, 30, 31 };
const int numberNine[] = { 32, 33, 34, 35 };
const int numberTen[] = { 36, 37, 38, 39 };
const int numberEleven[] = { 40, 41, 42, 43 };
const int numberTwelve[] = { 44, 45, 46, 47 };
const int numberThirtheen[] = { 48, 49, 50, 51 };
const int numberFourteen[] = { 52, 53, 54, 55 };
const int numberFifteen[] = { 56, 57, 58, 59 };
const int numberSixteen[] = { 60, 61, 62, 63 };
const int numberSeventeen[] = { 64, 65, 66, 67 };
const int numberEighteen[] = { 68, 69, 70, 71 };
const int numberNineteen[] = { 72, 73, 74, 75 };

// Constants
#define ATTEMPTS 7
#define LED_BRIGHTNESS 200
#define THRESHOLD 15
#define LED_PIN1 16
#define LED_PIN2 17
#define LED_AMOUNT1 77  // 76 leds
#define LED_AMOUNT2 77  // 76 leds

const int TOUCH_PINS1[] = { 4, 12, 13, 14 };
const int TOUCH_PINS2[] = { 27, 32, 33 };  //15 not working

Adafruit_NeoPixel strip1(LED_AMOUNT1, LED_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_AMOUNT2, LED_PIN2, NEO_GRB + NEO_KHZ800);

// Variables
int player1 = 1, player2 = 2;
int green = 1, red = 2, yellow = 3;
int startButton = 4;        // Used to let it blink
int lastButtonChosen = -1;  // Variable to store the previously chosen button

// Function prototypes
int startGame(int player);  //Main function to start the game for given player
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
void ledOff(const int* numbers, int size);
void ledOn(int player, const int* numbers, int size, int color);
void wait(int waitMillis);
void blinkStartLed(int color);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  delay(1000);  // Delay to launch the serial monitor
  randomSeed(analogRead(0));
  strip1.begin();
  strip2.begin();
  strip1.clear();  // Set all pixel colors to 'off'
  strip2.clear();  // Set all pixel colors to 'off'
  Serial.println("Setup complete");
}

void loop() {
  strip1.clear();  // Set all pixel colors to 'off'
  strip2.clear();  // Set all pixel colors to 'off'
  strip1.setBrightness(LED_BRIGHTNESS);
  strip2.setBrightness(LED_BRIGHTNESS);
  int startValue;
  Serial.println("Game starts with holding startbutton");
  do {
    //blinkLeds(green, player1);
    //blinkLeds(red, player2);
    strip1.clear();  // Set all pixel colors to 'off'
    strip2.clear();  // Set all pixel colors to 'off'
    strip1.setBrightness(LED_BRIGHTNESS);
    strip2.setBrightness(LED_BRIGHTNESS);
    blinkStartLed(yellow);
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

int startGame(int player) {
  int reactionTime = 0;
  int buttonPlayer = 0;
  Serial.print("Starting now!");
  delay(3000);

  if (player == 1) {
    Serial.println("Player 1 turn");
    //delay(1000);
  } else if (player == 2) {
    Serial.println("Player 2 turn");
    // delay(1000);
  } else {
    Serial.println("Error, no valid player selected");
  }

  for (int a = 0; a < ATTEMPTS; a++) {
    Serial.print("Touch button: ");
    buttonPlayer = randomButton(player);
    Serial.println(buttonPlayer);
    activateButton(player, buttonPlayer);
    //turnOnLeds(1, green);
    int timePerRound = waitForTouch(buttonPlayer);
    turnOffLeds();
    reactionTime += timePerRound;
    printTime(timePerRound, reactionTime);
    int waitTime = random(1000, 3501);
    delay(waitTime);
  }

  return reactionTime;
}

int randomButton(int player) {
  int randomIndex;
  int buttonChosen = 0;
  do {
    if (player == 1) {
      randomIndex = random(0, 4);
      buttonChosen = TOUCH_PINS1[randomIndex];
    } else if (player == 2) {
      randomIndex = random(0, 3);
      buttonChosen = TOUCH_PINS2[randomIndex];
    } else {
      Serial.println("ERROR, no chosen player");
    }
  } while (buttonChosen == lastButtonChosen);  // Keep choosing a button until it's different from the last one

  lastButtonChosen = buttonChosen;  // Update the last chosen button
  return buttonChosen;
}

void activateButton(int player, int buttonPlayer) {
  int buttonNumber;
  int buttonToLedMapping[] = { -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, 1, 2, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, -1, -1, -1, -1, 6, 7 };
  //                           0   1   2   3  4   5   6   7   8   9   10 11 12  13 14 15  16  17  18  19  20  21  22  23  24  25 26 27   28  29  30  31 32 33

  buttonNumber = buttonToLedMapping[buttonPlayer];
  Serial.print("buttonNumber = ");
  Serial.println(buttonNumber);
  if (buttonNumber == -1) {
    Serial.println("Error, activatebutton2");
  } else {
    int randomColor = random(1, 4);
    turnOnLeds(buttonNumber, randomColor);
  }
  Serial.print("Value of touchbutton: ");
  Serial.println(touchRead(buttonNumber));
  Serial.println("Button activated");
}

int waitForTouch(int button) {
  int capValue;
  unsigned long currentMillis, nextMillis;

  currentMillis = millis();
  capValue = touchRead(button);
  Serial.print("Value of touchbutton: ");
  Serial.println(capValue);
  do {
    capValue = touchRead(button);
  } while (capValue > THRESHOLD);
  Serial.print("Value of touchbutton AFTER: ");
  Serial.println(capValue);
  nextMillis = millis();
  Serial.println("Touched, time captured");
  return nextMillis - currentMillis;
}

void printTime(int timePerRound, int totalTime) {
  unsigned long secondsRound = timePerRound / 1000;       // Convert milliseconds to seconds
  unsigned long millisecondsRound = timePerRound % 1000;  // Extract milliseconds
  unsigned long secondsTotal = totalTime / 1000;          // Convert milliseconds to seconds
  unsigned long millisecondsTotal = totalTime % 1000;     // Extract milliseconds
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

int determineWinner(int timePlayer1, int timePlayer2) {
  Serial.println("Winner determined");
  if (timePlayer1 < timePlayer2) {
    return 1;
  }
  if (timePlayer1 > timePlayer2) {
    return 2;
  }
  return 0;
}
//
void showWinner(int winner) {
  strip1.clear();  // Set all pixel colors to 'off'
  strip2.clear();  // Set all pixel colors to 'off'
  strip1.setBrightness(LED_BRIGHTNESS);
  strip2.setBrightness(LED_BRIGHTNESS);
  if (winner == 1) {
    Serial.println("Player 1 wins!");
    turnOnLeds(9, red);
    blinkLeds(green, 1);  // Green blinking leds for winner
    //blinkLeds(red, 2); //Red blinking leds for loser
  } else if (winner == 2) {
    Serial.println("Player 2 wins!");
    turnOnLeds(8, red);
    blinkLeds(green, 2);  // Green blinking leds for winner
    //blinkLeds(red, 1); //Red blinking leds for loser
  } else {
    Serial.println("It's a tie");
    // Leds bij beide spelers groen knipperen
    blinkLeds(yellow, player1);
    blinkLeds(yellow, player2);
    blinkLeds(yellow, player1);
    blinkLeds(yellow, player2);
  }
  Serial.println("Game over.");
  strip1.clear();  // Set all pixel colors to 'off'
  strip2.clear();  // Set all pixel colors to 'off'
  strip1.show();
  strip2.show();
}

void blinkLeds(int color, int side) {
  Serial.println("Blinking leds");

  if (side == 1) {
    switch (color) {
      case 1:
        // green
        fadeIn(strip1.Color(0, LED_BRIGHTNESS, 0), numberOne[0], numberNineteen[3], side);   // Fade in with green color
        delay(2500);                                                                         // Wait for a second
        fadeOut(strip1.Color(0, LED_BRIGHTNESS, 0), numberOne[0], numberNineteen[3], side);  // Fade out with green color
        delay(2500);                                                                         // Wait for a second
        break;
      case 2:
        // red
        fadeIn(strip1.Color(LED_BRIGHTNESS, 0, 0), numberOne[0], numberNineteen[3], side);   // Fade in with red color
        delay(2500);                                                                         // Wait for a second
        fadeOut(strip1.Color(LED_BRIGHTNESS, 0, 0), numberOne[0], numberNineteen[3], side);  // Fade out with red color
        delay(1000);                                                                         // Wait for a second
        break;
      case 3:
        // yellow
        fadeIn(strip1.Color(LED_BRIGHTNESS, LED_BRIGHTNESS, LED_BRIGHTNESS), numberOne[0], numberNineteen[3], side);   // Fade out with yellow color
        delay(500);                                                                                                    // Wait for a second
        fadeOut(strip1.Color(LED_BRIGHTNESS, LED_BRIGHTNESS, LED_BRIGHTNESS), numberOne[0], numberNineteen[3], side);  // Fade out with yellow color
        delay(500);                                                                                                    // Wait for a second
        break;
      default:
        break;
    }
  } else if (side == 2) {
    switch (color) {
      case 1:
        // green
        fadeIn(strip2.Color(0, LED_BRIGHTNESS, 0), numberOne[0], numberNineteen[3], side);   // Fade in with green color
        delay(2500);                                                                         // Wait for a second
        fadeOut(strip2.Color(0, LED_BRIGHTNESS, 0), numberOne[0], numberNineteen[3], side);  // Fade out with green color
        delay(1000);                                                                         // Wait for a second
        break;
      case 2:
        // red
        fadeIn(strip2.Color(LED_BRIGHTNESS, 0, 0), numberOne[0], numberNineteen[3], side);   // Fade in with red color
        delay(2500);                                                                         // Wait for a second
        fadeOut(strip2.Color(LED_BRIGHTNESS, 0, 0), numberOne[0], numberNineteen[3], side);  // Fade out with red color
        delay(1000);                                                                         // Wait for a second
        break;
      case 3:
        // yellow
        fadeIn(strip2.Color(LED_BRIGHTNESS, LED_BRIGHTNESS, LED_BRIGHTNESS), numberOne[0], numberNineteen[3], side);   // Fade out with yellow color
        delay(500);                                                                                                    // Wait for a second
        fadeOut(strip2.Color(LED_BRIGHTNESS, LED_BRIGHTNESS, LED_BRIGHTNESS), numberOne[0], numberNineteen[3], side);  // Fade out with yellow color
        delay(1000);                                                                                                   // Wait for a second
        break;
      default:
        break;
    }
  }
}

void fadeIn(uint32_t color, int startLed, int endLed, int side) {
  for (int brightness = 0; brightness <= LED_BRIGHTNESS; brightness++) {
    setBrightness(color, brightness, startLed, endLed, side);
    delay(10);  // Delay for smooth fading effect
  }
}

void fadeOut(uint32_t color, int startLed, int endLed, int side) {
  for (int brightness = LED_BRIGHTNESS; brightness >= 0; brightness--) {
    setBrightness(color, brightness, startLed, endLed, side);
    delay(10);  // Delay for smooth fading effect
  }
}

void setBrightness(uint32_t color, int brightness, int startLed, int endLed, int side) {
  if (side == 0) {
    for (int i = startLed; i <= endLed; i++) {
      strip1.setBrightness(brightness);
      strip1.setPixelColor(i, color);  // Set the specified color
      strip2.setBrightness(brightness);
      strip2.setPixelColor(i, color);  // Set the specified color
    }
    strip1.show();  // Update the LED strip with the new brightness and colors
    strip2.show();  // Update the LED strip with the new brightness and colors
  } else if (side == 1) {
    for (int i = startLed; i <= endLed; i++) {
      strip1.setBrightness(brightness);
      strip1.setPixelColor(i, color);  // Set the specified color
    }
    strip1.show();  // Update the LED strip with the new brightness and colors
  } else if (side == 2) {
    for (int i = startLed; i <= endLed; i++) {
      strip2.setBrightness(brightness);
      strip2.setPixelColor(i, color);  // Set the specified color
    }
    strip2.show();  // Update the LED strip with the new brightness and colors
  } else {
    Serial.println("Error setBrightness");
  }
}


void turnOffLeds() {
  // Code om alle LEDs uit te zetten
  //Serial.println("Leds off");
  Serial.println("Button off");
  for (int i = numberOne[0]; i < numberNineteen[3] + 1; i++) {
    strip1.setPixelColor(i, strip1.Color(0, 0, 0));  // RGB
    strip2.setPixelColor(i, strip2.Color(0, 0, 0));  // RGB
  }
  strip1.show();  // Send the updated pixel colors to the hardware.
  strip2.show();  // Send the updated pixel colors to the hardware.
}



void blinkStartLed(int color) {
  ledOn(player1, numberOne, sizeof(numberOne) / sizeof(numberOne[0]), color);
  ledOn(player1, numberTwo, sizeof(numberTwo) / sizeof(numberTwo[0]), color);
  ledOn(player1, numberThree, sizeof(numberThree) / sizeof(numberThree[0]), color);
  ledOn(player1, numberFour, sizeof(numberFour) / sizeof(numberFour[0]), color);
  ledOn(player1, numberFive, sizeof(numberFive) / sizeof(numberFive[0]), color);
  ledOn(player1, numberEleven, sizeof(numberEleven) / sizeof(numberEleven[0]), color);
  strip1.show();  // Update the LED strip

  delay(1000);  // Delay for one second between each blink

  ledOff(numberOne, sizeof(numberOne) / sizeof(numberOne[0]));
  ledOff(numberTwo, sizeof(numberTwo) / sizeof(numberTwo[0]));
  ledOff(numberThree, sizeof(numberThree) / sizeof(numberThree[0]));
  ledOff(numberFour, sizeof(numberFour) / sizeof(numberFour[0]));
  ledOff(numberFive, sizeof(numberFive) / sizeof(numberFive[0]));
  ledOff(numberEleven, sizeof(numberEleven) / sizeof(numberEleven[0]));
  strip1.show();  // Update the LED strip

  delay(1000);
}

void turnOnLeds(int button, int color) {
  strip1.clear();  // Set all pixel colors to 'off'
  strip2.clear();  // Set all pixel colors to 'off'
  strip1.setBrightness(LED_BRIGHTNESS);
  strip2.setBrightness(LED_BRIGHTNESS);
  int player = 0;
  // if (button < 4) {
  //   player = 1;
  // } else {
  //   player = 2;
  // }
  switch (button) {
    case 0:
      {
        ledOn(player1, numberOne, sizeof(numberOne) / sizeof(numberOne[0]), color);
        ledOn(player1, numberTwo, sizeof(numberTwo) / sizeof(numberTwo[0]), color);
        ledOn(player1, numberThree, sizeof(numberThree) / sizeof(numberThree[0]), color);
        ledOn(player1, numberFour, sizeof(numberFour) / sizeof(numberFour[0]), color);
        ledOn(player1, numberFive, sizeof(numberFive) / sizeof(numberFive[0]), color);
        ledOn(player1, numberEleven, sizeof(numberEleven) / sizeof(numberEleven[0]), color);
        strip1.show();  // Update the LED strip
        break;
      }
    case 1:
      {
        ledOn(player1, numberFive, sizeof(numberFive) / sizeof(numberFive[0]), color);
        ledOn(player1, numberSix, sizeof(numberSix) / sizeof(numberSix[0]), color);
        ledOn(player1, numberSeven, sizeof(numberSeven) / sizeof(numberSeven[0]), color);
        ledOn(player1, numberEight, sizeof(numberEight) / sizeof(numberEight[0]), color);
        ledOn(player1, numberNine, sizeof(numberNine) / sizeof(numberNine[0]), color);
        ledOn(player1, numberTen, sizeof(numberTen) / sizeof(numberTen[0]), color);
        strip1.show();  // Update the LED strip
        break;
      }
    case 2:
      {
        ledOn(player1, numberSix, sizeof(numberSix) / sizeof(numberSix[0]), color);
        ledOn(player1, numberEleven, sizeof(numberEleven) / sizeof(numberEleven[0]), color);
        ledOn(player1, numberTwelve, sizeof(numberTwelve) / sizeof(numberTwelve[0]), color);
        ledOn(player1, numberThirtheen, sizeof(numberThirtheen) / sizeof(numberThirtheen[0]), color);
        ledOn(player1, numberFourteen, sizeof(numberFourteen) / sizeof(numberFourteen[0]), color);
        ledOn(player1, numberFifteen, sizeof(numberFifteen) / sizeof(numberFifteen[0]), color);
        strip1.show();  // Update the LED strip
        break;
      }
    case 3:
      {
        ledOn(player1, numberSeven, sizeof(numberSeven) / sizeof(numberSeven[0]), color);
        ledOn(player1, numberFifteen, sizeof(numberFifteen) / sizeof(numberFifteen[0]), color);
        ledOn(player1, numberSixteen, sizeof(numberSixteen) / sizeof(numberSixteen[0]), color);
        ledOn(player1, numberSeventeen, sizeof(numberSeventeen) / sizeof(numberSeventeen[0]), color);
        ledOn(player1, numberEighteen, sizeof(numberEighteen) / sizeof(numberEighteen[0]), color);
        ledOn(player1, numberNineteen, sizeof(numberNineteen) / sizeof(numberNineteen[0]), color);
        strip1.show();  // Update the LED strip
        break;
      }
    case 4:
      {
        ledOn(player2, numberOne, sizeof(numberOne) / sizeof(numberOne[0]), color);
        ledOn(player2, numberTwo, sizeof(numberTwo) / sizeof(numberTwo[0]), color);
        ledOn(player2, numberThree, sizeof(numberThree) / sizeof(numberThree[0]), color);
        ledOn(player2, numberFour, sizeof(numberFour) / sizeof(numberFour[0]), color);
        ledOn(player2, numberFive, sizeof(numberFive) / sizeof(numberFive[0]), color);
        ledOn(player2, numberEleven, sizeof(numberEleven) / sizeof(numberEleven[0]), color);
        strip2.show();  // Update the LED strip
        break;
      }
    case 5:
      {
        ledOn(player2, numberFive, sizeof(numberFive) / sizeof(numberFive[0]), color);
        ledOn(player2, numberSix, sizeof(numberSix) / sizeof(numberSix[0]), color);
        ledOn(player2, numberSeven, sizeof(numberSeven) / sizeof(numberSeven[0]), color);
        ledOn(player2, numberEight, sizeof(numberEight) / sizeof(numberEight[0]), color);
        ledOn(player2, numberNine, sizeof(numberNine) / sizeof(numberNine[0]), color);
        ledOn(player2, numberTen, sizeof(numberTen) / sizeof(numberTen[0]), color);
        strip2.show();  // Update the LED strip
        break;
      }
    case 6:
      {
        ledOn(player2, numberSix, sizeof(numberSix) / sizeof(numberSix[0]), color);
        ledOn(player2, numberEleven, sizeof(numberEleven) / sizeof(numberEleven[0]), color);
        ledOn(player2, numberTwelve, sizeof(numberTwelve) / sizeof(numberTwelve[0]), color);
        ledOn(player2, numberThirtheen, sizeof(numberThirtheen) / sizeof(numberThirtheen[0]), color);
        ledOn(player2, numberFourteen, sizeof(numberFourteen) / sizeof(numberFourteen[0]), color);
        ledOn(player2, numberFifteen, sizeof(numberFifteen) / sizeof(numberFifteen[0]), color);
        strip2.show();  // Update the LED strip
        break;
      }
    case 7:
      {
        ledOn(player2, numberSeven, sizeof(numberSeven) / sizeof(numberSeven[0]), color);
        ledOn(player2, numberFifteen, sizeof(numberFifteen) / sizeof(numberFifteen[0]), color);
        ledOn(player2, numberSixteen, sizeof(numberSixteen) / sizeof(numberSixteen[0]), color);
        ledOn(player2, numberSeventeen, sizeof(numberSeventeen) / sizeof(numberSeventeen[0]), color);
        ledOn(player2, numberEighteen, sizeof(numberEighteen) / sizeof(numberEighteen[0]), color);
        ledOn(player2, numberNineteen, sizeof(numberNineteen) / sizeof(numberNineteen[0]), color);
        strip2.show();  // Update the LED strip
        break;
      }
    case 8:
      {  // Case to turn on all LEDs of strip1
        ledOn(player1, numberOne, sizeof(numberOne) / sizeof(numberOne[0]), color);
        ledOn(player1, numberTwo, sizeof(numberTwo) / sizeof(numberTwo[0]), color);
        ledOn(player1, numberThree, sizeof(numberThree) / sizeof(numberThree[0]), color);
        ledOn(player1, numberFour, sizeof(numberFour) / sizeof(numberFour[0]), color);
        ledOn(player1, numberFive, sizeof(numberFive) / sizeof(numberFive[0]), color);
        ledOn(player1, numberSix, sizeof(numberSix) / sizeof(numberSix[0]), color);
        ledOn(player1, numberSeven, sizeof(numberSeven) / sizeof(numberSeven[0]), color);
        ledOn(player1, numberEight, sizeof(numberEight) / sizeof(numberEight[0]), color);
        ledOn(player1, numberNine, sizeof(numberNine) / sizeof(numberNine[0]), color);
        ledOn(player1, numberTen, sizeof(numberTen) / sizeof(numberTen[0]), color);
        ledOn(player1, numberEleven, sizeof(numberEleven) / sizeof(numberEleven[0]), color);
        ledOn(player1, numberTwelve, sizeof(numberTwelve) / sizeof(numberTwelve[0]), color);
        ledOn(player1, numberThirtheen, sizeof(numberThirtheen) / sizeof(numberThirtheen[0]), color);
        ledOn(player1, numberFourteen, sizeof(numberFourteen) / sizeof(numberFourteen[0]), color);
        ledOn(player1, numberFifteen, sizeof(numberFifteen) / sizeof(numberFifteen[0]), color);
        ledOn(player1, numberSixteen, sizeof(numberSixteen) / sizeof(numberSixteen[0]), color);
        ledOn(player1, numberSeventeen, sizeof(numberSeventeen) / sizeof(numberSeventeen[0]), color);
        ledOn(player1, numberEighteen, sizeof(numberEighteen) / sizeof(numberEighteen[0]), color);
        ledOn(player1, numberNineteen, sizeof(numberNineteen) / sizeof(numberNineteen[0]), color);
        strip1.show();  // Update the LED strip
        break;
      }
    case 9:
      {  // Case to turn on all LEDs of strip2
        ledOn(player2, numberOne, sizeof(numberOne) / sizeof(numberOne[0]), color);
        ledOn(player2, numberTwo, sizeof(numberTwo) / sizeof(numberTwo[0]), color);
        ledOn(player2, numberThree, sizeof(numberThree) / sizeof(numberThree[0]), color);
        ledOn(player2, numberFour, sizeof(numberFour) / sizeof(numberFour[0]), color);
        ledOn(player2, numberFive, sizeof(numberFive) / sizeof(numberFive[0]), color);
        ledOn(player2, numberSix, sizeof(numberSix) / sizeof(numberSix[0]), color);
        ledOn(player2, numberSeven, sizeof(numberSeven) / sizeof(numberSeven[0]), color);
        ledOn(player2, numberEight, sizeof(numberEight) / sizeof(numberEight[0]), color);
        ledOn(player2, numberNine, sizeof(numberNine) / sizeof(numberNine[0]), color);
        ledOn(player2, numberTen, sizeof(numberTen) / sizeof(numberTen[0]), color);
        ledOn(player2, numberEleven, sizeof(numberEleven) / sizeof(numberEleven[0]), color);
        ledOn(player2, numberTwelve, sizeof(numberTwelve) / sizeof(numberTwelve[0]), color);
        ledOn(player2, numberThirtheen, sizeof(numberThirtheen) / sizeof(numberThirtheen[0]), color);
        ledOn(player2, numberFourteen, sizeof(numberFourteen) / sizeof(numberFourteen[0]), color);
        ledOn(player2, numberFifteen, sizeof(numberFifteen) / sizeof(numberFifteen[0]), color);
        ledOn(player2, numberSixteen, sizeof(numberSixteen) / sizeof(numberSixteen[0]), color);
        ledOn(player2, numberSeventeen, sizeof(numberSeventeen) / sizeof(numberSeventeen[0]), color);
        ledOn(player2, numberEighteen, sizeof(numberEighteen) / sizeof(numberEighteen[0]), color);
        ledOn(player2, numberNineteen, sizeof(numberNineteen) / sizeof(numberNineteen[0]), color);
        strip2.show();  // Update the LED strip
        break;
      }
    default:
      break;
  }
}


// Function to blink the LEDs specified in the given array
void ledOn(int player, const int* numbers, int size, int color) {
  // Turn on the LEDs
  if (player == 1) {
    if (color == 1) {
      for (int i = 0; i < size; i++) {
        strip1.setPixelColor(numbers[i], strip1.Color(0, LED_BRIGHTNESS, 0));  // Green
      }
    } else if (color == 2) {
      for (int i = 0; i < size; i++) {
        strip1.setPixelColor(numbers[i], strip1.Color(LED_BRIGHTNESS, 0, 0));  // Red
      }
    } else if (color == 3) {
      for (int i = 0; i < size; i++) {
        strip1.setPixelColor(numbers[i], strip1.Color(LED_BRIGHTNESS, LED_BRIGHTNESS, LED_BRIGHTNESS));  // Yellow
      }
    } else {
      Serial.println("Error, ledOn function");
    }
  } else if (player == 2) {
    if (color == 1) {
      for (int i = 0; i < size; i++) {

        strip2.setPixelColor(numbers[i], strip2.Color(0, LED_BRIGHTNESS, 0));  // Green
      }
    } else if (color == 2) {
      for (int i = 0; i < size; i++) {

        strip2.setPixelColor(numbers[i], strip2.Color(LED_BRIGHTNESS, 0, 0));  // Red
      }
    } else if (color == 3) {
      for (int i = 0; i < size; i++) {

        strip2.setPixelColor(numbers[i], strip2.Color(LED_BRIGHTNESS, LED_BRIGHTNESS, LED_BRIGHTNESS));  // Yellow
      }
    } else {
      Serial.println("Error, ledOn function");
    }
  }
}

void ledOff(const int* numbers, int size) {
  // Turn off the LEDs
  for (int i = 0; i < size; i++) {
    strip1.setPixelColor(numbers[i], strip1.Color(0, 0, 0));  // Set color to off (black)
  }
}


void wait(int waitMillis) {
  unsigned long startTime = millis();
  while (millis() - startTime < waitMillis) {
    // Wait
  }
}
