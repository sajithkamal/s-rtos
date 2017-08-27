
.globl _get_eip
_get_eip:
    mov 0(%esp), %eax
    ret


.globl _get_ebp
_get_ebp:
    mov 0(%ebp), %eax
    ret

