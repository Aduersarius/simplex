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
	bool g = 1;
	for (int i = 1; i < 3; i++) {
		if (m[3][i] < 0) {
			g = 0;
			for (int j = 0; j < 3; j++) {
				if (m[j][i] > 0)
					g = 1;
			}
			if (g == 0) return g;
		}
	}
	return g;
}

void print(double mat[4][4]) {
	for (int i = 0; i < 4; i++) {
		cout << endl << " ------------------" << endl;
		for (int j = 0; j < 4; j++) {
			cout << " | " << mat[i][j] ;
		}
		cout << " | ";
	}
	cout << endl << " ------------------" << endl;
}

int razresh_stroka(double m[4][4], int r) { //находим разрешающую строку
	double res = 1000; int k = 0;
	for (int i = 0; i < 3; i++) {
		if (m[i][r] > 0 && m[i][0]/m[i][r] < res) {
			res = m[i][0] / m[i][r];
			k = i;
		}
	}
	return k;
}

int razresh_stolbec(double m[4][4]) {
	int r = 0; double min, max;
	if (m[0][0] < 0) {
		for (int i = 1; i < 3; i++)
			if (m[0][i] < 0) r = i;
	}
	else if (m[1][0] < 0) {
		for (int i = 1; i < 3; i++)
			if (m[1][i] < 0) r = i;
	}
	else if (m[2][0] < 0) {
		for (int i = 1; i < 3; i++)
			if (m[2][i] < 0) r = i;
	}
	else {
		for (int i = 1; i < 4; i++) 
			if (m[3][i] > 0) return i;
		min = *min_element(&m[3][1], &m[3][3] + 1);
		max = *max_element(&m[3][1], &m[3][3] + 1);
		if (fabs(min) > fabs(max))
			r = distance(&m[3][0], min_element(&m[3][1], &m[3][3] + 1));
		else
			r = distance(&m[3][0], max_element(&m[3][1], &m[3][3] + 1));
	}
	return r;
}

void find_the_best(double  A[3][3], double  b[3], double  c[3]) {  // функция ищущая оптимальное значение
	X x1, x2, x3;
	X x[3] = { x1, x2, x3 };
	double m[4][4];
	for (int i = 0; i < 3; i++) {   //  создание симплекс матрицы
		m[i][0] = b[i];
		m[3][0] = 0;
		m[3][i+1] = c[i]*(-1);
		for (int j = 0; j < 3; j++)
			m[i][j+1] = A[i][j];
	}
	double m_[4][4];

	for (;;) {
		print(m);
		if (!(find_if(&m[3][0], &m[3][3], &pred))) {   //  случай если все значения в индексной строке положительны (найдено оптимальное значение)
			cout << "F = " << m[3][0] << endl;
			for (int i = 0; i < 3; i++)
				cout << 'x' << i + 1 << " = " << x[i].value << endl;
			break;
		}
		else if (!(condition(m))) {    // случай если в индексной строке есть хотя бы один отрицательный элемент, в столбце которого нет положительных.
			cout << "целевая функция F неограниченно убывает" << endl;
			break;
		}
		else {    // все другие случаи
			cout <<endl<< "F = " << m[3][0] << endl;
			int r = razresh_stolbec(m); // находим разрешающий столбец
			int k = razresh_stroka(m, r);  // находим разрешающую строку
			cout << endl << "разрешающий элемент = " << m[k][r] << endl;

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m_[i][j] = m[i][j];
				}
			}
			
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (i == k && j == r) m[k][r] = 1 / m_[k][r];
					else if (i == k) m[k][j] = m_[k][j] / m_[k][r];
					else if (j == r) m[i][r] = -m_[i][r] / m_[k][r];
					else m[i][j] = m_[i][j] - m_[i][r] * m_[k][j] / m_[k][r];
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
	}
}

int main()
{
	setlocale(LC_ALL, "");
	double A[3][3]={ {3, 1, 1}, {1, 2, 0}, {0, 0.5, 2} };
	double b[3] = { 3,8,1 };
	double c[3] = { 2,6,7 };

	find_the_best(A, b, c);
    return 0;
}

