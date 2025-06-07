/*
.\app_2.exe play
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

#define MAX_WRONG_GUESSES 5

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string boldGreen(const string &text) {
    return "\033[1;32m" + text + "\033[0m";
}

class HangmanGame {
private:
    vector<string> wordList;

    void saveScore(const string &name, int score) {
        ofstream file("leaderboard.txt", ios::app);
        if (file) {
            file << name << "," << score << "\n";
        }
    }

public:
    void loadWords(const string &filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error loading word list.\n";
            exit(1);
        }
        string word;
        while (file >> word) {
            wordList.push_back(word);
        }
    }

    void play() {
        clearScreen();
        srand(static_cast<unsigned>(time(nullptr)));

        if (wordList.empty()) {
            cout << "No words available to play.\n";
            return;
        }

        int totalScore = 0;
        bool gameOver = false;

        while (!gameOver) {
            string word = wordList[rand() % wordList.size()];
            int len = word.size();
            string guessed(len, '_');
            vector<bool> guessedLetters(26, false);

            guessed[0] = word[0];
            guessed[len - 1] = word[len - 1];
            for (int i = 1; i < len - 1; ++i) {
                if (word[i] == word[0] || word[i] == word[len - 1]) {
                    guessed[i] = word[i];
                }
            }

            int wrongGuesses = 0;
            bool wordCompleted = false;

            while (!wordCompleted && wrongGuesses < MAX_WRONG_GUESSES) {
                cout << "\nWord: " << guessed << endl;
                cout << "Guess a letter: ";
                char guess;
                cin >> guess;

                guess = tolower(guess);
                if (guessedLetters[guess - 'a']) {
                    cout << "Already guessed. Try another.\n";
                    continue;
                }
                guessedLetters[guess - 'a'] = true;

                bool found = false;
                for (int i = 1; i < len - 1; ++i) {
                    if (word[i] == guess) {
                        guessed[i] = guess;
                        found = true;
                    }
                }

                if (!found) {
                    wrongGuesses++;
                    cout << "Incorrect. Remaining: " << (MAX_WRONG_GUESSES - wrongGuesses) << endl;
                }

                if (guessed == word) {
                    cout << boldGreen("Correct!") << " " << "Word was: " << word << endl;
                    totalScore += len;
                    wordCompleted = true;
                }
            }

            if (wrongGuesses >= MAX_WRONG_GUESSES) {
                gameOver = true;
            }
        }

        cout << boldGreen("Game Over!");
        cout << " Score: " + to_string(totalScore) << endl;
        cout << "Enter your name: ";
        string name;
        cin >> name;
        saveScore(name, totalScore);
    }
};

int main(int argc, char *argv[]) {
    if (argc != 2 || string(argv[1]) != "play") {
        cout << "Usage: ./app_2.exe play\n";
        return 1;
    }

    HangmanGame game;
    game.loadWords("words.txt");
    game.play();
    return 0;
}
