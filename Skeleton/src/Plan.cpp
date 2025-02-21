#include "../include/Plan.h"
#include <iostream>
#include <sstream>
#include <algorithm>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) : 
    plan_id(planId) , settlement(settlement) , selectionPolicy(selectionPolicy) , status(PlanStatus::AVALIABLE) , facilities() , underConstruction() , facilityOptions(facilityOptions) ,
    life_quality_score(0), economy_score(0), environment_score(0) , constructionLimit(0) {
    if (settlement.getType() == SettlementType::VILLAGE)
        constructionLimit = 1;
    if (settlement.getType() == SettlementType::CITY)
        constructionLimit = 2;
    if (settlement.getType() == SettlementType::METROPOLIS) 
        constructionLimit = 3;}

Plan::Plan(const Plan &other) :
    plan_id(other.plan_id) , settlement(other.settlement),selectionPolicy() , status(other.status) , facilities(), underConstruction(),
    facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), 
    environment_score(other.environment_score), constructionLimit(other.constructionLimit)   {
    selectionPolicy=other.selectionPolicy->clone();
    for (const Facility* facility : other.facilities)
        facilities.push_back(new Facility(*facility));
    for (const Facility* facility : other.underConstruction)
        underConstruction.push_back(new Facility(*facility));
    }

Plan::Plan(const Plan &other, Settlement& settlement) :
    plan_id(other.plan_id) , settlement(settlement),selectionPolicy() , status(other.status) , facilities(), underConstruction(),
    facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), 
    environment_score(other.environment_score), constructionLimit(other.constructionLimit)   {
    selectionPolicy=other.selectionPolicy->clone();
    for (const Facility* facility : other.facilities)
        facilities.push_back(new Facility(*facility));
    for (const Facility* facility : other.underConstruction)
        underConstruction.push_back(new Facility(*facility));
    }


Plan::Plan(Plan &&other): plan_id(other.plan_id), settlement(std::move(other.settlement)), selectionPolicy(other.selectionPolicy), 
    status(other.status), facilities(std::move(other.facilities)), underConstruction(std::move(other.underConstruction)), 
    facilityOptions(std::move(other.facilityOptions)), life_quality_score(other.life_quality_score), economy_score(other.economy_score), 
    environment_score(other.environment_score), constructionLimit(other.constructionLimit)  {
        other.selectionPolicy = nullptr;
        other.facilities.clear();
        other.underConstruction.clear();
        other.constructionLimit = 0;
    }

const int Plan::getID() const{
    return plan_id;
}
const int Plan::getlifeQualityScore() const{
    return life_quality_score;
}
const int Plan::getEconomyScore() const{
    return economy_score;
}
const int Plan::getEnvironmentScore() const{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    delete this->selectionPolicy;
    this->selectionPolicy = selectionPolicy;
    if (selectionPolicy->toString() == "bal" ){
        int lifeQuality = life_quality_score;
        int economy = economy_score;
        int environment = environment_score;
        for (const Facility* facility : underConstruction){
            lifeQuality = lifeQuality + facility->getLifeQualityScore();
            economy = economy + facility->getEconomyScore();
            environment = environment + facility->getEnvironmentScore();
        }
        dynamic_cast<BalancedSelection*>(selectionPolicy)->setScores(lifeQuality, economy, environment);
    }
    else {
        selectionPolicy->resetFields();
    }
}
void Plan::step(){
    if(status == PlanStatus::AVALIABLE){
        while (constructionLimit > (int)(underConstruction.size() )){
            underConstruction.push_back(new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName()));
        }
    }
    for (auto i = underConstruction.begin(); i != underConstruction.end();){
        (*i)->step();
        if ((*i)->getStatus() == FacilityStatus::OPERATIONAL){
            addFacility((*i));
            life_quality_score = life_quality_score + (*i)->getLifeQualityScore();
            economy_score = economy_score + (*i)->getEconomyScore();
            environment_score = environment_score + (*i)->getEnvironmentScore();
            i = underConstruction.erase(i); 
        } else i++;
    }
    if (constructionLimit == (int)underConstruction.size())
        status = PlanStatus::BUSY;
    else
        status = PlanStatus::AVALIABLE;
}
void Plan::printStatus(){
    std::ostringstream oss;
    for (const Facility* facility : facilities) {
        oss <<  "facilityName:" + facility->getName() << "\n";
        oss <<  "facilityStatus:" << "OPERATIONAL" << "\n";
    }
    for (const Facility* facility : underConstruction) {
        oss <<  "facilityName:" + facility->getName() << "\n";
        oss <<  "facilityStatus:" << "UNDER_CONSTRUCTIONS" << "\n";
    }
    
    std::cout << oss.str() << std::endl;
}
const vector<Facility*> &Plan::getFacilities() const{
    return facilities;
}
void Plan::addFacility(Facility* facility){
    facilities.push_back(facility);
}

string Plan::policyToString(){
     if (selectionPolicy->toString() == "nve")
            return "nve";
        else if (selectionPolicy->toString() == "bal")  
           return "bal";
        else if (selectionPolicy->toString() == "eco")   
            return "eco";
        else //env
            return "env";
}

const string Plan::toString() const{
    string print;
    if (status == PlanStatus::BUSY)
        print="busy";
    else
        print="available";
    std::ostringstream oss;
    oss << "PlanID:" << plan_id << "\n";
    oss << "SettlementName:" << settlement.getName() << "\n";
    oss << "PlanStatus:" << print << "\n";
    oss << "SelectionPolicy:" << selectionPolicy->toString() << "\n";
    oss << "LifeQualityScore:" << life_quality_score << "\n";
    oss << "EconomyScore:" << economy_score << "\n";
    oss << "EnviromentScore:" << environment_score << "\n";
    return oss.str();
}

Plan::~Plan(){
    delete selectionPolicy;
    for (Facility* facility : facilities)
        delete facility;
    for (Facility* facility : underConstruction)
        delete facility;
    facilities.clear();
    underConstruction.clear();
}


const string Plan::getSettlementName() const {
    return settlement.getName();
}

SelectionPolicy* Plan::getSelectionPolicy() const {
    return selectionPolicy;
}

const vector<FacilityType> & Plan::getFacilitiesOptions() const{
    return facilityOptions;
}