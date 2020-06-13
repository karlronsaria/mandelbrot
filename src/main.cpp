#include "os.h"
#include "Application.h"
#include "verdanab_ttf.hpp"

const int_t WIDTH_PIXELS = 1200;
const int_t HEIGHT_PIXELS = 900;

#if defined(WINDOWS) && !defined(_CONSOLE)
#include <Windows.h>

int wWinMain(HINSTANCE h, HINSTANCE prev_h, PWSTR argv, int n_cmd_show)
{
	std::vector<std::string> args;

	{
		int argc;
		auto lpwstr = CommandLineToArgvW(GetCommandLine(), &argc);
		for (int i = 0; i < argc; ++i) {
			auto wstr = std::wstring(lpwstr[i]);
			args.push_back(std::string(wstr.begin(), wstr.end()));
		}
	}

#else

int main(int argc, char** argv)
{
	std::vector<std::string> args;

	for (int i = 0; i < argc; ++i)
		args.push_back(std::string(argv[i]));

#endif
	
	font_t myFont(&__verdanab_ttf, __verdanab_ttf_len);
	Application app(myFont, WIDTH_PIXELS, HEIGHT_PIXELS, args[0]);

	if (args.size() > 1)
		app.GoTo(args[1]);

	app.StartRenderAsync();

	bool change_history;
	bool change_overlay;
	sf::Event event;

	while (app.IsOpen()) {
		change_history = false;
		change_overlay = false;

		while (app.PollNext(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				app.Close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9) {
					app.StopRenderAsync();
					app.current_state.new_power(TO_INT(event.key.code) - TO_INT(sf::Keyboard::Num0));
					app.StartRenderAsync();
					change_history = true;
					change_overlay = true;
					Application::delay_next_poll = true;
				}
				else {
					switch (event.key.code) {
					case sf::Keyboard::Key::Escape:
						app.StopRenderAsync();
						app.current_state.init_model_stack().init_magnification();
						app.Mandelbrot();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::I:
						{
							int_t newMaximum;

							if (app.EnterNewMaximum(newMaximum)) {
								app.current_state.new_max_iterations(newMaximum);
								app.StopRenderAsync();
								app.StartRenderAsync();
								change_history = true;
								change_overlay = true;
							}
						}

						break;
					case sf::Keyboard::Key::J:
						{
							auto temp = app.current_state.j_coords;
							
							if (app.EnterNewCoordinates(temp)) {
								app.StopRenderAsync();
								app.Julia(temp);
								app.StartRenderAsync();
								change_history = true;
								change_overlay = true;
							}
						}
						
						break;
					case sf::Keyboard::Key::P:
						app.TogglePauseRender();
						break;
					case sf::Keyboard::Key::R:
						app.StopRenderAsync();
						app.StartRenderAsync();
						break;
					case sf::Keyboard::Key::S:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
							app.Save();
							app.StartTimedMessageAsync("Saved screen capture.");
							Application::delay_next_poll = true;
						}

						break;
					case sf::Keyboard::Key::T:
						app.ToggleOverlay();
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::PageUp:
						app.StopRenderAsync();
						app.current_state.next_power();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::PageDown:
						if (app.current_state.power > min_power()) {
							app.StopRenderAsync();
							app.current_state.prev_power();
							app.StartRenderAsync();
							change_history = true;
							change_overlay = true;
							Application::delay_next_poll = true;
						}

						break;
					case sf::Keyboard::Key::Up:
						app.StopRenderAsync();
						app.current_state.prev_algorithm();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::Down:
						app.StopRenderAsync();
						app.current_state.next_algorithm();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::Left:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)) {
							if (app.GoToPreviousState()) {
								app.StopRenderAsync();

								switch (app.current_state.type) {
								case MANDELBROT:
									app.Mandelbrot();
									break;
								case JULIA:
									app.Julia(app.current_state.j_coords);
									break;
								}

								app.StartRenderAsync();
								change_overlay = true;
								Application::delay_next_poll = true;
							}
						}
						else {
							app.StopRenderAsync();
							app.current_state.prev_color_scheme();
							app.StartRenderAsync();
							change_history = true;
							change_overlay = true;
							Application::delay_next_poll = true;
						}
						
						break;
					case sf::Keyboard::Key::Right:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)) {
							if (app.GoToNextState()) {
								app.StopRenderAsync();

								switch (app.current_state.type) {
								case MANDELBROT:
									app.Mandelbrot();
									break;
								case JULIA:
									app.Julia(app.current_state.j_coords);
									break;
								}

								app.StartRenderAsync();
								change_overlay = true;
								Application::delay_next_poll = true;
							}
						}
						else {
							app.StopRenderAsync();
							app.current_state.next_color_scheme();
							app.StartRenderAsync();
							change_history = true;
							change_overlay = true;
							Application::delay_next_poll = true;
						}

						break;
					}
				}

				break;
			case sf::Event::MouseButtonPressed:
				switch (event.mouseButton.button) {
				case sf::Mouse::Left:
					if (MouseInView(app.canvas())) {
						app.StopRenderAsync();
						app.Magnify();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
					}

					break;
				case sf::Mouse::Right:
					if (MouseInView(app.canvas()) && app.current_state.models.size() > 1) {
						app.StopRenderAsync();
						app.Demagnify();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
					}

					break;
				}

				break;
			}

			if (Application::delay_next_poll)
				app.StartPollDelayAsync();
		}

		if (change_history)
			app.PushHistory();

		if (change_overlay)
			app.PushOverlay();

		app.Update();
		app.Clear();
		app.Draw();
		app.Show();
	}

	return 0;
}
