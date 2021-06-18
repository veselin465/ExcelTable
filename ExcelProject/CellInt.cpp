#include "CellInt.h"

CellInt::CellInt(){
    int_ = 0;
}

CellInt::CellInt(const std::string& value){
    int_ = 0;
    setValue(value);
}

CellInt::CellInt(const CellInt& copy){
    int_ = copy.int_;
}

void CellInt::setValue(const std::string& value){
    if(isValid(value)){
        int_ = std::stoi(value);
    }else{
        throw std::invalid_argument("Not int");
    }
}

CellInt* CellInt::getPointer(){
    return this;
}

bool CellInt::isValid(const std::string& value){
    size_t initialPos = 0;
    if(value.size() >= 1 && (value[0] == '+' || value[0] == '-')){
        initialPos++;
    }
    if(value.size() < initialPos + 1) return false;
    for(size_t i = initialPos; i < value.size(); i++){
        if('0' > value[i] || value[i] > '9'){
            return false;
        }
    }
    return true;
}

std::string CellInt::getDisplayableString(){
    return std::to_string(int_);
}

std::string CellInt::getConstructString(){
    return getDisplayableString();
}

int CellInt::getValue(){
    return int_;
}

CellInt* CellInt::clone() const{
    return new CellInt(*this);
}
