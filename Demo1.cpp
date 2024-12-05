#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <map>

using namespace std;

// Function to shuffle and assign roles
void assignRoles(vector<string>& players, vector<string>& roles, map<string, string>& playerRoles) {
    random_shuffle(roles.begin(), roles.end());
    for (size_t i = 0; i < players.size(); i++) {
        playerRoles[players[i]] = roles[i];
    }
}

// Function to reveal Babu
void revealBabu(const map<string, string>& playerRoles) {
    for (const auto& pair : playerRoles) {
        if (pair.second == "Babu") {
            cout << pair.first << " is Babu and is revealed!\n";
        }
    }
}

// Function to randomly guess roles
string computerGuess(const vector<string>& players, const string& currentPlayer) {
    string suspect;
    do {
        suspect = players[rand() % players.size()];
    } while (suspect == currentPlayer); // Ensure the computer doesn't guess itself
    return suspect;
}

// Function for the Police's turn
bool userAsPolice(const map<string, string>& playerRoles, const vector<string>& players, const string& user) {
    string guess1, guess2;
    cout << "As Police, you must identify the Chor and Dakat.\n";
    cout << "Enter your first guess (Chor): ";
    cin >> guess1;
    cout << "Enter your second guess (Dakat): ";
    cin >> guess2;

    bool chorCorrect = false, dakatCorrect = false;
    for (const auto& pair : playerRoles) {
        if (pair.first == guess1 && pair.second == "Chor") {
            chorCorrect = true;
        }
        if (pair.first == guess2 && pair.second == "Dakat") {
            dakatCorrect = true;
        }
    }

    if (chorCorrect && dakatCorrect) {
        cout << "You correctly identified both the Chor and Dakat! You win this round!\n";
        return true;
    } else {
        cout << "Wrong guesses! Better luck next time.\n";
        return false;
    }
}

// Function to display final scores
void displayScores(const map<string, int>& totalPoints) {
    cout << "\nFinal Total Scores After 12 Rounds:\n";
    for (const auto& pair : totalPoints) {
        cout << pair.first << ": " << pair.second << " points\n";
    }
}

// Main game logic
int main() {
    srand(time(0));

    vector<string> players = {"Player1", "Player2", "Player3", "Player4"};
    vector<string> roles = {"Police", "Chor", "Dakat", "Babu"};
    map<string, string> playerRoles;

    string user = "Player1";
    map<string, int> totalPoints = {{"Player1", 0}, {"Player2", 0}, {"Player3", 0}, {"Player4", 0}};

    for (int round = 1; round <= 12; round++) {
        cout << "\n--- Round " << round << " ---\n";

        // Assign roles for this round
        assignRoles(players, roles, playerRoles);

        // Reveal the user's role
        cout << "Your role is: " << playerRoles[user] << endl;

        // Reveal who is Babu
        revealBabu(playerRoles);

        // Points for this round
        map<string, int> roundPoints = {{"Player1", 0}, {"Player2", 0}, {"Player3", 0}, {"Player4", 0}};

        // Game logic based on user's role
        if (playerRoles[user] == "Police") {
            bool win = userAsPolice(playerRoles, players, user);
            if (win) {
                roundPoints[user] += 80; // User wins as Police
            }
        } else if (playerRoles[user] == "Babu") {
            cout << "You are Babu! You earn 100 points this round!\n";
            roundPoints[user] += 100;
        } else if (playerRoles[user] == "Chor" || playerRoles[user] == "Dakat") {
            string guess = computerGuess(players, user);
            cout << "The computer guesses you are " << guess << endl;

            if (playerRoles[guess] == playerRoles[user]) {
                cout << "Computer guessed correctly. You earn 0 points this round.\n";
            } else {
                if (playerRoles[user] == "Chor") {
                    roundPoints[user] += 50;
                    cout << "Computer guessed wrong! As Chor, you earn 50 points.\n";
                } else if (playerRoles[user] == "Dakat") {
                    roundPoints[user] += 60;
                    cout << "Computer guessed wrong! As Dakat, you earn 60 points.\n";
                }
            }
        }

        // Logic for computer-controlled players
        if (playerRoles[user] != "Police") {
            string policeGuess = computerGuess(players, "Police");
            cout << "Police guesses that " << policeGuess << " is Chor or Dakat.\n";

            bool correct = false;
            for (const auto& pair : playerRoles) {
                if ((pair.first == policeGuess) && (pair.second == "Chor" || pair.second == "Dakat")) {
                    correct = true;
                    break;
                }
            }

            if (correct) {
                cout << "Police guessed correctly! Police wins this round.\n";
                for (const auto& pair : playerRoles) {
                    if (pair.second == "Police") roundPoints[pair.first] += 80;
                    if (pair.second == "Chor" || pair.second == "Dakat") roundPoints[pair.first] += 0;
                }
            } else {
                cout << "Police guessed wrong! Chor and Dakat win this round.\n";
                for (const auto& pair : playerRoles) {
                    if (pair.second == "Chor") roundPoints[pair.first] += 50;
                    if (pair.second == "Dakat") roundPoints[pair.first] += 60;
                    if (pair.second == "Police") roundPoints[pair.first] += 0;
                }
            }
        }

        // Add round points to total points
        for (const auto& pair : roundPoints) {
            totalPoints[pair.first] += pair.second;
        }
    }

    // Display final total scores
    displayScores(totalPoints);

    return 0;
}