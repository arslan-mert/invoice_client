
// appInvoice.c
#include <stdio.h>
#include <conio.h>
#include <wchar.h> // wchar_t ve ilgili fonksiyonlar için gerekli kütüphane
#include "appInvoice.h"

void showInvoiceList() {
    printf("Invoice List:\n");
    printf("1. HizliTicaret.inv\n");
    printf("2. HakanUslu.inv\n");
    printf("3. AyseYavuz.inv\n");
    printf("Press ESC to go back to Select Operation menu.\n");
    printf("Enter the number of the invoice you want to view: ");

    wchar_t ch = _getwch();
    if (ch == 27) {
        return; // Eðer ESC tuþuna basýlýrsa, "Select Operation" menüsüne geri dönün
    }

    int invoiceNumber = ch - L'0'; // Kullanýcýnýn girdiði karakteri sayýya dönüþtürün
    showInvoiceContent(invoiceNumber);
}

void showInvoiceContent(int invoiceNumber) {
    wchar_t filename[100];

    switch (invoiceNumber) {
    case 1:
        swprintf(filename, sizeof(filename), L"C:\\databese\\Hizli Ticaret.inv");
        break;
    case 2:
        swprintf(filename, sizeof(filename), L"C:\\databese\\Hakan Uslu.inv");
        break;
    case 3:
        swprintf(filename, sizeof(filename), L"C:\\databese\\Ayse Yavuz.inv");
        break;
    default:
        wprintf(L"Invalid invoice number. Please try again.\n");
        return;
    }

    FILE* file = _wfopen(filename, L"r");
    if (file == NULL) {
        wprintf(L"Error opening file %ls\n", filename);
        return;
    }

    wprintf(L"Invoice Content of %ls:\n", filename);

    wchar_t line[256];
    while (fgetws(line, sizeof(line), file) != NULL) {
        wprintf(L"%ls", line);
    }

    fclose(file);
}








