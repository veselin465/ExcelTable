#include <iostream>
#include <fstream>
#include "ControlCenter.h"

ControlCenter::ControlCenter(){
    filePath_ = "";
}

bool ControlCenter::confirmAction(char yes, char no){
    char input;
    std::cin >> input;
    std::cin.ignore();

    while(input != yes && input != no){
        std::cin >> input;
        std::cin.ignore();
    }

    return (yes == input);

}

bool ControlCenter::fileExist(const std::string& filename){

    bool doesExist = false;
    std::ifstream readFile(filename);

    if(readFile.is_open()){
        doesExist = true;
    }

    readFile.close();

    return doesExist;
}

bool ControlCenter::checkFormat(const std::string& str){
    if(str.size() <= 5) return false;
    std::string last4 = str.substr(str.size() - 4, 4);
    stringToUpper(last4);
    return last4 == ".CSV";
}

void ControlCenter::stringToUpper(std::string& str){
    for(size_t  i = 0; str[i]; i++){
        if('a' <= str[i] && str[i] <= 'z'){
            str[i] = str[i] + 'A' - 'a';
        }
    }
}

void ControlCenter::loadFromFile(const std::string& filename){

    if(!checkFormat(filename)){
        throw std::invalid_argument("Currently, only .csv file format is being supported. ");
    }

    if(!fileExist(filename)){
        throw std::invalid_argument("File not found. ");
    }

    std::ifstream readFile(filename);
    std::string dataLine;
    std::vector <std::string> dataLines;

    while (getline (readFile, dataLine)) {
        if(dataLine == "") continue;
        dataLines.push_back(dataLine);
    }

    if(!readFile.eof()){
        if(!fileExist(filename)){
            throw std::invalid_argument("File got removed or moved from its current location.");
        }

        throw std::invalid_argument((std::string)"File is not moved nor deleted, but still failed reading from it. " +
            "Possible reasons: 1) system permission deny, 2) Another software currently uses it, " +
            "3) Action was intentionally blocked by another software (like antivirus program or firewall)");
    }

    readFile.close();

    size_t rowsCount = std::max((size_t)1, dataLines.size());
    size_t columnsCount = 1;

    for(size_t line = 0; line < dataLines.size(); line++){
        size_t commaCount = 0;
        for(size_t i = 0; i < dataLines[line].size(); i++){
            if(dataLines[line][i] == ','){
                commaCount++;
            }
        }
        if(columnsCount < commaCount){
            columnsCount = commaCount;
        }
    }

    Table tmp(rowsCount, columnsCount);

    size_t successfulCells = 0;
    size_t totalCells = 0;

    for(size_t line = 0; line < dataLines.size(); line++){
        size_t stringStart = 0;
        size_t commaCount = 0;
        for(size_t i = 0; i < dataLines[line].size(); i++){
            if(dataLines[line][i] == ',' || i + 1 == dataLines[line].size()){
                // makes sure to take the last value in the column (if there's such)
                if(dataLines[line][i] != ',' && i + 1 == dataLines[line].size()){
                    i++;
                }
                if(stringStart - i > 0){
                    std::string str = dataLines[line].substr (stringStart, i - stringStart);
                    try{
                        tmp.setCellValue(line, commaCount, str);
                        successfulCells++;
                    }catch(std::invalid_argument& e){
                        std::cerr << "Error reading value on: " << ((char)('A' + commaCount)) << (line+1)
                                  << " -> " << str << "    \t(reason: " << e.what() << ")\n";

                    }
                    totalCells++;
                }

                stringStart = i + 1;
                commaCount++;
            }
        }
    }

    currentTable = tmp;

    filePath_ = filename;
    upToDate = true;

    std::cout << "File loading finished. [" << successfulCells << "/" << totalCells << "] successful data transfers";

}

void ControlCenter::saveToFile(const std::string& filename){
    if(!checkFormat(filename)){
        throw std::invalid_argument("Only .csv file format is being currently supported. ");
    }

    if(fileExist(filename)){
        std::cout << "File exists. Are you sure you want to continue and rewrite it? Y/N: ";
        bool res = confirmAction('Y', 'N');
        if(!res){
            return;
        }
    }

    std::ofstream writeFile(filename, std::ios::trunc);
    std::string commandLine;
    if(writeFile.fail()){
        throw std::invalid_argument((std::string)"Unexpected error while opening file. 1) Possible reasons: permission deny, " +
                                    "2) file exists, but another software denies access to it.");
    }

    for(size_t row = 0; row < currentTable.rowsCount(); row++){
        for(size_t col = 0; col < currentTable.columnsCount(); col++){
            if(writeFile.fail()){
                throw std::invalid_argument((std::string)"Unexpected error while opening file. 1) Possible reasons: permission deny, " +
                                            "2) file exists, but another software denies access to it.");
            }
            std::string s = currentTable.getConstructedCellValue(row, col);
            if(s != ""){
                writeFile << s;
            }
            if(col + 1 < currentTable.columnsCount()){
                writeFile << ',';
            }
        }
        if(row + 1 < currentTable.rowsCount())
        writeFile << std::endl;
    }
    writeFile.close();

    if(filePath_ == filename){
        upToDate = true;
    }
}

void ControlCenter::createEmptyFile(const std::string& filename){
    if(!checkFormat(filename)){
        throw std::invalid_argument("Only .csv file format is being currently supported. ");
    }
    std::ifstream readFile(filename);
    if(readFile.is_open()){
        std::cout << "File exists. Are you sure you want to continue and rewrite it? Y/N: ";
        bool res = confirmAction('Y', 'N');
        if(!res){
            return;
        }
    }
    readFile.close();

    std::ofstream writeFile(filename, std::ios::trunc);

    writeFile.close();

}

void ControlCenter::closeFile(){
    if(filePath_ == ""){
        currentTable.resetTable();
        return;
    }

    if(!upToDate){
        std::cout << "File not saved. Would you like to save? Y/N: ";
        bool res = confirmAction('Y', 'N');
        if(res){
            if(!fileExist(filePath_)){
                throw new std::invalid_argument("File not found. File was probably moved or deleted.");
            }
            saveToFile(filePath_);
        }
    }
    filePath_ = "";
    currentTable.resetTable();

}

std::vector <std::string> ControlCenter::splitWithQuotes(const std::string& commandLine){
    std::vector<std::string> argumentList;
    std::string currentStr;
    bool isInsideBracket = false;

    for(size_t i = 0 ; i < commandLine.size(); i++){

        if(commandLine[i] == ' '){
            if(isInsideBracket){
                currentStr += ' ';
            }else{
                if(currentStr != ""){
                    argumentList.push_back(currentStr);
                }
                currentStr = "";
            }
            continue;
        }

        if(commandLine[i] == '\"'){
            isInsideBracket = !isInsideBracket;
            currentStr += '\"';
            continue;
        }

        if(commandLine[i] == '\\'){
            if(i + 1 >= commandLine.size()){
                throw std::invalid_argument("Invalid use of symbol \'\\\'");
            }
            currentStr += '\\';
            i++;
            currentStr += commandLine[i];
            continue;
        }

        currentStr += commandLine[i];
    }

    if(currentStr != ""){
        argumentList.push_back(currentStr);
    }

    if(isInsideBracket){
        throw std::invalid_argument
            ("Not every opening quote has closing one. ");
    }

    return argumentList;

}

const std::string ControlCenter::executeCommand(const std::string& commandLine){

    std::vector<std::string> argumentList = splitWithQuotes(commandLine);


    if(argumentList.size() == 0){
        throw std::invalid_argument
            ("Empty or not meaningful string. ");
    }

    stringToUpper(argumentList[0]);

    std::string output = "";

    if(argumentList[0] == "EDIT"){

        if(argumentList.size() != 3){
            throw std::invalid_argument ("Invalid use of command: edit <position> <value>");
        }
        size_t row = Table::getRow(argumentList[1]);
        size_t col = Table::getColumn(argumentList[1]);
        currentTable.setCellValue(row, col, argumentList[2]);
        output += "Successfully set " + argumentList[1] + " to " + argumentList[2];
        upToDate = false;

    }else if(argumentList[0] == "PRINT"){

        if(argumentList.size() != 1){
            throw std::invalid_argument ("Invalid use of command print: too many arguments");
        }
        output = currentTable.print();

    }else if(argumentList[0] == "OPEN"){

        if(argumentList.size() != 2){
            throw std::invalid_argument ("Invalid use of command: open <path>");
        }

        size_t argSize = argumentList[1].size();
        if(argumentList[1][0] == '\"' && argumentList[1][argSize - 1] == '\"'){
            argumentList[1] = argumentList[1].substr(1, argSize - 2);
        }
        closeFile();
        loadFromFile(argumentList[1]);

    }else if(argumentList[0] == "CLOSE"){

        closeFile();

    }else if(argumentList[0] == "NEW"){

        if(argumentList.size() != 2){
            throw std::invalid_argument ("Invalid use of command: new <path>");
        }
        closeFile();
        createEmptyFile(argumentList[1]);

    }else if(argumentList[0] == "SAVEAS"){

        if(argumentList.size() != 2){
            throw std::invalid_argument ("Invalid use of command: saveas <path>");
        }
        saveToFile(argumentList[1]);

    }else if(argumentList[0] == "SAVE"){
        if(filePath_ == ""){
            throw std::invalid_argument ("File not opened.");
        }
        saveToFile(filePath_);

    }else{
        throw std::invalid_argument
            ("Unknown command: " + argumentList[0]);
    }

    return output;

}

