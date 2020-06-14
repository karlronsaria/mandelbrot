#include "Render.h"

volatile std::atomic<bool> Renderer::Threads::rendering = false;
volatile std::atomic<bool> Renderer::Threads::paused = false;

Renderer::Renderer(
	std::reference_wrapper<sf::Image> image,
	std::reference_wrapper<Overlay> display,
	const State& s
) :
	_type(s.type),
	_scales(Geometry2D(s.view, s.models.top())),
	_view(s.view),
	_image(image),
	_display(display),
	_iteration(0),
	_threshold(mnd::THRESHOLDS[s.threshold]),
	_max_iterations(s.max_iterations),
	_power(s.power),
	_j_coords(s.j_coords),
	_plot(new pair_t[s.view.bottom * s.view.right]),
	_fnc(mnd::FunctionByOrder(s.power)),
	_col(mnd::COLOR_SCHEMES[s.color_scheme_index]),
	_alg(mnd::ALGORITHMS[s.algorithm_index])
{}

void Renderer::Start() {
	Renderer::Threads::rendering = true;

	switch (_type) {
	case mnd::MANDELBROT:
		Interruptible::InitializePlot(_plot, _view);
		_color_pixel_method = &Renderer::ColorMandelbrotPixel;
		break;
	case mnd::JULIA:
		Interruptible::InitializeJulia(_plot, _view, _scales);
		_color_pixel_method = &Renderer::ColorJuliaPixel;
		break;
	}

	_display.get().init_iteration();
	_display.get().rendering(true);

	sf::Image someImage;
	someImage.create(_view.right, _view.bottom, mnd::INIT_COLOR);

	while (Renderer::Threads::rendering && _display.get().iteration() < _max_iterations) {
		while (Renderer::Threads::paused);

		if (RenderFrame(someImage) > 0)
			_image.get() = someImage;

		_iteration = _display.get().next_iteration();
	}

	_display.get().rendering(false);
	Renderer::Threads::rendering = false;

	if (_plot != nullptr) {
		delete[] _plot;
		_plot = nullptr;
	}
}

bool Renderer::ColorPixel(sf::Image& someImage, int_t x, int_t y, pair_t c) {
	if (someImage.getPixel(x, y) == mnd::INIT_COLOR) {
		auto index = (_view.right - _view.left) * y + x;
		auto value = _alg(_plot[index], c, _power, _iteration, _threshold, _fnc);

		if (value > 0LL) {
			someImage.setPixel(x, y, _col(value));
			return true;
		}
	}

	return false;
}

bool Renderer::ColorMandelbrotPixel(sf::Image& someImage, int_t x, int_t y) {
	return ColorPixel(someImage, x, y, pair_t{ _scales.coord_x(x), _scales.coord_y(y) });
}

bool Renderer::ColorJuliaPixel(sf::Image& someImage, int_t x, int_t y) {
	return ColorPixel(someImage, x, y, _j_coords);
}

int Renderer::RenderFrame(sf::Image& someImage) {
	std::vector<std::thread> threads;
	volatile std::atomic<int> rendered = 0;

	auto render_task = [&](view_t view) {
		int_t x, y;

		for (y = view.top; Renderer::Threads::rendering && y < view.bottom; ++y)
			for (x = view.left; Renderer::Threads::rendering && x < view.right; ++x)
				if ((this->*_color_pixel_method)(someImage, x, y))
					++rendered;
	};

	threads.push_back(std::thread(render_task, view_t{ _view.left, _view.right / 2, _view.top, _view.bottom / 2 }));
	threads.push_back(std::thread(render_task, view_t{ _view.right / 2, _view.right, _view.top, _view.bottom / 2 }));
	threads.push_back(std::thread(render_task, view_t{ _view.left, _view.right / 2, _view.bottom / 2, _view.bottom }));
	threads.push_back(std::thread(render_task, view_t{ _view.right / 2, _view.right, _view.bottom / 2, _view.bottom }));

	Join(threads);
	return rendered;
}

void Renderer::Interruptible::InitializePlot(plot_t& plot, const view_t& view) {
	for (int_t i = 0; Renderer::Threads::rendering && i < view.bottom * view.right; ++i)
		plot[i] = INIT_PAIR;
}

void Renderer::Interruptible::InitializeJulia(plot_t& plot, const view_t& view, const Geometry2D& scales) {
	int_t x, y;

	for (y = view.top; Renderer::Threads::rendering && y < view.bottom; ++y)
		for (x = view.left; Renderer::Threads::rendering && x < view.right; ++x)
			plot[(view.right - view.left) * y + x] = { scales.coord_x(x), scales.coord_y(y) };
}
