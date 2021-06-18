#ifndef CELL_H
#define CELL_H

#include <iostream>
/** Cell is an completely abstract class. Its purpose is to link all children-classes which extend this class.
 *  Every child of Cell holds a specific type of information unique for the given class.
 *  \par Additionally, every child of Cell has its own string format to represent this specific information.
 *  Ideally, those formats should not share common interface,
 *  unless one is a greater version of another (like integer and floating number).
 */
class Cell{
public:

    /** Returns pointer to the current instance of the object (which will be a child of this class).
     * Ideally, this method should be used to easily get the pointer to the child instance when dealing with polymorphism
     *
     * \return returns a pointer to the current instance of this object
     */
    virtual Cell* getPointer() = 0;

    /** Dynamically allocates a copy of the the current instance of the object (which will be a child of this class)
     *
     * \return returns a pointer to the instance of the newly dynamically created object
     */
    virtual Cell* clone() const = 0;

    /** Every instance of a class-child of this class should return a string which will be
     * associated with the current class-child
     *
     * \return returns this string
     */
    virtual std::string getDisplayableString() = 0;

    /** Every instance of a class-child of this class should return a string which will be
     * associated with the current class-child
     *
     *  \return returns this string
     */
    virtual std::string getConstructString() = 0;

    /** Every instance of a class-child of this class should hold information of some type.
     *  This information can be given as a string by calling this function.
     *  The resulted string is formatted by default in an appropriate way to be displayed.
     *
     *  \param initial information of the current instance of the class-child
     */
    virtual void setValue(const std::string& value) = 0;

    /** Every instance of a class-child of this class should define a string format, which
     *  tells whether a string should be considered a possible part of the child
     *  The string itself might have problems in further calculations, but isValid should
     *  be able to define whether a string fits the required format for the child class
     *
     *  \param string to be checked
     *  \return boolean value answering the question whether the string is a part of the child-class family
     */
    virtual bool isValid(const std::string& str) = 0;

    /** Empty virtual destructor. Used to avoid future undefined behavior when dealing with extending.
     */
    virtual ~Cell();
};


#endif // CELL_H
