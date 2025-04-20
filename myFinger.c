#include "myFinger.h"


const char *keys[4] = {"plan", "pgpkey", "project","forward"};
const char *values[4] = {"Plan:\n", "PGP key:\n", "Project:\n","Mail forwarded to "};

const char *getValue(const char *key) {
    // funzione che attraverso una chiave pernette di ottenere il valore associato
    for (int i = 0; i < 4; ++i) {
        if (strcmp(keys[i], key) == 0) {
            return values[i];
        }
    }
    return NULL;
};

struct UtmpInfo creaUtmpInfo(const char *username) {
    struct utmp *ut;
    struct UtmpInfo info;

    setutent(); // Inizia la lettura di utmp

    while ((ut = getutent()) != NULL) {
        if (ut->ut_type == USER_PROCESS && strncmp(ut->ut_user, username, UT_NAMESIZE) == 0) {
            // Copia il tty (ut_line) se disponibile, altrimenti imposta a '*'
            strcpy(info.tty, ut->ut_line != NULL ? ut -> ut_line : "*");
            info.login_time = ut->ut_tv.tv_sec;
            break;
        }
    }

    endutent(); // Chiude la lettura di utmp

    return info;
}


struct User creaUser(struct passwd *pw) {
    struct User user;
    char formattedPhone[50];

    // Inizializzazione dei campi della struct User
    strcpy(user.login_name, pw->pw_name);
    user.uid = pw->pw_uid;
    user.gid = pw->pw_gid;
    strcpy(user.home_dir, pw->pw_dir ? pw->pw_dir : "");
    strcpy(user.shell, pw->pw_shell ? pw->pw_shell : "");

    // Inizializzazione dei campi da pw_gecos
    if (pw->pw_gecos) {
        char *token = strtok(pw->pw_gecos, ",");
        
        // Copia del nome
        strcpy(user.name, token ? token : "");

        // Copia dell'ufficio
        token = strtok(NULL, ",");
        strcpy(user.office_n, token ? token : "");

        // Formattazione e copia del telefono
        token = strtok(NULL, ",");
        if (token) {
            snprintf(formattedPhone, sizeof(formattedPhone), "%.3s-%.3s-%.4s", token, token + 3, token + 6);
            strcpy(user.phone, formattedPhone);
        } else {
            strcpy(user.phone, "");
        }

        // Copia del telefono di casa
        token = strtok(NULL, ",");
        strcpy(user.home_phone, token ? token : "");
    } else {
        // Se pw_gecos non è presente, inizializza i campi con stringhe vuote
        strcpy(user.name, "");
        strcpy(user.office_n, "");
        strcpy(user.phone, "");
        strcpy(user.home_phone, "");
    }

    return user;
}


char *creaT(time_t timestamp) {
    struct tm *tm_info;
    char *t = malloc(50 * sizeof(char));  // Allocare memoria dinamicamente per la stringa
    if (t == NULL) {
        // Gestire errore di allocazione della memoria
        return NULL;
    }
    tm_info = localtime(&timestamp);  // Convertire il timestamp in struct tm locale
    strftime(t, 50, "%a %b %d %H:%M (%Z)", tm_info);

    return t;  // Restituire il puntatore alla stringa allocata dinamicamente
}

int getIdle(struct UtmpInfo user_utmp){
    // funzione che cacola l'idle time dell'utente
    time_t current_time = time(NULL);
    int idle = current_time - user_utmp.login_time;
    return idle;
}

int getHours(int i){
    return i / 3600; //converte il timestamp in ore
}

int getMinutes(int i, int h){
    return (i - (h * 3600)) / 60; //converte il timestamp in minuti
}

int getSeconds(int i){
    return i % 60; //converte il timestamp in secondi
}

void readFile(const char *home_dir, char *filename){
    // Funzione che legge file passato per argomento e printa il contenuto a schermo
    FILE *file;
    char filepath[200], ch;

    snprintf(filepath, sizeof(filepath), "%s/.%s",home_dir,filename);

    file = fopen(filepath,"r");
    if (file == NULL & filename == "plan"){
        printf("No plan.\n");
        return;
    } else if(file == NULL){
        return;
    }

    printf("%s",getValue(filename));
    while ((ch = fgetc(file)) != EOF){
        putchar(ch);
    }

    fclose(file);
}

void readMail(const char *username){
    // Funzione che legge il file .mail e printa a schermo il suo contenuto
    struct stat fileStat;
    char path[50];

    snprintf(path, sizeof(path), "/var/mail/%s", username);

    if (stat(path, &fileStat) == -1) {
        printf("No mail.\n");
        return;
    }

    char *accessTime = creaT(fileStat.st_atime); 

    char *creationTime = creaT(fileStat.st_ctime); 
     
    // Controllo se ci sono nuove mail
    if(accessTime < creationTime){
        printf("Mail last read %s\n",accessTime); // Non ci sono nuove mail
    }else{
        printf("New mail recieved %s\n",creationTime); // Ci sono nuove mail
    }
}

void s_option(struct User u, struct UtmpInfo ut){
    
    struct tm *tm_info;
    char login_time[100] = "No logins";
    char idle[100] = "*";

    if(localtime(&ut.login_time) != NULL){
        tm_info = localtime(&ut.login_time);
        strftime(login_time, sizeof(login_time), "%b %d %H:%M", tm_info);
        int i_t = getIdle(ut);
        int h = getHours(i_t);
        int m = getMinutes(i_t, h);
        sprintf(idle, h == 0 ? "%d" : "%02d:%02d", h == 0 ? m : h, m);
    }
    
    printf("Login       Name                  Tty        Idle     Login Time    Office      Office Phone\n");
    printf("%-10s  %-20s  %-9s  %-7s  %-12s  %-10s  %s\n",
           u.login_name, u.name, ut.tty, idle, login_time, u.office_n, u.phone
    );
};

void l_option(struct User u, struct UtmpInfo ut, int opt){
     
    // creaT mi permette di formattare il login_time proprio come lo formatta finger
    char *login_time = ctime(&ut.login_time) != NULL ? creaT(ut.login_time) : "Never logged in.";
    char idle[100];
    if (ctime(&ut.login_time) != NULL){
        int i_t = getIdle(ut); // ottengo idle time
        int h = getHours(i_t); // ottengo le ore dall'idle
        int m = getMinutes(i_t,h); // ottengo i minuti dall'idle
        int s = getSeconds(i_t);    // ottengo i secondi dall'idle
        sprintf(idle, h == 0 ? "    %d minutes %d seconds" : "%d hours %d minutes",h == 0 ? m : h, h == 0 ? s : m);
    }


    printf("Login: %s                      Name: %s\n", u.login_name, u.name);
    printf("Directory: %s            Shell: %s\n", u.home_dir, u.shell);

    if (u.office_n[0] != '\0' || u.phone[0] != '\0') {
        printf("Office: %s, %s                Home Phone: %s\n", u.office_n, u.phone, u.home_phone);
    }
    if(ctime(&ut.login_time) != NULL){
        printf("On since %s on %s from %s\n", login_time,ut.tty,ut.tty);
        printf("%s idle\n",idle);
    }

    
    readFile(u.home_dir,"forward"); // Funzione che mi permette di leggere file in questo caso *forward*
    readMail(u.login_name); // Funzione che mi permette di leggere il file contenente le mail

    if(opt){
        readFile(u.home_dir,"pgpkey");
        readFile(u.home_dir,"project");
        readFile(u.home_dir,"plan");
    }
}

struct passwd* getpwbylastname(const char* lastname) {
    struct passwd* pw;
    char* token;
    char* saveptr;

    setpwent(); // Ripristina la lettura del file passwd

    while ((pw = getpwent()) != NULL) {
        // Duplica la stringa pw_gecos per non modificarla direttamente
        char* gecos_copy = strdup(pw->pw_gecos);
        if (gecos_copy == NULL) {
            continue;
        }

        // Divide la stringa GECOS per ottenere il campo nome completo
        token = strtok_r(gecos_copy, ",", &saveptr);
        if (token != NULL) {
            // Controlla se il cognome corrisponde
            if (strstr(token, lastname) != NULL) {
                free(gecos_copy);
                endpwent(); // Chiude la lettura del file passwd
                return pw;  // Restituisce il record dell'utente trovato
            }
        }

        free(gecos_copy);
    }

    endpwent(); // Chiude la lettura del file passwd
    return NULL; // Nessun utente trovato
}


void process_user(const char *username, int s, int l, int p, int m) {

    // Se username nullo mi prendo l'utente corrente
    if (username == NULL) {
        username = getlogin();
    }

    struct passwd *pw = getpwnam(username);

    if (!pw && !m) {
        pw = getpwbylastname(username); // Funzione che mi permette di trovare user attraverso il cognome
    }

    if (!pw) {
        printf("finger: %s: no such user.\n", username);
        return;
    }

    // Struttura contenenti dati utente
    struct User user = creaUser(pw);
    struct UtmpInfo user_utmp = creaUtmpInfo(pw->pw_name); 

    if (l && !p) {
        l_option(user, user_utmp, 1); // Opzione di print -l
        return;
    }

    if (l && p) {
        l_option(user, user_utmp, 0);
        return;
    }

    if (s) {
        s_option(user, user_utmp); // Opzione di print -s
        return;
    }
}

void get_active_users(int s, int l, int p, int m) {
    // Funzione che ottiene la lista di tutti gli utenti attivi
    FILE *fp;
    char path[1035];

    fp = popen("who", "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        exit(1);
    }

    while (fgets(path, sizeof(path), fp) != NULL) {
        char *user = strtok(path, " ");
        if (user != NULL) {
            process_user(user,s,l,p,m);
        }
    }


    pclose(fp); // chiudo il file
}

int main(int argc, char *argv[]) {

    int opt, s = 0, m = 0, p = 0, l = 0;

    while ((opt = getopt(argc, argv, "slmp")) != -1) {
        switch (opt) {
            case 's':
                s = 1;
                break;
            case 'l':
                l = 1;
                break;
            case 'm':
                m = 1;
                break;
            case 'p':
                p = 1;
                break;
            default:
                fprintf(stderr, "finger: invalid option -- '%c'\nusage: finger [lmps] [...login]\n", opt);
                return 1;
        }
    }

    if (s == 0 && l == 0 && optind < argc) {
        l = 1;
    }

    if (s == 0 && l == 0) {
        s = 1;
    }


    if (optind >= argc) {
        get_active_users(1,l,p,m); // Funzione che mi permette di ottenere tutti gli utenti attivi
    } else {
        // Itero sopra i vari user passati tramite args
        for (int i = optind; i < argc; i++) {
            process_user(argv[i], s, l, p, m); // funzione che mi permette di processare gli utenti
            printf("\n");
        }
    }

    return 0;
}
