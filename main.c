#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 200
#define NAME_LEN 80


typedef struct
{
     char *word; //word and corresponding hint
     char *clue;
     int x; //Starting x and y positions
     int y;
     char direction; //H for horizontal, V for vertical
     int f; //solved or not
} Word_t;

Word_t* loadTextFile( FILE* myFile, int nrWords );
void displayBoard(int rows, int cols, char** myBoard);
int isBoardFilled(int rows, int cols, char** myBoard);
char** createArray(int rows, int cols);
int findIndex(int arr[], int size, int val);
void playGame(char** myBoard, int words, Word_t *wordsp, int x, int y, int countToFinish);
char** updateBoard(char** myBoard, Word_t *words, int solve);

int main()
{
    char** board;//a dynamic array of board
    int rows, columns, words;//number of rows, columns and words
    Word_t* myWords;//a dynamic array of Word_t type
    FILE* inp;//a file pointer to the file
    char line[LINE_LEN], inFile[NAME_LEN];


    printf("Enter the name of input file -> ");
    scanf("%s",inFile);

    inp = fopen(inFile,"r");

    //get the name of the file from the user and open the file
    fgets(line ,LINE_LEN, inp);

    //read number of rows, columns and words
    rows = atoi(strtok(line," "));
    columns = atoi(strtok(NULL," "));
    words = atoi(strtok(NULL," "));

   // dynamically allocate your Word_t type array in the size of words
    myWords = (Word_t*)malloc(words*sizeof(Word_t));

    myWords=loadTextFile(inp,words);
    board=createArray(rows,columns);
    //displayBoard(rows,columns,board);
    //updateBoard(board,myWords,words);
    //displayBoard(rows,columns,board);
    playGame(board,words,myWords,rows,columns,0);
    fclose(inp);
    int i;

    for(i=0; i<words; i++)
    {
        free(myWords[i].word);
        free(myWords[i].clue);
    }
    free(myWords);

    for(int k = 0; k < rows; k++)
    {
       free(board[k]);
    }
    free(board);
    return 0;
}

Word_t* loadTextFile( FILE* myFile, int nrWords ){

    char line[LINE_LEN];
    char *status;

    //Create a temporary Word_t type array and dynamically allocate it using the number of word
    Word_t * tempArr;
    tempArr = (Word_t*)malloc(nrWords*sizeof(Word_t));

    //For each element of this array, read the direction, row-column starting numbers, word to be
    //solved and its hint from the file.
    int j=0;
    for(status = fgets(line, LINE_LEN, myFile); status !=0; status = fgets(line, LINE_LEN, myFile))
    {
        int i=0;
        char *p;
        char *array[5];//no need to allocate space for array because we write the size =5

        //put lines element respectively: direction,x,y,word,clue with pointer p
        while(i<4){
            array[i] = (char*)malloc(LINE_LEN*sizeof(char)); //allocation  for array elements

            if(i==0){
                p=strtok(line," ");
            }
            else{
                p = strtok(NULL," ");
            }
            array[i++] = p;
            //printf("%s    ",array[i-1]);
        }

        p = strtok(NULL,"\n");
        array[4] =p;
        //printf("%s",array[4]);

        //array elements in the tempArr
        tempArr[j].direction=array[0][0];
        tempArr[j].x=atoi(array[1]);
        tempArr[j].y=atoi(array[2]);

        tempArr[j].word = malloc(strlen(array[3])+1);//make allocation for tempArr[j].word because it is string
        strcpy(tempArr[j].word, array[3]);

        tempArr[j].clue = malloc(strlen(array[4])+1);//make allocation for tempArr[j].clue because it is string
        strcpy(tempArr[j].clue, array[4]);

        //tempArr[j].word = strdup(array[3]);
        //tempArr[j].clue = strdup(array[4]);
        tempArr[j++].f = 0;

    }
    //int s=0;
    //tempArr[s];
   //for(int s=0; s<7; s++) {
    //    printf("%s %s %d %d %c\n", tempArr[s].word, tempArr[s].clue, tempArr[s].x, tempArr[s].y, tempArr[s].direction);
    //}
    return tempArr;
}

char** createArray(int rows, int cols){
    int i, j;

    //Dynamically allocate a temporary board array
    char **tempBoard = (char**)malloc(rows*sizeof(char*));
    for(i=0; i<rows; i++) {
        tempBoard[i] = (char*)malloc(cols*sizeof(char));
    }
    //nitialize all elements of the array to ‘#’
    for(i=0; i<rows; i++) {
        for(j=0; j<cols; j++) {
            tempBoard[i][j] = '#';
        }
    }

    return tempBoard;
}

void displayBoard(int rows, int cols, char** myBoard){
    int i,j;

    printf("\t ");
    for(i=0; i<rows; i++) {
        printf("%4d",i+1);
    }
    printf("\n");
    printf("\t ");
    for(i=0; i<cols; i++) {
        printf("%4s","--");
    }
    printf("\n");
    for(i=0; i<rows; i++) {
        printf("%d\t|",i+1);
      for(j=0;j<cols;j++) {
         printf("%4c", myBoard[i][j]);
         if(j==cols-1){
            printf("\n");
         }
      }

    }
    printf("\t ");
    for(i=0; i<cols; i++) {
        printf("%s", "****");
    }
    printf("\n");
}

int isBoardFilled(int rows, int cols, char** myBoard){

    //If there is any blank in the table (e.g. ‘_’), return 0, else return 1


    int i,j;
    for(i=0; i<rows; i++) {
      for(j=0;j<cols;j++) {
         if(myBoard[i][j]=='_'){
            return 0;
         }
      }

    }
    return 1;
}

int findIndex(int arr[], int size, int val){

    //Return 1 if the given value is found

    int i;
    for(i=0; i<size; i++){
        if(arr[i]==val){
            return 1;
        }
    }
    return 0;
}

char** updateBoard(char** myBoard, Word_t *words, int solve){

    //This function inserts the element with the index:solve into the board.
 //If the word is solved, insert the word. Else, put underscores (‘_’) in the length of this word

        char dir = words[solve].direction;
        int x = words[solve].x-1;
        int y = words[solve].y-1;
        char *myWords = words[solve].word;// get word from words[solve].word
        int f = words[solve].f;

        int j;

        if(f==1){

            if(dir == 'V'){
                for(j=0; j<strlen(myWords); j++)
                    myBoard[x+j][y]=myWords[j];
            }else if(dir == 'H'){
                for(j=0; j<strlen(myWords); j++)
                    myBoard[x][y+j]=myWords[j];
                }

        }else{

            if(dir == 'V'){
                for(j=0; j<strlen(myWords); j++)
                    myBoard[x+j][y]='_';
            }else if(dir == 'H'){
                for(j=0; j<strlen(myWords); j++)
                    myBoard[x][y+j]='_';
                }
        }

    return myBoard;

}

void playGame(char** myBoard, int words, Word_t *wordsp, int x, int y, int countToFinish){


    int arr[words];
    for(int a=1; a<=words; a++){
        arr[a-1] = a;
        myBoard=updateBoard(myBoard,wordsp,a-1);// update board to '_' for every word
    }
    //displayBoard(x,y,myBoard);
    printf("Game is %d rows x %d cols with %d words\n",x,y,words);
    printf("Words and clues are read!\n");


            int wordNum;
            //int gameOver=0;

            //if user doesn't enter -1 keep game going
            while(wordNum!=-1){
                     //print initial board and hint
            printf("\nCurrent puzzle:\n\n");
            displayBoard(x,y,myBoard);
            printf("\n\nAsk for hint:\n");
            printf("#  Direction    row  col  \n");
            printf("---------------------------\n");
            for(int i=0; i<words; i++){
                if(wordsp[i].f==0){
                    printf("%d: ",i+1);
                    if (wordsp[i].direction=='H'){
                        printf("Horizontal\t %d    %d\n",wordsp[i].x,wordsp[i].y);
                    }else if(wordsp[i].direction=='V'){
                        printf("Vertical\t %d    %d\n",wordsp[i].x,wordsp[i].y);
                    }
                }
            }printf("\n\n");
                printf("Enter -1 to exit\n");
                printf("Which word to solve next?: ");
                scanf("%d",&wordNum);

                if(wordNum==-1){
                    break;

                // checks if word has been solved
                }else if(wordsp[wordNum-1].f==1){
                    printf("\nThat word has been solved before\n");
                    continue;

                //checks number of word is valid or not
                }else if(findIndex(arr,words,wordNum)==0){
                    printf("\nThis word does not exist enter a number between 1 and %d\n",words);
                    continue;


                }else{

                    //print clue
                    printf("Current hint: ");
                    printf("%s",wordsp[wordNum-1].clue);

                    //get answer from user
                    char answer[NAME_LEN];
                    char *oanswer=wordsp[wordNum-1].word;
                    printf("\nEnter your solution: ");
                    scanf("%s",answer);
                    for(int m=0;m<strlen(answer);m++){
                        answer[m] = toupper(answer[m]);
                    }

                    //printf("%s\n",answer);
                    //printf("%s\n",oanswer);

//♣

                    //if user's answer equal original answer
                    if(strcmp(answer,oanswer)==0){
                        printf("\nCorrect!");
                        wordsp[wordNum-1].f=1;
                        countToFinish++;
                        myBoard=updateBoard(myBoard,wordsp,wordNum-1);

                        //else print hint

                    }else{
                        printf("\nWRONG ANSWER");
                        printf("\n");
                        continue;
                    }

                    //checks if countofinish equal words count if equal it means we have find all words we can finish game
                    if(isBoardFilled(x,y,myBoard)==1 || countToFinish==words){
                        printf("\n\nCurrent puzzle:\n\n");
                        displayBoard(x,y,myBoard);
                        printf("\n\nCongratulations! You beat the puzzle!\n\n");
                        break;
                    }


                   printf("\n");
                }
            }

        }


