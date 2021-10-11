cmake_minimum_required(VERSION 3.9) project(dno LANGUAGES CXX)
find_package(Boost)

    add_library(dno INTERFACE)

        target_include_directories(
            dno INTERFACE
                $<BUILD_INTERFACE : $ {CMAKE_CURRENT_SOURCE_DIR} / include>
                    $<BUILD_INTERFACE : $ {CMAKE_CURRENT_SOURCE_DIR}>
                        $<INSTALL_INTERFACE : include>)

            target_compile_features(dno INTERFACE cxx_std_20)

                include(GenerateExportHeader) generate_export_header(dno)
