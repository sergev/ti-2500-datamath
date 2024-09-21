// The Swift Programming Language
// https://docs.swift.org/swift-book

import Adwaita

@main
struct AdwaitaTemplate: App {

    let id = "org.sergev.ti2500"
    var app: GTUIApp!

    var scene: Scene {
        Window(id: "main") { window in
            Text(Loc.helloWorld)
                .padding()
                .topToolbar {
                    ToolbarView(app: app, window: window)
                }
        }
        .defaultSize(width: 450, height: 300)
    }

}
