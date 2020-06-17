#include "os.h"
#include "Application.h"
#include "verdanab_ttf.hpp"

const int_t WIDTH_PIXELS = 1200;
const int_t HEIGHT_PIXELS = 900;

const char* const HELP_MSG =
	"\n\n\n\n\n\n\nHELP"
	"\n"
	"\nT : Toggle Display"
	"\nP : Pause Render"
	"\nR : Restart Render"
	"\nI : Change Max Iterations"
	"\nJ : Select Julia Set"
	"\nM : Back to Mandelbrot Set"
	"\n"
	"\nEsc : Back to Default Magnification"
	"\n"
	"\nCtl + S : Save Screen Capture"
	"\nV : Start a Recorded Render"
	"\n"
	"\nLeft Click : Zoom In"
	"\nRight Click : Zoom Out"
	"\n"
	"\nAlt + Left : Go Back in History"
	"\nAlt + Right : Go Forward in History"
	"\n"
	"\nPgUp - PgDn : Change Function"
	"\nUp - Down : Change Algorithm"
	"\nLeft - Right : Change Color Scheme"
	"\n[ - ] : Change Threshold"
;

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
	sf::Event event;

	while (app.IsOpen()) {
		while (app.PollNext(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				app.Close();
				break;
			case sf::Event::Resized:
				app.RebuildGeometry();
				app.RebuildMagnifier();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9)
					app.ChangeOverlayAndHistory([&]() {
						app.current_state.new_power(TO_INT(event.key.code) - TO_INT(sf::Keyboard::Num0));
					});
				else
					switch (event.key.code) {
					case sf::Keyboard::Key::Escape:
						app.ChangeOverlayAndHistory([&]() {
							app.current_state.init_model_stack().init_magnification();
						});
						break;
					case sf::Keyboard::Key::H:
						app.ToggleHelpMessage(HELP_MSG);
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::I:
						{
							int_t newMaximum;

							if (app.EnterNewMaximum(newMaximum))
								app.ChangeOverlayAndHistory([&]() {
									app.current_state.new_max_iterations(newMaximum);
								});
						}

						break;
					case sf::Keyboard::Key::J:
						{
							auto temp = app.current_state.j_coords;
							
							if (app.EnterNewCoordinates(temp))
								app.ChangeOverlayAndHistory([&]() {
									app.Julia(temp);
								});
						}

						break;
					case sf::Keyboard::Key::M:
						if (app.current_state.type != mnd::MANDELBROT)
							app.ChangeOverlayAndHistory([&]() {
								app.Mandelbrot();
							});

						break;
					case sf::Keyboard::Key::P:
						app.TogglePauseRender();
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::R:
						app.RestartRender();
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
					case sf::Keyboard::Key::V:
						if (Renderer::Threads::paused)
							app.TogglePauseRender();

						app.StopRenderAsync();
						app.StartRecordingRenderAsync();
						Application::delay_next_poll = true;
						break;
					case sf::Keyboard::Key::PageUp:
						app.ChangeOverlayAndHistory([&]() {
							app.current_state.next_power();
						});
						break;
					case sf::Keyboard::Key::PageDown:
						if (app.current_state.power > min_power())
							app.ChangeOverlayAndHistory([&]() {
								app.current_state.prev_power();
							});

						break;
					case sf::Keyboard::Key::Up:
						app.ChangeOverlayAndHistory([&]() {
							app.current_state.prev_algorithm();
						});
						break;
					case sf::Keyboard::Key::Down:
						app.ChangeOverlayAndHistory([&]() {
							app.current_state.next_algorithm();
						});
						break;
					case sf::Keyboard::Key::LBracket:
						app.ChangeOverlayAndHistory([&]() {
							app.current_state.prev_threshold();
						});
						break;
					case sf::Keyboard::Key::RBracket:
						app.ChangeOverlayAndHistory([&]() {
							app.current_state.next_threshold();
						});
						break;
					case sf::Keyboard::Key::Left:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)) {
							if (app.GoToPreviousState())
								app.ChangeOverlay([&]() {
									switch (app.current_state.type) {
									case mnd::MANDELBROT:
										app.Mandelbrot();
										break;
									case mnd::JULIA:
										app.Julia(app.current_state.j_coords);
										break;
									}
								});
						}
						else {
							app.ChangeOverlayAndHistory([&]() {
								app.current_state.prev_color_scheme();
							});
						}
						
						break;
					case sf::Keyboard::Key::Right:
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)) {
							if (app.GoToNextState())
								app.ChangeOverlay([&]() {
									switch (app.current_state.type) {
									case mnd::MANDELBROT:
										app.Mandelbrot();
										break;
									case mnd::JULIA:
										app.Julia(app.current_state.j_coords);
										break;
									}
								});
						}
						else {
							app.ChangeOverlayAndHistory([&]() {
								app.current_state.next_color_scheme();
							});
						}

						break;
					}

				break;
			case sf::Event::MouseButtonPressed:
				switch (event.mouseButton.button) {
				case sf::Mouse::Left:
					if (MouseInView(app.canvas()))
						app.ChangeOverlayAndHistory([&]() {
							app.Magnify();
						});

					break;
				case sf::Mouse::Right:
					if (MouseInView(app.canvas()) && app.current_state.models.size() > 1)
						app.ChangeOverlayAndHistory([&]() {
							app.Demagnify();
						});

					break;
				}

				break;
			}

			if (Application::delay_next_poll)
				app.StartPollDelayAsync();
		}

		app.Update();
		app.Clear();
		app.Draw();
		app.Show();
	}

	return 0;
}
