.section    .rodata

format_int:     .string "%d%*c"
format_string:  .string "%[^\n]%*c"

.text

.type run_main, @function
.globl run_main
run_main:
    pushq %rbp
    movq %rsp, %rbp
    subq $528, %rsp

    xorq %rax, %rax
    movq $format_int, %rdi
    leaq -256(%rbp), %rsi
    call scanf

    xorq %rax, %rax
    movq $format_string, %rdi
    leaq -255(%rbp), %rsi
    call scanf

    xorq %rax, %rax
    movq $format_int, %rdi
    leaq -512(%rbp), %rsi
    call scanf

    xorq %rax, %rax
    movq $format_string, %rdi
    leaq -511(%rbp), %rsi
    call scanf

    xorq %rax, %rax
    movq $format_int, %rdi
    leaq -528(%rbp), %rsi
    call scanf

    movq -528(%rbp), %rdi
    leaq -256(%rbp), %rsi
    leaq -512(%rbp), %rdx
    call run_func

    leave
    ret
