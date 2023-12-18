/**
 * @file WxCalcWindow.cpp
 * @brief The implementation of the window class for the calculator.
 * @author Dominique LaSalle <dominique@solidlake.com>
 * Copyright 2017, Solid Lake LLC
 * @version 1
 * @date 2017-03-28
 */

#include "WxCalcWindow.hpp"

#include <cstdint>
#include <stdexcept>

#include "Calc.hpp"

namespace WxCalc {

namespace {

const int OP_OFFSET = 1024;

enum event_ids {
    // important these map to the numbers
    ID_0_BUTTON = 0,
    ID_1_BUTTON = 1,
    ID_2_BUTTON = 2,
    ID_3_BUTTON = 3,
    ID_4_BUTTON = 4,
    ID_5_BUTTON = 5,
    ID_6_BUTTON = 6,
    ID_7_BUTTON = 7,
    ID_8_BUTTON = 8,
    ID_9_BUTTON = 9,
    // let these be auto assigned
    ID_DECI_BUTTON,
    ID_SIGN_BUTTON,
    ID_ADD_BUTTON = OP_OFFSET + Calc::OP_ADD,
    ID_SUB_BUTTON = OP_OFFSET + Calc::OP_SUB,
    ID_MUL_BUTTON = OP_OFFSET + Calc::OP_MUL,
    ID_DIV_BUTTON = OP_OFFSET + Calc::OP_DIV,
    ID_CLS_BUTTON,
    ID_EQUAL_BUTTON
};

} // namespace

/******************************************************************************
 * PRIVATE FUNCTIONS ***********************************************************
 ******************************************************************************/

double WxCalcWindow::getCurrentValue()
{
    try {
        return std::stod(std::string(m_display->GetValue().mb_str()));
    } catch (const std::invalid_argument &e) {
        // assume unparsable equals 0
        return 0.0;
    }
}

void WxCalcWindow::updateDisplay()
{
    // this is not an efficient way to construct a number, but the user won't
    // notice a few ms here.
    std::string numStr("");
    if (!m_positive) {
        numStr += "-";
    }

    numStr += m_preDecimal;
    if (m_decimal) {
        numStr += "." + m_postDecimal;
    }

    m_display->SetValue(numStr);
}

void WxCalcWindow::clear()
{
    m_preDecimal.assign("");
    m_postDecimal.assign("");
    m_decimal = false;
    m_positive = true;
}

void WxCalcWindow::clearAndStore()
{
    m_total = getCurrentValue();

    // reset
    clear();

    updateDisplay();
}

void WxCalcWindow::performOp()
{
    if (m_op < 0) {
        // no op loaded -- do nothing
    } else {
        m_total = Calc::performOperation(m_total, m_last, m_op);

        showTotal();
    }
}

void WxCalcWindow::showTotal()
{
    clear();
    if (m_total == static_cast<int64_t>(m_total)) {
        m_display->SetValue(std::to_string(static_cast<int64_t>(m_total)));
    } else {
        m_display->SetValue(std::to_string(m_total));
    }
}

template <int NUM>
void WxCalcWindow::onNumButton(wxCommandEvent &)
{
    m_entryMode = true;

    // accumulate number
    if (!m_decimal) {
        m_preDecimal += std::to_string(NUM);
    } else {
        m_postDecimal += std::to_string(NUM);
    }

    updateDisplay();
}

template <int OP>
void WxCalcWindow::onOpButton(wxCommandEvent &)
{
    switch (OP) {
    case ID_DECI_BUTTON: {
        m_decimal = true;
        m_entryMode = true;
        updateDisplay();
        break;
    }
    case ID_SIGN_BUTTON: {
        m_positive = !m_positive;
        m_entryMode = true;
        updateDisplay();
        break;
    }
    case ID_ADD_BUTTON:
    case ID_SUB_BUTTON:
    case ID_MUL_BUTTON:
    case ID_DIV_BUTTON: {
        // perform any existing op (but not the current one)
        if (m_entryMode) {
            m_last = getCurrentValue();
            m_entryMode = false;
        }
        performOp();
        clearAndStore();
        showTotal();
        m_op = OP - OP_OFFSET;
        break;
    }
    case ID_CLS_BUTTON: {
        // clear everything
        clear();
        m_entryMode = true;
        m_total = 0;
        m_last = 0;
        m_op = -1;
        updateDisplay();
        break;
    }
    case ID_EQUAL_BUTTON: {
        if (m_entryMode) {
            m_last = getCurrentValue();
            m_entryMode = false;
        }
        performOp();
        break;
    }
    }
}

/******************************************************************************
 * EVENT TABLE *****************************************************************
 ******************************************************************************/

#define NUM_EVENT(id) EVT_BUTTON(id, WxCalcWindow::onNumButton<id>)

#define OP_EVENT(id) EVT_BUTTON(id, WxCalcWindow::onOpButton<id>)

// declare the window's event table
wxBEGIN_EVENT_TABLE(WxCalcWindow, wxFrame)
    // the numbers
    NUM_EVENT(ID_0_BUTTON)
    NUM_EVENT(ID_1_BUTTON)
    NUM_EVENT(ID_2_BUTTON)
    NUM_EVENT(ID_3_BUTTON)
    NUM_EVENT(ID_4_BUTTON)
    NUM_EVENT(ID_5_BUTTON)
    NUM_EVENT(ID_6_BUTTON)
    NUM_EVENT(ID_7_BUTTON)
    NUM_EVENT(ID_8_BUTTON)
    NUM_EVENT(ID_9_BUTTON)
    OP_EVENT(ID_DECI_BUTTON)
    OP_EVENT(ID_SIGN_BUTTON)
    OP_EVENT(ID_ADD_BUTTON)
    OP_EVENT(ID_SUB_BUTTON)
    OP_EVENT(ID_MUL_BUTTON)
    OP_EVENT(ID_DIV_BUTTON)
    OP_EVENT(ID_CLS_BUTTON)
    OP_EVENT(ID_EQUAL_BUTTON)
wxEND_EVENT_TABLE()

/******************************************************************************
 * PUBLIC FUNCTIONS ************************************************************
 ******************************************************************************/

WxCalcWindow::WxCalcWindow()
: wxFrame(NULL, wxID_ANY, "Wx-Calc", wxPoint(100, 100), wxDefaultSize, wxCLOSE_BOX | wxCAPTION),
  m_display(nullptr), m_numButtons(), m_deciButton(nullptr), m_signButton(nullptr),
  m_addButton(nullptr), m_subButton(nullptr), m_mulButton(nullptr), m_divButton(nullptr),
  m_clsButton(nullptr), m_equalsButton(nullptr), m_preDecimal(), m_postDecimal(),
  m_decimal(false), m_positive(true), m_total(0), m_last(0), m_op(-1), m_entryMode(true)
{
    wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

    m_display = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT);
    // There is a wxTE_READONLY style option, but it still allow selecting
    m_display->Disable();
    // have the text span our window horizontally
    wxBoxSizer *displaySizer = new wxBoxSizer(wxHORIZONTAL);
    displaySizer->Add(m_display, 1, wxEXPAND | wxALL, 0);

    // we don't want this to expand vertically
    topSizer->Add(displaySizer, 0, wxEXPAND, 0);

    // put a 10px spacer between the display and the buttons
    topSizer->AddSpacer(10);

    // create the button grid with no space between buttons
    wxGridSizer *buttonSizer = new wxGridSizer(4, 0, 0);

    wxSize buttonSize(64, 64);

    // create all number buttons
    m_numButtons.resize(10);
    for (int num = 0; num < 10; ++num) {
        m_numButtons[num] =
            new wxButton(this, num, std::to_string(num), wxDefaultPosition, buttonSize);
    }

    // create operation buttons
    m_addButton = new wxButton(this, ID_ADD_BUTTON, "+", wxDefaultPosition, buttonSize);
    m_subButton = new wxButton(this, ID_SUB_BUTTON, "-", wxDefaultPosition, buttonSize);
    m_mulButton = new wxButton(this, ID_MUL_BUTTON, "*", wxDefaultPosition, buttonSize);
    m_divButton = new wxButton(this, ID_DIV_BUTTON, "/", wxDefaultPosition, buttonSize);
    m_clsButton = new wxButton(this, ID_CLS_BUTTON, "CLS", wxDefaultPosition, buttonSize);
    m_equalsButton = new wxButton(this, ID_EQUAL_BUTTON, "=", wxDefaultPosition, buttonSize);

    // create decimal and plus/minus buttons
    m_deciButton = new wxButton(this, ID_DECI_BUTTON, ".", wxDefaultPosition, buttonSize);
    m_signButton = new wxButton(this, ID_SIGN_BUTTON, "+/-", wxDefaultPosition, buttonSize);

    // the order in which we add determines the place in the grid, so add in
    // row-major order.

    // first row
    for (int num = 1; num <= 3; ++num) {
        buttonSizer->Add(m_numButtons[num], 1, wxEXPAND, 0);
    }
    buttonSizer->Add(m_addButton, 1, wxEXPAND, 0);

    // second row
    for (int num = 4; num <= 6; ++num) {
        buttonSizer->Add(m_numButtons[num], 1, wxEXPAND, 0);
    }
    buttonSizer->Add(m_subButton, 1, wxEXPAND, 0);

    // third row
    for (int num = 7; num <= 9; ++num) {
        buttonSizer->Add(m_numButtons[num], 1, wxEXPAND, 0);
    }
    buttonSizer->Add(m_mulButton, 1, wxEXPAND, 0);

    // fourth row
    buttonSizer->Add(m_numButtons[0], 1, wxEXPAND, 0);

    buttonSizer->Add(m_deciButton, 1, wxEXPAND, 0);
    buttonSizer->Add(m_signButton, 1, wxEXPAND, 0);

    buttonSizer->Add(m_divButton, 1, wxEXPAND, 0);

    // special cls and equals buttons in the last row and last two columns, add
    // two spacers to move them over
    buttonSizer->AddStretchSpacer();
    buttonSizer->AddStretchSpacer();
    buttonSizer->Add(m_clsButton, 1, wxEXPAND, 0);
    buttonSizer->Add(m_equalsButton, 1, wxEXPAND, 0);

    topSizer->Add(buttonSizer, 1, wxEXPAND, 0);

    SetSizerAndFit(topSizer);
}

} // namespace WxCalc
