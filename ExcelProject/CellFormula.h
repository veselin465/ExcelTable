#ifndef CELL_FORMULA_H
#define CELL_FORMULA_H

#include <iostream>
#include "Cell.h"
#include "Table.h"


/** CellFormula is a class which extends the abstract class \ref Cell
 *  Every instance of this class takes care of one formula.
 *  A formula represents an algebraic expressing, whole return value is a floating number.
 *  \n A formula consists of operands (real numbers), unary operators (+,-) and binary ones (+,-,*,/,^)
 *  \n This class allows:
 *  \li set and change current formula - \ref setValue
 *  \li calculate the value this formula has - automatically calculated when setValue is called
 *  or calculated using the last remember formula when called \ref recalculate
 *  \li get current value as a double - \ref getValue
 *  \li get string representing current value - \ref getDisplayableString and \ref getConstructString
 *  \li clone this object - \ref clone
 */
class CellFormula : public Cell{
private:

    /** private constant representing an extremely low positive floating number. Used to indicate whether
     *  a floating number is so close to zero, that it can be freely considered as a zero
     */
    static constexpr double zero_ = 0.0000001;

    /** A formula might have a reference to another cell in the current or another table. In order to
     *  find the information this class needs of, a reference to the table should be provided.
     */
    const Table* tableRef_;

    /** Holds an unchanged copy of the last entered formula.
     */
    std::string formula_;

    /** Holds the last calculated result in a formula, so that it does not need to calculate it again
     */
    double result_;

    /** If the last calculated result was unsuccessful, this flag should be set to true.
     */
    bool error_ = false;

    /** Removes every whitespace inside the string. Used to provide the function which
     *  calculates the formula a valid string
     *  \n Used by \ref calculateFormula
     *  \param String to be trimmed.
     */
    void trimEntirely(std::string& str);

    /** Finds the first occurrence of a wanted char.
     *  \n Used by \ref calculateFormulaRecursively
     *
     *  \param String to search the wanted char.
     *  \param A char array of wanted strings (will return the first time it finds at least one match) (follows array's priority)
     *  \param Count of elements in the char array
     */
    int seekFirstOperation(const std::string& str, char ch[], size_t charCount);

    /** Finds the first occurrence of a wanted char.
     *  \n Used by \ref calculateFormulaRecursively
     *
     *  \param String to search the wanted char.
     *  \param A char array of wanted strings (will return the first time it finds at least one match) (follows array's priority)
     *  \param Count of elements in the char array
     */
    int seekLastOperation(const std::string& str, char ch[], size_t charCount);

    /** Given a position in 2D coordinates (row and column), returns the value on that position according to
     *  the provided table. The following calculate dependencies apply:
     *  \li 1) empty cell (or one outside of the provided table) is considered 0
     *  \li 2) string cells are considered as 0, even if they have number value
     *  \li 3) formula cells with errors share the error in this formula as well
     *  \li 4) int, double and formula without error gives right away the value they hold
     *
     *  \param Wanted row to get value from
     *  \param Wanted column to get value from
     *  \return wanted result as casted to a floating number
     */
    double extractCellValue(size_t row, size_t column);

    /** Given a string with an algebraic expression, this function analyzes it,
     *  determines operation priorities and smartly divide the problem into 2
     *  smaller problems which are again sent to this function to calculate them until
     *  an operand is reached. The following algorithm is applied
     *  \li 1) Start searching operations, where it first searches low priority, then high priority ones
     *  \li 1.1) When found, the entire string is split into 2 halves, without the operation itself
     *  \li 2) Then calls the algorithm on the 2 separated parts, gets the value from them and
     *  \li performs the appropriate operation on the parts
     *  \note Uses mathematically proven and tested methods to get the correct answer
     *  NOTE: This recursion depends on certain string rules to work properly, like:
     *  \li 1) provided string is entirely whitespace trimmed - \ref trimEntirely
     *  \li 2) if provided string contains references in table, they need to be calculated - \ref extractCellValue
     *  \li 3) used operands and operations are appropriately used (otherwise throws exception)
     *
     *  \exception invalid_argument - thrown if the expression is invalid. Can be thrown if:
     *  \li brackets without any expression in it is contained in the expression
     *  \li An operation does not have the required number of operands
     *
     *  \param expression to calculate
     *  \return wanted result casted to a floating number
     */
    double calculateFormulaRecursively(const std::string& value);

    /** Given a string with an algebraic expression, this function makes sure the
     *  expression is correct and properly formatted, so that it can call the following recursive
     *  function \ref calculateFormulaRecursively can use its result as an wanted result.
     *
     *  \exception invalid_argument - thrown if the expression is invalid. This includes:
     *  wrong use of brackets (unequal count of opening and closing ones as well as having
     *  more closing than opening brackets at some time
     *
     *  \param expression to calculate
     *  \return wanted result casted to a floating number
     */
    double calculateFormula(const std::string& value);

    /** Given a string which is supposed to represent a floating number, this function changes it,
     *  so that it does not have and exceed zeroes at the end of the floating part. Does not
     *  check whether the string is actually a floating number.
     */
    void trimZeroes(std::string& str);

public:

    /** Constructor which takes pointer to the table this formula is supposed to take all
     *  the required information if a reference to a cell is given.
     *  Automatically sets the error flag to true and sets default value to the result (0)
     *
     *  \exception invalid_argument - if the Table pointer is null.
     *  \param pointer to the Table class
     */
    CellFormula(const Table* tableRef);

    /** Constructor which takes pointer to the table this formula is supposed to take all
     *  the required information if a reference to a cell is given.
     *  Additionally, takes an initial string and tries to calculate it. If fails, sets the
     *  error flag to true and sets default value to the result (0) - \ref setValue
     *
     *  \exception invalid_argument - if the Table pointer is null.
     *  \param pointer to the Table class
     *  \param initial value
     */
    CellFormula(const Table* tableRef, const std::string& value);

    /** Copy constructor
     *  \param object of type CellFormula to copy from
     */
    CellFormula(const CellFormula& copy);

    /** Every instance of this class holds a value of type double. This method returns it.
     *
     * \return Current instance's hold value of type double
     */
    void setValue(const std::string& value);

    /** \return last calculated result of the formula
     */
    double getValue();

    /** \return recalculates last remembered formula
     */
    void recalculate();

    /** As a child of Cell, this method can be called by a Cell pointer to get
     *  the direct pointer to instance of this class.
     *
     *  \return Pointer to the current instance of this class
     */
    CellFormula* getPointer();

    /** \return Result of the formula formatted in a good to way to be displayed. This includes:
     *  \li 1) without extra (not meaningful) zeroes in the beginning of the whole part
     *  of the number or at the end of its floating part
     *  \li 2) does not display the floating part if it's equal to 0
     */
    std::string getDisplayableString();

    /** \return last remembered raw formula
     */
    std::string getConstructString();

    /** Returns whether the entered formula was successfully calculated.
     */
    bool error();

    /** Checks whether a string represents a correct formula format. It still can have error, though.
     */
    bool isValid(const std::string& value);

    /** Clones (creates an exact copy) of the current instance of this class (CellFormula).
     * \return Allocated pointer to the newly created copy.
     */
    CellFormula* clone() const;

};


#endif // CELL_FORMULA_H


