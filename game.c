#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>  //for stop watch
#include<windows.h>  //for failure sound


struct point{  //represents a point in the grid
    int x;
    int y;
};


char grid[100][100];  //the answer grid
int color[100][100];
int ctr=0;

int grid_size;  //size of the grid is grid_size*grid_size

char nullchar='z';   //the NULL character chosen for the grid because the grid will never have lowercase alphabets so any lowercase alphabet can be used as NULL character

int max_words;  //number of words that are inserted in the grid


enum direction{   //enumerator for direction in the grid in which the word is inserted
    UP=1,       //start the count from 1
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
};

/*data to choose the words from*/
//
char *animals[]={"deer","sheep","dog","cat","lion","tiger","elephant","wolf","hen"};
int animals_size=9;
char *fruits[]={"banana","apple","pear","grapes","guava","papaya","tomato","mango","lychee","pineapple","cherry"};
int fruits_size=11;
char *games[]={"cricket","baseball","football","tennis","rugby","boxing","wrestling","squash","bowling"};
int games_size=9;
char *starwars[]={"boba","anakin","yoda","maul","palpatine","vader","lightsaber","hansolo","millenium","llando"};
int starwars_size=10;

char *input[100]; //the words that are inserted in the grid
int input_point[100][3];//the x,y coordinate and direction of the words that are inserted in the grid
int mark[100]={0};//used to mark which all words have been identified by the user so that there is no duplicate answer accepted

int flag=0; //flag

struct point shift_point(struct point start, enum direction d){ //start is the position of the point to be shifted in the grid, d is the direction in which the point has to shift
    int i = start.x;
    int j = start.y;
    struct point new_point;  //final position of the shifted point depends on the direction d
    switch(d){
    //eight directions in which the point can move
        case UP:
            new_point.x = i-1; //Move up a row
            new_point.y = j;
            break;
        case DOWN:
            new_point.x = i+1;  //Move down a row
            new_point.y = j;
            break;
        case LEFT:
            new_point.x = i;
            new_point.y = j-1; //Column moves left
            break;
        case RIGHT:
            new_point.x = i;
            new_point.y = j+1; //Column moves right
            break;
        case UP_LEFT:
            new_point.x = i-1; //Row moves up
            new_point.y = j-1; //Column moves left
            break;
        case UP_RIGHT:
            new_point.x = i-1; //Row moves up
            new_point.y = j+1; //Column moves right
            break;
        case DOWN_LEFT:
            new_point.x = i+1; //Row moves down
            new_point.y = j-1; //Column moves to left
            break;
        case DOWN_RIGHT:
            new_point.x = i+1; //Row moves down
            new_point.y = j+1; //Column moves right
            break;
        default:
            new_point.x = i; //Row stays the same
            new_point.y = j; //Column stays the same
            break;
    }
//Handle out of bounds errors
    if(new_point.x < -1 || new_point.x > grid_size || new_point.y < -1 || new_point.y > grid_size){
        flag=1;   //exception handling
    }
    return new_point;  //return the new point
}


int check_insert(char* word, struct point start,enum direction d){  //function to check whether a word can be inserted in the grid or not at that position(start)
    int i = 0;   //loop variable
    struct point new_point = start;
    while(i < (int)strlen(word)) //Iterates through the word char array
    {
    //Attempt to shift the point to the new point
        if(grid[new_point.x][new_point.y] == nullchar||grid[new_point.x][new_point.y]==word[i]){
            new_point = shift_point(new_point,d);
            i++;
        }
        else{
            return 0;   //if cant be inserted return false
        }

        if(flag==1)
            return 0;  //this flag has been set in shift_point function due to array bounds exception
    }
    return 1;  //can be inserted so return true
}

int arr[10]={1,3,2,4,5,6,1,3,2,4};  //this array ensures 40,40,20 percent division of words to horizontal, vertical and diagonal direction
int p=0;   //used as an index to traverse the above array arr

void insertWordInGrid(char *word,int i)   //function to insert word in the grid
{  //i signifies that the i th word is being inserted

    struct point place; //point where the word is to inserted in the grid
    enum direction d;
    do{
        place.x = rand() % grid_size; //set to a random row
        place.y = rand() % grid_size; //set to a random column
        d = (arr[(p++)%10]); //get a direction according to the rule specified

    }
    while(!check_insert(word,place,d));  //run the loop until we cant insert the word

    int j = 0;//loop variable

    struct point new_point = place;
    while(j < (int)strlen(word)){
        grid[new_point.x][new_point.y] = (char)toupper(word[j]);   //insertion into the grid
        new_point = shift_point(new_point,d);   //shift according to direction
        j++;
    }
    input_point[i][0]=place.x;  //used in changing the grid afterwards when user enters the correct answer
    input_point[i][1]=place.y;
    input_point[i][2]=d;

}

char generate_random_char(){   //function to generate a random char
    return 'A' + rand()%26;
}

void fill_grid(){  //fill the remaining places with random characters
    int i,k;
    for(i=0;i<grid_size;i++){
        for(k=0;k<grid_size;k++){
            if(grid[i][k] == nullchar){
                grid[i][k] = generate_random_char(); //Set every null value to a random character
            }
        }
    }
}


void printGrid()   //function to print the grid
{
    int i,j;
    for(i=0;i<grid_size;i++){
        for(j=0;j<grid_size;j++){
            HANDLE h = GetStdHandle ( STD_OUTPUT_HANDLE );
            WORD wOldColorAttrs;
            CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

  /*
   * First save the current color information
   */
            GetConsoleScreenBufferInfo(h, &csbiInfo);
            wOldColorAttrs = csbiInfo.wAttributes;

  /*
   * Set the new color information
   */
            SetConsoleTextAttribute ( h, color[i][j] );



  /*
   * Restore the original colors
   */


            printf("%c ",grid[i][j]);
            SetConsoleTextAttribute ( h, wOldColorAttrs);
        }
        printf("\n");
    }
}

int checkans(char *str){  //function to check whether str is in the grid or not
    ctr++;//color counter
    int i=0;

    int flag=0;
    for(i=0;i<max_words;i++){  //check in input array

        if(strcmp(str,input[i])==0){  //condition satisfied and str is in the grid
            if (mark[i]==1){
                return 0;
            }


            flag=1;
            break;
        }

    }
    if(flag==1){  //then str is in the grid and change the grid
        mark[i]=1;
        int x=0;
        struct point p;
        p.x=input_point[i][0];
        p.y=input_point[i][1];
        enum direction d=input_point[i][2];
        for(x=0;x<strlen(str);x++){
            color[p.x][p.y]=ctr;
            //grid[p.x][p.y]='*';    //change the grid where the word is there
            p=shift_point(p,d);
        }
        return 1;   //true condition
    }
    else
        return 0;   //false condition
}


int main(){
    srand(1);   //seed the random number generator
    printf("Welcome to Word Search 3000!\n\n\n");
    char name[100];
    int ans;
    printf("What's your name? ");
    scanf(" %s",&name);
    printf("\nHi %s!!! What category do you want to choose? ",name);
    printf("\nPress 1 for Animals..");
    printf("\nPress 2 for Fruits..");
    printf("\nPress 3 for Games..");
    printf("\nPress 4 for Star Wars..");

    scanf("%d",&ans);
    printf("\nEnter the size of the grid..");
    scanf("%d",&grid_size);
    int i,j;

    for(i=0;i<grid_size;i++){   //initialize the grid to null char 'z'
        for(j=0;j<grid_size;j++){
            grid[i][j]='z';
            color[i][j]=15;
        }

    }

    max_words=grid_size-4;   //maximum words in the grid can be grid_size-4

    char *item;

    if(ans==1){
        item="animals";
        int i=0;
        if(max_words>animals_size)
            max_words=animals_size;
        for(i=0;i<max_words;i++){

            input[i]=animals[i];
            insertWordInGrid(animals[i],i);
        }

    }
    else if(ans==2){
        item="fruits";
        int i=0;
        if(max_words>fruits_size)
            max_words=fruits_size;
        for(i=0;i<max_words;i++){

            input[i]=fruits[i];
            insertWordInGrid(fruits[i],i);
        }

    }
    else if(ans==3){
        item="games";
        int i=0;
        if(max_words>games_size)
            max_words=games_size;
        for(i=0;i<max_words;i++){

            input[i]=games[i];
            insertWordInGrid(games[i],i);
        }
    }
    else if(ans==4){
        item="Star Wars";
        int i=0;
        if(max_words>starwars_size)
            max_words=starwars_size;
        for(i=0;i<max_words;i++){

            input[i]=starwars[i];
            insertWordInGrid(starwars[i],i);
        }
    }


    int t=max_words/4;   //time in minutes in which the user can do the puzzle
    system("cls");
    printf("Ok %s! There are %d %s hidden in this grid. Let's see if you can find them.\nYou have %d minutes to solve. GOOD LUCK!! \n\n",name,max_words,item,t);
    fill_grid();
    printGrid();

    clock_t start = clock();


    for(i=0;i<max_words;i++){
        char ans[50];

        clock_t end = clock();
        float seconds = (float)(end - start) / CLOCKS_PER_SEC;

        printf("\nYou have given %d right answers..",i);
        printf("\nTime Remaining: %f seconds",t*60-seconds);
        printf("\nEnter answer(in lower case)..");


        scanf(" %s",&ans);


        if(seconds>t*60){
            system("cls");
            printf("TIME UP!!!You suck!!!");
            return 1;
        }

        if(checkans(ans)){
            system("cls");
            printf("Your answer was correct!!\n");

            printGrid();
        }
        else{
            system("cls");
            printf("Wrong answer!!\n");
            Beep(1000,1000);
            printGrid();
            i--;
        }


    }
    system("cls");
    printGrid();
    printf("\nCongratulations!!!You win!!!");
    return 0;


}
