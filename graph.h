#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


char* remove$(char *str)
{
    int i,j = 1;
    char *result = malloc(sizeof(char)*(strlen(str) + 1));
for(i = 0; i<strlen(str); i++)
    {
    if(str[i]>=48 && str[i]<=57 || str[i]>=65 && str[i]<=90 || str[i]>=97 && str[i]<=122 || str[i] == 32 || str[i] == 95)
        result[i] = str[i];
    else
    {
    j = 0;
        result[i] = '_';
    }
}
    result[strlen(str)] = '\0';
    //if(j == 0)
    //printf("\"%s\"->This name contains special character, removing and continuing execution!!",str);
    return result;
}

int count = 0;
int starti,startj,flag =0;
struct net
{
    int name;
    double x,y;
};

struct element
{
    int present ;
    char* elem;int ele;
    struct net left ;
    struct net right ;
    double value;
    char unit;
    int b ,meg;
    double offset,amplitude,frequency,delay,damping;
    struct element* next;
};

struct location_array
{
    double x,y;
    struct location_array* next;
};

struct element  g[1000][1000];
int             visited[1000][1000];
char*           name[1000];
struct location_array* location;
char* filename ;
double net_location [1000][2];
int is_present [1000];
double i_length = 43;
double r_length = 100;
double c_length = 34;
double v_length = 90;
double a_length = 80;

int getHashIndex(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    int dd = (int)(hash%1000);
    return (dd);
}

void set_points(int n)
{
    FILE *f = fopen(filename, "a");
    struct location_array* top = (struct location_array *) malloc(sizeof(struct location_array));
    int i;
    double r = 100.0,cx = 400.0,cy = 400.0, thita = 0.0;
    top->x = cx;
    top->y = cy + r;
    fprintf(f, "<circle fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
    fprintf(f, "cx = \"%lf\" cy = \"%lf\" r= \"1\"/>\n", top->x, top->y);
    location = top;
    for(i = 1; i < n; i++)
    {
        struct location_array* temp = (struct location_array *) malloc(sizeof(struct location_array));
        thita += 2*3.14/n;
        temp->x = cx + r*sin(thita);
        temp->y = cy + r*cos(thita);
        fprintf(f, "<circle fill = \"black\" stroke = \"black\" stroke-width = \"1px\"\n");
        fprintf(f, "cx = \"%lf\" cy = \"%lf\" r= \"1\"/>\n", temp->x, temp->y);
        top->next = temp;
        top = temp;
    }
    fclose(f);
}

void print(struct element* a)
{
    FILE *f = fopen(filename, "a");
    double length,scale,tx,ty,rotate,inx,iny,fiy,fix,slopep;
    int i = 0;
    char c;

    while(a != NULL)
    { 
    c = a->unit;
        if(is_present[a->left.name] != 1)
        {
            net_location[a->left.name][0] = location->x;
            net_location[a->left.name][1] = location->y;
            location = location->next;
            is_present[a->left.name] = 1;
            fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\">%s</text>", net_location[a->left.name][0], net_location[a->left.name][1], remove$(name[a->left.name]));
        }
        if(is_present[a->right.name] != 1)
        {
            net_location[a->right.name][0] = location->x;
            net_location[a->right.name][1] = location->y;
            location = location->next;
            is_present[a->right.name] = 1;
            fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\">%s</text>", net_location[a->right.name][0], net_location[a->right.name][1], remove$(name[a->right.name]));
        }
        a->left.x = net_location[a->left.name][0];
        a->left.y = net_location[a->left.name][1];
        a->right.x = net_location[a->right.name][0];
        a->right.y = net_location[a->right.name][1];
        slopep = -atan((a->right.x - a->left.x)/(a->right.y - a->left.y));


        if(slopep >= 0.001 )
        {inx = a->left.x + 70.0*i*cos(slopep)*abs(slopep)/slopep;
        fix = a->right.x + 70.0*i*cos(slopep)*abs(slopep)/slopep;
        iny = a->left.y + 70.0*i*sin(slopep);
        fiy = a->right.y + 70.0*i*sin(slopep);}
        else
        {inx = a->left.x + 70.0*i*cos(slopep);
        fix = a->right.x + 70.0*i*cos(slopep);
        iny = a->left.y + 70.0*i*sin(slopep);
        fiy = a->right.y + 70.0*i*sin(slopep);}




        fprintf(f, "<path fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
        fprintf(f, "d = \"M %lf %lf L %lf %lf\"/>\n",a->left.x,a->left.y, inx, iny);
//  fprintf(f, "L %lf %lf\"/>\n", inx, iny);
        fprintf(f, "<path fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
    fprintf(f, "d = \"M %lf %lf L %lf %lf\"/>\n",a->right.x,a->right.y,fix,fiy);
//  fprintf(f, "L %lf %lf\"/>\n",fix,fiy);        
        fprintf(f, "<path fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
        fprintf(f, "d = \"M %lf %lf L %lf %lf\"/>\n", inx, iny, (3*inx/4 + fix/4), (3*iny/4 + fiy/4));
        fprintf(f, "<path fill = \"black\" stroke = \"black\" stroke-width = \"1\"\n");
        fprintf(f, "d = \"M %lf %lf L %lf %lf\"/>\n", fix, fiy, (inx/4 + 3*fix/4), (iny/4 + 3*fiy/4));
        inx = (3*inx/4 + fix/4);
        iny = (3*iny/4 + fiy/4); 
        fix = (inx/4 + 3*fix/4);
        fiy = (iny/4 + 3*fiy/4); 
        if(a->ele == 0)//resistor
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/r_length;
            if(inx < fix)
            {
                tx = inx/scale - 0.0;
                ty = iny/scale - 40.0;
            }
            else
            {
                tx = fix/scale - 0.0;
                ty = fiy/scale - 40.0;  
            }
            rotate = 180*atan((fiy - iny)/(fix - inx))/3.14159;
            fprintf(f, "<polyline fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "points=\"0,40 10,40 15,50 25,30 35,50 45,30 55,50 65,30 75,50 85,30 90,40 100,40\"");
            fprintf(f, "\n transform = \"scale(%lf)", scale);
            fprintf(f, "translate(%lf, %lf)", tx, ty);
            fprintf(f, "rotate(%lf %lf %lf)\"\n", rotate, 0.0, 40.0);
            fprintf(f, "/>\n");
        fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"black\">%s   %g%c</text>", a->elem, (inx + fix)/2, iny + 10.0, a->value, c);    
        }
        else if(a->ele == 1)//inductor
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/i_length;
            if(inx < fix)
            {
                tx = inx/scale - 1;
                ty = iny/scale - 8.5;
            }
            else
            {
                tx = fix/scale - 1;
                ty = fiy/scale - 8.5;   
            }
            rotate = 180*atan((fiy - iny)/(fix - inx))/3.14159;
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f , "d=\"M 1,8.5 L 6.5,8.5 C 6.5,8.5 6.5,4.5 10.5,4.5 C 14.5,4.5 14.5,8.5 14.5,8.5 C 14.5,8.5 14.5,4.5 18.5,4.5 C 22.5,4.5 ");
            fprintf(f , "22.5,8.5 22.5,8.5 C 22.5,8.5 22.5,4.5 26.5,4.5 C 30.5,4.5 30.5,8.5 30.5,8.5 C 30.5,8.5 30.5,4.5 34.5,4.5 C 38.5,4.5 38.5,8.5 ");
            fprintf(f, "38.5,8.5 L 44,8.5\"");
            fprintf(f, "\n transform = \"scale(%lf)", scale);
            fprintf(f, "translate(%lf, %lf)", tx, ty);
            fprintf(f, "rotate(%lf %lf %lf)\"\n", rotate, 1.0, 8.5);
            fprintf(f, "/>\n");
    fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"black\">%s   %g%cH</text>", a->elem, inx + 10.0, iny + 10.0, a->value, c);
        }
        else if(a->ele == 2)//capacitor
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/c_length;
            if(inx < fix)
            {
                tx = inx/scale - 1.0;
                ty = iny/scale - 16.0;
            }
            else
            {
                tx = fix/scale - 1.0;
                ty = fiy/scale - 16.0;  
            }
            rotate = 180*atan((fiy - iny)/(fix - inx))/3.14159;
            fprintf(f, "<g transform = \"scale(%lf) translate(%lf, %lf) rotate(%lf %lf %lf)\">\n", scale, tx, ty, rotate, 1.0, 16.0);
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f, "d=\"M 16,10.0010577 C 16,21.999992 16,21.999992 16,21.999992\"/>");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f, "d=\"M 19.000016,21.999994 L 19.000016,10.0010588\"/>");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f, "d=\"M 16.11348,16 1,16\"/>");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"0.5\"\n");
            fprintf(f, "d=\"M 18.980533,16 L 35,16\"");
            fprintf(f, "/>\n");
            fprintf(f, "</g>");
        fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"black\">%s   %g%cF</text>", a->elem, (inx + fix)/2, iny + 10.0, a->value, c);
        }
        else if(a->ele == 3)//current
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/a_length;
            if(inx < fix)
            {
                tx = inx/scale - 25.0;
                ty = iny/scale - 0.0;
            }
            else
            {
                tx = fix/scale - 25.0;
                ty = fiy/scale - 0.0;   
            }
            rotate = -90 + (180*atan((fiy - iny)/(fix - inx))/3.14159);
            fprintf(f, "<g transform = \"scale(%lf) translate(%lf, %lf) rotate(%lf, %lf, %lf)\">\n", scale, tx, ty, rotate, 25.0, 0.0);
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 25,35.04878 L 25,17\"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 25,15.5 L 27,18.5 L 23,18.5 L 25,15.5 z \"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 40,25 C 40,33.284271 33.284271,40 25,40 C 16.715729,40 10,33.284271 10,25 C 10,16.715729 16.715729,10 25,10 C 33.284271,10 40,16.715729 40,25 L 40,25 z \"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 25,10 L 25,0\"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 25,40 L 25,80\"/>\n");
            fprintf(f, "</g>");
fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"red\">%s  SINE ( %g %g %g%chz %gS %g )</text>", a->elem, (inx + fiy)/2, iny + 10.0, a->offset, a->amplitude, a->frequency, c, a->delay, a->damping);
        }
        else    //battery
        {
            length = sqrt(pow((fix - inx),2) + pow((fiy - iny),2));
            scale = length/v_length;
            if(inx < fix)
            {
                tx = inx/scale - 0.0;
                ty = iny/scale - 25.0;
            }
            else
            {
                tx = fix/scale - 0.0;
                ty = fiy/scale - 25.0;  
            }
            rotate = 180*atan((fiy - iny)/(fix - inx))/3.14159;
            fprintf(f, "<g transform = \"scale(%lf) translate(%lf, %lf) rotate(%lf, %lf, %lf)\">\n", scale, tx, ty, rotate, 0.0, 25.0);
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 40,25 C 40,33.284271 33.284271,40 25,40 C 16.715729,40 10,33.284271 10,25 C 10,16.715729 16.715729,10 25,10 C 33.284271,10 40,16.715729 40,25 L 40,25 z \"/>");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 0,25 L 10,25\"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 40,25 L 90,25\"/>\n");
            fprintf(f, "<path fill = \"none\" stroke = \"black\" stroke-width = \"1\"\n");
            fprintf(f, "d=\"M 16,25.000005 C 16,25.000005 17.5,20.000004 20.5,20.000004 C 23.5,20.000004 26.5,30.000006 29.5,30.000006 C 32.5,30.000006 34,25.000005 34,25.000005\"/>\n");
            fprintf(f, "</g>");
    fprintf(f, "<text x=\"%lf\" y=\"%lf\" font-size = \"10\" stroke = \"red\">%s   SINE ( %g %g %g%chz %gS %g )</text>", a->elem, (inx + fix)/2, iny + 10.0, a->offset, a->amplitude, a->frequency, c, a->delay, a->damping);
        }
        a = a->next;
        i++;
    }
    fclose(f);
}

void dfs(int i,int j)
{   
    if(visited[i][j]==1)
        return;
    else
    {   
        visited[i][j] = 1;
        visited[j][i] = 1;
        print(&g[i][j]);
        int k,flag = 0;
        for(k=0;k<1000;k++)
        {
            if(g[i][k].present == 1)
            {   
                if(k != j)
                    flag = 1;
                dfs(i,k);
            }
        }
        
        if(flag == 0 && strcmp(name[i], "0"))
            printf("Circuit has open component named : %s !!\n",remove$(name[i]));

        flag = 0;

        for(k=0;k<1000;k++)
        {
            if(g[k][j].present == 1)
            {   
                if( k != i)
                    flag = 1;
                dfs(k,j);
            }
        }
         
        if(flag == 0 && strcmp(name[j] , "0"))
            printf("Circuit has open component named : %s !!\n",remove$(name[j]));

        flag = 0;
    }
}
void printcircuit()
{
    int i,j;
    int w = 1000,h =1000;
    FILE *f = fopen(filename, "w");
    //printf("Count : %d \n",count);
    for(i=0;i<1000;i++)
        for(j=0;j<1000;j++)
            visited[i][j] = 0;

    if(name[getHashIndex("0")] == NULL)
        printf("Ground Net Not Found!! Ignoring and drawing circuit..........\n");

    fprintf(f, "<html>\n<body>\n<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"\n");
    fprintf(f, " width=\"%dpx\" height=\"%dpx\">\n", w, h);
    fclose(f);
    set_points(count);
    dfs(starti,startj);
f = fopen(filename, "a");
fprintf(f, "</svg>\n</body>\n</html>");
fclose(f);
    exit(0);
}
/*void printg()
{
    printf("\n");
    
    int i,j,k;
    for(i=0;i<1000;i++)
        for(j=0;j<1000;j++)
        {
            if(g[i][j].present == 1)
            {
                print(g[i][j]);
                struct element* temp = g[i][j].next;
                
            }
        }

    printf("\n");
}*/
struct element set_elementR(char* yytext)
{
    struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  char temp[20];
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
    }
    else if(i==0)
    {
        a.left.name = n; 
        
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
        

    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if (i==2)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;
        for(j=0;j<(strlen(pch) - 1);j++)
        {
            temp[j] = pch[j];
        }
        a.value = atof(temp);
        a.unit = pch[j];
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 0;
    a.present = 1;
    a.next = NULL;

    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}
    return a;
}
struct element set_elementRR(char* yytext)
{
    struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  char temp[20];
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
    }
    else if(i==0)
    {
        a.left.name = n; 
        
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
        

    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if (i==2)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;
        for(j=0;j<(strlen(pch));j++)
        {
            temp[j] = pch[j];
        }
        a.value = atof(temp);
        a.unit = '.';
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 0;
    a.present = 1;
    a.next = NULL;

    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}
    return a;
}

struct element set_elementRRR(char* yytext)
{
    struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  char temp[20];
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
    }
    else if(i==0)
    {
        a.left.name = n; 
        
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
        

    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if (i==2)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;
        for(j=0;j<(strlen(pch) - 3);j++)
        {
            temp[j] = pch[j];
        }
        a.value = atof(temp);
        a.meg = 1;
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 0;
    a.present = 1;
    a.next = NULL;

    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}
    return a;
}
struct element set_elementLC(char* yytext)
{

  struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  char temp[20];
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
    }
    else if(i==0)
    {
        a.left.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if (i==2)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;
        for(j=0;j<(strlen(pch) - 2);j++)
        {
            temp[j] = pch[j];
        }
        a.value = atof(temp);
        a.unit = pch[j];
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 0;
    a.present = 1;
    a.next = NULL;
    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}
    return a;
}
struct element set_elementIV(char* yytext)
{

  struct element* temp1 = (struct element *) malloc(sizeof(struct element));
  struct element a = *temp1;
  char * pch;
  
  int i=-1;
  
  pch = strtok (yytext," ,-");
  while (pch != NULL)
  {
    int n = getHashIndex(pch);
    if(i== -1)
    {
        a.elem = malloc(strlen(pch)+1); strcpy(a.elem,pch);
    }
    else if(i==0)
    {
        a.left.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }
    else if(i==1)
    {
        a.right.name = n; 
        if(name[n] == NULL)
        {
            name[n] = malloc(strlen(pch)+1); strcpy(name[n],pch);count++;
        }
    }

    else if (i==4)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        for(j=0;j<(strlen(pch) );j++)
        {
            temp[j] = pch[j];
        }
        a.offset = atof(temp);
        
    }
    else if (i==5)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        for(j=0;j<(strlen(pch) );j++)
        {
            temp[j] = pch[j];
        }
        a.amplitude = atof(temp);
        
    }
    else if (i==6)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        for(j=0;j<(strlen(pch) -3 );j++)
        {
            temp[j] = pch[j];
        }
        a.frequency = atof(temp);

        a.unit = pch[j];
        
    }
    else if (i==7)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        
        for(j=0;j<(strlen(pch) -1);j++)
        {
            temp[j] = pch[j];
        }
        
        a.delay = atof(temp);
        
    }
    else if (i==8)
    {   
        
        //printf("%d  ^^^  %s\n",strlen(pch),pch);
        int j;char temp[20];
        if(pch != NULL)
        for(j=0;j<(strlen(pch) );j++)
        {
            temp[j] = pch[j];
        }

        if(temp != NULL)
        a.damping = atof(temp);
        
    }
    i++;
    pch = strtok (NULL, " ,-");
  }
  
    a.b = 1;
    a.present = 1;
  

    a.next = NULL;

    if(flag == 0)
        {starti = a.left.name;startj = a.right.name; flag = 1;}

    return a;
}

