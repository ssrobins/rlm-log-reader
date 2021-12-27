from conans import ConanFile

class Conan(ConanFile):
    name = "RLMLogReader"
    version = ""
    settings = "os"
    generators = "cmake"

    def requirements(self):
        self.requires("boost/1.78.0")
        self.requires("gtest/1.11.0")
        #self.requires("qt/5.15.2")
        self.requires("qt/6.2.2")
