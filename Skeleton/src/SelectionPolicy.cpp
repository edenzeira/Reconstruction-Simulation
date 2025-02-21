#include "../include/SelectionPolicy.h"
#include <limits>
#include <algorithm>

NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

void NaiveSelection::resetFields(){
    lastSelectedIndex=-1;
}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "nve";
}

NaiveSelection *NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : 
    LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

void BalancedSelection::resetFields(){
    LifeQualityScore = 0;
    EconomyScore = 0;
    EnvironmentScore = 0;
}

void BalancedSelection::setScores (int LifeQuality, int Economy, int Environment){
    LifeQualityScore = LifeQuality;
    EconomyScore = Economy;
    EnvironmentScore = Environment;
}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    double minDiff = std::numeric_limits<double>::infinity();
    int nextFacilityInd;
    for (std::vector<FacilityType>::size_type i = 0; i<facilitiesOptions.size() && minDiff != 0; i++){
        int LifeQuality = facilitiesOptions[i].getLifeQualityScore() + LifeQualityScore;
        int Economy = facilitiesOptions[i].getEconomyScore() + EconomyScore;
        int Environment = facilitiesOptions[i].getEnvironmentScore() + EnvironmentScore;
        int maximum = std::max({LifeQuality, Economy, Environment});
        int minimum = std::min({LifeQuality, Economy, Environment});
        double diff = (double)(maximum-minimum);
        if (diff < minDiff){
            minDiff = diff;
            nextFacilityInd = i;
        }
    }
    setScores(LifeQualityScore + facilitiesOptions[nextFacilityInd].getLifeQualityScore() , EconomyScore + facilitiesOptions[nextFacilityInd].getEconomyScore() , EnvironmentScore + facilitiesOptions[nextFacilityInd].getEnvironmentScore());
    return facilitiesOptions[nextFacilityInd];
}


const string BalancedSelection::toString() const {
      return "bal";
}

BalancedSelection *BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

EconomySelection::EconomySelection() : lastSelectedIndex(0){}

void EconomySelection::resetFields(){
    lastSelectedIndex=0;
}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY){
        lastSelectedIndex = (lastSelectedIndex + 1)% facilitiesOptions.size();
    }
    lastSelectedIndex ++;
    return facilitiesOptions[lastSelectedIndex-1];
}

const string EconomySelection::toString() const {
    return "eco";
}

EconomySelection *EconomySelection::clone() const {
    return new EconomySelection(*this);
}

SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(0){}

void SustainabilitySelection::resetFields(){
    lastSelectedIndex=0;
}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ENVIRONMENT){
        lastSelectedIndex = (lastSelectedIndex + 1)% facilitiesOptions.size();
    }
    lastSelectedIndex ++;
    return facilitiesOptions[lastSelectedIndex-1];
}
const string SustainabilitySelection::toString() const {
    return "env";
}
SustainabilitySelection *SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}


