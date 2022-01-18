.globl even
.type even, @function
even:
    pushq %rcx                 # saving %rcx

    movq %rdi, %rax            # copying num to result
    movq %rsi, %rcx            # copying i to %rcx
    salq %cl, %rax             # result <<= %cl

    popq %rcx                  # restoring %rcx
    ret                        # return result

.globl go
.type go, @function
go:
    pushq %rbx                 # saving $rbx
    pushq %rcx                 # saving $rcx
    pushq %rdx                 # saving $rdx

    xorq %rbx, %rbx            # sum = 0
    xorq %rcx, %rcx            # i = 0
    movq %rdi, %rdx            # %rdx = A

loop:
    testq $1, (%rdx, %rcx, 4)  # testing whether A[i] % 2 = 0
    jz L1                      # if A[i] is even, adding even(A[i], i) to sum
    jmp L2                     # else, adding A[i] to sum
step:
    incq %rcx                  # i++;
    cmpq $10, %rcx             # comparing i with 10
    js loop                    # continue if i < 10

    movq %rbx, %rax            # result = sum
    popq %rdx                  # restoring %rdx
    popq %rcx                  # restoring %rcx
    popq %rbx                  # restoring %rbx
    ret                        # return result
L1:
    movq (%rdx, %rcx, 4), %rdi # %rdi = A[i]
    movq %rcx, %rsi            # %rsi = i
    call even                  # %rax = even(A[i], i)
    addq %rax, %rbx            # sum += %rax
    jmp step                   # continue
L2:
    addq (%rdx, %rcx, 4), %rbx # sum += A[i]
    jmp step                   # continue
