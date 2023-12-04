//
//  ModelProxy.swift
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
