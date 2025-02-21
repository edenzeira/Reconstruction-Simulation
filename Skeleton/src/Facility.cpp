
#include "../include/Facility.h"


FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
    name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score){}

const std::string &FacilityType::getName() const{
    return name;
}
int FacilityType::getCost() const{
    return price;
}
int FacilityType::getLifeQualityScore() const{
    return lifeQuality_score;
}
int FacilityType::getEnvironmentScore() const{
    return environment_score;    
}
int FacilityType::getEconomyScore() const{
    return economy_score;
}
FacilityCategory FacilityType::getCategory() const{
    return category;
}


Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) :
    FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
    settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price){}


Facility::Facility(const FacilityType &type, const string &settlementName):
    FacilityType(type),
    settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()){}

const std::string &Facility::getSettlementName() const{
    return settlementName;
}
const int Facility::getTimeLeft() const{
    return timeLeft;
}
FacilityStatus Facility::step(){
    timeLeft = timeLeft - 1;
    if (timeLeft == 0)
        this->setStatus(FacilityStatus::OPERATIONAL);
    return status;
}
void Facility::setStatus(FacilityStatus status){
    this->status=status;
}
const FacilityStatus& Facility::getStatus() const{
    return status;
}
const string Facility::toString() const{
    string categoryStr;
    if (category==FacilityCategory::LIFE_QUALITY)
        categoryStr="LIFE_QUALITY";
    else if (category==FacilityCategory::ECONOMY)
        categoryStr="ECONOMY";
    else
        categoryStr="ENVIRONMENT";
    return "facility" + name + "category" + categoryStr + std::to_string(price) + std::to_string(lifeQuality_score) + std::to_string(economy_score) + std::to_string(environment_score);
}