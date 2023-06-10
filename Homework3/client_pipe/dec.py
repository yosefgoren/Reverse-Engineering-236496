import json
import os

# ******************************************************************************************
#  *  *  *  *  *  *  *  *  *  *  *  *  Aux  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *
# ******************************************************************************************

def my_serialize(obj):
    if type(obj) is set:
        return [my_serialize(item) for item in obj]
    elif type(obj) is dict:
        return {k: my_serialize(v) for k, v in obj.items()}
    elif type(obj) is list:
        return [my_serialize(item) for item in obj]
    return obj

def all_same(items)->bool:
    if type(items) == set:
        return len(items) <= 1
    else:
        if len(items) == 0:
            return True
        for item in items:
            if item != items[0]:
                return False
        return True

def insert_or_add(s: dict, key, value):
    if key in s:
        s[key].add(value)
    else:
        s[key] = {value}

# ******************************************************************************************
#  *  *  *  *  *  *  *  *  *  *  *  *  Encryption Table  *  *  *  *  *  *  *  *  *  *  *  * 
# ******************************************************************************************

def enc(s: str)->str:
    prog_name = "secure_pipe.exe"
    #append 'a' to 'in.txt':
    with open("in.txt", "w") as f:
        f.write(s)
    #run the program on the input file:
    os.system(f"{prog_name} in.txt > out.txt")
    #read the output file:
    with open("out.txt", "r") as f:
        res = f.read()
    if len(res) > 0 and res[-1] == "\n":
        res = res[:-1]
    return res

def gen_encs(inputs: list)->dict:
    res = dict()
    for input in inputs:
        res[input] = [enc(input)]
    return res

def reset_encs():
    json.dump(dict(), open("encs.json", "w"), indent=4)

def get_enc_table():
    return json.load(open("encs.json", "r"))

def update_encs(table: dict):
    new_table = dict()
    for k, v in get_enc_table().items():
        new_table[k] = set(v)
    for k, v in table.items():
        if k not in new_table:
            new_table[k] = set(v)
        else:
            new_table[k] = new_table[k].union(v)

    json.dump(my_serialize(new_table), open("encs.json", "w"), indent=4)


# ******************************************************************************************
#  *  *  *  *  *  *  *  *  *  *  *  *  Decryption Table  *  *  *  *  *  *  *  *  *  *  *  * 
# ******************************************************************************************

def eval_addition(s: str)->str:
    """
    evaluates a string in the form of 'a', 'a+b' or 'a-b'
    """
    if "-" in s:
        a, b = s.split("-")
        return str(int(a) - int(b))
    elif "+" in s:
        a, b = s.split("+")
        return str(int(a) + int(b))
    else:
        return s

def evaluate_encs(encs: dict)->dict:
    res = dict()
    for k, v in encs.items():
        values = [eval_addition(s) for s in v]
        if not all_same(values):
            raise Exception(f"values for {k} are not all the same: {values}")
        res[k] = values[0]
    return res


def reset_decs():
    json.dump(dict(), open("decs.json", "w"), indent=4)

def update_decs(new_encs: dict):
    new_encs = evaluate_encs(new_encs)
    decs = dict()
    for k, v in new_encs.items():
        insert_or_add(decs, v, k)
    
    for k, values in json.load(open("decs.json", "r")).items():
        for v in values:
            insert_or_add(decs, k, v)
    json.dump(my_serialize(decs), open("decs.json", "w"), indent=4)

def get_dec_table():
    return json.load(open("decs.json", "r"))

def update_decs_from_encs()->dict:
    update_decs(get_enc_table())

class cipherParser():
    def parse_cipher(cipher: str):
        if len(cipher) == 0:
            raise StopIteration
        if len(cipher) == 1:
            raise Exception(f"cipher is not valid: '{cipher}'")
        if len(cipher) >= 3 and (cipher[2] == "+" or cipher[2] == "-"):
            return cipher[:4], cipher[4:]
        else:
            return cipher[:2], cipher[2:]

    def __init__(self, cipher: str):
        self.remaining = cipher

    def __next__(self)->str:
        """
        splits the remaining cipher into (token, rest) where token is the first atom to decode,
            and rest is the remaining cipher.
        Returns 'token'
        """
        res, self.remaining = cipherParser.parse_cipher(self.remaining)
        return res
    
    def __iter__(self):
        return self

def get_token_list(cipher: str)->list:
    return [token for token in cipherParser(cipher)]

def decrypt(cipher: str)->str:
    table = get_dec_table()
    plaintext = ""
    for token in cipherParser(cipher):
        value = eval_addition(token)
        pt_word = table[value][0] if value in table else '?'
        plaintext += pt_word
    return plaintext


def get_overloaded_values()->list:
    table = get_dec_table()
    return [(k, v) for k, v in table.items() if len(v) > 1]

# ******************************************************************************************
#  *  *  *  *  *  *  *  *  *  *  *  *  Experiments  *  *  *  *  *  *  *  *  *  *  *  *  *  *
# ******************************************************************************************


lowercase_letters = [chr(i) for i in range(ord('a'), ord('z')+1)]
uppercase_letters = [chr(i) for i in range(ord('A'), ord('Z')+1)]
letters = lowercase_letters + uppercase_letters
digits = [str(i) for i in range(10)]

ciphers = [
    "4926-67468+768+76Q22-274686522-27268+76262657226-6636A76-67469766528+628-8486523-3697327-768656K6423-36968+622-2433328+6",
    "496626-66668+77229-97368+766+76529-972656A7367+865+925-5776528-8736866+9756K6427-76672656523-374686529-96775792K",
    "68+767+76529-968+575737422-2666968+66426-66A26-66368+7646528-86A737366+963696A74656427-77769746823-374686529-95247+84242455256+9434A56-6545552454422-265766568+67425+9",
    "57686565+924-47468697329-96368+7646522-2697329-9757365642K26-67267+86K6K6968+66728-874686523-36469636527-7736867+8756K6429-9726573756K7422-27769746825-5637562657326-674686A7422-2737568+526-67466+922-27365766567+726+8",
    "4768+7626K6967+7",
]

if __name__ == "__main__":
    # reset_encs()
    # update_encs(gen_encs(letters+digits+[' ']))

    # reset_decs()
    # update_decs_from_encs()
    
    mixed_sets = [items[1] for items in get_overloaded_values()]
    if(len(mixed_sets) > 0):
        print("I some text where some letters were swapped.")
        print("Could you help me un-swap it so it makes sense?")
        first_set = mixed_sets[0]
        print(f"The mixed sets letters are: {mixed_sets}")
        print(f"for example, if there is a mixed set: \"{first_set}\", the letter \"{first_set[0]}\" -it could actually be \"{first_set[1]}\"")
    else:
        print("No overloading...")

    print("The scrambled text is:")
    print("\"")
    for cipher in ciphers:
        print(decrypt(cipher))
    print("\"")
    print("notes: - do not change letters outside the mixed sets")
    print("       - be concise and only output the un-swapped text")