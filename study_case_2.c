#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void removeCommas(char *line) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ',') {
            line[i] = '|'; // Ganti koma dengan spasi
        }
    }
}

void display(int rows){
    FILE *file = fopen("soalNde.csv", "r"); // Membuka file CSV
    if (!file) {
        printf("Gagal membuka file: %s\n");
        return;
    }

    char line[1024]; 
    int rowCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (rowCount >= rows) {
            break;
        }
        //printf("%s", strtok(line, " , "));
        removeCommas(line);
        printf("%s", line); // Cetak baris
        rowCount++;
    }

    if (rowCount == 0) {
        printf("File kosong atau tidak ada baris yang bisa ditampilkan.\n");
    }

    fclose(file);
}

void search(const char *columnName, const char *searchValue){
    FILE *file = fopen("soalNde.csv", "r"); // Membuka file CSV
    if (!file) {
        printf("Gagal membuka file: %s\n");
        return;
    }

    char line[1024];  // Buffer untuk membaca baris
    char tempLine[1024]; // Buffer sementara untuk manipulasi
    int rowNumber = 0; // Untuk menghitung baris
    int targetColumn = -1; // Indeks kolom target
    if (fgets(line, sizeof(line), file)) {
        targetColumn = getColumnIndex(line, columnName);
        if (targetColumn == -1) {
            printf("Kolom '%s' tidak ditemukan dalam file.\n", columnName);
            fclose(file);
            return;
        }
    }

    printf("\nHasil pencarian di kolom '%s' untuk nilai '%s':\n\n", columnName, searchValue);

    while (fgets(line, sizeof(line), file)) {
        strcpy(tempLine, line); // Salin baris ke buffer sementara
        char *token;
        int column = 0; // Hitungan kolom mulai dari 0
        int found = 0; // Flag jika nilai ditemukan di kolom target

        // Tokenisasi baris berdasarkan koma
        token = strtok(tempLine, ",");
        while (token) {
            if (column == targetColumn) {
                // Bandingkan nilai token dengan nilai pencarian
                if (strstr(token, searchValue)) {
                    found = 1;
                    break;
                }
            }
            token = strtok(NULL, ",");
            column++;
        }

        if (found) {
            printf("Baris %d: %s", rowNumber + 1, line); // Tampilkan baris asli
        }
        rowNumber++;
    }

    fclose(file);
}

// Fungsi untuk menemukan indeks kolom berdasarkan nama kolom
int getColumnIndex(const char *header, const char *columnName) {
    char tempHeader[1024];
    strcpy(tempHeader, header); // Salin header untuk diubah
    char *token;
    int columnIndex = 0;

    token = strtok(tempHeader, ",");
    while (token) {
        if (strcmp(token, columnName) == 0) {
            return columnIndex; // Indeks ditemukan
        }
        token = strtok(NULL, ",");
        columnIndex++;
    }

    return -1; // Jika nama kolom tidak ditemukan
}


void sort(){

}

void export(){

}


int main()
{
    display(7);
    search("Location 1", "Cheras");
    return 0;
}
