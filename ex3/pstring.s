              .section .rodata

PRINTF_PSTR:  .string  "length: %d, string: %s\n"
INVALID:      .string  "invalid input!\n"

              .text

              .type    pstrlen, @function
              .globl   pstrlen
pstrlen:
              movzbq   (%rdi), %rax
              ret

              .type    replaceChar, @function
              .globl   replaceChar
replaceChar:
              pushq    %rdi # Storing %rdi
.L0:
              incq     %rdi # i++
              cmpq     $0, (%rdi) # str[i] : 0
              jz       .L1        # if str[i] == 0: return str
              movb     (%rdi), %al  # %al = str[i]
              cmpb     (%rdi), %sil # str[i] : old
              cmovz    %rdx, %rax   # if str[i] == old: %al = new
              movb     %al, (%rdi)  # str[i] = %al
              jmp      .L0          # loop
.L1:
              popq     %rax
              ret

              .type    pstrijcpy, @function
              .globl   pstrijcpy
pstrijcpy:
              movq     %rdi, %rbx  # %rbx = dest
              cmpq     $0, %rdx    # i : 0
              jl       .L12        # if i == 0: error
              movq     %rbx, %rdi
              call     pstrlen
              decq     %rax
              cmpq     %rax, %rcx  # j : len(dest) - 1
              ja       .L12        # if j > len(dest) - 1: error
              movq     %rsi, %rdi
              call     pstrlen
              decq     %rax
              cmpq     %rax, %rcx  # j : len(src) - 1
              ja       .L12        # if j > len(src) - 1: error
              cmpq     %rcx, %rdx  # j : i
              ja       .L12        # if i > j: error
.L2:
              cmpq     %rdx, %rcx  # j : i
              jl       .L3         # if i > j: return dest
              movb     1(%rsi, %rdx), %al # %al = src[i + 1]
              movb     %al, 1(%rbx, %rdx) # dest[i + 1] = %al
              incq     %rdx # i++
              jmp      .L2  # loop
.L12:
              movq     $INVALID, %rdi
              xorq     %rax, %rax
              call     printf # print error
.L3:
              movq     %rbx, %rdi # return dest
              ret

              .type    swapCase, @function
              .globl   swapCase
swapCase:
              pushq    %rdi # storing str
.L4:    
              incq     %rdi # str++
              cmpb     $0, (%rdi)
              jz       .L5  # if *str == \0: return str
              cmpb     $0x41, (%rdi)
              jl       .L4  # if *str < 'A': continue
              cmpb     $0x7A, (%rdi)
              jg       .L4  # if *str > 'z': continue
              cmpb     $0x5A, (%rdi)
              jle      .L6  # if *str <= 'Z': *str = lower(*str)
              cmpb     $0x61, (%rdi)
              jge      .L7  # if *str > 'a': *str = upper(*str)
              jmp      .L4
.L6:
              addb     $32, (%rdi) # *str = lower(*str)
              jmp      .L4
.L7:
              subb     $32, (%rdi) # *str = upper(*str)
              jmp      .L4
.L5:
              popq     %rax # return str
              ret
          
              .type    pstrijcmp, @function
              .globl   pstrijcmp
pstrijcmp:
              movq     %rdi, %rbx  # %rbx = dest
              cmpq     $0, %rdx    # i : 0
              jl       .L13        # if i == 0: error
              movq     %rbx, %rdi
              call     pstrlen
              decq     %rax
              cmpq     %rax, %rcx  # j : len(dest) - 1
              ja       .L13        # if j > len(dest) - 1: error
              movq     %rsi, %rdi
              call     pstrlen
              decq     %rax
              cmpq     %rax, %rcx  # j : len(src) - 1
              ja       .L13        # if j > len(src) - 1: error
              cmpq     %rcx, %rdx  # j : i
              ja       .L13        # if i > j: error
.L8:
              cmpq     %rdx, %rcx
              jl       .L9 # if i > j: return 0
              movb     1(%rsi, %rdx), %al # %al = str2[i + 1]
              cmpb     %al, 1(%rbx, %rdx) # str1[i + 1] : %al
              jl       .L10 # if str1[i + 1] < str2[i + 1]: return -1
              jg       .L11 # if str1[i + 1] > str2[i + 1]: return 1
              incq     %rdx # i++
              jmp      .L8  # loop
.L10:
              movq     $-1, %rax # return -1
              ret
.L11:
              movq     $1, %rax  # return1
              ret
.L13:
              movq     $INVALID, %rdi
              xorq     %rax, %rax
              call     printf # print error
              movq     $-2, %rax # return -2
              ret
.L9:
              xorq     %rax, %rax # return 0
              ret

              .type    pstrprint, @function
              .globl   pstrprint
pstrprint:
              pushq    %rbx
              movq     %rdi, %rbx
        
              call     pstrlen
              movq     $PRINTF_PSTR, %rdi
              movq     %rax, %rsi
              movq     %rbx, %rdx
              incq     %rdx
              xorq     %rax, %rax
              call     printf
        
              popq     %rbx
              ret
