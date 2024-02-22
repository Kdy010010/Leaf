#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Leaf 코드를 어셈블리어로 변환하는 함수
char* transpile_leaf_to_asm(const char* leaf_code);

// Leaf 프로젝트 생성하는 함수
char* create_leaflight_project(const char* project_name, const char* leaf_code);

// include 지시문 처리 함수
void process_include_directive(char* asm_code, const char* filename);

// 메모리 추적을 위한 구조체
typedef struct {
    void* address;
    size_t size;
} MemoryAllocation;

// 메모리 추적을 위한 배열
MemoryAllocation* memory_allocations = NULL;
size_t num_allocations = 0;

// 가비지 컬렉션을 위한 함수
void garbage_collect();

// 입력 처리 함수
void process_input();

int main() {
    char project_name[50];
    printf("Enter new Leaflight project name: ");
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

    // Leaf 코드에서 메모리 해제를 위한 가비지 컬렉션
    atexit(garbage_collect);

    char* project_dir = create_leaflight_project(project_name, leaf_code);
    printf("Leaflight project created. Project directory: %s\n", project_dir);

    free(leaf_code);
    free(project_dir);

    return 0;
}

void garbage_collect() {
    // 할당된 모든 메모리를 순회하면서 해제
    for (size_t i = 0; i < num_allocations; ++i) {
        free(memory_allocations[i].address);
    }
    // 메모리 추적을 위한 배열도 해제
    free(memory_allocations);
}

// Leaf 코드에서 malloc()을 호출할 때 마다 호출되는 함수
void* leaf_malloc(size_t size) {
    // 메모리 할당
    void* ptr = malloc(size);
    if (ptr) {
        // 할당된 메모리 추적
        memory_allocations = realloc(memory_allocations, (num_allocations + 1) * sizeof(MemoryAllocation));
        memory_allocations[num_allocations].address = ptr;
        memory_allocations[num_allocations].size = size;
        ++num_allocations;
    }
    return ptr;
}

// Leaf 코드에서 free()를 호출할 때 마다 호출되는 함수
void leaf_free(void* ptr) {
    // 메모리 해제
    free(ptr);
    // 메모리 추적에서 해당 메모리 제거
    for (size_t i = 0; i < num_allocations; ++i) {
        if (memory_allocations[i].address == ptr) {
            // 해당 메모리 위치를 찾았으므로 이를 제거하고 배열을 재조정
            memmove(memory_allocations + i, memory_allocations + i + 1, (num_allocations - i - 1) * sizeof(MemoryAllocation));
            --num_allocations;
            memory_allocations = realloc(memory_allocations, num_allocations * sizeof(MemoryAllocation));
            break;
        }
    }
}

// Leaf 코드를 어셈블리어로 변환하는 함수
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

// Leaf 프로젝트 생성하는 함수
char* create_leaflight_project(const char* project_name, const char* leaf_code) {
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

// include 지시문 처리 함수
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

// 입력 처리 함수
void process_input() {
    // 여기에 입력 처리 로직을 추가합니다.
}
