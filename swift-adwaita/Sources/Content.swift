import Adwaita

struct Content: View {

    var app: GTUIApp
    var window: GTUIApplicationWindow

    var view: Body {
        Text("hello world")
            .topToolbar {
                ToolbarView(app: app, window: window)
            }
    }
}
