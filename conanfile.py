from conans import ConanFile

class Conan(ConanFile):
    name = "RLMLogReader"
    version = ""
    settings = "os"
    generators = "cmake"

    def requirements(self):
        self.requires("boost/1.78.0")
        self.requires("gtest/1.11.0")
