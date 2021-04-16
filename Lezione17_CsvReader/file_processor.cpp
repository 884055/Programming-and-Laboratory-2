#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

enum column_types {
    date, number, string_type
};

struct file_error {
    string message;
};

struct format_error {
    string message;
};

class Date {
    public:
        Date(int d, int m, int y) {
            year = y;
            month = m;
            day = d;
        }

        Date(string v) {
            istringstream s{v};
            int y, m, d;
            char delim1, delim2;

            s >> y >> delim1 >> m >> delim2 >> d;

            if(! s.eof())
                throw format_error{"String "+v+" is not a date"};
            if(delim1!='-' || delim2!='-')
                throw format_error{"String "+v+" is not a date"};
            if(y < 1900 || y > 2023)
                throw format_error{"String "+v+" is not a date"};
            if(m > 12 || m < 1)
                throw format_error{"String "+v+" is not a date"};
            if(d < 1 || d > 31)
                throw format_error{"String "+v+" is not a date"};
            year = y;
            month = m;
            day = d;
        }

        string to_string() {
            return std::to_string(day)+"\\"+std::to_string(month)+"\\"+std::to_string(year);
        }

    private:
        int day, month, year;
};

class Table {
    public:
        Table(string filepath) {
            ifstream file{filepath};

            if(file.good()) {
                string headers_row; 
                getline(file, headers_row, '\n');
                headers = parse_line(headers_row);
            }
            else throw new file_error{"Unable to read properly "+filepath};
            vector<vector<string>> data = vector<vector<string>>{headers.size()};
            while(file.good()) {
                string s;
                getline(file, s, '\n');
                if(! s.empty()) {
                    vector<string> line_values = parse_line(s);
                    for(int i = 0; i < line_values.size(); i++)
                        data.at(i).push_back(line_values.at(i));
                }
            }
            if(! file.eof())
                throw new file_error{"Error while reading file "+filepath};

            for(int i = 0; i < data.size(); i++) {
                vector<string> v = data.at(i);
                if(isDate(v)) {
                    col_types.push_back(date);
                    date_columns.push_back(transform_to_dates(v));
                }
                else if(isNumerical(v)){
                    col_types.push_back(number);
                    numerical_columns.push_back(transform_to_numerical(v));
                }
                else {
                    col_types.push_back(string_type);
                    string_columns.push_back(v);
                }
            }
        }

        Table project(string column_string, vector<string> columns_numerical) {
            int index_column_string = transform_index_to_string_columns(get_column_index(column_string));
            vector<int> index_columns_numerical;
            for(string c : columns_numerical)
                index_columns_numerical.push_back(transform_index_to_numerical_columns(get_column_index(c)));
            vector<column_types> types;
            vector<string> headers;
            vector<string> v_cs = string_columns.at(index_column_string);
            types.push_back(string_type);
            headers.push_back(column_string);
            for(string s : columns_numerical)
                headers.push_back(s);
            vector<vector<int>> v_numbers;
            for(int index : index_columns_numerical) {
                types.push_back(number);
                v_numbers.push_back(numerical_columns.at(index));
            }

            vector<vector<Date>> dates;
            vector<vector<string>> s;
            s.push_back(v_cs);


            Table result{types, headers, dates, v_numbers, s};
            return result;

        }


        void dump_to_file(string file_path) {
            ofstream output{file_path};
            if(!output.good())
                throw file_error{"The output file "+file_path+" cannot be opened"};
            for(int i = 0; i < headers.size(); i++) {
                output << headers.at(i);
                if(i!=headers.size()-1)
                    output << ",";
            }
            output << "\n";
            int number_of_rows = string_columns.at(0).size();
            for(int row = 0; row < number_of_rows; row++) {
                for(int column = 0; column < col_types.size(); column++) {
                    if(col_types.at(column)==date) {
                        int date_index = transform_index_to_date_columns(column);
                        output << date_columns.at(date_index).at(row).to_string();
                    }
                    else if(col_types.at(column)==number) {
                        int numerical_index = transform_index_to_numerical_columns(column);
                        output << numerical_columns.at(numerical_index).at(row);
                    }
                    else {
                        int string_index = transform_index_to_string_columns(column);
                        output << string_columns.at(string_index).at(row);
                    }

                    if(column!=col_types.size()-1)
                        output << ",";
                }
                output << "\n";
            }
            output.close();
        }


    //We assume that the current table contains only the first column with string values (the keys over whom we want to merge)
    //and all the other columns are numeric. This computes for each distinct value in the first column the sum of all the values
    //in the other columns
    Table summarize()
    {
        vector<string> strings_column;
        vector<vector<int>> numbers{numerical_columns.size()};
        vector<vector<Date>> dates;
        vector<string> headers;

        vector<string> keys = string_columns.at(0);
        for (int i = 0; i < keys.size(); i++)
        {
            string key = keys.at(i);
            auto previous_value = std::find(strings_column.begin(), strings_column.end(), key);
            if (previous_value != strings_column.end())
            {
                int index = previous_value - strings_column.begin();
                for (int j = 0; j < numerical_columns.size(); j++)
                {
                    int value = numerical_columns.at(j).at(i);
                    numbers.at(j).at(index) += value;
                }
            }
            else
            {
                strings_column.push_back(key);
                for (int j = 0; j < numerical_columns.size(); j++)
                    numbers.at(j).push_back(numerical_columns.at(j).at(i));
            }
        }
        vector<vector<string>> strings{strings_column};
        return Table{col_types, headers, dates, numbers, strings};
    }

    private:
        vector<column_types> col_types;

        vector<string> headers;
        
        vector<vector<Date>> date_columns;
        vector<vector<int>> numerical_columns;
        vector<vector<string>> string_columns;

        Table(vector<column_types> t, vector<string> h, vector<vector<Date>> d, vector<vector<int>> n, vector<vector<string>> s) {
            col_types = t;
            headers = h;
            date_columns = d;
            numerical_columns = n;
            string_columns = s;
        }

        vector<string> parse_line(string row) {
                istringstream single_header{row};
                vector<string> result;
                string value;
                while(single_header.good()) {
                    getline(single_header, value, ',');
                    result.push_back(value);
                }
                return result;
        }

        int transform_index_to_string_columns(int index) {
            int result = 0;
            for(int i = 0; i < index; i++)
                if(col_types.at(i)==string_type)
                    result++;
            return result;
        }


        int transform_index_to_date_columns(int index) {
            int result = 0;
            for(int i = 0; i < index; i++)
                if(col_types.at(i)==date)
                    result++;
            return result;
        }

        int transform_index_to_numerical_columns(int index) {
            int result = 0;
            for(int i = 0; i < index; i++)
                if(col_types.at(i)==number)
                    result++;
            return result;
        }

        int get_column_index(string header) {
            auto it_cs = find(headers.begin(), headers.end(), header);
            if(it_cs == headers.end())
                throw format_error{"Column "+header+" is not part of the table"};
            return it_cs-headers.begin();
        }
        bool isNumerical(vector<string> v) {

            string value = v.at(0);
            try {
                transform_value_to_number(value);
                return true;
            }
            catch(format_error e) {
                return false;
            }
        }

        vector<int> transform_to_numerical(vector<string> values) {
            vector<int> result;
            for(string v : values)
                result.push_back(transform_value_to_number(v));
            return result;
        }

        int transform_value_to_number(string v) {
            istringstream s{v};
            int value;
            s >> value;
            if(! s.eof())
                throw format_error{"String "+v+" is not a numerical value"};
            else
                return value;
        }


        bool isDate(vector<string> v) {
            string value = v.at(0);
            try {
                Date{value};
                return true;
            }
            catch(format_error e) {
                return false;
            }
        }

        vector<Date> transform_to_dates(vector<string> values) {
            vector<Date> result;
            for(string v : values)
                result.push_back(transform_value_to_date(v));
            return result;
        }
        Date transform_value_to_date(string v) {
            return Date{v};
        }

};

int main() {
    Table table{"/home/prog2021/labprog/LabProg_2021/Lezione17_CsvReader/somministrazioni-vaccini-summary-latest.csv"};
    
    
    vector<string> numericals;
    numericals.push_back("totale");
    numericals.push_back("categoria_over80");
    Table result=table.project("nome_area", numericals).summarize();
    result.dump_to_file("/home/prog2021/labprog/LabProg_2021/Lezione17_CsvReader/our_somministrazioni-vaccini-summary-latest.csv");
    
    return 0;
}