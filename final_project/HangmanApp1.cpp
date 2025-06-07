/*
.\app_1.exe add_word apple
.\app_1.exe view_words
.\app_1.exe delete_word apple
.\app_1.exe view_leaderboard
.\app_1.exe view_history player1
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

string boldCyan(const string &text) {
    return "\033[1;36m" + text + "\033[0m";
}

class WordManager {
public:
    static void addWord(const string &word) {
        clearScreen();
        ofstream file("words.txt", ios::app);
        if (!file) {
            cerr << "Error: Could not open words.txt for writing.\n";
            return;
        }
        file << word << "\n";
        cout << boldCyan("Word '" + word + "' added.") << "\n";
    }

    static void deleteWord(const string &word) {
        clearScreen();
        ifstream inFile("words.txt");
        ofstream outFile("temp.txt");
        if (!inFile || !outFile) {
            cerr << "Error: Could not open files for word deletion.\n";
            return;
        }

        string current;
        bool found = false;
        while (getline(inFile, current)) {
            if (current != word) {
                outFile << current << '\n';
            } else {
                found = true;
            }
        }

        inFile.close();
        outFile.close();

        remove("words.txt");
        rename("temp.txt", "words.txt");

        if (found)
            cout << boldCyan("Word '" + word + "' deleted.") << "\n";
        else
            cout << boldCyan("Word not found.") << "\n";
    }

    static void viewWords() {
        clearScreen();
        ifstream file("words.txt");
        if (!file) {
            cerr << "Error: Could not open words.txt for reading.\n";
            return;
        }

        string word;
        cout << boldCyan("--- Word List ---") << "\n";
        while (file >> word) {
            cout << word << '\n';
        }
    }
};

class Leaderboard {
public:
    static void viewLeaderboard() {
        clearScreen();
        ifstream file("leaderboard.txt");
        if (!file) {
            cout << "No leaderboard data found.\n";
            return;
        }

        vector<pair<string, int>> players;
        string line;
        while (getline(file, line)) {
            size_t comma = line.find(',');
            if (comma != string::npos) {
                string name = line.substr(0, comma);
                int score = stoi(line.substr(comma + 1));
                players.emplace_back(name, score);
            }
        }

        sort(players.begin(), players.end(), [](auto &a, auto &b) {
            return a.second > b.second;
        });

        cout << boldCyan("--- Leaderboard ---") << "\n";
        for (size_t i = 0; i < players.size(); ++i) {
            cout << i + 1 << ". " << players[i].first << " - " << players[i].second << " points\n";
        }
    }

    static void viewHistory(const string &name) {
        clearScreen();
        ifstream file("leaderboard.txt");
        if (!file) {
            cout << "No history found.\n";
            return;
        }

        string line;
        bool found = false;
        cout << boldCyan("--- History for " + name + " ---") << "\n";
        while (getline(file, line)) {
            size_t comma = line.find(',');
            if (comma != string::npos && line.substr(0, comma) == name) {
                cout << line << '\n';
                found = true;
            }
        }
        if (!found) {
            cout << "No history found for " << name << ".\n";
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Usage: ./app_1.exe <command> [args...]\n";
        return 1;
    }

    string command = argv[1];

    if (command == "add_word" && argc == 3) {
        WordManager::addWord(argv[2]);
    } else if (command == "delete_word" && argc == 3) {
        WordManager::deleteWord(argv[2]);
    } else if (command == "view_words") {
        WordManager::viewWords();
    } else if (command == "view_leaderboard") {
        Leaderboard::viewLeaderboard();
    } else if (command == "view_history" && argc == 3) {
        Leaderboard::viewHistory(argv[2]);
    } else {
        cout << "Invalid command or arguments.\n";
    }

    return 0;
}
