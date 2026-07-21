cmake_minimum_required(VERSION 3.16)

# 先定义项目信息（规范写法）
project(TikoLib VERSION 0.1 LANGUAGES CXX)

# 设置 C++ 标准和 Qt 自动处理
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

# 查找 Qt 依赖
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.8)

# 定义库目标
add_library(TikoLib STATIC)

# 标记单例 QML 文件
set_source_files_properties(TikoSeit.qml
        PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

qt_policy(SET QTP0004 NEW)

# 添加 QML 模块
qt_add_qml_module(TikoLib
        URI Tiko                  # 模块 URI，主项目通过这个导入
        VERSION 1.0               # 版本号
        QML_FILES
        TikoSeit.qml
        TikoFrameless.qml
        bar/TikoBarV.qml

        button/TikoButtonBase.qml
        button/TikoButtonCombox.qml
        button/TikoButtonSwitch.qml
        button/TikoButtonIcon.qml
        text/TikoTextInput.qml
        text/TikoTextLine.qml
        menu/TikoMenu.qml
        menu/TikoMenuItem.qml
        menu/TikoMenuSpeacer.qml

        popup/TikoPopup.qml
        popup/TikoPopupInput.qml

        slider/TikoSliderH.qml
        slider/TikoSliderV.qml
        vessel/TikoRightVessel.qml
        select/TikoSelectColor.qml
        select/TikoSelectFont.qml
        image/TikoImageAuto.qml
        image/TikoImage.qml


        theme/TikoTheme.qml

        button/TikoButtonDefault.qml
)

# 链接 Qt 依赖
target_link_libraries(TikoLib PRIVATE Qt6::Quick)

# 安装模块
install(TARGETS TikoLib
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
