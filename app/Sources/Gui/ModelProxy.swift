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

    func setup() {
        calc.run()
        display = String(calc.get_display())
    }

    func increment() {
        //TODO: calc.increment()
    }

    func decrement() {
        //TODO: calc.decrement()
    }
}
