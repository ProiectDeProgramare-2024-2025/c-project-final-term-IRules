#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


char questions[100][1000] = {
        "In what country did the first Starbucks open outside of North America?\nA. France  B. Japan  C. Germany"
};

char answers[100] = {
        "B"
};

int difficulty[100] = {1};

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
         tok && *tok;
         tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

void header()
{
    system("cls");
    printf("--------------\n");
    printf("1 - Start new game\n");
    printf("2 - Continue last game\n");
    printf("3 - Leaderboard\n");
    printf("0 - Exit\n");
    printf("--------------\n");
}


void new_game() {
    char a;
    printf("Not implemented please press enter");
    getchar();
    scanf("%c", &a );
}


void continue_game() {
    char a;
    printf("Not implemented please press enter");
    getchar();
    scanf("%c", &a );
}

void leaderboard() {
    system("cls");
    FILE* stream = fopen("leaderboard.csv", "r");
    char line[1024];
    printf("--------------\n");
    printf("Leaderboard\n");
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        printf("%s - %s points\n",getfield(tmp, 1), getfield(tmp, 2));
        free(tmp);
    }
    printf("--------------\n");
    printf("Type 0 to return to previous menu.\n");

    int option;

    do{
        printf("Enter option: ");
        scanf("%d", &option);
        if(option == 0) {
            break;
        }
    } while(option > 0 && option <= 3);
}


void menu(int op)
{
    switch(op)
    {
        case 1:
            new_game();
            break;
        case 2:
            continue_game();
            break;
        case 3:
            leaderboard();
            break;
        default:
            printf("EXIT!\n");
    }
}


int main()
{
    int option;

    do{
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        menu(option);
    } while(option > 0 && option <= 3);

    return 0;
}
