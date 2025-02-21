#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;
using namespace std;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        Simulation(const Simulation &other); //copy constructer
        Simulation& operator=(const Simulation &other); //copy assignement operator
        Simulation(Simulation &&other); //move constructer
        Simulation& operator=(Simulation &&other); //move assignement operator
        ~Simulation(); //destructor
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        void actAction(std::string action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isFacilityExists(const string &facilityName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        int getPlanCounter();
        void printActionsLog();
        void parseConfigFile(const string &line);
        void step();
        void close();
        void open();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};