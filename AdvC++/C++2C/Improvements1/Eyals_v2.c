#include <stdio.h>
#include <stdlib.h>

/*///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
Virtual pointer typedef
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////*/
typedef void (*VirtualFunc)(void);
typedef VirtualFunc VTable;
/*///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
struct Color {
	enum ColorEnum { RED, GREEN, DEFAULT };
	static void setColor(ColorEnum color){		
		static const char * pallete[] = { "\x1B[31m", "\x1B[32m", "\033[0m" };
		std::puts(pallete[color]);
	}
};
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////*/
typedef enum Color_ColorEnum
{
	RED,
	GREEN,
	DEFAULT
}Color_ColorEnum;
void Color_setColor(Color_ColorEnum color);

void Color_setColor(Color_ColorEnum color)
{
	static const char * Color_setColor_pallete[] = { "\x1B[31m", "\x1B[32m", "\033[0m" };
	puts(Color_setColor_pallete[color]);
}
/*///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
class Scaleable {
public:
	virtual ~Scaleable() { }	
	virtual void scale(double) = 0;
};
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////*/
typedef struct Scaleable
{
	VTable*	m_vPtr;
}Scaleable;
void Scaleable_COMPILER_Default_CTOR(Scaleable* const _this);
void Scaleable_DTOR(Scaleable* const _this);
void Scaleable_scale_DBL(Scaleable* const _this, double _f);
VTable scaleable_vTable[2] = {(VirtualFunc) Scaleable_DTOR
							,(VirtualFunc) Scaleable_scale_DBL};
/*
Compiler generated default constructor
*/
void Scaleable_COMPILER_Default_CTOR(Scaleable* const _this)
{
	_this->m_vPtr = scaleable_vTable;
	return;
}
/*
virtual ~Scaleable() { }
*/
void Scaleable_DTOR(Scaleable* const _this)
{
	/*Empty*/
}
/*
virtual void scale(double) = 0;
*/
void Scaleable_scale_DBL(Scaleable* const _this, double _f)
{
	perror("called pure virtual function scaleable");
}
/*///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
class Shape : public Scaleable {
public:
	Shape() : m_id(++NumOfShapes) {
		std::printf("Shape::Shape() - %d\n", m_id); 
	}

	~Shape() {
		draw();
		--NumOfShapes; 
		std::printf("Shape::~Shape - %d\n", m_id);
	}

	Shape(const Shape& other) : m_id(++NumOfShapes) {
		std::printf("Shape::Shape(Shape&) - %d from - %d\n", m_id, other.m_id);
	}

	virtual void draw() const {
		std::printf("Shape::draw() - %d\n", m_id);		
	}
	
	virtual void draw(Color::ColorEnum c) const {
		std::printf("Shape::draw(c) - %d\n", m_id);
		Color::setColor(c);
		draw();
		Color::setColor(Color::DEFAULT);
	}
	
	virtual void scale(double f = 1) { 
		std::printf("Shape::scale(%f)\n", f);
	}
	virtual double area() const { return -1; }
	static void printInventory() {
		std::printf("Shape::printInventory - %d\n", NumOfShapes);
	}

private:
	static int NumOfShapes;
	Shape& operator=(const Shape &); // disabled, no implementation provided

protected:
	 int m_id;
};

int Shape::NumOfShapes = 0;
///////////////////////////////////
///////////////////////////////////
///////////////////////////////////*/
typedef struct Shape
{
/*	VTable*		m_vPtr;*/
	Scaleable 	m_scaleable;
	int			m_id;
}Shape;
int Shape_int_NumOfShapes = 0;
void Shape_Default_CTOR(Shape* const _this);
void Shape_DTOR(Shape* const _this);
void Shape_Copy_CTOR(Shape* const _this, const Shape* const _other);
void Shape_draw(const Shape* const _this);
void Shape_draw_CLR(const Shape* const _this, Color_ColorEnum _c);
void Shape_scale_DBL(Shape* const _this, double _f);
double Shape_area(const Shape* const _this);
void Shape_printInventory();
VTable shape_vTable[5] = {(VirtualFunc) Shape_DTOR
						, (VirtualFunc) Shape_scale_DBL
						, (VirtualFunc) Shape_draw
						, (VirtualFunc) Shape_draw_CLR
						, (VirtualFunc) Shape_area};
/*
Shape() : m_id(++NumOfShapes) {
		std::printf("Shape::Shape() - %d\n", m_id); 
	}
*/
void Shape_Default_CTOR(Shape* const _this)
{
	Scaleable_COMPILER_Default_CTOR((Scaleable* const)_this);
	_this->m_scaleable.m_vPtr = shape_vTable;
	_this->m_id = ++Shape_int_NumOfShapes;
	printf("Shape::Shape() - %d\n", _this->m_id); 
}
/*
~Shape() {
	draw();
	--NumOfShapes; 
	std::printf("Shape::~Shape - %d\n", m_id);
}
*/
void Shape_DTOR(Shape* const _this)
{
	Shape_draw(_this);
	--Shape_int_NumOfShapes;
	printf("Shape::~Shape - %d\n", _this->m_id);
	_this->m_scaleable.m_vPtr = scaleable_vTable;
	Scaleable_DTOR((Scaleable* const) _this);
}
/*
Shape(const Shape& other) : m_id(++NumOfShapes) {
	std::printf("Shape::Shape(Shape&) - %d from - %d\n", m_id, other.m_id);
}
*/
void Shape_Copy_CTOR(Shape* const _this, const Shape* const _other)
{
	Scaleable_COMPILER_Default_CTOR((Scaleable* const)_this);
	_this->m_scaleable.m_vPtr = shape_vTable;
	_this->m_id = ++Shape_int_NumOfShapes;
	printf("Shape::Shape(Shape&) - %d from - %d\n", _this->m_id, _other->m_id);
}
/*
virtual void draw() const {
	std::printf("Shape::draw() - %d\n", m_id);		
}
*/
void Shape_draw(const Shape* const _this)
{
	printf("Shape::draw() - %d\n", _this->m_id);
}
/*
virtual void draw(Color::ColorEnum c) const {
	std::printf("Shape::draw(c) - %d\n", m_id);
	Color::setColor(c);
	draw();
	Color::setColor(Color::DEFAULT);
}
*/
void Shape_draw_CLR(const Shape* const _this, Color_ColorEnum _c)
{
	printf("Shape::draw(c) - %d\n", _this->m_id);
	Color_setColor(_c);
	Shape_draw(_this);
	Color_setColor(DEFAULT);
}
/*
virtual void scale(double f = 1) { 
	std::printf("Shape::scale(%f)\n", f);
}
*/
void Shape_scale_DBL(Shape* const _this, double _f)
{
	printf("Shape::scale(%f)\n", _f);
}
/*
virtual double area() const { return -1; }
*/
double Shape_area(const Shape* const _this)
{
	return -1;
}
/*
static void printInventory() {
	std::printf("Shape::printInventory - %d\n", NumOfShapes);
}
*/
void Shape_printInventory()
{
	printf("Shape::printInventory - %d\n", Shape_int_NumOfShapes);
}
/*///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
class Circle : public Shape {	
public:
	Circle() : m_radius(1) { 
		std::printf("Circle::Circle() - %d, r:%f\n", m_id, m_radius); 
	}

	Circle(double r) : m_radius(r) { 
		std::printf("Circle::Circle(double) - %d, r:%f\n", m_id, m_radius); 
	}

	Circle(const Circle& other)
	: Shape(other), m_radius(other.m_radius) { 
		std::printf("Circle::Circle(Circle&) - %d, r:%f\n", m_id, m_radius);
	}

	~Circle() { 
		std::printf("Circle::~Circle() - %d, r:%f\n", m_id, m_radius); 
	}

	void draw() const { 
		std::printf("Circle::draw()  - %d, r:%f\n", m_id, m_radius);
	}

	void scale(double f = 2) {
		std::printf("Circle::scale(%f)\n", f);
		m_radius *= f;
	}
    
	double area() const {
		return m_radius * m_radius * 3.1415;	
	}

	double radius() const { 
		std::printf("Circle::draw()  - %d, r:%f\n", m_id, m_radius);
		return m_radius;
	}

private:
	double m_radius;
};
///////////////////////////////////
///////////////////////////////////
//////////////////////////////////*/
typedef struct Circle
{
/*	VTable*	m_vPtr;*/
	Shape 	m_shape;
	double 	m_radius;
}Circle;
void Circle_Default_CTOR(Circle* const _this);
void Circle_CTOR_DBL(Circle* const _this, double _r);
void Circle_Copy_CTOR(Circle* const _this, const Circle* const _other);
void Circle_DTOR(Circle* const _this);
void Circle_draw(Circle* const _this);
void Circle_scale_DBL(Circle* const _this, double _f);
double Circle_area(const Circle* const _this);
double Circle_radius(Circle* const _this);
VTable circle_vTable[5] = {(VirtualFunc) Circle_DTOR
						, (VirtualFunc) Circle_scale_DBL
						, (VirtualFunc) Circle_draw
						, (VirtualFunc) Shape_draw_CLR
						, (VirtualFunc) Circle_area};
/*
Circle() : m_radius(1) { 
	std::printf("Circle::Circle() - %d, r:%f\n", m_id, m_radius); 
}
*/
void Circle_Default_CTOR(Circle* const _this)
{
	Shape_Default_CTOR((Shape* const) _this);
	_this->m_shape.m_scaleable.m_vPtr = circle_vTable;
	_this->m_radius = 1;
	printf("Circle::Circle() - %d, r:%f\n", _this->m_shape.m_id, _this->m_radius);
}
/*
Circle(double r) : m_radius(r) { 
	std::printf("Circle::Circle(double) - %d, r:%f\n", m_id, m_radius); 
}
*/
void Circle_CTOR_DBL(Circle* const _this, double _r)
{
	Shape_Default_CTOR((Shape* const) _this);
	_this->m_shape.m_scaleable.m_vPtr = circle_vTable;
	_this->m_radius = _r;
	printf("Circle::Circle(double) - %d, r:%f\n", _this->m_shape.m_id, _this->m_radius); 
}
/*
Circle(const Circle& other)
: Shape(other), m_radius(other.m_radius) { 
	std::printf("Circle::Circle(Circle&) - %d, r:%f\n", m_id, m_radius);
}
*/
void Circle_Copy_CTOR(Circle* const _this, const Circle* const _other)
{
/*	Shape_Default_CTOR((Shape* const) _this);*/
	Shape_Copy_CTOR((Shape* const) _this, (Shape* const) _other);
	_this->m_shape.m_scaleable.m_vPtr = circle_vTable;
	_this->m_radius = _other->m_radius;
	printf("Circle::Circle(Circle&) - %d, r:%f\n", _this->m_shape.m_id, _this->m_radius);
}
/*
~Circle() { 
	std::printf("Circle::~Circle() - %d, r:%f\n", m_id, m_radius); 
}
*/
void Circle_DTOR(Circle* const _this)
{
	printf("Circle::~Circle() - %d, r:%f\n", _this->m_shape.m_id, _this->m_radius);
}
/*
void draw() const { 
	std::printf("Circle::draw()  - %d, r:%f\n", m_id, m_radius);
}
*/
void Circle_draw(Circle* const _this)
{
	printf("Circle::draw()  - %d, r:%f\n", _this->m_shape.m_id, _this->m_radius);
}
/*
void scale(double f = 2) {
	std::printf("Circle::scale(%f)\n", f);
	m_radius *= f;
}
*/
void Circle_scale_DBL(Circle* const _this, double _f)
{
	printf("Circle::scale(%f)\n", _f);
	_this->m_radius *= _f;
}
/*  
double area() const {
	return m_radius * m_radius * 3.1415;	
}
*/
double Circle_area(const Circle* const _this)
{
	return _this->m_radius * _this->m_radius * 3.1415;
}
/*
double radius() const { 
	std::printf("Circle::draw()  - %d, r:%f\n", m_id, m_radius);
	return m_radius;
}
*/
double Circle_radius(Circle* const _this)
{
	printf("Circle::draw()  - %d, r:%f\n", _this->m_shape.m_id, _this->m_radius);
	return _this->m_radius;
}




/*///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
class Rectangle: public Shape {
public:
	Rectangle() : m_a(1), m_b(1) { 
		std::printf("Rectangle::Rectangle() - %d, [%d, %d]\n", m_id, m_a, m_b);
	}

	Rectangle(int a) : m_a(a), m_b(a) { 
		std::printf("Rectangle::Rectangle(int) - %d, [%d, %d]\n", m_id, m_a, m_b);
	}

	Rectangle(int a, int b) : m_a(a), m_b(b) { 
		std::printf("Rectangle::Rectangle(int, int) - %d, [%d, %d]\n", m_id, m_a, m_b);
	}
	
	Rectangle(const Rectangle &other ) 
	: m_a(other.m_a), m_b(other.m_b), Shape(other) { 
		std::printf("Rectangle::Rectangle(Rectangle&) - %d, a:%d/%d\n", m_id, m_a, m_b);
	}
	
	~Rectangle() { 
		std::printf("Rectangle::~Rectangle() - %d, [%d, %d]\n", m_id, m_a, m_b);
	}

	void draw() const { 
		std::printf("Rectangle::draw()  - %d, [%d, %d]\n", m_id, m_a, m_b);
	}

    void draw(Color::ColorEnum c) const {
		std::printf("Rectangle::draw(%d)  - %d, [%d, %d]\n", c, m_id, m_a, m_b);
	}

	void scale(double f = 4){
		std::printf("Rectangle::scale(%f)\n", f);
		m_a *= f;
		m_b *= f;
	}

	double area() const {
		return m_a * m_b;
	}

private:
	int m_a, m_b;
};

void report(const Shape& s) {
	std::puts("-----report-----");
	s.draw(); 
	Shape::printInventory();
	std::puts("-----report-----");
}
///////////////////////////////////
///////////////////////////////////
//////////////////////////////////*/
typedef struct Rectangle
{
/*	VTable* m_vPtr;*/
	Shape 	m_shape;
	int		m_a;
	int		m_b;
}Rectangle;
void Rectangle_Default_CTOR(Rectangle* const _this);
void Rectangle_CTOR_INT(Rectangle* const _this, int _a);
void Rectangle_CTOR_INT_INT(Rectangle* const _this, int _a, int _b);
void Rectangle_Copy_CTOR(Rectangle* const _this, const Rectangle* const _other);
void Rectangle_DTOR(Rectangle* const _this);
void Rectangle_draw(const Rectangle* const _this);
void Rectangle_draw_CLR(const Rectangle* const _this, Color_ColorEnum _c);
void Rectangle_scale_DBL(Rectangle* const _this, double _f);
double Rectangle_area(const Rectangle* const _this);
VTable rectangle_vTable[5] = {(VirtualFunc) Rectangle_DTOR
						, (VirtualFunc) Rectangle_scale_DBL
						, (VirtualFunc) Rectangle_draw
						, (VirtualFunc) Rectangle_draw_CLR
						, (VirtualFunc) Rectangle_area};
/*
Rectangle() : m_a(1), m_b(1) { 
	std::printf("Rectangle::Rectangle() - %d, [%d, %d]\n", m_id, m_a, m_b);
}
*/
void Rectangle_Default_CTOR(Rectangle* const _this)
{
	Shape_Default_CTOR((Shape* const) _this);
	_this->m_shape.m_scaleable.m_vPtr = rectangle_vTable;
	_this->m_a = 1;
	_this->m_b = 1;
	printf("Rectangle::Rectangle() - %d, [%d, %d]\n", _this->m_shape.m_id, _this->m_a, _this->m_b);
}
/*
Rectangle(int a) : m_a(a), m_b(a) { 
	std::printf("Rectangle::Rectangle(int) - %d, [%d, %d]\n", m_id, m_a, m_b);
}
*/
void Rectangle_CTOR_INT(Rectangle* const _this, int _a)
{
	Shape_Default_CTOR((Shape* const) _this);
	_this->m_shape.m_scaleable.m_vPtr = rectangle_vTable;
	_this->m_a = _a;
	_this->m_b = _a;
	printf("Rectangle::Rectangle(int) - %d, [%d, %d]\n", _this->m_shape.m_id, _this->m_a, _this->m_b);	
}
/*
Rectangle(int a, int b) : m_a(a), m_b(b) { 
	std::printf("Rectangle::Rectangle(int, int) - %d, [%d, %d]\n", m_id, m_a, m_b);
}
*/
void Rectangle_CTOR_INT_INT(Rectangle* const _this, int _a, int _b)
{
	Shape_Default_CTOR((Shape* const) _this);
	_this->m_shape.m_scaleable.m_vPtr = rectangle_vTable;
	_this->m_a = _a;
	_this->m_b = _b;
	printf("Rectangle::Rectangle(int, int) - %d, [%d, %d]\n", _this->m_shape.m_id, _this->m_a, _this->m_b);
}
/*
Rectangle(const Rectangle &other ) 
: m_a(other.m_a), m_b(other.m_b), Shape(other) { 
	std::printf("Rectangle::Rectangle(Rectangle&) - %d, a:%d/%d\n", m_id, m_a, m_b);
}
*/
void Rectangle_Copy_CTOR(Rectangle* const _this, const Rectangle* const _other)
{
	Shape_Copy_CTOR((Shape* const) _this, (Shape* const) _other);
	_this->m_shape.m_scaleable.m_vPtr = rectangle_vTable;
	_this->m_a = _other->m_a;
	_this->m_b = _other->m_b;
	printf("Rectangle::Rectangle(Rectangle&) - %d, a:%d/%d\n", _this->m_shape.m_id, _this->m_a, _this->m_b);
}
/*
~Rectangle() { 
	std::printf("Rectangle::~Rectangle() - %d, [%d, %d]\n", m_id, m_a, m_b);
}
*/
void Rectangle_DTOR(Rectangle* const _this)
{
	printf("Rectangle::~Rectangle() - %d, [%d, %d]\n", _this->m_shape.m_id, _this->m_a, _this->m_b);
	_this->m_shape.m_scaleable.m_vPtr = shape_vTable;
	Shape_DTOR((Shape* const) _this);
}
/*
void draw() const { 
	std::printf("Rectangle::draw()  - %d, [%d, %d]\n", m_id, m_a, m_b);
}
*/
void Rectangle_draw(const Rectangle* const _this)
{
	printf("Rectangle::draw()  - %d, [%d, %d]\n", _this->m_shape.m_id, _this->m_a, _this->m_b);
}
/*
void draw(Color::ColorEnum c) const {
	std::printf("Rectangle::draw(%d)  - %d, [%d, %d]\n", c, m_id, m_a, m_b);
}
*/
void Rectangle_draw_CLR(const Rectangle* const _this, Color_ColorEnum _c)
{
	printf("Rectangle::draw(%d)  - %d, [%d, %d]\n", _c, _this->m_shape.m_id, _this->m_a, _this->m_b);
}
/*
void scale(double f = 4){
	std::printf("Rectangle::scale(%f)\n", f);
	m_a *= f;
	m_b *= f;
}
*/
void Rectangle_scale_DBL(Rectangle* const _this, double _f)
{
	printf("Rectangle::scale(%f)\n", _f);
	_this->m_a *= _f;
	_this->m_b *= _f;
}
/*
double area() const {
	return m_a * m_b;
}
*/
double Rectangle_area(const Rectangle* const _this)
{
	return _this->m_a * _this->m_b;
}











/*///////////////////////////////////
///////////////////////////////////
//////////////////////////////////


void dispose(Rectangle* p){
  delete[] p;
}
///////////////////////////////////
///////////////////////////////////
//////////////////////////////////*/

/*
void report(const Shape& s) {
	std::puts("-----report-----");
	s.draw(); 
	Shape::printInventory();
	std::puts("-----report-----");
}
*/
void report_ShapeRef(const Shape* const _s)
{
	puts("-----report-----");
	((void(*)(const Shape* const))_s->m_scaleable.m_vPtr[2])(_s);
	Shape_printInventory();
	puts("-----report-----");
}
/*
inline void draw(Shape& obj) { 
	std::puts("-----draw(Shape&)-----");
	obj.scale();
	obj.draw();	
	std::puts("-----draw(Shape&)-----");
}
*/
void draw_ShapeRef(const Shape* const _obj)
{
	puts("-----draw(Shape&)-----");
	((void(*)(const Shape* const, double))_obj->m_scaleable.m_vPtr[1])(_obj, 1);
	((void(*)(const Shape* const))_obj->m_scaleable.m_vPtr[2])(_obj);
	puts("-----draw(Shape&)-----");
}
/*
void draw(Circle c) { 
	std::puts("-----draw(Circle)-----");

	static Circle unit(1);
	
	unit.draw();
	unit.scale(3);
	c.draw(); 
	std::puts("-----draw(Circle)-----");
}
*/
Circle Circle_unit;
int isInit_Circle_unit = 0;
void draw_Circle(Circle c)
{
	puts("-----draw(Circle)-----");
	if (!isInit_Circle_unit)
	{
		Circle_CTOR_DBL(&Circle_unit, 1);
		isInit_Circle_unit = 1;
	}
	Circle_draw(&Circle_unit);
	Circle_scale_DBL(&Circle_unit, 3);
	Circle_draw(&c);
	puts("-----draw(Circle)-----");
}
/*
void doObjArray(){
	Shape objects[] = {
	    Circle(),
	    Rectangle(4),
	    Circle(9)
	};

    for(int i = 0; i < 3; ++i) 
		objects[i].draw();
}
*/
void doObjArray()
{
	Shape objects[3];
	Circle_Default_CTOR((Circle* const) &objects[0]);
	Rectangle_CTOR_INT((Rectangle* const) &objects[1], 4);
	Circle_CTOR_DBL((Circle* const) &objects[2], 9);
	
	
	Circle_draw((Circle* const) &objects[0]);
	Rectangle_draw((Rectangle* const) &objects[1]);
	Circle_draw((Circle* const) &objects[2]);
}
/*
void disappear() {
	std::puts("-----disappear-----");

	Circle defaultCircle();

	std::puts("-----disappear-----");
}
*/
void disappear()
{
	puts("-----disappear-----");
	puts("-----disappear-----");
}
/*
template <class T>
double diffWhenDoubled(T& shape){
	double a0 = shape.area();
	shape.scale(2);
	double a1 = shape.area();
	return a1 - a0;
}
*/
double diffWhenDoubled(Circle* const _shape)
{
	double a0;
	double a1;
	double returnTemp;
	
	a0 = Circle_area(_shape);
	Circle_scale_DBL(_shape, 2);
	a1 = Circle_area(_shape);
	returnTemp = a1 - a0;
	return returnTemp;
}
/*
void doPointerArray(){
	std::puts("-----doPointerArray-----");
	Shape *array[] =  {
	    new Circle(),
	    new Rectangle(3),
	    new Circle(4)
	};

    for(int i = 0; i < 3; ++i){ 
		array[i]->scale();
		array[i]->draw();
	}

	std::printf("area: %f\n", diffWhenDoubled(*array[2]));

    for(int i = 0; i < 3; ++i) { 
		delete array[i]; 
		array[i] = 0; 
	}

	std::puts("-----doPointerArray-----");
}
*/
void doPointerArray()
{
	puts("-----doPointerArray-----");
	
}



















/*///////////////////////////////////
///////////////////////////////////
///////////////////////////////////
int main(int argc, char **argv, char **envp)
{	
	std::printf("---------------Start----------------\n");
    Circle c;
	Rectangle s(4);

	std::printf("0.-------------------------------\n");		
	draw(c);

	std::printf("+..............\n");		
	draw(c);

	std::printf("+..............\n");		
    draw(s);

	std::printf("+..............\n");		
	report(c);

	std::printf("1.-------------------------------\n");	
	
    doPointerArray();

	std::printf("2.-------------------------------\n");

    doObjArray();

	std::printf("3.-------------------------------\n");

    Shape::printInventory();
    Circle c2 = c;
    c2.printInventory();

	std::printf("4.-------------------------------\n");
   
    Circle olympics[5];
	std::printf("olympic diff %f\n", diffWhenDoubled(olympics[1]));

	std::printf("5.-------------------------------\n");

    Rectangle *fourRectangles = new Rectangle[4];
    dispose(fourRectangles);

	std::printf("6.-------------------------------\n");
	EmptyBag eb;
	std::printf("Empty things are: %zu %zu %zu", sizeof(Empty), sizeof(EmptyEmpty), sizeof(EmptyBag) );
	
	std::printf("7.-------------------------------\n");
	disappear();	

	std::printf("---------------END----------------\n");

    return 0;
}
////////////////////////////////////
///////////////////////////////////
//////////////////////////////////*/
int main()
{
/**************************
Local variable declarations
**************************/
	Circle c;
	Circle cTemp;
	Rectangle s;
/*	Circle c2 = c;*/
/*	Circle olympics[5];*/
/*	Rectangle* fourRectangles;*/
/*	EmptyBag eb;*/
/**************************
main body 
**************************/
	printf("---------------Start----------------\n");
	Circle_Default_CTOR(&c);
	Rectangle_CTOR_INT(&s, 4);
	printf("0.-------------------------------\n");
	Circle_Copy_CTOR(&cTemp, &c);
/*	draw_ShapeRef((Shape* const)&cTemp);*/
	draw_Circle(cTemp);
/*	printf("+..............\n");*/
/*	draw_ShapeRef(&c);*/
/*	printf("+..............\n");*/
/*	draw_ShapeRef(&s);*/
/*	printf("+..............\n");*/
/*	report_ShapeRef(&c);*/
/*	printf("1.-------------------------------\n");*/
/*	printf("2.-------------------------------\n");*/
/*	printf("3.-------------------------------\n");*/
/*	printf("4.-------------------------------\n");*/
/*	printf("5.-------------------------------\n");*/
/*	printf("6.-------------------------------\n");*/
/*	printf("Empty things are: %zu %zu %zu", sizeof(Empty), sizeof(EmptyEmpty), sizeof(EmptyBag) );*/
/*	printf("7.-------------------------------\n");*/
/*	printf("---------------END----------------\n");*/
	return 0;
}
































