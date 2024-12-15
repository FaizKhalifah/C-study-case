#include <stdio.h>
#include <string.h>
#include <ctype.h>

void reverse(char kata[]){
    int length = strlen(kata);
    int start = 0;
    int end = length-1;
    char temp;
    while (start<end)
    {
        temp = kata[start];
        kata[start]=kata[end];
        kata[end]=temp;
        printf("%d \n", start);
        printf("%d \n", end);
        start++;
        end--;
    }
    
}

void inversCaps(char kata[]){
    for(int i=0;i<strlen(kata);i++){
        if(islower(kata[i])){
            kata[i] = toupper(kata[i]);
        }else if(isupper(kata[i])){
            kata[i]=tolower(kata[i]);
        }
    }
}

int main() {
    char nama[]= "Faiz Khalifah";
    printf("%s \n", nama);
    reverse(nama);
    printf("%s \n", nama);
    inversCaps(nama);
    printf("%s \n", nama);
}
