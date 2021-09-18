#include <Arduboy2.h>

// game
enum class GameState { playing, gameOver, playerWon };
enum class Difficulty { easy = 16, normal = 26, hard = 51 };

class Game {
public:
  void setup();
  void guess();
  void reset();
  void increaseGuess() { guessedNumber += 1; }
  void decreaseGuess() { guessedNumber -= 1; }
  bool justPressed(uint8_t button) { return engine.justPressed(button); }
  void draw();
  GameState gameState;
private:
  Arduboy2 engine;
  uint8_t randomNumber, attempts, lastGuess, guessedNumber; 
  Difficulty difficulty;
};

// implementation
void Game::setup() {
  engine.begin();
  engine.setFrameRate(15);
  engine.clear();
  engine.initRandomSeed();
  difficulty = Difficulty::easy;
  reset();
}

void Game::guess() {
  lastGuess = guessedNumber;
  if (guessedNumber == randomNumber) {
    gameState = GameState::playerWon;
    return;
  }

  attempts += 1;

  if (attempts > 6) { gameState = GameState::gameOver; }
}

void Game::reset() {
  attempts = 0;
  lastGuess = 0;
  guessedNumber = 0;

  if (gameState == GameState::playerWon && difficulty != Difficulty::hard) {
    difficulty = (difficulty == Difficulty::easy) ? Difficulty::normal : Difficulty::hard;
  }

  if (gameState == GameState::gameOver) {
    difficulty = Difficulty::easy;
  }

  randomNumber = random(1, static_cast<int>(difficulty));
  gameState = GameState::playing;
}

void Game::draw() {
  engine.clear();
  engine.pollButtons();
  
  switch (gameState) {
  case GameState::playerWon:
    engine.setCursor(0, 0);
    engine.print("You won!\nCorrect Number: ");
    engine.print(guessedNumber);
    break;
  case GameState::gameOver:
    engine.setCursor(0, 0);
    engine.print("You lost!\nCorrect Number: ");
    engine.print(randomNumber);
    break;
  case GameState::playing:
    engine.setCursor(0, 0);
    engine.print("Attempt: ");
    engine.print(attempts);
    engine.print("\n");
    engine.print("Number to guess: ");
    engine.print(guessedNumber);
    engine.print("\n");

    if (attempts == 0) {
      engine.print("Good luck!");
    } else {
      engine.print(lastGuess);
      if (lastGuess > randomNumber) { engine.print(" is too high!"); }
      if (lastGuess < randomNumber) { engine.print(" is too low!"); }
    }
    break;
  }
  
  engine.display();
}

// global
Game game;

// arduboy
void setup() { game.setup(); }

void loop() {
  switch (game.gameState) {
  case GameState::playerWon:
  case GameState::gameOver:
    if (game.justPressed(B_BUTTON)) { game.reset(); }
    break;
  case GameState::playing:
    if (game.justPressed(B_BUTTON)) { game.guess(); }
    if (game.justPressed(UP_BUTTON)) { game.increaseGuess(); }
    if (game.justPressed(DOWN_BUTTON)) { game.decreaseGuess(); }
  }
  game.draw();
}
