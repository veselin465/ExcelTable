#include "CellDouble.h"

CellDouble::CellDouble(){
    double_ = 0;
}

CellDouble::CellDouble(const std::string& value){
    double_ = 0;
    setValue(value);
}

CellDouble::CellDouble(const CellDouble& copy){
    double_ = copy.double_;
}

void CellDouble::setValue(const std::string& value){
    if(isValid(value)){
        double_ = std::stod(value);
    }else{
        throw std::invalid_argument("Not double");
    }
}

CellDouble* CellDouble::getPointer(){
    return this;
}

void CellDouble::trimZeroes(std::string& str){
    size_t decimalPoint = 0;
    for(; decimalPoint < str.size(); decimalPoint++){
        if(str[decimalPoint]=='.'){
            break;
        }
    }
    int cut = str.size() - 1;
    for(; cut > decimalPoint; cut--){
        if(str[cut] != '0')
            break;
    }
    if(cut != decimalPoint){
        cut++;
    }
    str = str.substr(0, cut);
}

std::string CellDouble::getDisplayableString(){
    std::string str = std::to_string(double_);
    trimZeroes(str);
    return str;
}

std::string CellDouble::getConstructString(){
    return getDisplayableString();
}

bool CellDouble::isValid(const std::string& value){
    bool decimalPoint = false;
    size_t initialPos = 0;
    if(value.size() >= 1){
        if(value[0] == '.'){
            return false;
        }
        if(value[0] == '+' || value[0] == '-'){
            initialPos++;
        }
    }
    for(size_t i = initialPos ; i < value.size(); i++){
        if(value[i] == '.'){
            if(i + 1 >= value.size()){
                return false;
            }
            if(!decimalPoint){
                decimalPoint = true;
            }else{
                return false;
            }
            continue;
        }
        if('0' > value[i] || value[i] > '9'){
            return false;
        }
    }
    return true;
}

double CellDouble::getValue(){
    return double_;
}

Cell* CellDouble::clone() const{
    return new CellDouble(*this);
}
