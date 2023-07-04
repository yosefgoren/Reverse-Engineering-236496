#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


int evalAddition(const std::string& s) {
    if (s.find("-") != std::string::npos) {
        size_t pos = s.find("-");
        std::string a = s.substr(0, pos);
        std::string b = s.substr(pos + 1);
        return std::stoi(a) - std::stoi(b);
    } else if (s.find("+") != std::string::npos) {
        size_t pos = s.find("+");
        std::string a = s.substr(0, pos);
        std::string b = s.substr(pos + 1);
        return std::stoi(a) + std::stoi(b);
    } else {
        return std::stoi(s);
    }
}


class NewParser {
public:
    static std::pair<std::string, std::string> parseCipher(const std::string& cipher) {
        if (cipher.empty()) {
            return {"", ""};
        }
        if (cipher.size() >= 3 && (cipher[1] == '+' || cipher[1] == '-')) {
            return {cipher.substr(0, 3), cipher.substr(3)};
        } else {
            return {cipher.substr(0, 1), cipher.substr(1)};
        }
    }

    explicit NewParser(const std::string& cipher) : remaining(cipher) {}
    
	std::string next(){
        auto p = parseCipher(remaining);
		auto& res = p.first;
		auto& newRemaining = p.second;

        remaining = newRemaining;
        return res;
    }

    std::string operator*() {
        return next();
	}

private:
    std::string remaining;
};

std::string manualDecrypt(const std::string& s) {
    static const std::unordered_map<std::string, int> tokenLetterRules = {
        {"A", 0x1},
        {"J", 0xA},
        {"Q", 0xB},
        {"K", 0xC}
    };
    std::vector<int> nibbles;
	
	NewParser pa(s);
	while(true){
		const std::string& token = pa.next();
		if(token == ""){
			break;
		}
        if (tokenLetterRules.count(token) > 0) {
            nibbles.push_back(tokenLetterRules.at(token));
        } else {
            if (token.size() == 3) {
                nibbles.push_back(evalAddition(token));
            } else {
                nibbles.push_back(token[0] - '0');
            }
        }
    }
    std::vector<int> byteList;
    for (size_t i = 0; i < nibbles.size(); i += 2) {
        byteList.push_back((nibbles[i] << 4) | nibbles[i+1]);
    }
    std::string result;
    for (int byte : byteList) {
        result += static_cast<char>(byte);
    }
    return result;
}

int main() {
    std::string inputCipher = "-------place holder------";
    std::string decryptedText = manualDecrypt(inputCipher);
    std::cout << decryptedText << std::endl;
    return 0;
}
