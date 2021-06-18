#include "catch_amalgamated.hpp"

#include "../ExcelProject/CellString.h"
#include "../ExcelProject/Cell.h"

TEST_CASE ("CellString :: constructor (empty)"){
    CellString cs;
    REQUIRE (cs.getDisplayableString() == "");
}

TEST_CASE ("CellString :: constructor (std::string)"){
    CellString cs("\"string\"");
    REQUIRE (cs.getDisplayableString() == "string");
}

TEST_CASE ("CellString :: getPointer()"){
    CellString cs;
    Cell* cell = static_cast<Cell*>(&cs);
    REQUIRE (&cs == cs.getPointer());
    REQUIRE (cell == cs.getPointer());
}

TEST_CASE ("CellString :: getDisplayableString()"){
    CellString cs1("\"string\"");
    REQUIRE (cs1.getDisplayableString() == "string");
    CellString cs2;
    REQUIRE (cs2.getDisplayableString() == "");
}

TEST_CASE ("CellString :: getConstructString()"){
    CellString cs1("\"string\"");
    REQUIRE (cs1.getConstructString() == "\"string\"");
    CellString cs2;
    REQUIRE (cs2.getConstructString() == "\"\"");
}


TEST_CASE ("CellString :: constructor (invalid input)"){
    REQUIRE_THROWS_AS (CellString("str"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellString("\"str"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellString("str\""), std::invalid_argument);
    REQUIRE_THROWS_AS (CellString("\\"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellString("\""), std::invalid_argument);
    REQUIRE_THROWS_AS (CellString(" "), std::invalid_argument);
    REQUIRE_THROWS_AS (CellString("\\\""), std::invalid_argument);
    REQUIRE_THROWS_AS (CellString(""), std::invalid_argument);
}

TEST_CASE ("CellString :: setValue (invalid input)"){
    CellString cs;
    REQUIRE_THROWS_AS (cs.setValue("a12"), std::invalid_argument);
    REQUIRE_THROWS_AS (cs.setValue("\"1a2"), std::invalid_argument);
    REQUIRE_THROWS_AS (cs.setValue("12a\""), std::invalid_argument);
    REQUIRE_THROWS_AS (cs.setValue("\\"), std::invalid_argument);
    REQUIRE_THROWS_AS (cs.setValue("\""), std::invalid_argument);
    REQUIRE_THROWS_AS (cs.setValue(" "), std::invalid_argument);
    REQUIRE_THROWS_AS (cs.setValue("\\\""), std::invalid_argument);
    REQUIRE_THROWS_AS (cs.setValue(""), std::invalid_argument);
}

TEST_CASE ("CellString :: change values (getDisplayableString)"){
    CellString cs;
    cs.setValue("\"str1\"");
    REQUIRE (cs.getDisplayableString() == "str1");
    cs.setValue("\"str2\"");
    REQUIRE (cs.getDisplayableString() == "str2");
    cs.setValue("\"!str3\"");
    REQUIRE (cs.getDisplayableString() == "!str3");
}

TEST_CASE ("CellString :: change values (getConstructString)"){
    CellString cs;
    cs.setValue("\"str1\"");
    REQUIRE (cs.getConstructString() == "\"str1\"");
    cs.setValue("\"\\\"");
    REQUIRE (cs.getConstructString() == "\"\\\"");
    cs.setValue("\"!str3\"");
    REQUIRE (cs.getConstructString() == "\"!str3\"");
}

