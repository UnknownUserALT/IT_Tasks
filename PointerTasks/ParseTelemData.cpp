#include <iostream>

void parseTelemetry(const char* data) {
    const char* p = data;

    while (*p != '\0') {
        while (*p == ' ') p++;

        const char* nameStart = p;
        while (*p != ':' && *p != '\0') p++;

        if (*p == '\0') break;

        const char* nameEnd = p;
        p++; // Пропускаем ':'

        const char* valueStart = p;
        while (*p != ';' && *p != '\0') p++;

        const char* valueEnd = p;

        int nameLen = nameEnd - nameStart;
        if (nameLen == 4 && nameStart[0] == 'T' && nameStart[1] == 'E' &&
            nameStart[2] == 'M' && nameStart[3] == 'P') {
            std::cout << "Temperature: ";
            } else if (nameLen == 5 && nameStart[0] == 'P' && nameStart[1] == 'R' &&
                       nameStart[2] == 'E' && nameStart[3] == 'S' && nameStart[4] == 'S') {
                std::cout << "Pressure: ";
                       } else if (nameLen == 3 && nameStart[0] == 'H' && nameStart[1] == 'U' &&
                                  nameStart[2] == 'M') {
                           std::cout << "Humidity: ";
                                  } else {
                                      // Выводим неизвестное имя как есть
                                      for (const char* n = nameStart; n < nameEnd; n++) {
                                          std::cout << *n;
                                      }
                                      std::cout << ": ";
                                  }

        // Выводим значение
        for (const char* v = valueStart; v < valueEnd; v++) {
            std::cout << *v;
        }
        std::cout << std::endl;

        // Пропускаем ';'
        if (*p == ';') p++;
    }
}

int ParseTelemData() {
    const char* data = "TEMP:24.5;PRESS:101.3;HUM:55.2";
    parseTelemetry(data);

    return 0;
}