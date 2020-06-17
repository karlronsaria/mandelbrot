#pragma once
#ifdef DEBUG
#include "/devlib/cpp/og/include/Debug.h"
#endif
#include "Mandelbrot.h"
#include "Overlay.h"
#include "State.h"
#include "Threads.h"

typedef pair_t* plot_t;

class Renderer {
private:
	typedef bool(Renderer::* color_pixel_f)(sf::Image&, int_t x, int_t y);
	color_pixel_f _color_pixel_method;

	int_t _type;
	Geometry2D _scales;
	view_t _view;
	int_t _iteration;
	int_t _max_iterations;
	int_t _power;
	threshold_t _threshold;
	pair_t _j_coords;
	plot_t _plot;
	std::reference_wrapper<sf::Image> _image;
	std::reference_wrapper<Overlay> _display;
	mnd::complex_f _fnc;
	mnd::color_code_f _col;
	mnd::algorithm_f _alg;

	bool ColorPixel(sf::Image& someImage, int_t x, int_t y, pair_t c);
	bool ColorMandelbrotPixel(sf::Image& someImage, int_t x, int_t y);
	bool ColorJuliaPixel(sf::Image& someImage, int_t x, int_t y);
	int RenderFrame(sf::Image& someImage);
public:
	Renderer(
		std::reference_wrapper<sf::Image>,
		std::reference_wrapper<Overlay>,
		const State&
	);

	sf::Image Start();
	bool HasNext();
	void Next(sf::Image& someImage);
	void Close();
	void Run();

	class Interruptible {
	public:
		static void InitializePlot(plot_t& plot, const view_t& view);
		static void InitializeJulia(plot_t& plot, const view_t& view, const Geometry2D& scales);
	};

	class Threads {
	public:
		static volatile std::atomic<bool> rendering;
		static volatile std::atomic<bool> paused;
	};
};
