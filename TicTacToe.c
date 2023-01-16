#include<stdio.h> /* include standard input/output library */
#include<stdlib.h> /* include standard library */
#include<stdbool.h> /* include the standard boolean library */
#include<conio.h> /* use this library for clearing the screen */
#include<string.h> /* use for string editing */

/* ***************** */
/* Define Parameters */
/* ***************** */
#define PLAYER_1_SYMBOL 'x'
#define PLAYER_2_SYMBOL 'o'

#define BOARD_WIDTH 3
#define BOARD_HEIGHT 3

/* **************** */
/* Create Constants */
/* **************** */
const short GAME_BOARD_WIDTH = (BOARD_WIDTH < 11) ? ((BOARD_WIDTH+1)*4 + 1) : ((9+1)*4 + 1);
const short GAME_BOARD_HEIGHT = (BOARD_HEIGHT < 11) ? ((BOARD_HEIGHT+1)*2) : ((9+1)*2);

typedef enum gameStates {play, win, draw} gameStates;
typedef enum operatingSystem {windows, unix} operatingSystem;

/* ******** */
/* Check OS */
/* ******** */
#ifdef _WIN32
    const operatingSystem my_OS = windows;
#else
    const operatingSystem my_OS = unix;
#endif

/* ***************** */
/* Declare Functions */
/* ***************** */
bool my_strcmp(const char* str_1, const char* str_2);

void runApp();
void cleanScreen();
void showBoard(const char* show_board);
void showOptions(const char** options, const char* game_board, const int arr_size);
void playerSelection(char* p_selection, const char** option_board, const char* game_board, const int arr_size);
gameStates checkWinningCondintions(const char* game_board);
void resetBoard(char* show_board);
bool checkPlayAgain();

/* **** */
/* MAIN */
/* **** */
int main(){
    runApp();
    return 0;
}

bool my_strcmp(const char* str_1, const char* str_2){
    int str_1_len = sizeof(str_1)/sizeof(str_1[0]);
    int str_2_len = sizeof(str_2)/sizeof(str_2[0]);

    if(str_1_len != str_2_len){
        return false;
    }
    for(int i=0; i<str_1_len; ++i){
        if(str_1[i] != str_2[i])
            return false;
    }
    return true;
}

void runApp(){
    char* game_board=(char*)malloc(BOARD_WIDTH*BOARD_HEIGHT*sizeof(char));
    char* show_board=(char*)malloc((GAME_BOARD_WIDTH*GAME_BOARD_HEIGHT+1)*sizeof(char));
    char** options=(char**)malloc(BOARD_WIDTH*BOARD_HEIGHT*sizeof(char*));
    for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i){
        options[i] = (char*)malloc(3*sizeof(char));
        options[i][0] = '0'+i%3;
        options[i][1] = 'a'+i/3;
        options[i][2] = '\0';

        game_board[i] = ' ';
    }
    char* p_selection = (char*)malloc(3*sizeof(char));

    resetBoard(show_board);
    bool loop = true;
    int player_turn = 0;
    while(loop){
        cleanScreen(); /* clean the screen */
        printf("Player's %d(%c) turn!\n\n", player_turn+1, ((player_turn==0)?PLAYER_1_SYMBOL:PLAYER_2_SYMBOL)); /* print info message */
        showBoard(show_board); /* clean the screen */
        showOptions((const char**)options, game_board, BOARD_WIDTH*BOARD_HEIGHT); /* show available options */
        playerSelection(p_selection, (const char**)options, game_board, BOARD_WIDTH*BOARD_HEIGHT); /* get player's selection */

        /* update game board */
        game_board[(p_selection[1]-'a')*BOARD_WIDTH+(p_selection[0]-'0')] = ((player_turn==0)?PLAYER_1_SYMBOL:PLAYER_2_SYMBOL);
        show_board[(p_selection[1]+1-'a')*2*GAME_BOARD_WIDTH+(p_selection[0]+1-'0')*4+1] = game_board[(p_selection[1]-'a')*BOARD_WIDTH+(p_selection[0]-'0')];

        switch(checkWinningCondintions(game_board)){
        case win:
            cleanScreen(); /* clean the screen */
            printf("Player's %d(%c) turn!\n\n", player_turn+1, ((player_turn==0)?PLAYER_1_SYMBOL:PLAYER_2_SYMBOL)); /* print info message */
            showBoard(show_board); /* clean the screen */
            printf("\nPlayer %d won the game!", player_turn+1);
            //exit(100);
            if(checkPlayAgain()){
                resetBoard(show_board);
                for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i){
                    game_board[i] = ' ';
                }
            } else {
                loop = false;
            }
            break;
        case draw:
            cleanScreen(); /* clean the screen */
            printf("Player's %d(%c) turn!\n\n", player_turn+1, ((player_turn==0)?PLAYER_1_SYMBOL:PLAYER_2_SYMBOL)); /* print info message */
            showBoard(show_board); /* clean the screen */
            printf("\nThe game is a draw!");
            //exit(100);
            if(checkPlayAgain()){
                resetBoard(show_board);
                for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i){
                    game_board[i] = ' ';
                }
            } else {
                loop = false;
            }
            break;
        default:
            printf("\nPlay another round...\n");
            break;
        }

        player_turn = (player_turn+1)%2;
    }

    free(game_board);
    free(show_board);
    for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i)
        free(options[i]);
    free(options);
    free(p_selection);
}

void cleanScreen(){
    switch(my_OS){
    case windows:
        system("cls");
        break;
    case unix:
        system("clear");
        break;
    }
}

void showBoard(const char* show_board){
    printf("%s", show_board);
}

void showOptions(const char** options, const char* game_board, const int arr_size) {
    printf("\nAvailable Tiles: ");
    for(int i=0; i<arr_size; ++i){
        if(game_board[i] == ' ')
            printf("%s ",options[i]);
    }
    printf("\n");
}

void playerSelection(char* p_selection, const char** option_board, const char* game_board, const int arr_size) {
    bool loop = true;
    while(loop){
        printf("\nType your selection (i.e., 0a): ");
        scanf("%s", p_selection);
        for(int i=0; i<arr_size; ++i){
            if(game_board[i] == ' ' && my_strcmp(p_selection, option_board[i])){
                return ;
                break;
            }
        }
        if(loop){
            printf("Wrong Selection!\n");
        }
    }
}

gameStates checkWinningCondintions(const char* game_board) {
    for(int j=0; j<BOARD_HEIGHT; ++j){
        for(int i=0; i<BOARD_WIDTH; ++i){
            if(game_board[j*BOARD_WIDTH+i] != ' '){
                if((j-2)>=0 && (i+2)<BOARD_WIDTH){
                    if(game_board[j*BOARD_WIDTH+i]==game_board[(j-1)*BOARD_WIDTH+(i+1)] && game_board[(j-1)*BOARD_WIDTH+(i+1)]==game_board[(j-2)*BOARD_WIDTH+(i+2)]){
                        return win;
                    }
                }
                if((i+2)<BOARD_WIDTH){
                    if(game_board[j*BOARD_WIDTH+i]==game_board[j*BOARD_WIDTH+(i+1)] && game_board[j*BOARD_WIDTH+(i+1)]==game_board[j*BOARD_WIDTH+(i+2)]){
                        return win;
                    } else if((j+2)<BOARD_WIDTH) {
                        if(game_board[j*BOARD_WIDTH+i]==game_board[(j+1)*BOARD_WIDTH+(i+1)] && game_board[(j+1)*BOARD_WIDTH+(i+1)]==game_board[(j+2)*BOARD_WIDTH+(i+2)]){
                            return win;
                        }
                    }
                }
                if((j+2)<BOARD_HEIGHT){
                    if(game_board[j*BOARD_WIDTH+i]==game_board[(j+1)*BOARD_WIDTH+i] && game_board[(j+1)*BOARD_WIDTH+i]==game_board[(j+2)*BOARD_WIDTH+i]){
                        return win;
                    }
                }
            }
        }
    }

    for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i){
                if(game_board[i] == ' '){
                    return play;
                }
            }
    return draw;
}

void resetBoard(char* show_board) {
    for(int j=0; j<GAME_BOARD_HEIGHT-1; j+=2){
        for (int i=0; i<GAME_BOARD_WIDTH-1; i+=4){
            show_board[j*GAME_BOARD_WIDTH+i] = ' ';
            show_board[j*GAME_BOARD_WIDTH+i+1] = (i==0) ? ((j==0)? ' ' : ('a'+((j/2)-1))) : ((j==0)? ((i/4)-1)+'0' : ' ');
            show_board[j*GAME_BOARD_WIDTH+i+2] = ' ';
            show_board[j*GAME_BOARD_WIDTH+i+3] = '|';

            show_board[(j+1)*GAME_BOARD_WIDTH+i] = '-';
            show_board[(j+1)*GAME_BOARD_WIDTH+i+1] = '-';
            show_board[(j+1)*GAME_BOARD_WIDTH+i+2] = '-';
            show_board[(j+1)*GAME_BOARD_WIDTH+i+3] = '+';
        }
        show_board[j*GAME_BOARD_WIDTH+GAME_BOARD_WIDTH-1] = '\n';
        show_board[(j+1)*GAME_BOARD_WIDTH+GAME_BOARD_WIDTH-1] = '\n';
    }

    show_board[GAME_BOARD_WIDTH*GAME_BOARD_HEIGHT] = '\0';
}

bool checkPlayAgain() {
    printf("\nPlay Again? [Y/n]: ");
    char c[20];
    scanf("%s", c);
    if(c[0]=='n' || c[0]=='N')
        return false;
    return true;
}
