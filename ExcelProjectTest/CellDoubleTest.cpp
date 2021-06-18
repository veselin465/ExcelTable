#include "catch_amalgamated.hpp"

#include "../ExcelProject/CellDouble.h"
#include "../ExcelProject/Cell.h"

TEST_CASE ("CellDouble :: constructor (empty)"){
    CellDouble cd;
    REQUIRE (cd.getValue() == 0);
}

TEST_CASE ("CellDouble :: constructor (std::string)"){
    CellDouble cd1("12");
    REQUIRE (cd1.getValue() == 12);
    CellDouble cd2("12.3");
    REQUIRE (cd2.getValue() == 12.3);
}

TEST_CASE ("CellDouble :: getPointer()"){
    CellDouble cd;
    Cell* cell = static_cast<Cell*>(&cd);
    REQUIRE (&cd == cd.getPointer());
    REQUIRE (cell == cd.getPointer());
}

TEST_CASE ("CellDouble :: getDisplayableString()"){
    CellDouble cd1("12");
    REQUIRE (cd1.getDisplayableString() == "12");
    CellDouble cd2("12.567");
    REQUIRE (cd2.getDisplayableString() == "12.567");
    CellDouble cd3;
    REQUIRE (cd3.getDisplayableString() == "0");
}

TEST_CASE ("CellDouble :: getConstructString()"){
    CellDouble cd1("12");
    REQUIRE (cd1.getConstructString() == "12");
    CellDouble cd2("12.567");
    REQUIRE (cd2.getConstructString() == "12.567");
    CellDouble cd3;
    REQUIRE (cd3.getConstructString() == "0");
}

TEST_CASE ("CellDouble :: constructor (invalid input)"){
    REQUIRE_THROWS_AS (CellDouble("a12"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("1a2"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("12a"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("aa"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("."), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("5."), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble(".5"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("5.5.5"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("5.5."), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble(".5.5"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble(" "), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble(""), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("-5.5."), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("5-"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellDouble("5-."), std::invalid_argument);
}

TEST_CASE ("CellDouble :: setValue (invalid input)"){
    CellDouble cd;
    REQUIRE_THROWS_AS (cd.setValue("a12"), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("1a2"), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("12a"), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("aa"), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("."), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("5."), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue(".5"), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("5.5.5"), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("5.5."), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue(".5.5"), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue(" "), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue(""), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("-5.5."), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("5-"), std::invalid_argument);
    REQUIRE_THROWS_AS (cd.setValue("5-."), std::invalid_argument);
}

TEST_CASE ("CellDouble :: sign input (constructor)"){
    CellDouble("2");
    CellDouble("+2");
    CellDouble("-2");
    CellDouble("+2.5");
    CellDouble("-2.5");
}

TEST_CASE ("CellDouble :: sign input (setValue(std::string))"){
    CellDouble cd;
    cd.setValue("2");
    cd.setValue("+2");
    cd.setValue("-2");
    cd.setValue("+2.5");
    cd.setValue("-2.5");
}

TEST_CASE ("CellDouble :: change values"){
    CellDouble cd;
    cd.setValue("2");
    REQUIRE (cd.getValue() == 2.0);
    cd.setValue("-2.2");
    REQUIRE (cd.getValue() == -2.2);
    cd.setValue("+22.0");
    REQUIRE (cd.getValue() == 22.0);
    cd.setValue("0");
    REQUIRE (cd.getValue() == 0.0);
}
