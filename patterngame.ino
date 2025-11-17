int ssdPins[7] = {1, 2, 3, 4, 5, 6, 7}; 
int rgbPins[3] = {8, 9, 10};       
int ledPins[3] = {11, 12, 13};   

int buttonPins[6] = {A0, A1, A2, A3, A4, A5};
// buttons:
// A0 = button 1 (red)
// A1 = button 2 (green)
// A2 = button 3 (blue)
// A3 = start button
// A4 = increment numPatterns
// A5 = decrement numPatterns

const int minPatterns = 3;
const int maxPatterns = 9;
int numPatterns = 3;

int pattern[20];
int speed;

const byte digits[10] = {
  0b00111111, //0
  0b00000110, //1
  0b01011011, //2
  0b01001111, //3
  0b01100110, //4
  0b01101101, //5
  0b01111101, //6
  0b00000111, //7
  0b01111111, //8
  0b01101111  //9
};

void setup() {
  for (int i=0; i<3; i++) pinMode(ledPins[i], OUTPUT);
  for (int i=0; i<3; i++) pinMode(rgbPins[i], OUTPUT);
  for (int i=0; i<7; i++) pinMode(ssdPins[i], OUTPUT);

  for (int i=0; i<6; i++) pinMode(buttonPins[i], INPUT_PULLUP);

  randomSeed(analogRead(0));
  displaySSD(numPatterns);
}

void loop() {

  if(digitalRead(buttonPins[4]) == LOW){
    if(numPatterns < maxPatterns) numPatterns++;
    displaySSD(numPatterns);
    delay(200);
  }

  if(digitalRead(buttonPins[5]) == LOW){
    if(numPatterns > minPatterns) numPatterns--;
    displaySSD(numPatterns);
    delay(200);
  }

  // start game
  if(digitalRead(buttonPins[3]) == LOW){
    
    // reset game settings
    int lives = 3;
    int playerIndex = 0;
    speed = 700; 
    displaySSD(lives);
    delay(200);

    generatePattern(numPatterns);
    playPattern(numPatterns);

    while(lives > 0){

      int playerInput = getPlayerInput();

      if(playerInput != -1){

        showPlayerLED(playerInput); 
        delay(250);
        clearPlayerLEDs();

        if(playerInput == pattern[playerIndex]){
          playerIndex++;

          // completed the whole pattern
          if(playerIndex >= numPatterns){
            speed = max(200, speed - 100);  // harder next round
            generatePattern(numPatterns);
            playPattern(numPatterns);
            playerIndex = 0;
          }

        } else {
          // wrong press
          lives--;
          displaySSD(lives);
          playerIndex = 0;

          if(lives <= 0){
            delay(1000);
            displaySSD(numPatterns); // back to selection screen
          }
        }

        delay(200);
      }
    }
  }
}

void displaySSD(int num) {
  byte seg = digits[num];
  for(int i=0; i<7; i++){
    digitalWrite(ssdPins[i], (seg & (1 << i)) ? HIGH : LOW);
  }
}

void showRGB(int color) {
  for(int i=0; i<3; i++){
    digitalWrite(rgbPins[i], i==color ? HIGH : LOW);
  }
}

void clearRGB() {
  for(int i=0; i<3; i++) digitalWrite(rgbPins[i], LOW);
}

void showPlayerLED(int color){
  for(int i=0; i<3; i++){
    digitalWrite(ledPins[i], i==color ? HIGH : LOW);
  }
}

void clearPlayerLEDs(){
  for (int i=0; i<3; i++) digitalWrite(ledPins[i], LOW);
}

void generatePattern(int n) {
  for (int i=0; i<n; i++){
    pattern[i] = random(3);
  }
}

void playPattern(int n) {
  for (int i=0; i<n; i++){
    showRGB(pattern[i]);
    delay(speed);
    clearRGB();
    delay(200);
  }
}

int getPlayerInput() {
  for (int i = 0; i < 3; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      delay(20);  // debounce
      while (digitalRead(buttonPins[i]) == LOW); // wait until release
      return i;
    }
  }
  return -1;
}
