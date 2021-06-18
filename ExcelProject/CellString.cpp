#include "CellString.h"

CellString::CellString(){
    string_ = "\"\"";
}

CellString::CellString(const std::string& value){
    setValue(value);
}

CellString::CellString(const CellString& copy){
    setValue(copy.string_);
}

void CellString::setValue(const std::string& value){
    if(isValid(value)){
        string_ = value;
    }else{
        throw std::invalid_argument("Invalid string");
    }

}

CellString* CellString::getPointer(){
    return this;
}

bool CellString::isValid(const std::string& value){
    return value.size() >= 2
        && value[0] == '\"'
        && value[value.size() - 1] == '\"';
}

std::string CellString::getDisplayableString(){
    return string_.substr(1, string_.size() - 2);
}

std::string CellString::getConstructString(){
    return string_;
}

/*std::string CellString::getPureString(){
    return value_.substr(1, value_.size() - 2);
}*/

CellString* CellString::clone() const{
    return new CellString(*this);
}
