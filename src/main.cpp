#include "Application.h"
#include "verdanab_ttf.hpp"

int main(int argc, char** argv) {
	font_t myFont(&__verdanab_ttf, __verdanab_ttf_len);
	Application app(myFont);
	bool change_history;
	bool change_overlay;

	if (argc > 1)
		app.GoTo(argv[1]);

	app.StartRenderAsync();

	while (app._window.isOpen()) {
		change_history = false;
		change_overlay = false;

		while (app.PollNext()) {
			switch (app.event.type) {
			case sf::Event::Closed:
				app._window.close();
				break;
			case sf::Event::KeyPressed:
				if (app.event.key.code >= sf::Keyboard::Num0 && app.event.key.code <= sf::Keyboard::Num9) {
					app.StopRenderAsync();
					app.state().new_power((int_t)app.event.key.code - (int_t)sf::Keyboard::Num0);
					app.StartRenderAsync();
					change_history = true;
					change_overlay = true;
					Application::delay_next_poll = true;
				}
				else {
					switch (app.event.key.code) {
					case sf::Keyboard::Key::Escape:
						app.StopRenderAsync();
						app.state().init_model_stack().init_magnification();
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
								app.state().new_max_iterations(newMaximum);
								app.StopRenderAsync();
								app.StartRenderAsync();
								change_history = true;
								change_overlay = true;
							}
						}

						break;
					case sf::Keyboard::Key::J:
						{
							auto temp = app.state().j_coords;
							
							if (app.PollCoordinates(temp))
							{
								app.StopRenderAsync();
								app.Julia(temp);
								app.StartRenderAsync();
								change_history = true;
								change_overlay = true;
							}
						}
						
						break;
					case sf::Keyboard::Key::P:
						Renderer::Threads::paused = !Renderer::Threads::paused;

						if (Renderer::Threads::paused) {
							Application::notifying = false;
							app.main_overlay().notification("Paused.");
						}
						else {
							app.StartTimedMessageAsync("Unpaused.");
						}

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
						app._show_overlay = !app._show_overlay;
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::PageUp:
						app.StopRenderAsync();
						app.state().next_power();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::PageDown:
						if (app.state().power > min_power()) {
							app.StopRenderAsync();
							app.state().prev_power();
							app.StartRenderAsync();
							change_history = true;
							change_overlay = true;
							Application::delay_next_poll = true;
						}

						break;
					case sf::Keyboard::Key::Up:
						app.StopRenderAsync();
						app.state().prev_algorithm();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::Down:
						app.StopRenderAsync();
						app.state().next_algorithm();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::Left:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)) {
							if (app.GoToPreviousState()) {
								app.StopRenderAsync();

								switch (app.state().type) {
								case MANDELBROT:
									app.Mandelbrot();
									break;
								case JULIA:
									app.Julia(app.state().j_coords);
									break;
								}

								app.StartRenderAsync();
								change_overlay = true;
								Application::delay_next_poll = true;
							}
						}
						else {
							app.StopRenderAsync();
							app.state().prev_color_scheme();
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

								switch (app.state().type) {
								case MANDELBROT:
									app.Mandelbrot();
									break;
								case JULIA:
									app.Julia(app.state().j_coords);
									break;
								}

								app.StartRenderAsync();
								change_overlay = true;
								Application::delay_next_poll = true;
							}
						}
						else {
							app.StopRenderAsync();
							app.state().next_color_scheme();
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
				switch (app.event.mouseButton.button) {
				case sf::Mouse::Left:
					if (MouseInView(app._window)) {
						app.StopRenderAsync();
						app.Magnify();
						app.StartRenderAsync();
						change_history = true;
						change_overlay = true;
						Application::delay_next_poll = true;
					}

					break;
				case sf::Mouse::Right:
					if (MouseInView(app._window) && app.state().models.size() > 1) {
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
