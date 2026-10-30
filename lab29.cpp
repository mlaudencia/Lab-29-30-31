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
#include <cstdlib>
#include <ctime>
using namespace std;

const int ARRAY_SIZE = 3;
const int numIntervals = 20;
const int SZ_NAMES = 200;
const int RARITY = 100;
const int BEGINNER = 0;
const int AMATEUR = 1;
const int PRO = 2;
const int maxMatches = 6;

struct Fighter{
    string name;
    int level;
    int win;
    int loss;
    int draw;
    bool isInjured;
    bool isGenius;
};

void simulateGyms(map<string, array<list<Fighter>, ARRAY_SIZE>>& gyms, const vector<string>& names, int t);
void retireFighter(list<Fighter>& fighters, const Fighter& f, const string& gym);
void addFighter(map<string, array<list<Fighter>, ARRAY_SIZE>>& gyms, const string& gym, const vector<string>& names);
void displayGyms(const map<string, array<list<Fighter>, ARRAY_SIZE>>& gyms);
void promoteFighter(array<list<Fighter>, ARRAY_SIZE>& exp, const Fighter& f , const string& gym, int l);
void injureFighter(array<list<Fighter>, ARRAY_SIZE>& exp, const Fighter& f, const string& gym);
void simulateMatch(Fighter& f, const string& gym);

int main(){

    srand(time(0));

    map<string, array<list<Fighter>, ARRAY_SIZE>> gymMap;

    ifstream fin("fighters_info.txt");
    if(!fin){
        cout << "Could not open external file, check if name is correct " <<
        " then restart the program." << endl;
        return 1;
    }
    string gymName;
    int level, wins, losses, draws;
    string fighterName;
    bool isInjured, isGenius;
    while (fin >> gymName >> level >> fighterName >> wins >> losses >> draws >> isInjured >> isGenius){
        Fighter newFighter{fighterName, level, wins, losses, draws, isInjured, isGenius};
        gymMap[gymName][level].push_back(newFighter);
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
void simulateGyms(map<string, array<list<Fighter>, ARRAY_SIZE>>& gyms, const vector<string>& names, int t){
    for(int i = 0; i < t; i++){
        cout << "======= TIME INTERVAL " << i+1 << " =======\n";
        displayGyms(gyms);
        for(auto& pair: gyms){ // This goes into each gym
            string gymName = pair.first;
            auto& experience = pair.second; // This is the levels of fighters
                                            // in each gym
            cout << "\n";
            for(int l = 0; l < ARRAY_SIZE; l++){
                auto& fighters = experience[l]; // This is the fighters of
                                                // that specific level
                vector<Fighter> promote;
                vector<Fighter> retire;
                vector<Fighter> injure;
                for(Fighter& fighter: fighters){
                    for(int i = 0; i < (rand() % maxMatches); i++){
                        simulateMatch(fighter, gymName);
                    }
                    if(rand() % 20 == 0){
                        // Here's a 1/20 chance for a fighter to retire.
                        retire.push_back(fighter);
                    }
                    else if(l == 0 && rand() % 25 == 0){
                        promote.push_back(fighter);
                    }
                    else if(l == 1 && rand() % 50 == 0){
                        promote.push_back(fighter);
                    }

                    if(rand() % 20 == 0){
                        injure.push_back(fighter);
                    }
                }
                for(const Fighter& fighter: promote){
                    promoteFighter(experience, fighter, gymName, l);
                }
                for(const Fighter& fighter: retire){
                    retireFighter(fighters, fighter, gymName);
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
void retireFighter(list<Fighter>& fighters, const Fighter& f, const string& gym){
    auto it = find_if(fighters.begin(), fighters.end(), [&f](const Fighter& fighter){
        return fighter.name == f.name;
    });
    if(f.isGenius){
            cout << " * ";
        }
    if(it != fighters.end()){
        cout << f.name << " has retired from the " << gym << " gym.\n";
        fighters.erase(it);
    } else{
        cout << f.name << " was not found at the " << gym << " gym.\n";
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
void addFighter(map<string, array<list<Fighter>, ARRAY_SIZE>>& gyms, const string& gym, const vector<string>& names){
    int roll = rand() % RARITY;
    int genius = rand() % 10;
    int wins = 0, losses = 0, draws = 0;
    bool isGenius = false;
    int level = BEGINNER;
    string name = names[rand() % names.size()];
    if(roll < 5){
        level = PRO;
    } else if(roll < 20){
        level = AMATEUR;
    }
    if(genius == 0){
        isGenius = true;
    }
    if(level == AMATEUR){
        wins = rand() % 10;
        losses = rand() % 10;
        draws = rand() % 4;
    }
    else if(level == PRO){
        wins = rand() % 25;
        losses = rand() % 25;
        draws = rand() % 10;
    }
    Fighter newFighter{name, level, wins, losses, draws, false, isGenius};
    gyms[gym][level].push_back(newFighter);
    if(newFighter.isGenius){
            cout << " * ";
        }
    cout << newFighter.name << " has joined the " << gym << " gym as ";
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
void displayGyms(const map<string, array<list<Fighter>, ARRAY_SIZE>>& gyms){
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
                    cout << "    ";
                    if(fighter.isGenius == true){
                        cout << " * ";
                    }
                    cout << fighter.name << " " << fighter.win << "-";
                    cout << fighter.loss << "-" << fighter.draw << " ";
                    if(fighter.isInjured == true){
                        cout << "(injured)";
                    }
                    cout << "\n";
                }
            }
        }
        cout << "\n";
    }
}

void promoteFighter(array<list<Fighter>, ARRAY_SIZE>& exp, const Fighter& f, const string& gym, int l){
    if(l < ARRAY_SIZE - 1){
        exp[l].remove_if([&f](const Fighter& fighter){
            return fighter.name == f.name;
        });
        Fighter pf = f;
        pf.level += 1;
        exp[l + 1].push_back(pf);
        if(f.isGenius){
            cout << " * ";
        }
        cout << f.name << ", from the " << gym << " gym, has been promoted to a ";
        if(l == 0){
            cout << "amateur fighter! Congrats!\n";
        } else{
            cout << "pro fighter! Congrats!\n";
        }
    }
}

void injureFighter(array<list<Fighter>, ARRAY_SIZE>& exp, const Fighter& f, const string& gym){
    int roll = rand() % RARITY;
    bool yes = false;
    if(gym == "DoronDestroyers"){
        if(roll < 12){
            yes = true;
        }
    } else if(gym == "PotProtectors"){
        if(roll < 4){
            yes = true;
        }
    } else{
        if(roll < 9){
            yes = true;
        }
    }
    if(yes){
        for(int i = 0; i < ARRAY_SIZE; i++){
            auto it = find_if(exp[i].begin(), exp[i].end(), [&f](const Fighter& fighter){
                return fighter.name == f.name;
            });

            if(it != exp[i].end()){
                it->isInjured = true;
                if(f.isGenius){
                    cout << " * ";
                }
                cout << it->name << " has been injured at " << gym << " gym!\n";
            }
        }
    }
}

void simulateMatch(Fighter& f, const string& gym){
    int roll = rand() % 100;
    if(f.isGenius){
        if(roll - 20 < 0){
            roll = 0;
        }
        else{
            roll -= 20;
        }
    }

    if(gym == "DoronDestroyers"){
        if(roll < 70)
            f.win++;
        else if(roll < 80)
            f.draw++;
        else
            f.loss++;
    }
    
    else if(gym == "PotProtectors"){
        if(roll < 60)
            f.win++;
        else if(roll < 75)
            f.draw++;
        else
            f.loss++;
    }

    else{
        if(roll < 55)
            f.win++;
        else if(roll < 70)
            f.draw++;
        else
            f.loss++;
    }
}