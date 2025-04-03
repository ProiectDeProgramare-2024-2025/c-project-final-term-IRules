#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

struct Question
{
    char question[1000];
    char answer[10];
    int difficulty;
};

struct Question questions[100];

struct Question questions[100] = {
    {"What is the capital of France?", "Paris", 1},
    {"What is 2 + 2?", "4", 1},
    {"What is the largest planet in our solar system?", "Jupiter", 2},
    {"What is the square root of 16?", "4", 1},
    {"Who wrote 'Romeo and Juliet'?", "Shakespeare", 3},
    {"What is the capital of Germany?", "Berlin", 1},
    {"What is the chemical symbol for gold?", "Au", 2},
    {"What is the largest mammal?", "Blue Whale", 3},
    {"What is the speed of light?", "299792458", 3},
    {"What is the capital of Japan?", "Tokyo", 1},
    {"What is the smallest prime number?", "2", 1},
    {"What is the currency of the USA?", "Dollar", 1},
    {"What is the largest desert in the world?", "Sahara", 2},
    {"Who painted the Mona Lisa?", "Da Vinci", 3},
    {"What is the boiling point of water?", "100", 1},
    {"What is the capital of Italy?", "Rome", 1},
    {"What is the chemical symbol for water?", "H2O", 1},
    {"What is the largest ocean on Earth?", "Pacific", 2},
    {"Who discovered penicillin?", "Fleming", 3},
    {"What is the capital of Spain?", "Madrid", 1},
    {"What is the square root of 25?", "5", 1},
    {"What is the largest continent?", "Asia", 2},
    {"Who wrote '1984'?", "Orwell", 3},
    {"What is the capital of Canada?", "Ottawa", 1},
    {"What is the chemical symbol for silver?", "Ag", 2},
    {"What is the largest country in the world?", "Russia", 3},
    {"Who invented the telephone?", "Bell", 3},
    {"What is the capital of Australia?", "Canberra", 1},
    {"What is the chemical symbol for carbon?", "C", 1},
    {"What is the largest lake in the world?", "Caspian Sea", 2},
    {"Who wrote 'Pride and Prejudice'?", "Austen", 3},
    {"What is the capital of Brazil?", "Brasilia", 1},
    {"What is the chemical symbol for oxygen?", "O", 1},
    {"What is the largest volcano in the world?", "Mauna Loa", 2},
    {"Who discovered America?", "Columbus", 3},
    {"What is the capital of India?", "New Delhi", 1},
    {"What is the chemical symbol for iron?", "Fe", 1},
    {"What is the largest river in the world?", "Amazon", 2},
    {"Who wrote 'The Great Gatsby'?", "Fitzgerald", 3},
    {"What is the capital of Russia?", "Moscow", 1},
    {"What is the chemical symbol for sodium?", "Na", 1},
    {"What is the largest animal on land?", "Elephant", 2},
    {"Who wrote 'To Kill a Mockingbird'?", "Lee", 3},
    {"What is the capital of Egypt?", "Cairo", 1},
    {"What is the chemical symbol for potassium?", "K", 1},
    {"What is the largest city in the world?", "Tokyo", 2},
    {"Who wrote 'The Catcher in the Rye'?", "Salinger", 3},
    {"What is the capital of Mexico?", "Mexico City", 1},
    {"What is the largest organ in the human body?", "Skin", 2},
    {"Who wrote 'The Odyssey'?", "Homer", 3}
};

int question_count = 50;


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
    remove("savegame.txt");
    int i, j;
    char answer[10];
    int score = 0;
    int asked[question_count];
    for (i = 0; i < question_count; i++) {
        asked[i] = 0;
    }
    for (i = 0; i < 11; i++) {
        int random_index;
        do {
            random_index = rand() % question_count;
        } while (asked[random_index] == 1);
        asked[random_index] = 1;
        system("cls");
        printf("Current score: %d\n", score);

        if (questions[random_index].difficulty == 1) {
            printf("Easy: ");
        } else if (questions[random_index].difficulty == 2) {
            printf("Medium: ");
        } else {
            printf("Hard: ");
        }

        printf("%s\n", questions[random_index].question);
        printf("Answer: ");
        scanf("%s", answer);

        if (strcmp(answer, questions[random_index].answer) == 0) {
            printf("Correct!\n");
            score += questions[random_index].difficulty;
        } else {
            printf("Wrong! The correct answer is %s\n", questions[random_index].answer);
        }
        FILE* stream = fopen("savegame.txt", "w");
        if (stream == NULL) {
            printf("Error opening file!\n");
            return;
        }
        fprintf(stream, "%d\n", score);
        for (j = 0; j < question_count; j++) {
            fprintf(stream, "%d ", asked[j]);
        }
        fclose(stream);
    }

    system("cls");
    printf("Game over! Your score is %d\n", score);
    printf("Please enter your name: ");
    char name[50];
    scanf("%s", name);
    FILE* stream = fopen("leaderboard.csv", "a");
    if (stream == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(stream, "%s;%d\n", name, score);
    fclose(stream);

    remove("savegame.txt");

    system("cls");
    printf("Your score has been saved to the leaderboard.\n");
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


void continue_game() {
    int i, j;
    char answer[10];
    int score = 0;
    int asked[question_count];
    FILE* stream = fopen("savegame.txt", "r");
    if (stream == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fscanf(stream, "%d", &score);
    for (i = 0; i < question_count; i++) {
        fscanf(stream, "%d", &asked[i]);
    }
    fclose(stream);
    int total_asked = 0;
    for (i = 0; i < question_count; i++) {
        if (asked[i] == 1) {
            total_asked++;
        }
    }

    for (i = 0; i < 11 - total_asked ; i++) {
        if (asked[i] == 1) {
            continue;
        }
        system("cls");
        printf("Current score: %d\n", score);

        if (questions[i].difficulty == 1) {
            printf("Easy: ");
        } else if (questions[i].difficulty == 2) {
            printf("Medium: ");
        } else {
            printf("Hard: ");
        }

        printf("%s\n", questions[i].question);
        printf("Answer: ");
        scanf("%s", answer);

        if (strcmp(answer, questions[i].answer) == 0) {
            printf("Correct!\n");
            score += questions[i].difficulty;
            asked[i] = 1;
        } else {
            printf("Wrong! The correct answer is %s\n", questions[i].answer);
            asked[i] = 1;
        }
        FILE* stream = fopen("savegame.txt", "w");
        if (stream == NULL) {
            printf("Error opening file!\n");
            return;
        }
        fprintf(stream, "%d\n", score);
        for (j = 0; j < question_count; j++) {
            fprintf(stream, "%d ", asked[j]);
        }
        fclose(stream);
    }

    system("cls");
    printf("Game over! Your score is %d\n", score);
    printf("Please enter your name: ");
    char name[50];
    scanf("%s", name);
    FILE* stream2 = fopen("leaderboard.csv", "a");
    if (stream2 == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(stream2, "%s;%d\n", name, score);
    fclose(stream2);

    remove("savegame.txt");

    system("cls");
    printf("Your score has been saved to the leaderboard.\n");
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

void leaderboard() {
    system("cls");
    FILE* stream = fopen("leaderboard.csv", "r");
    char line[1024];
    printf("--------------\n");
    printf("Leaderboard\n");
    while (fgets(line, 1024, stream))
    {
        char* tmp = strdup(line);
        printf("%s: %s\n", getfield(tmp, 1), getfield(tmp, 2));

    }
    fclose(stream);
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
