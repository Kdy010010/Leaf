#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> // for directory handling

// Leaf 언어 트랜스파일러
char* transpile_leaf_to_asm(const char* leaf_code);

// Leaf 프로젝트 생성 및 Leaf 코드 컴파일
char* create_leaf_project(const char* project_name, const char* leaf_code);

// Resolve import statement and include imported code
char* resolve_and_include_import(const char* import_statement);

int main() {
    // Leaf 프로젝트 이름을 입력받음
    char project_name[50];
    printf("새 Leaf 프로젝트 이름을 입력하세요: ");
    scanf("%s", project_name);

    // Leaf 파일 읽기
    char leaf_file_path[100];
    printf("Leaf 파일 경로를 입력하세요: ");
    scanf("%s", leaf_file_path);

    // Leaf 파일 열기
    FILE* leaf_file = fopen(leaf_file_path, "r");
    if (leaf_file == NULL) {
        printf("Leaf 파일을 열 수 없습니다.\n");
        return 1;
    }

    // Leaf 코드 읽기
    fseek(leaf_file, 0, SEEK_END);
    long leaf_file_size = ftell(leaf_file);
    fseek(leaf_file, 0, SEEK_SET);
    char* leaf_code = (char*)malloc((leaf_file_size + 1) * sizeof(char));
    fread(leaf_code, sizeof(char), leaf_file_size, leaf_file);
    leaf_code[leaf_file_size] = '\0'; // Null-terminate the string
    fclose(leaf_file);

    // Leaf 프로젝트 생성 및 컴파일
    char* project_dir = create_leaf_project(project_name, leaf_code);
    printf("Leaf 프로젝트가 생성되었습니다. 프로젝트 디렉토리: %s\n", project_dir);

    // Free allocated memory
    free(leaf_code);
    free(project_dir);

    return 0;
}

char* transpile_leaf_to_asm(const char* leaf_code) {
    char* asm_code = (char*)malloc(1000 * sizeof(char)); // Allocate memory for assembly code
    asm_code[0] = '\0'; // Initialize as an empty string

    // 변수명에 대한 카운터 초기화
    int var_count = 0;

    // Leaf 코드를 줄 단위로 분할
    char* line;
    line = strtok((char*)leaf_code, "\n");

    // 각 줄에 대해 반복
    while (line != NULL) {
        // import 문장인지 확인
        if (strncmp(line, "import ", 7) == 0) {
            // Resolve and include imported code
            char* imported_code = resolve_and_include_import(line + 7);
            strcat(asm_code, imported_code);
            free(imported_code);
        } else {
            // 나머지 코드 변환 처리...
        }

        var_count++;
        line = strtok(NULL, "\n");
    }

    return asm_code;
}

char* resolve_and_include_import(const char* import_statement) {
    // Extract module name from import statement
    char* module_name = strtok(import_statement, " ;");

    // Resolve module name to file path (e.g., search directories, etc.)
    // For demonstration purposes, let's assume modules are stored in a "modules" directory
    char module_path[100];
    sprintf(module_path, "modules/%s.leaf", module_name);

    // Read the content of the module file
    FILE* module_file = fopen(module_path, "r");
    if (module_file == NULL) {
        printf("Error: Failed to open module file '%s'\n", module_path);
        exit(1);
    }

    // Read the content of the module file
    fseek(module_file, 0, SEEK_END);
    long module_file_size = ftell(module_file);
    fseek(module_file, 0, SEEK_SET);
    char* module_code = (char*)malloc((module_file_size + 1) * sizeof(char));
    fread(module_code, sizeof(char), module_file_size, module_file);
    module_code[module_file_size] = '\0'; // Null-terminate the string
    fclose(module_file);

    return module_code;
}

char* create_leaf_project(const char* project_name, const char* leaf_code) {
    // Leaf 프로젝트 디렉토리 생성
    char* project_dir = (char*)malloc(100 * sizeof(char)); // Allocate memory for project directory path
    sprintf(project_dir, "%s", project_name); // Copy project name as directory path
    mkdir(project_dir, 0777); // Create directory

    // Leaf 코드를 어셈블리로 변환
    char* asm_code = transpile_leaf_to_asm(leaf_code);

    // 어셈블리 코드를 Leaf 프로젝트 디렉토리에 저장
    char asm_file_path[100];
    sprintf(asm_file_path, "%s/%s.asm", project_dir, project_name);
    FILE* asm_file = fopen(asm_file_path, "w");
    fprintf(asm_file, "%s", asm_code);
    fclose(asm_file);

    return project_dir;
}
