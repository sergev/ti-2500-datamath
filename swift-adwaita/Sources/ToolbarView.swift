import Adwaita

struct ToolbarView: View {

    @State private var about = false
    var app: GTUIApp
    var window: GTUIApplicationWindow

    var view: Body {
        HeaderBar.end {
            Menu(icon: .default(icon: .openMenu), app: app, window: window) {
                MenuButton(Loc.newWindow, window: false) {
                    app.addWindow("main")
                }
                .keyboardShortcut("n".ctrl())
                MenuButton(Loc.closeWindow) {
                    window.close()
                }
                .keyboardShortcut("w".ctrl())
                MenuSection {
                    MenuButton(Loc.about, window: false) {
                        about = true
                    }
                }
            }
            .primary()
            .tooltip(Loc.mainMenu)
            .aboutDialog(
                visible: $about,
                app: "TI-2500 Datamath",
                developer: "david-swift",
                version: "dev",
                icon: .custom(name: "org.sergev.ti2500"),
                website: .init(string: "https://github.com/sergev/ti-2500-datamath")!,
                issues: .init(string: "https://github.com/sergev/ti-2500-datamath/issues")!
            )
        }
    }

}
