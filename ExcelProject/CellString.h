#ifndef CELL_STRING_H
#define CELL_STRING_H

#include <iostream>
#include "Cell.h"

/** CellString is a class which extends the abstract class \ref Cell
 *  Every instance of this class takes care of 1 string value (object of type std::string)
 *  \n This class allows:
 *  \li change current int value - \ref setValue
 *  \li get current value as a int - \ref getValue
 *  \li get pure string value of current string - \ref getDisplayableString
 *  \li clone this object - \ref clone
 */

class CellString : public Cell{
private:

    std::string string_;

public:

    /** Empty constructor. Has default value of ""
     */
    CellString();

    /** Constructor which builds a string with an initial value
     *
     *  \exception invalid_argument Thrown if the given string does not represent a valid string.
     *  Rules determining which strings are considered as valid ones are described in \ref isValid
     *  \param string which should represent an integer
     */
    CellString(const std::string& value);

    /** Copy constructor
     *  \param object of type CellString to copy from
     */
    CellString(const CellString& copy);

    /** Sets (or changes) the current saved value of the string.
     *
     *  \exception invalid_argument Thrown if the given string does not represent a valid string.
     *  Rules determining which strings are considered as valid ones are described in \ref isValid
     *  \param string which should represent a valid string
     */
    void setValue(const std::string& value);

    /** As a child of Cell, this method can be called by a Cell pointer to get
     *  the direct pointer to instance of this class.
     *
     *  \return Pointer to the current instance of this class
     */
    CellString* getPointer();

    /** \return the string this object holds
     */
    std::string getDisplayableString();

    /** \return string formatted in a way, that it has the same value
     *  as the string which is hold in this object, but it is also considered valid,
     *  according to validation system of this class.
     */
    std::string getConstructString();

    /** Checks whether a string represents a valid string according to the validation standarts
     *  of this class. A string is considered valid if:
     *  It is surrounded by double quotes (")
     */
    bool isValid(const std::string& value);

    /** Clones (creates an exact copy) of the current instance of this class (CellInt).
     * \return Allocated pointer to the newly created copy.
     */
    CellString* clone() const;

};


#endif // CELL_STRING_H

