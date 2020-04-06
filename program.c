#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>


typedef union {
    float f;
    char* str;
} value;


typedef struct {
    char* name;
    char type;
    value val;
} key;


typedef struct {
    char* name;
    size_t length;
    size_t size;
    key** keys;
} section;


typedef struct {
    size_t length;
    size_t size;
    section** sections;
} INIfile;


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

    char* line = NULL;
	section* cur_section = NULL;
	size_t i = 0;
	size_t line_length = 0;
	ssize_t read;

	INIfile* file = (INIfile*) malloc(sizeof(INIfile));
	file->length = 0;
    file->size = 5;
	file->sections = (section**) malloc(sizeof(section*) * file->size);

    while ((read = getline(&line, &line_length, fp) != -1)) {
		
		if(line[0] == '\n' || line[0] == ';') {
			continue;
		}
		if(line[0] == '[') {
			if(file->length == file->size) {
           	 	file->size += 5;
            	file->sections = realloc(file->sections, file->size * sizeof(section*));
			}
			i = file->length;
			file->sections[i] = (section*) malloc (sizeof(section));

        }
		printf("%s", line);
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
