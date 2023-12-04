//
// Main GUI window of the calculator.
//
// Copyright (c) 2023 Serge Vakulenko <serge.vakulenko@gmail.com>
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//
#pragma once
#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/grid.h>
#include <gtkmm/window.h>

#include <string>

class Calculator;

class Main_Window : public Gtk::Window {
public:
    Main_Window();
    virtual ~Main_Window();

private:
    std::unique_ptr<Calculator> calc;

protected:
    Gtk::Button one_button;
    Gtk::Button two_button;
    Gtk::Button three_button;
    Gtk::Button four_button;
    Gtk::Button five_button;
    Gtk::Button six_button;
    Gtk::Button seven_button;
    Gtk::Button eight_button;
    Gtk::Button nine_button;
    Gtk::Button zero_button;
    Gtk::Button dot_button;
    Gtk::Button plus_button;
    Gtk::Button minus_button;
    Gtk::Button multiply_button;
    Gtk::Button divide_button;
    Gtk::Button clear_button;
    Gtk::Button ce_button;
    Gtk::Button equal_buttton;

    Gtk::Label display;
    Gtk::Grid grid;
};

#endif // _MAIN_WINDOW_H_
