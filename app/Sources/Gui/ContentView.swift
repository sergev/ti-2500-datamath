//
//  ContentView.swift
//
import SwiftUI

public struct ContentView: View {
    @ObservedObject var proxy: ModelProxy = ModelProxy.shared

    // Sizes
    let u = 10.0

    // Colors
    let red = Color(red: 1, green: 0.5, blue: 0, opacity: 1)
    let orange = Color(red: 1, green: 0.8, blue: 0.5, opacity: 1)
    let darkGray = Color(red: 0.1, green: 0.1, blue: 0.1, opacity: 1)
    let lightGray = Color(red: 0.5, green: 0.5, blue: 0.5, opacity: 1)

    public var body: some View {
        VStack(spacing: u) {
            Text("\(proxy.display)")
                .lineLimit(1)
                .font(.system(size: 2.8*u, design: .monospaced))
                .padding(u)
                .foregroundColor(red)
                .frame(width: 20*u, alignment: .trailing)
            HStack(spacing: u) {
                Button("CE") {
                    proxy.send("e")
                }
                Button("÷") {
                    proxy.send("/")
                }
                Button("×") {
                    proxy.send("*")
                }
                VStack {
                    Text("chain")
                    Image(systemName: "lightswitch.off.fill")
                    Text("const")
                }
                .font(.system(size: u))
                .foregroundColor(lightGray)
            }
            HStack(spacing: u) {
                Button("7") {
                    proxy.send("7")
                }
                Button("8") {
                    proxy.send("8")
                }
                Button("9") {
                    proxy.send("9")
                }
                Button("C") {
                    proxy.send("c")
                }
            }
            HStack(spacing: u) {
                Button("4") {
                    proxy.send("4")
                }
                Button("5") {
                    proxy.send("5")
                }
                Button("6") {
                    proxy.send("6")
                }
                Button("-") {
                    proxy.send("-")
                }
            }
            HStack(spacing: u) {
                Button("1") {
                    proxy.send("1")
                }
                Button("2") {
                    proxy.send("2")
                }
                Button("3") {
                    proxy.send("3")
                }
                Button("+") {
                    proxy.send("+")
                }
            }
            HStack(spacing: u) {
                Button("0") {
                    proxy.send("0")
                }
                Button(".") {
                    proxy.send(".")
                }
                Button("=") {
                    proxy.send("=")
                }
            }
        }
        .font(.system(size: 2*u))
        .padding(2*u)
        .background(darkGray)
        .border(lightGray, width: u)
        .cornerRadius(u)
    }

    public init() {
        proxy.setup()
    }
}

#Preview {
    ContentView()
}
