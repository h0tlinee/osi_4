#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment (lib,"Ws2_32.lib")

void MAC_print(FILE* out, unsigned char* MAC);
void IP_print(FILE* out, unsigned char* IP);

int main() {
    FILE* in = NULL;
    FILE* out = NULL;
    bool enterfile = false;
    char filename[100];
    int filesize = 0;
    int framenumber = 1;
    unsigned char* DATA;
    int types[10] = { 0 };

    while (!enterfile) {
        printf("Enter the file name: ");
        scanf_s("%s", filename, sizeof(filename));
        fopen_s(&in, filename, "rb");
        if (in != NULL) enterfile = true;
        else printf("This does not exist! Try again.\n\n");
    }

    fopen_s(&out, "out.txt", "w");
    fseek(in, 0, SEEK_END);
    filesize = ftell(in);
    fseek(in, 0, SEEK_SET);
    DATA = new unsigned char[filesize];
    fread(DATA, filesize, 1, in);
    fclose(in);

    fprintf(out, "size of file: %d bytes\n", filesize);
    fprintf(out, "\n----------\n");
    unsigned char* p = DATA;

    while (p < DATA + filesize) {

        fprintf(out, "frame number: %d\n", framenumber);
        fprintf(out, "MAC dest: ");
        MAC_print(out, p);
        fprintf(out, "MAC source: ");
        MAC_print(out, p + 6);
        unsigned short LT = ntohs(*(unsigned short*)(p + 12));

        if (LT == 0x800) {
            fprintf(out, "frame type: IPv4\n");
            fprintf(out, "IP source: ");
            IP_print(out, p + 26);
            fprintf(out, "IP dest: ");
            IP_print(out, p + 30);
            LT = ntohs(*(unsigned short*)(p + 16)) + 14;
            fprintf(out, "frame size: %d", LT);
            fprintf(out, "\n----------\n");
            p += LT;
            framenumber++;
            types[0] += 1;
        }
        else {
            if (LT > 0x05FE) {
                fprintf(out, "frame type: Ethernet DIX (Ethernet II)\n");
                //fprintf(out, "frame size: %d", 14);
                types[1] += 1;
            }
            
            else if (LT <= 0x05FE) {
                unsigned short F = ntohs(*(unsigned short*)(p + 14));
                if (F == 0xFFFF) {
                    fprintf(out, "frame type: Raw 802.3 (Ethernet 802.3)\n");
                    fprintf(out, "frame size: %d", LT+14);
                    types[2] += 1;
                }
                else if (F == 0xAAAA) {
                    fprintf(out, "frame type: Ethernet SNAP\n");
                    fprintf(out, "frame size: %d", LT+14);
                    types[3] += 1;
                }
                else {
                    fprintf(out, "frame type: 802.3/LLC (Ethernet 802.2)\n");
                    fprintf(out, "frame size: %d", LT+14);
                    types[4] += 1;
                }
            }
            fprintf(out, "\n----------\n");
            p += LT + 14;
            framenumber++;
        }
    }

    fprintf(out, "total number of frames: %d", framenumber - 1);
    fprintf(out, "\nIPv4 frames: %d", types[0]);
    fprintf(out, "\nEthernet DIX (Ethernet II) frames: %d", types[1]);
    fprintf(out, "\nRaw 802.3 (Ethernet 802.3) frames: %d", types[2]);
    fprintf(out, "\nEthernet SNAP frames: %d", types[3]);
    fprintf(out, "\n802.3/LLC (Ethernet 802.2) frames: %d", types[4]);
    fclose(out);

    delete[] DATA; // Освобождаем память, выделенную под DATA

    printf("\nDone!");
    system("pause");

    return 0;
}

void MAC_print(FILE* out, unsigned char* MAC) {
    for (int i = 0; i < 5; i++)
        fprintf(out, "%02X:", MAC[i]);
    fprintf(out, "%02X\n", MAC[5]);
}

void IP_print(FILE* out, unsigned char* IP) {
    for (int i = 0; i < 3; i++)
        fprintf(out, "%d.", IP[i]);
    fprintf(out, "%d\n", IP[3]);
}


	


