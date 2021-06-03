#include <Vector.h>

// red, yellow, green, blue
const int colors[] = {10, 11, 12, 13};
const int buttons[] = {4, 5, 6, 7}; 
const int startButton = 3;

int highScore = 0; // global high score
int roundScore = 0; // score for individual rounds

Vector<int> sequence; // declare a vector of colors
int arr[50]; // declare maximum size of vector

bool gameInProgress = false; // track game state

void setup() {
  Serial.begin(9600);
  // set pin modes for the LEDs and buttons
  for (int i = 0; i < sizeof(colors) / sizeof(colors[0]); i++) {
    pinMode(colors[i], OUTPUT);
    digitalWrite(colors[i], LOW);
  }
  for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
    pinMode(buttons[i], OUTPUT);
  }
  randomSeed(analogRead(A0)); // set a random seed based on reading from A0
  sequence.setStorage(arr); // set maximum size of vector
  Serial.println("WELCOME TO SIMON SAYS! PRESS THE START BUTTON TO BEGIN.");
}

// function for turning a LED on and off
void blinkLED(int led, int duration) {
  digitalWrite(led, HIGH);
  delay(duration);
  digitalWrite(led, LOW);
  delay(duration);
}

// play the sequence of colors from a vector passed by reference
void playSequence(Vector<int>& seq) {
  Serial.println("PLAYING SEQUENCE... MEMORIZE THIS!");
  for (int i = 0; i < seq.size(); i++) {
    blinkLED(colors[seq.at(i)], 600);
  }
}

void playGame() {
  int answer; // the player's color choice
  bool won = false; // tracks whether or not the player beat the round
  Serial.print("ROUND ");
  Serial.print(roundScore + 1);
  Serial.println(" BEGIN...");
  for (int i = 0; i < sequence.size(); i++) { // iterate through the sequence
    while (true) { // wait for the player's answer and then check it, break the infinite loop once there is an answer
      if (digitalRead(buttons[0]) == HIGH) {
        blinkLED(colors[0], 600);
        answer = colors[0];
        break;
      }
      if (digitalRead(buttons[1]) == HIGH) {
        blinkLED(colors[1], 600);
        answer = colors[1];
        break;
      }
      if (digitalRead(buttons[2]) == HIGH) {
        blinkLED(colors[2], 600);
        answer = colors[2];
        break;
      }
      if (digitalRead(buttons[3]) == HIGH) {
        blinkLED(colors[3], 600);
        answer = colors[3];
        break;
      }
    }
    // check whether or not the answer is correct
    if (answer == colors[sequence.at(i)] && i == sequence.size() - 1) {
      won = true;
      break;
    }
    if (answer != colors[sequence.at(i)]) {
      gameInProgress = false;
      break;
    }
  }
  if (won) { // if the player passed the round, add a new random color to the sequence and increase the score;
    sequence.push_back(random(4));
    roundScore++;
    Serial.println("YOU PASS!");
  } else { // if the player lost, display the current and high scores and clear the vector containing the color sequence
    Serial.println("GAME OVER");
    if (roundScore > highScore) { // check if the player achieved a new high score
      highScore = roundScore;
      Serial.print("NEW HIGH SCORE ACHIEVED: ");
      Serial.println(highScore);
    } else { // otherwise, display the player's score as well as the high score
      Serial.print("YOU SCORED: ");
      Serial.println(roundScore);
      Serial.print("HIGH SCORE: ");
      Serial.println(highScore);
    }
    roundScore = 0;
    sequence.clear();
    gameInProgress = false;
    Serial.println("PRESS THE START BUTTON TO PLAY AGAIN.");
  }
}

void loop() {
  if (digitalRead(startButton) == HIGH && !gameInProgress) { // check to see if the start button is pressed and if a game is already in progress
    gameInProgress = true;
    sequence.push_back(random(4)); // add the first color to the vector
    Serial.print("GAME STARTING IN 3... ");
    delay(1000);
    Serial.print("2... ");
    delay(1000);
    Serial.println("1... ");
    delay(1000);
    playSequence(sequence);
    playGame();
  } else if (gameInProgress) { // if the player passed a round, show the new sequence and let the them choose a color again
    playSequence(sequence);
    playGame(); 
  }
}
