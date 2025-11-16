int ssdPins[7] = {1, 2, 3, 4, 5, 6, 7}; 
int rgbPins[3] = {8, 9, 10};
int ledPins[3] = {11, 12, 13};

int buttonPins[6] = {A0, A1, A2, A3, A4, A5};


const int maxPatterns = 9;
int numPatterns = 3;  //default
int lives = 3;

int pattern[20];       // store RGB pattern
int patternIndex = 0;
int playerIndex = 0;
int speed = 700;     

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

void displaySSD(int num) {
  for(int i=0;i<7;i++){
    digitalWrite(ssdPins[i], (digits[num] >> i) & 0x01);
  }
}

void showRGB(int color) {
  for(int i=0;i<3;i++){
    digitalWrite(rgbPins[i], i==color ? HIGH : LOW);
  }
}

void clearRGB() {
  for(int i=0;i<3;i++) digitalWrite(rgbPins[i], LOW);
}

void generatePattern(int n) {
  for(int i=0;i<n;i++){
    pattern[i] = random(0,3); // 0=Red,1=Green,2=Blue
  }
}

void playPattern(int n) {
  for(int i=0;i<n;i++){
    showRGB(pattern[i]);
    delay(speed);
    clearRGB();
    delay(200);
  }
}

// ------------------ Setup ------------------
void setup() {
  // Initialize LEDs
  for(int i=0;i<3;i++) pinMode(ledPins[i], OUTPUT);
  for(int i=0;i<3;i++) pinMode(rgbPins[i], OUTPUT);
  for(int i=0;i<7;i++) pinMode(ssdPins[i], OUTPUT);
  
  // Initialize buttons
  for(int i=0;i<6;i++) pinMode(buttonPins[i], INPUT_PULLUP);

  randomSeed(analogRead(0));
  displaySSD(numPatterns);
}

// ------------------ Main Loop ------------------
void loop() {
  // Increment/decrement number of patterns
  if(digitalRead(buttonPins[4])==LOW){ // Inc
    numPatterns++;
    if(numPatterns>maxPatterns) numPatterns=maxPatterns;
    displaySSD(numPatterns);
    delay(200);
  }
  if(digitalRead(buttonPins[5])==LOW){ // Dec
    numPatterns--;
    if(numPatterns<1) numPatterns=1;
    displaySSD(numPatterns);
    delay(200);
  }

  // Start game
  if(digitalRead(buttonPins[3])==LOW){ // Start button
    generatePattern(numPatterns);
    playPattern(numPatterns);

    lives = 3;
    displaySSD(lives);
    playerIndex = 0;
    speed = 700;

    while(lives>0){
      int playerInput = -1;
      if(digitalRead(buttonPins[0])==LOW) playerInput=0; // Red
      if(digitalRead(buttonPins[1])==LOW) playerInput=1; // Green
      if(digitalRead(buttonPins[2])==LOW) playerInput=2; // Blue

      if(playerInput != -1){
        showRGB(playerInput);
        delay(300);
        clearRGB();

        if(playerInput == pattern[playerIndex]){
          playerIndex++;
          if(playerIndex >= numPatterns){
            // Player completed pattern
            speed = max(200, speed-100); // increase difficulty
            generatePattern(numPatterns);
            playPattern(numPatterns);
            playerIndex = 0;
          }
        } else {
          lives--;
          displaySSD(lives);
          playerIndex = 0;
          if(lives <= 0){
            delay(1000);
            displaySSD(numPatterns); // reset to selection
          }
        }
        delay(200); // debounce
      }
    }
  }
}
