#include <stdio.h>
#include <string.h>
#include "C:\Users\USER\OneDrive\Masaüstü\hugin_work\hugin\sqlite_lib\sqlite3.h"
#include "appUser.h"
#include <time.h> // time fonksiyonu için gerekli kütüphane

// Kullanýcý adý ve soyadýný tutacak karakter dizileri
char loggedInUserId[50];
char loggedInNameSurname[100];

int loggedIn = 0; // Varsayýlan olarak giriþ yapýlmamýþ olarak baþlatýn

void logout() {
    // Giriþ yapmýþ kullanýcýyý oturumdan çýkartma iþlemlerini burada gerçekleþtirin
    loggedIn = 0; // Kullanýcý oturumunu sonlandýrýn

    // SQLite veritabanýna baðlantý saðlama
    sqlite3* db;
    int rc = sqlite3_open_v2(u8"C:\\databese\\upload_system.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Geçerli tarih ve saat bilgisi alýnmasý
    time_t now;
    time(&now);
    char logoutTime[50];
    strftime(logoutTime, sizeof(logoutTime), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // "Audit" tablosuna kayýt eklemek için sorgu oluþturma
    char query[200];
    snprintf(query, sizeof(query), "UPDATE Audit SET LogoutTime = '%s' WHERE UserId = '%s' AND LogoutTime IS NULL", logoutTime, loggedInUserId);

    // Sorguyu çalýþtýrma
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }

    // Veritabaný baðlantýsýný kapatma
    sqlite3_close(db);
}





int login() {
    // Giriþ iþlemlerini burada tamamlayýn
    char userId[50]; // Kullanýcý adýný tutacak karakter dizisi
    char password[50]; // Þifreyi tutacak karakter dizisi
    int loginSuccessful = 0; // Giriþ baþarýsýzý kontrol etmek için deðiþken

    printf("Enter User Id: ");
    scanf("%s", userId);

    printf("Enter Password: ");
    scanf("%s", password);

    // SQLite veritabanýna baðlantý saðlama
    sqlite3* db;
    int rc = sqlite3_open_v2(u8"C:\\databese\\upload_system.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 0; // Giriþ baþarýsýz
    }

    // Kullanýcý bilgilerini doðrulama için sorgu
    char query[200];
    snprintf(query, sizeof(query), u8"SELECT * FROM users WHERE Id='%s' AND Password='%s'", userId, password);

    // Sorguyu çalýþtýrma ve sonuçlarý almak için hazýrlýk
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0; // Giriþ baþarýsýz
    }

    // Sorguyu çalýþtýrma ve sonucu kontrol etme
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Kullanýcý bulundu, giriþ baþarýlý
        loginSuccessful = 1;

        // Kullanýcýnýn adý ve soyadýný alýn
        snprintf(loggedInUserId, sizeof(loggedInUserId), "%s", userId);
        snprintf(loggedInNameSurname, sizeof(loggedInNameSurname), "%s", sqlite3_column_text(stmt, 1)); // NameSurname sütunu kullanýcý adý ve soyadýný içeriyor, burada indeksine göre alýyoruz

        // Login iþlemi baþarýlý olduðunda LoginTime alanýna giriþ zamanýný ve kullanýcýnýn adý soyadýný ekleyin
        snprintf(query, sizeof(query), "INSERT INTO Audit (UserId, NameSurname, LoginTime) VALUES ('%s', '%s', DATETIME('now', 'localtime'))", loggedInUserId, loggedInNameSurname);
        rc = sqlite3_exec(db, query, 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        }
    }

    // Bellek temizle
    // Bellek temizleme ve veritabaný baðlantýsýný kapatma
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (loginSuccessful) {
        loggedIn = 1; // Giriþ baþarýlý, loggedIn deðiþkenini 1 olarak ayarlayýn
        printf("Login successful.\n"); // Login baþarýlý mesajýný burada yazdýk
        return 1; // Giriþ baþarýlý
    }
    else {
        loggedIn = 0; // Giriþ baþarýsýz, loggedIn deðiþkenini 0 olarak býrakýn
        printf("Login failed.\n");
        return 0; // Giriþ baþarýsýz
    }
}
