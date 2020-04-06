#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

int main(int argc, char* argv[])
{
	int action = -1;
	FILE* fp = fopen(argv[1], "r");
	if(!fp) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }
		if(strcmp(argv[2],"expression") == 0)
		{
			char target[strlen(argv[2]+1)];
			strcpy(target, argv[3]);

			char *part1 = strtok(target, " ");
			char *znak = strtok(NULL," ");
			char *part2 = strtok(NULL, " ");
			const char *sec1 = strtok(part1,".");
			char *key1 = strtok(NULL, ".");
			const char *sec2 = strtok(part2,".");
			char *key2 = strtok(NULL, ".");
			char *beginB = "[";
			char *endB = "]";
			char par1[strlen(sec1)+1];
			char par2[strlen(sec2)+1];
			strcpy(par1,beginB);
			strcpy(par2,beginB);
			strcat(par1,sec1);
			strcat(par2,sec2);
			strcat(par1,endB);
			strcat(par2,endB);
			printf("%s %s \n", par1,par2);
			action = 1;
		}
		else
		{
			char target[strlen(argv[2]+1)];
			strcpy(target, argv[2]);

			char *section = strtok(target, ".");
			char *key = strtok(NULL, ".");
			char *beginB = "[";
			char *endB = "]";
			char par1[strlen(section)+1];
			strcpy(par1,beginB);
			strcat(par1,section);
			strcat(par1,endB);
			printf("%s\n", par1 );
			action = 2;
		}
    char c[30];
    while (fgets(c,sizeof(c),fp)!= NULL)
		{
			//tworzenie struktur
    }
		//wyświetlanie poleceń
		if(action == 1)
		{

		}
		else if(action == 2)
		{

		}
		else
		{
			printf("%s\n", "Command not recognized!");
		}
    if (ferror(fp))
        puts("I/O error when reading");
    else if (feof(fp))
        puts("End of file reached successfully");

		fclose(fp);
}
