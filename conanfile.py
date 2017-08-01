from conans import ConanFile

class Csvparserrfc4180jinq0123Conan(ConanFile):
    name = "csv_parser_RFC4180_jinq0123"
    version = "0.1"
    license = "Public"
    url = "https://github.com/jinq0123/csv_parser_RFC4180"
    description = "Csv parser based on RFC 4180"
    settings = "os", "compiler", "build_type", "arch"
    generators = "Premake"  # A custom generator: PremakeGen/0.1@memsharded/testing
    exports_sources = "csv_parser/csv_parser.*"

    def requirements(self):
        self.requires("PremakeGen/0.1@memsharded/testing")
     
    def build(self):
        # cmake = CMake(self)
        # self.run('cmake %s/src %s' % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)
        self.run("echo XXXXXXXXXXXXXXXXXXXXXXXXXXX")

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["csv_parser_RFC4180_jinq0123"]
