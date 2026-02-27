
#include "contact.h"

int main()
{
    AddressBook addressbook;
    initialize(&addressbook);
    while(1)
    {
        printf("----------Menu for the address book----------\n");
        printf("1. list of contacts\n2. create contacts\n3. search contact\n4. edit contact\n5. delete contact\n6. save contacts\n7. exit the addressbook\n");
        int n;
        scanf("%d", &n);
        switch(n)
        {
            case 1: listContacts(&addressbook);
            break;
            case 2: createContact(&addressbook);
            break;
            case 3: searchContact(&addressbook);
            break;
            case 4: editContact(&addressbook);
            break;
            case 5: deleteContact(&addressbook);
            break;
            case 6: saveContacts(&addressbook);
            break;
            case 7: exit(1);
            break;
            default: printf("It is invalid number...\n");
            break;
        }
    }
}

