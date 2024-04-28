
#include <iostream>
#include <string>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include<mysql_driver.h>
#include<mysql_connection.h>
#include <cppconn/resultset.h>  
using namespace std;
using namespace sql;



//Name: TMenu
//Defination:TMenU(Connection* conn)
//Desc:Ask the user to select which table they wish to use.
string TMenu(Connection* conn) {
    ResultSet* res;
    Statement* stmt;
    int list = 1;
    int choice;
    stmt = conn->createStatement();
    res = stmt->executeQuery("SHOW TABLES");
    cout << "Which table do you wish to interact with" << endl;
    while (res->next()) {
        cout << list << '. ' << res->getString(1) << endl;
        list++;
    }
    cin >> choice;
    res->beforeFirst(); // Reset the cursor to the beginning of the result set

    list = 1;
    while (res->next()) {
        if (list == choice) {
            string tableName = res->getString(1);
            delete res;
            delete stmt;
            return tableName;
        }
        list++;
    }

    delete res;
    delete stmt;

    return ""; // Return empty string if no valid choice was made
}
//Name: Add
//Defination: Add(Connection* conn, Statement* smt)
//Desc:Add a row to a table of the users choice
void Add(Connection* conn, Statement* smt) {
    string tab = TMenu(conn);
    string info;
    string query;
    if (tab == "Field_Guide") {
        cout << "Enter the digimon information in this order and format 'Name', 'Type', 'Attribute', 'SpecialMove', 'Level', HabitatID, SpecialGroupID, FamilyID " << endl;
        //This clears the input stream to allow the try catch to function correctly
        cin.clear();
        cin.ignore(256, '\n');
        getline(cin, info);
        try {
            smt->executeQuery("Insert into Field_Guide(Name, Type, Attribute, SpecialMove, Level, HabitatID, SpecialGroupID, FamilyID) values (" + info + ");");
        }
        catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
        
    }
    else if(tab== "CombatData") {
        cout << "Enter the Comabt Data information in this order DigimonID, 'AttackPower', 'DefensePower', 'Speed'" << endl;
        cin.clear();
        cin.ignore(256, '\n');
        getline(cin, info);
        try { smt->executeQuery("Insert into CombatData(DigimonID, Attack, DefensePower, Speed) values (" + info + ");"); }
        catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
    }
    else if (tab == "Evolution") {
        cout << "Enter the digimon information in this order PreEvolutionID, 'EvoName', 'EvolutionType',PartnerID add PartnerID if it is a fusion. " << endl;
        cin.clear();
        cin.ignore(256, '\n');
        getline(cin, info);
        try { smt->executeQuery("Insert into Evolution(PreEvolutionID, EvoName, EvolutionType,PartnerID ) values (" + info + ");"); }
        catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
    }

    else if (tab == "Family") {
        cout << "Enter the digimon information in this order and format 'FamName','Description' " << endl;
        cin.clear();
        cin.ignore(256, '\n');
        getline(cin, info);
        query = "INSERT INTO Family(FamName) VALUES (" + info + ");";
        try { smt->executeUpdate(query);}
        catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
    }
    else if (tab == "Habitat") {
        cout << "Enter the digimon information in this order and format 'HabitatName', 'Description' " << endl;
        cin.clear();
        cin.ignore(256, '\n');
        getline(cin, info);
        try { smt->executeUpdate("Insert into Habitat(HabitatName,Description) values (" + info + ");"); }
        catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
    }
    else if (tab == "SpecialGroup") {
        cout << "Enter the digimon information in this order  'SpecialGroupID', 'SpecialGroupName' ,'Leader' ,'Description'" << endl;
        cin.clear();
        cin.ignore(256, '\n');
        getline(cin, info);
        try { smt->executeQuery("Insert into SpecialGroup( SpecialGroupID, SpecialGroupName ,Leader ,Description) values (" + info + ");"); }
        catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
    }
    else {
        cout << "Error" << endl;
    }
}
//Name: Detele
//Defination: Detele(Statement* smt,Connection* con) 
//Desc:Delete a row to a table of the users choice
void Detele(Statement* smt,Connection* con) {
    string row;
    string column;
    string tab = TMenu(con);
    cout << "Enter the delete condtion(name of the column)" << endl;
    cin >> column;
    cout << "Enter the name of the element you with to delete." << endl;
     cin.clear();
     cin.ignore(256, '\n');
     getline(cin, row);
    try { smt->executeUpdate("DELETE FROM " + tab + " WHERE " + column + " = '" + row +"';"); }
    catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
    
}//Name: Update Rows
//Defination: UpdateRows(Statement* smt, Connection* con)
//Desc:Updates a row to a table of the users choice
void UpdateRows(Statement* smt, Connection* con) {
    string name = TMenu(con);
    string col;
    string up;
    string ID;
    cout << "Enter the name of the value you wish to update." << endl;
    cin >> col;
    cout << "What is the new value?" << endl;
    cin >> up;
    cout << "Enter the ID of the item you wish to change." << endl;
    cin >> ID;
    try { smt->executeUpdate("UPDATE " + name + " SET " + col + " = '" + up + "' WHERE DigimonID = "+ID+";"); }
    catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
}

//Name: Alter
//Defination:  Alter(Connection* conn)
//Desc:Pulls up a menu asking weather the user wishes to add, delete, or update a row.
void Alter(Connection* conn) {
    Statement* stmt=conn->createStatement();
    int action = 0;
    cout << "What would you like to do?" <<endl;
    cout << "1. Update a row" << endl;
    cout << "2. Add a row" << endl;
    cout << "3. Delete a row" << endl;
    cout << "Enter your choice: ";
    cin >> action;
    while (action>3 or action<1) {
        cout << "Invalid choice" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Update a row" << endl;
        cout << "2. Add a row" << endl;
        cout << "3. Delete a row" << endl;
        cout << "Enter your choice: ";
        cin >> action;
    }
    switch (action)
    {
    case 1:
        UpdateRows(stmt, conn);
        cout << "DONE" << endl;
        delete stmt;
        break;
    case 2:
        Add(conn,stmt);
        cout << "DONE" << endl;
        delete stmt;
        break;
    case 3:
        Detele(stmt, conn);
        cout << "DONE" << endl;
        break;
    default:"ERROR";
        
    }
}
//Name: Search
//Defination: Search(Connection* conn, string input) 
//Desc:Allows the user to search the Field_guide for a certain Digimon.
void Search(Connection* conn, string input) {
    Statement* stmt = conn->createStatement();
    try {
        ResultSet* res = stmt->executeQuery("SELECT d.*, f.FamName, h.HabitatName, s.SpecialGroupName FROM Field_Guide d LEFT JOIN Family f ON d.FamilyID = f.FamID LEFT JOIN Habitat h ON d.HabitatID = h.HabitatID LEFT JOIN SpecialGroup s ON d.SpecialGroupID = s.SpecialGroupID WHERE d.Name = '"
            + input + "';");
        while (res->next()) {
            cout << "ID: " << res->getInt("DigimonID") << endl; 
            cout<< "Name: " << res->getString("Name") << endl; 
            cout << "Type: " << res->getString("Type") << endl; 
            cout << "Attribute: " << res->getString("Attribute") << endl;
            cout << "SpecialMove: " << res->getString("SpecialMove") << endl;
            cout << "Level: " << res->getString("Level") << endl; 
            cout<<"Family: " << res->getString("FamName") << endl;
            cout << "Habitat:  " << res->getString("HabitatName") << endl;
            cout<<"Special Group: " << res->getString("SpecialGroupName") << endl;
        }
    }
    catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
    
}
//Name: Fliter
//Defination: Fliter(Connection* conn)
//Desc:Allows the user to show Digimon of a certain condition.
void Fliter(Connection* conn) {
    Statement* stmt = conn->createStatement();
    string filter;
    string word;
    cout << "What do you want to filter? (Enter the name of the columon)" << endl;
    cin >> filter;
    cout << "What is the term you want to use?" << endl;
    cin >> word;
    try {
        ResultSet* res = stmt->executeQuery("SELECT d.*, f.FamName, h.HabitatName, s.SpecialGroupName,c.* FROM Field_Guide d LEFT JOIN Family f ON d.FamilyID = f.FamID LEFT JOIN Habitat h ON d.HabitatID = h.HabitatID LEFT JOIN SpecialGroup s ON d.SpecialGroupID = s.SpecialGroupID LEFT JOIN CombatData c ON d.DigimonID = c.DigimonID WHERE d."+filter+"='"+word+"';");
        while (res->next()) {
            cout << "ID: " << res->getInt("DigimonID") << endl;
            cout << "Name: " << res->getString("Name") << endl;
            cout << "Type: " << res->getString("Type") << endl;
            cout << "Attribute: " << res->getString("Attribute") << endl;
            cout << "SpecialMove: " << res->getString("SpecialMove") << endl;
            cout << "Level: " << res->getString("Level") << endl;
            cout << "Family: " << res->getString("FamName") << endl;
            cout << "Habitat:  " << res->getString("HabitatName") << endl;
            cout << "Special Group: " << res->getString("SpecialGroupName") << endl;
            cout << "Attack Power: " << res->getInt("AttackPower") << endl;
            cout << "Defense Power : " << res->getInt("DefensePower") << endl;
            cout << "Speed: " << res->getInt("Speed") << endl;
        }
    }
    catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }

}
//Name: displayALL
//Defination:displayALL(Connection* conn, string name)
//Desc:Shows all the information in every table in the database.
void displayALL(Connection* conn, string name) {
    ResultSet* res;
    Statement* stmt;
    try {
        stmt = conn->createStatement();
        if (name == "Field_Guide") {
            res = stmt->executeQuery("SELECT d.*, f.FamName, h.HabitatName, s.SpecialGroupName FROM Field_Guide d LEFT JOIN Family f ON d.FamilyID = f.FamID LEFT JOIN Habitat h ON d.HabitatID = h.HabitatID LEFT JOIN SpecialGroup s ON d.SpecialGroupID = s.SpecialGroupID; ");
            while (res->next()) {
                cout << "ID: " << res->getInt("DigimonID") << endl;
                cout << "Name: " << res->getString("Name") << endl;
                cout << "Type: " << res->getString("Type") << endl;
                cout << "Attribute: " << res->getString("Attribute") << endl;
                cout << "SpecialMove: " << res->getString("SpecialMove") << endl;
                cout << "Level: " << res->getString("Level") << endl;
                cout << "Family: " << res->getString("FamName") << endl;
                cout << "Habitat:  " << res->getString("HabitatName") << endl;
                cout << "Special Group: " << res->getString("SpecialGroupName") << endl;
                cout << '\n' << endl;
            }
        }
        else {
            if (name == "CombatData") {
                res = stmt->executeQuery("SELECT * FROM Field_Guide LEFT JOIN CombatData ON Field_Guide.DigimonID = CombatData.DigimonID");
                while (res->next()) {
                    cout << "ID: " << res->getInt("DigimonID") << endl; 
                    cout << "Name: " << res->getString("Name") << endl; 
                    cout << "Attack Power: " << res->getInt("AttackPower") << endl; 
                    cout << "Defense Power : " << res->getInt("DefensePower") << endl;
                   cout << "Speed: " << res->getInt("Speed") << endl;
                   cout << '\n' << endl;
                }
            }
            else if (name == "Evolution") {
                res = stmt->executeQuery("SELECT * FROM Evolution;");
                while (res->next()) {
                    cout << "ID: " << res->getInt(1) << endl; 
                    cout << "Name: " << res->getString("EvoName") << endl; 
                    cout << "PreEvolutionID: " << res->getInt("PreEvolutionID") << endl;
                    cout << "EvolutionType: " << res->getString("EvolutionType") << endl;
                    cout << "EvoItem: " << res->getString("EvoItem") << endl; 
                    cout<<"PartnerID: " << res->getInt("PartnerID") << endl;
                    cout << '\n' << endl;
                }
            }
            else if (name == "Family") {
                res = stmt->executeQuery("SELECT * FROM " + name);
                while (res->next()) {
                    cout << "ID: " << res->getInt("FamID") << endl; 
                    cout << "Name: " << res->getString("FamName") << endl; 
                    cout<<"Description: " << res->getString("Description") << endl;
                    cout << '\n' << endl;
                }
            }
            else if (name == "Habitat") {
                res = stmt->executeQuery("SELECT * FROM " + name);
                while (res->next()) {
                    cout << "ID: " << res->getInt("HabitatID") << endl; 
                    cout << "Name: " << res->getString("HabitatName") << endl; 
                    cout<<"Description: " << res->getString("Description") << endl;
                    cout << '\n' << endl;
                }
            }
            else if (name == "SpecialGroup") {
                res = stmt->executeQuery("SELECT * FROM " + name);
                while (res->next()) {
                    cout << "ID: " << res->getInt("SpecialGroupID") << endl; 
                    cout << "Name: " << res->getString("SpecialGroupName") << endl; 
                    cout << "Leader: " << res->getString("Leader") << endl; 
                    cout<<"Description: " << res->getString("Description") << endl;
                    cout << '\n' << endl;
                }
            }
            else { cout << "Error" << endl; }

        }
    }
    catch (SQLException& e) { cerr << "SQL Exception: " << e.what() << endl; }
}

//Name:Menu
//Defination: Menu(int choice,Connection* conn)
//Desc:Pulls up the menu for the database.
void Menu(int choice,Connection* conn) {
    do {
        cout << "1. Display Table\n2. Alter Table\n3. Search Digimon\n4. Filter Data\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        string table;
        string name;
        switch (choice) {
        case 1:
            table = TMenu(conn);
            displayALL(conn,table);
            break;
        case 2:
            Alter(conn);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            break;
        case 3:
            cout << "What is the name of the Digimon you are looking for?" << endl;
            cin >> name;
            Search(conn, name);
            break;
        case 4:
            Fliter(conn);
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
}
    


int main()
{
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;
    //Statement* stmt;
    sql::PreparedStatement* pstmt;
    //ResultSet* res;
    int choice=0;
    string input;
    try
    {
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3307", "root", "RCF112101");
        con->setSchema("DigiDatabase");
    }
    catch (sql::SQLException e)
    {
        cout << "Could not connect to server. Error message: " << e.what() << endl;
        system("pause");
        exit(1);
    }

    Menu(choice, con);

    
    
        //delete res;
        //delete stmt;
        delete con;
 
    return 0;
}
