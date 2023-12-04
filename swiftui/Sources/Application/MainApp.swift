//
//  MainApp.swift
//
import SwiftUI
import Gui

@main
struct MainApp: App {

    var body: some Scene {
        WindowGroup {
            ContentView()
                .navigationTitle("TI-2500 Datamath")
                .onDisappear {
                    terminateApp()
                }
        }
        .windowResizability(.contentSize)
    }

    private func terminateApp() {
        NSApplication.shared.terminate(self)
    }
}
