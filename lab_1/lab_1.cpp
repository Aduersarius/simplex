#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
using namespace std;

struct X {
	double value = 0;
	bool basis = 0;
};

int pred(int n) { //предикат для условия
	return n < 0;
}

bool condition(double m[4][4]) {   //условие неограниченного убывания
	bool g = 0;
	for (int i = 0; i < 3; i++) {
		if (m[3][i] < 0) {
			for (int j = 0; j < 3; j++) {
				if (m[j][i] > 0)
					g = 1;
			}
		}
	}
	return g;
}

void print(double mat[4][4]) {
	for (int i = 0; i < 4; i++) {
		cout << endl << "---------------" << endl;
		for (int j = 0; j < 4; j++) {
			cout << mat[i][j] << " | ";
		}
	}
}

int razresh_stroka(double m[4][4], int r) { //находим разрешающую строку
	double res = 0; int k = 0;
	for (int i = 0; i < 2; i++) {
		if (m[i][r] > 0 && m[i][0]/m[i][r] > res) {
			res = m[i][0] / m[i][r];
			k = i;
		}
	}
	return k;
}

inline void jordan(double m[4][4], int r, int k) {  // жордановы преобразования
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == k && j == r) m[k][r] = 1 / m[k][r];
			else if (i == k) m[k][j] /= m[k][r];
			else if (j == r) m[i][r] /= -m[k][r];
			else m[i][j] -= m[i][r] * m[k][j] / m[k][r];
		}
	}
}

void find_the_best(double  A[3][3], double  b[3], double  c[3]) {  // функция ищущая оптимальное значение
	X x1, x2, x3;
	X x[3] = { x1, x2, x3 };
	double m[4][4];
	for (int i = 0; i < 3; i++) {
		m[i][0] = b[i];
		m[3][0] = 0;
		m[3][i+1] = c[i]*(-1);
		for (int j = 0; j < 3; j++)
			m[i][j+1] = A[i][j];
	}
	int count = 0;
	print(m);
	for (;;) {
		if (!(find_if(&m[3][0], &m[3][3], &pred))) {   //  случай если все значения в индексной строке положительны (найдено оптимальное значение)
			cout << "F = " << m[3][0] << endl;
			for (int i = 0; i < 3; i++)
				cout << 'x' << i + 1 << " = " << x[i].value << endl;
			break;
		}
		else if (!(condition(m))) {    // случай если в индексной строке есть хотя бы один отрицательный элемент, в столбце которого нет положительных.
			cout << "целевая функция F→∞ неограниченно убывает" << endl;
			break;
		}
		else {    // все другие случаи
			int r = distance(&b[0], min_element(&m[3][0], &m[3][3])); // находим разрешающий столбец
			int k = razresh_stroka(m, r);  // находим разрешающую строку

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (i == k && j == r) m[k][r] = 1 / m[k][r];
					else if (i == k) m[k][j] /= m[k][r];
					else if (j == r) m[i][r] /= -m[k][r];
					else m[i][j] -= m[i][r] * m[k][j] / m[k][r];
				}
			}

			if (x[r].basis == 0) {
				x[r].value = m[k][0];
				x[r].basis = true;
			}
			else {
				x[r].value = 0;
				x[r].basis = false;
			}
		}
		print(m);
		
		cout << ++count << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "");
	double A[3][3]={ {3, 1, 1}, {1, 2, 0}, {0, 0.5, 2} };
	double b[3] = { 3, 8, 1 };
	double c[3] = { 2, 6, 7 };
	//sort(&b[0], &b[0] + 3);
	//cout << distance(&b[0], min_element(&b[0], &b[0] + 3));
	//for (int i = 0; i < 3; i++)
	//	cout << b[i];
	//for (int i = 0; i<4; i++){
	//vector<double> d(A[1][i], A[1][i] +3);}
	//distance(d.begin(), min_element(d.begin(), d.end()));
	find_the_best(A, b, c);
    return 0;
}

