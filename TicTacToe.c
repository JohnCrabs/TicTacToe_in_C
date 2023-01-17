#include<stdio.h> /* include standard input/output library */
#include<stdlib.h> /* include standard library */
#include<stdbool.h> /* include the standard boolean library */
#include<string.h> /* use for string editing */

/* ***************** */
/* Define Parameters */
/* ***************** */
#define PLAYER_1_SYMBOL 'x'
#define PLAYER_2_SYMBOL 'o'

#define D_BOARD_WIDTH 10
#define D_BOARD_HEIGHT 10

/*
    if board_size = (3 x 3) the showed board must be 1 block higher,
    thus show_board_size = (4 x 4). This happens because we want to
    show the coordinates.
*/
#define BOARD_WIDTH ((D_BOARD_WIDTH > 2) ? ((D_BOARD_WIDTH < 11) ? D_BOARD_WIDTH : 10) : 3)
#define BOARD_HEIGHT ((D_BOARD_HEIGHT > 2) ? ((D_BOARD_HEIGHT < 11) ? D_BOARD_HEIGHT : 10) : 3)

#define GAME_BOARD_WIDTH ((BOARD_WIDTH+1)*4 + 1)
#define GAME_BOARD_HEIGHT ((BOARD_HEIGHT+1)*2)

#define NEW_LINE printf("\n");
#define INFO_MESSAGE(msg) printf("%s", msg);

/*
    We define some enumerated states: (a) gameState, and (b) operatingSystem
*/
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

/* *************** */
/* FUNCTION BODIES */
/* *************** */
bool my_strcmp(const char* str_1, const char* str_2){
    /* calculate the length of the strings */
    int str_1_len = sizeof(str_1)/sizeof(str_1[0]);
    int str_2_len = sizeof(str_2)/sizeof(str_2[0]);

    /* if not the same length */
    if(str_1_len != str_2_len){
        return false; /* return false */
    }
    /* for each element */
    for(int i=0; i<str_1_len; ++i){
        /* if not the same symbol */
        if(str_1[i] != str_2[i])
            return false; /* return false */
    }
    return true; /* return true */
}

void runApp(){
    /* Dynamically allocate memory for the base arrays */
    /* Game Board */
    char* game_board=(char*)malloc(BOARD_WIDTH*BOARD_HEIGHT*sizeof(char));
    /* Show Board */
    char* show_board=(char*)malloc((GAME_BOARD_WIDTH*GAME_BOARD_HEIGHT+1)*sizeof(char));
    /* Options */
    char** options=(char**)malloc(BOARD_WIDTH*BOARD_HEIGHT*sizeof(char*));
    for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i){
        options[i] = (char*)malloc(3*sizeof(char));
        options[i][0] = '0'+i%BOARD_WIDTH;
        options[i][1] = 'a'+i/BOARD_HEIGHT;
        options[i][2] = '\0';

        game_board[i] = ' ';
    }
    /* Player Selection */
    char* p_selection = (char*)malloc(3*sizeof(char));

    /* ********* */
    /* Work flow */
    /* ********* */
    resetBoard(show_board); /* reset the board */
    bool loop = true; /* create a loop variable and set it to true (start game) */
    int player_turn = 0; /* create a player's counter value and it to player 0*/

    /* ********* */
    /* MAIN GAME */
    /* ********* */
    while(loop){
        cleanScreen(); /* clean the screen */
        printf("Player's %d(%c) turn!\n\n", player_turn+1, ((player_turn==0)?PLAYER_1_SYMBOL:PLAYER_2_SYMBOL)); /* print info message */
        showBoard(show_board); /* show the board */
        showOptions((const char**)options, game_board, BOARD_WIDTH*BOARD_HEIGHT); /* show available options */
        playerSelection(p_selection, (const char**)options, game_board, BOARD_WIDTH*BOARD_HEIGHT); /* get player's selection */

        /* update game board */
        game_board[(p_selection[1]-'a')*BOARD_WIDTH+(p_selection[0]-'0')] = ((player_turn==0)?PLAYER_1_SYMBOL:PLAYER_2_SYMBOL);
        show_board[(p_selection[1]+1-'a')*2*GAME_BOARD_WIDTH+(p_selection[0]+1-'0')*4+1] = game_board[(p_selection[1]-'a')*BOARD_WIDTH+(p_selection[0]-'0')];

        /* Check winning/draw conditions */
        switch(checkWinningCondintions(game_board)){
        case win: /* WIN STATE */
            cleanScreen(); /* clean the screen */
            printf("Player's %d(%c) turn!\n\n", player_turn+1, ((player_turn==0)?PLAYER_1_SYMBOL:PLAYER_2_SYMBOL)); /* print info message */
            showBoard(show_board); /* show the board */
            printf("\nPlayer %d won the game!", player_turn+1); /* print info message */
            /* if play again */
            if(checkPlayAgain()){
                resetBoard(show_board); /* reset the show_board */
                /* reset the game board */
                for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i){
                    game_board[i] = ' ';
                }
            } else {  /* else */
                loop = false;  /* set the loop to false */
            }
            break;
        case draw: /* DRAW STATE */
            cleanScreen(); /* clean the screen */
            printf("Player's %d(%c) turn!\n\n", player_turn+1, ((player_turn==0)?PLAYER_1_SYMBOL:PLAYER_2_SYMBOL)); /* print info message */
            showBoard(show_board); /* show the board */
            printf("\nThe game is a draw!"); /* print info message */
            /* if play again */
            if(checkPlayAgain()){
                resetBoard(show_board); /* reset the show_board */
                /* reset the game board */
                for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i){
                    game_board[i] = ' ';
                }
            } else { /* else */
                loop = false; /* set the loop to false */
            }
            break;
        default:
            NEW_LINE /* add a new line */
            INFO_MESSAGE("Play another round...") /* print info message */
            NEW_LINE /* add a new line */
            break;
        }

        player_turn = (player_turn+1)%2; /* change player id */
    }

    /* Deallocation Memory */
    free(game_board);
    free(show_board);
    for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i)
        free(options[i]);
    free(options);
    free(p_selection);
}

void cleanScreen(){
    /* Check the OS */
    switch(my_OS){
    case windows: /* Case of Windows */
        system("cls"); /* Use windows cmd command to clear the screen */
        break;
    case unix: /* Case of Unix */
        system("clear"); /* Use Unix cmd command to clear the screen */
        break;
    }
}

void showBoard(const char* show_board){
    printf("%s", show_board); /* print the board to screen */
}

void showOptions(const char** options, const char* game_board, const int arr_size) {
    NEW_LINE /* add new line */
    INFO_MESSAGE("Available Tiles: ") /* print info message */
    for(int i=0; i<arr_size; ++i){  /* for each option */
        if(game_board[i] == ' ')  /* check if the game board tile is available */
            printf("%s ",options[i]); /* print the available option */
    }
    NEW_LINE /* add new line */
}

void playerSelection(char* p_selection, const char** option_board, const char* game_board, const int arr_size) {
    bool loop = true;  /* create a variable loop = true */
    /* while true */
    while(loop){
        NEW_LINE /* add new line */
        INFO_MESSAGE("Type your selection (i.e., 0a): ") /* print info message */
        scanf("%s", p_selection); /* get user input */
        /* Error checking */
        for(int i=0; i<arr_size; ++i){ /* for each option */
            /* check if is available tile and if the user input exists in the option_list */
            if(game_board[i] == ' ' && my_strcmp(p_selection, option_board[i])){
                return ; /* return */
            }
        }
        INFO_MESSAGE("Wrong Selection!") /* print info message */
        NEW_LINE /* add new line */
    }
}

gameStates checkWinningCondintions(const char* game_board) {
    /* For each height element */
    for(int j=0; j<BOARD_HEIGHT; ++j){
        /* For each width element */
        for(int i=0; i<BOARD_WIDTH; ++i){
            /* if current game board tile is not <SPACE> (Empty) */
            if(game_board[j*BOARD_WIDTH+i] != ' '){
                /*
                    if negative diagonal exists
                      / (j-2, i+2)
                     /  (j-1, i+1)
                    /   (j,i)
                */
                if((j-2)>=0 && (i+2)<BOARD_WIDTH){
                    /* check if they have same symbol */
                    if(game_board[j*BOARD_WIDTH+i]==game_board[(j-1)*BOARD_WIDTH+(i+1)] && game_board[(j-1)*BOARD_WIDTH+(i+1)]==game_board[(j-2)*BOARD_WIDTH+(i+2)]){
                        return win; /* return win state */
                    }
                }
                /*
                    if (i+2) horizontal line exists
                        -----------------
                      (j,i) (j,i+1) (j,i+2)
                */
                if((i+2)<BOARD_WIDTH){
                    /* check if they have same symbol */
                    if(game_board[j*BOARD_WIDTH+i]==game_board[j*BOARD_WIDTH+(i+1)] && game_board[j*BOARD_WIDTH+(i+1)]==game_board[j*BOARD_WIDTH+(i+2)]){
                        return win; /* return win state */
                    }
                    /*
                        if positive diagonal exists
                        \   (j, i)
                         \  (j+1, i+1)
                          \ (j+2,i+2)
                    */
                    else if((j+2)<BOARD_WIDTH) {
                        /* check if they have same symbol */
                        if(game_board[j*BOARD_WIDTH+i]==game_board[(j+1)*BOARD_WIDTH+(i+1)] && game_board[(j+1)*BOARD_WIDTH+(i+1)]==game_board[(j+2)*BOARD_WIDTH+(i+2)]){
                            return win; /* return win state */
                        }
                    }
                }
                /*
                    if (j+2) vertical line exists
                    |  (j, i)
                    |  (j+1, i)
                    |  (j+2,i)
                */
                if((j+2)<BOARD_HEIGHT){
                    /* check if they have same symbol */
                    if(game_board[j*BOARD_WIDTH+i]==game_board[(j+1)*BOARD_WIDTH+i] && game_board[(j+1)*BOARD_WIDTH+i]==game_board[(j+2)*BOARD_WIDTH+i]){
                        return win; /* return win state */
                    }
                }
            }
        }
    }
    /* for all elements in board */
    for(int i=0; i<BOARD_WIDTH*BOARD_HEIGHT; ++i){
        /* check if there is at least an EMPTY tile */
        if(game_board[i] == ' '){
            return play; /* return play state */
        }
    }
    return draw; /* return draw state */
}

void resetBoard(char* show_board) {
    /* for height in show_board dim */
    for(int j=0; j<GAME_BOARD_HEIGHT; j+=2){
        /* for width in show_board dim */
        for (int i=0; i<GAME_BOARD_WIDTH-1; i+=4){
            /* create the for i-width elements of first j-height row */
            show_board[j*GAME_BOARD_WIDTH+i] = ' ';
            show_board[j*GAME_BOARD_WIDTH+i+1] = (i==0) ? ((j==0)? ' ' : ('a'+((j/2)-1))) : ((j==0)? ((i/4)-1)+'0' : ' ');
            show_board[j*GAME_BOARD_WIDTH+i+2] = ' ';
            show_board[j*GAME_BOARD_WIDTH+i+3] = '|';

            /* create the for i-width elements of second j-height row */
            show_board[(j+1)*GAME_BOARD_WIDTH+i] = '-';
            show_board[(j+1)*GAME_BOARD_WIDTH+i+1] = '-';
            show_board[(j+1)*GAME_BOARD_WIDTH+i+2] = '-';
            show_board[(j+1)*GAME_BOARD_WIDTH+i+3] = '+';
        }
        /* add the new line at the end of each row */
        show_board[j*GAME_BOARD_WIDTH+GAME_BOARD_WIDTH-1] = '\n';
        show_board[(j+1)*GAME_BOARD_WIDTH+GAME_BOARD_WIDTH-1] = '\n';
    }
    /* add the '\0' symbol at the end of the string */
    show_board[GAME_BOARD_WIDTH*GAME_BOARD_HEIGHT] = '\0';
}

bool checkPlayAgain() {
    NEW_LINE /* add new line */
    INFO_MESSAGE("Play Again? [Y/n]: ") /* print info message */
    char c[20];  /* create a variable to store the answer 20 char long */
    scanf("%s", c); /* get the answer */
    if(c[0]=='n' || c[0]=='N') /* if first char is 'n' or 'N' (No) */
        return false; /* return false */
    return true; /* return true */
}
