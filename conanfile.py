from conans import ConanFile, CMake

class Csvparserrfc4180Conan(ConanFile):
    name = "csv_parser_RFC4180"
    version = "0.1"
    license = "Public"
    url = "https://github.com/jinq0123/csv_parser_RFC4180"
    description = "Csv parser based on RFC 4180"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "csv_parser/csv_parser.*", "CMakeLists.txt"

    def build(self):
        cmake = CMake(self)
        self.run('cmake %s %s' % (self.source_folder, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["csv_parser_RFC4180"]
