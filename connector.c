#include<stdio.h>
#include<string.h>

typedef struct{
    char name[30];
    char password[30];
    char status[30];
    char connected[5];
    int strength;
}Connect;    

void swap(Connect c[10], int j, int k){
    Connect temp;                       
    temp = c[j];
    c[j] = c[k];   
    c[k] = temp;
}

void sortWifi(Connect connect[10], int n){

    int i, j, k, m;
            
    if(n == 10){
        k = 1;
        m = 1;
    }
    else{
        k = 2;
        m = 2;
    }

    for(i=k;i<=8;i++){
        for(j=m;j<=9;j++){

            if(strcmp(connect[j].status,connect[j+1].status) == 1){continue;}
            else if(strcmp(connect[j].status,connect[j+1].status) == -1)
                swap(connect,j,j+1);
            else{ 
                if(connect[j].strength < connect[j+1].strength)
                    swap(connect,j,j+1);    
            }
        }
    }
}

void readFile(Connect connect[10]){
    char name[30], password[30];
    int strength,i=1;
    FILE *f = fopen("samplewifi.txt", "r");       
 
    if(!f){
        printf("cannot open file\n");
        exit(0);
    }

    while(fscanf(f,"%s %s %d", connect[i].name, connect[i].password, &connect[i].strength) != EOF){
        strcpy(connect[i].status , "not saved");
        strcpy(connect[i].connected , "n");
        i++;
    }
         
    fclose(f);
    sortWifi(connect, 10);
}

int checkPassword(char p[1000], char c[40]){ 
    int res;
    res = strcmp(p,c);
    if(res == 0)
        return 1;
    return 0;
}

void displayWifi(Connect connect[10], int n){

    int i;                      
    printf("\n%-4s %-9s %-10s %-10s\n", "Sl.", "Name", "Strength", "Status");
    if(n == 10){
        for(i=1;i<=10;i++){
            printf("%-4d %-9s %-10d %-10s\n", i, connect[i].name, connect[i].strength, connect[i].status);
        }
    }
    else{
        for(i=1;i<=9;i++){
            printf("%-4d %-9s %-10d %-10s\n", i, connect[i+1].name, connect[i+1].strength, connect[i+1].status);
        }
    }
}     

int checkWifiNum(char str[100]){
    if(strlen(str) > 2 || strlen(str) < 1){
        return 0;
    }
    else{   
        int x = atoi(str); 
        if(x > 10 || x < 1)    return 0;
        return 1;
    }    
}

int checkResponse(char response[100]){
    char ch;
    if(strlen(response) != 1 ){
        return 0;
    }
    else{
        ch = response[0];
        if(ch == 'Y' || ch == 'y' || ch == 'N' || ch == 'n')
            return 1;
        
        return 0;
    }
}
              
void startConnector(Connect connect[10]){

    Connect temp;
    int choice;
    char connected = 'n', response[100], str1[100],disconnect;
    char password[100];

    while(1){
        while(connected == 'n'){
            displayWifi(connect,10);
            printf("Enter wifi number to connect: ");
            gets(str1);  
            if(checkWifiNum(str1)){
                choice = atoi(str1);
                if(strcmp(connect[choice].status, "saved") == 0){
                    printf("\n%s connected.", connect[choice].name);
                    strcpy(connect[choice].connected,"y");
                    swap(connect,choice,1);
                    sortWifi(connect, 9); 
                    displayWifi(connect,9);
                    connected = 'y';
                }
                else{
                    printf("Enter %s password: ", connect[choice].name);
                    gets(password);
                    if(checkPassword(password, connect[choice].password)){
                        printf("\n%s connected.", connect[choice].name);
                        strcpy(connect[choice].status , "saved");
                        swap(connect,choice,1);
                        sortWifi(connect, 9);
                        displayWifi(connect,9);
                        strcpy(connect[choice].connected,"y");
                        connected = 'y';
                    }
                    else
                        printf("Incorrect password!\n");
                }
            
            }
            else{
                printf("Invalid input!\n");
            }
        }

        printf("\nDo you want to disonnect (y/n): ");
        
        while(connected == 'y'){
            gets(response);
            if(checkResponse(response)){
                disconnect = response[0];
                if(disconnect == 'y' || disconnect == 'Y'){
                    strcpy(connect[1].connected,"n");
                    connected = 'n';
                    printf("\n%s disconnected\n", connect[1].name);
                    sortWifi(connect,10);
                    break;
                }  
                else{
                    printf("\nDo you want to disonnect (y/n): ");
                }
            }   
            else{
                printf("Invalid input!");
                printf("\nDo you want to disonnect (y/n): ");
            }        
        }
    }    
}

int main(){
    Connect connect[10];
    readFile(connect);
    startConnector(connect);
}
