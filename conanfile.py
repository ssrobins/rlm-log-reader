from conans import ConanFile

class Conan(ConanFile):
    name = "RLMLogReader"
    version = ""
    settings = "os"
    generators = "cmake", "cmake_find_package", "cmake_paths"

    def requirements(self):
        self.requires("date/3.0.1")
        self.requires("gtest/1.11.0")
        self.requires("qt/5.15.2")
        self.requires("expat/2.4.2")
        self.requires("openssl/1.1.1n")
