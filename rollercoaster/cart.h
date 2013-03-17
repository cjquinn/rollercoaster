#ifndef CART_H
#define CART_H

class Frame;
class ObjModel;
class Spline;

class Cart
{
public:
	Cart();
	~Cart();

	void create(Spline *spline);

	void render();
	void update(double dt);
private:
	Frame *frame_;
	ObjModel *model_;
	Spline *spline_;
};

#endif