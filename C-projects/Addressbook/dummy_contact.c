
#include "contact.h"
// Dummy contact data
static Contact dummyContacts[] = {
    {"John Doe", "1234567890", "john@example.com"},
    {"Alice Smith", "0987654321", "alice@example.com"},
    {"Bob Johnson", "1112223333", "bob@company.com"},
    {"Carol White", "4445556666", "carol@company.com"},
    {"David Brown", "7778889999", "david@example.com"},
    {"Eve Davis", "6665554444", "eve@example.com"},
    {"Frank Miller", "3334445555", "frank@example.com"},
    {"Grace Wilson", "2223334444", "grace@example.com"},
    {"Hannah Clark", "5556667777", "hannah@example.com"},
    {"Ian Lewis", "8889990000", "ian@example.com"}
};

void initialize(AddressBook *addressBook)
{
     FILE *fptr;
     fptr = fopen("contact.txt", "r");
    // assign 0 to addressBook->contactCount
    addressBook->contactCount = 0;
   // cal size of dummyContacts (10)
   int size = sizeof(dummyContacts)/sizeof(Contact);
   //run a loop for size of dummyContacts (10 time)
   for(int i = 0; i < size; i++)
   {
        //1. copy dummyContacts[index] to addressbook->contacts[index]
        addressBook->contacts[i] = dummyContacts[i];
        //2. inc (addressBook->contactCount)++
         (addressBook->contactCount)++;

   }

   if(fptr == NULL)
   {
        printf("File does not exist!!\n");
        return ;
   }
   addressBook->contactCount = 0;
   while(fscanf(fptr, "%49[^\n], %10s, %49[^\n]\n",addressBook->contacts[addressBook->contactCount].name,
               addressBook->contacts[addressBook->contactCount].phone,
               addressBook->contacts[addressBook->contactCount].email) == 3)
               {
                    (addressBook->contactCount)++;
               }
    fclose(fptr);


}
