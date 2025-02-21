#include "../include/Action.h"
#include "../include/Simulation.h"
#include "../include/Settlement.h"
#include <iostream>

string BaseAction::enumToString(ActionStatus status) const{
    string ans = "COMPLETED";
    if (status == ActionStatus::COMPLETED)
        ans = "COMPLETED";
    else
        ans="ERROR";
    return ans;
}

ActionStatus BaseAction::getActionStatus() const {
    return status; 
}

BaseAction::BaseAction():errorMsg(""), status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction::getStatus() const{
    return status;
}
void BaseAction::complete(){
    status=ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    this->errorMsg = errorMsg;
    std::cout << errorMsg << std::endl;
    status=ActionStatus::ERROR;
}
const string &BaseAction::getErrorMsg() const{
    return errorMsg;
}

SimulateStep::SimulateStep(const int numOfSteps) : BaseAction(), numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    int step=numOfSteps;
    while (step>0){
       simulation.step();
       step--;
    }
    complete();
}
const string SimulateStep::toString() const {
    return "Step " + std::to_string(numOfSteps) + " " + enumToString(getActionStatus());
}
SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) : BaseAction(), settlementName(settlementName) , selectionPolicy(selectionPolicy){}

void AddPlan::act(Simulation &simulation) {
    if (!simulation.isSettlementExists(settlementName)){
        error("Cannot create this plan");
        return;
    }
    SelectionPolicy* policy = nullptr;
    if (selectionPolicy == "nve")
        policy = new NaiveSelection();
    else if (selectionPolicy == "bal")  
        policy = new BalancedSelection(0, 0, 0);
    else if (selectionPolicy == "eco")   
        policy = new EconomySelection();
    else if (selectionPolicy == "env")
        policy = new SustainabilitySelection();
    else {
        error("Cannot create this plan");
        return;
    }
    simulation.addPlan(simulation.getSettlement(settlementName), policy);
    complete();
}
const string AddPlan::toString() const {
    return "plan " + settlementName + selectionPolicy + " " +  enumToString(getActionStatus());
}
AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}

AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType) : BaseAction(), settlementName(settlementName) , settlementType(settlementType){}

void AddSettlement::act(Simulation &simulation) {
    if (simulation.isSettlementExists(settlementName))
        error("ERROR: settlment already exist");
    else {
        Settlement* newSett =new Settlement(settlementName,settlementType);
        simulation.addSettlement(newSett);
        complete();
    }
}
AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}
const string AddSettlement::toString() const {
    string typeStr;
    if (settlementType==SettlementType::VILLAGE)
        typeStr="0";
    else if (settlementType==SettlementType::CITY)
        typeStr="1";
    else
        typeStr="2";
    return "settlement " + settlementName + " " + typeStr + " " +  enumToString(getActionStatus());
}


AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) :
BaseAction(), facilityName(facilityName) , facilityCategory(facilityCategory) , price(price) , lifeQualityScore(lifeQualityScore) , economyScore(economyScore) , environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    if (simulation.isFacilityExists(facilityName))
        error("ERROR: facility already exist");
    else {
        FacilityType newFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
        simulation.addFacility(newFacility);
        complete();
    }
}
AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}
const string AddFacility::toString() const {
    string type = nullptr;
    if (facilityCategory == FacilityCategory::LIFE_QUALITY)
        type = "0";
    else if (facilityCategory == FacilityCategory::ECONOMY)
        type = "1";
    else
        type = "2";
    return "facility " + facilityName + type + std::to_string(price) + std::to_string(lifeQualityScore) + std::to_string(economyScore) + std::to_string(environmentScore) + " " +  enumToString(getActionStatus());
}

PrintPlanStatus::PrintPlanStatus(int planId) : BaseAction(), planId(planId) {}
void PrintPlanStatus::act(Simulation &simulation) {
    if (planId >= simulation.getPlanCounter() || planId < 0)
        error("ERROR: plan doesn't exist");
    else {
        std::cout << simulation.getPlan(planId).toString() << std::endl;
        simulation.getPlan(planId).printStatus();
        complete();
    }
}
PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}
const string PrintPlanStatus::toString() const {
    return "planStatus " + std::to_string(planId) + " " +  enumToString(getActionStatus());
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) : BaseAction(), planId(planId) , newPolicy(newPolicy){}

void ChangePlanPolicy::act(Simulation &simulation) {
    string oldPolicy = (simulation.getPlan(planId)).policyToString();
    if (planId > simulation.getPlanCounter() || planId < 0 || oldPolicy == newPolicy)
        error("ERROR: cannot change selection policy");
    else{
        SelectionPolicy* policy = nullptr;
        if (newPolicy == "nve")
            policy = new NaiveSelection();
        else if (newPolicy == "bal")  
            policy = new BalancedSelection(0, 0, 0);
        else if (newPolicy == "eco")   
            policy = new EconomySelection();
        else if (newPolicy == "env")
            policy = new SustainabilitySelection();
        (simulation.getPlan(planId)).setSelectionPolicy(policy);
        complete();
        std::cout << "planID:" + std::to_string(planId) << "\n";
        std::cout << "previousPolicy:" + oldPolicy << "\n";
        std::cout << "newPolicy:" + newPolicy << std::endl;
    }
}
ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}
const string ChangePlanPolicy::toString() const {
    return "changePolicy "  + std::to_string(planId) + newPolicy + " " +  enumToString(getActionStatus());; 
}

PrintActionsLog::PrintActionsLog() : BaseAction(){};

void PrintActionsLog::act(Simulation &simulation) {
    simulation.printActionsLog();
    complete();
}
PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}
const string PrintActionsLog::toString() const {
    return "log " +  enumToString(getActionStatus());
}

Close::Close() : BaseAction(){}

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}
Close *Close::clone() const {
    return new Close(*this);
}
const string Close::toString() const {
    return "close " +  enumToString(getActionStatus());
}

BackupSimulation::BackupSimulation() : BaseAction(){}

void BackupSimulation::act(Simulation &simulation) {
    if (backup != nullptr)
        delete(backup);
    backup = new Simulation(simulation);
    complete();
}
BackupSimulation *BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}
const string BackupSimulation::toString() const {
    return "backup " +  enumToString(getActionStatus());
}

RestoreSimulation::RestoreSimulation() : BaseAction(){}

void RestoreSimulation::act(Simulation &simulation) {
    if (backup == nullptr)
        error("ERROR: no backup available");
    else{
        simulation = *backup;
        complete();
    }
}
RestoreSimulation *RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}
const string RestoreSimulation::toString() const {
    return "restore " +  enumToString(getActionStatus());
}