//
//  ContentView.swift
//
import SwiftUI

public struct ContentView: View {
    @ObservedObject var proxy: ModelProxy = ModelProxy.shared

    public var body: some View {
        HStack {
            Button("-") {
                //TODO: proxy.decrement()
            }
            Text("\(proxy.display)")
            Button("+") {
                //TODO: proxy.increment()
            }
        }
        .padding()
    }

    public init() {
        proxy.setup()
    }
}

#Preview {
    ContentView()
}
