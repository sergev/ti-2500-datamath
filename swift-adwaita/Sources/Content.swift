//
// Main window of the calculator.
//
// Copyright (c) 2024 Serge Vakulenko <serge.vakulenko@gmail.com>
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//
import Adwaita

//
// Main window
//
struct Content: View {

    var app: GTUIApp
    var window: GTUIApplicationWindow

    var view: Body {
        VStack {
            Text("12345678.9")
                .title1()
                .padding(20)
            HStack {
                Button("CE") {
                    //proxy.send("e")
                }.pill()
                Button("÷") {
                    //proxy.send("/")
                }.pill()
                Button("×") {
                    //proxy.send("*")
                }.pill()
                VStack {
                    Text("chain")
                    //TODO: Image(systemName: "lightswitch.off.fill")
                    Text("const")
                }
            }
            HStack {
                Button("7") {
                    //proxy.send("7")
                }.pill()
                Button("8") {
                    //proxy.send("8")
                }.pill()
                Button("9") {
                    //proxy.send("9")
                }.pill()
                Button("C") {
                    //proxy.send("c")
                }.pill()
            }
            HStack {
                Button("4") {
                    //proxy.send("4")
                }.pill()
                Button("5") {
                    //proxy.send("5")
                }.pill()
                Button("6") {
                    //proxy.send("6")
                }.pill()
                Button("-") {
                    //proxy.send("-")
                }.pill()
            }
            HStack {
                Button("1") {
                    //proxy.send("1")
                }.pill()
                Button("2") {
                    //proxy.send("2")
                }.pill()
                Button("3") {
                    //proxy.send("3")
                }.pill()
                Button("+") {
                    //proxy.send("+")
                }.pill()
            }
            HStack {
                Button("0") {
                    //proxy.send("0")
                }.pill()
                Button("·") {
                    //proxy.send(".")
                }.pill()
                Button("=") {
                    //proxy.send("=")
                }.pill()
            }
        }
        .padding(20)
        .topToolbar {
            ToolbarView(app: app, window: window)
        }
    }
}
