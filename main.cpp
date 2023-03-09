//TODO - Add high-scores

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

using namespace std;

class Word {
private:
    static int RandomNumber() {
        //Select seed from current time and generate random number
        srand(time(nullptr));
        int randomNum = rand() % 7776;
        return randomNum;
    }

public:
    static string WordSelect() {
        //Select random number
        int randomNum = RandomNumber();
        //Open file
        ifstream file("wordlist.txt");
        if (!file.is_open()) {
            cout << "Error opening file" << endl;
            exit(1);
        }
        //Read file
        int n;
        file >> n;
        string word[n];
        for (int i = 0; i < n; i++) {
            file >> word[i];
        }
        //Select random word from random number
        string wordSelected = word[randomNum];
        //cout << wordSelected << endl;
        return wordSelected;

    }

    static void WordSymbols() {
        // Draw _ for each letter in word
        WordSelect();
        int wordLength = WordSelect().length();
        cout << "Word to guess:" << endl;
        for (int i = 0; i < wordLength; i++) {
            cout << "_ ";
        }
        cout << endl;
    }
};


static int DifficultySelect() {
    //Select difficulty. Difficulty determines number of guesses
    int difficulty;
    int guesses;
    cout << "Select difficulty: " << endl;
    cout << "1. Easy" << endl;
    cout << "2. Medium" << endl;
    cout << "3. Hard" << endl;
    cout << "4. Insane" << endl;
    cout << "5. Impossible" << endl;
    cin >> difficulty;
    //check if input is valid
    if (cin.fail()) {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "Invalid input" << endl;
        return 0;
    }
    switch (difficulty) {
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


class Game {
public:
    int guesses = 0;
    int wordLength = 0;
    vector<char> guessedLetters;

    void GameStart() {
        string word;
        guesses = DifficultySelect();
        word = Word::WordSelect();
        wordLength = word.length();
        guessedLetters.clear();
        int correctLetters = 0;
        Word::WordSymbols();

        while (guesses > 0) {
            cout << "Guess a letter: ";
            char letter;
            cin >> letter;

            // Check if input is valid
            if (cin.fail()) {
                cin.clear();
                cin.ignore(256, '\n');
                cout << "Invalid input" << endl;
                continue;
            }

            // Check if letter has already been guessed
            if (find(guessedLetters.begin(), guessedLetters.end(), letter) != guessedLetters.end()) {
                cout << "You already guessed that letter" << endl;
                continue;
            }
            guessedLetters.push_back(letter);

            bool foundLetter = false;

            // Replace _ with letter or show already guessed letters
            for (int i = 0; i < wordLength; i++) {
                if (word[i] == letter) {
                    cout << letter << " ";
                    correctLetters++;
                    foundLetter = true;
                } else {
                    bool guessed = false;
                    for (char c: guessedLetters) {
                        if (c == word[i]) {
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

            // Check if letter is in word
            if (foundLetter) {
                cout << "Correct!" << endl;
                if (correctLetters == wordLength) {
                    cout << "You won!" << endl;
                    break;
                }
            } else {
                cout << "Wrong!" << endl;
                guesses--;
            }

            cout << "Guesses left: " << guesses << endl;
        }

        if (guesses == 0) {
            cout << "GAME OVER" << endl;
            cout << "The word was: " << word << endl;
            cout << "You guessed " << correctLetters << " letters correctly" << endl;
        }

        cout << "Play again? (y/n)" << endl;
        char playAgain;
        cin >> playAgain;
        //check if input is valid
        if (cin.fail()) {
            cin.clear();
            cin.ignore(256, '\n');
            cout << "Invalid input" << endl;
            return;
        }
        if (playAgain == 'y') {
            GameStart();
        } else {
            cout << "Returning to menu" << endl;
        }
    }
};

class HighScores {
public:

};

void Menu() {
    cout << "Hangman Game" << endl;
    cout << "Select an option: " << endl;
    cout << "1. Play" << endl;
    cout << "2. High-scores" << endl;
    cout << "3. Exit" << endl;
}


int main() {
    int choice = 0;
    while (choice != 3) {
        Menu();
        cin >> choice;
        //check if input is valid
        if (cin.fail()) {
            cin.clear();
            cin.ignore(256, '\n');
            cout << "Invalid input" << endl;
            continue;
        }
        Game game;
        switch (choice) {
            case 1:
                cout << "Starting game" << endl;
                game.GameStart();
                break;
            case 2:
                cout << "High-scores" << endl;
                break;
            case 3:
                cout << "Exiting program" << endl;
                break;
            default:
                cout << "Invalid input" << endl;
                break;
        }
    }
    return 0;

}