#include "../include/Simulation.h"
#include "../include/Settlement.h"
#include "../include/Facility.h"
#include "../include/SelectionPolicy.h"
#include "../include/Plan.h"
#include <iostream>
#include <vector>

using namespace std;

Simulation* backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        cout << "usage: simulation <config_path>" << endl;
        return 0;
    }
    string configurationFile =argv[1];
    Simulation simulation(configurationFile);
    simulation.start();
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    return 0;
}