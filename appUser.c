#include <stdio.h>
#include <string.h>
#include "C:\Users\USER\OneDrive\Masa�st�\hugin_work\hugin\sqlite_lib\sqlite3.h"
#include "appUser.h"
#include <time.h> // time fonksiyonu i�in gerekli k�t�phane

// Kullan�c� ad� ve soyad�n� tutacak karakter dizileri
char loggedInUserId[50];
char loggedInNameSurname[100];

int loggedIn = 0; // Varsay�lan olarak giri� yap�lmam�� olarak ba�lat�n

void logout() {
    // Giri� yapm�� kullan�c�y� oturumdan ��kartma i�lemlerini burada ger�ekle�tirin
    loggedIn = 0; // Kullan�c� oturumunu sonland�r�n

    // SQLite veritaban�na ba�lant� sa�lama
    sqlite3* db;
    int rc = sqlite3_open_v2(u8"C:\\databese\\upload_system.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Ge�erli tarih ve saat bilgisi al�nmas�
    time_t now;
    time(&now);
    char logoutTime[50];
    strftime(logoutTime, sizeof(logoutTime), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // "Audit" tablosuna kay�t eklemek i�in sorgu olu�turma
    char query[200];
    snprintf(query, sizeof(query), "UPDATE Audit SET LogoutTime = '%s' WHERE UserId = '%s' AND LogoutTime IS NULL", logoutTime, loggedInUserId);

    // Sorguyu �al��t�rma
    rc = sqlite3_exec(db, query, 0, 0, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }

    // Veritaban� ba�lant�s�n� kapatma
    sqlite3_close(db);
}





int login() {
    // Giri� i�lemlerini burada tamamlay�n
    char userId[50]; // Kullan�c� ad�n� tutacak karakter dizisi
    char password[50]; // �ifreyi tutacak karakter dizisi
    int loginSuccessful = 0; // Giri� ba�ar�s�z� kontrol etmek i�in de�i�ken

    printf("Enter User Id: ");
    scanf("%s", userId);

    printf("Enter Password: ");
    scanf("%s", password);

    // SQLite veritaban�na ba�lant� sa�lama
    sqlite3* db;
    int rc = sqlite3_open_v2(u8"C:\\databese\\upload_system.db", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    if (rc) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        return 0; // Giri� ba�ar�s�z
    }

    // Kullan�c� bilgilerini do�rulama i�in sorgu
    char query[200];
    snprintf(query, sizeof(query), u8"SELECT * FROM users WHERE Id='%s' AND Password='%s'", userId, password);

    // Sorguyu �al��t�rma ve sonu�lar� almak i�in haz�rl�k
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 0; // Giri� ba�ar�s�z
    }

    // Sorguyu �al��t�rma ve sonucu kontrol etme
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Kullan�c� bulundu, giri� ba�ar�l�
        loginSuccessful = 1;

        // Kullan�c�n�n ad� ve soyad�n� al�n
        snprintf(loggedInUserId, sizeof(loggedInUserId), "%s", userId);
        snprintf(loggedInNameSurname, sizeof(loggedInNameSurname), "%s", sqlite3_column_text(stmt, 1)); // NameSurname s�tunu kullan�c� ad� ve soyad�n� i�eriyor, burada indeksine g�re al�yoruz

        // Login i�lemi ba�ar�l� oldu�unda LoginTime alan�na giri� zaman�n� ve kullan�c�n�n ad� soyad�n� ekleyin
        snprintf(query, sizeof(query), "INSERT INTO Audit (UserId, NameSurname, LoginTime) VALUES ('%s', '%s', DATETIME('now', 'localtime'))", loggedInUserId, loggedInNameSurname);
        rc = sqlite3_exec(db, query, 0, 0, 0);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        }
    }

    // Bellek temizle
    // Bellek temizleme ve veritaban� ba�lant�s�n� kapatma
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    if (loginSuccessful) {
        loggedIn = 1; // Giri� ba�ar�l�, loggedIn de�i�kenini 1 olarak ayarlay�n
        printf("Login successful.\n"); // Login ba�ar�l� mesaj�n� burada yazd�k
        return 1; // Giri� ba�ar�l�
    }
    else {
        loggedIn = 0; // Giri� ba�ar�s�z, loggedIn de�i�kenini 0 olarak b�rak�n
        printf("Login failed.\n");
        return 0; // Giri� ba�ar�s�z
    }
}
