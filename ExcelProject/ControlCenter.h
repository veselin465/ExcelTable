#ifndef CONTROL_CENTER_H
#define CONTROL_CENTER_H

#include <iostream>
#include <vector>

#include "Table.h"

/** ControlCenter is a class which aims to centralize all the main logic of this app.
 *  An instance of this class is required in order to work with the functionality
 *  of class \ref Table in the most user friendly way
 *  \n This class can execute a command based on a provided string - \ref executeCommand
 */

class ControlCenter{

private:

    /** If a fail is opened, this variable checks whether local changes have been made
     *  without save
     */
    bool upToDate;

    /** Holds the path to the last opened file
     */
    std::string filePath_;

    /** Holds an instance of class \ref Table. All commands
     *  are based on the provided functionality by this object.
     */
    Table currentTable;

    /** Given a string, this method splits it by whitespace where
     *  \li Quotes indicate that anything inside them should be considered as a whole.
     *  Does not include the quotes themselves.
     *  \li many white spaces in a sequence still count as one separator white space
     *
     *  \exception invalid_argument Thrown whenever an invalid command is provided. This includes:
     *  \li Unsupported and unknown command
     *  \li Command with wrong count of arguments after it
     *  \li Odd count of quotes
     */
    std::vector <std::string> splitWithQuotes(const std::string& commandLine);

    /** Makes the user to confirm its most recent action. Usually used to warn
     *  the user that their action might have result, which does not satisfy
     *  the user themselves.
     *
     *  \param 2 char parameters which should be considered as yes and no respectively
     */
    bool confirmAction(char yes, char no);

    /** Given a string, changes all lower case English chars inside into upper case
     *
     *  \param string to be turned into upper case
     */
    void stringToUpper(std::string& str);

    /** Checks whether a file exist by providing its relative or absolute location
     *
     *  \param path or filename to check
     *  \return whether the file exists or not
     */
    bool fileExist(const std::string& filename);

    /** Checks whether a string has acceptable format. For this app, this means
     *  to end on '.csv' and have non-empty filename
     *
     *  \param path or filename to check
     *  \return whether the file is from valid format or not
     */
    bool checkFormat(const std::string& str);

    /** Creates an empty file with preset path or filename. Also warns the user if
     *  file with such a path or filename exists.
     *
     *  \exception invalid_argument unsupported file format
     *  \param path or filename of the file to be created
     */
    void createEmptyFile(const std::string& filename);

    /** Similar to other text editor's close file functionality, this method also:
     *  \li checks whether there's actually a file to be closed
     *  \li remind the user to save if it means that they would lose local changes otherwise - \ref confirmAction
     *
     *  \exception invalid_argument unsupported file format
     *  \param path or filename of the file to be created
     */
    void closeFile();

    /** Reads table cell values from csv file, where every rows means new table row and
     *  every comma means start of a new column. The information is saved into a local instance
     *  of class Table in this class
     *
     *  \exception invalid_argument unsupported file format
     *  \exception invalid_argument file not found
     *  \exception invalid_argument process was interrupted, leading to it being unsuccessful
     *  \param path or filename of the file to be created
     */
    void loadFromFile(const std::string& filename);

    /** Saves the current state of local instance of class Table to a file of wanted path or
     *  filename, where before processing to action, checks whether file with such a path
     *  or filename exists and if it does, informs the user and waits for its confirmation or disallowing
     *  of continuing the process.
     *
     *  \exception invalid_argument unsupported file format
     *  \exception invalid_argument Unexpected error while processing to write into the file.
     *  \param path or filename of the file to be created
     */
    void saveToFile(const std::string& filename);



public:
    /** Empty constructor
     */
    ControlCenter();

    /** Given a command, this function tries to execute it. 3 Possible outcomes:
     *  \li command execution was successful, but had no purpose of giving feedback,
     *  so it returns empty string
     *  \li command execution was successful and it gives a feedback or displayable
     *  information related with the executed command
     *  \li command execution was not successful, so it throws an exception
     *  any or does not catch any thrown exception
     *
     *  \exception invalid_argument Thrown to signal that the request failed
     */
    const std::string executeCommand(const std::string& commandLine);

};


#endif // CONTROL_CENTER_H

