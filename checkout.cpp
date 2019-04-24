// Name: Dante Padley
// Section #301-03
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include "person.cpp"
#include "book.cpp"

using namespace std;

//-----------
//TOC
//---ctrl+f search any of the following:
//Prototypes
//Utilities
//Read Functions
//Write Functions
//Primary Functions
//Person Handlers
//Book Handlers
//main
//-----------



//-----------
//Prototypes
//-----------
Person * searchPerson(vector<Person *> & myCardholders, int id);
void readBooks(vector<Book *> & myBooks);
int readPersons(vector<Person *> & myCardholders);
void checkoutBook(vector<Person *> & myCardholders, vector<Book *> & myBooks);
Person * searchPerson(vector<Person *> & myCardholders, int id);
Person * getValidPerson(vector<Person *> & myCardholders);
Book * getAvailBook(vector<Book *> & myBooks);
Book * searchBook(vector<Book *> & myBooks, int id);
void cinFullClear();
void returnBook(vector<Book *> & myBooks);
void listAvailableBooks(vector<Book *> & myBooks);
void listRentals(vector<Book *> & myBooks);
void listMemberRentals(vector<Book *> & myBooks, vector<Person *> & myCardholders);
void openCard(vector<Person *> & myCardholders, string nextId);
Person * searchByName(vector<Person *> & myCardHolders, string fullName);
void closeCard(vector<Person *> & myCardholders);
void writePersons(vector<Person *> myCardholders);
void writeRentals(vector<Book *> myBooks, vector<Person *> myCardholders);

//-----------
//Utilities
//-----------
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

void cinFullClear(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
//--------------
//Read Functions
//--------------
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

int readPersons(vector<Person *> & myCardholders) {
    ifstream in("persons.txt");
    int nextId = 0;
    int cardNo;
    bool act;
    string fName, lName;

    while (in >> cardNo >> act >> fName >> lName){
        myCardholders.push_back(new Person(cardNo, act, fName, lName));
        if(cardNo >= nextId){
            nextId = cardNo + 1;
        }
        //cout << cardNo << " " << act << " " << fName << lName << endl;
    }
    in.close();
    return nextId;
}

//This reads the rentals file and stores the pointers of the people who have checked out books in their
//respective book objects in the books vector
void readRentals(vector<Book *> & myBooks, vector<Person *> myCardholders) {
    ifstream in("rentals.txt");
    Person * personPtr;
    Book * bookPtr;

    int book, card;
    while(in >> book >> card){
        personPtr = searchPerson(myCardholders, card);
        bookPtr = searchBook(myBooks, book);
        bookPtr->setPersonPtr(personPtr);
        //cout << bookPtr->getPersonPtr()->fullName() << endl;
    }

    return;
}

//------------
//Write Functions
//------------

void writePersons(vector<Person *> myCardholders){
    ofstream wr("persons.txt");

    for(int i=0; i < myCardholders.size(); i++){
        wr << myCardholders.at(i)->getId() << " "
        << myCardholders.at(i)->isActive() << " "
        << myCardholders.at(i)->fullName() << "\n";
    }
    wr.close();
}

void writeRentals(vector<Book *> myBooks){
    ofstream wr("rentals.txt");

    for(int i=0; i < myBooks.size(); i++){
        if(myBooks.at(i)->getPersonPtr() != nullptr){
            wr << myBooks.at(i)->getId() << " " << myBooks.at(i)->getPersonPtr()->getId() << "\n";
        }
    }
    wr.close();
}
//------------
//Primary Functions
//------------

//Book checkout flow
//User is asked for member id and book id
//If all are valid, the rental is carried out and book object is set a person pointer
void checkoutBook(vector<Person *> & myCardholders, vector<Book *> & myBooks){
    Person * ptrPerson = getValidPerson(myCardholders);

    if(ptrPerson != nullptr){
        Book * ptrBook = getAvailBook(myBooks);
        if (ptrBook != nullptr){
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

    //need to select the book by id
    //use searchBook function
    //if book is checked out, update book object to clear its person ptr
    //if book is not checked out, do nothing

    Book * ptr = nullptr;

    cout << "Please enter the book ID to return: " << endl;
    int id;
    cin >> id;

    if(!cin.fail()){
        cinFullClear();
        ptr = searchBook(myBooks, id);

        if(ptr != nullptr){
            cout << "Title: " << ptr->getTitle() << endl;
            ptr->setPersonPtr(nullptr);
            cout << "Return completed" << endl;
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

void listAvailableBooks(vector<Book *> & myBooks){
    //iterate through vector
    //check value of personPtr
    //if nullptr, cout the getters
    //if not nullptr, do nothing
    Person * ptr;
    //flag that raises when there are available books
    bool available = false;
    for(int i = 0; i < myBooks.size(); i++){
        ptr = myBooks.at(i)->getPersonPtr();
        if (ptr == nullptr){
            cout << "\nBook ID: " << myBooks.at(i)->getId() << endl;
            cout << "Title: " << myBooks.at(i)->getTitle() << endl;
            cout << "Author: " << myBooks.at(i)->getAuthor() << endl;
            cout << "Category: " << myBooks.at(i)->getCategory() << endl;
            available = true;
        }
    }
    if(available = false){
        cout << "No available books" << endl;
    }
}

void listRentals(vector<Book *> & myBooks){
    Person * ptr;
    //flag that raises when there are no available books
    bool checkedout = false;
    for(int i = 0; i < myBooks.size(); i++){
        ptr = myBooks.at(i)->getPersonPtr();
        if (ptr != nullptr){
            cout << "\nBook ID: " << myBooks.at(i)->getId() << endl;
            cout << "Title: " << myBooks.at(i)->getTitle() << endl;
            cout << "Author: " << myBooks.at(i)->getAuthor() << endl;
            cout << "Cardholder: " << ptr->fullName() << endl;
            cout << "Card ID: " << ptr->getId() << endl;
            checkedout = true;
        }
    }
    if(checkedout = false){
        cout << "No outstanding rentals" << endl;
    }
}

void listMemberRentals(vector<Book *> & myBooks, vector<Person *> & myCardholders){
    Person * personPtr = getValidPerson(myCardholders);

    bool myFlag = false;
    for(int i = 0; i < myBooks.size(); i++){
        if(myBooks.at(i)->getPersonPtr() == personPtr){
            cout << "\nBook ID: " << myBooks.at(i)->getId() << endl;
            cout << "Title: " << myBooks.at(i)->getTitle() << endl;
            cout << "Author: " << myBooks.at(i)->getAuthor() << endl;
            myFlag = true;
        }
    }
    if(myFlag == false){
        cout << "No books currently checked out" << endl;
    }
}

void openCard(vector<Person *> & myCardholders, int newId){
    //get name from user
    //concatenate to fullname
    //function to compare fullnames
    //--if fullnames are the same, return pointer to person in vector
    //if person pointer is nullptr, create a new cardholder
    //else if person pointer is not nullptr,  set person to active
    cout << "Please enter the first name: ";
    string fName;
    cin >> fName;
    cout << "Please enter the last name: ";
    string lName;
    cin >> lName;
    string fullName = fName + " " + lName;

    Person * personPtr = searchByName(myCardholders, fullName);

    if (personPtr == nullptr){
        //create cardholder
        myCardholders.push_back(new Person(newId, true, fName, lName));
        personPtr = myCardholders.back();
    }
    else{
        //make active
        personPtr->setActive(true);
    }
    cout << "Card ID " << personPtr->getId() << " active" << endl;
    cout << "Cardholder: " << personPtr->fullName() << endl;
}

void closeCard(vector<Person *> & myCardholders){
    Person * personPtr = getValidPerson(myCardholders);

    if (personPtr != nullptr){
        cout << "Are you sure you want to deactivate card(y/n)? ";
        char yn;
        cin >> yn;
        if (yn == 'y'){
            personPtr->setActive(false);
            cout << "Card Deactivated" << endl;
        }
    }
}
//--------------
//Person Handlers
//--------------


//Simply searches for a person by id and returns a pointer to it
Person * searchPerson(vector<Person *> & myCardholders, int id){
    Person * ptr = nullptr;
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
//returns a pointer to it or nullptr
Person * getValidPerson(vector<Person *> & myCardholders){
    Person * ptr = nullptr;
    
    cout << "Please enter the card ID: ";
    int id;
    cin >> id;
    
    if(!cin.fail()){
        cinFullClear();
        //try to look for the person whose id matches what was entered
        ptr = searchPerson(myCardholders, id);

        //if it matched an id, proceed
        if(ptr != nullptr){

            //if the cardholder is active, proceed
            if(ptr->isActive() == true){
                cout << "Cardholder: " << ptr->fullName() << endl;
            }

            //if not active, just show a message
            if (ptr->isActive() == false){
                cout << "Card is inactive" << endl;
                ptr = nullptr;
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

Person * searchByName(vector<Person *> & myCardholders, string fullName){
    Person * ptr = nullptr;

    for(int i = 0; i < myCardholders.size(); i++){
        if(myCardholders.at(i)->fullName().compare(fullName) == 0){
            ptr = myCardholders.at(i);
            cout << "tick" << endl;
        }
    }
    return ptr;
}


//----------------
//Book Handlers
//----------------

//simply searches for a book by id and returns a pointer to it
Book * searchBook(vector<Book *> & myBooks, int id){
    Book * ptr = nullptr;
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
//returns a pointer to a book or nullptr
Book * getAvailBook(vector<Book *> & myBooks){
    Book * ptr = nullptr;

    cout << "Please enter the book ID: ";
    int id;
    cin >> id;

    if(!cin.fail()){
        cinFullClear();
        ptr = searchBook(myBooks, id);

        if(ptr != nullptr){
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




//----------
//Main code
//----------
int main()
{
    vector<Book *> books;
    vector<Person *> cardholders;
    int nextId = readPersons(cardholders);
    readBooks(books);
    readRentals(books, cardholders);

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
                returnBook(books);
                break;

            case 3:
                // View all available books
                listAvailableBooks(books);
                break;

            case 4:
                // View all outstanding rentals
                listRentals(books);
                break;

            case 5:
                // View outstanding rentals for a cardholder
                listMemberRentals(books, cardholders);
                break;

            case 6:
                // Open new library card
                openCard(cardholders, nextId);
                break;

            case 7:
                // Close library card
                closeCard(cardholders);
                break;
                
            case 8:
                // Must update records in files here before exiting the program
                //cleanPtrs();
                writePersons(cardholders);
                writeRentals(books);
                break;

            default:
                cout << "Invalid entry" << endl;
                break;
        }
        cout << endl;
   } while(choice != 8);
      return 0;
}
