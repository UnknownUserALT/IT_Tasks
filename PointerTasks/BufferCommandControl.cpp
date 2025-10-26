#include <iostream>
#include <vector>
#include <string>
#include <cctype>

struct Command {
    std::string name;
    int value;
};

void parseCommand(const char* input, std::vector<Command>& buffer) {
    if (input == nullptr) {
        return;
    }
    
    const char* p = input;
    Command cmd;
    cmd.value = 0;
    
    while (*p == ' ') {
        p++;
    }
    
    const char* nameStart = p;
    while (*p != ' ' && *p != '\0') {
        p++;
    }
    const char* nameEnd = p;

    cmd.name = std::string(nameStart, nameEnd - nameStart);
    
    if (*p == ' ') {
        p++;
        
        while (*p == ' ') {
            p++;
        }
        
        if (*p != '\0') {
            int num = 0;
            bool isNegative = false;
            
            if (*p == '-') {
                isNegative = true;
                p++;
            } else if (*p == '+') {
                p++;
            }
            
            while (*p >= '0' && *p <= '9') {
                num = num * 10 + (*p - '0');
                p++;
            }
            
            cmd.value = isNegative ? -num : num;
        }
    }
    
    buffer.push_back(cmd);
}

int BufferCommand() {
    std::vector<Command> buffer;
    
    parseCommand("FORWARD 10", buffer);
    parseCommand("TURN 90", buffer);
    parseCommand("STOP", buffer);
    parseCommand("BACKWARD -5", buffer);
    parseCommand("   ROTATE   45   ", buffer); // С пробелами
    parseCommand("WAIT", buffer);
    
    std::cout << "Результаты разбора команд:" << std::endl;
    for (const auto& cmd : buffer) {
        std::cout << "Command: " << cmd.name << ", Value: " << cmd.value << std::endl;
    }
    
    return 0;
}