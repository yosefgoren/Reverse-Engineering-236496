	.intel_syntax noprefix

_TGT: 
    push ebp
    mov ebp, esp
    sub esp, 0x10
    mov DWORD PTR [ebp-4], 0
    mov DWORD PTR [ebp-8], 1
L1:
    mov eax, DWORD PTR [ebp+8]
    mov edx,eax
    shr edx,0x1f
    add eax,edx
    sar eax,1
    cmp DWORD PTR [ebp-8],eax
    jge L3
    mov eax, DWORD PTR [ebp+8]
    cdq
    idiv DWORD PTR [ebp-8]
    mov eax, edx
    test eax, eax
    jne L2
    mov eax, DWORD PTR [ebp-8]
    add DWORD PTR [ebp-4], eax
L2:
    add DWORD PTR [ebp-8], 1
    jmp L1
L3:
    mov eax, DWORD PTR [ebp-4]
    cmp eax, DWORD PTR [ebp+8]
    sete al
    movzx eax, al
    leave
    ret


	.def	___main;	.scl	2;	.type	32;	.endef
	.globl	_main
_main:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	and	esp, -16
	sub	esp, 16
	call	___main
	mov	DWORD PTR [esp], 3
	call	_TGT
	mov	eax, 0
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
