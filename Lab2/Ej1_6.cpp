#include <iostream>
#include <math.h>

using namespace std;

class Conversor
{
private:
	int ndhm1;
	int ndvm1;
	float ndc_inf;
	float ndc_sup;
	float xmin;
	float xmax;
	float ymin;
	float ymax;

public:
	Conversor(int horizontal_max, int vertical_max, float ndc_min, float ndc_max, float w_xmin, float w_xmax, float w_ymin, float w_ymax)
	{
		ndhm1 = horizontal_max;
		ndvm1 = vertical_max;
		ndc_inf = ndc_min;
		ndc_sup = ndc_max;
		xmin = w_xmin;
		xmax = w_xmax;
		ymin = w_ymin;
		ymax = w_ymax;
	}

	void inp_to_ndc(int dcx, int dcy, float &ndcx, float &ndcy)
	{
		ndcx = (ndc_sup - ndc_inf)/ndhm1 * dcx + ndc_inf;
		ndcy = (ndc_sup - ndc_inf)/ndvm1 * dcy + ndc_inf;
	}

	void ndc_to_user(float ndcx, float ndcy, float &x, float &y)
	{
		x = (xmax - xmin) * ndcx + xmin;
		y = (ymax - ymin) * ndcy + ymin;
	}

	void user_to_ndc(float x, float y, float &ndcx, float &ndcy)
	{
		ndcx = (x - xmin)/(xmax - xmin);
		ndcy = (y - ymin)/(ymax - ymin);
	}

	void ndc_to_dc(float ndcx, float ndcy, int &dcx, int &dcy)
	{
		dcx = (int) roundf((ndcx - ndc_inf) * ndhm1 / (ndc_sup - ndc_inf));
		dcy = (int) roundf((ndcy - ndc_inf) * ndvm1 / (ndc_sup - ndc_inf));
	}
};

int main()
{
	Conversor C(800, 600, -1, 1, -100, 100, -100, 100);

	int dcx, dcy;
	float ndcx1, ndcy1, ndcx2, ndcy2, x, y;
	cout << "DCX: "; cin >> dcx;
	cout << "DCY: "; cin >> dcy;

	C.inp_to_ndc(dcx, dcy, ndcx1, ndcy1);
	C.ndc_to_user(ndcx1, ndcy1, x, y);
	C.user_to_ndc(x, y, ndcx2, ndcy2);
	C.ndc_to_dc(ndcx2, ndcy2, dcx, dcy);

	cout << endl;
	cout << "(inp_to_ndc)" << endl;
	cout << "NDCX: " << ndcx1 << endl;
	cout << "NDCY: " << ndcy1 << endl;

	cout << endl;
	cout << "(ndc_to_user)" << endl;
	cout << "X: " << x << endl;
	cout << "Y: " << y << endl;

	cout << endl;
	cout << "(user_to_ndc)" << endl;
	cout << "NDCX: " << ndcx2 << endl;
	cout << "NDCY: " << ndcy2 << endl;

	cout << endl;
	cout << "(ndc_to_dc)" << endl;
	cout << "DCX: " << dcx << endl;
	cout << "DCY: " << dcy << endl;
	return 0;
}