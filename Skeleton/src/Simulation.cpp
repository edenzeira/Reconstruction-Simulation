#include "../include/Simulation.h"
#include "../include/Action.h"
#include "../include/Settlement.h" 
#include "../include/Facility.h"   
#include "../include/Plan.h"       
#include "../include/SelectionPolicy.h" 
#include "../include/Auxiliary.h"
#include <fstream>      
#include <sstream>      
#include <iostream>     
#include <stdexcept>    
#include <string>       
#include <vector>       
#include <algorithm>
using namespace std;

Simulation::~Simulation(){
    for (BaseAction* action : actionsLog)
        delete action;
    for (Settlement* settlement : settlements)
        delete settlement;
    actionsLog.clear();
    settlements.clear();
    facilitiesOptions.clear();
    plans.clear();
}


Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog() ,  plans(), settlements(), facilitiesOptions(){
    std::ifstream configFile(configFilePath);
    std::string line;
    while (std::getline(configFile, line)) {  
        if (line.empty() || line[0] == '#')   
            continue;;
        parseConfigFile(line); 
    }
    configFile.close();
    }  

Simulation::Simulation(const Simulation &other) : 
isRunning(other.isRunning) , planCounter(other.planCounter) , actionsLog() , plans() , settlements() , facilitiesOptions() {
    for (BaseAction* action : other.actionsLog)
            this->actionsLog.push_back(action->clone());
    for (Settlement* settlement : other.settlements)
            this->settlements.push_back(new Settlement(*settlement));
    for (FacilityType facility : other.facilitiesOptions)
            this->facilitiesOptions.emplace_back(facility);
    for (Plan plan : other.plans)
            this->plans.push_back(Plan(plan, getSettlement(plan.getSettlementName())));
}

Simulation& Simulation::operator=(const Simulation &other){
    if (this != &other){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        for (BaseAction* action : actionsLog)
            delete action;
        for (Settlement* settlement : settlements)
            delete settlement;
        actionsLog.clear();
        settlements.clear();
        facilitiesOptions.clear();
        plans.clear();

        for (BaseAction* action : other.actionsLog)
            this->actionsLog.push_back(action->clone());
        for (Settlement* settlement : other.settlements)
            this->settlements.push_back(new Settlement(*settlement));
        for (FacilityType facility : other.facilitiesOptions)
            this->facilitiesOptions.push_back(facility);
        for (Plan plan : other.plans)
            this->plans.push_back(Plan(plan, getSettlement(plan.getSettlementName())));
    }
    return *this;
}
Simulation::Simulation(Simulation &&other) : 
isRunning(other.isRunning) , planCounter(other.planCounter) , actionsLog() , plans() , settlements() , facilitiesOptions() {
    for (BaseAction* action : other.actionsLog)
        this->actionsLog.push_back(action);
    for (Settlement* settlement : other.settlements)
        this->settlements.push_back(settlement);
    for (FacilityType facility : other.facilitiesOptions)
        this->facilitiesOptions.push_back(facility);
    for (Plan plan : other.plans)
        this->plans.push_back(Plan(plan, getSettlement(plan.getSettlementName())));
    other.actionsLog.clear();
    other.settlements.clear();
    other.facilitiesOptions.clear();
    other.plans.clear();
}

Simulation& Simulation::operator=(Simulation &&other){
    if (this != &other){
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        for (BaseAction* action : actionsLog)
            delete action;
        for (Settlement* settlement : settlements)
            delete settlement;
        actionsLog.clear();
        settlements.clear();
        facilitiesOptions.clear();
        plans.clear();

        for (BaseAction* action : other.actionsLog)
            this->actionsLog.push_back(action);
        for (Settlement* settlement : other.settlements)
            this->settlements.push_back(settlement);
        for (FacilityType facility : other.facilitiesOptions)
            this->facilitiesOptions.push_back(facility);
        for (Plan plan : other.plans)
            this->plans.push_back(Plan(plan, getSettlement(plan.getSettlementName())));
        other.actionsLog.clear();
        other.settlements.clear();
        other.facilitiesOptions.clear();
        other.plans.clear();
    }
    return *this;
}

void Simulation::start(){
    open();
    std::cout << "The simulation has started" << endl;
    std::cout << "Please enter an action:" << endl;
    std::string action;
    std::getline(std::cin, action);
    while(action != "close"){
        actAction(action);
        std::cout << "Please enter an action:" << endl;
        std::getline(std::cin, action);
    }
    Close *close = new Close();
    close->act(*this);
    addAction(close);
}
void Simulation::actAction(std::string action){
    vector <string> v = Auxiliary::parseArguments(action); 
    if (v[0] == "step"){  
        SimulateStep *step = new SimulateStep(std::stoi(v[1]));
        step->act(*this);
        addAction(step);
    } 
    else if (v[0] == "plan"){
         AddPlan *addPlan = new AddPlan(v[1], v[2]);
         addPlan->act(*this);
         addAction(addPlan);
    }
    else if (v[0] == "settlement"){
        AddSettlement *addSett = new AddSettlement(v[1], SettlementType(std::stoi(v[2])));
        addSett->act(*this);
        addAction(addSett);
    }
    else if (v[0] == "facility"){
        AddFacility *facility = new AddFacility(v[1], FacilityCategory(std::stoi(v[2])), std::stoi(v[3]), std::stoi(v[4]), std::stoi(v[5]), std::stoi(v[6]));
        facility->act(*this);
        addAction(facility);
    }
    else if (v[0] == "planStatus"){
        PrintPlanStatus *planStatus = new PrintPlanStatus(std::stoi(v[1]));
        planStatus->act(*this);
        addAction(planStatus);
    }
    else if (v[0] == "changePolicy"){
        ChangePlanPolicy *changePolicy = new ChangePlanPolicy(std::stoi(v[1]), v[2]);
        changePolicy->act(*this);
        addAction(changePolicy);
    }
    else if (v[0] == "log"){
        PrintActionsLog *printLog = new PrintActionsLog();
        printLog->act(*this);
        addAction(printLog);
    }
    else if (v[0] == "backup"){
        BackupSimulation *backup = new BackupSimulation();
        backup->act(*this);
        addAction(backup);
    }
    else{ //restore
        RestoreSimulation *restore = new RestoreSimulation();
        restore->act(*this);
        addAction(restore);
    } 

}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy){
    plans.emplace_back(planCounter, settlement, selectionPolicy, facilitiesOptions);
    planCounter++;
}

void Simulation::addAction(BaseAction *action){
        actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement){
        settlements.push_back(settlement);
        return true;
}
bool Simulation::addFacility(FacilityType facility){
        facilitiesOptions.push_back(facility);
        return true;
}
bool Simulation::isFacilityExists(const string &facilityName){
    bool ans=false;
    for (FacilityType temp : facilitiesOptions){
        if (temp.getName() == facilityName){
            ans=true;
            break;
        }
    }
    return ans;
}
bool Simulation::isSettlementExists(const string &settlementName){
    bool ans=false;
    for (Settlement* temp : settlements){
        if (temp->getName() == settlementName){
            ans=true;
            break;
        }
    }
    return ans;
}
Settlement & Simulation::getSettlement(const string &settlementName){
    for (Settlement* temp : settlements){
        if (settlementName==temp->getName())
            return *temp;
    }
    Settlement* s = nullptr;
    return *s;
}
Plan &Simulation::getPlan(const int planID){
    return plans[planID];
}

int Simulation::getPlanCounter(){
        return planCounter;
}

void Simulation::printActionsLog(){
    for (BaseAction* action : actionsLog){
            std::cout << action->toString() << "\n";
    }
}


void Simulation::step(){
    for (Plan& plan : plans)
        plan.step();
}
 
void Simulation::close(){
    for (Plan plan : plans){
        std::cout << plan.toString() << std::endl;
    }    
    isRunning = false;
}

void Simulation::open(){
    isRunning = true;
}

void Simulation::parseConfigFile(const string &line){
        vector<string> s = Auxiliary::parseArguments(line);
        if (s[0] == "settlement"){
                Settlement* settlement = new Settlement(s[1], SettlementType(std::stoi(s[2])));
                settlements.push_back(settlement); 
        }
        else if (s[0] == "facility"){
                FacilityType facility(s[1], FacilityCategory(std::stoi(s[2])), std::stoi(s[3]), std::stoi(s[4]), std::stoi(s[5]), std::stoi(s[6]));
                facilitiesOptions.push_back(facility);
        }
        else if (s[0] == "plan"){
                SelectionPolicy *policy = nullptr;
                if (s[2] == "nve")
                        policy = new NaiveSelection(); 
                else if (s[2] == "bal")  
                        policy = new BalancedSelection(0, 0, 0);
                else if(s[2] == "eco")   
                        policy = new EconomySelection();
                 else 
                        policy = new SustainabilitySelection(); //env
            Settlement& add = getSettlement(s[1]);
            plans.emplace_back(planCounter, add, policy, facilitiesOptions);
            planCounter++;
        }
}