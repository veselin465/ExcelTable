#include <iostream>

#include "Table.h"
#include "Cell.h"
#include "CellInt.h"
#include "CellString.h"
#include "ControlCenter.h"

std::string stringToUpper(const std::string& str){
    std::string res(str);
    for(size_t  i = 0; i < res.size(); i++){
        if('a' <= res[i] && res[i] <= 'z'){
            res[i] = res[i] + 'A' - 'a';
        }
    }
    return res;

}

int main()
{

    std::string input;

    std::cout << "> ";
    getline(std::cin, input);

    ControlCenter cc;

    while(stringToUpper(input) != "EXIT"){

        try{
            if(input != ""){
                std::string output = cc.executeCommand(input);
                std::cout << "  OK: ";
                if(output != ""){
                    std::cout << output;
                }
                std::cout << "\n";
            }
        }catch(std::invalid_argument& e){
            std::cerr << "FAIL: " << e.what() << "\n";
        }
        std::cout << "> ";
        getline(std::cin, input);
    }

    std::cout << "\nSuccessfully exited\n";

    return 0;
}
