#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <utility>
#include <vector>

using namespace std;

void Title();

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
                //Easter egg
                cout << "Instant fail" << endl;
                guesses = 0;
                return guesses;
            default:
                cout << "Invalid input" << endl;
                return 0;
        }
    }
};

class Score {
private:
    string playerName;
    int playerScore;

public:
    Score() {
        playerName = "";
        playerScore = 0;
    }

    void SetName(string name) {
        playerName = std::move(name);
    }

    void SetScore(int score) {
        playerScore = score;
    }

    [[nodiscard]] string GetName() const {
        return playerName;
    }

    [[nodiscard]] int GetScore() const {
        return playerScore;
    }

    void SaveScore() const {
        ofstream file;
        file.open("highscores.txt", ios::app);
        file << playerName << " " << playerScore << endl;
        file.close();
    }

    static vector<Score> LoadScores() {
        vector<Score> scores;
        ifstream file("highscores.txt");
        if (file.is_open()) {
            string name;
            int score;
            while (file >> name >> score) {
                Score s;
                s.SetName(name);
                s.SetScore(score);
                scores.push_back(s);
            }
            file.close();
        }
        return scores;
    }

    static void DisplayScores() {
        cout << "High Scores:" << endl;
        vector<Score> scores = Score::LoadScores();
        if (scores.empty()) {
            cout << "No scores to display" << endl;
            return;
        }

        // Sort scores in descending order
        sort(scores.begin(), scores.end(), [](const Score &a, const Score &b) {
            return a.GetScore() > b.GetScore();
        });

        for (const Score &s: scores) {
            cout << s.GetName() << ": " << s.GetScore() << endl;
        }
    }


};

class Hangman : public Score {
private:
    vector<char> guessedLetters;
    string wordToGuess;
    int wordLength;
    char letter{};
    int guesses;
    bool foundLetter{};
    int correctLetters;
    int score{};

public:
    Hangman() {
        wordToGuess = Word::WordSelect();
        //wordLength has to be set to number of unique letters in the word
        wordLength = wordToGuess.length();
        guesses = 0;
        correctLetters = 0;
        score = 0;
    }

    void PrintUnderscores() const {
        for (int i = 0; i < wordLength; i++) {
            cout << "_ ";
        }
    }

    void GameInitialize() {
        cout << "Welcome to Hangman!" << endl;
        Difficulty::difficultyMenu();
        guesses = Difficulty::DifficultySelect();
        score += guesses * -1 + 10;
        cout << "The word to guess is: " << endl;
        PrintUnderscores();
        cout << endl;
        GameLoop();
    }

    void GameLoop() {
        while (guesses > 0) {
            EnterLetter();
            CheckLetter();
            CheckIfWon();
            if (correctLetters == wordLength) {
                break;
            }
        }
        GameOver();
    }

    void EnterLetter() {
        while (true) {
            cout << "Enter a letter: ";
            cin >> letter;
            CheckInput();
            CheckIfGuessed();
            if (isalpha(letter)) {
                letter = tolower(letter);
                break;
            } else {
                cout << "Invalid input. Please enter a letter." << endl;
            }
        }
        guessedLetters.push_back(letter);
    }

    void CheckLetter() {
        foundLetter = false;
        for (int i = 0; i < wordLength; i++) {
            if (wordToGuess[i] == letter) {
                foundLetter = true;
                correctLetters++;

            }
        }
        if (foundLetter) {
            cout << "Correct!" << endl;
            score += 2;
            ReplaceUnderscore();
        } else {
            cout << "Wrong!" << endl;
            guesses--;
        }

        cout << "Guesses left: " << guesses << endl;
    }


    void CheckIfGuessed() {
        for (char c: guessedLetters) {
            if (c == letter) {
                cout << "You already guessed that letter" << endl;
                EnterLetter();
            }
        }
    }

    void CheckIfWon() const {
        if (correctLetters == wordLength) {
            cout << "You won!" << endl;
            PlayAgain();
        }
    }

    void ReplaceUnderscore() {
        for (int i = 0; i < wordLength; i++) {
            if (wordToGuess[i] == letter) {
                cout << letter << " ";
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

    void GameOver() {
        cout << "Game Over!" << endl;
        if (guesses == 0) {
            cout << "You lost! The word was " << wordToGuess << endl;
        } else {
            cout << "You won!" << endl;
        }
        PlayAgain();
    }

    void PlayAgain() const {
        char playAgain;
        cout << "Play again? (y/n): ";
        cin >> playAgain;
        CheckInput();
        if (playAgain == 'y') {
            Hangman newGame;
            newGame.GameInitialize();
        } else {

            string name;
            cout << "Enter your name: ";
            cin >> name;

            Score s;
            s.SetName(name);
            s.SetScore(score);
            s.SaveScore();
            cout << "Thanks for playing!" << endl;
            exit(0);
        }

    }

};

int main() {
    int choice = 0;
    Hangman game;
    Score score;
    Title();
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
                Score::DisplayScores();
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

void Title() {
    cout << " __  __                                                           \n"
            "/\\ \\/\\ \\                                                          \n"
            "\\ \\ \\_\\ \\     __      ___      __     ___ ___      __      ___    \n"
            " \\ \\  _  \\  /'__`\\  /' _ `\\  /'_ `\\ /' __` __`\\  /'__`\\  /' _ `\\  \n"
            "  \\ \\ \\ \\ \\/\\ \\L\\.\\_/\\ \\/\\ \\/\\ \\L\\ \\/\\ \\/\\ \\/\\ \\/\\ \\L\\.\\_/\\ \\/\\ \\ \n"
            "   \\ \\_\\ \\_\\ \\__/.\\_\\ \\_\\ \\_\\ \\____ \\ \\_\\ \\_\\ \\_\\ \\__/.\\_\\ \\_\\ \\_\\\n"
            "    \\/_/\\/_/\\/__/\\/_/\\/_/\\/_/\\/___L\\ \\/_/\\/_/\\/_/\\/__/\\/_/\\/_/\\/_/\n"
            "                               /\\____/                            \n"
            "                               \\_/__/                             " << endl;
}