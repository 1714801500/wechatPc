#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>

using namespace std;

class MD5 {
private:
	unsigned int tempA, tempB, tempC, tempD, strlength;
public:
	MD5();
	// F����
	unsigned int F(unsigned int b, unsigned int c, unsigned int d);
	// G����
	unsigned int G(unsigned int b, unsigned int c, unsigned int d);
	// H����
	unsigned int H(unsigned int b, unsigned int c, unsigned int d);
	// I����
	unsigned int I(unsigned int b, unsigned int c, unsigned int d);
	// ��λ��������
	unsigned int shift(unsigned int a, unsigned int n);
	// ���뺯��
	string encode(string src);
	// ѭ��ѹ��
	void iterateFunc(unsigned int* X, int size);
	// ����ַ���
	vector<unsigned int> padding(string src);
	// �������
	string format(unsigned int num);
};