#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <iterator>
#include <iomanip>
using namespace std;

struct X {    // структура для переменных х1 х2 х3
	double value = 0;
	int basis = 0;
};

int pred(int n) { // предикат для условия
	return n < 0;
}

bool condition(double m[4][4]) {   // условие неограниченного убывания
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

void print(double mat[4][4], string x[7]) {    // функция печати
	cout.width(3);
	cout.precision(3);
	cout.setf(ios::fixed);
	cout << "       Si       " << x[0] << "       " << x[1] << "       " << x[2];
	for (int i = 0; i < 4; i++) {
		cout << endl << "   -------------------------------------" << endl << x[i+3];
		for (int j = 0; j < 4; j++) {
			cout << " | " << setprecision(2)<< setw(6)<< mat[i][j] ;
		}
		cout << " | ";
	}
	cout << endl << "   -------------------------------------" << endl;
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

int razresh_stolbec(double m[4][4]) {    // находим разрешающий столбец
	int r = 0;
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
	else
		for (int i = 1; i < 4; i++)
			if (m[3][i] > 0) return i;
	return r;
}

	void find_the_best(double  A[3][3], double  b[3], double  c[3]) {  // функция ищущая оптимальное значение
		X x1, x2, x3; //инициализация переменных для ответа
		x1.basis = 3;
		x2.basis = 4;
		x3.basis = 5;
		bool res = 1;
		string xh[7] = { "x1", "x2", "x3", "x4", "x5", "x6", " F" }; // инициализация массива для подписей в таблице

		cout << "Каноническая форма задачи:" << endl << 
		
	X x[3] = { x1, x2, x3 };
	double m[4][4];   // инициализация симплекс матрицы

	for (int i = 0; i < 3; i++) {   //  заполнение симплекс матрицы
		m[i][0] = b[i];
		m[3][0] = 0;
		m[3][i+1] = c[i];
		for (int j = 0; j < 3; j++)
			m[i][j+1] = A[i][j];
	}

	double m_[4][4];   // создаем дубликат таблицы для жордановых преобразований

	for (;;) {
		print(m, xh); res = 1;
		for (int i = 0; i < 3; i++)
			if (m[3][i+1] > 0 || m[i][0] < 0) res = 0;   // проверка случая ниже
		
		if (res) { //  случай если все значения в индексной строке положительны (найдено оптимальное значение)
			cout<< "решение найдено" <<endl<< "F = " << m[3][0] << endl;
			for (int i = 0; i < 3; i++)
				cout << 'x' << i + 1 << " = " << x[i].value << endl;
			cout << "проведем проверку подставив значения в изначальное уравнение:" << endl;
			cout <<c[0]<< " * " << x[0].value << " + "<<c[1]<<" * " << x[1].value << " + "<<c[2]<<" * " << x[2].value << " = " << c[0] * x[0].value + c[1] * x[1].value + c[2] * x[2].value<<endl;
			break;
		}

		else if (!(condition(m))) {    // случай если в индексной строке есть хотя бы один отрицательный элемент, в столбце которого нет положительных.
			cout << "целевая функция F неограниченно убывает" << endl;
			break;
		}

		else {    // все другие случаи
			cout << endl << "F = " << m[3][0] << endl;
			int r = razresh_stolbec(m); // находим разрешающий столбец
			int k = razresh_stroka(m, r);  // находим разрешающую строку
			for (int i = 0; i < 3; i++)
				cout << 'x' << i + 1 << " = " << x[i].value << endl;
			cout << endl << "разрешающий элемент = " << m[k][r] << "   " << "m[" << k << "][" << r << ']' << endl << endl;

			for (int i = 0; i < 4; i++) {  // копируем таблицу в дубликат
				for (int j = 0; j < 4; j++) {
					m_[i][j] = m[i][j];
				}
			}

			for (int i = 0; i < 4; i++) {        //  жордановы преобразования с помощью дубликата
				for (int j = 0; j < 4; j++) {
					if (i == k && j == r) m[k][r] = 1 / m_[k][r];
					else if (i == k) m[k][j] = m_[k][j] / m_[k][r];
					else if (j == r) m[i][r] = -m_[i][r] / m_[k][r];
					else m[i][j] = m_[i][j] - (m_[i][r] * m_[k][j] / m_[k][r]);
				}
			}

			swap(xh[r - 1], xh[k + 3]);

			for (int i = 0; i < 3; i++) {          //  пристраиваем значения х1 х2 х3
				if (x[i].basis == r + 2) x[i].basis = k;
				else if (x[i].basis == k) x[i].basis = r + 2;
			}
			for (int i = 0; i < 3; i++) {
			if (x[i].basis < 3) x[i].value = m[x[i].basis][0];
			if (x[i].basis > 2) x[i].value = 0;
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

