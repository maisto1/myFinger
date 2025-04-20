#ifndef MY_FINGER_H
#define MY_FINGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <utmp.h>
#include <utmpx.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>


#define MAX_USER 200

// Struttura User che mantiene i valori estratti dal campo gecos di passwd
struct User {
    char login_name[MAX_USER];
    int uid;
    int gid;
    char home_dir[MAX_USER];
    char shell[MAX_USER];
    char name[MAX_USER];
    char office_n[MAX_USER];
    char phone[MAX_USER];
    char home_phone[MAX_USER];
};  

// Struttura UtmpInfo che mantiene i valori estratti dal file utmp
struct UtmpInfo {
    char tty[MAX_USER];
    time_t login_time;
};

// Ottiene il valore associato a una chiave
const char *getValue(const char *key);

// Crea una struttura UtmpInfo per l'utente specificato
struct UtmpInfo creaUtmpInfo(const char *username);

// Crea una struttura User a partire da una struttura passwd
struct User creaUser(struct passwd *pw);

// Converte un timestamp in una stringa formattata
char *creaT(time_t timestamp);

// Calcola il tempo di inattività di un utente
int getIdle(struct UtmpInfo user_utmp);

// Converte un timestamp in ore
int getHours(int i);

// Converte un timestamp in minuti
int getMinutes(int i, int h);

// Converte un timestamp in secondi
int getSeconds(int i);

// Legge un file specifico nella directory home dell'utente
void readFile(const char *home_dir, char *filename);

// Legge e visualizza il contenuto della carte /var/mail dell'utente
void readMail(const char *username);

// Stampa informazioni sull'utente in formato breve
void s_option(struct User u, struct UtmpInfo ut);

// Stampa informazioni sull'utente in formato dettagliato
void l_option(struct User u, struct UtmpInfo ut, int opt);

// Cerca un utente nel file passwd tramite cognome
struct passwd* getpwbylastname(const char* lastname);

// Processa un utente specificato con le opzioni -s, -l, -p, -m
void process_user(const char *username, int s, int l, int p, int m);

// Ottiene una lista di tutti gli utenti attivi e li processa
void get_active_users(int s, int l, int p, int m);

int main(int argc, char *argv[]);

#endif