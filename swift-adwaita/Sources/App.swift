// The Swift Programming Language
// https://docs.swift.org/swift-book

import Adwaita

@main
struct CalculatorApp: App {

    let id = "org.sergev.ti2500"
    var app: GTUIApp!

    var scene: Scene {
        Window(id: "main") { window in
            Content(app: app, window: window)
        }
        .defaultSize(width: 450, height: 300)
        .title("TI-2500 Datamath")
        .quitShortcut()
        .closeShortcut()
    }
}
