#include <iostream>
#include <map>
#include <array>
#include <list>
#include <string>
#include <fstream>
using namespace std;

const int ARRAY_SIZE = 3;

void simulateFighters(map<string, array<list<string>, ARRAY_SIZE>>, int t);
void createGym(map<string, array<list<string>, ARRAY_SIZE>>& gyms);

int main(){

    
}

void createGym(map<string, array<list<string>, ARRAY_SIZE>>& gyms){
    gyms["Dorony Gym"][2].push_back("Scary Man Sam");
}