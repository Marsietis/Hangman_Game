#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

void CheckInput() {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input" << endl;
        exit(1);
    }
}

static void MainMenu() {
    cout << "Main Menu:" << endl;
    cout << "1. Play" << endl;
    cout << "2. Scores" << endl;
    cout << "3. Quit" << endl;
}

class Word {
private:
    static int RandomNumber() {
        srand(time(nullptr));
        return rand() % 7776;
    }

public:
    static string WordSelect() {
        int randomNum = RandomNumber();
        ifstream file("wordlist.txt");
        if (!file.is_open()) {
            cout << "Error opening file" << endl;
            exit(1);
        }
        int n;
        file >> n;
        string word[n];
        for (int i = 0; i < n; i++) {
            file >> word[i];
        }
        return word[randomNum];
    }

    // Prints underscores for each letter of the selected word
    static void DrawWord() {
        int wordLength = WordSelect().length();
        for (int i = 0; i < wordLength; i++) {
            cout << "_ ";
        }
    }
};

class Difficulty {
public:


    static void difficultyMenu() {
        cout << "Select difficulty: " << endl;
        cout << "1. Easy" << endl;
        cout << "2. Medium" << endl;
        cout << "3. Hard" << endl;
        cout << "4. Insane" << endl;
        cout << "5. Impossible" << endl;
    }

    static int DifficultySelect() {
        int difficultyChoice;
        int guesses;
        //Select difficulty. Difficulty determines number of guesses

        cin >> difficultyChoice;
        CheckInput();

        switch (difficultyChoice) {
            case 1:
                cout << "Easy" << endl;
                guesses = 10;
                return guesses;
            case 2:
                cout << "Medium" << endl;
                guesses = 7;
                return guesses;
            case 3:
                cout << "Hard" << endl;
                guesses = 5;
                return guesses;
            case 4:
                cout << "Insane" << endl;
                guesses = 3;
                return guesses;
            case 5:
                cout << "Impossible" << endl;
                guesses = 1;
                return guesses;
            case 6:
                //Easter egg for testing
                cout << "Instant fail" << endl;
                guesses = 0;
                return guesses;
            default:
                cout << "Invalid input" << endl;
                return 0;
        }
    }
};

class Game {
private:
    vector<char> guessedLetters;
    string wordToGuess = Word::WordSelect();
    int wordLength = wordToGuess.length();
    char letter{};
    int guesses{};
    bool foundLetter{};
    int correctLetters = 0;

public:

    void GameInitialize() {
        cout << wordToGuess << endl;
        cout << "Welcome to Hangman!" << endl;
        Difficulty::difficultyMenu();
        guesses = Difficulty::DifficultySelect();
        cout << "The word to guess is: " << endl;
        Word::DrawWord();
        cout << endl;
        GameLoop();
    }

    void GameLoop() {
        while (guesses > 0) {
            EnterLetter();
            CheckIfGuessed();
            ReplaceUnderscore();
            CheckLetter();
        }
        GameOver();
        PlayAgain();
    }

    void GameOver() {
        cout << "The word was: " << wordToGuess << endl;
        cout << "You guessed " << correctLetters << " letters correctly" << endl;
    }

    void EnterLetter() {
        cout << "Enter a letter: ";
        cin >> letter;
        CheckInput();
        guessedLetters.push_back(letter);
    }

    void CheckIfGuessed() {
        if (find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end()) {
            cout << "You already guessed that letter" << endl;
        }
        guessedLetters.push_back(letter);

        foundLetter = false;
    }

    void CheckLetter() {
        if (foundLetter) {
            cout << "Correct!" << endl;
            if (correctLetters == wordLength) {
                cout << "You won!" << endl;
            }
        } else {
            cout << "Wrong!" << endl;
            guesses--;
        }

        cout << "Guesses left: " << guesses << endl;
    }

    void ReplaceUnderscore() {
        for (int i = 0; i < wordLength; i++) {
            if (wordToGuess[i] == letter) {
                cout << letter << " ";
                correctLetters++;
                foundLetter = true;
            } else {
                bool guessed = false;
                for (char c: guessedLetters) {
                    if (c == wordToGuess[i]) {
                        cout << c << " ";
                        guessed = true;
                        break;
                    }
                }
                if (!guessed) {
                    cout << "_ ";
                }
            }
        }
        cout << endl;
    }

    void PlayAgain() {
        char playAgain;
        cout << "Play again? (y/n): ";
        cin >> playAgain;
        CheckInput();
        if (playAgain == 'y') {
            GameInitialize();
        } else {
            cout << "Thanks for playing!" << endl;
            exit(0);
        }
    }
};

int main() {
    int choice = 0;
    Game game;
    do {
        MainMenu();
        cin >> choice;
        CheckInput();
        switch (choice) {
            case 1:
                game.GameInitialize();
                break;
            case 2:
                cout << "Scores" << endl;
                break;
            case 3:
                cout << "Thanks for playing!" << endl;
                exit(0);
            default:
                cout << "Invalid input" << endl;
                break;
        }
    } while (choice != 3);
}