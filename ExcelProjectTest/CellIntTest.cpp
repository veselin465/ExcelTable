#include "catch_amalgamated.hpp"

#include "../ExcelProject/CellInt.h"
#include "../ExcelProject/Cell.h"

TEST_CASE ("CellInt :: constructor (empty)"){
    CellInt ci;
    REQUIRE (ci.getValue() == 0);
}

TEST_CASE ("CellInt :: constructor (std::string)"){
    CellInt ci("12");
    REQUIRE (ci.getValue() == 12);
}

TEST_CASE ("CellInt :: getPointer()"){
    CellInt ci;
    Cell* cell = static_cast<Cell*>(&ci);
    REQUIRE (&ci == ci.getPointer());
    REQUIRE (cell == ci.getPointer());
}

TEST_CASE ("CellInt :: getDisplayableString()"){
    CellInt ci1("12");
    REQUIRE (ci1.getDisplayableString() == "12");
    CellInt ci2;
    REQUIRE (ci2.getDisplayableString() == "0");
}

TEST_CASE ("CellInt :: constructor (invalid input)"){
    REQUIRE_THROWS_AS (CellInt("a12"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt("1a2"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt("12a"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt("aa"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt("."), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt("5."), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt(".5"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt(".5."), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt(" "), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt(" 5"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt("-5 "), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt("5 "), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt(""), std::invalid_argument);
    REQUIRE_THROWS_AS (CellInt("5-"), std::invalid_argument);
}

TEST_CASE ("CellInt :: setValue (invalid input)"){
    CellInt ci;
    REQUIRE_THROWS_AS (ci.setValue("a12"), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue("1a2"), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue("12a"), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue("aa"), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue("."), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue("5."), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue(".5"), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue(".5."), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue(" "), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue(" 5"), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue("-5 "), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue("5 "), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue(""), std::invalid_argument);
    REQUIRE_THROWS_AS (ci.setValue("5-"), std::invalid_argument);
}

// no exceptions should be thrown
TEST_CASE ("CellInt :: sign input (constructor)"){
    CellInt("2");
    CellInt("+2");
    CellInt("-2");
}

// no exceptions should be thrown
TEST_CASE ("CellInt :: sign input (setValue(std::string))"){
    CellInt ci;
    ci.setValue("2");
    ci.setValue("+2");
    ci.setValue("-2");
}

TEST_CASE ("CellInt :: change values"){
    CellInt ci;
    ci.setValue("2");
    REQUIRE (ci.getValue() == 2);
    ci.setValue("-22");
    REQUIRE (ci.getValue() == -22);
    ci.setValue("0");
    REQUIRE (ci.getValue() == 0);
}
