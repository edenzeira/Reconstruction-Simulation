#include "../include/Settlement.h"

Settlement::Settlement(const string &name, SettlementType type) : name(name), type(type) {}

const string &Settlement::getName() const{
    return name;
}

SettlementType Settlement::getType() const{
    return type;
}
const string Settlement::toString() const{
    string typeStr;
    if (type==SettlementType::VILLAGE)
        typeStr="Village";
    else if (type==SettlementType::CITY)
        typeStr="City";
    else
        typeStr="Metropolis";
    return "Settlment:" + name + "Type:" + typeStr;
}