//
//  ContentView.swift
//
import SwiftUI

// Sizes
let u = 10.0

// Colors
let darkGray = Color(red: 0.16, green: 0.16, blue: 0.16, opacity: 1)
let red = Color(red: 0.88, green: 0.3, blue: 0.35, opacity: 1)
let gray = Color(red: 0.69, green: 0.64, blue: 0.57, opacity: 1)
let cream = Color(red: 0.8, green: 0.8, blue: 0.8, opacity: 1)
let orange = Color(red: 0.78, green: 0.49, blue: 0.26, opacity: 1)

struct GrayButton: ButtonStyle {
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .frame(width: 3*u, height: 3*u)
            .background(RoundedRectangle(cornerRadius: 5.0).fill(gray)
        )
    }
}

struct CreamButton: ButtonStyle {
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .frame(width: 3*u, height: 3*u)
            .background(RoundedRectangle(cornerRadius: 5.0).fill(cream)
        )
    }
}

struct WideCreamButton: ButtonStyle {
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .frame(width: 5*u, height: 3*u)
            .background(RoundedRectangle(cornerRadius: 5.0).fill(cream)
        )
    }
}

struct WideOrangeButton: ButtonStyle {
    func makeBody(configuration: Configuration) -> some View {
        configuration.label
            .frame(width: 5*u, height: 3*u)
            .background(RoundedRectangle(cornerRadius: 5.0).fill(orange)
        )
    }
}

public struct ContentView: View {
    @ObservedObject var proxy: ModelProxy = ModelProxy.shared

    public var body: some View {

        VStack(spacing: u) {
            Text("\(proxy.display)")
                .lineLimit(1)
                .font(.system(size: 2.5*u, design: .monospaced))
                .padding(u)
                .foregroundColor(red)
                .frame(width: 18*u, alignment: .trailing)
            HStack(spacing: u) {
                Button("CE") {
                    proxy.send("e")
                }.buttonStyle(GrayButton())
                Button("÷") {
                    proxy.send("/")
                }.buttonStyle(GrayButton())
                Button("×") {
                    proxy.send("*")
                }.buttonStyle(GrayButton())
                VStack {
                    Text("chain")
                    Image(systemName: "lightswitch.off.fill")
                    Text("const")
                }
                .font(.system(size: u))
                .foregroundColor(darkGray)
                .shadow(color: gray, radius: 1)
            }
            HStack(spacing: u) {
                Button("7") {
                    proxy.send("7")
                }.buttonStyle(CreamButton())
                Button("8") {
                    proxy.send("8")
                }.buttonStyle(CreamButton())
                Button("9") {
                    proxy.send("9")
                }.buttonStyle(CreamButton())
                Button("C") {
                    proxy.send("c")
                }.buttonStyle(GrayButton())
            }
            HStack(spacing: u) {
                Button("4") {
                    proxy.send("4")
                }.buttonStyle(CreamButton())
                Button("5") {
                    proxy.send("5")
                }.buttonStyle(CreamButton())
                Button("6") {
                    proxy.send("6")
                }.buttonStyle(CreamButton())
                Button("-") {
                    proxy.send("-")
                }.buttonStyle(GrayButton())
            }
            HStack(spacing: u) {
                Button("1") {
                    proxy.send("1")
                }.buttonStyle(CreamButton())
                Button("2") {
                    proxy.send("2")
                }.buttonStyle(CreamButton())
                Button("3") {
                    proxy.send("3")
                }.buttonStyle(CreamButton())
                Button("+") {
                    proxy.send("+")
                }.buttonStyle(GrayButton())
            }
            HStack(spacing: u) {
                Button("0") {
                    proxy.send("0")
                }.buttonStyle(WideCreamButton())
                Button(".") {
                    proxy.send(".")
                }.buttonStyle(CreamButton())
                Button("=") {
                    proxy.send("=")
                }.buttonStyle(WideOrangeButton())
            }
            Spacer()
                .frame(height: 0.5*u)
        }
        .font(.system(size: 2*u, design: .monospaced))
        .padding(2*u)
        .background(darkGray)
        .border(gray, width: u)
        .cornerRadius(u)
    }

    public init() {
        proxy.setup()
    }
}

#Preview {
    ContentView()
}
