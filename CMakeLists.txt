cmake_minimum_required(VERSION 3.6)

set(APP_NAME FifteenPuzzle)

project(${APP_NAME})

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )

cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})

list (APPEND GAME_LOGIC
	Classes/Board.h
	Classes/Board.cpp

    Classes/Cell.h
    Classes/Cell.cpp
)

set(GUI_SOURCE)

list(APPEND GUI_SOURCE
    Classes/AppDelegate.h
    Classes/AppDelegate.cpp

    Classes/BlockNode.h
    Classes/BlockNode.cpp

    Classes/ClockNode.h
    Classes/ClockNode.cpp

    Classes/EndgameBannerNode.h
    Classes/EndgameBannerNode.cpp

    Classes/FifteenPuzzleBoardScene.h
    Classes/FifteenPuzzleBoardScene.cpp
	 
    Classes/GridNode.h
    Classes/GridNode.cpp

    Classes/UIConstants.h
)

# Windows specific.
    list(APPEND GUI_SOURCE
         proj.win32/main.h
         proj.win32/resource.h


         proj.win32/main.cpp
         proj.win32/game.rc
         ${common_res_files}
         )

# mark app complie info and libs info
set(all_code_files
    ${GUI_SOURCE}
    ${GAME_LOGIC}
    )

    add_executable(${APP_NAME} ${all_code_files})

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

# mark app resources
setup_cocos_app_config(${APP_NAME})
cocos_copy_target_dll(${APP_NAME})

set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})


# Testing files and libs.
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

list(APPEND TEST_SOURCE
    ${GAME_LOGIC}

	Tests/BoardTests.cpp
    Tests/CellTests.cpp
    Tests/unit_tests.cpp
 )

add_executable(unit_tests ${TEST_SOURCE})
target_include_directories(unit_tests
        PRIVATE Classes
)
target_link_libraries(unit_tests gtest_main)
add_test(NAME unit_tests COMMAND example)