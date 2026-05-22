/*=========================================================
    PROJECT NAME : BANK ACCOUNT MANAGEMENT SYSTEM
    FILE NAME    : cdetrans.c
    DESCRIPTION  : Random Access File Processing in C
==========================================================*/

#include <stdio.h>
#include <stdlib.h>

/* structure definition */
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

/* function prototypes */
unsigned int enterChoice(void);
void createTextFile(FILE *readPtr);
void updateAccount(FILE *fPtr);
void addAccount(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void displayAccounts(FILE *fPtr);

/* main function */
int main()
{
    FILE *cfPtr;
    unsigned int choice;

    /* open file */
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        /* create file if it does not exist */
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be opened.\n");
            exit(1);
        }

        /* initialize file with empty records */
        struct clientData blankClient = {0, "", "", 0.0};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blankClient,
                   sizeof(struct clientData),
                   1,
                   cfPtr);
        }

        rewind(cfPtr);
    }

    printf("\n=========================================");
    printf("\n BANK ACCOUNT MANAGEMENT SYSTEM");
    printf("\n=========================================\n");

    /* menu loop */
    while ((choice = enterChoice()) != 6)
    {
        switch (choice)
        {
        case 1:
            addAccount(cfPtr);
            break;

        case 2:
            updateAccount(cfPtr);
            break;

        case 3:
            deleteAccount(cfPtr);
            break;

        case 4:
            displayAccounts(cfPtr);
            break;

        case 5:
            createTextFile(cfPtr);
            break;

        default:
            printf("\nInvalid Choice.\n");
        }
    }

    fclose(cfPtr);

    printf("\nProgram Ended Successfully.\n");

    return 0;
}

/* menu function */
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n========== MAIN MENU ==========\n");
    printf("1. Add New Account\n");
    printf("2. Update Account\n");
    printf("3. Delete Account\n");
    printf("4. Display Accounts\n");
    printf("5. Create Text File\n");
    printf("6. Exit\n");
    printf("Enter your choice : ");

    scanf("%u", &choice);

    return choice;
}

/* add account */
void addAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("\nEnter New Account Number (1-100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter Last Name : ");
        scanf("%14s", client.lastName);

        printf("Enter First Name : ");
        scanf("%9s", client.firstName);

        printf("Enter Balance : ");
        scanf("%lf", &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("\nAccount Added Successfully.\n");
    }
}

/* update account */
void updateAccount(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;
    double amount;

    printf("\nEnter Account Number to Update : ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        printf("\nAccount Details\n");
        printf("---------------------------------\n");
        printf("Account Number : %u\n",
               client.acctNum);

        printf("Customer Name  : %s %s\n",
               client.firstName,
               client.lastName);

        printf("Current Balance: %.2f\n",
               client.balance);

        printf("\nEnter Amount\n");
        printf("(+ Deposit / - Withdraw): ");

        scanf("%lf", &amount);

        client.balance += amount;

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("\nBalance Updated Successfully.\n");

        printf("New Balance : %.2f\n",
               client.balance);
    }
}

/* delete account */
void deleteAccount(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("\nEnter Account Number to Delete : ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("\nAccount Deleted Successfully.\n");
    }
}

/* display all accounts */
void displayAccounts(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    rewind(fPtr);

    printf("\n=====================================================\n");
    printf("%-10s %-15s %-15s %-10s\n",
           "Acc No",
           "Last Name",
           "First Name",
           "Balance");

    printf("=====================================================\n");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-10u %-15s %-15s %-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

/* create text file */
void createTextFile(FILE *readPtr)
{
    FILE *writePtr;

    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be created.\n");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "=============================================\n");

        fprintf(writePtr,
                "        ACCOUNT DETAILS REPORT\n");

        fprintf(writePtr,
                "=============================================\n\n");

        fprintf(writePtr,
                "%-10s %-15s %-15s %-10s\n",
                "Acc No",
                "Last Name",
                "First Name",
                "Balance");

        fprintf(writePtr,
                "--------------------------------------------------\n");

        while (fread(&client,
                     sizeof(struct clientData),
                     1,
                     readPtr))
        {
            if (client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-10u %-15s %-15s %-10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.balance);
            }
        }

        fclose(writePtr);

        printf("\naccounts.txt File Created Successfully.\n");
    }
}