
from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class poloniumRecipe(ConanFile):
    name = "polonium"
    version = "0.7.0"
    package_type = "library"

    # Optional metadata
    license = "MIT"
    author = "Les-S-niK"
    url = "https://github.com/Les-S-niK/Polonium"
    description = "Polonium is a HTTP API framework. Not for production."
    topics = ("cpp", "c++", "http api framework")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def requirements(self):
        self.requires("llhttp/9.3.0", transitive_headers=True)
        self.requires("ctre/3.10.0", transitive_headers=True)
        self.requires("nlohmann_json/3.12.0", transitive_headers=True)

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)
    
    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["polonium"]
        self.cpp_info.set_property("cmake_target_name", "polonium::polonium")
