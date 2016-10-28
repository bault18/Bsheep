#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

class point {
public:
	int x, y, num;

	point() {}
	point(int a, int b, int n)
	{
		x = a;
		y = b;
		num = n;
	}

	bool operator == (point a)
	{
		if (a.x == x && a.y == y)
			return true;
		return false;
	}
};


vector<point> upper_hull(point p1, point p2, vector<point> points)
{
	point maxpt;
	double h = 0;
	for (int i = 0; i < points.size(); i++)
	{
		point p3 = points.at(i);

		double currh = ((p1.x * (p2.y - p3.y)) + (p2.x * (p3.y - p1.y)) + (p3.x * (p1.y - p2.y)));
		if (currh > h)
		{
			maxpt = p3;
			h = currh;
		}
	}
	
	//end case
	if (h <= 0)
	{
		vector<point> fin;
		//put the items back in in counter clockwise order
		fin.push_back(p2);
		//fin.push_back(p1);
		return fin;
	}

	//left half
	vector<point> left_half = upper_hull(p1, maxpt, points);
	//right half
	vector<point> right_half = upper_hull(maxpt, p2, points);

	//add items into hull
	for (int i = 0; i < left_half.size(); i++)
		right_half.push_back(left_half[i]);

	right_half.push_back(p1);

	return right_half;
}

vector<point> lower_hull(point p1, point p2, vector<point> points)
{
	point maxpt;
	double h = 0;
	int loc;
	for (int i = 0; i < points.size(); i++)
	{
		point p3 = points.at(i);

		double currh = ((p1.x * (p2.y - p3.y)) + (p2.x * (p3.y - p1.y)) + (p3.x * (p1.y - p2.y)));

		if (currh < h)
		{
			maxpt = p3;
			h = currh;
			loc = i;
		}
	}

	if (h >= 0)
	{
		vector<point> fin;
		fin.push_back(p1);
		return fin;
	}

	points.erase(points.begin() + loc);

	//left half
	vector<point> left_half = lower_hull(p1, maxpt, points);
	//right half
	vector<point> right_half = lower_hull(maxpt, p2, points);

	//create the whole hull
	for (int i = 0; i < right_half.size(); i++)
	{
		left_half.push_back(right_half[i]);
	}

	left_half.push_back(p2);


	return left_half;
}

int main()
{
	int cases;
	cin >> cases;

	for (int z = 0; z < cases; z++)
	{
		//~~~~~~~~~~~~~~~~~~~Inputs~~~~~~~~~~~~~
		int num_input;
		cin >> num_input;

		vector<point> points;
		for (int j = 0; j < num_input; j++)
		{
			int x, y;
			cin >> x >> y;
			point temp(x, y, j+1);
			points.push_back(temp);
			
		}

		if (num_input == 0)
		{
			cout << "0.00" << endl << endl;
			continue;
		}
		else if (num_input == 1)
		{
			cout << "0.00" << endl;
			cout << "1" << endl << endl;
			continue;
		}


		//Remove duplicates (THIS IS BAD)
		for (int j = 0; j < points.size(); j++)
		{
			for (int k = j + 1; k < points.size(); k++)
			{
				if (points[j] == points[k])
				{
					points.erase(points.begin() + k);
					k--;
				}
			}
		}
		

		//~~~~~~~~~~~Finding Edge Points~~~~~~~~~~~

		point left_most(10000000, 1000000, 0);
		point right_most(-10000000, 0, 0);

		//Find left and rightmost points
		for (int i = 0; i < points.size(); i++)
		{
			point temp = points[i];

			//Finds rightmost item
			if (temp.x > right_most.x)
				right_most = temp;
			//Finds highest rightmost item
			else if (temp.x == right_most.x && temp.y > right_most.y)
				right_most = temp;
			//Finds lowest item
			if (temp.y < left_most.y)
				left_most = temp;
			//Finds leftmost lowest item
			else if (temp.y == left_most.y && temp.x < left_most.x)
				left_most = temp;
		}

		//~~~~~~~~~~Run Quickhull~~~~~~~~~~

		//note: upperhull has already been sorted into counter clockwise order
		vector<point> upperhull = upper_hull(left_most, right_most, points);
		vector<point> lowerhull = lower_hull(left_most, right_most, points);

		vector<point> completehull = lowerhull;
		for (int i = 0; i < upperhull.size(); i++)
		{
			completehull.push_back(upperhull.at(i));
		}

		if (completehull.size() == 2)
			completehull.push_back(left_most);

		/* Remove Duplicates
		* -This is important if it is in a line
		*/
		for (int i = 0; i < completehull.size() - 2; i++)
		{
			for (int j = i + 1; j < completehull.size() - 1; j++)
				if (completehull[i] == completehull[j])
					completehull.erase(completehull.begin() + j);
		}

		//Calculate circumference
		double circ = 0;
		for (int i = 0; i < completehull.size() - 1; i++)
		{
			point p1 = completehull[i];
			point p2 = completehull[i + 1];

			circ += sqrt(abs(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2)));
		}

		cout << fixed << setprecision(2) << circ << endl;

		//display order
		for (int i = 0; i < completehull.size() - 1; i++)
		{
			cout << completehull[i].num << " ";
		}
		cout << endl << endl;
	}
}