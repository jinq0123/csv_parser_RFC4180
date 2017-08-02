from conans import ConanFile, VisualStudioBuildEnvironment, tools

class Csvparserrfc4180jinq0123Conan(ConanFile):
    name = "csv_parser_RFC4180"
    version = "0.1"
    license = "Public"
    url = "https://github.com/jinq0123/csv_parser_RFC4180"
    description = "Csv parser based on RFC 4180"
    settings = "os", "compiler", "build_type", "arch"
    generators = "Premake"  # A custom generator: PremakeGen/0.1@memsharded/testing
    exports_sources = "csv_parser/csv_parser.*", "premake5.lua"

    build_requires = "PremakeGen/0.1@memsharded/testing"
     
    def build(self):
        if self.settings.compiler == "Visual Studio":
            self.run("premake5.exe --os=windows vs2015")
            env_build = VisualStudioBuildEnvironment(self)
            with tools.environment_append(env_build.vars):
                cmd = tools.msvc_build_command(self.settings,
                    "csv_parser_RFC4180.sln",
                    targets=["csv_parser_RFC4180"],
                    upgrade_project = False)
                if self.settings.arch == "x86":
                    cmd = cmd.replace('p:Platform="x86"', 'p:Platform="Win32"')
                self.run(cmd)
        # End of if.

    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["csv_parser_RFC4180"]
