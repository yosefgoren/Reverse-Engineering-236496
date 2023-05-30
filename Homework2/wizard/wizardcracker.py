# wanted = "DGKPCOEIPCFKNEAMBKLKGFFJOOLK"

# potentials = []
# # fit_list = []
# # for inx in range(32,126):
# #     x=chr(inx)
# #     y = ((ord(wanted[0])-65)^(ord(x)-65))+65
# #     if y<32 or y>126:
# #         continue
# #     y =chr(y)
# #     fit_list.append((x,y))

# # print("fit list:", fit_list)

# for inx in range(32,126):
#     fx = chr(inx)
#     x=fx
#     in_put = [fx]
#     np=0
#     for i in range(len(wanted)-1):
#         y = ((ord(wanted[i])-65)^(ord(x)-65))+65
#         if y<32 or y>126:
#             np =1
#             break
#         y =chr(y)
#         if i==len(wanted)-1 and ((ord(y)-65)^(ord(fx)-65))+65 != ord(wanted[0]):
#             np=1
#             continue
#         in_put.append(y)
#         x=y
#     if np==0:
#         potentials.append(in_put)
#         print("".join(in_put))

def check_str(s):
    ns = []
    for i in range(len(s)):
        if i == len(s)-1:
            ns.append(chr(((ord(s[i])-65)^(ord(s[0])-65))+65))
            continue   
        ns.append(chr(((ord(s[i])-65)^(ord(s[i+1])-65))+65))
    # print("".join(ns))
    return("".join(ns))


# for p in potentials:
#     if check_str(p)[-1] == 'K':
#         print("succses !! ", check_str(p))
def decrypt():
    input_list = ['a'] * 28
    output_str = "DGKPCOEIPCFKNEAMBKLKGFFJOOLK"

    last_value = (ord(output_str[27]) - ord('A')) ^ (ord(output_str[0]) - ord('A'))
    input_list[27] = (chr(last_value + ord('A')))
    for i in reversed(range(1, len(output_str))):
        value = (ord(input_list[i]) - ord('A')) ^ (ord(output_str[i - 1]) - ord('A'))
        input_list[i - 1] = (chr(value + ord('A')))

    input_str = ''.join(input_list)
    return input_str


if __name__ == '__main__':
    print(decrypt())
    print(check_str("ADFPACMIAPNICPLLHGMHNLOLCMCJ"))