
#include "contact.h"

void listContacts(AddressBook *addressBook)
{
    printf("S.No    Name                Phone       Email\n");
    printf("------------------------------------------------------\n");

    if (addressBook->contactCount == 0) {
        printf("No contacts found.\n");
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++) {
        printf("%-6d  %-18s  %-8s  %s\n", i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
}



void createContact(AddressBook *addressBook)
{
    printf("Enter createContact\n");
    char name[50], phone[11], email[50];
    int isDuplicate;

    while (1)
    {
        isDuplicate = 0;
        printf("Enter Name: ");
        scanf("%49s", name);
        while (getchar() != '\n'); // Flush buffer

        printf("Enter Phone: ");
        scanf("%10s", phone);
        while (getchar() != '\n'); // Flush buffer

        printf("Enter Email: ");
        scanf("%49s", email);
        while (getchar() != '\n'); // Flush buffer

        // Check if name already exists
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcmp(addressBook->contacts[i].name, name) == 0) {
                printf("Name already exists!\n");
                isDuplicate = 1;
                break;
            }
        }
        if (isDuplicate) continue;

        // Check if phone number already exists
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcmp(addressBook->contacts[i].phone, phone) == 0) {
                printf("Phone number already exists!\n");
                isDuplicate = 1;
                break;
            }
        }
        if (isDuplicate) continue;

        // Check if email already exists
        for (int i = 0; i < addressBook->contactCount; i++) {
            if (strcmp(addressBook->contacts[i].email, email) == 0) {
                printf("Email already exists!\n");
                isDuplicate = 1;
                break;
            }
        }
        if (isDuplicate) continue;

        // Validate Name (only letters & spaces)
        for (int i = 0; name[i] != '\0'; i++) {
            if (!isalpha(name[i]) && name[i] != ' ') {
                printf("Invalid name! Only letters & spaces allowed.\n");
                isDuplicate = 1;
                break;
            }
        }
        if (isDuplicate) continue;

        // Validate Phone (only 10-digit numbers)
        if (strlen(phone) != 10) {
            printf("Invalid phone number! Must be exactly 10 digits.\n");
            continue;
        }
        for (int i = 0; i < 10; i++) {
            if (!isdigit(phone[i])) {
                printf("Invalid phone number! Only digits allowed.\n");
                isDuplicate = 1;
                break;
            }
        }
        if (isDuplicate) continue;

        // Validate Email (@ and ".com" must be present)
        if (!strchr(email, '@') || !strstr(email, ".com")) {
            printf("Invalid email! Must contain '@' and end with '.com'.\n");
            continue;
        }

        // Add new contact
        if (addressBook->contactCount < 100) {
            strcpy(addressBook->contacts[addressBook->contactCount].name, name);
            strcpy(addressBook->contacts[addressBook->contactCount].phone, phone);
            strcpy(addressBook->contacts[addressBook->contactCount].email, email);
            addressBook->contactCount++;

            printf("Contact added successfully!\n");

            // **Call listContacts() to print updated contacts**
            //listContacts(addressBook);
        } else {
            printf("Error: Address book is full.\n");
        }
        break;
    }
}

void searchContact(AddressBook *addressBook)
{
    printf("Enter search contact\n");
    printf("---------Menu for the Search contact--------\n1. Name\n2. Phone\n3. Email\n");
    int n;
    scanf("%d", &n);
    getchar(); // Flush input buffer
    printf("\n");
    switch(n)
    {
        case 1:
                char name[50];
                printf("Based on name: \n");
                scanf("%49[^\n]", name);
                getchar();
                for (int i = 0; i < addressBook->contactCount; i++) {
                    if (strstr(addressBook->contacts[i].name, name)) {
                        printf("%-6d  %-20s  %-10s  %s\n", i + 1,
                               addressBook->contacts[i].name,
                               addressBook->contacts[i].phone,
                               addressBook->contacts[i].email);
                    }
                }
            break;
        case 2:
                char phone[11];
                printf("Based on phone: ");
                scanf("%10s", phone);
                getchar();
                for (int i = 0; i < addressBook->contactCount; i++) {
                    if (strcmp(addressBook->contacts[i].phone, phone) == 0) {
                        printf("%-6d  %-20s  %-10s  %s\n", i + 1,
                               addressBook->contacts[i].name,
                               addressBook->contacts[i].phone,
                               addressBook->contacts[i].email);
                    }
                }
            break;
        case 3:
                char email[50];
                printf("Based on email: ");
                scanf("%49s", email);
                getchar();
                for (int i = 0; i < addressBook->contactCount; i++) {
                    if (strcmp(addressBook->contacts[i].email, email) == 0) {
                        printf("%-6d  %-20s  %-10s  %s\n", i + 1,
                               addressBook->contacts[i].name,
                               addressBook->contacts[i].phone,
                               addressBook->contacts[i].email);
                    }
                }
            break;
        default:
            printf("Invalid input!!\n");
            break;
    }
}


void editContact(AddressBook *addressBook)
{
    printf("\nEnter edit contact\n");
    printf("--------- Search Contact to Edit --------\n1. Name\n2. Phone\n3. Email\n");

    int choice, i, contactIndex = -1;
    int number, count = 0;
    char name[50], phone[11], email[50];

    printf("Enter your choice: ");
    scanf("%d", &choice);
    while (getchar() != '\n'); // Flush input buffer

    switch(choice)
    {
        case 1:
            printf("Search by Name: ");
            scanf("%49s", name);
            while (getchar() != '\n');// Flush input buffer


            for(i = 0; i < addressBook->contactCount; i++)
            {
                if(strstr(addressBook->contacts[i].name, name))
                {
                    count++;
                    printf("%-6d  %-20s  %-10s  %s\n", i + 1,
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                }
            }
            printf("Enter the serial number of the contact you want to edit: ");
            scanf("%d", &number);

            // Validate the serial number
            if(number > 0 && number <= addressBook->contactCount)
            {
                contactIndex = number - 1; // Adjust for 0-indexing
            }
            else
            {
                printf("Invalid serial number\n");
                break;
            }

            printf("Enter new Name: ");
            scanf("%49s", name);
            while (getchar() != '\n');// Flush input buffer
            printf("Enter new Phone: ");
            scanf("%10s", phone);
            while (getchar() != '\n'); // Flush input buffer
            printf("Enter new Email: ");
            scanf("%49s", email);
            while (getchar() != '\n'); // Flush input buffer

            // validate name
            if (!isalpha(name[i]) && name[i] != ' ')
            {
                printf("Invalid name! Only letters & spaces allowed.\n");
                break;
            }

            // Validate phone number
            if(strlen(phone) != 10)
            {
                printf("Invalid phone number.\n");
                break;
            }

            // Validate email
            if (!strchr(email, '@') || !strstr(email, ".com"))
            {
                printf("Invalid email! Must contain '@' and end with '.com'.\n");
                break;
            }

            // Update the contact with new details
            strcpy(addressBook->contacts[contactIndex].name, name);
            strcpy(addressBook->contacts[contactIndex].phone, phone);
            strcpy(addressBook->contacts[contactIndex].email, email);
            printf("Contact updated successfully!\n");
            break;

        case 2:
            printf("Search by Phone: ");
            scanf("%10s", phone);
            getchar(); // Flush input buffer

            for(i = 0; i < addressBook->contactCount; i++)
            {
                if(strstr(addressBook->contacts[i].phone, phone))
                {
                    count++;
                    printf("%-6d  %-20s  %-10s  %s\n", i + 1,
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                }
            }
            printf("Enter the serial number of the contact you want to edit: ");
            scanf("%d", &number);

            // Validate the serial number
            if(number>0 && number <= addressBook->contactCount)
            {
                contactIndex = number - 1; // Adjust for 0-indexing
            }
            else
            {
                printf("Invalid serial number\n");
                break;
            }

            printf("Enter new Name: ");
            scanf("%49s", name);
            getchar(); // Flush input buffer
            printf("Enter new Phone: ");
            scanf("%10s", phone);
            getchar(); // Flush input buffer
            printf("Enter new Email: ");
            scanf("%49s", email);
            getchar(); // Flush input buffer

            // validate name
            if (!isalpha(name[i]) && name[i] != ' ')
            {
                printf("Invalid name! Only letters & spaces allowed.\n");
                break;
            }

            // Validate phone number
            if(strlen(phone) != 10)
            {
                printf("Invalid phone number.\n");
                break;
            }

            // Validate email
            if (!strchr(email, '@') || !strstr(email, ".com"))
            {
                printf("Invalid email! Must contain '@' and end with '.com'.\n");
                break;
            }

            // Update the contact with new details
            strcpy(addressBook->contacts[contactIndex].name, name);
            strcpy(addressBook->contacts[contactIndex].phone, phone);
            strcpy(addressBook->contacts[contactIndex].email, email);
            printf("Contact updated successfully!\n");
            break;

        case 3:
            printf("Search by Email: ");
            scanf("%49s", email);
            getchar(); // Flush input buffer

            for(i = 0; i < addressBook->contactCount; i++)
            {
                if(strstr(addressBook->contacts[i].email, email))
                {
                    count++;
                    printf("%-6d  %-20s  %-10s  %s\n", i + 1,
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                }
            }
            printf("Enter the serial number of the contact you want to edit: ");
            scanf("%d", &number);

            // Validate the serial number
            if(number > 0 && number <= addressBook->contactCount)
            {
                contactIndex = number - 1; // Adjust for 0-indexing
            }
            else
            {
                printf("Invalid serial number\n");
                break;
            }

            printf("Enter new Name: ");
            scanf("%49s", name);
            getchar(); // Flush input buffer
            printf("Enter new Phone: ");
            scanf("%10s", phone);
            getchar(); // Flush input buffer
            printf("Enter new Email: ");
            scanf("%49s", email);
            getchar(); // Flush input buffer

            // validate name
            if (!isalpha(name[i]) && name[i] != ' ')
            {
                printf("Invalid name! Only letters & spaces allowed.\n");
                break;
            }

            // Validate phone number
            if(strlen(phone) != 10)
            {
                printf("Invalid phone number.\n");
                break;
            }

            // Validate email
            if (!strchr(email, '@') || !strstr(email, ".com"))
            {
                printf("Invalid email! Must contain '@' and end with '.com'.\n");
                break;
            }

            // Update the contact with new details
            strcpy(addressBook->contacts[contactIndex].name, name);
            strcpy(addressBook->contacts[contactIndex].phone, phone);
            strcpy(addressBook->contacts[contactIndex].email, email);
            printf("Contact updated successfully!\n");
            break;

        default:
            printf("Invalid choice\n");
            break;
    }
}

void deleteContact(AddressBook *addressBook)
{
    printf("\nEnter delete contact\n");
    printf("--------- Search Contact to Delete --------\n1. Name\n2. Phone\n3. Email\n");

    int choice, i, j, contactIndex = -1;
    int number, count = 0;
    char name[50], phone[11], email[50];

    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // Flush input buffer

    switch(choice)
    {
        case 1: // Search by Name
            printf("Search by Name: ");
            scanf("%49[^\n]", name);
            getchar(); // Flush input buffer

            for(i = 0; i < addressBook->contactCount; i++)
            {
                if(strstr(addressBook->contacts[i].name, name))
                {
                    count++;
                    printf("%-6d  %-20s  %-10s  %s\n", i + 1,
                           addressBook->contacts[i].name,
                           addressBook->contacts[i].phone,
                           addressBook->contacts[i].email);
                }
            }

            if(count == 0)
            {
                printf("No matching contact found.\n");
                return;
            }

            printf("Enter the serial number of the contact you want to delete: ");
            scanf("%d", &number);

            if(number > 0 && number <= addressBook->contactCount)
            {
                contactIndex = number - 1;
            }
            else
            {
                printf("Invalid serial number.\n");
                return;
            }

            // Shift contacts to remove the selected one
            for(j = contactIndex; j < addressBook->contactCount - 1; j++)
            {
                addressBook->contacts[j] = addressBook->contacts[j + 1];
            }

            addressBook->contactCount--; // Reduce total contacts
            printf("Contact deleted successfully!\n");
            break;

        case 2: // Search by Phone
            printf("Search by Phone: ");
            scanf("%10s", phone);
            getchar(); // Flush input buffer

            for(i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(addressBook->contacts[i].phone, phone) == 0)
                {
                    contactIndex = i;
                    break;
                }
            }

            if(contactIndex == -1)
            {
                printf("No matching contact found.\n");
                return;
            }

            // Shift contacts to remove the selected one
            for(j = contactIndex; j < addressBook->contactCount - 1; j++)
            {
                addressBook->contacts[j] = addressBook->contacts[j + 1];
            }

            addressBook->contactCount--; // Reduce total contacts
            printf("Contact deleted successfully!\n");
            break;

        case 3: // Search by Email
            printf("Search by Email: ");
            scanf("%49s", email);
            getchar(); // Flush input buffer

            for(i = 0; i < addressBook->contactCount; i++)
            {
                if(strcmp(addressBook->contacts[i].email, email) == 0)
                {
                    contactIndex = i;
                    break;
                }
            }

            if(contactIndex == -1)
            {
                printf("No matching contact found.\n");
                return;
            }

            // Shift contacts to remove the selected one
            for(j = contactIndex; j < addressBook->contactCount - 1; j++)
            {
                addressBook->contacts[j] = addressBook->contacts[j + 1];
            }

            addressBook->contactCount--; // Reduce total contacts
            printf("Contact deleted successfully!\n");
            break;

        default:
            printf("Invalid choice.\n");
            return;
    }
}
void saveContacts(AddressBook *addressBook)
{
    FILE *fptr;
    fptr = fopen("contact.txt", "w");
    if (fptr == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fptr, "%s,", addressBook->contacts[i].name); // Write name
        fprintf(fptr, "%s,", addressBook->contacts[i].phone); // Write phone
        fprintf(fptr, "%s", addressBook->contacts[i].email); // Write email
        fputc('\n', fptr); // Write newline
    }

    fclose(fptr); // Close file
    printf("Contacts saved successfully.\n");
}

