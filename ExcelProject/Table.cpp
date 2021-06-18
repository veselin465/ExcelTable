#include <iostream>
#include <vector>

#include "Table.h"
#include "Cell.h"
#include "CellInt.h"
#include "CellDouble.h"
#include "CellString.h"
#include "CellFormula.h"

Cell*** Table::allocateTable(std::size_t rows, std::size_t columns){

    try{
        Cell*** tmp = new Cell**[rows];
        for(size_t  currentRow = 0; currentRow < rows; currentRow++){
            tmp[currentRow] = new Cell*[columns];
            for(size_t currentCol = 0; currentCol< columns; currentCol++){
                tmp[currentRow][currentCol] = nullptr;
            }
        }
        return tmp;
    }catch(std::bad_alloc& e){
        std::cerr << "\nTable allocation failed!\n";
        releaseTableData(false);
        throw e;
    }
}

void Table::releaseTableData(bool fullDataClear){
    for(size_t row = 0; row < rowsCount_; row++){
        for(size_t  col = 0; col < columnsCount_ && fullDataClear; col++){
            delete table_[row][col];
        }
        delete[] table_[row];
    }
    delete[] table_;
}

void Table::extendTable(size_t rows, size_t columns){

    size_t newRowsCount = std::max(rows, rowsCount_);
    size_t newColumnsCount = std::max(columns, columnsCount_);

    Cell*** tmp = allocateTable(newRowsCount, newColumnsCount);

    if(tmp == nullptr) return;

    for(size_t row = 0; row < rowsCount_; row++){
        for(size_t col = 0; col < columnsCount_; col++){
            // intended pointer copy
            tmp[row][col] = table_[row][col];
        }
    }

    releaseTableData(false);

    table_ = tmp;
    rowsCount_ = newRowsCount;
    columnsCount_ = newColumnsCount;

}

Table::Table(){
    rowsCount_ = 1;
    columnsCount_ = 1;
    table_ = allocateTable(rowsCount_, columnsCount_);
}

Table::Table(size_t rows, size_t cols){
    rowsCount_ = rows;
    columnsCount_ = cols;
    table_ = allocateTable(rowsCount_, columnsCount_);
}

// doesnt copy objects, only pointers
// TO DO: object copy
Table& Table::operator=(const Table& other){
    releaseTableData(false);
    rowsCount_ = other.rowsCount_;
    columnsCount_ = other.columnsCount_;
    table_ = allocateTable(rowsCount_, columnsCount_);
    for(size_t row = 0; row < rowsCount_; row++){
        for(size_t col = 0; col < columnsCount_; col++){
            if(other.table_[row][col] == nullptr){
                table_[row][col] = nullptr;
            }else{
                table_[row][col] = other.table_[row][col]->clone();
            }

        }
    }
    return *this;
}

Table::Table(const Table& copy){
    *this = copy;
}

Table::~Table(){
    releaseTableData(true);
}

bool Table::isCellInsideTable(size_t row, size_t column) const{
    if(row >= rowsCount_ || column >= columnsCount_) return false;
    return true;
}

void Table::recalculateAllFormulas(){
    for(size_t row = 0; row < rowsCount_; row++){
        for(size_t col = 0; col < columnsCount_; col++){
            if(table_[row][col] != nullptr){
                CellFormula* cf = dynamic_cast<CellFormula*>(table_[row][col]);
                if(cf != nullptr){
                    cf->recalculate();
                }
            }
        }
    }
}

void Table::setCellValue(size_t row, size_t column, const std::string& value){
    if(!isCellInsideTable(row, column)){
        extendTable(row + 1, column + 1);
    }

    Cell* newCellPtr = nullptr;

    try{
        CellInt* newCell = new CellInt(value);
        newCellPtr = dynamic_cast<Cell*>(newCell);
    }catch(std::invalid_argument& e){
        // not an integer
    }

    try{
        CellDouble* newCell = new CellDouble(value);
        newCellPtr = dynamic_cast<Cell*>(newCell);
    }catch(std::invalid_argument& e){
        // not a floating number
    }

    try{
        CellFormula* newCell = new CellFormula(this, value);
        newCellPtr = dynamic_cast<CellFormula*>(newCell);
    }catch(std::invalid_argument& e){
        // not a formula
    }

    try{
        CellString* newCell = new CellString(value);
        newCellPtr = dynamic_cast<Cell*>(newCell);
    }catch(std::invalid_argument& e){
        // not a string
    }

    if(newCellPtr != nullptr){
        deleteCellValue(row, column);
        table_[row][column] = newCellPtr;
    }else{
        throw std::invalid_argument("Invalid type");
    }

    recalculateAllFormulas();

}

void Table::deleteCellValue(size_t row, size_t column){
    if(!isCellInsideTable(row, column)){
        return;
    }
    delete table_[row][column];
}

void Table::resetTable(){
    releaseTableData(true);
    rowsCount_ = 1;
    columnsCount_ = 1;
    table_ = allocateTable(rowsCount_, columnsCount_);
}

std::string Table::getDisplayableCellValue(size_t row, size_t column){
    if(isCellInsideTable(row, column) && table_[row][column] != nullptr){
        return table_[row][column]->getDisplayableString();
    }else{
        return "";
    }
}

std::string Table::getConstructedCellValue(size_t row, size_t column){
    if(isCellInsideTable(row, column) && table_[row][column] != nullptr){
        return table_[row][column]->getConstructString();
    }else{
        return "";
    }
}

const Cell* Table::getCellPointer(size_t row, size_t column) const{
    if(isCellInsideTable(row, column) && table_[row][column] != nullptr){
        return table_[row][column];
    }else{
        return nullptr;
    }
}

void Table::getCenteredString(std::string& result, const std::string& value, size_t length, char filling){
    if(value.size() > length){
        throw std::invalid_argument("new length cannot be smaller than original length");

    }
    std::string s(length, filling);
    size_t valueSize = value.size();
    size_t fillingCount = length - valueSize;
    size_t pos = fillingCount/2;
    for(size_t i = 0; i < valueSize; i++){
        s[pos + i] = value[i];
    }
    result = s;
}

std::string Table::print(){

    std::vector<size_t> columnsLength (columnsCount_);
    std::string output;

    output += "\n";

    for(size_t col = 0; col < columnsCount_; col++){
        columnsLength.push_back(0);
        columnsLength[col] = 1;
        for(size_t row = 0; row < rowsCount_; row++){
            if(isCellInsideTable(row, col) && table_[row][col] != nullptr){
                size_t s = table_[row][col]->getDisplayableString().size();
                if(columnsLength[col] < s)
                    columnsLength[col] = s;
            }
        }

        columnsLength[col] += 2;
    }

    size_t rowsDigit = std::to_string(rowsCount_).size();

    std::string res;
    getCenteredString(res, "", rowsDigit, ' ');
    output += res;

    for(size_t col = 0 ; col < columnsCount_; col++){
        output += "|";
        output += " " + std::string(1, (char)('A' + col));
        getCenteredString(res, "", columnsLength[col]-2, ' ');
        output += res;
    }

    output += (std::string)"|" + "\n";

    for(size_t row = 0 ; row < rowsCount_; row++){
        std::string res;
        std::string out = std::to_string(row);
        getCenteredString(res, out, rowsDigit, ' ');
        output += res + '|';
        for(size_t col = 0 ; col < columnsCount_; col++){
            out = "";
            if(table_[row][col] != nullptr){
                out = table_[row][col]->getDisplayableString();
            }
            getCenteredString(res, out, columnsLength[col], ' ');
            output += res + "|";
        }
        output += '\n';
    }

    return output;
}

size_t Table::rowsCount(){
    return rowsCount_;
}

size_t Table::columnsCount(){
    return columnsCount_;
}

size_t Table::getRow(const std::string& pos){
    if(pos.size() <= 1){
        throw std::invalid_argument("Invalid position (column) format.");
    }
    size_t col = 0;
    for(size_t i = 1; i < pos.size(); i++){
        if('0' <= pos[i] && pos[i] <= '9'){
            col *= 10;
            col += (pos[i] - '0');
        }else{
            throw std::invalid_argument("Invalid position (column) format.");
        }
    }
    return col;
}

size_t Table::getColumn(const std::string& pos){
    if(pos.size() < 1){
        throw std::invalid_argument("Invalid position (row) format.");
    }
    if('A' <= pos[0] && pos[0] <= 'Z'){
        return (pos[0] - 'A');
    }else if('a' <= pos[0] && pos[0] <= 'z'){
        return (pos[0] - 'a');
    }else{
        throw std::invalid_argument("Invalid position (row) format.");
    }
}
