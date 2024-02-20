#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // mkdir 함수 사용을 위한 헤더

// 어셈블리 코드 변환 및 프로젝트 생성을 위한 함수들
char* transpile_leaf_to_asm(const char* leaf_code);
char* create_leaf_project(const char* project_name, const char* leaf_code);
void process_include_directive(char* asm_code, const char* filename);

int main() {
    char project_name[50];
    printf("Enter new Leaf project name: ");
    scanf("%49s", project_name);

    char leaf_file_path[100];
    printf("Enter Leaf file path: ");
    scanf("%99s", leaf_file_path);

    FILE* leaf_file = fopen(leaf_file_path, "r");
    if (!leaf_file) {
        printf("Cannot open Leaf file.\n");
        return 1;
    }

    fseek(leaf_file, 0, SEEK_END);
    long leaf_file_size = ftell(leaf_file);
    fseek(leaf_file, 0, SEEK_SET);
    char* leaf_code = (char*)malloc(leaf_file_size + 1);
    fread(leaf_code, 1, leaf_file_size, leaf_file);
    leaf_code[leaf_file_size] = '\0';
    fclose(leaf_file);

    char* project_dir = create_leaf_project(project_name, leaf_code);
    printf("Leaf project created. Project directory: %s\n", project_dir);

    free(leaf_code);
    free(project_dir);

    return 0;
}

char* transpile_leaf_to_asm(const char* leaf_code) {
    char* asm_code = (char*)malloc(10000);
    asm_code[0] = '\0';

    char* line = strtok((char*)leaf_code, "\n");
    while (line) {
        if (strncmp(line, "include ", 8) == 0) {
            char* filename = line + 8;
            process_include_directive(asm_code, filename);
        } else {
            // Example of adding mock translation for non-include lines
            strcat(asm_code, "; Mock translation of Leaf code line\n");
        }
        line = strtok(NULL, "\n");
    }

    return asm_code;
}

char* create_leaf_project(const char* project_name, const char* leaf_code) {
    char* project_dir = (char*)malloc(256);
    sprintf(project_dir, "./%s", project_name);
    mkdir(project_dir, 0777);

    char* asm_code = transpile_leaf_to_asm(leaf_code);
    char asm_file_path[256];
    sprintf(asm_file_path, "%s/%s.asm", project_dir, project_name);
    FILE* asm_file = fopen(asm_file_path, "w");
    if (asm_file) {
        fputs(asm_code, asm_file);
        fclose(asm_file);
    } else {
        printf("Failed to write the assembly file.\n");
        free(asm_code);
        free(project_dir);
        exit(1);
    }

    free(asm_code);
    return project_dir;
}

void process_include_directive(char* asm_code, const char* filename) {
    char file_path[256];
    sprintf(file_path, "module/%s", filename);

    FILE* file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open included file '%s'\n", file_path);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* file_contents = (char*)malloc(filesize + 1);
    fread(file_contents, 1, filesize, file);
    file_contents[filesize] = '\0';

    strcat(asm_code, "\n; Begin included ASM file: ");
    strcat(asm_code, filename);
    strcat(asm_code, "\n");
    strcat(asm_code, file_contents);
    strcat(asm_code, "\n; End included ASM file\n");

    fclose(file);
    free(file_contents);
}
