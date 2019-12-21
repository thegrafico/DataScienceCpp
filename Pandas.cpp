#include "pandas.h"

Pandas::Pandas() {
    cout << "Default constructor" << endl;
}

Pandas::~Pandas() {
    dataframe.clear();
    col_value.clear();
    column_types.clear();

    vector<vector<string>>().swap(dataframe);
    vector<string>().swap(col_value);
    vector<char>().swap(column_types);
}

void Pandas::read_csv(string filepath, char delimiter) {

    ifstream df; // read the file 
    string line; // complete row value 
    string item; // individual column value

    setSize(filepath);

    if (SIZE < 1) {
        cout << "File is empty" << endl;
        return;
    }

    //open the file
    df.open(filepath, ios::out | ios::in);

    if (!df.is_open()) {
        cout << "Unable to open the file: " << filepath << endl;
        return;
    }

    fileIsLoaded = true;

    int i = 0;
    while (getline(df, line, '\n') && (SIZE) > i) {

        stringstream record(line);

        while (getline(record, item, delimiter)) {
            // cout << item<< endl;
            col_value.push_back(item);
        }

        dataframe.push_back(col_value);
        col_value.clear();
        i++;
    }
    df.close();
    setColumnsTypes();
    cout << "Completed!\n";
}

void Pandas::setSize(string filepath) {
    ifstream df;
    string line;
    int column_size = 0;

    df.open(filepath, ios::out | ios::in);

    if (!df.is_open()) {
        cout << "Unable to open file";
        return;
    }

    while (getline(df, line, '\n')) {
        if (line.size() > 1) {
            column_size++;
        }
    }
    df.close();
    SIZE = column_size;
}

int Pandas::getSize() {

    if (!fileIsLoaded) return 0;

    return SIZE;
}

//Display the header
void Pandas::displayHeader(int width) {
    if (fileIsLoaded) {
        cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
        for (int i = 0; i < dataframe[COLUMNS_INDEX].size(); i++) {
            width = (i < 2) ? 10 : 25;
            cout << setw(width) << dataframe[COLUMNS_INDEX][i];
        }
        cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    }
    else {
        cout << "Need to load a file" << endl;
    }
}

//display value
void Pandas::display_all() {
    if (fileIsLoaded) {
        int width = 20;
        displayHeader(width);

        for (int i = 1; i < dataframe.size(); i++) {
            for (int j = 0; j < dataframe[i].size(); j++) {
                width = (j < 2) ? 10 : 25;
                cout << setw(width) << dataframe[i][j];
            }
            cout << endl;
        }
        cout << "\n------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    }
    else {
        cout << "Need to load a file" << endl;
    }
}
// 
void Pandas::displayByValueAndColumn(string valueToSearch, string column) {

    if (!fileIsLoaded) return;

    if (dataframe.size() < 1) { cout << "Not value for iteration\n"; return;}

    if (valueToSearch.empty() || column.empty()) return;
    
    int col = getColumnIndex(column);

    //if can not find the id record
    if (col == ERROR_NUMBER) {
        cout << "Can't find the record " << endl;
        return;
    }

    bool found = false;
   
    for (int i = 1; i < dataframe.size(); i++) {
        if (to_lower(dataframe[i][col]) == to_lower(valueToSearch)) {

            string _col, _value;
            int width = 20;
            
            //Just to display the header one time
            if(!found)
                displayHeader(width);

            for (int j = 0; j < dataframe[i].size(); j++) {
                width = (j < 2) ? 10 : 25;
                cout << setw(width) << dataframe[i][j];
            }
            cout << endl;
            found = true;
            //break;
        }
    }
    //if can not find the id record
    if (!found) {
        cout << "Can't find the record |column: '" << column << "', Value: " << valueToSearch << endl;
        return;
    }
}

bool Pandas::addRecordInColumnOrder(vector<string> values) {

    if (!fileIsLoaded) return false;

    bool wasAdded = true;
    if (values.empty()) {
        cout << "Can not add empty values\n";
        return !wasAdded;
    }else if (values.size() > dataframe[0].size()) {
        cout << "To many values for add\n";
        return !wasAdded;
    } else if (values.size() != column_types.size()) {
        cout << "Not enough values to add\n";
        return !wasAdded;
    }

    regex double_regex(number_regex_pattern);
    int isNumber = 0;
    char type;

    //Validate values
    for (int i = 0; i < values.size(); i++) {

        //if the value that the user enter ( 1 for number, 0 for string)
        isNumber = regex_match(values[i], double_regex);
        type = column_types[i];
        
        //Validate the type of number
        if (type == 'd' && !isNumber) {
            values[i] = "0";
        }
    }
    dataframe.push_back(values);
    return wasAdded;
}
//Remove an element by column name and identifier. 
bool Pandas::removeRecordByValueAndColumn(string identifier, string column) {
    
    //Validate values
    if (!fileIsLoaded || dataframe.empty() || column.empty() || identifier.empty()) return false;

    //get column index
    int col_indx = getColumnIndex(column);

    //can not find the column
    if (col_indx == ERROR_NUMBER) return false;

    for (int i = 0; i < dataframe.size(); i++) {
        if (to_lower(dataframe[i][col_indx]) == to_lower(identifier)) {
            dataframe.erase(dataframe.begin() + i);
            return true;
        }
    }
    return false;
}
// set the types of all columns, string o double
void Pandas::setColumnsTypes() {

    if (!fileIsLoaded) return;

    column_types.clear();
    bool isNumber = true;
    char type;
    
    regex double_regex(number_regex_pattern);
    
    for (int i = 0; i < dataframe[COLUMNS_INDEX].size(); i++) {
   
        for (int j = 1; j < dataframe.size(); j++) {

            // if the regex if alse, set string by default
            if (!regex_match(dataframe[j][i], double_regex)) {
                // cout << "\t\tThe column '" << dataframe[0][i] << "' is string with the value '" << dataframe[j][i] << "'\n";
                isNumber = false;
                break;
            }
        }
        //possible types (s = string, d = double)
        type = !isNumber ? 's' : 'd';

        column_types.push_back(type);

        isNumber = true;
    }
}

//get the values in double type
vector<double> Pandas::getColumnValues_D(string column) {
    
    if (!fileIsLoaded) return {};

    int col_indx= getColumnIndex(column);

    if (col_indx == ERROR_NUMBER) {
        cout << "column not found\n";
        return {};
    }
    vector<double> temp_val;
    
    try {
        for (int i = 1; i < dataframe.size(); i++) {
            temp_val.push_back(stod(dataframe[i][col_indx]));
        }
    } catch (const std::exception&) {
        cout << "There is an error trying to cast the value to a number" << endl;
        return {};
    }
    //cout << "Values returned" << endl;
    return temp_val;
}

//Get all the values in string
vector<string> Pandas::getColumnValues_S(string column) {
    if (!fileIsLoaded) return {};

    int col_indx = getColumnIndex(column);
    vector<string> temp_val;
 
    for (int i = 1; i < dataframe.size(); i++) {
        temp_val.push_back(dataframe[i][col_indx]);
    }    
    //cout << "Values returned" << endl;
    return temp_val;
}

//save the data to a new CSV
void Pandas::saveToCSV(string name, char delimiter) {

    if (!fileIsLoaded) return;

    ofstream tempFile; // read the file 
    tempFile.open(name);
    int column_number = column_types.size();
    
    if (!tempFile.is_open()) {
        cout << "Can't open the file\n";
        return;
        tempFile.close();
    }
    
    for (int i = 0; i < dataframe.size(); i++) {
        for (int j = 0; j < column_number; j++) {
            tempFile << dataframe[i][j] << delimiter;
        }
        tempFile << endl;
    }
     
    tempFile.close();
    cout << "Data was saved in the file: " << name << endl;
}
//save the column to a CSV file
void Pandas::saveColumnToNewFile(string name, string column, char delimiter) {
    if (name.empty() || column.empty() || !fileIsLoaded) return;

    int col_indx = getColumnIndex(column);

    if (col_indx == ERROR_NUMBER) return;

    ofstream tempFile; // read the file 
    tempFile.open(name); // open the file
    
    //if file can't opens
    if (!tempFile.is_open()) { cout << "can not open the new file\n"; tempFile.close(); return; }

    if (column_types[col_indx] == 's') {
        vector<string> temp = getColumnValues_S(column);
        for (auto i = temp.begin(); i != temp.end(); i++) {
            tempFile << *i << delimiter;
        }
    } else {
        vector<double> temp = getColumnValues_D(column);
        for (auto i = temp.begin(); i != temp.end(); i++) {
            tempFile << *i << delimiter;
        }
    }

    tempFile.close();
}



//get the size of the columns
int Pandas::getColumnsSize() {

    if (!fileIsLoaded) return ERROR_NUMBER;

    return column_types.size();
}

//get all the columns
vector<string> Pandas::getColumns() {

    if (dataframe.empty()) {
        return {};
    }

    return dataframe[0];
}

//get the index of the column by column name
int Pandas::getColumnIndex(string column) {
    //find column
    for (int j = 0; j < dataframe[COLUMNS_INDEX].size(); j++) {
        if (to_lower(dataframe[COLUMNS_INDEX][j]) == to_lower(column)) {
            return j;
        }
    }
    return ERROR_NUMBER;
}

//transform a string to lowercase string
string Pandas::to_lower(string line) {
    
    if (line.empty()) return "";
    
    string temp = "";

    for (int i = 0; i < line.size(); i++) {
        temp += tolower(line[i]);
    }
    return temp;
}

//Column index by default is 0
void Pandas::setColumnIndex(int index) {
    if (index < 0 || index > dataframe.size()) {
        cout << "Can not set the index, bounds error\n";
        return;
    }
    COLUMNS_INDEX = index;
}

//SORTING ALGORITHM
vector<double> Pandas::bubblesort(vector<double> values) {
    
    if ( values.empty() || values.size() == 1) return values;

    bool wasSwap = false;

    do {
        wasSwap = false;

        for (int i = 0; i < (values.size() -1); i++) {
            if (values[i + 1] < values[i]) {
                swap(&values[i], &values[i + 1]);
                wasSwap = true;
            }
        }
    } while (wasSwap);

    return values;
}

vector<double> Pandas::selectionSort(vector<double> values) {
    if (values.empty() || values.size() == 1) return values;

    bool wasSwap = false;
    int min_index;
    int size = values.size();
    int start_index = 0;

    do {
        min_index = start_index;
        wasSwap = false;

        for (int i = start_index; i < (size - 1); i++) {
            
            if (values[min_index] > values[i + 1]) {
                min_index = (i + 1);
                wasSwap = true;
            }
        }
        if (wasSwap)
            swap(&values[start_index], &values[min_index]);

        start_index++;
    } while (wasSwap || start_index < size);

    return values;
}

void Pandas::swap(double *x, double *y) {
    double temp = *x;
    *x = *y;
    *y = temp;
}

void Pandas::displayColumnValues(vector<double> values) {
    if (values.empty()) return;

    for (auto i = values.begin(); i != values.end(); i++) {
        cout << *i << endl;
    }
}

// return stac_mean, stac_median, mode 
void Pandas::calculate_central_tendecy(string column, bool isSort) {
    if (column.empty()) return;
    
    vector<double> temp = getValuesForStatistics(column);
    
    if (temp.empty()) return;
    
    if (!isSort) {
        temp = selectionSort(temp);
        isSort = true;
    }
       
    calculate_central_tendecy(temp, isSort);
}

void Pandas::calculate_central_tendecy(vector<double>values, bool isSort) {

    if (values.empty()) return;

    if (!isSort) {
        values = selectionSort(values);
        isSort = true;
    }

    double mean = stac_mean(values);
    double median = stac_median(values, isSort);
    vector<double> moda = stac_moda(values, isSort);

    cout << "Mean: " << mean << endl;
    cout << "Median: " << median << endl;
    cout << "Mode: ";
    
    if (moda.empty()) cout << "N/A" << endl;
    else {
        for (auto var : moda) {
            cout << var << ", ";
        }
    }
    cout << endl;
}


//=================================================statistic functions=================================================
double Pandas::stac_sum(string column) {

    if (column.empty()) return 0;

    vector<double> temp = getValuesForStatistics(column);
    
    if (temp.empty()) return {};

    return stac_sum(temp);
}

double Pandas::stac_sum(vector<double> values) {
    if (values.empty()) return 0;

    double temp = 0;

    for (int i = 0; i < values.size(); i++) {
        temp += values[i];
    }

    return temp;
}


//Mean 
double Pandas::stac_mean(string column) {
    if (column.empty()) return 0;

    vector<double> temp = getValuesForStatistics(column);

    if (temp.empty()) return {};

    return stac_mean(temp);
}
//Mean
double Pandas::stac_mean(vector<double> values) {
    if (values.empty()) return 0;
    cout << "Calculating mean\n";
    return stac_sum(values) / values.size();
}

//Median
double Pandas::stac_median(string column, bool sort) {
    if (column.empty()) return 0;

    vector<double> temp = getValuesForStatistics(column);

    if (temp.empty()) return {};

    return stac_median(temp, sort);
}
//Median
double Pandas::stac_median(vector<double> values, bool isSort) {
    //verifing
    if (values.empty()) return 0;
    else if (values.size() == 1) return values[0];

    //sort is not sort
    if (!isSort) values = selectionSort(values);

    int size = values.size();
    if (size % 2 == 0) {
        return (values[(size - 1) / 2] + values[size / 2]) / 2.0;
    } else {
        return values[size / 2];
    }
}


//Max number
double Pandas::stac_max(string column) {
    if (column.empty()) return 0;
    vector<double> temp = getValuesForStatistics(column);
    
    if (temp.empty()) return {};

    return stac_max(temp);
}
//Max number
double Pandas::stac_max(vector<double> values) {
    if (values.empty()) return 0;
    else if(values.size() == 1) return values[0];

    double max = values[0];

    //get the max number with one iteration of bubble sort
    for (int i = 1; i < values.size(); i++) {
        if (max < values[i]) {
            max = values[i];
        }
    }
    return max;
}

//Min number
double Pandas::stac_min(string column) {
    
    if (column.empty()) return 0;
   
    vector<double> temp = getValuesForStatistics(column);

    if (temp.empty()) return {};

    return stac_max(temp);
}
//Min number
double Pandas::stac_min(vector<double> values) {
    if (values.empty()) return 0;
    else if (values.size() == 1) return values[0];

    double min = values[0];

    //get the max number with one iteration of bubble sort
    for (int i = 1; i < values.size(); i++) {
        if (min > values[i]) {
            min = values[i];
        }
    }
    return min;
}

//statistics moda function
vector<double> Pandas::stac_moda(string column, bool isSort) {
    if (column.empty()) return {};

    vector<double> temp = getValuesForStatistics(column);

    if (temp.empty()) return {};

    return stac_moda(temp, isSort);
}

//statistics moda function
vector<double> Pandas::stac_moda(vector<double> values, bool isSort) {
    
    if (values.empty() || values.size() == 1) return values;
 
    if (!isSort) values = selectionSort(values);

    const int start_index = 1;
    
    vector<double> moda;
   
    //calculate what is the max frequency
    int max_freq = 1, freq = 1;
    for (int i = start_index; i < values.size(); i++) {
        if (values[i - 1] == values[i]) {
            freq++;
            //cout << values[i - 1] << ", " << values[i] << ", " << freq << endl;
        } else {

            if (freq > 1 && max_freq < freq) {
                max_freq = freq;
            }
            freq = 1;
        }
    }

    //reset freq
    freq = 1;
    
    if (max_freq == 1) return {};
    
    //get all numbers that have the max_freq
    for (int i = start_index; i < values.size(); i++) {
        
        if (values[i - 1] == values[i]) freq++;
        
        else {
            if (max_freq == freq)  moda.push_back(values[i - 1]);    
            freq = 1;
        }
    }

    return moda;
}

vector<double> Pandas::stac_normalize(string column, int range) {
    if (column.empty() || range < 1) return {};

    vector<double> temp = getValuesForStatistics(column);

    if (temp.empty()) return {};

    return stac_normalize(temp, range);
}

//Normalize values
vector<double> Pandas::stac_normalize(vector<double> values, int range) {
    
    if (values.empty() || range < 1) return {};
    
    values = selectionSort(values);

    double max = values[values.size()-1], min= values[0];
    
    for (int i = 0; i < values.size(); i++) {
        values[i] = ((values[i] - min) / (max - min)) * range;
    }

    return values;
}
//==================================================================================================

//Get all values from a column
vector<double> Pandas::getValuesForStatistics(string column) {

    int col_idx = getColumnIndex(column);

    if (col_idx == ERROR_NUMBER || column_types[col_idx] == 's') return {};

    return getColumnValues_D(column);
}

//Histogram display
void Pandas::displayHistogram(string column) {

    if (column.empty() || !fileIsLoaded) return;

    vector<double> temp = getValuesForStatistics(column);

    if (temp.empty()) return;

    return displayHistogram(temp);
}

void Pandas::displayHistogram(vector<double>column, int bins) {
    if (column.empty() || !fileIsLoaded || dataframe.empty()) return;

    int limit = 100;
    column = stac_normalize(column, limit);

    double size = column.size();
    //int bins = limit/bins;

    int step = bins;
    int range_to_print = bins + 1;

    vector<double> frequency;
    vector <double> percentage;
    vector<string> range;

    int freq = 0;
    double p;
    bool canAdd = false;
    
    //range,freq, percentage
    for (int i = 0; i < size; i++) {
        if (column[i] <= bins) {
            freq++;
            canAdd = true;
        }else {
        
            range.push_back(to_string(bins - step) + "-" + to_string(bins));
            frequency.push_back(freq);
            percentage.push_back((freq / size) * 100);

            freq = 0;
            bins += step;
            i = i - 1;
            canAdd = false;
        }
    }

    if (canAdd) {
        range.push_back(to_string(bins - step) + "-" + to_string(bins));
        frequency.push_back(freq);
        percentage.push_back((freq / size) * 100);
    }

    //for (int i = 0; i < percentage.size(); i++) {
    //    cout << range[i] << ", " << percentage[i] << endl;
    //}
    
    bool firstColumn;
    bool last_row = false;
    int l_size = percentage.size();

    int width = 10;
    cout << "\n------------------------------------------------------------------------------------\n";
    for (int i = 0; i < l_size + 1; i++) {
        
        firstColumn = true;
        
        if (i == (l_size)) last_row = true;

        for (int j = 0; j < l_size+ 1; j++) {

            if (firstColumn) {
                cout << "%" << setw(width-5) << limit;
                limit -= step;
                firstColumn = !firstColumn;
            } else if (last_row) {
                cout << setw(width) << range[j-1];
            }else {
                if (percentage[j-1] >= limit)
                    cout << setw(width) <<  " * ";
                else
                    cout << setw(width) << "   ";
            }
        }
        cout << endl;
    }
}