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
            Text("TODO display")
                .padding(10)
            HStack {
                Button("CE") {
                    //proxy.send("e")
                }
                Button("÷") {
                    //proxy.send("/")
                }
                Button("×") {
                    //proxy.send("*")
                }
                VStack {
                    Text("chain")
                    //TODO: Image(systemName: "lightswitch.off.fill")
                    Text("const")
                }
            }
            HStack {
                Button("7") {
                    //proxy.send("7")
                }
                Button("8") {
                    //proxy.send("8")
                }
                Button("9") {
                    //proxy.send("9")
                }
                Button("C") {
                    //proxy.send("c")
                }
            }
            HStack {
                Button("4") {
                    //proxy.send("4")
                }
                Button("5") {
                    //proxy.send("5")
                }
                Button("6") {
                    //proxy.send("6")
                }
                Button("-") {
                    //proxy.send("-")
                }
            }
            HStack {
                Button("1") {
                    //proxy.send("1")
                }
                Button("2") {
                    //proxy.send("2")
                }
                Button("3") {
                    //proxy.send("3")
                }
                Button("+") {
                    //proxy.send("+")
                }
            }
            HStack {
                Button("0") {
                    //proxy.send("0")
                }
                Button("·") {
                    //proxy.send(".")
                }
                Button("=") {
                    //proxy.send("=")
                }
            }
        }
        .padding(20)
        .topToolbar {
            ToolbarView(app: app, window: window)
        }
    }
}
