#include <iostream>
#include <map>
#include <array>
#include <list>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

const int ARRAY_SIZE = 3;

void simulateGyms(map<string, array<list<string>, ARRAY_SIZE>>, int t);
void createGyms(map<string, array<list<string>, ARRAY_SIZE>>& gyms);
void retireFighter(list<string> fighters, const string& fighter, const string& gym);


int main(){
    map<string, array<list<string>, ARRAY_SIZE>> gymMap;

    createGyms(gymMap);

    ifstream fin("gym_info.txt");
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


}

void simulateGyms(map<string, array<list<string>, ARRAY_SIZE>> gyms, int t){
    for(int i = 0; i < t; i++){
        for(auto& pair: gyms){
            string gymName = pair.first;
            auto& experience = pair.second;
            for(int l = 0; l < ARRAY_SIZE; l++){
                auto& fighters = experience[l];
                for(string fighter: fighters){
                    if(rand() % 1 == 0){
                        retireFighter(fighters, fighter, gymName);
                    }
                }
            }
        }
    }
}

void createGym(map<string, array<list<string>, ARRAY_SIZE>>& gyms){
    gyms["Doron Destroyers"][2].push_back("Scary Man Sam");
}

void retireFighter(list<string> fighters, const string& fighter, const string& gym){
    auto it = find(fighters.begin(), fighters.end(), fighter);
    if(it != fighters.end()){
        cout << fighter << " has retired from the " << gym << " gym.\n";
        fighters.erase(it);
    } else{
        cout << fighter << " was not found at the " << gym << " gym.\n";
    }

}