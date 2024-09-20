// swift-tools-version: 6.0
// The swift-tools-version declares the minimum version of Swift required to build this package.
//
// Copyright (c) 2023 Serge Vakulenko <serge.vakulenko@gmail.com>
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.
//
import PackageDescription

let package = Package(
    name: "Calculator",
    platforms: [
        .macOS(.v14),
    ],
    targets: [
        .target(
            name: "Gui",
            dependencies: ["ModelCxx"],
            swiftSettings: [
                .interoperabilityMode(.Cxx),
                .unsafeFlags([
                    "-I", "../src",
            ])]
        ),
        .target(
            name: "ModelCxx",
            publicHeadersPath: ".",
            cxxSettings: [.unsafeFlags([
                "-I", "../src",
            ])]
        ),
        .executableTarget(
            name: "Application",
            dependencies: ["ModelCxx", "Gui"],
            swiftSettings: [.interoperabilityMode(.Cxx)]
        ),
    ],
    cxxLanguageStandard: .cxx20
)
