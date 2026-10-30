// COMSC-210 | Lab 29 | Jeremy Laudencia

// Here, I include <map>, <array>, and <list> per the lab instructions,
// <iostream> to handle couts, <string> to handle gym and fighter names,
// <fstream> to read in external file data, <algorithm> to find a fighter in
// a list, and <vector> to hold all the random names
#include <iostream>
#include <map>
#include <array>
#include <list>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

const int ARRAY_SIZE = 3;
const int numIntervals = 20;
const int SZ_NAMES = 200;
const int RARITY = 100;
const int BEGINNER = 0;
const int AMATEUR = 1;
const int PRO = 2;

void simulateGyms(map<string, array<list<string>, ARRAY_SIZE>>& gyms, const vector<string>& names, int t);
void retireFighter(list<string>& fighters, const string& fighter, const string& gym);
void addFighter(map<string, array<list<string>, ARRAY_SIZE>>& gyms, const string& gym, const vector<string>& names);
void displayGyms(const map<string, array<list<string>, ARRAY_SIZE>>& gyms);


int main(){
    map<string, array<list<string>, ARRAY_SIZE>> gymMap;

    ifstream fin("fighters_info.txt");
    if(!fin){
        cout << "Could not open external file, check if name is correct " <<
        " then restart the program." << endl;
        return 1;
    }
    string gymName;
    int level;
    string fighterName;
    while (fin >> gymName >> level >> fighterName){
        gymMap[gymName][level].push_back(fighterName);
    }
    fin.close();

    ifstream fin1("names.txt");
    if(!fin1){
        cout << "Could not open external file, check if name is correct " <<
        " then restart the program." << endl;
        return 1;
    }
    vector<string> names;
    string tempName;
    int i = 0;
    while (fin1 >> tempName){
        names.push_back(tempName);
    }
    fin1.close();

    simulateGyms(gymMap, names, numIntervals);
}

// simulateGyms() simulates random events over a time period, t
// arguments: map<string, array<list<string>, ARRAY_SIZE>>& gyms, const vector<string>& names, int t
// returns: nothing

// When I was making this, making the random events and stuff reminded me of
// video game RNG, so I enjoyed it. Right now, I only have retireFighter() and
// addFighter(), but there will come more events like levelUpFighter(), 
// fighterBreak() for injuries, and then fighterSwitchGym().
void simulateGyms(map<string, array<list<string>, ARRAY_SIZE>>& gyms, const vector<string>& names, int t){
    for(int i = 0; i < t; i++){
        cout << "======= TIME INTERVAL " << i+1 << " =======\n";
        displayGyms(gyms);
        for(auto& pair: gyms){ // This goes into each gym
            string gymName = pair.first;
            auto& experience = pair.second; // This is the levels of fighters
                                            // in each gym
            for(int l = 0; l < ARRAY_SIZE; l++){
                auto& fighters = experience[l]; // This is the fighters of
                                                // that specific level
                for(string fighter: fighters){
                    if(rand() % 20 == 0){
                        // Here's a 1/20 chance for a fighter to retire.
                        retireFighter(fighters, fighter, gymName);
                    }
                }
            }
            if(rand() % 3 == 0){ // This is a 1/3 chance for a fighter to
                                 // join a gym, and then a 1/50 and 1/100
                                 // chance for additional fighters to join
                                 // in the same time interval.
                addFighter(gyms, gymName, names);
                if(rand() % 50 == 0){
                    addFighter(gyms, gymName, names);
                    if(rand() % 100 == 0){
                        addFighter(gyms, gymName, names);
                    }
                }
            }
        }
    }
}

// retireFighter() retires a fighter and removes them from their gym
// arguments: list<string>& fighters, const string& fighter, const string& gym
// returns: nothing

// I am thinking of adding a retired fighters list just so that with a random
// chance, a retired fighter can come out of retirement.
void retireFighter(list<string>& fighters, const string& fighter, const string& gym){
    auto it = find(fighters.begin(), fighters.end(), fighter);
    if(it != fighters.end()){
        cout << fighter << " has retired from the " << gym << " gym.\n";
        fighters.erase(it);
    } else{
        cout << fighter << " was not found at the " << gym << " gym.\n";
    }
}

// addFighter() adds a fighter of random level to a gym
// arguments: map<string, array<list<string>, ARRAY_SIZE>>& gyms, const string& gym, const vector<string>& names
// returns: nothing

// I thought this was really videogame-like with the random chance for rarity.
// At first, I had this as a fighter joining a certain level in a gym, but
// that isn't very realistic, so I changed it so it was random what level the
// new fighter was. As seen below, a majority of the time, the new fighter will
// be a beginner with an 80% chance, then amateur 20% chance, and then pro 5%.
void addFighter(map<string, array<list<string>, ARRAY_SIZE>>& gyms, const string& gym, const vector<string>& names){
    int roll = rand() % RARITY;
    int level = BEGINNER;
    string newFighter = names[rand() % names.size()];
    if(roll < 5){
        level = PRO;
    } else if(roll < 20){
        level = AMATEUR;
    }
    gyms[gym][level].push_back(newFighter);
    cout << newFighter << " has joined the " << gym << " gym as ";
    if(level == PRO){
        cout << "a pro fighter.\n";
    } else if(level == AMATEUR){
        cout << "an amateur fighter.\n";
    } else{
        cout << "a beginner fighter.\n";
    }
}

// displayGyms() shows the current rosters and level of fighters of the gyms.
// arguments: const map<string, array<list<string>, ARRAY_SIZE>>& gyms
// returns: nothing
void displayGyms(const map<string, array<list<string>, ARRAY_SIZE>>& gyms){
    cout << "===== CURRENT GYM ROSTERS =====\n";
    for(const auto& pair: gyms){
        cout << pair.first << "\n";
        const auto& experience = pair.second;
        string fighterLevels[ARRAY_SIZE] = {"Beginner", "Amateur", "Pro"};
        for(int i = 0; i < ARRAY_SIZE; i++){
            cout << "  " << fighterLevels[i] << "\n";
            if(experience[i].empty()){
                cout << "    No fighters of this level.\n";
            }
            else{
                for(const auto& fighter: experience[i]){
                    cout << "    " << fighter << "\n";
                }
            }
        }
        cout << "\n";
    }
}