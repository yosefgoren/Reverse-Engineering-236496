	.intel_syntax noprefix

_TGT: 
	mov edi, arr_0
	mov ecx, n
	mov edx, arr_2
	xor ebx, ebx //C = 0
	
	LOOP:
	mov eax, DWORD PTR [edi] //eax = arr_0[4i]
	mov esi, DWORD PTR [edx] //esi = arr_2[4i]
	add eax, ebx //eax = arr_0[4i]+C
	xor ebx, ebx //C=0
	add eax, esi //eax = arr_0[4i]+C+arr_2[4i]
	adc ebx, ebx //C=CF
	mov DWORD PTR [edi], eax //arr_0[4i]=arr_0[4i]+arr_2[4i]+C
	04 add edi, 0x4 // i++
	04 add edx, 0x4 // i++
	dec ecx
	00 cmp ecx, 0x0
	jg LOOP

	xor eax,eax
	inc eax


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
