#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <WinSock.h>
#pragma comment (lib,"Ws2_32.lib")

using namespace std;

void MAC_print(char* MAC) {
	int i;
	for (i = 0; i < 5; i++) {
		printf( "%02X:", (unsigned char)MAC[i]);
	}
	printf("%02X:",(unsigned char)MAC[i]);
	cout << endl;
}

void IP_print(char* IP) {
	int i;
	for (i = 0; i < 3; i++) {
		printf("%d.", (unsigned char)IP[i]);
	}
	printf("%d", (unsigned char)IP[i]);
	cout << endl;
}



int main() {
	//ifstream in;
	ifstream out;
	bool file_name_true=false;
	string file_name;
	int frame_number=1;
	int filesize = 0;
	FILE* in = NULL;
	char* DATA;
	setlocale(LC_ALL, "Russian");

	////���� ����� �����
	/*while (!file_name_true) {
		cout << "������� ��� ����� ��� ������� ������ ������: ";
		cin >> file_name;

		in.open(file_name, ios_base::in, ios::binary);
		if (!in.is_open()) {
			cout<<endl << "���� �� ��� ������! ���������� ��� ���" << endl;
		}
		else {
			cout << "���� ������� ������! " << endl;
			file_name_true = true;
		}
	}*/
	//in.seekg(0, ios_base::end);
	////��������� ������ �����
	//int file_sz = in.tellg();
	//cout << "������ ����� (� ������): " << file_sz << endl;
	//char* DATA = new char[file_sz];
	//in.seekg(0, ios_base::beg);
	//in.read(DATA, file_sz);
	//cout << DATA << endl;

	////���������� � ����� �����
	//in.seekg(0, ios_base::end);
	////��������� ������ �����
	//cout << "������ ����� (� ������): " << in.tellg() << endl;
	while (!file_name_true) {
		cout << "������� ��� ����� ��� ������� ������ ������: ";
		cin >> file_name;
		in = fopen(file_name.c_str(), "rb");
		if (in != NULL) file_name_true = true;
		else cout << "������ ����� ���!" << endl;
	}
	fseek(in, 0, SEEK_END);
	filesize = ftell(in);
	fseek(in, 0, SEEK_SET);
	DATA = new char[filesize];
	fread(DATA, filesize, 1, in);
	fclose(in);
	cout << "������ �����: " << filesize << " ����" << endl;

	/*vector<char> data;
	if (!in.eof() && !in.fail()) {
		in.seekg(0, ios_base::end);
		cout << "������ �����(� ������): " << in.tellg() << endl;
		streampos FileSize = in.tellg();
		filesize = in.tellg();

		data.resize(FileSize);
		in.seekg(0, ios_base::beg);
		in.read(&data[0], FileSize);
	}*/


	char* p = DATA;
	//char *p = data.data();
	while (p < (DATA+filesize)) {
		cout << "����� �����: " << frame_number << endl;
		cout << "MAC ����� ����������: "<<endl;
		MAC_print(p);
		cout << "MAC ����� �����������: " << endl;
		MAC_print(p + 6);
		unsigned short LT = ntohs(*(unsigned short*)(p + 12));
		if (LT == 0x800) {
			cout << "��� �����: IPv4" << endl;
			cout << "IP ����� �����������: " << endl;
			IP_print(p + 26);
			cout << "IP ����� ����������: " << endl;
			IP_print(p + 30);
			LT = ntohs(*(unsigned short*)(p + 16)) + 14;
			cout << "______ " << endl;
			p += LT;
			frame_number += 1;
		}
		else {
			if (LT <= 0x05FE) {
				cout << "��� �����: Ethernet DIX (Ethernet II)" << endl;
			}
			else {
				if (LT <= 0x05FE) {
					unsigned short F = ntohs(*(unsigned short*)(p + 14));
					if (F == 0xFFFF) {
						cout << "��� �����: Raw 802.3 (Ethernet 802.3)" << endl;
					}
					else {
						if (F == 0xAAAA) {
							cout << "��� �����: Ethernet SNAP" << endl;
						}
						else {
							cout << "��� �����: 802.3/LLC (Ethernet 802.2)" << endl;
						}
					}
				}
			}
			cout << "______" << endl;
			p += LT + 14;
			frame_number += 1;
		}
		//cout <<hex<<LT;
	}
	cout << "����� ������: " << frame_number - 1 << endl;

	


}