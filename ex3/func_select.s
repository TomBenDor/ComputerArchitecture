                .section .rodata

INVALID:        .string "invalid option!\n"
PRINTF_LENGTHS: .string "first pstring length: %hhu, second pstring length: %hhu\n"
PRINTF_REPLACE: .string "old char: %c, new char: %c, first string: %s, second string: %s\n"
PRINTF_COMPARE: .string "compare result: %d\n"
SCANF_CHAR:     .string "%c%*c"
SCANF_INT:      .string "%hhu%*c"

                .align  8
.L0:
                .quad  .L1
                .quad  .L6
                .quad  .L2
                .quad  .L3
                .quad  .L4
                .quad  .L5
                .quad  .L6
                .quad  .L6
                .quad  .L6
                .quad  .L6
                .quad  .L1

                .text

                .type  run_func, @function
                .globl run_func
run_func:
                pushq  %rbp # storing %rbp
                movq   %rsp, %rbp # %rbp = %rsp 
                subq   $32, %rsp  # allocate 32 bytes

                movq   %rsi, -8(%rbp) # save str1 to stack
                movq   %rdx, -16(%rbp) # save str2 to stack
  
                subq   $50, %rdi # option -= 50
                cmpq   $10, %rdi
                ja     .L6       # if option > 10: error
                jmp    *.L0(,%rdi,8) # switch(option)
.L1:             # 50  , 60
                movq   -8(%rbp), %rdi
                call   pstrlen
                pushq  %rax # push len(str1)
                movq   -16(%rbp), %rdi
                call   pstrlen
                pushq  %rax # push len(str2)
                movq   $PRINTF_LENGTHS, %rdi
                popq   %rdx
                popq   %rsi
                xorq   %rax, %rax
                call   printf # print lengths
                jmp    .L7
.L2:             # 52
                movq   $SCANF_CHAR, %rdi
                leaq   -17(%rbp), %rsi
                xorq   %rax, %rax
                call   scanf # getchar(&old)
                movq   $SCANF_CHAR, %rdi
                leaq   -18(%rbp), %rsi
                xorq   %rax, %rax
                call   scanf # getchar(&new)
                movq   -8(%rbp), %rdi
                movq   -17(%rbp), %rsi
                movq   -18(%rbp), %rdx
                call   replaceChar # replaceChar(str1, old, new)
                movq   -16(%rbp), %rdi
                movq   -17(%rbp), %rsi
                movq   -18(%rbp), %rdx
                call   replaceChar # replaceChar(str2, old, new)
                movq   $PRINTF_REPLACE, %rdi
                movzb  -17(%rbp), %rsi
                movzb  -18(%rbp), %rdx
                movq   -8(%rbp), %rcx
                incq   %rcx
                movq   -16(%rbp), %r8
                incq   %r8
                xorq   %rax, %rax
                call   printf # print both strings
                jmp    .L7
.L3:             # 53
                movq   $SCANF_INT, %rdi
                leaq   -17(%rbp), %rsi
                xorq   %rax, %rax
                call   scanf # getint(&i)
                movq   $SCANF_INT, %rdi
                leaq   -18(%rbp), %rsi
                xorq   %rax, %rax
                call   scanf # getint(&j)
                movq   -8(%rbp), %rdi
                movq   -16(%rbp), %rsi
                movzbq -17(%rbp), %rdx
                movzbq -18(%rbp), %rcx
                call   pstrijcpy # pstrijcpy(str1, str2, i, j)
                movq   -8(%rbp), %rdi
                call   pstrprint # print(str1)
                movq   -16(%rbp), %rdi
                call   pstrprint # print(str2)
                jmp    .L7
.L4:             # 54
                movq   -8(%rbp), %rdi
                call   swapCase # swapCase(str1)
                movq   -16(%rbp), %rdi
                call   swapCase # swapCase(str2)
                movq   -8(%rbp), %rdi
                call   pstrprint # print(str1)
                movq   -16(%rbp), %rdi
                call   pstrprint # print(str2)
                jmp    .L7
.L5:             # 55
                movq   $SCANF_INT, %rdi
                leaq   -17(%rbp), %rsi
                xorq   %rax, %rax
                call   scanf # getint(&i)
                movq   $SCANF_INT, %rdi
                leaq   -18(%rbp), %rsi
                xorq   %rax, %rax
                call   scanf # getint(&j)
                movq   -8(%rbp), %rdi
                movq   -16(%rbp), %rsi
                movzbq -17(%rbp), %rdx
                movzbq -18(%rbp), %rcx
                call   pstrijcmp # pstrijcmp(str1, str2, i, j)
                movq   $PRINTF_COMPARE, %rdi
                movq   %rax, %rsi
                call   printf # print result
                jmp    .L7
.L6:            
                movq   $INVALID, %rdi
                xorq   %rax, %rax
                call   printf # print error
.L7:            
                leave
                ret
