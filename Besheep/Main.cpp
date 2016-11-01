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
};

//used to sort points
//chooses lowest y, then lowest x, then lowest input point
bool srt(point p1, point p2)
{
	//see which has lesser y coord
	if (p1.y < p2.y)
		return true;
	if (p1.y > p2.y)
		return false;
	//see which has lesser x coord
	if (p1.x < p2.x)
		return true;
	if (p1.x > p2.x)
		return false;
	//see which has lesser id
	if (p1.num < p2.num)
		return true;
	if (p1.num > p2.num)
		return false;
	return false;
}

//checks if there are dupicate points in the input
bool unique_pts(point p1, point p2)
{
	if (p1.x == p2.x && p1.y == p2.y)
		return true;
	return false;
}

vector<point> upper_hull(point p1, point p2, vector<point> points)
{
	point maxpt;
	double h = 0;
	vector<point> newpts;
	for (int i = 0; i < points.size(); i++)
	{
		point p3 = points.at(i);

		double currh = ((p1.x * (p2.y - p3.y)) + (p2.x * (p3.y - p1.y)) + (p3.x * (p1.y - p2.y)));
		if (currh > h)
		{
			maxpt = p3;
			h = currh;
		}
		if (currh > 0)
			newpts.push_back(p3);
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
	vector<point> left_half = upper_hull(p1, maxpt, newpts);
	//right half
	vector<point> right_half = upper_hull(maxpt, p2, newpts);

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
	vector<point> newpts;
	for (int i = 0; i < points.size(); i++)
	{
		point p3 = points[i];

		double currh = ((p1.x * (p2.y - p3.y)) + (p2.x * (p3.y - p1.y)) + (p3.x * (p1.y - p2.y)));

		if (currh < h)
		{
			maxpt = p3;
			h = currh;
		}
		if (currh < 0)
			newpts.push_back(p3);
	}

	if (h >= 0)
	{
		vector<point> fin;
		fin.push_back(p1);
		return fin;
	}

	//left half
	vector<point> left_half = lower_hull(p1, maxpt, newpts);
	//right half
	vector<point> right_half = lower_hull(maxpt, p2, newpts);

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
		//Read in points
		for (int j = 0; j < num_input; j++)
		{
			int x, y;
			cin >> x >> y;
			point temp(x, y, j+1);
			points.push_back(temp);
			
		}


		//Edge Cases
		if (num_input == 0)
		{
			cout << "0.00" << endl << endl;
			continue;
		}
		if (num_input == 1)
		{
			cout << "0.00" << endl;
			cout << "1" << endl << endl;
			continue;
		}

		//sort points
		sort(points.begin(), points.end(), srt);

		//Remove dupicates
		vector<point>::iterator it;
		it = unique(points.begin(), points.end(), unique_pts);

		points.resize(distance(points.begin(), it));
		
		//One more edge
		if (num_input == 2)
		{
			double circumference = 2 * sqrt(abs(pow((points[0].x - points[1].x), 2) + pow((points[0].y - points[1].y), 2)));
			cout << fixed << setprecision(2) << circumference << endl;
			cout << points[0].num << " " << points[1].num << endl << endl;
			continue;
		}

		//~~~~~~~~~~~Finding Edge Points~~~~~~~~~~~

		point left_most = points.front();
		point right_most = points.back();
		points.erase(points.begin());
		points.erase(points.begin() + points.size() - 1);

		//~~~~~~~~~~Run Quickhull~~~~~~~~~~

		//note: upperhull has already been sorted into counter clockwise order
		vector<point> upperhull = upper_hull(left_most, right_most, points);
		vector<point> lowerhull = lower_hull(left_most, right_most, points);

		vector<point> completehull = lowerhull;


		for (int i = 0; i < upperhull.size(); i++)
		{
			completehull.push_back(upperhull.at(i));
		}

		completehull.push_back(left_most);

		/* Remove Duplicates
		* -This is important if it is in a line
		*/
		for (int i = 0; i < completehull.size() - 2; i++)
		{
			for (int j = i + 1; j < completehull.size() - 1; j++)
				if (unique_pts(completehull[i], completehull[j]))
				{
					completehull.erase(completehull.begin() + j);
					j--;
				}
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