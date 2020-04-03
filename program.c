#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
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
			char *sec1 = strtok(part1,".");
			char *key1 = strtok(NULL, ".");
			char *sec2 = strtok(part2,".");
			char *key2 = strtok(NULL, ".");

			printf("%s ", sec1);
			printf("%s ", key1);
			printf("%s ", znak);
			printf("%s ", sec2);
			printf("%s\n", key2);
		}
		else
		{
			char target[strlen(argv[2]+1)];
			strcpy(target, argv[2]);

			char *section = strtok(target, ".");
			char *key = strtok(NULL, ".");

			printf("%s\n",section);
			printf("%s\n",key);
		}

    char c[30]; // note: int, not char, required to handle EOF
    while (fgets(c,sizeof(c),fp) != NULL)
		{
    	printf("%s", c);
    }

    if (ferror(fp))
        puts("I/O error when reading");
    else if (feof(fp))
        puts("End of file reached successfully");

		fclose(fp);
}
