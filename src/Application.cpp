#include "Application.h"

const int_t Application::MSG_DURATION_SEC = 3;
const int_t Application::POLLING_DELAY_MSEC = 250;
const char* Application::DEFAULT_EXTENSION = ".png";

volatile std::atomic<bool> Application::delay_next_poll;
volatile std::atomic<bool> Application::notifying;

union convert_t {
	flt_t f;
	unsigned char s[FLT_T_SIZE];
};

std::string to_hex_str(flt_t value) {
	convert_t x;
	x.f = value;
	std::stringstream buf;
	buf.fill('0');

	for (int i = 0; i < FLT_T_SIZE; ++i)
		buf << std::setw(COUPLET_SIZE) << std::hex << (int)x.s[i];

	return buf.str();
}

flt_t to_float(const std::string& word) {
	convert_t u = { 0.F };
	int i = 0, j, k = 0;
	std::string couplet;

	while (i < word.length()) {
		couplet = "";
		j = 0;

		while (j < COUPLET_SIZE) {
			couplet += word[i];
			j = j + 1;
			i = i + 1;
		}

		u.s[k] = std::stoi(couplet, nullptr, 16);
		k = k + 1;
	}

	return u.f;
}

std::string GetDateTimeString() {
	std::time_t temp = std::time(nullptr);
	struct tm time;
	localtime_s(&time, &temp);

	std::string mon = std::to_string(time.tm_mon + 1);
	std::string day = std::to_string(time.tm_mday);
	std::string hour = std::to_string(time.tm_hour);
	std::string min = std::to_string(time.tm_min);
	std::string sec = std::to_string(time.tm_sec);

	mon = mon.length() < 2 ? "0" + mon : mon;
	day = day.length() < 2 ? "0" + day : day;
	hour = hour.length() < 2 ? "0" + hour : hour;
	min = min.length() < 2 ? "0" + min : min;
	sec = sec.length() < 2 ? "0" + sec : sec;

	std::string str =
		std::to_string(time.tm_year + 1900)
		+ "_"
		+ mon
		+ "_"
		+ day
		+ "_"
		+ hour + min + sec;

	return str;
}

Application::Application(font_t& font) :
	_font(font),
	_scales(std::make_shared<Geometry2D>()),
	_states(State()),
	_main_overlay(font, _scales),
	_window(sf::VideoMode(WIDTH_PIXELS, HEIGHT_PIXELS, 32), "Test"),
	_magnifier(_window),
	event(),
	_show_overlay(true)
{
	RebuildGeometry();
	Threads::Add(_render_thread);
	Threads::Add(_clock_thread);
}

Application::~Application() {
	Renderer::Threads::rendering = false;
	Threads::Join();
}

const State& Application::state() const {
	return _current_state;
}

State& Application::state() {
	return _current_state;
}

const Overlay& Application::main_overlay() const {
	return _main_overlay;
}

Overlay& Application::main_overlay() {
	return _main_overlay;
}

void Application::Mandelbrot() {
	state()
		.new_type(MANDELBROT);
}

void Application::Julia(pair_t coordinates) {
	state()
		.new_type(JULIA)
		.new_j_coords(coordinates);
}

void Application::PushHistory() {
	_states.add(state());
}

void Application::PushOverlay() {
	main_overlay().state(state());
}

bool Application::GoToPreviousState() {
	if (_states.prev()) {
		state() = _states.current();
		return true;
	}
	
	return false;
}

bool Application::GoToNextState() {
	if (_states.next()) {
		state() = _states.current();
		return true;
	}

	return false;
}

void Application::RebuildGeometry() {
	*_scales = Geometry2D(state().view, state().models.top());
}

bool Application::PollNext() {
	return !Application::delay_next_poll && _window.pollEvent(event);
}

flt_t Application::GetPixelMinimum(flt_t rate) const {
	auto view = state().view;
	return rate * (flt_t)(view.right - view.left) * (view.bottom - view.top);
}

void Application::GetCenter(flt_t& x, flt_t& y) const {
	auto left = state().models.top().left;
	auto top = state().models.top().top;
	x = state().models.top().left + abs(left - state().models.top().right) / 2;
	y = state().models.top().top + abs(top - state().models.top().bottom) / 2;
}

// Format:
// 
//    yyyy_MM_dd_HHmmss_-_power_magnification_iteration_x_y
//    
std::string Application::NewFileName(std::string extension) const {
	int_t iteration = main_overlay().iteration();
	int_t power = state().power;
	int_t magnification = state().magnification;

	flt_t x, y;

	GetCenter(x, y);

	std::ostringstream buf;

	buf << std::hex << power << "_"
		<< std::hex << magnification << "_"
		<< std::hex << iteration << "_"
		<< to_hex_str(x)
		<< "_"
		<< to_hex_str(y);

	return GetDateTimeString() + "_-_" + buf.str() + extension;
}

void Application::Magnify() {
	state()
		.push_model(_magnifier.get_boundaries(*_scales))
		.next_magnification();

	RebuildGeometry();
}

void Application::Demagnify() {
	state()
		.pop_model()
		.prev_magnification();

	RebuildGeometry();
}

void Application::Update() {
	_texture.loadFromImage(_image);
	_sprite.setTexture(_texture);
	main_overlay().update(_window);
	_magnifier.update(_window);
}

void Application::Clear() {
	_window.clear();
}

void Application::Draw() {
	_window.draw(_sprite);

	if (_show_overlay) {
		main_overlay().draw_to(_window);
		_magnifier.draw_to(_window);
	}
}

void Application::Show() {
	_window.display();
}

bool Application::Save() {
	auto temp = Renderer::Threads::paused;
	Renderer::Threads::paused = true;

	sf::RenderTexture myTexture;

	if (!myTexture.create(state().view.right, state().view.bottom)) {
		Renderer::Threads::paused = temp;
		return false;
	}

	myTexture.display();
	myTexture.draw(_sprite);

	if (_show_overlay)
		main_overlay().draw_to(myTexture);

	myTexture
		.getTexture()
		.copyToImage()
		.saveToFile(NewFileName());

	Renderer::Threads::paused = temp;
	return true;
}

void Application::GoTo(const std::string& str) {
	std::stringstream buf(str);
	std::string temp;

	buf.ignore(50, '-');

	if (buf.eof())
		buf = std::stringstream(str);
	else
		buf.ignore(1, '_');

	buf << "_";

	getline(buf, temp, '_');
	int_t power = std::stoll(temp);

	getline(buf, temp, '_');
	int_t magnification = std::stoll(temp);

	getline(buf, temp, '_');
	int_t iteration = std::stoll(temp);

	getline(buf, temp, '_');
	flt_t x_coord = to_float(temp);

	getline(buf, temp, '.');
	flt_t y_coord = to_float(temp);

	state()
		.new_power(power)
		.init_magnification()
		.init_max_iterations();

	RebuildGeometry();

	int_t x_pixel;
	int_t y_pixel;

	while (magnification < state().magnification) {
		x_pixel = _scales->horz().to_pixel(x_coord);
		y_pixel = _scales->vert().to_pixel(y_coord);
		_magnifier.move(x_pixel, y_pixel);
		Demagnify();
	}

	while (magnification > state().magnification) {
		x_pixel = _scales->horz().to_pixel(x_coord);
		y_pixel = _scales->vert().to_pixel(y_coord);
		_magnifier.move(x_pixel, y_pixel);
		Magnify();
	}
}

bool IsValidCharacter(char temp) {
	return isdigit(temp);
}

void Insert(TextEntry& info, char payload) {
	if (info.str.length() < info.max_length) {
		if (info.pos == info.str.length())
			info.str.append(1, payload);
		else
			info.str.insert(info.pos, 1, payload);

		info.pos = info.pos + 1;
	}
}

void Backspace(TextEntry& info) {
	if (info.str.length() > 0)
		if (info.pos > 0)
			info.str.erase(--info.pos, 1);
}

void Delete(TextEntry& info) {
	if (info.str.length() > 0) {
		if (info.pos >= 0)
			info.str.erase(info.pos, 1);

		if (info.pos > info.str.length())
			info.pos = info.pos - 1;
	}
}

void PositionStart(TextEntry& info) {
	info.pos = 0;
}

void PositionEnd(TextEntry& info) {
	info.pos = info.str.length() > 0 ? info.str.length() : 0;
}

void PositionForward(TextEntry& info) {
	if (info.str.length() > 0 && info.pos < info.str.length())
		info.pos = info.pos + 1;
}

void PositionBack(TextEntry& info) {
	if (info.pos > 0)
		info.pos = info.pos - 1;
}

int_t ToInteger(TextEntry& info) {
	return info.str.length() > 0 ? std::stoll(info.str) : 0;
}

bool Application::EnterNewMaximum(int_t& max)
{
	bool maximumChanged = false;
	bool acceptingInput = true;
	bool prevState = Renderer::Threads::paused;
	Renderer::Threads::paused = true;

	InputBox inputOverlay(_font);
	std::string promptMsg = "Enter new maximum: ";
	std::string helpMsg = "\nD for default";

	TextEntry info;
	int_t currentMax;
	char temp;

	while (_window.isOpen() && acceptingInput) {
		while (PollNext()) {
			switch (event.type) {
			case sf::Event::Closed:
				_window.close();
				acceptingInput = false;
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Key::Escape:
					acceptingInput = false;
					break;
				case sf::Keyboard::Key::D:
					info.str = std::to_string(DEFAULT_MAX_ITERATIONS);
					inputOverlay.set(promptMsg + info.str + helpMsg);
					PositionEnd(info);
					break;
				case sf::Keyboard::Key::Up:
					currentMax = ToInteger(info);

					if (currentMax < (int_t)pow(10, info.max_length + 1)) {
						currentMax = ToInteger(info);
						info.str = std::to_string(currentMax + 1);
						inputOverlay.set(promptMsg + info.str + helpMsg);
					}

					break;
				case sf::Keyboard::Key::Down:
					currentMax = ToInteger(info);

					if (currentMax > 0) {
						info.str = std::to_string(currentMax - 1);
						inputOverlay.set(promptMsg + info.str + helpMsg);
					}

					break;
				case sf::Keyboard::Key::Left:
					PositionBack(info);
					break;
				case sf::Keyboard::Key::Right:
					PositionForward(info);
					break;
				case sf::Keyboard::Key::Home:
					PositionStart(info);
					break;
				case sf::Keyboard::Key::End:
					PositionEnd(info);
					break;
				case sf::Keyboard::Key::Delete:
					Delete(info);
					inputOverlay.set(promptMsg + info.str + helpMsg);
					break;
				case sf::Keyboard::Key::Enter:
					maximumChanged = info.str.length() > 0;
					acceptingInput = false;
					break;
				}

				break;
			case sf::Event::TextEntered:
				temp = (char)event.text.unicode;

				if (temp == '\b')
					Backspace(info);
				else if (IsValidCharacter(temp))
					Insert(info, temp);

				inputOverlay.set(promptMsg + info.str + helpMsg);
				break;
			}
		}

		Update();
		Clear();
		Draw();
		inputOverlay.draw_to(_window);
		Show();
	}

	if (maximumChanged)
		max = std::stoll(info.str);

	Renderer::Threads::paused = prevState;
	return maximumChanged;
}

bool Application::PollCoordinates(pair_t& coords)
{
	bool acceptingInput = true;
	bool actionConfirmed = false;
	bool prevState = Renderer::Threads::paused;
	Renderer::Threads::paused = true;

	InputBox inputOverlay(_font);
	inputOverlay.set("Click on a point\n[Esc] to cancel");

	TextEntry info;
	int_t currentMax;
	char temp;

	while (_window.isOpen() && acceptingInput) {
		while (PollNext()) {
			switch (event.type) {
			case sf::Event::Closed:
				_window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Key::Escape:
					acceptingInput = false;
					break;
				}

				break;
			case sf::Event::MouseButtonPressed:
				switch (event.mouseButton.button) {
				case sf::Mouse::Left:
					if (MouseInView(_window)) {
						coords.first = _scales->coord_x((int_t)sf::Mouse::getPosition(_window).x);
						coords.secnd = _scales->coord_y((int_t)sf::Mouse::getPosition(_window).y);
						acceptingInput = false;
						actionConfirmed = true;
					}

					break;
				}

				break;
			}
		}

		Update();
		Clear();
		Draw();
		inputOverlay.draw_to(_window);
		Show();
	}

	Renderer::Threads::paused = prevState;
	return actionConfirmed;
}

void Application::Render() {
	Renderer r(std::ref(_image), std::ref(_main_overlay), state());
	r.Start();
}

void Application::StartRenderAsync() {
	_image.create(state().view.right, state().view.bottom, INIT_COLOR);
	Renderer::Threads::rendering = true;
	_render_thread = std::thread(&Application::Render, this);
}

void Application::StopRenderAsync() {
	Renderer::Threads::rendering = false;

	if (_render_thread.joinable())
		_render_thread.join();
}

void Application::StartTimedMessageAsync(const std::string& message, int_t seconds) {
	main_overlay().notification(message);

	if (_clock_thread.joinable()) {
		notifying = false;
		_clock_thread.join();
	}

	_clock_thread = std::thread([=]() {
		auto msg = message;
		auto duration = seconds;
		notifying = true;
		main_overlay().notification(msg);

		for (int i = duration; notifying && i > 0; --i)
			std::this_thread::sleep_for(std::chrono::seconds(1));

		if (notifying)
			main_overlay().notification("");
	});
}

void Application::StartPollDelayAsync() {
	std::thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(Application::POLLING_DELAY_MSEC));
		Application::delay_next_poll = false;
	}).detach();
}
