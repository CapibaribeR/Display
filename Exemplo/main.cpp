#include "mbed.h"

// General functions
void setPalette();
void printImage(int type);
const char* intToHex(int value);

// Battery checking
int battLevel = 100;
void printBattRectangle(int type);
void printBattLevel(int percen);
void printBattImage(int type, int percen);

// Speed checking
int speed_km = 0;
void printSpeedRectangle();
void printSpeed(int speed);

// Hexadecimal variables
char c[] = "\x00";
char p[] = "\x25";

// Creating serial
static BufferedSerial DWIN(PA_9, PA_10, 115200);
FileHandle *mbed::mbed_override_console(int fd){
    return &DWIN;
}

int main(){
    setPalette();
    printImage(1);
    while(true){
        printBattRectangle(0);
        printBattRectangle(1);
        printSpeedRectangle();
        printBattLevel(battLevel);
        printBattImage(1, battLevel);
        printBattImage(0, 100);
        printSpeed(speed_km);
        
        battLevel = battLevel - 1;
        speed_km = speed_km + 15;

        if (battLevel < 0){
            battLevel = 100;
        }

        if (speed_km > 999){
            speed_km = 0;
        }

        ThisThread::sleep_for(100ms);
    }
}

void setPalette(){
    printf("\xAA");
    printf("\x40");

    // Foreground color
    printf("%c", c[0]);
    printf("%c", c[0]);

    // Background color
    printf("\x07");
    printf("\xE0");

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");
}

void printImage(int type){
    printf("\xAA");

    if (type == 0){
        printf("\x52");
    } else if (type == 1){
        printf("\x70");
        printf("%c", c[0]);
    }

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");
}

const char* intToHex(int value){
    if (value == 0){
        return "\x30";
    } else if (value == 1){
        return "\x31";
    } else if (value == 2){
        return "\x32";
    } else if (value == 3){
        return "\x33";
    } else if (value == 4){
        return "\x34";
    } else if (value == 5){
        return "\x35";
    } else if (value == 6){
        return "\x36";
    } else if (value == 7){
        return "\x37";
    } else if (value == 8){
        return "\x38";
    } else{
        return "\x39";
    }
}

void printBattRectangle(int type){
    printf("\xAA");

    if (type == 0){
        printf("\x5A");
        printf("%c", c[0]);
        printf("\x17");
        printf("%c", c[0]);
        printf("\x64");
        printf("%c", c[0]);
        printf("\x47");
        printf("%c", c[0]);
        printf("\x80");
    } else if (type == 1){
        printf("\x5B");
        printf("%c", c[0]);
        printf("\x17");
        printf("%c", c[0]);
        printf("\x85");
        printf("%c", c[0]);
        printf("\x64");
        printf("%c", c[0]);
        printf("\xFF");
    }

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");   
}

void printBattLevel(int percen){
    printf("\xAA");
    printf("\x98");
    printf("%c", c[0]);
    printf("\x17");
    printf("%c", c[0]);
    printf("\x64");
    printf("\x1A");
    printf("\x41");
    printf("\x02");
    printf("%c", c[0]);
    printf("%c", c[0]);
    printf("%c", c[0]);
    printf("%c", c[0]);

    int cent = (int)(percen / 100);
    int dez = (int)((percen - cent * 100) / 10);
    int uni = (int)(percen - cent * 100 - dez * 10);

    if (cent != 0){
        printf("%s", intToHex(cent));
    } else{
        printf("\x20");
    }
    
    printf("%s", intToHex(dez));
    printf("%s", intToHex(uni));

    printf("%c", p[0]);
    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");
}

void printBattImage(int type, int percen){
    printf("\xAA");

    if (type == 0){
        printf("\x59");
    } else if(type == 1){
        printf("\x5A");
    }
    
    printf("%c", c[0]);
    printf("\x17");
    printf("%c", c[0]);

    if (percen > 90){
        printf("\x85");
    } else if (percen >= 75){
        printf("\xA4");
    } else if (percen >= 50){
        printf("\xC2");
    } else if (percen >= 25){
        printf("\xE1");
    } else if (percen >= 0){
        printf("\xFF");
    }
    
    printf("%c", c[0]);
    printf("\x64");
    printf("%c", c[0]);
    printf("\xFF");
    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");   
}

void printSpeedRectangle(){
    printf("\xAA");

    printf("\x5A");
    printf("%c", c[0]);
    printf("\x17");
    printf("%c", c[0]);
    printf("\x10");
    printf("\x01");
    printf("\x1F");
    printf("%c", c[0]);
    printf("\x55");

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");   
}


void printSpeed(int speed){
    printf("\xAA");
    printf("\x98");
    printf("%c", c[0]);
    printf("\x17");
    printf("%c", c[0]);
    printf("\x10");
    printf("\x1A");
    printf("\x41");
    printf("\x07");
    printf("%c", c[0]);
    printf("%c", c[0]);
    printf("%c", c[0]);
    printf("%c", c[0]);

    int cent = (int)(speed / 100);
    int dez = (int)((speed - cent * 100) / 10);
    int uni = (int)(speed - cent * 100 - dez * 10);

    if (cent != 0){
        printf("%s", intToHex(cent));
    } else{
        printf("\x20");
    }
    
    printf("%s", intToHex(dez));
    printf("%s", intToHex(uni));

    printf("\x6B");
    printf("\x6D");
    printf("\x2F");
    printf("\x68");
    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");
}