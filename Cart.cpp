//
// Created by chr1s on 11/24/2023.
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
#include <thread>
#include <mutex>

using namespace std;
/**
 * FileReader is a class that uses static callbacks and has a callback wrapper
 * callback is the function for parsing the file.
 * string filename is the name of file to be scanned.
 * int size is size of the file.
 */
mutex globalMutex;
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
    string barcode;
    Product(tuple<string,double> input)
    {
        this->name = get<0>(input);
        this->price = get<1>(input);
        this->desc = input;
        this->barcode = "";
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
    void setBarcode(string val)
    {
        this->barcode = val;
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
            temp->setBarcode(barcode);
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
    Product* searchDatabase(string barcode){
        if(this->dbmap.find(barcode) != this->dbmap.end())
        {
            return &this->dbmap.find(barcode)->second;
        }
        return nullptr;
    }
};
class Cart
{
public:
    queue<string> contents;
    int capacity;
    int size;
    int accesses;
    Cart()
    {
        this->size = 0;
        this->capacity = 25;
        this->accesses = 0;
    }
    bool addToCart(string pCode)
    {
        if(this->size < this->capacity)
        {
            this->contents.push(pCode);
            this->size += 1;
            return true;
        }
        else
        {
            return false;
        }
    }
    string* removeFromCart()
    {
        if(this->contents.empty())
        {
            return nullptr;
        }
        else
        {
            string *first = &(this->contents.front());
            this->contents.pop();
            this->size -= 1;
            return first;
        }
    }
    void updateCounter()
    {
        this->accesses++;
    }
};
class CartManager{
public:
    queue<shared_ptr<Cart>> container;
    shared_ptr<DataBase> database;
    CartManager(shared_ptr<DataBase> db){
        this->database = db;
    }
    bool processCart(string cart)
    {
        globalMutex.lock();
        cout << cart << endl;
        if(this->container.empty() == true)
        {
            cout << "Empty Cart\n" << endl;
            globalMutex.unlock();
            return false;
        }
        else
        {
            shared_ptr<Cart> current = this->container.front();
            while(current->size > 0)
            {
                string *prodCode = current->removeFromCart();
                current->updateCounter();
                Product *result = this->database->searchDatabase(*prodCode);
                if(result == nullptr)
                {
                    cout << "Faulty item detected\n" << endl;
                }
                else
                {
                    cout << "Product: " << result->getName() << endl;
                    cout << "Price: " << result->getPrice() << "\n" << endl;
                }
            }
            this->container.pop();
        }
        globalMutex.unlock();
    }

};
class CartQueueManager{
public:
    vector<shared_ptr<CartManager>> qmContainer;
    shared_ptr<DataBase> database;
    CartQueueManager(shared_ptr<DataBase> db){
        this->database = db;
        for(int i = 0; i < 10; i++)
        {
            CartManager temp(this->database);
            this->qmContainer.push_back(make_shared<CartManager>(temp));
        }
    }
    void workQueue()
    {
        bool allClear = false;
        int round = 1;
        while(allClear == false) {
            int countEmpty = 0;
            for_each(this->qmContainer.begin(), this->qmContainer.end(), [&](shared_ptr<CartManager> ptr){
                if(ptr->container.size() == 0)
                {
                    //cout << ptr->container.size() << endl;
                    countEmpty += 1;
                }
            });
            //cout << "\n" << countEmpty << endl;
            if(countEmpty == 10)
            {
                allClear = true;
                return;
            }
            //cout << "MAKING" << endl;
            thread c1(&CartManager::processCart, this->qmContainer.at(0), "Line 1");
            thread c2(&CartManager::processCart, this->qmContainer.at(1), "Line 2");
            thread c3(&CartManager::processCart, this->qmContainer.at(2), "Line 3");
            thread c4(&CartManager::processCart, this->qmContainer.at(3), "Line 4");
            thread c5(&CartManager::processCart, this->qmContainer.at(4), "Line 5");
            thread c6(&CartManager::processCart, this->qmContainer.at(5), "Line 6");
            thread c7(&CartManager::processCart, this->qmContainer.at(6), "Line 7");
            thread c8(&CartManager::processCart, this->qmContainer.at(7), "Line 8");
            thread c9(&CartManager::processCart, this->qmContainer.at(8), "Line 9");
            thread c10(&CartManager::processCart, this->qmContainer.at(9), "Line 10");
            //cout << "JOINING" << endl;
            c1.join();
            c2.join();
            c3.join();
            c4.join();
            c5.join();
            c6.join();
            c7.join();
            c8.join();
            c9.join();
            c10.join();
            cout << "\nRound " << round << " finished.\n" << endl;
            round++;
        }
    }
    void addToQueue(int index, shared_ptr<Cart> obj)
    {
        this->qmContainer.at(index)->container.push(obj);
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
    DataBase db(tuples, filename2);
    shared_ptr<DataBase> database = make_shared<DataBase>(db);
    //db->printDB();


    Cart c1;
    shared_ptr<Cart> car1 = make_shared<Cart>(c1);
    car1->addToCart("101001000100010010101001000100010010100001001");
    car1->addToCart("101001000100010010101001000100010010100001001");
    car1->addToCart("101001000100011000001000011100011000100000110");
    car1->addToCart("101001000100011000100000110100011000100001001");
    car1->addToCart("101001000110000001101001000110000001101000010");
    car1->addToCart("110010000100010010000001101110000001100000110");
    car1->addToCart("110010000100011000001000011001000011100010010");

    Cart c2;
    shared_ptr<Cart> car2 = make_shared<Cart>(c2);
    car2->addToCart("111000000100001100100010010001000011100011000");
    car2->addToCart("111000000100001100100010010001000011100011000");
    car2->addToCart("101001000100010010100010010100000011001001100");
    car2->addToCart("101001000100001100001001100001010010011000100");
    car2->addToCart("101001000100001100001001100001010010011000100");

    Cart c3;
    shared_ptr<Cart> car3 = make_shared<Cart>(c3);
    car3->addToCart("101001000100001001100000011100011000011000100");
    car3->addToCart("101001000100001001100000011100011000011000100");
    car3->addToCart("101001000100001001000010110011000100001011000");
    car3->addToCart("101001000100001001000010110011000100001011000");

    Cart c4;
    shared_ptr<Cart> car4 = make_shared<Cart>(c4);
    car4->addToCart("101001000100001001000010011000011001110010000");
    car4->addToCart("101001000100001001000010011000011001110010000");
    car4->addToCart("101001000100001001000010011000011001110010000");

    Cart c5;
    shared_ptr<Cart> car5 = make_shared<Cart>(c5);
    car5->addToCart("101000010110000001001000110100001100100000110");
    car5->addToCart("101000010110000001001000110100001100100000110");
    car5->addToCart("101000010110000001001000110100001100100000110");

    Cart c6;
    shared_ptr<Cart> car6 = make_shared<Cart>(c6);
    car6->addToCart("101000010100001001101001000100001001100000110");
    car6->addToCart("101000010100001001101001000100001001100000110");
    car6->addToCart("101000010100001001101001000100001001100000110");
    car6->addToCart("101000010100001001101001000100001001100000110");
    car6->addToCart("101000010100001001101001000100001001100000110");
    car6->addToCart("101000010100001001101001000100001001100000110");


    Cart c7;
    shared_ptr<Cart> car7 = make_shared<Cart>(c7);
    car7->addToCart("101000010100001001101001000011000100100001001");
    car7->addToCart("101000010100001001101001000011000100100001001");
    car7->addToCart("101000010100001001101001000011000100100001001");

    Cart c8;
    shared_ptr<Cart> car8 = make_shared<Cart>(c8);
    car8->addToCart("101000010001001100010010001100011000000011001");
    car8->addToCart("101000010001001100010010001100011000000011001");
    car8->addToCart("101000010001001100010010001100011000000011001");

    Cart c9;
    shared_ptr<Cart> car9 = make_shared<Cart>(c9);

    Cart c10;
    shared_ptr<Cart> car10 = make_shared<Cart>(c10);
    car10->addToCart("100011000000010011000011001001001100011000001");
    car10->addToCart("100011000000010011000011001001001100011000001");
    car10->addToCart("100011000000010011000011001001001100011000001");
    car10->addToCart("100011000000010011000011001001001100011000001");
    car10->addToCart("100011000000010011000011001001001100011000001");
    car10->addToCart("100011000000010011000011001001001100011000001");

    Cart c11;
    shared_ptr<Cart> car11 = make_shared<Cart>(c11);
    car11->addToCart("100011000000001101000001101011000100001000110");

    Cart c12;
    shared_ptr<Cart> car12 = make_shared<Cart>(c12);
    car12->addToCart("100010010110010000001000110000010110100011000");
    car12->addToCart("100010010110010000001000110000010110100011000");
    car12->addToCart("100010010110010000001000110000010110100011000");

    Cart c13;
    shared_ptr<Cart> car13 = make_shared<Cart>(c13);
    car13->addToCart("100010010100001001000010110101000010100011000");
    car13->addToCart("100010010100000110100011000000001101100001001");
    car13->addToCart("100010010100000110100001001000010011000001101");

    Cart c14;
    shared_ptr<Cart> car14 = make_shared<Cart>(c14);
    car14->addToCart("100010010100001001000010110101000010100011000");
    car14->addToCart("100010010100000110100011000000001101100001001");
    car14->addToCart("100010010011000001100011000000010011011000100");

    Cart c15;
    shared_ptr<Cart> car15 = make_shared<Cart>(c15);
    car15->addToCart("100010010001001100001000011011000100101001000");
    car15->addToCart("100010010001000011001001100011000001100011000");
    car15->addToCart("100010010000010011001001100100010010000010011");
    car15->addToCart("100001100100001001001001100100000110011000100");
    car15->addToCart("100001001011000001100010010101001000100001001");
    car15->addToCart("100001100100010010000010011100011000110010000");

    Cart c16;
    shared_ptr<Cart> car16 = make_shared<Cart>(c16);
    car16->addToCart("100001001001000110001010010100001001100000110");
    car16->addToCart("100001100100001001001000011001001100001001001");
    car16->addToCart("100001100100001001000010011000011001011000100");
    car16->addToCart("100010010000010011001001100100010010000010011");

    Cart c17;
    shared_ptr<Cart> car17 = make_shared<Cart>(c17);
    car17->addToCart("100001100100010010000010110011000100001000110");

    //error input
    Cart c18;
    shared_ptr<Cart> car18 = make_shared<Cart>(c18);
    car18->addToCart("000000000000000000000000000000000000000000000");

    Cart c19;
    shared_ptr<Cart> car19 = make_shared<Cart>(c19);
    car19->addToCart("100001001001010010100000110001001100101001000");
    car19->addToCart("100000110110010000100011000011000100001001001");
    car19->addToCart("100000110100011000001000011001001100001000110");

    //error input
    Cart c20;
    shared_ptr<Cart> car20 = make_shared<Cart>(c20);
    car20->addToCart("100000110100001001011000001001001100100010010");
    car20->addToCart("100001001001000110001010010100001001100000110");
    car20->addToCart("100001001001000011110000001101000010001001100");
    car20->addToCart("100001001001000011001011000100000110100011000");
    car20->addToCart("000000000000000000000000000000000000000000000");
    car20->addToCart("100001001000010011101001000100001100100010010");

    Cart c21;
    shared_ptr<Cart> car21 = make_shared<Cart>(c21);
    car21->addToCart("100000110100011000000011001011000100001010010");
    car21->addToCart("100001001000010011000010110100001001101001000");
    car21->addToCart("011000001100001001000010011001001100001000011");

    Cart c22;
    shared_ptr<Cart> car22 = make_shared<Cart>(c22);
    car22->addToCart("001011000001000011100011000100001001011000100");
    car22->addToCart("001011000001000011100010010110000001100000110");
    car22->addToCart("001011000001000011100010010110000001000010011");
    car22->addToCart("001010010100011000100001001101001000100001100");
    car22->addToCart("001010010100010010100000110100000011011000100");
    car22->addToCart("001010010100010010001010010101001000100010010");
    car22->addToCart("001010010100010010001010010001000110001001100");
    car22->addToCart("001010010100001001001010010100000110001001100");
    car22->addToCart("001010010100001001000010011101001000100001001");
    car22->addToCart("001010010001001100101001000100000011001000011");
    car22->addToCart("001010010001001100011010000011010000100001001");
    car22->addToCart("001010010001000011110000001101000010001000110");
    CartQueueManager *queueManager = new CartQueueManager(database);
    queueManager->addToQueue(0, car1);
    queueManager->addToQueue(0, car2);
    queueManager->addToQueue(1, car3);
    queueManager->addToQueue(1, car4);
    queueManager->addToQueue(2, car5);
    queueManager->addToQueue(2, car6);
    queueManager->addToQueue(3, car7);
    queueManager->addToQueue(3, car8);
    queueManager->addToQueue(3, car9);
    queueManager->addToQueue(3, car10);
    queueManager->addToQueue(3, car11);
    queueManager->addToQueue(5, car12);
    queueManager->addToQueue(5, car13);
    queueManager->addToQueue(6, car14);
    queueManager->addToQueue(6, car15);
    queueManager->addToQueue(7, car16);
    queueManager->addToQueue(7, car17);
    queueManager->addToQueue(8, car18);
    queueManager->addToQueue(9, car19);
    queueManager->addToQueue(9, car20);
    queueManager->addToQueue(9, car21);
    queueManager->addToQueue(9, car22);

    queueManager->workQueue();

}



