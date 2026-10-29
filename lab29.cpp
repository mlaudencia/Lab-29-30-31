#include <iostream>
#include <map>
#include <array>
#include <list>
#include <string>
#include <fstream>
using namespace std;

const int ARRAY_SIZE = 3;

void simulateFighters(map<string, array<list<string>, ARRAY_SIZE>>, int t);
void createGyms(map<string, array<list<string>, ARRAY_SIZE>>& gyms);

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

void createGym(map<string, array<list<string>, ARRAY_SIZE>>& gyms){
    gyms["Doron Destroyers"][2].push_back("Scary Man Sam");
}