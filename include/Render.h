#pragma once
#ifdef DEBUG
#include "/devlib/cpp/og/include/Debug.h"
#endif
#include "Mandelbrot.h"
#include "Overlay.h"
#include "State.h"
#include "Threads.h"

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
	pair_t _j_coords;
	pair_t* _plot;
	std::reference_wrapper<sf::Image> _image;
	std::reference_wrapper<Overlay> _display;
	complex_f _fnc;
	color_code_f _col;
	algorithm_f _alg;

	static std::vector<complex_f> _complex_functions;
	static std::vector<algorithm_f> _algorithms;
	static std::vector<color_code_f> _color_schemes;

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
	void Start();

	static complex_f FunctionByIndex(int_t index);
	static color_code_f ColorSchemeByIndex(int_t index);
	static algorithm_f AlgorithmByIndex(int_t index);

	static complex_f FunctionByOrder(int_t order);

	static std::vector<complex_f> GetFunctions();
	static std::vector<algorithm_f> GetAlgorithms();
	static std::vector<color_code_f> GetColorSchemes();

	class Interruptible {
	public:
		static void InitializePlot(pair_t*& plot, const view_t& view);
		static void InitializeJulia(pair_t*& plot, const view_t& view, const Geometry2D& scales);
	};

	class Threads {
	public:
		static volatile std::atomic<bool> rendering;
		static volatile std::atomic<bool> paused;
	};
};
