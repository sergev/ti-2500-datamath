//
// Proxy object which connects Swift to the C++ code.
//
// Copyright (c) 2023 Serge Vakulenko <serge.vakulenko@gmail.com>
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//
import Foundation
import ModelCxx

class ModelProxy: ObservableObject {
    @Published var display: String = ""

    static let shared = ModelProxy()

    // Allocate C++ object.
    private var calc = Calculator()

    // Show initial display contents.
    func setup() {
        calc.run()
        display = String(calc.get_display())
    }

    // Press key.
    func send(_ ch: Character) {
        calc.press_key(CChar(ch.asciiValue!))
        display = String(calc.get_display())
    }
}
