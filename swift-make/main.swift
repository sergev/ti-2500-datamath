//
// Main routine of the application.
//
// Copyright (c) 2023 Serge Vakulenko <serge.vakulenko@gmail.com>
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//
import SwiftUI
import Gui

@main
struct MainApp: App {

    var body: some Scene {
        WindowGroup {
            Content()
                .navigationTitle("TI-2500 Datamath")
                .onDisappear {
                    exit(0)
                }
        }
        .windowResizability(.contentSize)
    }
}
