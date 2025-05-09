#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"

struct Question {
    char question[1000];
    char answer[100];
    char difficulty[10];
};

struct ScoreEntry {
    char name[50];
    int score;
};

struct Question questions[100];
int question_count = 0;

int isalpha_custom(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

const char* getfield(char* line, int num) {
    const char* tok;
    for (tok = strtok(line, ";"); tok && *tok; tok = strtok(NULL, ";\n")) {
        if (!--num) return tok;
    }
    return NULL;
}

void wait_for_key() {
    printf(WHT "Press any key to continue...\n");
    getch();
}

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void save_to_leaderboard(int score) {
    system("cls");
    printf(GRN "Game over! " WHT "Your score is " YEL "%d\n" WHT, score);

    char name[50];
    int valid;
    do {
        valid = 1;
        printf("Please enter your name (letters only): ");
        scanf("%s", name);
        flush_input();
        for (int i = 0; i < strlen(name); i++) {
            if (!isalpha_custom(name[i])) {
                printf(RED "Invalid name! " WHT "Only letters are allowed.\n");
                valid = 0;
                break;
            }
        }
    } while (!valid);

    FILE* in = fopen("leaderboard.csv", "r");
    FILE* out = fopen("leaderboard_temp.csv", "w");
    if (!out) {
        printf(RED "Error writing to leaderboard!\n" WHT);
        return;
    }

    int replaced = 0;
    int score_updated = 1;
    char line[1024];
    while (in && fgets(line, sizeof(line), in)) {
        char* tmp = strdup(line);
        char* uname = strdup(getfield(tmp, 1));
        char* uscore_str = strdup(getfield(tmp, 2));
        int uscore = atoi(uscore_str);

        if (strcmp(uname, name) == 0) {
            if (score > uscore) {
                fprintf(out, "%s;%d\n", name, score);
            } else {
                fprintf(out, "%s;%d\n", name, uscore);
                score_updated = 0;
            }
            replaced = 1;
        } else {
            fputs(line, out);
        }

        free(tmp);
        free(uname);
        free(uscore_str);
    }
    if (!replaced) {
        fprintf(out, "%s;%d\n", name, score);
    }

    if (in) fclose(in);
    fclose(out);
    remove("leaderboard.csv");
    rename("leaderboard_temp.csv", "leaderboard.csv");

    if (!score_updated) {
        printf(YEL "Note: " WHT "Your score was lower than a previous one and was not updated.\n");
        wait_for_key();
    }
}

void header() {
    system("cls");
    printf(WHT "--------------\n");
    printf("1 - Start new game\n");
    printf("2 - Continue last game\n");
    printf("3 - Leaderboard\n");
    printf("0 - Exit\n");
    printf("--------------\n");
}

void display_question(struct Question q) {
    if (strcmp(q.difficulty, "Easy") == 0) {
        printf(GRN "Easy: " WHT);
    } else if (strcmp(q.difficulty, "Medium") == 0) {
        printf(YEL "Medium: " WHT);
    } else {
        printf(RED "Hard: " WHT);
    }
    printf("%s\n", q.question);
}

int calculate_score(struct Question q) {
    if (strcmp(q.difficulty, "Easy") == 0) return 1;
    if (strcmp(q.difficulty, "Medium") == 0) return 2;
    return 3;
}

void save_progress(int score, int asked[]) {
    FILE* stream = fopen("savegame.txt", "w");
    if (stream == NULL) {
        printf(RED "Error opening save file!\n" WHT);
        return;
    }
    fprintf(stream, "%d\n", score);
    for (int j = 0; j < question_count; j++) {
        fprintf(stream, "%d ", asked[j]);
    }
    fclose(stream);
}

void play_game(int continue_flag) {
    int i, score = 0, asked[question_count], total_asked = 0;
    char answer[1000];

    for (i = 0; i < question_count; i++) asked[i] = 0;

    if (continue_flag) {
        FILE* stream = fopen("savegame.txt", "r");
        if (stream == NULL) {
            printf(RED "No saved game found!\n" WHT);
            wait_for_key();
            return;
        }
        fscanf(stream, "%d", &score);
        for (i = 0; i < question_count; i++) fscanf(stream, "%d", &asked[i]);
        fclose(stream);
    }

    for (i = 0; i < 11; ) {
        int idx = continue_flag ? i : rand() % question_count;
        if (asked[idx]) {
            if (continue_flag) continue;
            continue;
        }
        asked[idx] = 1;
        system("cls");
        printf("Current score: " CYN "%d\n" WHT, score);
        display_question(questions[idx]);
        printf("Answer (single word): ");
        scanf("%s", answer);
        flush_input();

        if (strcmp(answer, questions[idx].answer) == 0) {
            printf(GRN "Correct!\n" WHT);
            score += calculate_score(questions[idx]);
        } else {
            printf(RED "Wrong! " WHT "The correct answer is: " CYN "%s\n" WHT, questions[idx].answer);
        }
        wait_for_key();
        save_progress(score, asked);
        i++;
    }

    save_to_leaderboard(score);
    remove("savegame.txt");
    system("cls");
    printf(GRN "Your score has been saved to the leaderboard.\n" WHT);
    wait_for_key();
}

int compare_scores(const void* a, const void* b) {
    struct ScoreEntry* sa = (struct ScoreEntry*)a;
    struct ScoreEntry* sb = (struct ScoreEntry*)b;
    return sb->score - sa->score;
}

void leaderboard() {
    system("cls");
    FILE* stream = fopen("leaderboard.csv", "r");
    struct ScoreEntry entries[100];
    int count = 0;
    char line[1024];

    while (fgets(line, 1024, stream) && count < 100) {
        char* tmp = strdup(line);
        char* token = strtok(tmp, ";\n");
        if (token) {
            strcpy(entries[count].name, token);
            token = strtok(NULL, ";\n");
            if (token) {
                entries[count].score = atoi(token);
                count++;
            }
        }
        free(tmp);
    }
    fclose(stream);

    qsort(entries, count, sizeof(struct ScoreEntry), compare_scores);

    printf("--------------\nLeaderboard:\n");
    for (int i = 0; i < count; i++) {
        printf(YEL "%s: " CYN "%d" WHT "\n", entries[i].name, entries[i].score);
    }
    printf("--------------\n");
    wait_for_key();
}

void menu(int op) {
    switch(op) {
        case 1: play_game(0); break;
        case 2: play_game(1); break;
        case 3: leaderboard(); break;
        default: printf(RED "EXIT!\n" WHT); break;
    }
}

void load_questions() {
    FILE* stream = fopen("questions.csv", "r");
    if (stream == NULL) {
        printf(RED "Error opening questions file!\n" WHT);
        exit(1);
    }
    char line[1024];
    int i = 0;
    while (fgets(line, sizeof(line), stream)) {
        char* tmp1 = strdup(line);
        char* tmp2 = strdup(line);
        char* tmp3 = strdup(line);
        strcpy(questions[i].question, getfield(tmp1, 1));
        strcpy(questions[i].answer, getfield(tmp2, 2));
        strcpy(questions[i].difficulty, getfield(tmp3, 3));
        free(tmp1); free(tmp2); free(tmp3);
        i++;
    }
    fclose(stream);
    question_count = i;
}

int main() {
    load_questions();
    int option;
    do {
        header();
        printf("Enter option (0-3): ");
        int result = scanf("%d", &option);
        flush_input();
        if (result != 1 || option < 0 || option > 3) {
            printf(RED "Invalid option! " WHT "Please enter a number between 0 and 3.\n");
            wait_for_key();
            continue;
        }
        menu(option);
    } while (option > 0 && option <= 3);
    return 0;
}
