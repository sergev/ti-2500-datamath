#include "main_window.h"

#include <iostream>

#include "calculator.h"

Main_Window::Main_Window()
    : one_button("1"), two_button("2"), three_button("3"), four_button("4"), five_button("5"),
      six_button("6"), seven_button("7"), eight_button("8"), nine_button("9"), zero_button("0"),
      dot_button("•"), plus_button("+"), minus_button("-"), multiply_button("×"),
      divide_button("÷"), clear_button("C"), ce_button("CE"), equal_buttton("=")
{
    set_title("TI-2500 Datamath");
    set_resizable(false);

    //
    // Set display alignment and font.
    //
    display.set_halign(Gtk::Align::END);

    auto context = display.get_pango_context();
    auto font = context->get_font_description();
    font.set_family("Monospace");
    font.set_size(32 * PANGO_SCALE);
    context->set_font_description(font);

    //
    // Connect actions to buttons.
    //
    one_button.signal_clicked().connect([this] {
        calc->press_key('1');
        display.set_text(calc->get_display());
    });
    two_button.signal_clicked().connect([this] {
        calc->press_key('2');
        display.set_text(calc->get_display());
    });
    three_button.signal_clicked().connect([this] {
        calc->press_key('3');
        display.set_text(calc->get_display());
    });
    four_button.signal_clicked().connect([this] {
        calc->press_key('4');
        display.set_text(calc->get_display());
    });
    five_button.signal_clicked().connect([this] {
        calc->press_key('5');
        display.set_text(calc->get_display());
    });
    six_button.signal_clicked().connect([this] {
        calc->press_key('6');
        display.set_text(calc->get_display());
    });
    seven_button.signal_clicked().connect([this] {
        calc->press_key('7');
        display.set_text(calc->get_display());
    });
    eight_button.signal_clicked().connect([this] {
        calc->press_key('8');
        display.set_text(calc->get_display());
    });
    nine_button.signal_clicked().connect([this] {
        calc->press_key('9');
        display.set_text(calc->get_display());
    });
    zero_button.signal_clicked().connect([this] {
        calc->press_key('0');
        display.set_text(calc->get_display());
    });
    dot_button.signal_clicked().connect([this] {
        calc->press_key('.');
        display.set_text(calc->get_display());
    });
    plus_button.signal_clicked().connect([this] {
        calc->press_key('+');
        display.set_text(calc->get_display());
    });
    minus_button.signal_clicked().connect([this] {
        calc->press_key('-');
        display.set_text(calc->get_display());
    });
    multiply_button.signal_clicked().connect([this] {
        calc->press_key('*');
        display.set_text(calc->get_display());
    });
    divide_button.signal_clicked().connect([this] {
        calc->press_key('/');
        display.set_text(calc->get_display());
    });
    clear_button.signal_clicked().connect([this] {
        calc->press_key('c');
        display.set_text(calc->get_display());
    });
    ce_button.signal_clicked().connect([this] {
        calc->press_key('e');
        display.set_text(calc->get_display());
    });
    equal_buttton.signal_clicked().connect([this] {
        calc->press_key('=');
        display.set_text(calc->get_display());
    });

    //
    // ┌─────────────────────┐
    // │       Display       │
    // └─────────────────────┘
    // ┌───┐ ┌───┐ ┌───┐
    // │ CE│ │ ÷ │ │ × │
    // └───┘ └───┘ └───┘
    // ┌───┐ ┌───┐ ┌───┐ ┌───┐
    // │ 7 │ │ 8 │ │ 9 │ │ C │
    // └───┘ └───┘ └───┘ └───┘
    // ┌───┐ ┌───┐ ┌───┐ ┌───┐
    // │ 4 │ │ 5 │ │ 6 │ │ – │
    // └───┘ └───┘ └───┘ └───┘
    // ┌───┐ ┌───┐ ┌───┐ ┌───┐
    // │ 1 │ │ 2 │ │ 3 │ │ + │
    // └───┘ └───┘ └───┘ └───┘
    // ┌──────┐ ┌───┐ ┌──────┐
    // │  0   │ │ • │ │  =   │
    // └──────┘ └───┘ └──────┘
    //
    grid.attach(display, 0, 0, 4);

    grid.attach(ce_button, 0, 1);
    grid.attach(divide_button, 1, 1);
    grid.attach(multiply_button, 2, 1);

    grid.attach(seven_button, 0, 2);
    grid.attach(eight_button, 1, 2);
    grid.attach(nine_button, 2, 2);
    grid.attach(clear_button, 3, 2);

    grid.attach(four_button, 0, 3);
    grid.attach(five_button, 1, 3);
    grid.attach(six_button, 2, 3);
    grid.attach(minus_button, 3, 3);

    grid.attach(one_button, 0, 4);
    grid.attach(two_button, 1, 4);
    grid.attach(three_button, 2, 4);
    grid.attach(plus_button, 3, 4);

    grid.attach(zero_button, 0, 5, 2);
    grid.attach(dot_button, 2, 5);
    grid.attach(equal_buttton, 3, 5);

    set_child(grid);

    //
    // Instantiate the calculator model.
    //
    calc = std::make_unique<Calculator>();
    calc->run();
    display.set_text(calc->get_display());
}

Main_Window::~Main_Window()
{
    // Empty.
}
