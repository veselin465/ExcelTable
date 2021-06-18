#ifndef CELL_INT_H
#define CELL_INT_H

#include <iostream>
#include "Cell.h"

/** CellInt is a class which extends the abstract class \ref Cell
 *  Every instance of this class takes care of 1 integer value
 *  \n This class allows:
 *  \li change current int value - \ref setValue
 *  \li get current value as a int - \ref getValue
 *  \li get string representing current value - \ref getDisplayableString and \ref getConstructString
 *  \li clone this object - \ref clone
 */

class CellInt : public Cell{
private:
    int int_;
public:

    /** Empty constructor. Has default value of 0
     */
    CellInt();

    /** Constructor which builds an object with an initial value
     *  based on the given string
     *
     *  \exception invalid_argument Thrown if the given string does not represent an integer
     *  \param string which should represent an integer
     */
    CellInt(const std::string& value);

    /** Copy constructor
     *  \param object of type CellInt to copy from
     */
    CellInt(const CellInt& copy);

    /** Sets (or changes) the current saved value (integer type) of this instance.
     *  The method takes a string, casts it to int type and keeps the new value.
     *
     *  \exception InvalidArgument Thrown if the given string does not represent an integer
     *  \param string which should represent an integer
     */
    void setValue(const std::string& value);

    /** Every instance of this class holds a value of type int. This method returns it.
     *
     *  \return Current instance's hold value of type int
     */
    int getValue();

    /** As a child of Cell, this method can be called by a Cell pointer to get
     *  the direct pointer to instance of this class.
     *
     *  \return Pointer to the current instance of this class
     */
    CellInt* getPointer();

    /** \return Casts current value into a string. String is formatted, so that
     *  it can be displayed in best possible way. This means:
     *  \li 1) without extra (not meaningful) zeroes in the beginning of the whole part
     */
    std::string getDisplayableString();

    /** \return Last string used to successfully change this object's value
     */
    std::string getConstructString();

    /** Checks whether a string represents a valid integer.
     *  \exception out_of_range if the provided string is too large to fit in an int -
     *  \link check https://www.cplusplus.com/reference/string/stoi/
     */
    bool isValid(const std::string& value);

    /** Clones (creates an exact copy) of the current instance of this class (CellInt).
     *  \return Allocated pointer to the newly created copy.
     */
    CellInt* clone() const;

};


#endif // CELL_INT_H

