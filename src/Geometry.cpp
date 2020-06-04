#include "Geometry.h"

Scale::Scale(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord) :
	_min_pixel(minPixel),
	_max_pixel(maxPixel),
	_min_coord(minCoord),
	_max_coord(maxCoord)
{
	_coord_to_pixel = Scale::GetCoordToPixelFactor(_min_pixel, _max_pixel, _min_coord, _max_coord);
}

Scale::~Scale() {}

flt_t Scale::GetCoordToPixelFactor(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord)
{
	return (maxPixel - minPixel) / (maxCoord - minCoord);
}

/*

Model length:  M = y - x
View length:   V = b - a

Model segment: W = w - x
View segment:  C = c - a

Note

	M is congruent to V

Then

	C/W = V/M
	(c - a)/(w - x) = (b - a)/(y - x)

Let

	f = (b - a)/(y - x)

Then

	(c - a)/(w - x) = f
	c - a = (w - x)f
	c = (w - x)f + a

Thus

	coord_to_pixel = (max_pixel - min_pixel)/(max_coord - min_coord)
	pixel = (coord - min_coord)(coord_to_pixel) + min_pixel

*/
int_t Scale::to_pixel(flt_t coord) const
{
	return (int_t)((coord - _min_coord) * _coord_to_pixel) + _min_pixel;
}

/*
Recall

	(c - a)/(w - x) = f

Then

	(w - x)/(c - a) = 1/f
	w - x = (c - a)(1/f)
	w = (c - a)(1/f) + x

Thus

	coord = (pixel - min_pixel)(1/coord_to_pixel) + min_coord

*/
flt_t Scale::to_coord(int_t pixel) const
{
	return ((pixel - _min_pixel) / _coord_to_pixel) + _min_coord;
}

int_t Scale::min_pixel() const { return _min_pixel; }
int_t Scale::max_pixel() const { return _max_pixel; }
flt_t Scale::min_coord() const { return _min_coord; }
flt_t Scale::max_coord() const { return _max_coord; }


YScale::YScale(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord) :
	Scale(minPixel, maxPixel, minCoord, maxCoord) {}

YScale::~YScale() {}

int_t YScale::to_pixel(flt_t coord) const
{
	return max_pixel() - Scale::to_pixel(coord);
}

flt_t YScale::to_coord(int_t pixel) const
{
	return Scale::to_coord(max_pixel() - pixel);
}

flt_t YScale::to_coord_uninverted(int_t pixel) const
{
	return Scale::to_coord(pixel);
}


Geometry2D::Geometry2D(const view_t& view, const model_t& model) :
	_horz(view.left, view.right, model.left, model.right),
	_vert(view.top, view.bottom, model.top, model.bottom) {}

Geometry2D::Geometry2D() :
	_horz(1, 1, 1, 1),
	_vert(1, 1, 1, 1) {}

Geometry2D::~Geometry2D() {}

Scale Geometry2D::horz() const { return _horz; }
Scale Geometry2D::vert() const { return _vert; }

void Geometry2D::horz(const Scale& other)
{
	_horz = other;
}

void Geometry2D::vert(const Scale& other)
{
	_vert = other;
}

void Geometry2D::horz(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord)
{
	_horz = Scale(minPixel, maxPixel, minCoord, maxCoord);
}

void Geometry2D::vert(int_t minPixel, int_t maxPixel, flt_t minCoord, flt_t maxCoord)
{
	_vert = YScale(minPixel, maxPixel, minCoord, maxCoord);
}

flt_t Geometry2D::coord_x(int_t pixel_x) const
{
	return _horz.to_coord(pixel_x);
}

flt_t Geometry2D::coord_y(int_t pixel_x) const
{
	return _vert.to_coord(pixel_x);
}

flt_t Geometry2D::coord_y_uninverted(int_t pixel_x) const
{
	return _vert.to_coord(pixel_x);
}

void Geometry2D::resize(int_t min_x, int_t max_x, int_t min_y, int_t max_y)
{
	horz(min_x, max_x, _horz.min_coord(), _horz.max_coord());
	vert(min_y, max_y, _vert.min_coord(), _vert.max_coord());
}

void Geometry2D::transform(flt_t min_x, flt_t max_x, flt_t min_y, flt_t max_y)
{
	horz(_horz.min_pixel(), _horz.max_pixel(), min_x, max_x);
	vert(_vert.min_pixel(), _horz.max_pixel(), min_y, max_y);
}

void Geometry2D::resize(const view_t& view)
{
	resize(view.left, view.right, view.top, view.bottom);
}

void Geometry2D::transform(const model_t& model)
{
	transform(model.left, model.right, model.top, model.bottom);
}


const Geometry2D& Plot2D::geography() const
{
	return *_geography;
}

void Plot2D::geography(geo_ptr& other)
{
	_geography = other;
}

Plot2D::~Plot2D() {}

flt_t Plot2D::coord_x(int_t pixel_x) const
{
	return geography().coord_x(pixel_x);
}

flt_t Plot2D::coord_y(int_t pixel_y) const
{
	return geography().coord_y(pixel_y);
}


Map2D::~Map2D() {}


Functional2D::Functional2D(geo_ptr& geo, unary_t map, flt_t epsilon) :
	_map(map),
	_epsilon(epsilon)
{
	geography(geo);
}

bool Functional2D::contains(int_t x, int_t y) const
{
	return abs(coord_y(y) - _map(coord_x(x))) < _epsilon;
}


Parametric2D::Parametric2D(geo_ptr& geo, unary_t xInverse, unary_t yInverse, flt_t epsilon) :
	_x_inverse(xInverse),
	_y_inverse(yInverse),
	_epsilon(epsilon)
{
	geography(geo);
}

bool Parametric2D::contains(int_t x, int_t y) const
{
	return abs(_y_inverse(coord_y(y)) - _x_inverse(coord_x(x))) < _epsilon;
}


void Surface2D::height(binary_t definition)
{
	_height = definition;
}

Surface2D::~Surface2D() {};

flt_t Surface2D::height(int_t x, int_t y) const
{
	return _height(coord_x(x), coord_y(y));
}

bool Surface2D::contains(int_t x, int_t y) const
{
	return _contains(coord_x(x), coord_y(y));
}
