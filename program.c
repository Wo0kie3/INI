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

section* find_section(INIfile* file, char* sect_name) {
	for (size_t i = 0; i < file->length; i++) {
		if (strcmp(file->sections[i]->name, sect_name) == 0) {
			return file->sections[i];
		}
	}
	return NULL;
}

key* find_key(section* section, char* key_name) {
	for (size_t i = 0; i < section->length; i++) {
		if (strcmp(section->keys[i]->name, key_name) == 0) {
			return section->keys[i];
		}
	}
	return NULL;
}

char* get_section_name (const char* str) {
	size_t i = 0;
	size_t str_size = strlen(str);
	char* output = (char*) malloc(sizeof(char) * str_size);
	while(str[i + 1] != ']' && i < str_size) {
		if(isalnum(str[i + 1])) {
			output[i] = str[i + 1];
		} else {
			return NULL;
		}
		i++;
	}
	if(strlen(output) == 0) {
		return NULL;
	}
	output[i] = '\0';
	return output;
}

char* get_key_name (const char* str) {
	size_t i = 0;
	size_t str_size = strlen(str);
	char* output = (char*) malloc(sizeof(char) * str_size);
	while(str[i] != ' ' && str[i] != '=' && i < str_size) {
		if(isalnum(str[i])) {
			output[i] = str[i];
		} else {
			return NULL;
		}
		i++;
	}
	while(str[i] == ' ' && i < str_size) {
		if(str[i + 1] != ' ' && str[i + 1] != '=') {
			return NULL;
		}
		i++;
	}
	if(strlen(output) == 0) {
		return NULL;
	}
	output[i] = '\0';
	return output;
}


char* get_key_value (const char* str) {
	size_t i = 0;
	size_t j = 0;
	size_t str_size = strlen(str);
	char* output = (char*) malloc(sizeof(char) * str_size);
	while(str[i] != '=' && i < str_size) {
		i++;
	}
	i++;
	while(str[i] == ' ' && i < str_size) {
		i++;
	}
	while(str[i] != '\n' && str[i] != '\0' && i < str_size) {
		output[j++] = str[i++];
	}
	if(strlen(output) == 0) {
		return NULL;
	}
	output[j] = '\0';
	return output;
}


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
	char* temp_name = NULL;
	char* temp_value = NULL;
	section* curr_section = NULL;
	key* curr_key = NULL;
	size_t i = 0;
	size_t j = 0;
	size_t line_length = 0;
	ssize_t read;

	INIfile* file = (INIfile*) malloc(sizeof(INIfile));
	file->length = 0;
    file->size = 8;
	file->sections = (section**) malloc(sizeof(section*) * file->size);

    while ((read = getline(&line, &line_length, fp) != -1)) {
		
		if(line[0] == '\n' || line[0] == ';') {
			continue;
		}
		if(line[0] == '[') {
			if(file->length == file->size) {
           	 	file->size += 8;
            	file->sections = realloc(file->sections, file->size * sizeof(section*));
			}

			if((temp_name= get_section_name(line)) == NULL) {
				action = 0;
				printf("Error: Invalid section identifier.\n");
				break;
			}

			file->sections[i] = (section*) malloc (sizeof(section));
			curr_section = file->sections[i];
			curr_section->name = temp_name;
			curr_section->length = 0;
			curr_section->size = 16;
			curr_section->keys = (key**) malloc (sizeof(key*) * curr_section->size);
			i = ++(file->length);
			j = 0;
        }

		if(isalnum(line[0])) {
			if(curr_section == NULL){
				action = 0;
				printf("Error: a key must belong to a section!\n");
				break;
			}

			if(curr_section->length == curr_section->size) {
				curr_section->size += 16;
				curr_section->keys = realloc(curr_section->keys, sizeof(key*) * curr_section->size);
			}

			if((temp_name = get_key_name(line)) == NULL) {
				action = 0;
				printf("Error: Invalid key identifier.\n");
				break;
			}

			if((temp_value = get_key_value(line)) == NULL) {
				action = 0;
				printf("Error: Invalid value.\n");
				break;
			}

			curr_section->keys[j] = (key*) malloc(sizeof(key));
			curr_key = curr_section->keys[j];
			curr_key->name = temp_name;
			if(atof(temp_value) != 0 || strcmp(temp_value, "0") == 0) {
				curr_key->type = 'f';
				curr_key->val.f = atof(temp_value);
			} else {
				curr_key->type = 's';
				curr_key->val.str = temp_value;
			}
			j = ++(curr_section->length);
		}
    }

	for(int i = 0; i < file->length; i++) {
		printf("%s = %d\n", file->sections[i]->name, file->sections[i]->length);
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
