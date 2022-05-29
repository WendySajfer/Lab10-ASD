#include <iostream>
#include "windows.h"
#include <vector>
#include "string"
#include <cmath>

using namespace std;

class Matrix {
private:
	vector<vector<int>> matrix;
	vector<int> width_form;
	int m = 0, n = 0;

	void Format() {
		width_form.clear();
		int width, buf_width, width_null;
		string str_width;
		width = 1;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				str_width = to_string(matrix[j][i]);
				for (int i = str_width.size() - 1; i >= 0; i--) {
					if (str_width[i] == '0' || str_width[i] == ',') {
						str_width.erase(i);
					}
					else break;
				}
				buf_width = str_width.size();
				if (width < buf_width) width = buf_width;
			}
			width_form.push_back(width);
		}
	}
	void Size() {
		m = matrix.size();
		n = matrix[0].size();
		for (int i = 1; i < m; i++) {
			if (matrix[i].size() != n) {
				n = 0;
				break;
			}
		}
	}
public:
	void Input_Matrix(int m, int n)
	{
		double number;
		vector<int> str_matrix;
		cout << "Enter the [" << m << "," << n << "] matrix:" << endl;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				cin >> number;
				str_matrix.push_back(number);
			}
			matrix.push_back(str_matrix);
			str_matrix.clear();
		}
	}
	void Output_Matrix() {
		Size();
		if (n == 0 || m == 0) {
			return;
		}
		Format();
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				cout.width(width_form[j]);
				cout << matrix[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	vector<vector<int>> get_Matrix() {
		return matrix;
	}
	void set_Matrix(vector<vector<int>> matrix_) {
		matrix = matrix_;
	}
	void Matrix_dot_B(vector<vector<int>> matrix_b) {
		m = matrix.size();
		n = matrix_b[0].size();
		int n_ = matrix_b.size();
		if (matrix[0].size() != matrix_b.size()) return;
		vector<vector<int>> dot_matrix;
		for (int i = 0; i < m; i++) {
			vector<int> buf_vector;
			for (int j = 0; j < n; j++) {
				double buf_element = 0;
				for (int k = 0; k < n_; k++) {
					buf_element += matrix[i][k] * matrix_b[k][j];
				}
				buf_vector.push_back(buf_element);
			}
			dot_matrix.push_back(buf_vector);
		}
		matrix = dot_matrix;
	}
};

class Graph {
private:
	vector<vector<int>> matrix_A, matrix_EA, matrix_buf;
	Matrix A;
	Matrix Buf;
	int m;
	bool Flag_Is_Contact = false;
	void Output_A() {
		A.set_Matrix(matrix_A);
		A.Output_Matrix();
	}
	void Output_Buf(vector<vector<int>> matrix_buf) {
		Buf.set_Matrix(matrix_buf);
		Buf.Output_Matrix();
	}
	bool Transpos_A() {
		for (int i = 0; i < m; i++) {
			for (int j = i; j < m; j++) {
				if (matrix_A[j][i] != matrix_A[i][j])
					return false;
			}
		}
		return true;
	}
	vector<vector<int>> Unification(vector<vector<int>> matrix_) {
		int size = matrix_.size();
		int size_ = matrix_[0].size();
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size_; j++) {
				if (matrix_[i][j] != 0 && matrix_[i][j] != 1)
					matrix_[i][j] = 1;
			}
		}
		return matrix_;
	}
	bool Str_NotHave_0(vector<int> buf, int size) {
		for (int i = 0; i < size; i++) {
			if (buf[i] == 0)
				return false;
		}
		return true;
	}
	bool Matrix_Have_Good_Str(vector<vector<int>> matrix_) {
		int size = matrix_.size();
		for (int i = 0; i < size; i++) {
			if (Str_NotHave_0(matrix_[i], size))
				return true;
		}
		return false;
	}
	bool Matrix_not_change(vector<vector<int>> last_matrix, vector<vector<int>> matrix_) {
		int size = matrix_.size();
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
				if (last_matrix[i][j] == 0 && matrix_[i][j] != 0)
					return false;
			}
		}
		return true;
	}
	bool Graph_is_Contact() {
		vector<vector<int>> last_matrix;
		matrix_buf = matrix_EA;
		if (Matrix_Have_Good_Str(matrix_buf))
			return true;
		Buf.set_Matrix(matrix_buf);
		for (int i = 0; i < m - 1; i++) {
			last_matrix = matrix_buf;
			Buf.Matrix_dot_B(matrix_EA);
			matrix_buf = Buf.get_Matrix();
			matrix_buf = Unification(matrix_buf);
			Buf.set_Matrix(matrix_buf);
			if (Matrix_Have_Good_Str(matrix_buf))
				return true;
			if (Matrix_not_change(last_matrix, matrix_buf))
				return false;
		}
		Buf.Matrix_dot_B(matrix_EA);
		matrix_buf = Buf.get_Matrix();
		matrix_buf = Unification(matrix_buf);
		Buf.set_Matrix(matrix_buf);
		if (Matrix_Have_Good_Str(matrix_buf))
			return true;
		else 
			return false;
	}
public:
	bool Input(vector<vector<int>> matrix_a) {
		if (matrix_a.size() != matrix_a[0].size()) {
			return false;
		}
		matrix_A = matrix_a;
		m = matrix_A.size();
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
				if (matrix_A[i][j] < 0)
					return false;
			}
		}
		if (!Transpos_A()) {
			m = 0;
			return false;
		}
		matrix_EA = Unification(matrix_A);
		for (int i = 0; i < m; i++) {
			matrix_EA[i][i] = 1;
		}
		return true;
	}
	void Decision() {
		if (m == 0)
			return;
		Flag_Is_Contact = Graph_is_Contact();
	}
	bool get_Flag_IC() { return Flag_Is_Contact; }
};

class Task {
private:
	Matrix M;
	Graph G;
	int m;
	void Create() {
		M.Input_Matrix(m, m);
	}
	bool Is_Correct_Graph() {
		vector<vector<int>> matrix = M.get_Matrix();
		int buf = 0;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
				buf = matrix[i][j];
				if (i == j) {
					if (buf != 0)
						return false;
				}
				else
					if (buf != 0 && buf != 1)
						return false;
			}
		}
		return true;
	}
public:
	void Task_1() {
		cout << "Input the size of the matrix." << endl << "m = ";
		cin >> m;
		if (m < 2 || m > 100) {
			cout << "Incorrect size of the matrix." << endl;
			return;
		}
		Create();
		if (!Is_Correct_Graph()) {
			cout << "Incorrect matrix data." << endl;
			return;
		}
		if (!G.Input(M.get_Matrix())) {
			cout << "Matrix isn't equal to Transposed." << endl;
			return;
		}
		G.Decision();
		if (G.get_Flag_IC()) {
			cout << "Yes. Graph is connected." << endl;
		}
		else {
			cout << "No. Graph isn't connected." << endl;
		}
	}
};

int main() {
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251);
	int task, exit = 1;
	while (exit == 1) {
		Task T;
		cout << "1.Olympiad" << endl << "2.Exit" << endl << "Choose a way:" << endl;
		cin.clear();
		cin >> task;
		switch (task)
		{
		case 1:
			T.Task_1();
			cin.clear();
			cin.ignore(1000, '\n');
			break;
		case 2:
			exit = 0;
			break;
		default:
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "This task does not exist" << endl;
			break;
		}
	}
	system("pause");
	return 0;
}
/*
y
0 1
1 0

Incorrect
1 1 0 0
1 1 0 0
0 0 1 1
0 0 1 1

N
0 1 0 0
1 0 0 0
0 0 0 1
0 0 1 0

Y
0 1 1 0 0
1 0 1 0 1
1 1 0 1 0
0 0 1 0 0
0 1 0 0 0

*/