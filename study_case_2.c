#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **fields; // Array untuk menyimpan data setiap kolom
    int fieldCount; // Jumlah kolom
} Row;

Row rowsToExport[10000]; // Menyimpan data yang dipilih untuk diekspor
int rowsToExportCount = 0; // Jumlah baris yang disimpan untuk ekspor

void removeCommas(char *line) {
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ',') {
            line[i] = '|'; // Ganti koma dengan spasi
        }
    }
}

int sortColumnIndex;

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

int compareAscending(const void *a, const void *b) {
    Row *rowA = (Row *)a;
    Row *rowB = (Row *)b;

    return strcmp(rowA->fields[sortColumnIndex], rowB->fields[sortColumnIndex]);
}

// Fungsi pembanding untuk sorting descending
int compareDescending(const void *a, const void *b) {
    Row *rowA = (Row *)a;
    Row *rowB = (Row *)b;

    return strcmp(rowB->fields[sortColumnIndex], rowA->fields[sortColumnIndex]);
}

void display(int rows){
    FILE *file = fopen("soalNde.csv", "r"); // Membuka file CSV
    if (!file) {
        printf("Gagal membuka file: soalNde.csv\n");
        return;
    }

    char line[1024]; 
    int rowCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (rowCount >= rows) {
            break;
        }
        removeCommas(line);
        printf("%s", line); // Cetak baris
        // Simpan data untuk diekspor
        if (rowsToExportCount < 10000) {
            rowsToExport[rowsToExportCount].fields = malloc(1024 * sizeof(char *));
            rowsToExport[rowsToExportCount].fieldCount = 0;

            char *token = strtok(line, ",");
            while (token) {
                rowsToExport[rowsToExportCount].fields[rowsToExport[rowsToExportCount].fieldCount] = strdup(token);
                rowsToExport[rowsToExportCount].fieldCount++;
                token = strtok(NULL, ",");
            }
            rowsToExportCount++;
        }
        rowCount++;
    }

    if (rowCount == 0) {
        printf("File kosong atau tidak ada baris yang bisa ditampilkan.\n");
    }

    fclose(file);
}

void search(const char *columnName, const char *searchValue){
    FILE *file = fopen("soalNde.csv", "r");
    if (!file) {
        printf("Gagal membuka file: soalNde.csv\n");
        return;
    }

    char line[1024];  
    char tempLine[1024]; 
    int rowNumber = 0; 
    int targetColumn = -1;
    
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
        strcpy(tempLine, line); 
        char *token;
        int column = 0; 
        int found = 0;

        token = strtok(tempLine, ",");
        while (token) {
            if (column == targetColumn) {
                if (strstr(token, searchValue)) {
                    found = 1;
                    break;
                }
            }
            token = strtok(NULL, ",");
            column++;
        }

        if (found) {
            printf("Baris %d: %s", rowNumber + 1, line);
            // Simpan data untuk diekspor
            if (rowsToExportCount < 10000) {
                rowsToExport[rowsToExportCount].fields = malloc(1024 * sizeof(char *));
                rowsToExport[rowsToExportCount].fieldCount = 0;

                char *token = strtok(line, ",");
                while (token) {
                    rowsToExport[rowsToExportCount].fields[rowsToExport[rowsToExportCount].fieldCount] = strdup(token);
                    rowsToExport[rowsToExportCount].fieldCount++;  // Tambahkan 1 setelah menambah data
                    token = strtok(NULL, ",");
                }
                rowsToExportCount++;
            }
        }
        rowNumber++;
    }

    fclose(file);
}

void sort(const char *columnName, int isAscending){
    FILE *file = fopen("soalNde.csv", "r");
    if (!file) {
        printf("Gagal membuka file: soalNde.csv\n");
        return;
    }

    char line[1024];
    int rowCount = 0;

    if (!fgets(line, sizeof(line), file)) {
        printf("File kosong atau tidak dapat dibaca.\n");
        fclose(file);
        return;
    }

    int sortColumnIndex = getColumnIndex(line, columnName);
    if (sortColumnIndex == -1) {
        printf("Kolom '%s' tidak ditemukan dalam file.\n", columnName);
        fclose(file);
        return;
    }

    Row rows[10000];
    while (fgets(line, sizeof(line), file)) {
        rows[rowCount].fields = malloc(1024 * sizeof(char *));
        rows[rowCount].fieldCount = 0;

        char *token = strtok(line, ",");
        while (token) {
            rows[rowCount].fields[rows[rowCount].fieldCount] = strdup(token);
            rows[rowCount].fieldCount++;
            token = strtok(NULL, ",");
        }
        rowCount++;
    }
    fclose(file);

    qsort(rows, rowCount, sizeof(Row), isAscending ? compareAscending : compareDescending);

    printf("\n10 Data Teratas setelah Sorting:\n\n");
    for (int i = 0; i < 10 && i < rowCount; i++) {
        for (int j = 0; j < rows[i].fieldCount; j++) {
            printf("%s%s", rows[i].fields[j], (j == rows[i].fieldCount - 1) ? "\n" : ", ");
        }
    }

    // Simpan data untuk ekspor
    rowsToExportCount = rowCount;
    memcpy(rowsToExport, rows, sizeof(Row) * rowCount);
}

void exportToCSV(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Gagal membuka file untuk ekspor: %s\n", filename);
        return;
    }

    for (int i = 0; i < rowsToExportCount; i++) {
        for (int j = 0; j < rowsToExport[i].fieldCount; j++) {
            fprintf(file, "%s", rowsToExport[i].fields[j]);
            if (j < rowsToExport[i].fieldCount - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    printf("Data berhasil diekspor ke %s\n", filename);

    fclose(file);
}

// Menu utama
void menu() {
    int option;
    char columnName[1024], searchValue[1024], filename[1024];

    do {
        printf("\nMenu:\n");
        printf("1. Display\n");
        printf("2. Search\n");
        printf("3. Sort\n");
        printf("4. Export\n");
        printf("5. Exit\n");
        printf("Pilih opsi: ");
        scanf("%d", &option);
        getchar(); // Clear newline character from buffer

        switch (option) {
            case 1:
                printf("Masukkan jumlah baris yang ingin ditampilkan: ");
                int rows;
                scanf("%d", &rows);
                display(rows);
                break;
            case 2:
                printf("Masukkan nama kolom untuk pencarian: ");
                scanf("%s", columnName);
                printf("Masukkan nilai untuk pencarian: ");
                scanf("%s", searchValue);
                search(columnName, searchValue);
                break;
            case 3:
                printf("Masukkan nama kolom untuk sorting: ");
                scanf("%s", columnName);
                printf("Pilih urutan (1: Ascending, 0: Descending): ");
                int order;
                scanf("%d", &order);
                sort(columnName, order);
                break;
            case 4:
                printf("Masukkan nama file untuk ekspor: ");
                scanf("%s", filename);
                exportToCSV(filename);
                break;
            case 5:
                printf("Keluar dari program.\n");
                break;
            default:
                printf("Opsi tidak valid. Silakan coba lagi.\n");
        }
    } while (option != 5);
}

int main() {
    menu();
    return 0;
}
