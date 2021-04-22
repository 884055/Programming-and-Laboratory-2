#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct wrong_format_error
{
};

class Coordinate
{

public:
    Coordinate(int lat, int longi)
    {
        latitude = lat;
        longitude = longi;
    }
    int latitude, longitude;

    bool operator<=(const Coordinate &c1) const
    {
        return latitude <= c1.latitude && longitude <= c1.longitude;
    };

    bool operator>=(const Coordinate &c1) const
    {
        return latitude >= c1.latitude && longitude >= c1.longitude;
    };
};

class Time
{
public:
    Time(istream &is)
    {
        char sep1, sep2, sep3, sep4, sep5;
        is >> day >> sep1 >> month >> sep2 >> year >> sep3 >> hour >> sep4 >> minute >> sep5 >> second;
        if (is.bad() || is.fail() || sep1 != '/' || sep2 != '/' || sep3 != '-' || sep4 != ':' || sep5 != ':')
            throw wrong_format_error{};
    }

    bool operator<=(const Time &c1) const
    {
        if (year < c1.year)
            return true;
        if (year > c1.year)
            return false;
        if (month < c1.month)
            return true;
        if (month > c1.month)
            return false;
        if (day < c1.day)
            return true;
        if (day > c1.day)
            return false;
        if (hour < c1.hour)
            return true;
        if (hour > c1.hour)
            return false;
        if (minute < c1.minute)
            return true;
        if (minute > c1.minute)
            return false;
        if (second < c1.second)
            return true;
        if (second > c1.second)
            return false;
        return true;
    }

    bool operator>=(const Time &c1) const
    {
        if (year < c1.year)
            return false;
        if (year > c1.year)
            return true;
        if (month < c1.month)
            return false;
        if (month > c1.month)
            return true;
        if (day < c1.day)
            return false;
        if (day > c1.day)
            return true;
        if (hour < c1.hour)
            return false;
        if (hour > c1.hour)
            return true;
        if (minute < c1.minute)
            return false;
        if (minute > c1.minute)
            return true;
        if (second < c1.second)
            return false;
        if (second > c1.second)
            return true;
        return true;
    }

private:
    int day, month, year, hour, minute, second;
};

int average_temperature(istream &is)
{
    Time tmin{is};
    char sep;
    is >> sep;
    Time tmax{is};
    if (sep != ',')
        throw wrong_format_error{};
    char sep1, sep2, sep3, sep4;
    int lat_min, lat_max, long_min, long_max;
    is >> sep1 >> lat_min >> sep2 >> lat_max >> sep3 >> long_min >> sep4 >> long_max;
    if (is.bad() || is.fail() || sep1 != ',' || sep2 != ',' || sep3 != ',' || sep4 != ',')
        throw wrong_format_error{};
    Coordinate min{lat_min, long_min}, max{lat_max, long_max};
    int sum = 0, number_of_measures = 0;
    while (!is.eof())
    {
        Time measure_time{is};
        char sep1, sep2, sep3, sep4, sep5, sep6, sep7, sep8;
        is >> sep1 >> sep2 >> sep3;
        if (is.bad() || is.fail() || sep1 != ',' || sep2 != '{')
            throw wrong_format_error{};
        Coordinate measure_coordinate{0, 0};
        int temperature;
        if (sep3 == '[')
        {
            is >> measure_coordinate.latitude >> sep4 >> measure_coordinate.longitude >> sep5 >> sep6 >> temperature;
            if (is.bad() || is.fail() || sep4 != ',' || sep5 != ']' || sep6 != ',')
                throw wrong_format_error{};
        }
        else if ((sep3 >= '0' && sep3 <= '9') || sep3=='-')
        {
            is.putback(sep3);
            is >> temperature >> sep3 >> sep4 >> measure_coordinate.latitude >> sep5 >> measure_coordinate.longitude >> sep6;
            if (is.bad() || is.fail() || sep3 != ',' || sep4 != '[' || sep5 != ',' || sep6 != ']')
                throw wrong_format_error{};
        }
        else
            throw wrong_format_error{};
        string note;
        is >> sep7 >> sep8 >> note;
        if (is.bad() || is.fail() || sep7 != '}' || sep8 != ',')
            throw wrong_format_error{};
        if (note == "accept" && measure_coordinate >= min && measure_coordinate <= max && measure_time >= tmin && measure_time <= tmax)
        {
            sum += temperature;
            number_of_measures++;
        }
    }
    return number_of_measures == 0 ? 0 : sum / number_of_measures;
}

int main()
{
    cout << average_temperature(cin);
}