#ifndef CELL_DOUBLE_H
#define CELL_DOUBLE_H

#include <iostream>
#include "Cell.h"

/** CellDouble is a class which extends the abstract class \ref Cell
 *  Every instance of this class takes care of 1 double value
 *  \n This class allows:
 *  \li change current double value - \ref setValue
 *  \li get current value as a double - \ref getValue
 *  \li get string representing current value - \ref getDisplayableString and \ref getConstructString
 *  \li clone this object - \ref clone
 */
class CellDouble : public Cell{
private:

    /**< Holds the double value this object actually respresents */
    double double_;

    /** Given a string which represents a floating number, this function removes
     * all the unnecessary zeroes after the floating point (including the floating point
     * point itself if the number is integer.
     *
     * \param string to be trimmed
     */
    void trimZeroes(std::string& str);

public:

    /** Empty constructor. Has default value of 0.0
     */
    CellDouble();

    /** Constructor which builds an object with an initial value
     *  based on the given string
     *
     * \exception InvalidArgument Thrown if the given string does not represent a floating number
     * \param string which should represent a floating number
     */
    CellDouble(const std::string& value);

    /** Copy constructor
     *  \param object of type CellDouble to copy from
     */
    CellDouble(const CellDouble& copy);

    /** Sets (or changes) the current saved value (floating number) of this instance.
     * The method takes a string, casts it to double type and keeps the new value.
     *
     * \exception InvalidArgument Thrown if the given string does not represent a floating number
     * \param string which should represent a floating number
     */
    void setValue(const std::string& value);

    /** Every instance of this class holds a value of type double. This method returns it.
     *
     *  \return Current instance's hold value of type double
     */
    double getValue();

    /** As a child of Cell, this method can be called by a Cell pointer to get
     *  the direct pointer to instance of this class.
     *
     *  \return Pointer to the current instance of this class
     */
    CellDouble* getPointer();

    /** \return Casts current value into a string. String is formatted, so that
     *  it can be displayed in best possible way. This means:
     *  \li 1) without extra (not meaningful) zeroes in the beginning of the whole part
     *  of the number or at the end of its floating part
     *  \li 2) does not display the floating part if it's equal to 0
     */
    std::string getDisplayableString();

    /** \return Last string used to successfully change this object's value
     */
    std::string getConstructString();

    /** Checks whether a string represents a valid floating number.
     *  \exception out_of_range if the provided string is too large to fit in a double -
     *  \link check https://www.cplusplus.com/reference/string/stoi/
     */
    bool isValid(const std::string& value);

    /** Clones (creates an exact copy) of the current instance of this class (CellDouble).
     *  \return Allocated pointer to the newly created copy.
     */
    Cell* clone() const;

};


#endif // CELL_DOUBLE_H
