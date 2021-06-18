#include <math.h>
#include "CellFormula.h"
#include "CellFormula.h"
#include "CellDouble.h"

CellFormula::CellFormula(const Table* tableRef)
    :tableRef_(tableRef)
{
    if(tableRef == nullptr){
        throw std::invalid_argument("Table pointer cannot be null.");
    }
    error_ = true;
    result_ = 0;
    formula_ = "=";
}

CellFormula::CellFormula(const Table* tableRef, const std::string& value)
    :tableRef_(tableRef)
{
    if(tableRef == nullptr){
        throw std::invalid_argument("Table pointer cannot be null.");
    }

    result_ = 0;
    formula_ = value;
    setValue(value);
}

CellFormula::CellFormula(const CellFormula& copy)
    :tableRef_(copy.tableRef_){
    error_ = copy.error_;
    result_ = copy.result_;
    formula_ = copy.formula_;
}

void CellFormula::trimEntirely(std::string& str){
    std::string res = "";
    for(size_t i = 0; i < str.size(); i++){
        if(str[i] == ' '){
            continue;
        }
        res += (char)str[i];
    }
    str = res;
}

int CellFormula::seekFirstOperation(const std::string& str, char ch[], size_t charCount){
    int bracketsBalance = 0;
    for(size_t i = 0; i < str.size(); i++){
        if(str[i] == '('){
            bracketsBalance++;
            continue;
        }
        if(str[i] == ')'){
            bracketsBalance--;
            continue;
        }
        if(bracketsBalance != 0){
            continue;
        }
        for(size_t charI = 0; charI < charCount; charI++){
            if(str[i] == ch[charI]){
                return i;
            }
        }

    }
    return -1;
}

int CellFormula::seekLastOperation(const std::string& str, char ch[], size_t charCount){
    int bracketsBalance = 0;
    for(int i = str.size() - 1; i >= 0; i--){
        if(str[i] == '('){
            bracketsBalance++;
            continue;
        }
        if(str[i] == ')'){
            bracketsBalance--;
            continue;
        }
        if(bracketsBalance != 0){
            continue;
        }
        for(size_t charI = 0; charI < charCount; charI++){
            if(str[i] == ch[charI]){
                return i;
            }
        }
    }
    return -1;
}

double CellFormula::extractCellValue(size_t row, size_t column){
    const Cell* cell = tableRef_->getCellPointer(row, column);
    if(cell == nullptr){
        return 0;
    }
    Cell* copy = cell->clone();
    CellFormula* cf = dynamic_cast<CellFormula*>(copy);
    if(cf != nullptr && cf->error()){
        delete copy;
        throw std::invalid_argument("Reference to formula with error error");
    }

    std::string v = copy->getDisplayableString();
    delete copy;
    try{
        CellDouble ctd(v);
        double res = ctd.getValue();
        return res;
    }catch(std::invalid_argument& e){
        // not a number
    }

    return 0.0;

}

double CellFormula::calculateFormulaRecursively(const std::string& currrentFormula){

    if(currrentFormula.size() == 0){
        throw std::invalid_argument("Invalid string. Possibly, a binary operation with fewer than 2 operands was given.");
    }

    if(currrentFormula[0] == '(' && currrentFormula[currrentFormula.size() - 1] == ')'){
        if(currrentFormula.size() == 2){
            throw std::invalid_argument("There's an empty string inside 1 set of brackets");
        }
        std::string sub_str = currrentFormula.substr(1, currrentFormula.size() - 2);
        return this->calculateFormulaRecursively(sub_str);
    }

    char arr[2];
    int pos = 0;

    arr[0] = '+';
    arr[1] = '-';
    pos = seekLastOperation(currrentFormula, arr, 2);
    if(pos >= 0){
        double l_res = 0;
        if(pos != 0){
            std::string left = currrentFormula.substr (0, pos);
            l_res = calculateFormulaRecursively(left);
        }
        std::string right = currrentFormula.substr (pos + 1, currrentFormula.size() - pos - 1);
        double r_res = calculateFormulaRecursively(right);
        if(currrentFormula[pos] == '+'){
            return (l_res + r_res);
        }
        if(currrentFormula[pos] == '-'){
            return (l_res - r_res);
        }

    }

    arr[0] = '*';
    arr[1] = '/';
    pos = seekFirstOperation(currrentFormula, arr, 2);
    if(pos >= 0){
        std::string left = currrentFormula.substr (0, pos);
        std::string right = currrentFormula.substr (pos + 1, currrentFormula.size() - pos - 1);
        double l_res = calculateFormulaRecursively(left);
        double r_res = calculateFormulaRecursively(right);
        if(currrentFormula[pos] == '*'){
            return (l_res * r_res);
        }
        if(currrentFormula[pos] == '/'){
            if(abs(r_res) < zero_){
                throw std::invalid_argument("Dividing by zero");
            }else{
                return l_res / r_res;
            }
        }
    }

    arr[0] = '^';
    pos = seekLastOperation(currrentFormula, arr, 1);
    if(pos >= 0){
        std::string left = currrentFormula.substr (0, pos);
        std::string right = currrentFormula.substr (pos + 1, currrentFormula.size() - pos - 1);
        double l_res = calculateFormulaRecursively(left);
        double r_res = calculateFormulaRecursively(right);
        return pow(l_res, r_res);
    }

    double operand = 0.0;

    try{
        CellDouble d(currrentFormula);
        operand = d.getValue();
        return operand;
    }catch(std::invalid_argument& e){
        // not a number
    }

    // check if currentString is a reference to a cell (any cell)

    size_t row;
    size_t col;
    bool isCell = false;
    try{
        row = Table::getRow(currrentFormula);
        col = Table::getColumn(currrentFormula);
        isCell = true;
    }catch(std::invalid_argument& e){
        // not a reference to a cell
    }

    if(isCell){
        double res = extractCellValue(row, col);
        return res;
    }

    // then it should be a random string
    throw std::invalid_argument("Entered formula is incorrect - contains unrecognizable characters");

}

double CellFormula::calculateFormula(const std::string& formula){
    std::string pureFormula = formula.substr(1, formula.size() - 1);
    trimEntirely(pureFormula);
    int bracketsBalance = 0;
    for(size_t i = 1; i < formula.size(); i++){
        if(formula[i] == '('){
            bracketsBalance++;
        }
        if(formula[i] == ')'){
            bracketsBalance--;
            if(bracketsBalance < 0){
                throw std::invalid_argument("Incorrect formula");
            }
        }
    }

    return calculateFormulaRecursively(pureFormula);
}

void CellFormula::setValue(const std::string& value){

    if(isValid(value)){
        try{
            result_ = calculateFormula(value);
            error_ = false;
        }catch(std::invalid_argument& e){
            result_ = 0;
            error_ = true;
        }
    }else{
        throw std::invalid_argument("Not a formula");
    }
}

void CellFormula::recalculate(){
    setValue(formula_);
}

CellFormula* CellFormula::getPointer(){
    return this;
}

bool CellFormula::isValid(const std::string& value){
    return (value.size() >= 1 && value[0] == '=');
}

void CellFormula::trimZeroes(std::string& str){
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

std::string CellFormula::getDisplayableString(){
    if(error_){
        return "#ERROR";
    }
    std::string str = std::to_string(result_);
    trimZeroes(str);
    return str;
}

std::string CellFormula::getConstructString(){
    return formula_;
}

double CellFormula::getValue(){
    return result_;
}

bool CellFormula::error(){
    return error_;
}

CellFormula* CellFormula::clone() const{
    return new CellFormula(*this);
}
