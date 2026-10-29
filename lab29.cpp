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

void simulateGyms(map<string, array<list<string>, ARRAY_SIZE>>& gyms, const vector<string>& names, int t){
    for(int i = 0; i < t; i++){
        cout << "======= TIME INTERVAL " << i+1 << " =======\n";
        displayGyms(gyms);
        for(auto& pair: gyms){
            string gymName = pair.first;
            auto& experience = pair.second;
            for(int l = 0; l < ARRAY_SIZE; l++){
                auto& fighters = experience[l];
                for(string fighter: fighters){
                    if(rand() % 20 == 0){
                        retireFighter(fighters, fighter, gymName);
                    }
                }
            }
            if(rand() % 3 == 0){
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

void retireFighter(list<string>& fighters, const string& fighter, const string& gym){
    auto it = find(fighters.begin(), fighters.end(), fighter);
    if(it != fighters.end()){
        cout << fighter << " has retired from the " << gym << " gym.\n";
        fighters.erase(it);
    } else{
        cout << fighter << " was not found at the " << gym << " gym.\n";
    }
}

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
    cout << newFighter << " has joined the " << gym << " gym as an";
    if(level == PRO){
        cout << " pro fighter.\n";
    } else if(level == AMATEUR){
        cout << " amateur fighter.\n";
    } else{
        cout << " beginner fighter.\n";
    }
}

void displayGyms(const map<string, array<list<string>, ARRAY_SIZE>>& gyms){
    cout << "===== CURRENT GYM ROSTERS =====\n";
    for(const auto& pair: gyms){
        cout << pair.first << "\n";
        const auto& experience = pair.second;
        string fighterLevels[ARRAY_SIZE] = {"Beginner", "Amateur", "Pro"};
        for(int i = 0; i < ARRAY_SIZE; i++){
            cout << fighterLevels[i] << "\n";
            if(experience[i].empty()){
                cout << "No fighters of this level.\n";
            }
            else{
                for(const auto& fighter: experience[i]){
                    cout << fighter << "\n";
                }
            }
        }
        cout << "\n";
    }
}