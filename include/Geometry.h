#pragma once
#include <memory>

#define INT_TYPE   long long
#define FLOAT_TYPE long double

typedef INT_TYPE   int_t;
typedef FLOAT_TYPE flt_t;

class Scale
{
private:
	flt_t _coord_to_pixel;

	int_t _min_pixel;
	int_t _max_pixel;

	flt_t _min_coord;
	flt_t _max_coord;
public:
	Scale(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord);
	virtual ~Scale();

	static flt_t GetCoordToPixelFactor(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord);

	int_t to_pixel(flt_t coord) const;
	flt_t to_coord(int_t pixel) const;

	int_t min_pixel() const;
	int_t max_pixel() const;
	flt_t min_coord() const;
	flt_t max_coord() const;
};

class YScale : public Scale
{
public:
	YScale(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord);
	virtual ~YScale();

	int_t to_pixel(flt_t coord) const;
	flt_t to_coord(int_t pixel) const;
	flt_t to_coord_uninverted(int_t pixel) const;
};

template <typename T>
struct Boundaries
{
	T left;
	T right;
	T top;
	T bottom;
};

typedef Boundaries<int_t> view_t;
typedef Boundaries<flt_t> model_t;

class Geometry2D
{
private:
	Scale _horz;
	Scale _vert;
public:
	Geometry2D(const view_t& view, const model_t& model);
	Geometry2D(const Geometry2D&) = default;
	Geometry2D(Geometry2D&&) = default;
	Geometry2D();
	virtual ~Geometry2D();

	Geometry2D& operator=(const Geometry2D&) = default;
	Geometry2D& operator=(Geometry2D&&) = default;

	Scale horz() const;
	Scale vert() const;

	void horz(const Scale& other);
	void vert(const Scale& other);
	void horz(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord);
	void vert(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord);

	flt_t coord_x(int_t pixel_x) const;
	flt_t coord_y(int_t pixel_x) const;
	flt_t coord_y_uninverted(int_t pixel_y) const;

	void resize(int_t min_x, int_t max_x, int_t min_y, int_t max_y);
	void transform(flt_t min_x, flt_t max_x, flt_t min_y, flt_t max_y);
	void resize(const view_t& view);
	void transform(const model_t& model);
};

typedef std::shared_ptr<Geometry2D> geo_ptr;

class Plot2D
{
private:
	geo_ptr _geography;
protected:
	void geography(geo_ptr& other);
public:
	virtual ~Plot2D();

	flt_t coord_x(int_t pixel_x) const;
	flt_t coord_y(int_t pixel_y) const;

	const Geometry2D& geography() const;
};

class Map2D : public Plot2D
{
public:
	virtual ~Map2D();
	virtual bool contains(int_t, int_t) const = 0;
};

typedef flt_t(*unary_t)(flt_t);
typedef flt_t(*binary_t)(flt_t, flt_t);

class Functional2D : public Map2D
{
private:
	unary_t _map;
	flt_t _epsilon;
public:
	Functional2D(geo_ptr& geo, unary_t map, flt_t epsilon);
	virtual bool contains(int_t x, int_t y) const override;
};

class Parametric2D : public Map2D
{
private:
	unary_t _x_inverse;
	unary_t _y_inverse;
	flt_t _epsilon;
public:
	Parametric2D(geo_ptr& geo, unary_t xInverse, unary_t yInverse, flt_t epsilon);
	virtual bool contains(int_t x, int_t y) const override;
};

class Surface2D : public Map2D
{
private:
	binary_t _height;
	bool (*_contains)(flt_t, flt_t);
protected:
	void height(binary_t definition);
public:
	virtual ~Surface2D();

	virtual flt_t height(int_t x, int_t y) const;
	virtual bool contains(int_t x, int_t y) const override;
};
