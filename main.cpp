#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <windows.h>

using namespace std;

struct Item
{
    string name;
    int amount;
};

Item *createItems(int &);
void populateItems(Item &, ifstream &);
void mine(Item &, vector<Item> &);
void saveData(vector<Item> &, fstream &);
void restoreData(vector<Item> &, fstream &);
void displayInventory(const vector<Item> &);
int getInvSum(const vector<Item> &);
int main()
{
    fstream saveFile;

    int SIZE, gemToMine, counter = 0;
    vector<Item> inventory;
    Item *items = createItems(SIZE);
    restoreData(inventory, saveFile);
    srand(time(NULL));

    do
    {
        cout << "How many gems would you like to mine?: ";
        cin >> gemToMine;
        cin.ignore();
        cout << endl;
        int targetInvSize = getInvSum(inventory) + gemToMine;
        while(getInvSum(inventory) < targetInvSize){
            int randIndex = rand() % SIZE;
            mine(items[randIndex], inventory);
            counter++;
        }
        cout << "Press any key to continue, or 'q' to quit.\n\n";
    }while(cin.get() != 'q');

    displayInventory(inventory);
    saveData(inventory, saveFile);

    delete [] items;

    items = nullptr;

    return 0;
}

Item *createItems(int &size)
{
    string line;
    ifstream infile("items.txt");
    if(!infile){
        cout << "Error: file could not open!\n\n";
        infile.close();
    }else{
        if(!infile.fail())
            while(getline(infile, line)){
                size++;
            }
        infile.close();
    }

    infile.open("items.txt");
    Item *items = new Item[size];

    for(int i = 0; i < size; i++)
    {
        populateItems(items[i], infile);
    }

    infile.close();

    return items;

}

void populateItems(Item &items, ifstream &infile)
{
    string itemName;
    getline(infile, itemName);
    items.name = itemName;
}

void mine(Item &items, vector<Item> &inventory)
{
    bool flag = false;
    cout << "you are mining...\n\n";

    if(rand() % 10 == 2){
        items.amount = 2;
        cout << "You received an extra gem!\n\n";
        cout << "you received " << items.name << endl << endl;
    }else{
        items.amount = 1;
        cout << "you received " << items.name << endl << endl;
    }

    Sleep(2000);

    for(int i = 0; i < inventory.size(); i++){
        if(inventory[i].name == items.name){
            flag = true;
            inventory[i].amount += items.amount;
        }
    }

    if(!flag){
        inventory.push_back(items);
    }
}

void saveData(vector<Item> &inv, fstream &file)
{
    file.open("inventory.txt", ios::out);

    if(!file){
        cout << "Error: cannot open file!\n\n";
        file.close();
    }

    for(int i = 0; i < inv.size();i++)
    {
        file << inv[i].name << ',' << inv[i].amount << endl;
    }

    file.close();
}

void restoreData(vector<Item> &inv, fstream &file)
{
    string line;
    int size = 0;
    string name, amount;
    file.open("inventory.txt", ios::in);

    if(file.fail())
    {
        file.open("inventory.txt", ios::out);
        file.close();
    }else{
        while(getline(file, line))
        {
            size++;
        }

        file.close();

        Item *itemPtr = new Item[size];

        file.open("inventory.txt", ios::in);

        for(int i = 0; i < size; i++){
            getline(file, itemPtr[i].name, ',');
            getline(file, amount);
            itemPtr[i].amount = stoi(amount);
            inv.push_back(itemPtr[i]);
        }

        file.close();

        delete [] itemPtr;
    }

}

void displayInventory(const vector<Item> &inv)
{
    cout << "Inventory\n";
    cout << "Total: " << getInvSum(inv) << endl;
    cout << left << string(25, '-') << endl;

    for(int i = 0; i < inv.size(); i++){
        cout << inv[i].name << " " << inv[i].amount << endl;
    }
}

int getInvSum(const vector<Item> &inv)
{
    int sum = 0;
    for(int i = 0; i < inv.size(); i++)
    {
        sum += inv[i].amount;
    }
    return sum;
}

