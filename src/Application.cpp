#include "Application.h"

const int_t Application::MSG_DURATION_SEC = 3LL;
const int_t Application::POLLING_DELAY_MSEC = 250LL;
const char* Application::DEFAULT_EXTENSION = ".png";

volatile std::atomic<bool> Application::delay_next_poll;
volatile std::atomic<bool> Application::notifying;

Application::Application(font_t& font, int_t width_pixels, int_t height_pixels, const std::string& title) :
	_font(font),
	_scales(std::make_shared<Geometry2D>()),
	_window(
		sf::VideoMode(
			width_pixels, 
			height_pixels, 
			sf::VideoMode::getDesktopMode().bitsPerPixel
		), 
		title
	),
	_titlebar_height(_window.getSize().y - height_pixels),
	_states(State(width_pixels, height_pixels)),
	_main_overlay(font, _scales, _window),
	_magnifier(_window),
	_show_overlay(true),
	_show_help(false)
{
	_main_overlay.endnote(DEFAULT_END_NOTE);
	RebuildGeometry();
	Threads::Add(_render_thread);
	Threads::Add(_clock_thread);
}

Application::~Application() {
	Renderer::Threads::rendering = false;
	Threads::Join();
}

const canvas_t& Application::canvas() const {
	return _window;
}

const Geometry2D& Application::scales() const {
	return *_scales;
}

const sf::Image& Application::image() const {
	return _image;
}

void Application::Mandelbrot() {
	current_state
		.new_type(mnd::MANDELBROT);
}

void Application::Julia(pair_t coordinates) {
	current_state
		.new_type(mnd::JULIA)
		.new_j_coords(coordinates);
}

void Application::PushHistory() {
	_states.add(current_state);
}

void Application::PushOverlay() {
	_main_overlay.state(current_state);
}

void Application::ChangeState(std::function<void()> action_f) {
	if (Renderer::Threads::paused)
		TogglePauseRender();

	StopRenderAsync();
	action_f();
	StartRenderAsync();
	Application::delay_next_poll = true;
}

void Application::RestartRender() {
	ChangeState([]() {});
}

void Application::ChangeOverlay(std::function<void()> action_f) {
	ChangeState(action_f);
	PushOverlay();
}

void Application::ChangeOverlayAndHistory(std::function<void()> action_f) {
	ChangeOverlay(action_f);
	PushHistory();
}

bool Application::GoToPreviousState() {
	if (_states.prev()) {
		current_state = _states.current();
		return true;
	}
	
	return false;
}

bool Application::GoToNextState() {
	if (_states.next()) {
		current_state = _states.current();
		return true;
	}

	return false;
}

void Application::RebuildGeometry() {
	*_scales = Geometry2D(current_state.view, current_state.models.top());
}

void Application::RebuildMagnifier() {
	_magnifier = TrackingBox(_window);
}

bool Application::PollNext(sf::Event& e) {
	return !Application::delay_next_poll && _window.pollEvent(e);
}

void Application::Magnify() {
	current_state
		.push_model(_magnifier.get_boundaries(*_scales))
		.next_magnification();

	RebuildGeometry();
}

void Application::Demagnify() {
	current_state
		.pop_model()
		.prev_magnification();

	RebuildGeometry();
}

void Application::Update() {
	_texture.loadFromImage(_image);
	_sprite.setTexture(_texture);
	_main_overlay.update(_window);
	_magnifier.update(_window);
}

void Application::Clear() {
	_window.clear();
}

void Application::Draw() {
	_window.draw(_sprite);

	if (_show_overlay) {
		_main_overlay.draw_to(_window);
		_magnifier.draw_to(_window);
	}
}

void Application::Show() {
	_window.display();
}

pair_t Application::GetCenterCoords() const {
	auto left = current_state.models.top().left;
	auto top = current_state.models.top().top;
	return pair_t{
		left + abs(left - current_state.models.top().right) / 2,
		top + abs(top - current_state.models.top().bottom) / 2
	};
}

// Format:
// 
//    yyyy_MM_dd_HHmmss_-_power_magnification_iteration_x_y
//    
std::string Application::NewFileName(std::string extension) const {
	int_t type = current_state.type;
	int_t iteration = _main_overlay.iteration();
	int_t power = current_state.power;
	int_t magnification = current_state.magnification;
	pair_t j_coords = current_state.j_coords;
	auto center = GetCenterCoords();
	std::ostringstream buf;

	buf << type << '_';
	put_int(buf, power);

	auto fill = buf.fill();

	buf << '_'
		<< std::hex << magnification << '_'
		<< std::setfill('0')
		<< std::setw(std::to_string(current_state.max_iterations).length())
		<< std::dec << iteration << '_'
		<< std::setfill(fill);

	put_flt(buf, center.re());
	buf << '_';
	put_flt(buf, center.im());

	if (type == mnd::JULIA) {
		buf << '_';
		put_flt(buf, j_coords.re());
		buf << '_';
		put_flt(buf, j_coords.im());
	}

	return GetDateTimeString() + "_-_" + buf.str() + extension;
}

bool Application::Save(sf::Sprite& someSprite, const sf::Image& someImage) {
	sf::Texture someTexture;
	someTexture.loadFromImage(someImage);
	someSprite.setTexture(someTexture);
	return Save(someSprite);
}

bool Application::Save(sf::Sprite& someSprite) {
	sf::RenderTexture target;
	target.setActive(true);

	if (!target.create(current_state.view.right, current_state.view.bottom))
		return false;

	target.display();
	target.draw(someSprite);
	
	if (_show_overlay)
		_main_overlay.draw_static_to(target);

	target
		.getTexture()
		.copyToImage()
		.saveToFile(NewFileName());

	target.setActive(false);
	return true;
}

bool Application::Save() {
	auto temp = Renderer::Threads::paused;
	Renderer::Threads::paused = true;
	auto success = Save(_sprite);
	Renderer::Threads::paused = temp;
	return success;
}

void Application::GoTo(const std::string& str) {
	std::stringstream buf(str);
	std::string temp;

	buf.ignore(std::numeric_limits<std::streamsize>::max(), '-');
	
	if (!buf || buf.eof())
		buf = std::stringstream(str);
	else
		buf.ignore(1, '_');

	getline(buf, temp, '_');
	int_t type = std::stoll(temp);

	getline(buf, temp, '_');
	int_t power = get_int(temp);
	
	getline(buf, temp, '_');
	int_t magnification = std::stoll(temp);

	getline(buf, temp, '_');
	int_t iteration = std::stoll(temp);

	getline(buf, temp, '_');
	flt_t coord_x = get_flt(temp);

	getline(buf, temp, type == mnd::JULIA ? '_' : '.');
	flt_t coord_y = get_flt(temp);

	current_state
		.new_type(type)
		.new_power(power)
		.init_magnification()
		.init_max_iterations();

	if (type == mnd::JULIA) {
		getline(buf, temp, '_');
		flt_t j_coord_x = get_flt(temp);

		getline(buf, temp, '.');
		flt_t j_coord_y = get_flt(temp);

		current_state
			.new_j_coords(pair_t{ j_coord_x, j_coord_y });
	}

	RebuildGeometry();

	while (magnification < current_state.magnification) {
		_magnifier.move(
			_scales->horz().to_pixel(coord_x),
			_scales->vert().to_pixel(coord_y)
		);
		Demagnify();
	}

	while (magnification > current_state.magnification) {
		_magnifier.move(
			_scales->horz().to_pixel(coord_x),
			_scales->vert().to_pixel(coord_y)
		);
		Magnify();
	}

	_main_overlay.state(current_state);
}

bool Application::EnterNewMaximum(int_t& max) {
	bool maximumChanged = false;
	bool acceptingInput = true;
	bool prevState = Renderer::Threads::paused;
	Renderer::Threads::paused = true;

	InputBox inputOverlay(_font);
	std::string promptMsg = "Enter new maximum: ";
	std::string helpMsg =
		"\nD for default"
		"\n[Esc] to cancel";

	TextEntry info;
	int_t currentMax;
	char temp;
	sf::Event event;

	while (IsOpen() && acceptingInput) {
		while (PollNext(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				Close();
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
					info.PositionEnd();
					break;
				case sf::Keyboard::Key::Up:
					currentMax = info.ToInteger();

					if (currentMax < TO_INT(pow(10, info.max_length + 1))) {
						currentMax = info.ToInteger();
						info.str = std::to_string(currentMax + 1);
						inputOverlay.set(promptMsg + info.str + helpMsg);
					}

					break;
				case sf::Keyboard::Key::Down:
					currentMax = info.ToInteger();

					if (currentMax > 0) {
						info.str = std::to_string(currentMax - 1);
						inputOverlay.set(promptMsg + info.str + helpMsg);
					}

					break;
				case sf::Keyboard::Key::Left:
					info.PositionBack();
					break;
				case sf::Keyboard::Key::Right:
					info.PositionForward();
					break;
				case sf::Keyboard::Key::Home:
					info.PositionStart();
					break;
				case sf::Keyboard::Key::End:
					info.PositionEnd();
					break;
				case sf::Keyboard::Key::Delete:
					info.Delete();
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
					info.Backspace();
				else if (isdigit(temp))
					info.Insert(temp);

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

bool Application::EnterNewCoordinates(pair_t& coords) {
	bool acceptingInput = true;
	bool actionConfirmed = false;
	bool prevState = Renderer::Threads::paused;
	Renderer::Threads::paused = true;

	InputBox inputOverlay(_font);
	inputOverlay.set(
		"Click on a point"
		"\n[Esc] to cancel"
	);

	TextEntry info;
	sf::Event event;

	while (IsOpen() && acceptingInput) {
		while (PollNext(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				Close();
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
						coords = pair_t(
							_scales->coord_x(TO_INT(sf::Mouse::getPosition(_window).x)),
							_scales->coord_y(TO_INT(sf::Mouse::getPosition(_window).y))
						);
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

void Application::StartRenderAsync() {
	_main_overlay.rendering_msg("Rendering...");
	_image.create(current_state.view.right, current_state.view.bottom, mnd::INIT_COLOR);
	_render_thread = std::thread(
		[i = std::ref(_image), o = std::ref(_main_overlay), s = current_state]() {
			Renderer r(i, o, s);
			r.Run();
			o.get().rendering_msg("");
		}
	);
}

void Application::StartRecordingRenderAsync() {
	_main_overlay.rendering_msg("Recording...");
	_image.create(current_state.view.right, current_state.view.bottom, mnd::INIT_COLOR);
	_render_thread = std::thread(
		[&, i = std::ref(_image), o = std::ref(_main_overlay), s = current_state]() {
			Renderer r(i, o, s);

			sf::Sprite someSprite;
			auto someImage = r.Start();

			while (r.HasNext()) {
				r.Next(someImage);
				Save(someSprite, i.get());
			}

			r.Close();
			o.get().rendering_msg("");
		}
	);
}

void Application::StopRenderAsync() {
	Renderer::Threads::rendering = false;
	_main_overlay.rendering_msg("");

	if (_render_thread.joinable())
		_render_thread.join();
}

void Application::StartTimedMessageAsync(const std::string& message, int_t seconds) {
	_main_overlay.notification(message);

	if (_clock_thread.joinable()) {
		notifying = false;
		_clock_thread.join();
	}

	_clock_thread = std::thread([=]() {
		auto msg = message;
		auto duration = seconds;
		notifying = true;
		_main_overlay.notification(msg);

		for (int_t i = duration; notifying && i > 0; --i)
			std::this_thread::sleep_for(std::chrono::seconds(DELAY_INTERVAL_SEC));

		if (notifying)
			_main_overlay.notification("");
	});
}

void Application::StartPollDelayAsync() {
	std::thread([=]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(Application::POLLING_DELAY_MSEC));
		Application::delay_next_poll = false;
	}).detach();
}

bool Application::ToggleOverlay() {
	_show_overlay = !_show_overlay;
	return _show_overlay;
}

bool Application::TogglePauseRender() {
	Renderer::Threads::paused = !Renderer::Threads::paused;

	if (Renderer::Threads::paused) {
		Application::notifying = false;
		_main_overlay.notification("Paused.");
	}
	else {
		StartTimedMessageAsync("Unpaused.");
	}

	return Renderer::Threads::paused;
}

bool Application::ToggleHelpMessage(const char* msg) {
	_show_help = !_show_help;

	if (_show_help)
		_main_overlay
			.notification(msg)
			.endnote("");
	else
		_main_overlay
			.notification("")
			.endnote(DEFAULT_END_NOTE);

	return _show_help;
}

bool Application::IsOpen() const {
	return _window.isOpen();
}

void Application::Close() {
	_window.close();
}
