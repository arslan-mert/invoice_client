// appMain.c
#include <stdio.h>
#include "appUser.h"
#include "appInvoice.h"

int main() {
    int choice;
    char ch;

    do {
        printf("Select Operation:\n");
        printf("1. Login\n");
        printf("2. Invoice List\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // Login i�lemi
            login();
            break;
        case 2:
            // Invoice List i�lemi
            showInvoiceList();
            break;
        case 3:
            // Logout i�lemi
            logout();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }

        // Kullan�c�n�n devam edip etmeyece�ini kontrol etme
        printf("Press ESC to return to Select Operation menu, or any other key to continue: ");
        ch = getch();
        if (ch == 27) { // 27 is the ASCII code for ESC key
            choice = -1; // Men�ye geri d�nmek i�in choice'� -1 yapar�z
        }
    } while (choice == -1);

    return 0;
}

