#include "catch_amalgamated.hpp"

#include "../ExcelProject/CellFormula.h"
#include "../ExcelProject/Cell.h"

double constexpr zero = 0.000000001;

TEST_CASE ("CellFormula :: constructor (Table*)"){
    Table t;
    CellFormula cf1(&t);
    REQUIRE (cf1.error() == true);
    REQUIRE (cf1.getValue() == 0);
}

TEST_CASE ("CellFormula :: constructor (Table*, std::string)"){
    Table t;
    CellFormula cf1(&t, "=");
    REQUIRE (cf1.error() == true);
    REQUIRE (cf1.getValue() == 0);

    CellFormula cf2(&t, "=5");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == 5);
}

TEST_CASE ("CellFormula :: getPointer()"){
    Table t;
    CellFormula cf(&t);
    Cell* cell = static_cast<Cell*>(&cf);
    REQUIRE (&cf == cf.getPointer());
    REQUIRE (cell == cf.getPointer());
}

TEST_CASE ("CellFormula :: constant expression calculation (set)"){
    Table t;

    CellFormula cf1(&t, "=5");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 5);

    CellFormula cf2(&t, "=5.5");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == 5.5);
}

TEST_CASE ("CellFormula :: constant expression calculation (set reference - empty)"){
    Table t;
    CellFormula cf(&t, "=A0");
    REQUIRE (cf.error() == false);
    REQUIRE (cf.getValue() == 0.0);
}

TEST_CASE ("CellFormula :: constant expression calculation (set reference - int)"){
    Table t;
    t.setCellValue(0, 0, "5");
    CellFormula cf(&t, "=A0");
    REQUIRE (cf.error() == false);
    REQUIRE (cf.getValue() == 5.0);
}

TEST_CASE ("CellFormula :: constant expression calculation (set reference - double)"){
    Table t;
    t.setCellValue(0, 0, "5.5");
    CellFormula cf(&t, "=A0");
    REQUIRE (cf.error() == false);
    REQUIRE (cf.getValue() == 5.5);
}

TEST_CASE ("CellFormula :: constant expression calculation (set reference - string)"){
    Table t;
    t.setCellValue(0, 0, "\"str\"");
    CellFormula cf(&t, "=A0");
    REQUIRE (cf.error() == false);
    REQUIRE (cf.getValue() == 0);
}

TEST_CASE ("CellFormula :: constant expression calculation (set reference - formula)"){

    Table t;

    t.setCellValue(0, 0, "=-2");
    CellFormula cf(&t, "=A0");
    REQUIRE (cf.error() == false);
    REQUIRE (cf.getValue() == -2);

    t.setCellValue(0, 0, "=2.5");
    CellFormula cf1(&t, "=A0");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 2.5);

    t.setCellValue(0, 0, "=");
    CellFormula cf2(&t, "=A0");
    REQUIRE (cf2.error() == true);

}

TEST_CASE ("CellFormula :: binary expression calculation (addition)"){
    Table t;
    CellFormula cf1(&t, "=3+5");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 8);

    CellFormula cf2(&t, "=3.3+5.9");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == 9.2);
}

TEST_CASE ("CellFormula :: binary expression calculation (subtraction)"){
    Table t;
    CellFormula cf1(&t, "=3-5");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == -2);
    CellFormula cf2(&t, "=5.9-3.3");
    REQUIRE (cf2.error() == false);
    REQUIRE (std::abs(cf2.getValue() - 2.6) < zero);
}

TEST_CASE ("CellFormula :: binary expression calculation (multiplication)"){
    Table t;
    CellFormula cf1(&t, "=3*5");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 15);
    CellFormula cf2(&t, "=3.3*5.9");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == 19.47);
}

TEST_CASE ("CellFormula :: binary expression calculation (division)"){
    Table t;
    CellFormula cf1(&t, "=3/5");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 0.6);

    CellFormula cf2(&t, "=5/0");
    REQUIRE (cf2.error() == true);
}

TEST_CASE ("CellFormula :: binary expression calculation (power)"){
    Table t;
    CellFormula cf1(&t, "=5^3");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 125);
    CellFormula cf2(&t, "=5^0");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == 1);
}

TEST_CASE ("CellFormula :: unary expression calculation (+)"){
    Table t;
    CellFormula cf1(&t, "=+5");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 5);
    CellFormula cf2(&t, "=+5.5");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == 5.5);
    CellFormula cf3(&t, "=+5-2");
    REQUIRE (cf3.error() == false);
    REQUIRE (cf3.getValue() == 3);
}

TEST_CASE ("CellFormula :: unary expression calculation (-)"){
    Table t;
    CellFormula cf1(&t, "=-5");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == -5);
    CellFormula cf2(&t, "=-5.5");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == -5.5);
    CellFormula cf3(&t, "=-5-2");
    REQUIRE (cf3.error() == false);
    REQUIRE (cf3.getValue() == -7);
}

TEST_CASE ("CellFormula :: order expression calculation (unary, +, -)"){
    Table t;

    CellFormula cf1(&t, "=+5+5");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 10);

    CellFormula cf2(&t, "=-5.5+5-5");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == -5.5);

    CellFormula cf3(&t, "=-5.5-5-5-5-5.5");
    REQUIRE (cf3.error() == false);
    REQUIRE (cf3.getValue() == -26);

    CellFormula cf4(&t, "=-5.5-5-5+5-5.5");
    REQUIRE (cf4.error() == false);
    REQUIRE (cf4.getValue() == -16);
}

TEST_CASE ("CellFormula :: order expression calculation (unary, *, /, +, -)"){
    Table t;

    CellFormula cf1(&t, "=5*5+6*6-7*7");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 12);

    CellFormula cf2(&t, "=5/5+2*2*2*2");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == 17);

    CellFormula cf3(&t, "=2*2*5/5");
    REQUIRE (cf3.error() == false);
    REQUIRE (cf3.getValue() == 4);

    CellFormula cf4(&t, "=-5/5+2*2*2*2");
    REQUIRE (cf4.error() == false);
    REQUIRE (cf4.getValue() == 15);
}

TEST_CASE ("CellFormula :: order expression calculation (^, *, /)"){
    Table t;

    CellFormula cf1(&t, "=-5^2");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == -25);

    CellFormula cf2(&t, "=2^3^4");  // should be interpreted as 2^(3^4)
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == 4096);

    CellFormula cf3(&t, "=2*3^2");
    REQUIRE (cf3.error() == false);
    REQUIRE (cf3.getValue() == 18);

    CellFormula cf4(&t, "=10/2^2");
    REQUIRE (cf4.error() == false);
    REQUIRE (cf4.getValue() == 2.5);

    CellFormula cf5(&t, "=2^2*3");
    REQUIRE (cf5.error() == false);
    REQUIRE (cf5.getValue() == 12);

    CellFormula cf6(&t, "=3^3/3");
    REQUIRE (cf6.error() == false);
    REQUIRE (cf6.getValue() == 9);
}

TEST_CASE ("CellFormula :: brackets expression calculation (*, /, +, -)"){
    Table t;

    CellFormula cf1(&t, "=5*(2+3)");
    REQUIRE (cf1.error() == false);
    REQUIRE (cf1.getValue() == 25);

    CellFormula cf2(&t, "=10/(2-3)");
    REQUIRE (cf2.error() == false);
    REQUIRE (cf2.getValue() == -10);

    CellFormula cf3(&t, "=(2+3)*2");
    REQUIRE (cf3.error() == false);
    REQUIRE (cf3.getValue() == 10);

    CellFormula cf4(&t, "=(2+3)/5");
    REQUIRE (cf4.error() == false);
    REQUIRE (cf4.getValue() == 1);
}


TEST_CASE ("CellFormula :: constructor (invalid input)"){
    Table t;
    REQUIRE_THROWS_AS (CellFormula(&t, "str"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellFormula(&t, "0="), std::invalid_argument);
    REQUIRE_THROWS_AS (CellFormula(&t, "\\="), std::invalid_argument);
    REQUIRE_THROWS_AS (CellFormula(&t, "5=="), std::invalid_argument);
    REQUIRE_THROWS_AS (CellFormula(&t, "3"), std::invalid_argument);
    REQUIRE_THROWS_AS (CellFormula(&t, ""), std::invalid_argument);
}

TEST_CASE ("CellFormula :: setValue (invalid input)"){
    Table t;
    CellFormula cf(&t);
    REQUIRE_THROWS_AS (cf.setValue("str"), std::invalid_argument);
    REQUIRE_THROWS_AS (cf.setValue("0="), std::invalid_argument);
    REQUIRE_THROWS_AS (cf.setValue("\\="), std::invalid_argument);
    REQUIRE_THROWS_AS (cf.setValue("5=="), std::invalid_argument);
    REQUIRE_THROWS_AS (cf.setValue("3"), std::invalid_argument);
    REQUIRE_THROWS_AS (cf.setValue(""), std::invalid_argument);
}
