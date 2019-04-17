// Name
// Section #
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include "person.cpp"
#include "book.cpp"

using namespace std;

Person * searchPerson(vector<Person *> & myCardholders, int id);
void readBooks(vector<Book *> & myBooks);
void readPersons(vector<Person *> & myCardholders);
void checkoutBook(vector<Person *> & myCardholders, vector<Book *> & myBooks);
Person * searchPerson(vector<Person *> & myCardholders, int id);
Person * getValidPerson(vector<Person *> & myCardholders);
Book * getAvailBook(vector<Book *> & myBooks);
Book * searchBook(vector<Book *> & myBooks, int id);
void cinFullClear();
void returnBook(vector<Book *> & myBooks);

void printMenu() {
    cout << "----------Library Book Rental System----------" << endl;
    cout << "1.  Book checkout" << endl;
    cout << "2.  Book return" << endl;
    cout << "3.  View all available books" << endl;
    cout << "4.  View all outstanding rentals" << endl;
    cout << "5.  View outstanding rentals for a cardholder" << endl;
    cout << "6.  Open new library card" << endl;
    cout << "7.  Close library card" << endl;
    cout << "8.  Exit system" << endl;
    cout << "Please enter a choice: ";
}

/* You are not obligated to use these function declarations - they're just given as examples*/
void readBooks(vector<Book *> & myBooks) {
    ifstream in("books.txt");
    
    int id;
    string bookName, auth, cat, junk;

    int i = 0;
    while(in >> id){
        //getline to clear the end of line behind "id"
        getline(in, junk);
        getline(in, bookName);
        getline(in, auth);
        getline(in, cat);
        //getline to clear the extra line below "category"
        getline(in, junk);
        //cout << id << bookName << auth << cat << endl;
        myBooks.push_back(new Book(id, bookName, auth, cat));
        
    }
    //myBooks.at(0) is the vector way of referring to that element
    //-> is how you can dereference the pointer within
    //getAuthor() is the member function
    ///cout << myBooks.at(0)->getAuthor() << endl;
    in.close();
    return;
}

void readPersons(vector<Person *> & myCardholders) {
    ifstream in("persons.txt");

    int cardNo;
    bool act;
    string fName, lName;

    while (in >> cardNo >> act >> fName >> lName){
        myCardholders.push_back(new Person(cardNo, act, fName, lName));
        //cout << cardNo << " " << act << " " << fName << lName << endl;
    }
    in.close();
    return;
}
//------------
//Primary Functions
//------------

//Book checkout flow
//User is asked for member id and book id
//If all are valid, the rental is carried out and book object is set a person pointer
void checkoutBook(vector<Person *> & myCardholders, vector<Book *> & myBooks){
    Person * ptrPerson = getValidPerson(myCardholders);

    if(ptrPerson != NULL){
        Book * ptrBook = getAvailBook(myBooks);
        if (ptrBook != NULL){
            if(ptrBook->getPersonPtr() == nullptr){
                ptrBook->setPersonPtr(ptrPerson);
                cout << "The book, " << ptrBook->getTitle() << ", will be checked out by " << ptrBook->getPersonPtr()->fullName() << endl;
                cout << "Rental completed" << endl;
            }
            else{
                cout << "Book already checked out" << endl;
            }
        }
    }

}

//Book return flow
void returnBook(vector<Book *> & myBooks){

}
//--------------
//Person Handlers
//--------------

//Simply searches for a person by id and returns a pointer to it
Person * searchPerson(vector<Person *> & myCardholders, int id){
    Person * ptr = NULL;
    int size = myCardholders.size();
    int currId;
    for(int i = 0; i < size; i++){
        int currId = myCardholders.at(i)->getId();
        if (currId == id){
            ptr = myCardholders.at(i);
        }
    }

    return ptr;
}

//Gets a person and checks that their membership is valid
//returns a pointer to it or NULL
Person * getValidPerson(vector<Person *> & myCardholders){
    Person * ptr = NULL;
    
    cout << "Please enter the card ID: ";
    int id;
    cin >> id;
    
    if(!cin.fail()){
        cinFullClear();
        //try to look for the person whose id matches what was entered
        ptr = searchPerson(myCardholders, id);

        //if it matched an id, proceed
        if(ptr != NULL){

            //if the cardholder is active, proceed
            if(ptr->isActive() == true){
                cout << "Cardholder: " << ptr->fullName() << endl;
            }

            //if not active, just show a message
            if (ptr->isActive() == false){
                cout << "Cardholder " << ptr->fullName() << " is not an active member." << endl;
            }
        }

        //no matches
        else{
        cout << "Card ID not found." << endl;
    }
    }
    else{
        cinFullClear();
        cout << "Invalid Text" << endl;
    }
    
    
    
    return ptr;
}


//----------------
//Book Handlers
//----------------

//simply searches for a book by id and returns a pointer to it
Book * searchBook(vector<Book *> & myBooks, int id){
    Book * ptr = NULL;
    int size = myBooks.size();
    int currId;
    for(int i = 0; i < size; i++){
        currId = myBooks.at(i)->getId();
        if(currId == id){
            ptr = myBooks.at(i);
        }
    }
    return ptr;
}

//gets a book and checks that it has not been checked out
//returns a pointer to a book or NULL
Book * getAvailBook(vector<Book *> & myBooks){
    Book * ptr = NULL;

    cout << "Please enter the book ID: ";
    int id;
    cin >> id;

    if(!cin.fail()){
        cinFullClear();
        ptr = searchBook(myBooks, id);

        if(ptr != NULL){
            cout << "Title: " << ptr->getTitle() << endl;
        }
        else{
            cout << "Book ID not found." << endl;
        }
    }
    else{
        cinFullClear();
        cout << "Invalid text" << endl;
    }
    

    return ptr;
}
//work in progress. Need to make sure this is okay for returns
//might be able to just use the other function and run the return process higher up.
Book * getCheckedBook(vector<Book *> & myBooks){
    Book * ptr = NULL;

    cout << "Please enter the book ID to return: " << endl;
    int id;
    cin >> id;

    if(!cin.fail()){
        cinFullClear();
        ptr = searchBook(myBooks, id);

        if(ptr != NULL){
            cout << "Title: " << ptr->getTitle() << endl;
        }
        else{
            cout << "Book ID not found." << endl;
        }
    }
    else{
        cinFullClear();
        cout << "Invalid text" << endl;
    }
}
/*
void readRentals(vector<Book *> & myBooks, vector<Person *> myCardholders) {
    return;
}

void openCard(vector<Person *> & myCardholders, int nextID) {
    return;
}

Book * searchBook(vector<Book *> myBooks, int id) {
    return nullptr;
}
*/
//----------
//Small Etc. Helpler Functions
//----------
void cinFullClear(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}




//----------
//Main code
//----------
int main()
{
    vector<Book *> books;
    vector<Person *> cardholders;
    readPersons(cardholders);
    readBooks(books);

    int choice;
    do
    {
        // If you use cin anywhere, don't forget that you have to handle the <ENTER> key that 
        // the user pressed when entering a menu option. This is still in the input stream.
        printMenu();
        cin >> choice;
        switch(choice)
        {
            case 1:
                // Book checkout
                checkoutBook(cardholders, books);
                break;

            case 2:
                // Book return
                break;

            case 3:
                // View all available books
                break;

            case 4:
                // View all outstanding rentals
                break;

            case 5:
                // View outstanding rentals for a cardholder
                break;

            case 6:
                // Open new library card
                break;

            case 7:
                // Close library card
                break;
                
            case 8:
                // Must update records in files here before exiting the program
                //cleanPtrs();
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
   } while(choice != 8);
      return 0;
}
