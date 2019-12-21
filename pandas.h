#pragma once
#ifndef PANDAS_H
#define PANDAS_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "../../cpp_header/stdc++.h" 
#include <iomanip>
#include <regex>
#include <cctype> 
using namespace std;

class Pandas {
private:
    int SIZE = 0;
    int COLUMNS_INDEX = 0;
    const int ERROR_NUMBER = -1;
    string filename = "";
    vector<vector<string>> dataframe;
    vector<string> col_value;
    vector<char> column_types;
    bool fileIsLoaded = false;
    string number_regex_pattern = "^[0-9]+(\\.[0-9]+)?(\\s+)?$";
  
    //private functions
    void setSize(string filepath);
    void displayHeader(int width = 15);
    void setColumnsTypes();
    void swap(double* x, double* y);
    string to_lower(string line); // convert string to lower case
    int getColumnIndex(string column);

    vector<double> getValuesForStatistics(string column);

public:
    Pandas();
    ~Pandas();

    void read_csv(string filepath, char delimieter = ','); // read the file
    int getSize();                      // get how many rows are in the file
    int getColumnsSize();               // get how many columns are in the CVS
    void setColumnIndex(int index);     // set the index where the columns are located in the CVS
    
    void display_all();    //display all rows in the dataframe
    void saveToCSV(string name, char delimiter = ',');        // save the all rows into a new file
    void saveColumnToNewFile(string name, string column, char delimiter = ',' ); //save the value of column in a new file
    void displayByValueAndColumn(string column, string valueToSearch);      //get the values by column and identifier
    void displayColumnValues(vector<double> values);                        //display but with vector

    bool validateID(string id);
    bool addRecordInColumnOrder(vector<string> record);  //add a new record to the dataframe
    bool removeRecordByValueAndColumn(string valueToRemove, string column);     //remove the record

    //============================================statistic functions============================================
    double stac_sum(vector<double> values);
    double stac_sum(string colum);
    
    double stac_mean(vector<double> values);
    double stac_mean(string colum);

    double stac_median(vector<double> values, bool sort);
    double stac_median(string column, bool sort);

    double stac_max(vector<double>values);
    double stac_max(string column);

    double stac_min(vector <double> values);
    double stac_min(string column); 

    vector<double> stac_moda(vector<double>values, bool sort);
    vector< double> stac_moda(string columm, bool sort);

    vector<double> stac_normalize(vector<double> values, int toValue);
    vector<double> stac_normalize(string column, int toValue);


    void calculate_central_tendecy(vector<double> values, bool isSort);
    void calculate_central_tendecy(string column, bool isSort);


    void displayHistogram(vector<double>values, int bins = 10);
    void displayHistogram(string column);
    //========================================================================================

    vector<double> bubblesort(vector<double> values);
    vector<double> selectionSort(vector<double> values);
  
    vector<double> getColumnValues_D(string column);  //get column number
    vector<string> getColumnValues_S(string column); // get column string value
    vector<string> getColumns(); // get columns names

    
};
#endif