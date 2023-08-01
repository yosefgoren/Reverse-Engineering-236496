# Dry 1
## Subsection 1
We can translate `1073741823` to `0x3fffffff`
which can also be seen as `0x40000000-0x1`,
thus the operation can be seen as `(x+0x40000000-0x1)*4`
note how `4` at the start will be overflown, Thus the operation
calculates `(x-1)*4` when `x` is not `0`.

## Subsection 2
Psudo code:
```
unsigned Foo(int n){
    unsigned x = 1; //[ebp-168]
    unsigned y = 1; //[ebp-164]

    unsigned idx = 2; //ecx
    while((int)idx > n){
        Bar(&x, idx);
        idx++;
    }
    return = x[n];
}

```

# Dry 2
## Subsection 1
Hardware breakpoints:
* Hardware dependent
* In x86: Limited to 4 breakpoints, saved in dedicated registers.
* Implemented by comparing the program counter to the saved registers at each cycle.
* Do not require modifying the instructions.

Softawre breakpoints:
* Implemented with a special instruction which causes a self-interrupt coresponding to the breakpoint.
* Requires overriding the target instruction with `0xCC`.
* Requires restoring the overriden instruction to properly execute the code, much like a hook.

### Subsection 2
It copies `ecx` bytes from `esi` to `edi` and sets `eax` to `0`.

### Subsection 3
ssumptions & Hints:
* Shellcode is at `[[ecx]]`
* `GetJITMem(int n)` allocated `rwx` memory of size `n` bytes
* `Gadget 1` copies `ecx` bytes from `esi` to `edi`
```
//now code executed from the copied shellcode in the allocated memory area.
Gadget6:    jmp ebx
            ret

//now shellcode copied into the allocated memory area
Gadget1:    rep movsb
            xor eax, eax
            ret
V:          0x1000
Gadget4:    pop ecx
            ret
Gadget5:    mov edi, eax
            ret

// now ebx points at new memory area
Gadget5:    mov ebx, eax
            ret

// now eax points at new memory area
V:          0x1000
Gadget4:    pop ebx
            ret
Gadget6:    jmp ebx
            ret
Gadget5:    mov ebx, eax
            ret
V:          <addr of func 'GetJITMem'>
Gadget4:    pop eax
            ret

//now esi points at shellcode
Gadget0:    mov esi, [esi]
            ret
Gadget0:    mov esi, [ecx]
            ret
```