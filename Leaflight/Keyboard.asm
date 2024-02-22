.MODEL SMALL
.STACK 100h

.DATA
    buffer DB 100 DUP(?)
    input_prompt DB 13, 10, 'Enter a string (type "exit" to quit): $'
    exit_string DB 'exit$'
    newline DB 13, 10, '$'
    invalid_input DB 13, 10, 'Invalid input! Try again.$'

.CODE
MAIN PROC
    MOV AX, @DATA
    MOV DS, AX

input_loop:
    ; Display prompt message
    CALL DisplayMessage
    ; Read string from keyboard
    CALL ReadString

    ; Check if entered string is "exit"
    CALL CompareStrings
    JE exit_program  ; Jump if strings are equal

    ; Display entered string
    CALL DisplayMessage
    ; Display newline
    CALL DisplayNewLine

    ; Convert entered string to uppercase
    CALL ConvertToUppercase

    ; Check if entered string is a palindrome
    CALL CheckPalindrome

    ; Count the number of vowels in the entered string
    CALL CountVowels

    ; Reverse the entered string
    CALL ReverseString

    ; Calculate and display the length of the entered string
    CALL CalculateStringLength

    JMP input_loop  ; Repeat input loop

exit_program:
    ; Display newline
    CALL DisplayNewLine
    ; Display exit message
    CALL DisplayMessage

    MOV AH, 4Ch    ; Exit program
    INT 21h

MAIN ENDP

; Function to display a message
DisplayMessage PROC
    MOV AH, 9
    LEA DX, input_prompt
    INT 21h
    RET
DisplayMessage ENDP

; Function to read string from keyboard
ReadString PROC
    MOV AH, 0Ah
    LEA DX, buffer
    INT 21h
    RET
ReadString ENDP

; Function to compare two strings
CompareStrings PROC
    MOV SI, OFFSET buffer + 2 ; Offset 2 skips the length byte and fills byte
    MOV DI, OFFSET exit_string
    MOV CX, 4  ; Compare only 4 characters ("exit" + null terminator)
    REPE CMPSB
    RET
CompareStrings ENDP

; Function to display newline
DisplayNewLine PROC
    MOV AH, 9
    LEA DX, newline
    INT 21h
    RET
DisplayNewLine ENDP

; Function to convert string to uppercase
ConvertToUppercase PROC
    ; Implement uppercase conversion logic here
    RET
ConvertToUppercase ENDP

; Function to check if string is a palindrome
CheckPalindrome PROC
    ; Implement palindrome checking logic here
    RET
CheckPalindrome ENDP

; Function to count vowels in string
CountVowels PROC
    ; Implement vowel counting logic here
    RET
CountVowels ENDP

; Function to reverse a string
ReverseString PROC
    ; Implement string reversal logic here
    RET
ReverseString ENDP

; Function to calculate string length
CalculateStringLength PROC
    ; Implement string length calculation logic here
    RET
CalculateStringLength ENDP

END MAIN
