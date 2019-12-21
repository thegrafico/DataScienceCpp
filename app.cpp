#include "pandas.h"

const string CVS_PATH = "swingStatesDF.csv";
//Prototipes
void menu();
void pressAnyKey();
void header(string message);
int getInt(string message, bool isMenu = false);
bool run(Pandas* dataframe);
string getString(string message);
void addToDataFrame(Pandas * df);

//Main app
int main() {

    Pandas df;

    bool isRunning = true;

    do {
        isRunning = run(&df);
    } while (isRunning);

    return 0;
}

//start the applicacion
bool run(Pandas* df) {
    bool isRunning = true;
    menu();
    //Select the option
    switch (getInt("Enter option number: ", true)) {
    case 1: // display dataframe
        df->display_all();
        break;
    case 2: // display an observation 
        header("Searching element by ID");
        df->displayByValueAndColumn(getString("Insert value to search: "), "id");
        break;
    case 3: // add an observation
        header("Adding to the dataframe");
        addToDataFrame(df);
        break;
    case 4: // delete an observation
        header("Deleting a record");

        if (df->removeRecordByValueAndColumn(getString("Insert the ID of the column: "), "id")) {
            cout << "Record removed!\n";
        } else {
            cout << "Can not remove the record\n";
        }
        break;
    case 5: // find observation by state
        header("Searching element by State");
        df->displayByValueAndColumn(getString("Insert value to search: "), "state");
        break;
    case 6: // find observation by county
        header("Searching element by County");
        df->displayByValueAndColumn(getString("Insert value to search: "), "county");
        break;
    case 7: // Save observation to file
        header("Saving the new CVS");
        df->saveToCSV("Votes.csv");
        break;
    case 8: // Load observation from file
        header("Reading the file!");
        df->read_csv(CVS_PATH);
        break;
    case 9: // Create histogram
        header("Display histogram");
        df->displayHistogram(getString("Insert the column to display: "));
        break;
    case 10: // Calculate Measures of central tendency 
        header("Calculating the Central Tendency");
        df->calculate_central_tendecy(getString("Insert the column: "), false);
        break;
    case 11: // display dataframe
        return false;
        break;
    default:
      /*  df->saveColumnToNewFile("total_votes.csv","total_votes");
        df->saveColumnToNewFile("dem_share.csv", "dem_share");
        df->saveColumnToNewFile("rep_votes.csv", "rep_votes");
        df->saveColumnToNewFile("dem_share.csv", "dem_share");*/
        cout << "Invalid option \n";
    }
    pressAnyKey();
    return isRunning;
}

void menu() {
    cout << "CoRe Data Analyzer APPLICATION\n"
        << "-----------------------------\n"
        << "1.  Display Data Frame\n"
        << "2.  Display an observation\n"
        << "3.  Add an observation\n"
        << "4.  Delete an observation\n"
        << "5.  Find observation by state\n"
        << "6.  Find observation by county\n"
        << "7.  Save observation to file\n"
        << "8.  Load observations from file\n"
        << "9.  Create Histogram\n"
        << "10. Calculate Measures of Central Tendency\n"
        << "11. Exit\n"
        << "-----------------------------\n";
}

// Get the user input to choose the option
int getInt(string message, bool isMenu) {

    string userInput = "";
    int option = -1; //-1 default

    //Try catch for string inputs entered by the user
    try {
        cout << message;
        cin >> userInput;
        option = stoi(userInput);
        cout << endl;
    } catch (const exception & e) {
        cout << "Invalid option, Please enter a number\n";
        return getInt(message);
    }
    //Validate the range of the option entered by the user
    if (isMenu) {
        if (option < 1 || option > 11) {
            cout << "Invalid option, Please enter a valid option from the menu\n";
            return getInt(message);
        }
    }

    cin.ignore();
    return option;
}

void header(string message) {
    cout << "------------------------------------------------------\n";
    cout << message << endl;
    cout << "------------------------------------------------------\n";
}
string getString(string message) {
    string line;
    cout << message;

    getline(cin, line);

    cout << endl;

    if (line.size() == 0) {
        cout << "Empty value, try again" << endl;
        return getString(message);
    }
    cin.clear();
    //cout << line << endl;
    return line;
}

void addToDataFrame(Pandas * df) {
    vector<string> columns = df->getColumns();
    vector<string> newRow;
    string temp;
    string tempMessage;
    
    for (int i = 0; i < columns.size(); i++) {
        tempMessage = "Insert " + columns[i] + ": ";
        temp = getString(tempMessage);
        newRow.push_back(temp);
    }

    if (df->addRecordInColumnOrder(newRow)) {
        cout << "Value added!" << endl;
    } else {
        cout << "Can not add the value\n";
    }
}
//Press enter key to continue
void pressAnyKey() {
    cout << "\n<PRESS ENTER TO CONTINUE>\n\n";
    string temp;

    cin.clear();
    cin.ignore();
    getline(cin, temp);
}
