/*

	Projekt no. 1 "INI file parser" by:
	Konrad Szewczyk, 145466
	Jerzy Lukaszewicz, 145458


	How to use:
	1) parseINI path_to_file section1.key1
		- prints out the value of the "key1" key in the "section1" section
		of the file "path_to_file"
		
	2) parseINI path_to_file expression "section1.key1 + section2.key2"
		- calculated and prints out the value of a given expression
		- possible operations:
			- addition (number + number)
			- concatenation without additional whitespace character (string + string)
			- subtraction (number + number)
			- multiplication (number + number)
			- division (number + number)
		- the result of operation on two integers is an integer number
		- the result of operation on an integer and a float is a float
		- the result of operation on two floats is a float
		- !!! there must be a single space between the operator and the variables !!!


	INI file assuptions:
	1) 0 - integer (number)
	2) 0.0 - float (number)
	3) 0.00, 0.000... - string
	4) 1, 2, 523 - integer (number)
	5) 1.1, 0.01. - float (number)
	6) test, 1.2.2, 127.0.0.1 - string
	7) section and key names cannot be empty
	8) section and key names may consist only of alphanumerical characters
	9) key value can take zero length values (treated as a string)
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>


typedef struct {
    char* name;
    char type;
    char* value;
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
	for(unsigned int i = 0; i < str_size; i++){
		output[i] = '\0';
	}
	while(str[i + 1] != ']' && i < str_size) {
		if(isalnum(str[i + 1])) {
			output[i] = str[i + 1];
		} else {
			free(output);
			return NULL;
		}
		i++;
	}
	if(strlen(output) == 0) {
		free(output);
		return NULL;
	}
	output[i] = '\0';
	return output;
}

char* get_key_name (const char* str) {
	size_t i = 0;
	size_t str_size = strlen(str);
	char* output = (char*) malloc(sizeof(char) * str_size);
	for(unsigned int i = 0; i < str_size; i++){
		output[i] = '\0';
	}
	while(str[i] != ' ' && str[i] != '=' && i < str_size) {
		if(isalnum(str[i])) {
			output[i] = str[i];
		} else {
			free(output);
			return NULL;
		}
		i++;
	}
	while(str[i] == ' ' && i < str_size) {
		if(str[i + 1] != ' ' && str[i + 1] != '=') {
			free(output);
			return NULL;
		}
		i++;
	}
	if(strlen(output) == 0) {
		free(output);
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
	output[j] = '\0';
	return output;
}


int main(int argc, char* argv[])
{
	if (argc < 3) {
		printf("Error: Insufficient number of arguments.\n");
	} else {
		int action = -1;
		FILE* fp = fopen(argv[1], "r");

		if(!fp) {
			perror("File opening failed");
			return EXIT_FAILURE;
		}

		char operation = '\0';
		char* section1_name = NULL;
		char* section2_name = NULL;
		char* key1_name = NULL;
		char* key2_name = NULL;

		if(strcmp(argv[2],"expression") == 0)
		{
			char target[strlen(argv[2]+1)];
			strcpy(target, argv[3]);

			char *part1 = strtok(target, " ");
			char* oper= strtok(NULL," ");
			char *part2 = strtok(NULL, " ");

			char* s1_name = strtok(part1,".");
			char* k1_name = strtok(NULL, ".");
			char* s2_name = strtok(part2,".");
			char* k2_name = strtok(NULL, ".");

			section1_name = (char*) malloc(sizeof(char) * (strlen(s1_name) + 1));
			strcpy(section1_name, s1_name);
			section1_name[strlen(s1_name)] = '\0';

			key1_name = (char*) malloc(sizeof(char) * (strlen(k1_name) + 1));
			strcpy(key1_name, k1_name);
			key1_name[strlen(k1_name)] = '\0';

			section2_name = (char*) malloc(sizeof(char) * (strlen(s2_name) + 1));
			strcpy(section2_name, s2_name);
			section2_name[strlen(s2_name)] = '\0';

			key2_name = (char*) malloc(sizeof(char) * (strlen(k2_name) + 1));
			strcpy(key2_name, k2_name);
			key2_name[strlen(k2_name)] = '\0';

			operation = oper[0];

			action = 1;
		}
		else
		{
			char target[strlen(argv[2]+1)];
			strcpy(target, argv[2]);

			char* s1_name = strtok(target, ".");
			char* k1_name = strtok(NULL, ".");

			section1_name = (char*) malloc(sizeof(char) * (strlen(s1_name) + 1));
			strcpy(section1_name, s1_name);
			section1_name[strlen(s1_name)] = '\0';

			key1_name = (char*) malloc(sizeof(char) * (strlen(k1_name) + 1));
			strcpy(key1_name, k1_name);
			key1_name[strlen(k1_name)] = '\0';

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
					action = -1;
					printf("Error: Invalid section identifier.\n");
					break;
				}

				file->sections[i] = (section*) malloc (sizeof(section));
				curr_section = file->sections[i];
				curr_section->name = NULL;
				curr_section->name = temp_name;
				curr_section->length = 0;
				curr_section->size = 16;
				curr_section->keys = (key**) malloc (sizeof(key*) * curr_section->size);
				i = ++(file->length);
				j = 0;
			}

			if(isalnum(line[0])) {
				if(curr_section == NULL){
					action = -1;
					printf("Error: a key must belong to a section!\n");
					break;
				}

				if(curr_section->length == curr_section->size) {
					curr_section->size += 16;
					curr_section->keys = realloc(curr_section->keys, sizeof(key*) * curr_section->size);
				}

				if((temp_name = get_key_name(line)) == NULL) {
					action = -1;
					printf("Error: Invalid key identifier.\n");
					break;
				}

				if((temp_value = get_key_value(line)) == NULL) {
					action = -1;
					printf("Error: Invalid value.\n");
					break;
				}

				curr_section->keys[j] = (key*) malloc(sizeof(key));
				curr_key = curr_section->keys[j];
				curr_key->name = NULL;
				curr_key->name = temp_name;
				if(atof(temp_value) != 0 || strcmp(temp_value, "0") == 0 || strcmp(temp_value, "0.0") == 0) {
					if(atof(temp_value) == atoi(temp_value) && strcmp(temp_value, "0.0") != 0) {
						curr_key->type = 'i';
						curr_key->value = temp_value;
					} else {
						curr_key->type = 'f';
						curr_key->value = temp_value;
					}
				} else {
					curr_key->type = 's';
					curr_key->value = temp_value;
				}
				j = ++(curr_section->length);
			}
			
		}
		free(line);
		section* section1 = NULL;
		section* section2 = NULL;

		key* key1 = NULL;
		key* key2 = NULL;
			
		if(action == 1) {
			if((section1 = find_section(file, section1_name)) != NULL) {
				if((key1 = find_key(section1, key1_name)) != NULL) {
					if((section2 = find_section(file, section2_name)) != NULL) {
						if((key2 = find_key(section2, key2_name)) != NULL) {
							if(operation == '+') {
								if (key1->type == 's'|| key2->type == 's') {
									if (key1->type == 's' && key2->type =='s') {
										printf("%s%s", key1->value, key2->value);
									} else {
										printf("Error: Concatenation of number and string is invalid.\n");
									}
								} else if (key1->type == 'i' && key2->type == 'i') {
									printf("%d\n", atoi(key1->value) + atoi(key2->value));
								} else {
									printf("%f\n", atof(key1->value) + atof(key2->value));
								}
							}
							else if(operation == '-') {
								if (key1->type == 's' || key2->type == 's') {
									printf("Error: Invalid operation on a string");
								} else if (key1->type == 'i' && key2->type == 'i') {
									printf("%d\n", atoi(key1->value) - atoi(key2->value));
								} else {
									printf("%f\n", atof(key1->value) - atof(key2->value));
								}
							}
							else if(operation == '*') {
								if (key1->type == 's' || key2->type == 's') {
									printf("Error: Invalid operation on a string");
								} else if (key1->type == 'i' && key2->type == 'i') {
									printf("%d\n", atoi(key1->value) * atoi(key2->value));
								} else {
									printf("%f\n", atof(key1->value) * atof(key2->value));
								}
							}
							else if(operation == '/') {
								if (key1->type == 's' || key2->type == 's') {
									printf("Error: Invalid operation on a string");
								} else if(atof(key2->value) != 0){
									if (key1->type == 'i' && key2->type == 'i') {
										printf("%d\n", atoi(key1->value) / atoi(key2->value));
									} else {
										printf("%f\n", atof(key1->value) / atof(key2->value));
									}
								} else {
									printf("Error: Division by 0 is illegal.\n");
								}
							}
							else {
								printf("Error: %c is not a recognisable operation.\n", operation);
							}
						} else {
							printf("Error: Failed to find key %s.\n", key2_name);
						}
					} else {
						printf("Error: Failed to find section [%s]\n", section2_name);
					}
				} else {
					printf("Error: Failed to find key %s.\n", key1_name);
				}
			} else {
				printf("Error: Failed to find section [%s]\n", section1_name);
			}
			free(section1_name);
			free(key1_name);
			free(section2_name);
			free(key2_name);
		}
		else if(action == 2) {
			if((section1 = find_section(file, section1_name)) != NULL) {
				if((key1 = find_key(section1, key1_name)) != NULL) {
					printf("%s\n", key1->value);
				} else {
					printf("Error: Failed to find key %s.\n", key1_name);
				}
			} else {
				printf("Error: Failed to find section [%s]\n", section1_name);
			}
			free(section1_name);
			free(key1_name);
		} else if(action == -1) {
			// some boring error
		}
		else
		{
			printf("%s\n", "Command not recognized!");
		}

		for(unsigned int i = 0; i < file->length; i++) {
			for(unsigned int j = 0; j < file->sections[i]->length; j++) {
				free(file->sections[i]->keys[j]->value);
				free(file->sections[i]->keys[j]->name);
				free(file->sections[i]->keys[j]);
			}
			free(file->sections[i]->name);
			free(file->sections[i]->keys);
			free(file->sections[i]);
		}
		free(file->sections);
		free(file);

		if (ferror(fp))
			puts("\nI/O error when reading");
		else if (feof(fp))
			puts("\nEnd of file reached successfully");
		fclose(fp);
	}
	return 0;
}
