/*
PROGRAM TO SOLVE THE RUBIK'S CUBE

Programmer : kurithesheep
Version : 1.1

This program aim's to solve a rubiks cube in the most efficient way possible, without the program becoming unreadable.

I've decided to use a 6X3X3 3D array to store the currenct state of the rubik's cube.
Each rotation involves 21(20+1 redundant centre piece) transitions.

Alright, that's enough documentation, let's hit the code.
*/


/*
REPRESENTATION OF THE FACES
Here, I've chosen a 6X3X3 integer array to do the job.
There are six faces in total :
0 - Front
1 - Up
2 - Down
3 - Right
4 - Left
5 - Back

there are six colours in total:
B - Blue
Y - Yellow
W - White
R - Red
O - Orange
G - Green

thus [0][1][1] = 'B' means that the centre pieace at the front face is Blue
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define FRONT 0
#define UP 1
#define TOP 1
#define DOWN 2
#define BOTTOM 2
#define RIGHT 3
#define LEFT 4
#define BACK 5

struct node{
	char face;
	int up;
	int down;
	int left;
	int right;
}ori[6];

struct coord{
	int x;
	int y;
	int z;
};

char str[15];
char final_algo[5000];
int counter;

typedef struct node node;
typedef struct coord coord;

void strrev(char *stri)
{
   char str[100], temp;
   int i, j = 0;

   for(j=0;stri[j]!='\0'; j++);
   i = 0;
   j--;

   while (i < j) {
      temp = stri[i];
      stri[i] = stri[j];
      stri[j] = temp;
      i++;
      j--;
   }
}

void strinv(char inv[]){
	int i;
	for(i=0; inv[i]!='\0'; i++){
		switch(inv[i]){
			case 'B':
				inv[i]='A';
				break;
			case 'D':
				inv[i]='C';
				break;
			case 'F':
				inv[i]='E';
				break;
			case 'L':
			inv[i]='G';
				break;
			case 'R':
				inv[i]='H';
				break;
			case 'U':
				inv[i]='I';
				break;
			case 'A':
				inv[i]='B';
				break;
			case 'C':
				inv[i]='D';
				break;
			case 'E':
				inv[i]='F';
				break;
			case 'G':
				inv[i]='L';
				break;
			case 'H':
				inv[i]='R';
				break;
			case 'I':
				inv[i]='U';
				break;
			default:
				printf("Fatal error in inverse\n");
		}
	}
}

void FaceCounter(int face, int faces[6][3][3])
{
	//subroutine to rotate the contents of a face
	//counter clockwise
	int temp;

	//corners first
	temp=faces[face][0][0];
	faces[face][0][0] = faces[face][0][2];
	faces[face][0][2] = faces[face][2][2];
	faces[face][2][2] = faces[face][2][0];
	faces[face][2][0] = temp;

	//then middles
	temp=faces[face][0][1];
	faces[face][0][1] = faces[face][1][2];
	faces[face][1][2] = faces[face][2][1];
	faces[face][2][1] = faces[face][1][0];
	faces[face][1][0] = temp;
}

void FaceClock(int face, int faces[6][3][3])
{
	//subroutine to rotate the contents of a face
	//clockwise
	int temp;

	//corners first
	temp=faces[face][0][0];
	faces[face][0][0] = faces[face][2][0];
	faces[face][2][0] = faces[face][2][2];
	faces[face][2][2] = faces[face][0][2];
	faces[face][0][2] = temp;

	//then middles
	temp=faces[face][0][1];
	faces[face][0][1] = faces[face][1][0];
	faces[face][1][0] = faces[face][2][1];
	faces[face][2][1] = faces[face][1][2];
	faces[face][1][2] = temp;
}

coord get_edgepos(int faces[6][3][3]){
	//reads the colour at buffer, return's the position of the piece needed (edges)
	char x, y;
	coord c1;
	int cur_face, adj_face,i;
	x=(char)faces[TOP][1][2];
	y=(char)faces[RIGHT][0][1];

	switch(x){
		case 'B':
			cur_face = FRONT;
			break;
		case 'Y':
			cur_face = UP;
			break;
		case 'O':
			cur_face = LEFT;
			break;
		case 'G':
			cur_face = BACK;
			break;
		case 'W':
			cur_face = DOWN;
			break;
		case 'R':
			cur_face = RIGHT;
			break;
		default:
			printf("Fatal error at get_edgepos\n");
	}


	c1.x = cur_face;
	c1.y = 1;
	c1.z = 1;

	if(y == (char)faces[ori[cur_face].right][1][1]){
		c1.z++;
	}
	else if(y == (char)faces[ori[cur_face].left][1][1]){
		c1.z--;
	}
	else if(y == (char)faces[ori[cur_face].up][1][1]){
		c1.y--;
	}
	else if(y == (char)faces[ori[cur_face].down][1][1]){
		c1.y++;
	}
	else{
		printf("Fatal error in finding adjoining face\n");
	}
	return c1;
}

char* edge_check(coord c1){

	strcpy(str,"");
	if(c1.x==TOP && c1.y==0 && c1.z==1)
	{
		strcpy(str,"RRIRR");
	}
	else if(c1.x==TOP && c1.y==2 && c1.z==1){
		strcpy(str,"RRURR");
	}
	else if(c1.x==LEFT && c1.y==0 && c1.z==1){
		strcpy(str,"HGAHIRR");
	}
	else if(c1.x==LEFT && c1.y==2 && c1.z==1){
		strcpy(str,"LLHGAHIRR");
	}
	else if(c1.x==LEFT && c1.y==1 && c1.z==0){
		strcpy(str,"HAHIRR");
	}
	else if(c1.x==LEFT && c1.y==1 && c1.z==2){
		strcpy(str,"LLHAHIRR");
	}
	else if(c1.x==FRONT && c1.y==1 && c1.z==0){
		strcpy(str,"G");
	}
	else if(c1.x==FRONT && c1.y==0 && c1.z==1){
		strcpy(str,"REGH");
	}
	else if(c1.x==FRONT && c1.y==1 && c1.z==2){
		strcpy(str,"UURUU");
	}
	else if(c1.x==FRONT && c1.y==2 && c1.z==1){
		strcpy(str,"RFGH");
	}
	else if(c1.x==RIGHT && c1.y==1 && c1.z==0){
		strcpy(str,"IEU");
	}
	else if(c1.x==RIGHT && c1.y==2 && c1.z==1){
		strcpy(str,"UURUUIEU");
	}
	else if(c1.x==RIGHT && c1.y==1 && c1.z==2){
		strcpy(str,"UBI");
	}
	else if(c1.x==BACK && c1.y==0 && c1.z==1){
		strcpy(str,"HBLR");
	}
	else if(c1.x==BACK && c1.y==1 && c1.z==0){
		strcpy(str,"UBBIL");
	}
	else if(c1.x==BACK && c1.y==1 && c1.z==2){
		strcpy(str,"L");
	}
	else if(c1.x==BACK && c1.y==2 && c1.z==1){
		strcpy(str,"UAIL");
	}
	else if(c1.x==DOWN && c1.y==0 && c1.z==1){
		strcpy(str,"CLL");
	}
	else if(c1.x==DOWN && c1.y==1 && c1.z==0){
		strcpy(str,"LL");
	}
	else if(c1.x==DOWN && c1.y==1 && c1.z==2){
		strcpy(str,"DDLL");
	}
	else if(c1.x==DOWN && c1.y==2 && c1.z==1){
		strcpy(str,"DLL");
	}
	else if(c1.x==UP && c1.y==1 && c1.z==0){
		return str;
	}
	else{
        printf("FATAL ERROR IN FUNCTION OF EDGES\n");
    	strcpy(str,"");
	};
	return str;
}



int isedgesolved(int faces[6][3][3])
{

	int i,flag=1;
	for(i=0;i<6;i++)
	{
		if(faces[i][1][1]==faces[i][0][1] && faces[i][1][1]==faces[i][1][2] && faces[i][1][1]==faces[i][2][1] && faces[i][1][1]==faces[i][1][0] )
		{
			continue;
		}
		else{
			flag=0;
			return flag;
		}
	}
	return flag;
 }

coord return_unsolvede(int faces[6][3][3])
{

	coord c1;
	int i;

	for(i=0;i<6;i++)
	{
		if(faces[i][1][1]!=faces[i][0][1] )
		{
			c1.x=i;
			c1.y=0;
			c1.z=1;
			if((c1.x==TOP && c1.y==1 && c1.z==2)||(c1.x==RIGHT && c1.y==0 && c1.z==1))
				continue;
			return c1;
		}
		else if(faces[i][1][1]!=faces[i][1][2])
		{
			c1.x=i;
			c1.y=1;
			c1.z=2;
			if((c1.x==TOP && c1.y==1 && c1.z==2)||(c1.x==RIGHT && c1.y==0 && c1.z==1))
				continue;
			return c1;
		}
		else if(faces[i][1][1]!=faces[i][2][1])
		{
			c1.x=i;
			c1.y=2;
			c1.z=1;
			if((c1.x==TOP && c1.y==1 && c1.z==2)||(c1.x==RIGHT && c1.y==0 && c1.z==1))
				continue;
			return c1;
		}
		else if(faces[i][1][1]!=faces[i][1][0])
		{
			c1.x=i;
			c1.y=1;
			c1.z=0;
			if((c1.x==TOP && c1.y==1 && c1.z==2)||(c1.x==RIGHT && c1.y==0 && c1.z==1))
				continue;
			return c1;
		}
	}
	printf("FATAL ERROR IN return_unsolvede()");
	return c1;
}

char* corner_check(coord c1){

	//the stupidest part of the program (2)
	strcpy(str,"");
	if(c1.x==TOP && c1.y==0 && c1.z==2)
	{
		strcpy(str,"RR");
	}
	else if(c1.x==TOP && c1.y==2 && c1.z==2){
		strcpy(str,"HHC");
	}
	else if(c1.x==TOP && c1.y==2 && c1.z==0){
		strcpy(str,"IHUHC");
	}
	else if(c1.x==LEFT && c1.y==0 && c1.z==2){
		strcpy(str,"IHU");
	}
	else if(c1.x==LEFT && c1.y==2 && c1.z==2){
		strcpy(str,"DHC");
	}
	else if(c1.x==LEFT && c1.y==2 && c1.z==0){
		strcpy(str,"DDFD");
	}
	else if(c1.x==FRONT && c1.y==0 && c1.z==0){
		strcpy(str,"FFHC");
	}
	else if(c1.x==FRONT && c1.y==0 && c1.z==2){
		strcpy(str,"H");
	}
	else if(c1.x==FRONT && c1.y==2 && c1.z==2){
		strcpy(str,"HC");
	}
	else if(c1.x==FRONT && c1.y==2 && c1.z==0){
		strcpy(str,"DFD");
	}
	else if(c1.x==RIGHT && c1.y==0 && c1.z==0){
		strcpy(str,"HFD");
	}
	else if(c1.x==RIGHT && c1.y==0 && c1.z==2){
		strcpy(str,"ACB");
	}
	else if(c1.x==RIGHT && c1.y==2 && c1.z==2){
		strcpy(str,"RFD");
	}
	else if(c1.x==RIGHT && c1.y==2 && c1.z==0){
		strcpy(str,"FD");
	}
	else if(c1.x==BACK && c1.y==0 && c1.z==0){
		strcpy(str,"RC");
	}
	else if(c1.x==BACK && c1.y==2 && c1.z==2){
		strcpy(str,"DDHC");
	}
	else if(c1.x==BACK && c1.y==2 && c1.z==0){
		strcpy(str,"CFD");
	}
	else if(c1.x==DOWN && c1.y==0 && c1.z==0){
		strcpy(str,"D");
	}
	else if(c1.x==DOWN && c1.y==2 && c1.z==2){
		strcpy(str,"C");
	}
	else if(c1.x==DOWN && c1.y==2 && c1.z==0){
		strcpy(str,"DD");
	}
	else if(c1.x==DOWN && c1.y==0 && c1.z==2){
		return str;
	}
	else{

        printf("FATAL ERROR IN FUCKALL FUNCTION OF CORNERS\n");
		strcpy(str,"");
	}
	return str;
}

coord get_cornerpos(int faces[6][3][3]){
	//reads the colour at buffer, return's the position of the piece needed (edges)

	char x, y, z;
	coord c1;
	int cur_face, adj_face,i;
	z=(char)faces[TOP][0][0];
	y=(char)faces[BACK][0][2];
	x=(char)faces[LEFT][0][0];


	switch(x){
		case 'B':
			cur_face = FRONT;
			break;
		case 'Y':
			cur_face = UP;
			break;
		case 'O':
			cur_face = LEFT;
			break;
		case 'G':
			cur_face = BACK;
			break;
		case 'W':
			cur_face = DOWN;
			break;
		case 'R':
			cur_face = RIGHT;
			break;
		default:
			printf("Fatal error at get_edgepos \n");
	}

	c1.x = cur_face;
	c1.y = 1;
	c1.z = 1;

	if(y == (char)faces[ori[cur_face].right][1][1]){
		c1.z++;
	}
	else if(y == (char)faces[ori[cur_face].left][1][1]){
		c1.z--;
	}
	else if(y == (char)faces[ori[cur_face].up][1][1]){
		c1.y--;
	}
	else if(y == (char)faces[ori[cur_face].down][1][1]){
		c1.y++;
	}
	else{
		printf("Fatal error in finding adjoining face \n");
	}

	if(z == (char)faces[ori[cur_face].right][1][1]){
		c1.z++;
	}
	else if(z == (char)faces[ori[cur_face].left][1][1]){
		c1.z--;
	}
	else if(z == (char)faces[ori[cur_face].up][1][1]){
		c1.y--;
	}
	else if(z == (char)faces[ori[cur_face].down][1][1]){
		c1.y++;
	}
	else{
		printf("Fatal error in finding adjoining face \n");
	}

	return c1;
}

int iscbuffersolved(int faces[6][3][3]){

	if((faces[TOP][1][1] == faces[TOP][0][0] || faces[TOP][1][1]==faces[BACK][0][2] || faces[TOP][1][1] == faces[LEFT][0][0]) && (faces[BACK][1][1]==faces[BACK][0][2] || faces[BACK][1][1]==faces[LEFT][0][0]  || faces[BACK][1][1]==faces[TOP][0][0]) && (faces[LEFT][1][1]==faces[BACK][0][2] || faces[LEFT][1][1]==faces[TOP][0][0] || faces[LEFT][1][1]==faces[LEFT][0][0] ))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int iscornersolved(int faces[6][3][3])
{

	int i,flag=1;
	for(i=0;i<6;i++)
	{
		if(faces[i][1][1]==faces[i][0][0] && faces[i][1][1]==faces[i][0][2] && faces[i][1][1]==faces[i][2][0] && faces[i][1][1]==faces[i][2][2] )
		{
			continue;
		}
		else{
			flag=0;
			return flag;
		}
	}
	return flag;
 }

 coord return_unsolvedc(int faces[6][3][3])
{

	coord c1;
	int i;

	for(i=0;i<6;i++)
	{
		if(faces[i][1][1]!=faces[i][0][2])
		{
			c1.x=i;
			c1.y=0;
			c1.z=2;
			if((c1.x==4 && c1.y == 0 && c1.z ==0) || (c1.x==1 && c1.y == 0 && c1.z ==0) || (c1.x==5 && c1.y == 0 && c1.z ==2))
				continue;
			return c1;
		}
		else if(faces[i][1][1]!=faces[i][2][2])
		{
			c1.x=i;
			c1.y=2;
			c1.z=2;
			if((c1.x==4 && c1.y == 0 && c1.z ==0) || (c1.x==1 && c1.y == 0 && c1.z ==0) || (c1.x==5 && c1.y == 0 && c1.z ==2))
				continue;
			return c1;
		}
		else if(faces[i][1][1]!=faces[i][2][0])
		{
			c1.x=i;
			c1.y=2;
			c1.z=0;
			if((c1.x==4 && c1.y == 0 && c1.z ==0) || (c1.x==1 && c1.y == 0 && c1.z ==0) || (c1.x==5 && c1.y == 0 && c1.z ==2))
				continue;
			return c1;
		}
		else if(faces[i][1][1]!=faces[i][0][0])
		{
			c1.x=i;
			c1.y=0;
			c1.z=0;
			if((c1.x==4 && c1.y == 0 && c1.z ==0) || (c1.x==1 && c1.y == 0 && c1.z ==0) || (c1.x==5 && c1.y == 0 && c1.z ==2))
				continue;
			return c1;
		}
	}
	printf("FATAL ERROR IN return_unsolvede()");
	return c1;
}



void orient_init(){

  //front
  ori[FRONT].face = 'B';
  ori[FRONT].up=UP;
  ori[FRONT].down=DOWN;
  ori[FRONT].left=LEFT;
  ori[FRONT].right=RIGHT;

  //top
  ori[TOP].face = 'Y';
  ori[TOP].up=BACK;
  ori[TOP].down=FRONT;
  ori[TOP].left=LEFT;
  ori[TOP].right=RIGHT;

  //Down
  ori[DOWN].face = 'W';
  ori[DOWN].up=FRONT;
  ori[DOWN].down=BACK;
  ori[DOWN].left=LEFT;
  ori[DOWN].right=RIGHT;

  //Left
  ori[LEFT].face = 'O';
  ori[LEFT].up=UP;
  ori[LEFT].down=DOWN;
  ori[LEFT].left=BACK;
  ori[LEFT].right=FRONT;

  //Right
  ori[RIGHT].face = 'R';
  ori[RIGHT].up=UP;
  ori[RIGHT].down=DOWN;
  ori[RIGHT].left=FRONT;
  ori[RIGHT].right=BACK;

  //back
  ori[BACK].face = 'G';
  ori[BACK].up=UP;
  ori[BACK].down=DOWN;
  ori[BACK].left=RIGHT;
  ori[BACK].right=LEFT;
}


void SolveInit(int f[6][3][3]){

	//Function to initialize it to a solved state

	int i, j, k;
	for(i=0; i<6; i++){
		for  (j=0; j<3; j++){
			for(k=0; k<3; k++){

				switch(i){
					case FRONT:
						f[i][j][k]='B';
						break;
					case UP:
						f[i][j][k]='Y';
						break;
					case DOWN:
						f[i][j][k]='W';
						break;
					case RIGHT:
						f[i][j][k]='R';
						break;
					case LEFT:
						f[i][j][k]='O';
						break;
					case BACK:
						f[i][j][k]='G';
						break;

				}
			}
		}
	}
}



void Display(int faces[6][3][3]){

	//Function to display the faces of the cube

	int i, j, k;
	for(i=0; i<6; i++){

		switch(i){
			case FRONT:
				printf("FRONT FACE:\n");
				break;
			case UP:
				printf("UP FACE:\n");
				break;
			case DOWN:
				printf("DOWN FACE:\n");
				break;
			case RIGHT:
				printf("RIGHT FACE:\n");
				break;
			case LEFT:
				printf("LEFT FACE:\n");
				break;
			case BACK:
				printf("BACK FACE:\n");
				break;
		}

		for(j=0;j<3;j++){
			for(k=0;k<3;k++){
				printf("%c ", (char)faces[i][j][k]);
			}
			printf("\n");
		}

		printf("\n\n");
	}
}

void BackClock(int faces[6][3][3])
{
	strcat(final_algo,"B ");
	counter++;
	//Back face, clockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[UP][0][x];
		faces[UP][0][x] = faces[RIGHT][x][2];
		faces[RIGHT][x][2] = faces[DOWN][2][2-x];
		faces[DOWN][2][2-x] = faces[LEFT][2-x][0];
		faces[LEFT][2-x][0] = temp[x];

	}

	//now rotate the face surface
	FaceClock(BACK, faces);
}

void BackCounter(int faces[6][3][3])
{
	strcat(final_algo,"Bi ");
	counter++;
	//Back face, counterclockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[UP][0][x];
		faces[UP][0][x] = faces[LEFT][2-x][0];
		faces[LEFT][2-x][0] = faces[DOWN][2][2-x];
		faces[DOWN][2][2-x] = faces[RIGHT][x][2];
		faces[RIGHT][x][2] = temp[x];

	}

	//now rotate the face surface
	FaceCounter(BACK, faces);
}

void DownClock(int faces[6][3][3])
{
	strcat(final_algo,"D ");
	counter++;
	//  down face, clockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[FRONT][2][x];
		faces[FRONT][2][x] = faces[LEFT][2][x];
		faces[LEFT][2][x] = faces[BACK][2][x];
		faces[BACK][2][x] = faces[RIGHT][2][x];
		faces[RIGHT][2][x] = temp[x];

	}

	//now rotate the face surface
	FaceClock(DOWN, faces);
}

void DownCounter(int faces[6][3][3])
{
	strcat(final_algo,"Di ");
	counter++;
	//  down face, counterclockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[FRONT][2][x];
		faces[FRONT][2][x] = faces[RIGHT][2][x];
		faces[RIGHT][2][x] = faces[BACK][2][x];
		faces[BACK][2][x] = faces[LEFT][2][x];
		faces[LEFT][2][x] = temp[x];

	}

	//now rotate the face surface
	FaceCounter(DOWN, faces);
}

void FrontClock(int faces[6][3][3])
{
	strcat(final_algo,"F ");
	counter++;
	//  front face, clockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[UP][2][x];
		faces[UP][2][x] = faces[LEFT][2-x][2];
		faces[LEFT][2-x][2] = faces[DOWN][0][2-x];
		faces[DOWN][0][2-x] = faces[RIGHT][x][0];
		faces[RIGHT][x][0] = temp[x];

	}

	//now rotate the face surface
	FaceClock(FRONT, faces);
}

void FrontCounter(int faces[6][3][3])
{
	strcat(final_algo,"Fi ");
	counter++;
	//  front face, counterclockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[UP][2][x];
		faces[UP][2][x] = faces[RIGHT][x][0];
		faces[RIGHT][x][0] = faces[DOWN][0][2-x];
		faces[DOWN][0][2-x] = faces[LEFT][2-x][2];
		faces[LEFT][2-x][2] = temp[x];

	}

	//now rotate the face surface
	FaceCounter(FRONT, faces);
}

void LeftClock(int faces[6][3][3])
{
	strcat(final_algo,"L ");
	counter++;
	//  left face, clockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[UP][x][0];
		faces[UP][x][0] = faces[BACK][2-x][2];
		faces[BACK][2-x][2] = faces[DOWN][x][0];
		faces[DOWN][x][0] = faces[FRONT][x][0];
		faces[FRONT][x][0] = temp[x];

	}

	//now rotate the face surface
	FaceClock(LEFT, faces);
}

void LeftCounter(int faces[6][3][3])
{
	strcat(final_algo,"Li ");
	counter++;
	//  left face, counter clockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[UP][x][0];
		faces[UP][x][0] = faces[FRONT][x][0];
		faces[FRONT][x][0] = faces[DOWN][x][0];
		faces[DOWN][x][0] = faces[BACK][2-x][2];
		faces[BACK][2-x][2] = temp[x];

	}

	//now rotate the face surface
	FaceCounter(LEFT, faces);
}

void RightClock(int faces[6][3][3])
{
	strcat(final_algo,"R ");
	counter++;
	//  right face, clockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[UP][x][2];
		faces[UP][x][2] = faces[FRONT][x][2];
		faces[FRONT][x][2] = faces[DOWN][x][2];
		faces[DOWN][x][2] = faces[BACK][2-x][0];
		faces[BACK][2-x][0] = temp[x];

	}

	//now rotate the face surface
	FaceClock(RIGHT, faces);
}

void RightCounter(int faces[6][3][3])
{
	strcat(final_algo,"Ri ");
	counter++;
	//  right face, counterclockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[UP][x][2];
		faces[UP][x][2] = faces[BACK][2-x][0];
		faces[BACK][2-x][0] = faces[DOWN][x][2];
		faces[DOWN][x][2] = faces[FRONT][x][2];
		faces[FRONT][x][2] = temp[x];

	}

	//now rotate the face surface
	FaceCounter(RIGHT, faces);
}

void UpClock(int faces[6][3][3])
{
	strcat(final_algo,"U ");
	counter++;
	//  up face, clockwise rotation
	int temp[3];
	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[FRONT][0][x];
		faces[FRONT][0][x] = faces[RIGHT][0][x];
		faces[RIGHT][0][x] = faces[BACK][0][x];
		faces[BACK][0][x] = faces[LEFT][0][x];
		faces[LEFT][0][x] = temp[x];

	}

	//now rotate the face surface
	FaceClock(UP, faces);
}

void UpCounter(int faces[6][3][3])
{
	strcat(final_algo,"Ui ");
	counter++;
	//  up face, counterclockwise rotation
	int temp[3];

	int x; //counter

	//move the outer edges affected by the rotation
	for (x=0; x<3; x++)
	{
		temp[x] = faces[FRONT][0][x];
		faces[FRONT][0][x] = faces[LEFT][0][x];
		faces[LEFT][0][x] = faces[BACK][0][x];
		faces[BACK][0][x] = faces[RIGHT][0][x];
		faces[RIGHT][0][x] = temp[x];

	}

	//now rotate the surface
	FaceCounter(UP, faces);
}


int check(int faces[6][3][3])
{
	//Determine how many cubies are misplaced
	int x,y,z; //counter
	int misplaced = 0;

	//move the outer edges affected by the rotation
	for (x=0; x<6; x++)
		for (y=0; y<3; y++)
			for (z=0; z<3; z++)
				if (faces[x][y][z] != faces[x][1][1])
				{ misplaced++; }

	return misplaced;
}



void RandomShuffle(int faces[6][3][3]){
	int i;
	for(i=0; i<20; i++){
		int x=rand() % 6;
		switch(x){
			case 0:
				BackClock(faces);
				break;
			case 1:
				FrontClock(faces);
				break;
			case 2:
				UpClock(faces);
				break;
			case 3:
				RightClock(faces);
				break;
			case 4:
				LeftClock(faces);
				break;
			case 5:
				DownClock(faces);
				break;
		}
	}
}

void str_algo(char st[], int faces[6][3][3])
{
  int i;
  for(i=0;st[i]!='\0';i++)
  {
    if(st[i]=='B')
      BackClock(faces);

    else if(st[i]=='D')
      DownClock(faces);

    else if(st[i]=='F')
      FrontClock(faces);

    else if(st[i]=='L')
      LeftClock(faces);

    else if(st[i]=='R')
      RightClock(faces);

    else if(st[i]=='U')
      UpClock(faces);

    else if(st[i]=='A')
        BackCounter(faces);

    else if(st[i]=='C')
        DownCounter(faces);

    else if(st[i]=='E')
        FrontCounter(faces);

    else if(st[i]=='G')
        LeftCounter(faces);

    else if(st[i]=='H')
        RightCounter(faces);

    else if(st[i]=='I')
        UpCounter(faces);

  }

}

int isebuffersolved(int faces[6][3][3]){
	char stri[15];
	if((faces[TOP][1][1] == faces[TOP][1][2] && faces[RIGHT][1][1]==faces[RIGHT][0][1])||(faces[TOP][1][1] == faces[RIGHT][0][1] && faces[RIGHT][1][1]==faces[TOP][1][2]))
	{
		return 1;
	}

	else
	{
		return 0;
	}
}

void edge(int faces[6][3][3]){
	//based on the pos, it'll get the edge pos opp to buffer, and then perform switch algo

	char stri[15];
	int count=0;


	while(!(isedgesolved(faces)) /*&& isebuffersolved(faces)*/)
	{		count++;

			if(count>=13 && count<20 && !isebuffersolved(faces))
			{
				str_algo("RUHIHFRRIHIRUHE", faces);
				strcpy(stri,"HGAHIRR");		//algorith for left 0-1 to opp buffer position
				str_algo(stri, faces);
				str_algo("RUHIHFRRIHIRUHE", faces);
				strrev(stri);
				strinv(stri);
				str_algo(stri, faces);

			}
			if(count>=20)
			{
				printf("\n\n\n  Edges not being solved going infinite. \n\n ");

			}
		if(isebuffersolved(faces))
		{
			//find unsolved edge and replace with buffer

			coord c;
			c=return_unsolvede(faces);
			strcpy(stri,edge_check(c));
			str_algo(stri, faces);

			str_algo("RUHIHFRRIHIRUHE", faces);

			strrev(stri);
			strinv(stri);
			str_algo(stri, faces);


		}

		coord c1;
		c1=get_edgepos(faces);
		strcpy(stri,edge_check(c1));
		str_algo(stri, faces);

		str_algo("RUHIHFRRIHIRUHE", faces);

		strrev(stri);
		strinv(stri);
		str_algo(stri, faces);

	}

	printf("\n\n\n\n  EDGES SOLVED  \n\n\n");
	Display(faces);
	getch();

}

void corner(int faces[6][3][3])
{
		//based on the pos, it'll get the corner pos opp to buffer, and then perform switch algo

	char stri[15];
	int count=0;


	while(!(iscornersolved(faces)))
	{
		count++;
		if(count>=15)
		{
			printf("\n\n\n   Corner not being solved. \n\n");

		}
		if(iscbuffersolved(faces))
		{

			coord c;
			c=return_unsolvedc(faces);
			strcpy(stri,corner_check(c));
			str_algo(stri, faces);
			str_algo("RIHIRUHERUHIHFR", faces);
			strrev(stri);
			strinv(stri);
			str_algo(stri, faces);



		}

		coord c1;
		c1=get_cornerpos(faces);


		strcpy(stri,corner_check(c1));

		str_algo(stri, faces);
		str_algo("RIHIRUHERUHIHFR", faces);
		strrev(stri);
		strinv(stri);
		str_algo(stri, faces);

	}

	printf("\n\n\n   CORNER SOLVED \n\n");


}

void parity(int faces[6][3][3]){
	str_algo("RIHIRURDHIRCHUUHI", faces);
}

int main(){
	//initialization
	int faces[6][3][3];		//the original cube. Consists of 6 3X3 arrays, in other words, 6 faces of the cube
	strcpy(final_algo,"");
	counter=0;
	SolveInit(faces);
	orient_init();

	//shuffling the cube
	srand(time(NULL));
	RandomShuffle(faces);	
	printf("Shuffle Alogorithm : \n%s\n", final_algo);
	printf("\n\n");
	getch();
	strcpy(final_algo,"");
	counter=0;	
	printf("Original Shuffled Cube:\n");
	Display(faces);

	getch();
	
	
	//solving the cube
	edge(faces);
	corner(faces);


	//parity
	if(check(faces)){
		parity(faces);
	}

	
	printf("Final solution!\n");
	Display(faces);

	
	getch();
	printf("Final Algorithm (%d moves) : \n", counter);
	printf("%s\n", final_algo);

	return 0;
}
