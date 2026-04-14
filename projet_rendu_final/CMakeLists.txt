cmake_minimum_required(VERSION 3.16)

project(akropolis_app VERSION 0.1 LANGUAGES CXX)

include_directories(
    abstract
    jeu
    plateau
    exceptions
    .
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# --- LISTE DE TOUS LES FICHIERS DU PROJET ---
set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h

        vuetuile.cpp
        vuetuile.h
        vuechantier.cpp
        vuechantier.h
        vuehexagone.cpp
        vuehexagone.h
        vueplateau.cpp
        vueplateau.h
        vuepartie.cpp
        vuepartie.h
        vuemenue.cpp
        vuemenue.h
        vuescore.cpp
        vuescore.h

        jeu/Partie.cpp
        jeu/Partie.h
        jeu/Joueur.cpp
        jeu/Joueur.h
        jeu/Score.cpp
        jeu/Score.h
        jeu/IllustreConstructeur.cpp
        jeu/IA.cpp
        jeu/IllustreConstructeur.h
        jeu/IA.h
        jeu/Memento.h
        jeu/Caretaker.h

        plateau/Chantier.cpp
        plateau/Chantier.h
        plateau/Cite.cpp
        plateau/Cite.h
        plateau/Hexagone.cpp
        plateau/Hexagone.h
        plateau/Tuile.cpp
        plateau/Tuile.h
        plateau/Coordonnee.cpp
        plateau/Coordonnee.h
        plateau/Jardin.cpp
        plateau/Jardin.h
        plateau/Caserne.cpp
        plateau/Caserne.h
        plateau/Temple.cpp
        plateau/Temple.h
        plateau/Marche.cpp
        plateau/Marche.h
        plateau/Habitation.cpp
        plateau/Habitation.h
        plateau/Place.h
        plateau/Place.cpp
        plateau/Carriere.h

        abstract/Construction.h
        abstract/Quartier.h
        abstract/Quartier.cpp

        exceptions/JoueurException.h
        exceptions/JoueurException.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(akropolis_app
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        vueconstruction.h vueconstruction.cpp
    )
else()
    if(ANDROID)
        add_library(akropolis_app SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(akropolis_app
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(akropolis_app PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.akropolis_app)
endif()
set_target_properties(akropolis_app PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS akropolis_app
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(akropolis_app)
endif()
