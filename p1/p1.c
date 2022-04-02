/*Nombres miembros del grupo:
 * Brais González Piñeiro
 * Raul Fernandez del Blanco
 * Armando Martínez Noya
 *
 * Grupo de prácticas 2.4
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <math.h>
#include "hist_list.h"
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#define MAX_LETTERRS 4096

int TrocearCadena(char  *cadena, char * trozos[]){
    int i=1;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}

void imprimirPrompt(){
    printf("->");
}

void leerEntrada(char *cadena){
    fgets(cadena,MAX_LETTERRS,stdin);
}

void borrarLista(tList *L){
    tPosL p,q;

    p = last(*L);

    while (p != LNULL){
        q = p;
        p = previous(p, *L);
        deleteAtPosition(q, L);
    }
}

void autores(int palabras, char *trozos[MAX_LETTERRS]){

    if(palabras==1){
        printf("Brais Gonzalez Piñeiro: brais.gonzalezp\nRaul Fernandez del Blanco: r.delblanco\nArmando Martínez Noya: a.mnoya\n");
    }else{
        if(strcmp(trozos[1], "-l") == 0){
            printf("brais.gonzalezp\nr.delblanco\na.mnoya\n");

        }else {
            if(strcmp(trozos[1], "-n") == 0){
                printf("Brais Gonzalez Piñeiro\nRaúl Fernández del Blanco\nArmando Martínez Noya\n");
            }
        }
    }

}

void pid(int palabras, char *trozos[MAX_LETTERRS]){
    if(palabras==1){
        printf("PID de shell: %d\n", getpid());
    }else{
        if(strcmp(trozos[1], "-p") == 0){
            printf("Pid del padre shell: %d\n", getppid());
        }
    }

}

void carpeta(int palabras, char *trozos[MAX_LETTERRS]){

    if (palabras == 1) {
        char *dir = getcwd(NULL, 0);
        if(dir != NULL){
            printf("Current working directory: %s\n", dir);
        } else {
            printf("Error al leer el directorio actual: %s\n" , strerror(errno));
        }
        free(dir);
    } else {
        if (chdir(trozos[1]) != 0) {
            printf("Error al moverse de directorio: %s\n" ,strerror(errno));
        }
    }

}

void fecha(int palabras, char *trozos[MAX_LETTERRS]){


    time_t tiempo = time(0);
    if(localtime(&tiempo)!=NULL) {
        struct tm *tlocal = localtime(&tiempo);
        char output[128];

        if (palabras == 1) {
            strftime(output, 128, "%H:%M:%S\n%d/%m/%y", tlocal);
            printf("%s\n", output);
        } else {
            if ((strcmp(trozos[1], "-d")) == 0) {
                strftime(output, 128, "%d/%m/%y", tlocal);
                printf("%s\n", output);
            } else {
                if ((strcmp(trozos[1], "-h")) == 0) {
                    strftime(output, 128, "%H:%M:%S", tlocal);
                    printf("%s\n", output);
                }
            }
        }
    }
    else{
        printf("No se ha podido obtener el tiempo: %s\n" , strerror(errno));
    }

}

void hist(int palabras, char *trozos[MAX_LETTERRS], tList *L){

    tPosL p,q;
    tItemL com;

    if(palabras == 1){

        if(isEmptyList(*L)){
            puts("Error: Hist not possible");
        } else {
            p = first(*L);
            while (p != LNULL){
                com = getItem(p, *L);
                printf("%d -> %s", com.orden, com.cmd);
                p = next(p, *L);
            }
        }

    } else {
        if(strcmp(trozos[1], "-c") == 0){

            if(isEmptyList(*L)){
                puts("Error: Hist -c not possible");
            } else {
                borrarLista(L);
            }

        } else {
            p = last(*L);
            while(p != *L) {
                com = getItem(p, *L);
                if (fabs((double)strtol(trozos[1], NULL, 10)) == com.orden) {
                    q = first(*L);
                    while (q != p) {
                        com = getItem(q, *L);
                        printf("%d -> %s", com.orden, com.cmd);
                        q = next(q, *L);
                    }
                }
                p = previous(p, *L);
            }
        }
    }

}

void infosis(int palabras){

    if(palabras==1) {
        struct utsname uts;
        if (uname(&uts) == 0) {
            printf("%s (%s) OS:%s %s %s\n", uts.nodename, uts.machine, uts.sysname, uts.release, uts.version);
        }
        else{
            printf("Información del sistema no disponible: %s\n" , strerror(errno));
        }
    }

    else{
        printf("Opción no disponible\n");
    }

}

void ayuda(int palabras, char *trozos[MAX_LETTERRS]){

    if(palabras == 1){
        puts("'ayuda cmd' donde cmd es uno de los siguientes comandos:");
        puts("fin salir bye fecha pid autores hist comando carpeta infosis ayuda");
    } else {
        if(strcmp(trozos[1], "autores") == 0){
            puts("autores [-l|-n]\tMuestra los nombres y logins de los autores");
        } else {
            if(strcmp(trozos[1], "pid") == 0){
                puts("pid [-p]\t\tMuestra el pid del shell o de su proceso padre");
            } else{
                if(strcmp(trozos[1], "carpeta") == 0){
                    puts("carpeta [dir]\tCambia (o muestra) el directorio actual del shell");
                }else {
                    if(strcmp(trozos[1], "fecha") == 0){
                        puts("fecha [-d|-h]\tMuestra la fecha y o la hora actual");
                    } else {
                        if(strcmp(trozos[1], "hist") == 0){
                            puts("hist [-c|-N]\tMuestra el historico de comandos, con -c lo borra");
                        }else {
                            if(strcmp(trozos[1], "comando") == 0){
                                puts("comando [-N]\tRepite el comando N (del historico)");
                            } else {
                                if(strcmp(trozos[1], "ayuda") == 0){
                                    puts("ayuda [cmd]\t\tMuestra ayuda sobre los comandos");
                                } else {
                                    if(strcmp(trozos[1], "fin") == 0){
                                        puts("fin \t\t\tTermina la ejecucion del shell");
                                    } else {
                                        if(strcmp(trozos[1], "salir") == 0){
                                            puts("salir \t\t\tTermina la ejecucion del shell");
                                        } else {
                                            if(strcmp(trozos[1], "bye") == 0){
                                                puts("bye \t\t\tTermina la ejecucion del shell");
                                            } else {
                                                if(strcmp(trozos[1], "crear") == 0){
                                                    puts("crear [-f] [name]\t Crea un fichero o directorio");
                                                } else {
                                                    if(strcmp(trozos[1], "borrar") == 0){
                                                        puts("borrar [name1 name2 ..]\t Borra ficheros o directorios vacios");
                                                    } else {
                                                        if(strcmp(trozos[1], "borrarrec") == 0){
                                                            puts("borrarrec [name1 name2 ..]\t Borra ficheros o directorios no vacios");
                                                        } else {
                                                            if(strcmp(trozos[1], "listfich") == 0){
                                                                puts("listfich [-long][-link][-acc] n1 n2 ..\tlista ficheros");
                                                            } else {
                                                                if(strcmp(trozos[1], "listdir") == 0){
                                                                    puts("listdir [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..\tlista ficheros contenidos de directorios");
                                                                } else {
                                                                    puts("Opción incorrecta");
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void crear(int palabras, char *trozos[MAX_LETTERRS]){

    if(palabras == 1){
        carpeta(palabras, trozos);
    } else {
        if(strcmp(trozos[1], "-f") == 0){
            FILE *fichero;
            fichero = fopen(trozos[2], "a+");
            fclose(fichero);
        } else {
            if (mkdir(trozos[1], 0755) != 0) {
                printf("No se ha podido crear carpeta: %s\n" , strerror(errno));
            }
        }
    }

}

void borrar(int palabras, char *trozos[MAX_LETTERRS]){

    if(palabras == 1){
        carpeta(palabras, trozos);
    } else {
        for (int i = 1; i < palabras; i++) {
            if (rmdir(trozos[i]) != 0 && remove(trozos[i])!=0) {
                printf("No se ha podido eliminar %s: %s\n",trozos[i], strerror(errno));
            }
        }
    }
}

int remove_directory(const char *path) {

    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d) {
        struct dirent *p;

        r = 0;

        while (!r && (p=readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if (buf) {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        r2 = remove_directory(buf);
                    } else {
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }
    if (!r) {
        r = rmdir(path);
    }

    return r;
}


void borrarrec(int palabras, char *trozos[MAX_LETTERRS]){
    if (palabras == 1) {
        carpeta(palabras,trozos);
    }
    else {
        for (int i = 1; i < palabras; ++i) {
            if (remove_directory(trozos[i]) != 0) {
                printf("No se ha podido eliminar: %s\n", strerror(errno));
            }
        }
    }
}

void Fichnamesize(char *trozos[MAX_LETTERRS]){
    struct stat estadistica;
        if (stat(*trozos, &estadistica) != 0) {
            printf("Error al leer fichero %s: %s\n",*trozos,strerror(errno));
        } else {
            printf("%ld %s\n", estadistica.st_size,*trozos);
        }

}

char LetraTF (mode_t m)
{
    switch (m&S_IFMT) {
        case S_IFSOCK: return 's'; /*socket*/
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /*fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio*/
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no debería aparecer*/
    }
}

char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"----------");
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='x';
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    return permisos;
}

void Fich(char *trozos[MAX_LETTERRS],bool lg,bool link, bool acc){
    struct stat stats;
    time_t tiempo;
    char permisos[10];
    char name [10000];
    char output[128];

    if(lstat(*trozos,&stats)!=0){
        printf("Error al leer fichero %s: %s\n",*trozos,strerror(errno));
    }
    else {

        if(acc){
            tiempo = stats.st_atime;
        } else {
            tiempo = stats.st_mtime;
        }

        strftime(output, 128, "%y/%m/%d-%H:%M", localtime(&tiempo));
        struct passwd *userf = getpwuid(stats.st_uid);
        struct group *groupf = getgrgid(stats.st_gid);
        char tipo;
        tipo = LetraTF(stats.st_mode);

        if(lg){
            if(link && tipo == 'l'){
                char buffer[4096]="";
                if (readlink(*trozos, buffer, sizeof(buffer)) <= 0) {
                    sprintf(name, "%s -> No encontrado", *trozos);
                    printf("%s \n", buffer);
                } else {
                    sprintf(name, "%s -> %s", *trozos, buffer);
                }
            } else {
                strcpy(name, *trozos);
            }

            printf("%16s%4ld (%8ld)%10s%10s%11s%8ld %s\n", output, stats.st_nlink, stats.st_ino, userf->pw_name, groupf->gr_name, ConvierteModo(stats.st_mode, permisos), stats.st_size, name);
        } else {
            Fichnamesize(trozos);
        }

    }
}

void listfich(int palabras, char *trozos[MAX_LETTERRS]){

    int cont = 1;
    bool link = false, lg = false, acc = false;

    if(palabras == 1){
        carpeta(palabras, trozos);
    } else {

        for (int i = 1; i < palabras && (strcmp(trozos[i], "-long") == 0 || strcmp(trozos[i], "-link") == 0 || strcmp(trozos[i], "-acc") == 0); ++i) {
            if(strcmp(trozos[i], "-link") == 0){
                link = true;
            }
            if(strcmp(trozos[i], "-acc") == 0){
                acc = true;
            }
            if(strcmp(trozos[i], "-long") == 0){
                lg = true;
            }
            cont++;
        }

        if (cont == palabras) {
            carpeta(1, trozos);
        } else {
            for (int i = cont; i < palabras; ++i) {
                Fich(&trozos[i], lg, link, acc);
            }
        }

    }
}

void Dir(char *trozos[MAX_LETTERRS],bool lg, bool link, bool acc, bool hid){
    DIR *folder;
    struct dirent *fich;
    struct stat stats;
    time_t tiempo;
    char permisos[12];
    char name [10000];
    char path[1000];
    char buffer[4096] = "";
    char output[128];
    folder=opendir(*trozos);


    if(folder==NULL){
        Fich(trozos,lg, link, acc);
    } else {
        while ((fich = readdir(folder)) != NULL) {

            strcpy(path, *trozos);
            strcat(path, "/");
            strcat(path, fich->d_name);

            if(lstat(path, &stats) != 0){
                printf("Error al leer fichero %s: %s\n", fich->d_name, strerror(errno));
            } else {

                if(acc){
                    tiempo = stats.st_atime;
                } else {
                    tiempo = stats.st_mtime;
                }
                strftime(output, 128, "%y/%m/%d-%H:%M", localtime(&tiempo));
                struct passwd *userf = getpwuid(stats.st_uid);
                struct group *groupf = getgrgid(stats.st_gid);
                char tipo;
                tipo = LetraTF(stats.st_mode);

                if(lg){
                    if(link && tipo == 'l'){

                        if (readlink(fich->d_name, buffer, sizeof(buffer)) <= 0) {
                            sprintf(name, "%s -> No encontrado", fich->d_name);
                            printf("%s", buffer);
                        } else {
                            sprintf(name, "%s -> %s", fich->d_name, buffer);
                        }
                    } else {
                        strcpy(name, fich->d_name);
                    }

                    if(hid){
                        printf("%16s%4ld (%8ld)%10s%10s%11s%8ld %s\n", output, stats.st_nlink, stats.st_ino, userf->pw_name, groupf->gr_name, ConvierteModo(stats.st_mode, permisos), stats.st_size, name);
                    } else {
                        if(fich->d_name[0] != '.'){
                            printf("%16s%4ld (%8ld)%10s%10s%11s%8ld %s\n", output, stats.st_nlink, stats.st_ino, userf->pw_name, groupf->gr_name, ConvierteModo(stats.st_mode, permisos), stats.st_size, name);
                        }
                    }

                } else {

                    if(hid){
                        printf("%8ld %s\n", stats.st_size, fich->d_name);
                    } else {
                        if(fich->d_name[0] != '.'){
                            printf("%8ld %s\n", stats.st_size, fich->d_name);
                        }
                    }
                }
            }
        }
    }
    closedir(folder);
}

void DirA(char *path[MAX_LETTERRS],bool lg, bool link, bool acc, bool hid){

    DIR *d = opendir(*path);
    size_t path_len = strlen(*path);

    if (d) {
        struct dirent *p;

        printf("************%s\n", *path);
        Dir(path, lg, link, acc, hid);

        while ((p=readdir(d))) {

            char *buf;

            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }
            len = path_len + strlen(p->d_name) + 2;
            buf= malloc(len);
            struct stat statbuf;
            snprintf(buf, len, "%s/%s", *path, p->d_name);

            if (!stat(buf, &statbuf)) {
                if (LetraTF(statbuf.st_mode) == 'd') {
                    DirA(&buf, lg, link, acc, hid);
                }
            }
            free(buf);
        }

        closedir(d);
    } else {
        Fich(path, lg, link, acc);
    }
}

void DirB(char *path[MAX_LETTERRS],bool lg, bool link, bool acc, bool hid) {

    DIR *d = opendir(*path);
    size_t path_len = strlen(*path);

    if (d) {
        struct dirent *p;

        while ((p=readdir(d))) {

            char *buf;

            size_t len;

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }
            len = path_len + strlen(p->d_name) + 2;
            buf= malloc(len);
            struct stat statbuf;
            snprintf(buf, len, "%s/%s", *path, p->d_name);

            if (!stat(buf, &statbuf)) {
                if (LetraTF(statbuf.st_mode) == 'd') {
                    DirB(&buf, lg, link, acc, hid);
                }
            }
            free(buf);
        }
        printf("************%s\n", *path);
        Dir(path, lg, link, acc, hid);
        closedir(d);
    } else {
        Fich(path, lg, link, acc);
    }
}

void listdir(int palabras, char *trozos[MAX_LETTERRS]){

    int cont = 1;
    bool link = false,hid = false,reca=false,recb=false, acc = false, lg = false;

    if(palabras == 1){
        carpeta(palabras, trozos);
    } else {

        for (int i = 1; i < palabras && (strcmp(trozos[i], "-long") == 0 || strcmp(trozos[i], "-link") == 0 || strcmp(trozos[i], "-acc") == 0 || strcmp(trozos[i], "-hid") == 0 || strcmp(trozos[i], "-reca") == 0 || strcmp(trozos[i], "-recb") == 0); ++i) {

            if(strcmp(trozos[i], "-link") == 0){
                link = true;
            }
            if(strcmp(trozos[i], "-acc") == 0){
                acc = true;
            }
            if(strcmp(trozos[i], "-long") == 0){
                lg = true;
            }
            if(strcmp(trozos[i], "-hid") == 0){
                hid = true;
            }
            if(strcmp(trozos[i], "-reca") == 0){
                reca = true;
            }
            if(strcmp(trozos[i], "-recb") == 0){
                recb = true;
            }
            cont++;
        }

        if (cont == palabras) {
            carpeta(1, trozos);
        } else {

            for (int i = cont; i < palabras; ++i) {

                if(reca){
                    DirA(&trozos[i], lg, link, acc, hid);
                } else {
                    if(recb){
                        DirB(&trozos[i], lg, link, acc, hid);
                    } else {
                        printf("************%s\n", trozos[i]);
                        Dir(&trozos[i], lg, link, acc, hid);
                    }
                }
            }
        }
    }
}

bool procesarEntrada(char cadena[MAX_LETTERRS], tList *list, char *trozos[MAX_LETTERRS]){

    int palabras;
    tItemL comand;

    stpcpy(comand.cmd, cadena);
    palabras = TrocearCadena(cadena, trozos);

    if (strcmp(cadena, "\n") != 0 && strcmp(trozos[0], "comando") != 0 && !insertItem(comand, list)) {
        puts("Error en la inserción\n");
    }

    if(trozos[0] == NULL){

        return false;
    } else {
        if ((strcmp(trozos[0], "fin") == 0) || (strcmp(trozos[0], "salir") == 0) || (strcmp(trozos[0], "bye") == 0)) {
            return true;
        } else {
            if (strcmp(trozos[0], "autores") == 0) {
                autores(palabras, trozos);
            } else {
                if (strcmp(trozos[0], "pid") == 0) {
                    pid(palabras, trozos);
                } else {
                    if (strcmp(trozos[0], "carpeta") == 0) {
                        carpeta(palabras, trozos);
                    } else {
                        if (strcmp(trozos[0], "fecha") == 0) {
                            fecha(palabras, trozos);
                        } else {
                            if (strcmp(trozos[0], "hist") == 0) {
                                hist(palabras, trozos, list);
                            } else {
                                if (strcmp(trozos[0], "comando") == 0) {
                                    tPosL p = *list;
                                    tPosL q;
                                    int a = 0;
                                    int b=0;
                                    tItemL j;

                                    if(isdigit(*trozos[1])) {
                                        long h = (strtol(trozos[1], NULL, 10));
                                        if (!isEmptyList(*list)) {
                                            if (h >= 0) {
                                                for (int i = 0; i <= h; i++) {
                                                    while (next(p, *list) != LNULL && a < i) {
                                                        a++;
                                                        p = next(p, *list);
                                                    }
                                                }

                                                for (q = *list; next(q, *list) != LNULL; q = next(q, *list)) {
                                                    b++;
                                                }

                                                if (h <= b) {
                                                    strcpy(j.cmd, getItem(p, *list).cmd);
                                                    printf("Ejecutando hist (%d): %s", a, j.cmd);
                                                    procesarEntrada(j.cmd, list, trozos);
                                                } else {
                                                    printf("No hay elemento %ld en el historico\n", h);
                                                }
                                            }
                                        } else {
                                            printf("No hay elemento %ld en el historico\n", h);
                                        }
                                    }
                                } else {
                                    if (strcmp(trozos[0], "infosis") == 0) {
                                        infosis(palabras);
                                    } else {
                                        if (strcmp(trozos[0], "ayuda") == 0) {
                                            ayuda(palabras, trozos);
                                        } else {
                                            if(strcmp(trozos[0], "crear") == 0){
                                                crear(palabras, trozos);
                                            } else{
                                                if(strcmp(trozos[0], "borrar") == 0){
                                                    borrar(palabras, trozos);
                                                } else{
                                                    if(strcmp(trozos[0], "borrarrec") == 0){
                                                        borrarrec(palabras, trozos);
                                                    } else{
                                                        if(strcmp(trozos[0], "listfich") == 0){
                                                            listfich(palabras, trozos);
                                                        } else{
                                                            if(strcmp(trozos[0], "listdir") == 0){
                                                                listdir(palabras, trozos);
                                                            } else{
                                                                printf("%s: no es un comando del shell\n", cadena);
                                                            }
                                                        }
                                                    }
                                                }
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            return false;
        }
    }

}

int main() {

    bool terminado = false;
    char cadena[MAX_LETTERRS];
    char *trozos[MAX_LETTERRS];
    tList list;

    createEmptyList(&list);

    while(!terminado){

        imprimirPrompt();
        leerEntrada((char *) &cadena);
        terminado = procesarEntrada(cadena, &list, trozos);
    }

    borrarLista(&list);

    return 0;
}
