
#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio_ext.h>


typedef struct {
    char name[50];
    char phone[11];
    char email[50];
} Contact;


typedef struct {
    Contact contacts[100];
    int contactCount;
} AddressBook;


void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContacts(AddressBook *addressBook);

#endif

