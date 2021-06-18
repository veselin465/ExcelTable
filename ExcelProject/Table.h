#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include "Cell.h"
#include "Cell.h"

/** Table is a class which takes care of a collection of objects of abstract type \ref Cell
 *  Table holds a 2-dimensional dynamic array of pointers to Cell.
 *  Using polymorphism, it can create objects of any class type which extends the abstract class Cell.
 *  Currently, only 4 child-classes are being supported:
 *  \li CellInt
 *  \li CellDouble
 *  \li CellString
 *  \li CellFormula
 *  \n This class allows:
 *  \li allocates an object extending Cell based on a provided string and associate it with a
 *  2-dimensional coordinates (position) - \ref setCellValue
 *  \li delete an existing object on a wanted position - \ref deleteCellValue
 *  \li delete all objects ever created by this class - \ref resetTable
 *  \li get displayable and constructable string on a wanted position -
 *    \ref getDisplayableCellValue and \ref getConstructedCellValue
 *  \li Checks whether a cell is inside table's borders - \ref isCellInsideTable
 *  \li get direct access to pointer of the appropriate class a cell is created by - \ref getCellPointer
 *  \li prints the entire table this class holds in an appropriate way - \ref print
 *  \li get row and column max count the table has ever reached - \ref rowsCount and \ref columnsCount
 */

class Table{
private:

    /** 2-dimensional dynamic array of Cell pointers */
    Cell*** table_;

    /** Allocated count of rows in this table */
    size_t rowsCount_;

    /** Allocated count of column in this table */
    size_t columnsCount_;

    /** Allocates dynamic 2-dimensional array of type Cell pointer (Cell*).
     *
     * \param row allocated new dynamic array's count of rows
     * \param column allocated new dynamic array's count of columns (count of elements in every row)
     * \return Pointer to the newly allocated 2-dimensional dynamic array of type Cell*.
     *  Exception Safety. If an exception occurs, the function handles it and returns nullptr.
     */
    Cell*** allocateTable(std::size_t row, std::size_t column);

    /** Release any dynamically allocated memory
     *  associated with private member: Cell*** tableData
     */
    void releaseTableData(bool fullDataClear);

    /** Extends table up to given new values for rows and columns.
     *  Shrinking is not possible in neither dimension.
     *  \param new rows and columns count
     */
    void extendTable(size_t rows, size_t columns);

    /** Takes a string and centers it based on wanted length and fills the whitespace with a wanted char
     *  \exception invalid_argument thrown if new length is smaller than the length of the string to be centered
     *  \param result centered string
     *  \param value string to be centered
     *  \param length new wanted lenth
     *  \param filling what char to fill the whitespace when centering the string
     *  \note example:
     *  \code {.cpp}
     *  getCenteredString(std::string& result, "some example", 20, '-');
     *  // result now has this value: "----some example----"
     *  \endcode
     */
    void getCenteredString(std::string& result, const std::string& value, size_t length, char filling);

public:

    /** Empty constructor. Table has default size of 1x1
     */
    Table();

    /** Constructor which takes initial table size (rows count and columns count)
     *  \exception invalid_argument thrown if either initial rows count, or initial columns count is zero
     */
    Table(size_t rows, size_t cols);

    /** Operator= which created perfect copy (with copied allocated memory) of an object
     */
    Table& operator=(const Table& other);

    /** Copy constructor which created perfect copy (with copied allocated memory) of an object
     */
    Table(const Table& copy);

    /** Destructor which makes sure to free any allocated dynamic memory by this class
     */
    ~Table();

    /** Checks whether a cell is inside or outside of table's size borders
     */
    bool isCellInsideTable(size_t row, size_t column) const;

    /** Find all allocated objects of type \ref CellFormula and calls its public member fucntion
     *  \ref recalculate
     */
    void recalculateAllFormulas();

    /** Creates a new dynamically allocated cell of proper type based on the provided string and
     *  associates it with 2-dimensional coordinates, respectively row and column
     *
     *  \exception invalid_argument Thrown if provided string does not represent any valid and
     *  and supported class type
     *  \param row the row this new cell will be associated with
     *  \param column the column this new cell will be associated with
     *  \param value the value this new cell will be associated with
     */
    void setCellValue(size_t row, size_t column, const std::string& value);

    /** Deletes any allocated dynamic memory associated by a cell on the provided row and column
     *
     *  \param wanted position (row and column)
     */
    void deleteCellValue(size_t row, size_t column);

    /** Deletes any allocated dynamic memory associated by this class. Automatically sets the
     *  table to size 1x1 and ensures it can still be used right after this command is called
     */
    void resetTable();

    /** Tries to get the displayable value of a cell on position row and column.
     *  If found, returns it. If not, returns empty string.
     */
    std::string getDisplayableCellValue(size_t row, size_t column);

    /** Tries to get the constructed value of a cell on position row and column.
     *  If found, returns it. If not, returns empty string.
     */
    std::string getConstructedCellValue(size_t row, size_t column);

    /** Tries to find the cell on position row and column.
     *  If found, returns its pointer. If not, returns null pointer.
     */
    const Cell* getCellPointer(size_t row, size_t column) const;

    /** \return The entire table gets convented into a string, which can be displayed.
     *  The string represents the current table formatted in a readable way.
     *  Takes the displayable string of all existing cells in this class
     */
    std::string print();

    /** \return Count of rows in this this table
     */
    size_t rowsCount();

    /** \return Count of columns in this this table
     */
    size_t columnsCount();

    /** Static function, which takes a typical excel representation of a position of a cell
     *  and returns the row it actually refers to as a positive integer.
     */
    static size_t getRow(const std::string& pos);

    /** Static function, which takes a typical excel representation of a position of a cell
     *  and returns the column it actually refers to as a positive integer.
     */
    static size_t getColumn(const std::string& pos);

    /** Function, which takes a typical excel representation of a position of a cell.
     *  If found returns a pointer to the cell this position refers to in the table
     *  If not found, returns a null pointer.
     */
    const Cell* getCellByString(const std::string& pos);

};


#endif // TABLE_H


