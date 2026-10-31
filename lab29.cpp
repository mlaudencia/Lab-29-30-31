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
const int injRetChance = 12;
const int retChance = 20;
const int beginPromoteChance = 25;
const int amatPromoteChance = 50;
const int injChance = 15;
const int joinChance = 3;
const int joinSecChance = 50;
const int joinThirdChance = 100;
const int proChance = 5;
const int amatChance = 20;
const int amatRange = 10;
const int amatDraws = 4;
const int proRange = 25;
const int proDraws = 10;
const int injChanceDoron = 30;
const int injChancePot = 10;
const int injChanceIsaac = 15;
const int geniusBonus = 20;
const int winChanceDoron = 60;
const int drawChanceDoron = 70;
const int winChancePot = 45;
const int drawChancePot = 65;
const int winChanceIsaac = 40;
const int drawChanceIsaac = 70;
const int recChance = 5;
const int geniusChanceIsaac = 20;
const int geniusChance = 5;

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
void recoverFighter(array<list<Fighter>, ARRAY_SIZE>& exp, Fighter& f, const string& gym, int l);
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
                    if(fighter.isInjured){
                        if(rand() % injRetChance == 0){
                            retire.push_back(fighter);
                        } else{
                        recoverFighter(experience, fighter, gymName, l);
                        continue;
                        }
                    }
                    for(int i = 0; i < (rand() % maxMatches + 1); i++){
                        simulateMatch(fighter, gymName);
                    }
                    if(rand() % retChance == 0){
                        // Here's a 1/20 chance for a fighter to retire.
                        retire.push_back(fighter);
                    }
                    else if(l == 0 && rand() % beginPromoteChance == 0){
                        // Here's a 1/25 chance to promote from beginner to
                        // amateur
                        promote.push_back(fighter);
                    }
                    else if(l == 1 && rand() % amatPromoteChance == 0){
                        // Here's a 1/50 chance to promot from amateur to pro
                        promote.push_back(fighter);
                    }
                    else if(rand() % injChance == 0){
                        injure.push_back(fighter);
                    }
                }
                for(const Fighter& fighter: promote){
                    promoteFighter(experience, fighter, gymName, l);
                }
                for(const Fighter& fighter: retire){
                    retireFighter(fighters, fighter, gymName);
                }
                for(const Fighter& fighter: injure){
                    injureFighter(experience, fighter, gymName);
                }
            }
            if(rand() % joinChance == 0){ // This is a 1/3 chance for a fighter to
                                 // join a gym, and then a 1/50 and 1/100
                                 // chance for additional fighters to join
                                 // in the same time interval.
                addFighter(gyms, gymName, names);
                if(rand() % joinSecChance == 0){
                    // This is 1/50 chance for a second fighter to join the
                    // gym in the same time interval
                    addFighter(gyms, gymName, names);
                    if(rand() % joinThirdChance == 0){
                        // This is a 1/100 chance for a third fighter to join
                        // the gym in the same time interval
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
        cout << f.name << " has retired from the " << gym << " gym";
        if(f.isInjured){
            cout << " due to injury.\n";
        }
        else{
            cout << ".\n";
        }
        fighters.erase(it);
    } else{
        cout << f.name << " was not found at the " << gym << " gym.";
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
    int genius = rand() % RARITY;
    int wins = 0, losses = 0, draws = 0;
    bool isGenius = false;
    int level = BEGINNER;
    string name = names[rand() % names.size()];
    if(gym == "IsaacMovement"){
        if(genius - geniusChanceIsaac < 0){
            genius = 0;
        }
        else{
            genius -= geniusChanceIsaac;
        }
    }
    if(roll < proChance){
        level = PRO;
    } else if(roll < amatChance){
        level = AMATEUR;
    }
    if(genius < geniusChance){
        isGenius = true;
        // If a fighter is a genius, they have increased chances for wins and
        // losses in matches.
    }
    if(level == AMATEUR){
        wins = rand() % amatRange;
        losses = rand() % amatRange;
        // random num from 0-10 for how many wins/losses amateurs start with
        draws = rand() % amatDraws;
        // same thing but from 0-4 for draws
    }
    else if(level == PRO){
        wins = rand() % proRange;
        losses = rand() % proRange;
        // random num from 0-25 for how many wins/losses pros start with
        draws = rand() % proDraws;
        // same thing but from 0-10 for draws
    }
    Fighter newFighter{name, level, wins, losses, draws, false, isGenius};
    gyms[gym][level].push_back(newFighter);
    if(newFighter.isGenius){
            cout << " * ";
            // Give geniuses a little mark to easily recognize them
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

// promoteFighter() promotes a fighter to the next experience level if they 
// aren't a pro.
// arguments: array<list<Fighter>, ARRAY_SIZE>& exp, const Fighter& f, const string& gym, int l
// returns: nothing
void promoteFighter(array<list<Fighter>, ARRAY_SIZE>& exp, const Fighter& f, const string& gym, int l){
    if(l < ARRAY_SIZE - 1){
        exp[l].remove_if([&f](const Fighter& fighter){
            return fighter.name == f.name;
        });
        // The above removes a fighter from their old experience level
        Fighter pf = f;
        pf.level += 1;
        exp[l + 1].push_back(pf);
        // The above makes a copy of said fighter, and just increase their
        // experience level by 1, and puts the fighter at that level.
        if(f.isGenius){
            cout << " * ";
        }
        cout << f.name << ", from the " << gym << " gym, has been promoted to a";
        if(l == 0){
            cout << "n amateur fighter! Congrats!\n";
        } else{
            cout << " pro fighter! Congrats!\n";
        }
    }
}

// injureFighter() injures a fighter and makes them unable to compete depending
// on the gym they are from.
// arguments: array<list<Fighter>, ARRAY_SIZE>& exp, const Fighter& f, const string& gym
// returns: nothing
void injureFighter(array<list<Fighter>, ARRAY_SIZE>& exp, const Fighter& f, const string& gym){
    int roll = rand() % RARITY;
    bool yes = false;
    if(gym == "DoronDestroyers"){
        if(roll < injChanceDoron){
            yes = true;
        }
    } else if(gym == "PotProtectors"){
        if(roll < injChancePot){
            yes = true;
        }
    } else{
        if(roll < injChanceIsaac){
            yes = true;
        }
    }
    // All the if statements above just determine whether or not a fighter
    // gets injured or not. My intent was for DoronDestroyers to have a higher
    // win chance in exchange for higher injury chance, since they spar so
    // hard. PotProtectors are more safe and have a lower win chance, but 
    // a much lower chance for injury. IsaacMovement has the lowest win chance
    // but they make up for it for their high rate of geniuses, which increases
    // a fighter's win chance by a large amount.

    // The below just makes sure that the injury status of a fighter is changed
    // to true. After a fighter gets injured, they have a 1/5 chance to recover
    // every time interval. While they are injured, they cannot have any
    // matches or get promoted.
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
    int roll = rand() % RARITY;
    if(f.isGenius){
        if(roll - geniusBonus < 0){
            roll = 0;
        }
        else{
            roll -= geniusBonus;
        }
    }

    if(gym == "DoronDestroyers"){
        if(roll < winChanceDoron)
            f.win++;
        else if(roll < drawChanceDoron)
            f.draw++;
        else
            f.loss++;
    }
    
    else if(gym == "PotProtectors"){
        if(roll < winChancePot)
            f.win++;
        else if(roll < drawChancePot)
            f.draw++;
        else
            f.loss++;
    }

    else{
        if(roll < winChanceIsaac)
            f.win++;
        else if(roll < drawChanceIsaac)
            f.draw++;
        else
            f.loss++;
    }
}

void recoverFighter(array<list<Fighter>, ARRAY_SIZE>& exp, Fighter& f, const string& gym, int l){
    if(rand() % recChance == 0){
        for(int i = 0; i < ARRAY_SIZE; i++){
            auto it = find_if(exp[i].begin(), exp[i].end(), [&f](const Fighter& fighter){
                return fighter.name == f.name;
            });

            if(it != exp[i].end()){
                it->isInjured = false;
                if(f.isGenius){
                    cout << " * ";
                }
                cout << it->name << " has recovered from injury at " << gym << " gym!\n";
            }
        }
    }
}