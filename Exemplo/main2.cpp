#include "mbed.h"

DigitalIn botao(D4);

// Funções gerais
void setPalette();
void printImage(int type);
const char* intToHex(int value);

void printStatusRectangle();
void printStatus(int status);
int st = 0;

// Checagem da bateria
int battLevel = 100;          // Variável para armazenar a carga da bateria
int temperature = 30;
void printBattRectangle(int type);
void printBattLevel(int percen);
void printBattImage(int type, int percen);
void printTempRectangle();
void printTemp(int temp);

// Checagem de velocidade
int speed_km = 0;             // Variável para armazenar a velocidade
void printSpeedRectangle();
void printSpeed(int speed);

// Variáveis que armazenam hexadecimais
char c[] = "\x00";
char p[] = "\x25";

// Criando a serial de comunicação (115200 de baud rate)
static BufferedSerial DWIN(PA_9, PA_10, 115200);
FileHandle *mbed::mbed_override_console(int fd){
    return &DWIN;
}

int main(){
    setPalette();   // Determina a paleta de cores iniciais
    printImage(1);  // Colocar a foto de ID 0 (patinho)
    while(true){
        printBattRectangle(0);  // Retângulo que cobre a imagem da bateria
        printBattRectangle(1);  // Retângulo que cobre o texto da bateria
        printSpeedRectangle();  // Retângulo que cobre o texto da velocidade
        printStatusRectangle();
        printTempRectangle();

        printBattLevel(battLevel);     // Texto da bateria
        printBattImage(1, battLevel);  // Imagem da bateria (nível de carregamento)
        printBattImage(0, 100);        // Contorno da imagem da bateria
        printSpeed(speed_km);   // Texto da velocidade

        
        printStatus(botao.read());
        printTemp(temperature);
        
        battLevel = battLevel - 5;   // Atualiza o valor da carga da bateria
        speed_km = speed_km + 5;    // Atualiza o valor da velocidade

        if (battLevel < 0){          // Reinicia a variável da bateria
            battLevel = 100;
            temperature = temperature - 5;
        }

        if (speed_km > 120){         // Reinicia a variável da bateria
            speed_km = 0;
            temperature = temperature + 5;
        }

        ThisThread::sleep_for(500ms);   // Espera um tempo para recomeçar
    }
}

// Função para determinar a paleta de cor
void setPalette(){
    printf("\xAA");
    printf("\x40");

    // Cor do foreground
    printf("%c", c[0]);
    printf("%c", c[0]);

    // Cor do background
    printf("\x07");
    printf("\xE0");

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");
}

// Função para colocar a imagem de fundo
void printImage(int type){
    printf("\xAA");

    if (type == 0){
        printf("\x52");    // Limpa a imagem
    } else if (type == 1){
        printf("\x70");    // Coloca a imagem de ID 0
        printf("%c", c[0]);
    }

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");
}

// Função para converter inteiro para hexadecimal
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

// Função para colocar um retângulo na tela (bateria)
void printBattRectangle(int type){
    printf("\xAA");

    if (type == 0){         // Retângulo no fundo da imagem da bateria
        printf("\x5A");
        printf("%c", c[0]);
        printf("\x17");
        printf("%c", c[0]);
        printf("\x64");
        printf("%c", c[0]);
        printf("\x47");
        printf("%c", c[0]);
        printf("\x79");
    } else if (type == 1){ // Retângulo no fundo do texto da carga da bateria
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

// Função para colocar o texto da carga da bateria
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

    // Obter a centena, dezena e unidade e converte para inteiro
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

// Função para colocar a imagem da bateria
void printBattImage(int type, int percen){
    printf("\xAA");

    if (type == 0){       // Bordas da imagem
        printf("\x59");
    } else if(type == 1){ // Imagem da bateria em si
        printf("\x5A");
    }
    
    printf("%c", c[0]);
    printf("\x17");
    printf("%c", c[0]);

    // Varia a altura da imagem
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

// Função para colocar um retângulo na tela (velocidade)
void printSpeedRectangle(){
    printf("\xAA");

    printf("\x5A");
    printf("%c", c[0]);
    printf("\x17");
    printf("%c", c[0]);
    printf("\x10");
    printf("%c", c[0]);
    printf("\xF6");
    printf("%c", c[0]);
    printf("\x50");

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");   
}

// Função para colocar o texto da velocidade
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

void printTempRectangle(){
    printf("\xAA");
    printf("\x5A");

    printf("%c", c[0]);
    printf("\x50");
    printf("%c", c[0]);
    printf("\x64");

    printf("%c", c[0]);
    printf("\x80");
    printf("%c", c[0]);
    printf("\x79");

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");  
}

void printTemp(int temp){
    printf("\xAA");
    printf("\x98");
    printf("%c", c[0]);
    printf("\x50");
    printf("%c", c[0]);
    printf("\x64");
    printf("\x1A");
    printf("\x41");
    printf("\x02");
    printf("%c", c[0]);
    printf("%c", c[0]);
    printf("%c", c[0]);
    printf("%c", c[0]);

    // Obter a centena, dezena e unidade e converte para inteiro
    int cent = (int)(temp / 100);
    int dez = (int)((temp - cent * 100) / 10);
    int uni = (int)(temp - cent * 100 - dez * 10);

    if (cent != 0){
        printf("%s", intToHex(cent));
    } else{
        printf("\x20");
    }
    
    printf("%s", intToHex(dez));
    printf("%s", intToHex(uni));

    printf("\x43");
    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");
}

void printStatusRectangle(){
    printf("\xAA");
    printf("\x5A");

    printf("%c", c[0]);
    printf("\xFF");
    printf("%c", c[0]);
    printf("\xAA");

    printf("\x01");
    printf("\x9E");
    printf("%c", c[0]);
    printf("\xE9");

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");  
}

void printStatus(int status){
    printf("\xAA");
    printf("\x98");
    printf("%c", c[0]);
    printf("\xFF");
    printf("%c", c[0]);
    printf("\xAA");
    printf("\x1A");
    printf("\x41");
    printf("\x07");
    printf("%c", c[0]);
    printf("%c", c[0]);
    printf("%c", c[0]);
    printf("%c", c[0]);

    if (status == 0){
        printf("\x4F");
        printf("\x4B");
    } else{
        printf("\x46");
        printf("\x41");
        printf("\x4C");
        printf("\x48");
        printf("\x41");
    }

    printf("\xCC");
    printf("\x33");
    printf("\xC3");
    printf("\x3C");
}
