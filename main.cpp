//
// Created by chr1s on 11/12/2023.
//
#include <iostream>
#include <fstream>
#include <functional>
#include <regex>
#include <tuple>
#include <vector>
#include <stdio.h>
#include <iomanip>
#include <algorithm>
#include <string>
#include <memory>
#include <typeinfo>
#include <ostream>
#include <queue>

using namespace std;
/**
 * FileReader is a class that uses static callbacks and has a callback wrapper
 * callback is the function for parsing the file.
 * string filename is the name of file to be scanned.
 * int size is size of the file.
 */
class FileReader
{
    std::function<string (string name)>callback;
private:
    string filename;
    int size;
public:
    FileReader(string name)
    {
        ifstream file;
        streampos beg, end;
        file.open(name);
        this->filename = name;
        beg = file.tellg();
        file.seekg(0, ios::end);
        end = file.tellg();
        this->size = end-beg;
        file.close();
    }
    int Size()
    {
        return this->size;
    }
    string Name()
    {
        return this->filename;
    }
    /**
     * Raw() returns the raw output of the file being read, opening it as if it were a regular file.
     * @return char pointer memory to a character array.
     */
    char *Raw()
    {
        ifstream file;
        streampos beg, end;
        file.open(this->filename);
        char * memory = new char[this->size*2];
        file.read(memory,this->size);
        file.close();
        return memory;
    }
    /**
     * ReadTXT() is a static method that reads a txt file line by line into the buffer and returns the string result of
     * the file. This function is used for callback in Set.
     * @param name filename to be read
     * @return string result of a the file read
     */
    static string ReadTXT(string name)
    {
        ifstream file;
        streampos beg, end;
        file.open(name);
        string result = "";
        while(!file.eof())
        {
            string buffer;
            getline(file, buffer);
            result += buffer;
        }
        file.close();
        return result;
    }
    /**
     * PunchCardTXT() reader is a method that is used specifically for the PunchCard lab. It only works for the PunchCard.txt format.
     * @param name is the name of the PunchCard formatted txt file.
     * @return result, the vector of strings in each punchcard.
     */
    vector<vector<string>> PunchCardTXT()
    {
        ifstream file;
        streampos beg, end;
        file.open(this->filename);
        vector<vector<string>> result;
        string separator = "--------------------------------------------------------------------------------";
        while(!file.eof())
        {
            vector<string> curCard;
            string buffer;
            getline(file, buffer);
            //cout << buffer << endl;
            if(buffer == separator)
            {
                for(int i = 0; i < 12; i++)
                {
                    string buffer2;
                    getline(file, buffer2);
                    //cout << buffer2 << endl;
                    curCard.push_back(buffer2);
                }
            }
            result.push_back(curCard);
        }
        file.close();
        /*for(auto itr = result.begin(); itr != result.end(); itr++)
        {
            for(auto itr2 = itr->begin(); itr2 != itr->end(); itr2++)
            {
                cout << *itr2;
            }
            cout << "\n";
        }*/

        return result;
    }
    /**
     * ReadBIN() is a static method that reads a txt file line by line into the buffer and returns the string result of
     * the file. This function is used for callback in Set.
     * @param name filename to be read
     * @return string result of a the file read
     */
    static string ReadBIN(string name)
    {
        ifstream file;
        streampos beg, end;
        file.open(name, ios::binary);
        string result = "";
        while(!file.eof())
        {
            string buffer;
            getline(file, buffer);
            result += buffer;
        }
        file.close();
        return result;
    }
    /**
     * ReadCSV() is a static method that reads a txt file line by line into the buffer and returns the string result of
     * the file. This function is used for callback in Set.
     * @param name filename to be read
     * @return string result of a the file read
     */
    static string ReadCSV(string name)
    {
        ifstream file;
        file.open(name);
        string result = "";
        getline(file,result);
        result = "";
        while(!file.eof())
        {
            string buffer;
            vector<string> break_buffer;
            sregex_token_iterator end;
            regex pattern("[^,]");
            getline(file, buffer);
            for(sregex_token_iterator pos(buffer.begin(), buffer.end(), pattern); pos != end; pos++)
            {
                if((*pos).length() > 0)
                {
                    break_buffer.push_back(*pos);
                }
            }
            for(auto itr = break_buffer.begin(); itr != break_buffer.end(); itr++)
            {
                result += *itr;
            }
        }
        file.close();
        return result;
    }
    /**
     * ParseTXT() makes vector of tuples with a txt file in the format of character frequency lists.
     * @param name is the name of the file to be opened. This method is exclusively used if we want to make a vector
     * of tuples. This function isn't used as a callback with Set()
     * @return tuples, the vector of tuples we extracted after reading the files.
     */
    vector<tuple<char,int,string>> ParseTXT(string name)
    {
        vector<tuple<char,int,string>> tuples;
        //Character[  "  ] = Frequency[ 46 ]
        ifstream file;
        file.open(name);
        string result = "";
        int count = 1;
        while(!file.eof())
        {
            string buffer;
            vector<string> break_buffer;
            sregex_token_iterator end;
            regex pattern(" ");
            getline(file, buffer);
            if(buffer.length() > 0)
            {

                sregex_token_iterator pos(buffer.begin(), buffer.end(), pattern, -1);
                while (pos != end)
                {
                    break_buffer.push_back(*pos);
                    pos++;
                }
                //cout << "Buffer size: " << break_buffer.size() << endl;
                //This is character cout << break_buffer.at(2) << endl;
                if(break_buffer.size() > 9)
                {
                    tuple<char,int,string> current = make_tuple(' ', stoi(break_buffer.at(8)), "");
                    tuples.push_back(current);
                }
                else
                {
                    tuple<char,int,string> current = make_tuple(break_buffer.at(2).at(0), stoi(break_buffer.at(7)), "");
                    tuples.push_back(current);
                }
                //cout << break_buffer.at(7);//frequency for most chars
                //cout << break_buffer.at(8) << endl;//freq for space

            }
            count++;
        }
        file.close();
        return tuples;
    }
    /**
     * ExtractTXT() makes vector of tuples with a txt file.
     * @param name is the name of the file to be opened. This method is exclusively used if we want to make a vector
     * of tuples. This function isn't used as a callback with Set()
     * @return tuples, the vector of tuples we extracted after reading the files.
     */
    vector<tuple<char,int,string>> ExtractTXT(string name)
    {
        vector<tuple<char,int,string>> tuples;
        ifstream file;
        file.open(name);
        string result = "";
        int count = 1;
        while(!file.eof())
        {
            string buffer;
            getline(file, buffer);
            if(buffer.length() > 0)
            {
                for(int i = 0; i < buffer.length(); i++)
                {
                    bool found = false;
                    for_each(tuples.begin(), tuples.end(), [&](tuple<char,int,string> &ptr){
                        if(get<0>(ptr) == buffer.at(i))
                        {
                            get<1>(ptr) += 1;
                            found = true;
                        }
                    });
                    if(found == false)
                    {
                        tuple<char,int,string> newTuple = make_tuple(buffer.at(i), 1, "");
                        tuples.push_back(newTuple);
                    }

                }
            }
            count++;
        }
        file.close();
        return tuples;
    }
    /**
     * ParseXML2
     * @param name
     * @return
     */
    vector<tuple<string, double>> ParseXML2(string filename)
    {
        ifstream file;
        file.open(filename);
        vector<tuple<string, double>> result;

        while(!file.eof())
        {
            string buffer;
            vector<string> break_buffer;
            sregex_token_iterator end;
            regex pattern("\\s*<[^<>]*>");
            getline(file, buffer);
            for(sregex_token_iterator pos(buffer.begin(), buffer.end(), pattern, -1); pos != end; pos++)
            {
                if((*pos).length() > 0)
                {
                    break_buffer.push_back(*pos);
                }
            }
            if(break_buffer.size() == 1)
            {
                string name = break_buffer.at(0);
                //cout << name << endl;

                string buffer2;
                vector<string> break_buffer2;
                getline(file, buffer2);
                for(sregex_token_iterator pos2(buffer2.begin(), buffer2.end(), pattern, -1); pos2 != end; pos2++)
                {
                    if((*pos2).length() > 0)
                    {
                        break_buffer2.push_back(*pos2);
                    }
                }
                if(break_buffer2.size() == 1)
                {
                    string price = break_buffer2.at(0);
                    //cout << price << endl;
                    tuple<string,double> tempTup = make_tuple(name, stod(price));
                    result.push_back(tempTup);
                }
                else
                {
                    cout << "error in format of product price" << endl;
                    return result;
                }
            }
            else if(break_buffer.size() > 1)
            {
                cout << "error in format: unknown" << endl;
                return result;
            }
        }
        file.close();
        return result;
    }
    /**
     * ParseXML() makes vector of tuples with an XML file.
     * @param name is the name of the file to be opened. This method is exclusively used if we want to make a vector
     * of tuples. This function isn't used as a callback with Set()
     * @return tuples, the vector of tuples we extracted after reading the files.
     */
    vector<tuple<string, double>> ParseXML(string name)
    {
        vector<tuple<string,double>> tuples;
        ifstream file;
        file.open(name);
        bool firstLine = true;
        while(!file.eof())
        {
            string buffer;
            getline(file, buffer);
            if(firstLine == true)
            {
                firstLine = false;
            }
            else if(buffer.length() > 0)
            {
                if(buffer == "<data>")
                {
                    cout << "Reading from " << name << ":" << endl;
                }
                else if(buffer == "  <row>")
                {
                    string buffer2;
                    getline(file, buffer2);
                    string buffer3;
                    getline(file, buffer3);
                    string buffer4;
                    getline(file, buffer4);
                    if(buffer4 != "  </row>")
                    {
                        cout << "Error parsing xml file" << endl;
                        cout << "Contents of buffer 4: " << buffer4 << endl;
                        file.close();
                        return tuples;
                    }
                    //cout << buffer2.length() << endl;
                    //cout << buffer3.length() << endl;
                    if(buffer2.length()-23 > 0 && buffer3.length()-19 > 0)
                    {
                        string name = buffer2.substr(13,buffer2.size()-23);
                        //cout << name << endl;
                        string price = buffer3.substr(11,buffer3.size()-19);
                        //cout << price << endl;
                        tuple<string,double> temp = make_tuple(name, stod(price));
                        tuples.push_back(temp);
                    }
                }
                else if(buffer == "</data>")
                {
                    cout << "Successful reading and parsing" << endl;
                    file.close();
                    return tuples;
                }
                else
                {
                    cout << "Error parsing xml file" << endl;
                    cout << "Contents of buffer: " << buffer << endl;
                    file.close();
                    return tuples;
                }
            }
        }
        file.close();
        return tuples;
    }
    /**
     * printVector() is a method that prints the vector of tuples, detailing each tuple and each column value.
     * Empty columns get a " " (space) to denote it.
     * @param vec is the vector of tuples.
     */
    void printVector(vector<tuple<char,int,string>> vec)
    {
        cout << "Tuples" << endl;
        int index = 0;
        for_each(vec.begin(), vec.end(),[&](tuple<char,int,string> &itr){
            cout << "TUPLE " << index << " {";
            cout << "C1: " << get<0>(itr);
            cout << " C2: " << get<1>(itr);
            cout << " C3: " << get<2>(itr) << "}" << endl;
            index++;
        });
    }
    /**
     * Set() sets the callback for the class.
     * @param c is the callback function
     */
    void Set(function<string(string)> c){
        callback = c;
    }
    /**
     * searchTuple() searches for a specific tuple in the vector of tuples provided
     * @param vec the vector of tuples
     * @param item the specific tuple we want.
     * @return true if found, false otherwise.
     */
    bool searchTuple(vector<tuple<char,int,string>> vec, tuple<char,int,string> item)
    {
        return std::find(vec.begin(), vec.end(), item)!= vec.end();
    }
    /**
     * Parse runs the callback function to parse the input.
     * @param filename
     * @return callback function determined by Set(): either one of {ReadTXT, ReadBIN, ReadCSV},
     */
    string Parse(string filename)
    {
        return callback(filename);
    }
};
class Product
{
public:
    string name;
    double price;
    tuple<string,double> desc;
    Product(tuple<string,double> input)
    {
        this->name = get<0>(input);
        this->price = get<1>(input);
        this->desc = input;
    }
    string getName()
    {
        return this->name;
    }
    double getPrice()
    {
        return this->price;
    }
    tuple<string,double> getDesc()
    {
        return this->desc;
    }
};
class DataBase{
public:
    map<char,string> translator;
    map<string,Product> dbmap;
    DataBase(vector<tuple<string,double>> tuples, string filename)
    {
        this->translator = this->BarcodeCSV(filename);
        for_each(tuples.begin(),tuples.end(),[&](tuple<string,double> &ptr){
            Product *temp = new Product(ptr);
            string barcode = "";
            for(int i = 0; i < 5; i++)
            {
                if(i >= temp->getName().length())
                {
                    barcode += "000000000";
                }
                else
                {
                    char val = temp->getName().at(i);
                    if(isalpha(val))
                    {
                        barcode += this->translator.find(toupper(val))->second;
                    }
                    else
                    {
                        barcode += this->translator.find(val)->second;
                    }
                }
            }
            pair<string,Product> tPair = make_pair(barcode,*temp);
            this->dbmap.insert(tPair);
        });
    }
    map<char,string> BarcodeCSV(string name)
    {
        ifstream file;
        file.open(name);
        map<char,string> result;
        while(!file.eof())
        {
            string buffer;
            vector<string> break_buffer;
            sregex_token_iterator end;
            regex pattern(",");
            getline(file, buffer);
            for(sregex_token_iterator pos(buffer.begin(), buffer.end(), pattern, -1); pos != end; pos++)
            {
                if((*pos).length() > 0)
                {
                    break_buffer.push_back(*pos);
                }
            }
            if(break_buffer.size() != 2)
            {
                cout << break_buffer.size() << endl;
                cout << break_buffer.at(0);
                cout << break_buffer.at(1);
                cout << "Error reading Barcode 3of9.csv file: break buffer size is wrong" << endl;
                return result;

            }
            else
            {
                string decoded = decode(break_buffer.at(1));
                if(decoded == "error")
                {
                    cout << "Error reading Barcode 3of9.csv file: decoded = error" << endl;
                    return result;
                }
                if(break_buffer.at(0).length() == 1)
                {
                    pair<char,string> newTup = make_pair(break_buffer.at(0).at(0),decoded);
                    result.insert(newTup);
                }
                else if(break_buffer.at(0) == "SP")
                {
                    pair<char,string> newTup = make_pair(' ',decoded);
                    result.insert(newTup);
                }
                else
                {
                    cout << "Error reading Barcode 3of9.csv file: character is wrong" << endl;
                    return result;
                }
            }
        }
        file.close();
        return result;
    }
    string decode(string encrypt)
    {
        string decrypted = "";
        for(int i = 0; i < encrypt.length(); i++)
        {
            if(encrypt.at(i) == 'n')
            {
                decrypted += "0";
            }
            else if(encrypt.at(i) == 'w')
            {
                decrypted += "1";
            }
            else if(encrypt.at(i) != ' ')
            {
                return "error";
            }
        }
        return decrypted;
    }
    void printDB()
    {
        for_each(this->dbmap.begin(),this->dbmap.end(), [](pair<string, Product> ptr){
           cout << "Barcode: " << ptr.first << endl;
           cout << "Name: " << ptr.second.getName() << endl;
           cout << "Price: " << ptr.second.getPrice() << "\n" << endl;
        });
    }
};





/**
 * main is used to stage unit testing.
 * NOTE: THE COLUMN, LINES, AND MATRIX INDEX START AT 0, so when writing tests, read notepad lines and columns as -1,
 * @return 0
 */
int main() {
    string filename = "C:\\Users\\chr1s\\CLionProjects\\DataBase\\Products.xml";
    string filename2 = "C:\\Users\\chr1s\\CLionProjects\\DataBase\\3of9.csv";
    FileReader *fr = new FileReader(filename);
    vector<tuple<string,double>> tuples = fr->ParseXML2(filename);
    DataBase *db = new DataBase(tuples, filename2);
    db->printDB();
}



