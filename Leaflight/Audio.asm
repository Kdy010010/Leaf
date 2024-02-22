section .text
global main

extern printf
extern init_audio
extern play_music
extern pause_music
extern stop_music
extern close_audio

main:
    ; 오디오/스피커 초기화
    call init_audio
    cmp eax, 0
    jne init_fail
    
    ; 음악 재생
    call play_music
    cmp eax, 0
    jne play_fail
    
    ; 일정 시간 대기
    call pause_music
    cmp eax, 0
    jne pause_fail
    
    ; 음악 정지
    call stop_music
    cmp eax, 0
    jne stop_fail
    
    ; 오디오/스피커 종료
    call close_audio
    cmp eax, 0
    jne close_fail
    
    ; 프로그램 종료
    mov eax, 0
    ret
    
init_fail:
    ; 오디오/스피커 초기화 실패 처리
    mov eax, 1
    ret
    
play_fail:
    ; 음악 재생 실패 처리
    mov eax, 2
    ret
    
pause_fail:
    ; 음악 일시정지 실패 처리
    mov eax, 3
    ret
    
stop_fail:
    ; 음악 정지 실패 처리
    mov eax, 4
    ret
    
close_fail:
    ; 오디오/스피커 종료 실패 처리
    mov eax, 5
    ret
