#include<iostream>
#include<vector>
#include<cmath>
#include<random>
#include<ctime>
#include<algorithm>
using namespace std;

class Element
{
public:
	Element() {}
	vector<double>x;
	bool isCluster;

};

class APC
{
public:
	APC(int dim, int m, double lambda, int max_its, int con_its)
	{
		this->dim = dim;
		this->m = m;
		this->max_its = max_its;
		this->con_its = con_its;
		element.resize(m);
		S.resize(m);
		R.resize(m);
		A.resize(m);
		vector<double>temp;
		temp.resize(m);
		vector<double>tempElement;
		tempElement.resize(dim);

		for (int i = 0; i < m; i++)
		{

			S[i] = temp;
			A[i] = temp;
			R[i] = temp;
			element[i].x = tempElement;
			element[i].isCluster = false;
			for (int j = 0; j < m; j++)
			{
				A[i][j] = 0;
				R[i][j] = 0;
			}
		}

		randomInitial();
	}

	void randomInitial()
	{
		static default_random_engine engine(time(nullptr));
		static uniform_real_distribution<double> dis(-10, 10);
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < dim; j++)
			{
				element[i].x[j] = dis(engine);
			}
		}
	}

	double EuD(int i,int j)
	{
		
		vector<double> e1 = element[i].x;
		vector<double> e2 = element[j].x;
		double dis = 0;
		for (int num = 0; num < e1.size(); num++)
		{
			dis += pow((e1[num] - e2[num]), 2);
		}

		return sqrt(dis);
	}

	void compute_S()
	{
		vector<double>dis;
		for (int i = 0; i < m - 1; i++)
		{
			for (int j = i + 1; j < m; j++)
			{
				S[i][j] = -EuD(i, j);
				S[j][i] = S[i][j];
				dis.push_back(S[i][j]);
			}
		}
		sort(dis.begin(), dis.end());
		double median = 0;
		if (dis.size() % 2 == 0)
			median = (dis[dis.size() / 2] + dis[dis.size() / 2 - 1]) / 2;
		else
			median = dis[dis.size() / 2];
		for (int i = 0; i < m; i++)
		{
			S[i][i] = median;
		}
	}

	void compute_R()
	{
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < m; j++)
			{
				int max_index = 0;
				do
				{
					max_index++;
				} while (max_index == j);
				for (int num = 0; num < m; num++)
				{
					if (num != j && (A[i][num]+S[i][num])> (A[i][max_index] + S[i][max_index]))
					{
						max_index = num;
					}
				}
				R[i][j] = (1 - lambda)*(S[i][j] - (A[i][max_index] + S[i][max_index])) + lambda * R[i][j];
			}
		}
	}

	void compute_A()
	{
		for (int i = 0; i < m; i++)
		{
			for (int k = 0; k < m; k++)
			{
				double re = 0;
				for (int i_ = 0; i_ < m; i_++)
				{
					if (i_ != i && i_ != k)
					{
						re += max(0.0, R[i_][k]);
					}
				}

				double temp = re + R[k][k];
				A[i][k] = (1 - lambda)*min(0.0, temp) + lambda * A[i][k];
			}
		}
	}

	void print(vector<vector<double>> ele)
	{
		for (int i = 0; i < ele.size(); i++)
		{
			for (int j = 0; j < ele[0].size(); j++)
			{
				cout << ele[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}

	void printCluterCondition()
	{
		vector<int> exampler;
		for (int i = 0; i < m; i++)
		{
			if ((A[i][i] + R[i][i]) > 0)
				exampler.push_back(i);
		}
		vector<double> Isexampler;
		Isexampler.resize(m);

		for (int i = 0; i < m; i++)
		{
			int index = 0;
			for (int j = 0; j < exampler.size(); j++)
			{
				if (S[i][exampler[j]] < S[i][exampler[index]])
				{
					index = j;
				}
			}
			Isexampler[i] = exampler[index];
		}

		for (int i = 0; i < exampler.size(); i++)
			cout << exampler[i] + 1 << " ";
		cout << endl;

		for (int i = 0; i < Isexampler.size(); i++)
			cout << Isexampler[i] + 1 << " ";
		cout << endl;


	}

	void run()
	{
		print(A);
		compute_S();
		print(S);
		for (int i = 0; i < max_its; i++)
		{
			compute_R();
			compute_A();
			
		}
		cout << "finished" << endl;
		printCluterCondition();
	}

	vector<vector<double>>S;
	vector<vector<double>>R;
	vector<vector<double>>A;
	vector<Element>element;
	int dim;
	int m;
	double lambda;
	int max_its;
	int con_its;
};


int main()
{
	int dim = 4;
	int m = 10;
	double lambda = 0.9;
	int max_its = 100;
	int con_its = 30;
	APC apc = APC(dim, m, lambda, max_its, con_its);
	apc.run();
	system("pause");
}