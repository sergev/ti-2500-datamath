// swift-tools-version: 5.8
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "TI-2500 Datamath",
    platforms: [
        .macOS(.v13)
    ],
    dependencies: [
        .package(url: "https://github.com/AparokshaUI/Adwaita", from: "0.2.0"),
        .package(url: "https://github.com/AparokshaUI/Localized", from: "0.2.0")
    ],
    targets: [
        .executableTarget(
            name: "ti2500",
            dependencies: [
                .product(name: "Adwaita", package: "Adwaita"),
                .product(name: "Localized", package: "Localized")
            ],
            path: "Sources",
            resources: [
                .process("Localized.yml")
            ],
            plugins: [
                .plugin(name: "GenerateLocalized", package: "Localized")
            ]
        )
    ]
)
