#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;
using std::string;
using namespace std;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const Plan &other); //copy constructor
        Plan(const Plan& other, Settlement& settlement); //copy constructor
        Plan(Plan &&other);  //move constructor
        Plan& operator=(const Plan&)=delete;
        const int getID() const;
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        const string getSettlementName() const;
        SelectionPolicy* getSelectionPolicy() const;
        const vector<FacilityType> & getFacilitiesOptions() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        string policyToString();
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        ~Plan();

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
        int constructionLimit;
};